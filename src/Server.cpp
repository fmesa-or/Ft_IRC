#include "Server.hpp"
#include "Client.hpp"
#include "IRC.hpp"
#include "helpers.hpp"
#include "Command.hpp"
#include "CommandDispatcher.hpp"
#include "Parser.hpp"
#include "Replies.hpp"

#include <sys/socket.h>
#include <cstring>
#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <cerrno>

Server::Server(int port, const std::string& password) : _port(port), _password(password), _listen_fd(-1) {}

const std::string& Server::getPassword() const {
	return _password;
}

void Server::sendToClient(int fd, const std::string& message) {
	send(fd, message.c_str(), message.size(), 0);
}

/****************************************
* SEND TO CHANNEL: broadcast to members *
****************************************/
void	Server::sendToChannel(const Channel& channel, const std::string& message) {
	const std::set<Client*> &members = channel.getMembers();
	for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (*it != NULL) {
			sendToClient((*it)->getFd(), message);
		}
	}
}

 /******************************************************************************
 * Accepts and registers all pending client connections on @param listening_fd *
 *.                                                                            *
 * Loops until no more connections are pending (EAGAIN / EWOULDBLOCK).         *
 *                                                                             *
 * For each new connection:                                                    *
 *  - Retrieves the client IP address.                                         *
 *  - Sets TCP_NODELAY to disable Nagle's algorithm.                           *
 *  - Sets the socket to non-blocking mode with O_NONBLOCK.                    *
 *  - Registers the client fd in @param pollfds for POLLIN events.             *
 *  - Creates and stores a new Client object in @param _clients.               *
 *                                                                             *
 * If setting non-blocking mode fails, the client fd is closed and skipped.    *
 ******************************************************************************/
void Server::registerClients(int listening_fd, std::vector<pollfd> &pollfds) {
	while (true) {
		sockaddr_in client_address;
		socklen_t client_address_len = sizeof(client_address);
		
		int client_fd = accept(listening_fd, (sockaddr *)&client_address, &client_address_len);
		if (client_fd == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			} else {
				ERROR("accept failed.");
				break;
			}
		} else {
			// Register client.
			std::string ip_string = getIpStringFromNetwork(client_address);

			int option_value = 1;
			if (setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &option_value, sizeof(option_value)) == -1) {
				ERROR("Couldn't set TCP_NODELAY on client " << ip_string);
			}

			int flags = fcntl(client_fd, F_GETFL);
			if (flags == -1) {
				close(client_fd);
				continue;
			}
			if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
				close(client_fd);
				continue;
			}

			struct pollfd client_pollfd;
			memset(&client_pollfd, 0, sizeof(client_pollfd));
			client_pollfd.fd = client_fd;
			client_pollfd.events = POLLIN;

			pollfds.push_back(client_pollfd);
			_clients[client_fd] = Client(client_fd);

			LOG_DEBUG("Client connected from " << ip_string);
		}
	}
}

void Server::completeClientRegistration(Client &client) {
	if (!client.isRegistered())
		return;
	if (client.getRegistrationCompleted())
		return;
	client.setRegistrationCompleted(true);
	sendToClient(client.getFd(), Replies::welcome(client));
}

bool Server::disconnectClient(int fd, size_t pollfds_idx) {
	_pollfds.erase(_pollfds.begin() + pollfds_idx);
	_clients.erase(fd);
	close(fd);
	LOG_DEBUG("client (fd) " << fd << " disconnected.");
	return true;
}

int Server::createConfigureAndSetUpListeningSocket() {
	int listening_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listening_fd < 0) {
		ERROR("Couldn't create socket.");
	}

	int option_value = 1;
	if (setsockopt(listening_fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value)) == -1) {
		ERROR("Couldn't set server socket reuse address option.");
		close_socket_and_exit_with_failure(listening_fd);
	}

	// Set Non-Blocking socket. Requires later upkeep.
	int fd_flags = fcntl(listening_fd, F_GETFL);
	if (fd_flags == -1) {
		ERROR("Couldn't get server socket flags.");
		close_socket_and_exit_with_failure(listening_fd);
	}

	if (fcntl(listening_fd, F_SETFL, fd_flags | O_NONBLOCK) == -1) {
		ERROR("Couldn't set non-blocking option on server socket.");
		close_socket_and_exit_with_failure(listening_fd);
	}

	sockaddr_in server_address;
	std::memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(_port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(listening_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
		ERROR("Couldn't bind address to server socket.");
		close_socket_and_exit_with_failure(listening_fd);
	}

	if (listen(listening_fd, SOMAXCONN) == -1) {
		ERROR("Couldn't set server socket as listening socket.");
		close_socket_and_exit_with_failure(listening_fd);
	}

	return listening_fd;
}

void Server::processClientBuffer(Client &client, char buffer[BUFFER_SIZE], ssize_t bytes_received, int pollfds_idx) {
	std::string &recv_buffer = client.getRecvBuffer();
	recv_buffer.append(buffer, bytes_received);

	if (recv_buffer.size() > MAX_RECV_BUFFER_SIZE) {
		disconnectClient(client.getFd(), pollfds_idx);
		return;
	}

	int fd = client.getFd(); // Saves fd before process

	size_t line_end_position = 0;
	while ((line_end_position = recv_buffer.find("\r\n")) != std::string::npos) {
		line_end_position += 2;
		std::string message = recv_buffer.substr(0, line_end_position);
		recv_buffer.erase(0, line_end_position);

		LOG_DEBUG("Message: " << message);

		Parser parser;
		Command command = parser.parseLine(message);
		_dispatcher.dispatch(*this, client, command);

		// Stops if client disconects meanwhile dispatch.
		if (_clients.find(fd) == _clients.end())
			return;
	}
}

void Server::continuouslyPollSockets(int listening_fd) {
	pollfd listening_pollfd;
	std::memset(&listening_pollfd, 0, sizeof(listening_pollfd));
	listening_pollfd.fd = listening_fd,
	listening_pollfd.events = POLLIN,

	_pollfds.push_back(listening_pollfd);

	while (true) {

		int fds_with_events = poll(&_pollfds[0], _pollfds.size(), -1); // AQUI CUELGA
		if (fds_with_events < 0) {
			if (errno == EINTR) {
				break;
			}

			ERROR("Poll failed unexpectedly");
			break;
		}

		size_t polled_fd_count = _pollfds.size();
		for (size_t idx = 0; idx < polled_fd_count; idx++) {

			int fd = _pollfds[idx].fd;
			short revents = _pollfds[idx].revents;

			bool disconnected = false;

			if (revents == 0) {
				continue;
			} else if ((revents & (POLLERR | POLLHUP)) != 0) {
				if (fd == listening_fd) {
					ERROR("Listening socket failed.");
					return;
				} else {
					Client* client = findClientByFd(fd);
					if (client) {
						const std::string quitMsg = ":" + client->getNickname() + "!" 
							+ client->getUsername() + "@localhost QUIT :Connection lost\r\n";
						removeClientFromAllChannels(*this, *client, quitMsg);
					}
					disconnected = disconnectClient(fd, idx);
				}
			} else if ((revents & POLLIN) != 0) {
				bool is_listening_fd = fd == listening_fd;
				if (is_listening_fd) {
					registerClients(listening_fd, _pollfds);
				} else {
					// Handle IRC message.
					char buffer[BUFFER_SIZE];
					ssize_t bytes_received = recv(fd, buffer, sizeof(buffer), 0);

					if (bytes_received > 0) {
						std::string text_received(buffer, bytes_received);
						LOG_DEBUG("client (fd) " << fd << ": " << text_received);
						Client& client = _clients[fd];
						processClientBuffer(client, buffer, bytes_received, idx);
					} else if (bytes_received == 0) {
						Client* client = findClientByFd(fd);
						if (client) {
							const std::string quitMsg = ":" + client->getNickname() + "!"
								+ client->getUsername() + "@localhost QUIT :Connection closed\r\n";
							removeClientFromAllChannels(*this, *client, quitMsg);
						}
						disconnected = disconnectClient(fd, idx);
					} else if (errno == EAGAIN || errno == EWOULDBLOCK) {
						continue;
					} else {
						ERROR("recv error from client " << fd << ".");
						Client* client = findClientByFd(fd);
						if (client) {
							const std::string quitMsg = ":" + client->getNickname() + "!"
								+ client->getUsername() + "@localhost QUIT :Read error\r\n";
							removeClientFromAllChannels(*this, *client, quitMsg);
						}
						disconnected = disconnectClient(fd, idx);
					}
				}
			}

			if (disconnected) {
				--idx;
				--polled_fd_count;
			}
		}
	}
}


Client* Server::findClientByFd(int fd) {
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end()) {
		return NULL;
	} else {
		return &it->second;
	}
}

Client* Server::findClientByNick(const std::string& nickname) {
	std::map<int, Client>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it) {
		if (it->second.getNickname() == nickname) {
			return &it->second;
		}
	}
	return NULL;
}

void Server::cleanup() {
	LOG_DEBUG("Closing server...");

	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		close(it->first);
	}

	if (_listen_fd != -1) {
		close(_listen_fd);
	}
	_listen_fd = -1;
}

/***********************************************************
 * Returns a channel from the map container.               *
 * If it doesn't find it or the map is empty returns NULL. *
 **********************************************************/
Channel* Server::findChannel(const std::string& name) {
	std::map<std::string, Channel>::iterator it = _channels.find(name);

	if (it == _channels.end())
		return NULL;
	return &it->second;
	
}

/******************************************************
 * Adds a new channel in the map container _channels. *
 *****************************************************/
Channel*	Server::addChannel(const std::string& name) {
	Channel* target = findChannel(name);
	if (target)
		return target;

	std::pair<std::map<std::string, Channel>::iterator, bool> inserted =
		_channels.insert(std::make_pair(name, Channel(name)));

		return &inserted.first->second;
}

/**************************************************
 * Removes a channel from map container _channels *
 *************************************************/
void Server::removeChannel(const std::string& name) {
	if (_channels.find(name) == _channels.end()) {
		LOG_DEBUG("removeChannel: channel " << name << " not found");
		return;
	}
	_channels.erase(name);
	LOG_DEBUG("Channel " << name << " removed");
}

/**********************************************
 * Removes a single Client from every channel *
 *********************************************/
void Server::removeClientFromAllChannels(Server& server, Client& client, const std::string& quitMsg) {
	std::vector<std::string> emptyChannels;

	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		Channel& channel = it->second;
		if (channel.hasMember(client)) {
			sendToChannel(channel, quitMsg);
			channel.handlePart(client);
			// Stores all empty channels
			if (channel.getMembers().empty()) {
				emptyChannels.push_back(it->first);
			} else if (channel.getOperators().empty()) {
				Client* newOp = *channel.getMembers().begin();
				channel.addOperator(*newOp);
				server.sendToChannel(channel,
					":ft_irc MODE " + channel.getName() +
					" +o " + newOp->getNickname() + "\r\n"); // ← newOp, no client
			}
		}
	}
	// Remove empty channels
	for (size_t i = 0; i < emptyChannels.size(); i++)
		removeChannel(emptyChannels[i]);

}

/**********************************
 * Disconnects Client from Server *
 *********************************/
void Server::disconnectClientByFd(int fd) {
	for (size_t i = 0; i < _pollfds.size(); i++) {
		if (_pollfds[i].fd == fd) {
			disconnectClient(fd, i);
			return;
		}
	}
}

void Server::flushClientMessages(Client& client) {
	TODO();
	(void)client;
}

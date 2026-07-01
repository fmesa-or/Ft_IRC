#include "Command.hpp"
#include "IRC.hpp"
#include "Server.hpp"
#include <cstring>
#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <cctype>
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
#include <sstream>


#define TERMINAL_COLOR_RED "\033[31m"
#define TERMINAL_COLOR_GREEN "\033[32m"
#define TERMINAL_COLOR_YELLOW "\033[33m"
#define TERMINAL_COLOR_RESET "\033[0m"

#define ERROR(x) (std::cerr << TERMINAL_COLOR_RED "ERROR: " TERMINAL_COLOR_RESET << x << std::endl, false)
#define LOG_DEBUG(x) (std::cerr << TERMINAL_COLOR_YELLOW "DEBUG: " TERMINAL_COLOR_RESET << x << std::endl)
#define LOG(x) (std::cerr << x << std::endl)

typedef struct Input_Data {
	long port;
	char *password;
} Input_Data;

bool validateInput(int argc, char **argv, Input_Data *input_data) {
	if (argc != 3) {
		return ERROR("format: ircserv <port> <password>");
	}

	char *port_str = argv[1];
	char *port_str_end = NULL;
	long port = std::strtol(port_str, &port_str_end, 10);

	if (port < 1024 || port > 65535) {
		return ERROR("Only port numbers 1024 - 65535 are valid.");
	}

	if (*port_str_end != '\0') {
		return ERROR("Couldn't parse port argument. Invalid character \'" << *port_str_end << "\' found.");
	}

	char *password = argv[2];
	int i = 0;
	for (; password[i] != '\0'; i++) {
		if (std::isspace(static_cast<int>(password[i]))) {
			return ERROR("Password cannot contain whitespace.");
		}
	}
	if (i == 0) {
		return ERROR("Password cannot be empty.");
	}

	LOG_DEBUG("Successfully parsed input.");

	input_data->port = port;
	input_data->password = password;

	return true;
}

void close_socket_and_exit_with_failure(int socket_fd) {
	close(socket_fd);
	exit(EXIT_FAILURE);
}

std::string getIpStringFromNetwork(const sockaddr_in& socket_address) {
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &socket_address.sin_addr, ip, INET_ADDRSTRLEN);

	uint16_t port = ntohs(socket_address.sin_port);

	std::stringstream address_string_stream;
	address_string_stream << ip << ":" << port;

	return address_string_stream.str();
}

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
				// FULL DISCONNECT LOGIC?
				close(client_fd);
				continue;
			}
			if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
				// FULL DISCONNECT LOGIC?
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
	}

	size_t line_end_position = 0;
	while ((line_end_position = recv_buffer.find("\r\n")) != std::string::npos) {
		line_end_position += 2;
		std::string message = recv_buffer.substr(0, line_end_position);
		recv_buffer.erase(0, line_end_position);

		LOG_DEBUG("Message: " << message);

		//Command command = parse_command(message);
	}
}

void Server::continuouslyPollSockets(int listening_fd) {
	pollfd listening_pollfd;
	std::memset(&listening_pollfd, 0, sizeof(listening_pollfd));
	listening_pollfd.fd = listening_fd,
	listening_pollfd.events = POLLIN,

	_pollfds.push_back(listening_pollfd);

	while (true) {

		int fds_with_events = poll(&_pollfds[0], _pollfds.size(), -1);
		if (fds_with_events <= 0) {
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
						disconnected = disconnectClient(fd, idx);
					} else if (errno == EAGAIN || errno == EWOULDBLOCK) {
						continue;
					} else {
						ERROR("recv error from client " << fd << ".");
						disconnected = disconnectClient(fd, idx);
					}
				}
			}

			if (disconnected) {
				idx--;
				polled_fd_count--;
			}
		}
	}
}

int main(int argc, char **argv) {
	Input_Data input_data;
	bool ok = validateInput(argc, argv, &input_data);
	if (!ok) {
		return EXIT_FAILURE;
	}

	Server server(input_data.port, input_data.password);

	int listening_socket_fd = server.createConfigureAndSetUpListeningSocket();
	server.continuouslyPollSockets(listening_socket_fd);

	return 0;
}

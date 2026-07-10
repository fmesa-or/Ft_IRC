#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <map>
# include <vector>
# include <sys/poll.h>
# include <sys/types.h>

# include "CommandDispatcher.hpp"
# include "IRC.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class Server
{
public:
	Server(int port, const std::string& password);

	int createConfigureAndSetUpListeningSocket();
	void continuouslyPollSockets(int listening_fd);

	/* Client lookup */

	Client* findClientByFd(int fd);
	Client* findClientByNick(const std::string& nickname);

	/* Channel lookup */

	Channel* findChannel(const std::string& name);

	/* Outgoing communication */

	void flushClientMessages(Client& client);
	void sendToClient(int fd, const std::string& message);
	void sendToChannel(const Channel& channel, const std::string& message);
	const std::string& getPassword() const;

	void cleanup();

private:

	int         _port;
	std::string _password;

	int         _listen_fd;

    CommandDispatcher _dispatcher;

	std::map<int, Client>          _clients;
	std::map<std::string, Channel> _channels;
	std::vector<pollfd>            _pollfds;

	void registerClients(int listening_fd, std::vector<pollfd> &pollfds);
	bool disconnectClient(int fd, size_t pollfds_idx);
	void processClientBuffer(Client &client, char buffer[BUFFER_SIZE], ssize_t bytes_received, int pollfds_idx);
};

#endif

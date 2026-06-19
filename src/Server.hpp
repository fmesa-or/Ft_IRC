#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <map>

# include "Client.hpp"
# include "Channel.hpp"

class Server
{
public:

	Server(int port, const std::string& password);

	void run();

	/* Client lookup */

	Client* findClientByFd(int fd);
	Client* findClientByNick(const std::string& nickname);

	/* Channel lookup */

	Channel* findChannel(const std::string& name);

	/* Outgoing communication */

	void flushClientMessages(Client& client);

private:

	int         _port;
	std::string _password;

	int         _listen_fd;

	std::map<int, Client>          _clients;
	std::map<std::string, Channel> _channels;
};

#endif

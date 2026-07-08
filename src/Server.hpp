#ifndef SERVER_HPP
# define SERVER_HPP

#include <map>
#include <string>

#include "Client.hpp"
#include "Channel.hpp"

class CommandDispatcher;

class Server
{
public:

    Server(int port, const std::string& password);

    void run();

    /*======================================================
    =                     Configuration                    =
    ======================================================*/

    const std::string& getPassword() const;

    /*======================================================
    =                     Client lookup                    =
    ======================================================*/

    Client* findClientByFd(int fd);

    Client* findClientByNick(const std::string& nickname);

    /*======================================================
    =                    Channel lookup                    =
    ======================================================*/

    Channel* findChannel(const std::string& name);

    /*======================================================
    =                 Channel management                   =
    ======================================================*/

    Channel* createChannel(const std::string& name);

    void removeChannel(const std::string& name);

    void addClientToChannel(Client& client, const std::string &channelName);

    void removeClientFromChannel(Client& client, const std::string &channelName);

    /*======================================================
    =                  Client management                   =
    ======================================================*/

    void disconnectClient(Client &client);

    /*======================================================
    =                  Outgoing messages                   =
    ======================================================*/

    void sendToClient(Client &client, const std::string &message);

    void sendToChannel(const std::string &channelName, const std::string &message);

    void flushClientMessages(Client &client);

private:

    int _port;

    std::string _password;

    int _listen_fd;

    std::map<int, Client> _clients;

    std::map<std::string, Channel> _channels;

    CommandDispatcher _dispatcher;
};

#endif

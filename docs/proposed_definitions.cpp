#include <string>
#include <vector>
#include <map>
#include <set>

/*
 * Ownership Rules
 * ----------------
 *
 * Server owns all Client objects.
 * Server owns all Channel objects.
 *
 * Channels may store Client* references,
 * but never create, destroy, or own Clients.
 *
 * All networking operations belong exclusively
 * to Server:
 *
 *   - socket
 *   - bind
 *   - listen
 *   - accept
 *   - poll
 *   - recv
 *   - send
 *   - disconnect handling
 *
 * Client objects only store connection state,
 * IRC state, and message buffers.
 *
 * Command handlers are responsible for IRC
 * behavior and may create Channels when
 * appropriate (e.g. JOIN on a non-existing
 * channel).
 */


/******************************************************************************/
/* COMMAND                                                                    */
/******************************************************************************/

struct Command
{
    std::string              name;
    std::vector<std::string> params;
};

Command parse_command(const std::string& line);


/******************************************************************************/
/* FORWARD DECLARATIONS                                                       */
/******************************************************************************/

class Client;
class Channel;


/******************************************************************************/
/* SERVER                                                                     */
/******************************************************************************/

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

    std::map<int, Client>           _clients;
    std::map<std::string, Channel>  _channels;
};


/******************************************************************************/
/* CLIENT                                                                     */
/******************************************************************************/

class Client
{
public:

    Client();
    Client(int fd);

    /* Connection */

    int getFd() const;

    /* Identity */

    const std::string& getNickname() const;
    const std::string& getUsername() const;

    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);

    /* Registration */

    bool hasPassword() const;
    bool hasNickname() const;
    bool hasUsername() const;

    void setHasPassword(bool value);
    void setHasNickname(bool value);
    void setHasUsername(bool value);

    bool isRegistered() const;

    /* Incoming data */

    std::string& recvBuffer();

    /* Outgoing data */

    void queueMessage(const std::string& message);

    std::string& sendBuffer();

private:

    int         _fd;

    std::string _nickname;
    std::string _username;

    bool        _has_password;
    bool        _has_nickname;
    bool        _has_username;

    std::string _recv_buffer;
    std::string _send_buffer;
};


/******************************************************************************/
/* CHANNEL                                                                    */
/******************************************************************************/

class Channel
{
public:

    Channel();
    Channel(const std::string& name);

    /* Identity */

    const std::string& getName() const;

    /* Membership */

    void addMember(Client& client);
    void removeMember(Client& client);

    bool hasMember(const Client& client) const;

    const std::set<Client*>& getMembers() const;

    /* Operators */

    bool isOperator(const Client& client) const;

    void addOperator(Client& client);
    void removeOperator(Client& client);

    const std::set<Client*>& getOperators() const;

private:

    std::string       _name;

    std::set<Client*> _members;
    std::set<Client*> _operators;
};

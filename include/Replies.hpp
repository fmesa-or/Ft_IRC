#ifndef REPLIES_HPP
# define REPLIES_HPP

#include <string>

class Client;

class Replies
{
public:

    // Registration
    static std::string welcome(const Client &client);

    // Errors
    static std::string needMoreParams(const Client &client, const std::string &command);

    static std::string nicknameInUse(const Client &client, const std::string &nick);

    static std::string noNicknameGiven();

    static std::string passwordIncorrect();

	static std::string alreadyRegistered(const Client &client);

    static std::string notRegistered(const Client &client);

};

#endif

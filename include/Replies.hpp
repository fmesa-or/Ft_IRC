#ifndef REPLIES_HPP
# define REPLIES_HPP

#include <string>

class Client;

class Replies
{
public:

    // Registration
    static std::string	welcome(const Client &client);

    // Errors
    static std::string	needMoreParams(const Client &client, const std::string &command);

    static std::string	nicknameInUse(const Client &client, const std::string &nick);

    static std::string	noNicknameGiven();

	static std::string	noSuchNick(const Client &client, const std::string &nick);

    static std::string	passwordIncorrect();

	static std::string	alreadyRegistered(const Client &client);

    static std::string	notRegistered(const Client &client);


	//Messages
	static std::string	privMsg(const Client &sender, const std::string &target, const std::string &text);

	static std::string	notice(const Client &sender, const std::string &target, const std::string &text);

	static std::string	cannotSendToChannel(const Client &client, const std::string &channel);

	static std::string	part(const Client &client, const std::string &channel);

	static std::string	partReason(Client &client, const std::string &channel, const std::string &reason);

	static std::string	join(const Client &client, const std::string &channel);


	//Channel protocol msg
	static std::string	noSuchChannel(const Client &client, const std::string &channel);

	static std::string	badChannelMask(const Client &client, const std::string &cmd);

	static std::string	noTopicSet(const Client &client, const std::string &channel);

	static std::string	topic(const Client &client, const std::string &channel, const std::string &topic);

	static std::string	nameReply(const Client &client, const std::string &channel, const std::string &names);

	static std::string	endOfNames(const Client &client, const std::string &channel);
};

#endif

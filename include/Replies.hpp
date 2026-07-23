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

	static std::string	whoReply(const Client &requestor, const std::string &channel, const Client &target,
		const std::string &prefix);
 
	static std::string	endOfWho(const Client &client, const std::string &target);


	//Messages
	static std::string	privMsg(const Client &sender, const std::string &target, const std::string &text);

	static std::string	noticeMsg(const Client &sender, const std::string &target, const std::string &text);

	static std::string	partMsg(const Client &client, const std::string &channel);

	static std::string	partReasonMsg(Client &client, const std::string &channel, const std::string &reason);

	static std::string	joinMsg(const Client &client, const std::string &channel);

	static std::string	kickMsg(const Client &client, const std::string &channel, const std::string &target,
		const std::string &reason);

	static std::string	topicMsg(const Client &client, const std::string &channel, const std::string &topic);

	static std::string	removeTopicMsg(const Client &client, const std::string &channel);

	static std::string	quitMsg(const Client &client, const std::string &reason);

	static std::string Replies::closingLinkMsg(const Client &client, const std::string &reason);


	//Channel protocol msg
	static std::string	noSuchChannel(const Client &client, const std::string &channel);

	static std::string	cannotSendToChannel(const Client &client, const std::string &channel);

	static std::string	badChannelMask(const Client &client, const std::string &channel);

	static std::string	noTopicSet(const Client &client, const std::string &channel);

	static std::string	topic(const Client &client, const std::string &channel, const std::string &topic);

	static std::string	nameReply(const Client &client, const std::string &channel, const std::string &names);

	static std::string	endOfNames(const Client &client, const std::string &channel);

	static std::string	channelModeIs(const Client &client, const std::string &channel, const std::string &modes);

	static std::string	unknownMode(const Client &client, const std::string &mode);

	static std::string	notChannelOperator(const Client &client, const std::string &channel);

	static std::string	userNotInChannel(const Client &client, const std::string &nickname, const std::string &channel);

	static std::string	notOnChannel(const Client &client, const std::string &channel);

	static std::string	userOnChannel(const Client &client, const std::string &nickname, const std::string &channel);
	
	static std::string	alreadyInvited(const Client &client, const std::string &nickname, const std::string &channel);

	static std::string	inviting(const Client &client, const std::string &nickname, const std::string &channel);

	static std::string	invite(const Client &client, const std::string &nickname, const std::string &channel);

	static std::string	modeNewOperator(const std::string &channel, const std::string &nickname);
};

#endif

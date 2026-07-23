#include "Replies.hpp"
#include "Client.hpp"

std::string Replies::welcome(const Client &client) {
    return ":ft_irc 001 " + client.getNickname() + " :Welcome to the Internet Relay Network\r\n";
}

std::string Replies::noNicknameGiven() {
    return ":ft_irc 431 :No nickname given\r\n";
}

std::string Replies::nicknameInUse(const Client &client, const std::string &nick) {
    return ":ft_irc 433 " + client.getNickname() + " " + nick + " :Nickname is already in use\r\n";
}

std::string Replies::noSuchNick(const Client &client, const std::string &target) {
    return ":ft_irc 401 " + client.getNickname() + " " + target + " :No such nick/channel\r\n";
}

std::string Replies::needMoreParams(const Client &client, const std::string &cmd) {
    return ":ft_irc 461 " + client.getNickname() + " " + cmd + " :Not enough parameters\r\n";
}

std::string Replies::passwordIncorrect() {
    return ":ft_irc 464 :Password incorrect\r\n";
}

std::string Replies::noticeWrongPassword(const std::string& nickname) {
	return ":ft_irc NOTICE " + nickname + " :Incorrect channel key\r\n";
}

std::string Replies::alreadyRegistered(const Client &client) {
	return ":ft_irc 462 " + client.getNickname() + " :You may not reregister\r\n";
}

std::string Replies::notRegistered(const Client &client) {
    return ":ft_irc 451 " + client.getNickname() + " :You have not registered\r\n";
}


// MSG PROTOCOL
std::string Replies::privMsg(const Client &sender, const std::string &target, const std::string &text) {
    return ":" + sender.getNickname() + "!" + sender.getUsername() + " PRIVMSG " + target + " :" + text + "\r\n";
}

std::string Replies::noticeMsg(const Client &sender, const std::string &target, const std::string &text) {
    return ":" + sender.getNickname() + "!" + sender.getUsername() + " NOTICE " + target + " :" + text + "\r\n";
}


std::string Replies::partMsg(const Client &client, const std::string &channel) {
   return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channel + "\r\n";
}

std::string Replies::partReasonMsg(Client &client, const std::string &channel, const std::string &reason) {
    return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channel + " :" + reason + "\r\n";
}

std::string Replies::joinMsg(const Client &client, const std::string &channel) {
    return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost JOIN " + channel + "\r\n";
}

std::string Replies::kickMsg(const Client &client, const std::string &channel, const std::string &target,
	const std::string &reason) {
    	return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost KICK " + channel +
			" " + target + " :" + reason + "\r\n";
}

std::string Replies::topicMsg(const Client &client, const std::string &channel, const std::string &topic) {
    return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost TOPIC " + channel + " :" + topic + "\r\n";
}

std::string Replies::removeTopicMsg(const Client &client, const std::string &channel) {
    return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost TOPIC " + channel + " :\r\n";
}

std::string Replies::quitMsg(const Client &client, const std::string &reason) {
    return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost QUIT :" + reason + "\r\n";
}

std::string Replies::closingLinkMsg(const Client &client, const std::string &reason) {
    return ":ft_irc ERROR :Closing Link: " + client.getNickname() + " (" + reason + ")\r\n";
}






//CHANNEL PROTOCOL
std::string Replies::noSuchChannel(const Client &client, const std::string &channel) {
    return ":ft_irc 403 " + client.getNickname() + " " + channel + " :No such channel\r\n";
}

std::string	Replies::cannotSendToChannel(const Client &client, const std::string &channel) {
	return ":ft_irc 404 " + client.getNickname() + " " + channel + " :Cannot send to channel\r\n";
}

std::string Replies::badChannelMask(const Client &client, const std::string &channel) {
	return ":ft_irc 476 " + client.getNickname() + " " + channel + " :Bad Channel Mask\r\n";
}

std::string Replies::noTopicSet(const Client &client, const std::string &channel) {
    return ":ft_irc 331 " + client.getNickname() + " " + channel + " :No topic is set\r\n";
}

std::string Replies::topic(const Client &client, const std::string &channel, const std::string &topic) {
    return ":ft_irc 332 " + client.getNickname() + " " + channel + " :" + topic + "\r\n";
}

std::string Replies::nameReply(const Client &client, const std::string &channel, const std::string &names) {
    return ":ft_irc 353 " + client.getNickname() + " = " + channel + " :" + names + "\r\n";
}

std::string Replies::endOfNames(const Client &client, const std::string &channel) {
    return ":ft_irc 366 " + client.getNickname() + " " + channel + " :End of /NAMES list\r\n";
}

std::string Replies::channelModeIs(const Client &client, const std::string &channel, const std::string &modes) {
    return ":ft_irc 324 " + client.getNickname() + " " + channel + " " + modes + "\r\n";
}

std::string Replies::unknownMode(const Client &client, const std::string &mode) {
    return ":ft_irc 472 " + client.getNickname() + " " + mode + " :is unknown mode char to me\r\n";
}

std::string Replies::whoReply(const Client &requestor, const std::string &channel, const Client &target,
	const std::string &prefix) {
    	return ":ft_irc 352 " + requestor.getNickname() + " " + channel + " " + target.getUsername() + " localhost ft_irc "
			+ target.getNickname() + " H" + prefix + " :0 " + target.getNickname() + "\r\n";
}

std::string Replies::endOfWho(const Client &client, const std::string &target) {
    return ":ft_irc 315 " + client.getNickname() + " " + target + " :End of /WHO list.\r\n";
}

std::string Replies::notChannelOperator(const Client &client, const std::string &channel) {
    return ":ft_irc 482 " + client.getNickname() + " " + channel + " :You're not channel operator\r\n";
}

std::string Replies::userNotInChannel(const Client &client, const std::string &nickname, const std::string &channel) {
    return ":ft_irc 441 " + client.getNickname() + " " + nickname + " " + channel + " :They aren't on that channel\r\n";
}

std::string Replies::notOnChannel(const Client &client, const std::string &channel) {
	return ":ft_irc 442 " + client.getNickname() + " " + channel + " :You're not on that channel\r\n";
}

std::string	Replies::userOnChannel(const Client &client, const std::string &nickname, const std::string &channel) {
	return	":ft_irc 443 " + client.getNickname() + " " + nickname + " " + channel + " :is already on channel\r\n";
}

std::string	Replies::alreadyInvited(const Client &client, const std::string &nickname, const std::string &channel) {
	return	":ft_irc 443 " + client.getNickname() + " " + nickname + " " + channel + " :is already invited\r\n";
}

std::string Replies::inviting(const Client &client, const std::string &nickname, const std::string &channel) {
    return ":ft_irc 341 " + client.getNickname() + " " + nickname + " " + channel + "\r\n";
}

std::string Replies::invite(const Client &client, const std::string &nickname, const std::string &channel) {
    return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost INVITE " + nickname + " :" + channel + "\r\n";
}

std::string Replies::modeNewOperator(const std::string &channel, const std::string &nickname) {
    return ":ft_irc MODE " + channel + " +o " + nickname + "\r\n";
}

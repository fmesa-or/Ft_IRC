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

std::string Replies::notice(const Client &sender, const std::string &target, const std::string &text) {
    return ":" + sender.getNickname() + "!" + sender.getUsername() + " NOTICE " + target + " :" + text + "\r\n";
}

std::string	Replies::cannotSendToChannel(const Client &client, const std::string &channel) {
	return ":ft_irc 404 " + client.getNickname() + " " + channel + " :Cannot send to channel\r\n";
}

std::string Replies::part(const Client &client, const std::string &channel) {
   return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channel + "\r\n";
}

std::string Replies::partReason(Client &client, const std::string &channel, const std::string &reason) {
    return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " + channel + " :" + reason + "\r\n";
}

std::string Replies::join(const Client &client, const std::string &channel) {
    return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost JOIN " + channel + "\r\n";
}

//CHANNEL PROTOCOL
std::string Replies::noSuchChannel(const Client &client, const std::string &channel) {
    return ":ft_irc 403 " + client.getNickname() + " " + channel + " :No such channel\r\n";
}

std::string Replies::badChannelMask(const Client &client, const std::string &cmd) {
	return ":ft_irc 476 " + client.getNickname() + " " + cmd + " :Bad Channel Mask\r\n";
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

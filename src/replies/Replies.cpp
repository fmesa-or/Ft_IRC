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

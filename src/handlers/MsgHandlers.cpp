#include "CommandDispatcher.hpp"
#include "Server.hpp"
#include "Replies.hpp"
#include "Command.hpp"
#include "Channel.hpp"


void CommandDispatcher::handlePrivMsg(Server &server, Client &client, const Command &cmd) {
	if (cmd.params.size() < 2) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "PRIVMSG"));
		return ;
	}

	std::string	target	= cmd.params[0];
	std::string text	= cmd.params[1];

	if (target[0] == '#') {
		Channel *broadcast = server.findChannel(target);
		if (!broadcast) {
			server.sendToClient(client.getFd(), Replies::noSuchNick(client, target));
			return ;
		}
		if (!broadcast->hasMember(client)) {
			server.sendToClient(client.getFd(), Replies::cannotSendToChannel(client, target));
			return ;
		}
		for (std::set<Client *>::const_iterator it =  broadcast->getMembers().begin(); it != broadcast->getMembers().end(); it++) {
			Client *member = *it;
			if (member != &client) {
				server.sendToClient(member->getFd(), Replies::privMsg(client, target, text));
			}
		}
	}
	else {
		Client *receiver = server.findClientByNick(target);
			if (!receiver) {
				server.sendToClient(client.getFd(), Replies::noSuchNick(client, target));
				return ;
			}
		server.sendToClient(receiver->getFd(), Replies::privMsg(client, target, text));
	}
}


void CommandDispatcher::handleNotice(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.size() < 2) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "PRIVMSG"));
		return ;
	}

	std::string	target	= cmd.params[0];
	std::string text	= cmd.params[1];

	if (target[0] == '#') {
		Channel *broadcast = server.findChannel(target);
		if (!broadcast)
			return ;
		if (!broadcast->hasMember(client))
			return ;
		for (std::set<Client *>::const_iterator it =  broadcast->getMembers().begin(); it != broadcast->getMembers().end(); it++) {
			Client *member = *it;
			if (member != &client) {
				server.sendToClient(member->getFd(), Replies::notice(client, target, text));
			}
		}
	}
	else {
		Client *receiver = server.findClientByNick(target);
			if (!receiver)
				return ;
		server.sendToClient(receiver->getFd(), Replies::notice(client, target, text));
	}
}

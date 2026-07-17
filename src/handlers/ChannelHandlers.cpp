#include "CommandDispatcher.hpp"
#include "IRC.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include "Replies.hpp"
#include <iostream>
#include <sstream>
#include <cctype>

 /******************************************************************************
 * Builds the names list string for a @param channel.                          *
 *                                                                             *
 * Iterates over all members and concatenates their nicknames separated        *
 * by spaces.                                                                  *
 *                                                                             *
 * Operators are prefixed with '@'.                                            *
 *                                                                             *
 * Example output: "@Paco user1 user2"                                         *
 *                                                                             *
 * Used to build the RPL_NAMREPLY (353) numeric response.                      *
 ******************************************************************************/
namespace
{
	/**
	 * Checks the name of the channel.
	 * IT MUST START BY #
	 */
	bool isValidChannelName(const std::string &name)
	{
		return !name.empty() && name[0] == '#';
	}

	/**
	 * 
	 */
	std::string buildNamesList(const Channel &channel)
	{
		std::ostringstream names;
		const std::set<Client*> &members = channel.getMembers();
		bool first = true;

		for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
		{
			if (!first)
				names << ' ';
			if (channel.isOperator(**it))
				names << '@';
			names << (*it)->getNickname();
			first = false;
		}
		return names.str();
	}
}

/**
 * 
 */
void	CommandDispatcher::handleJoin(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.empty())
	{
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "JOIN"));
		return;
	}

	if (!isValidChannelName(cmd.params[0]))
	{
		server.sendToClient(
			client.getFd(),
			":ft_irc 476 " + client.getNickname() + " " + cmd.params[0] +
			" :Bad Channel Mask\r\n");
		return;
	}

	// Creates a new channel and returns it, if does exits, just returns the channel
	Channel* channel = server.addChannel(cmd.params[0]);
	const std::string key = (cmd.params.size() > 1) ? cmd.params[1] : "";

	// Comprobar que el cliente se puede unir y lo une dentro del objeto canal
	// Revisar, porque creo que es redundante!!!!
	// Añadir mensaje de rechazo!
	if (!channel->canJoin(client, key))
		return;

	channel->handleJoin(client, key);

	const std::string joinMessage = ":" + client.getNickname() + "!" + client.getUsername()
		+ "@localhost JOIN " + channel->getName() + "\r\n";
	server.sendToChannel(*channel, joinMessage);

	// Manda un mensaje informando del topic del canal
	if (channel->getTopic().empty())
	{
		server.sendToClient(
			client.getFd(),
			":ft_irc 331 " + client.getNickname() + " " + channel->getName() +
			" :No topic is set\r\n");
	}
	else
	{
		server.sendToClient(
			client.getFd(),
			":ft_irc 332 " + client.getNickname() + " " + channel->getName() +
			" :" + channel->getTopic() + "\r\n");
	}

	// Añade cliente a la lista de nombres
	server.sendToClient(
		client.getFd(),
		":ft_irc 353 " + client.getNickname() + " = " + channel->getName() +
		" :" + buildNamesList(*channel) + "\r\n");

	// Fin de la lista clientes
	server.sendToClient(
		client.getFd(),
		":ft_irc 366 " + client.getNickname() + " " + channel->getName() +
		" :End of /NAMES list.\r\n");



}

/**
 * Inputs: /mod <#channel> <mode> <argmnts>
 */
void CommandDispatcher::handleMode(Server &server, Client &client, const Command &cmd)
{
	// Check if empty params
	if (cmd.params.empty()) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "JOIN"));
		return;
	}

	// Check if channel is valid
	if (!isValidChannelName(cmd.params[0])) {
		server.sendToClient(
			client.getFd(),
			":ft_irc 476 " + client.getNickname() + " " + cmd.params[0] +
			" :Bad Channel Mask\r\n");
		return;
	}

	// Check if channel exist
	Channel*	channel = server.findChannel(cmd.params[0]);
	if (!channel) {
		// Comunicate with client (hexChat)
		return ;
	}
	// If there is only 1 param (channel) -> Checks all modes
	if (cmd.params.size() == 1) {
		std::string modeStr = "+";
		if (channel->getInviteOnly())     modeStr += "i";
		if (channel->getTopicRestricted()) modeStr += "t";
		if (!channel->getKey().empty())   modeStr += "k";
		if (channel->getUserLimit() > 0)  modeStr += "l";

		server.sendToClient(client.getFd(),
			":ft_irc 324 " + client.getNickname() + " " + channel->getName() +
			" " + modeStr + "\r\n");
		return;
	}

	// Checks if it has a proper mode
	if (cmd.params.size() < 2)
		return;
	if (cmd.params[1].size() < 2)
		return;

	// Sends to especific mod handler
	char		mode = cmd.params[1][1]; // Ignoring the +/- simbol


	switch (mode) {
		case 'k':
			channel->setKey(server, client, cmd);
			break;
		case 'l':
			channel->setUserLimit(server, client, cmd);
			break;
		case 'i': {
			channel->setInvitedOnly(server, client, cmd);
			break;
		}
		case 't': {
			channel->setTopicRestricted(server, client, cmd);
			break;
		}
		case 'o': {
			channel->handleOperatorinator(server, client, cmd);
			break;
		}
	}
	// If no mod founded returns an error (mod is not implemented)
	server.sendToClient(client.getFd(),
		":ft_irc 472 " + client.getNickname() + " " + cmd.params[1] +
		" :is unknown mode char to me\r\n");
}

 /**************************************************************************
 * Handles the WHO command for a given channel.                            *
 *                                                                         *
 * Responds with RPL_WHOREPLY (352) for each member in the channel,        *
 * including username, host, nickname, and operator status (@).            *
 * Ends with RPL_ENDOFWHO (315).                                           *
 *                                                                         *
 * If the channel does not exist or no params are given, sends only (315). *
 **************************************************************************/
void	CommandDispatcher::handleWho(Server &server, Client &client, const Command &cmd) {
	if (cmd.params.empty()) {
		server.sendToClient(client.getFd(),
			":ft_irc 315 " + client.getNickname() + " * :End of /WHO list.\r\n");
		return;
	}

	const std::string& target = cmd.params[0];
	Channel* channel = server.findChannel(target);

	if (!channel) {
		server.sendToClient(client.getFd(),
			":ft_irc 315 " + client.getNickname() + " " + target + " :End of /WHO list.\r\n");
		return;
	}

	const std::set<Client*>& members = channel->getMembers();
	for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it) {
		std::string prefix = channel->isOperator(**it) ? "@" : "";
		// 352: RPL_WHOREPLY
		// :server 352 <requestor> <channel> <user> <host> <server> <nick> H[@] :<hopcount> <realname>
		server.sendToClient(client.getFd(),
			":ft_irc 352 " + client.getNickname() + " " + target +
			" " + (*it)->getUsername() +
			" localhost ft_irc " + (*it)->getNickname() +
			" H" + prefix + " :0 " + (*it)->getNickname() + "\r\n");
	}

	server.sendToClient(client.getFd(),
		":ft_irc 315 " + client.getNickname() + " " + target + " :End of /WHO list.\r\n");
}

/**
 * 
 */
void	CommandDispatcher::handleTopic(Server &server, Client &client, const Command &cmd) {

	// Check params
	if (cmd.params.empty()) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "TOPIC"));
		return;
	}

	// Checks if channel exist
	const std::string& target = cmd.params[0];
	Channel* channel = server.findChannel(target);

	 if (!channel) {
		server.sendToClient(client.getFd(),
			":ft_irc 403 " + client.getNickname() + " " + target + " :No such channel\r\n");
		return;
	}

	// Checks if we recieve a topic and can be overwrited by the client
	if (cmd.params.size() > 1) {
		if (channel->getTopicRestricted() && !channel->isOperator(client)) {
			server.sendToClient(client.getFd(),
				":ft_irc 482 " + client.getNickname() + " " + target +
				" :You're not channel operator\r\n");
			return;
		}
		// Agrupates all the params forming the topic
		std::string newTopic = cmd.params[1];
		for (size_t i = 2; i < cmd.params.size(); i++)
			newTopic += " " + cmd.params[i];

		if (newTopic == "remove") {
			newTopic = "";
		}

		channel->setTopic(newTopic);

		if (newTopic.empty()) {
				// Broadcast to everyone that the topic has changed
				server.sendToChannel(*channel,
					":" + client.getNickname() + "!" + client.getUsername() +
					"@localhost TOPIC " + target + " :\r\n");

					// Confirms client with 331
				server.sendToClient(client.getFd(),
					":ft_irc 331 " + client.getNickname() + " " + target +
					" :No topic is set\r\n");
		} else {
			server.sendToChannel(*channel,
				":" + client.getNickname() + "!" + client.getUsername() +
				"@localhost TOPIC " + target + " :" + newTopic + "\r\n");
		}
		return;
	}

	// Checks the actual topic
	if (channel->getTopic().empty()) {
		server.sendToClient(client.getFd(),
			":ft_irc 331 " + client.getNickname() + " " + target + " :No topic is set\r\n");
	} else {
		server.sendToClient(client.getFd(),
			":ft_irc 332 " + client.getNickname() + " " + target +
			" :" + channel->getTopic() + "\r\n");
	}

}


/*
void CommandDispatcher::handlePart(Server &, Client &, const Command &)
{
    TODO();
}

void CommandDispatcher::handleQuit(Server &, Client &, const Command &)
{
    TODO();
}
*/

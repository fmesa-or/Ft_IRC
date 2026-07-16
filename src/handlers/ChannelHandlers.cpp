#include "CommandDispatcher.hpp"
#include "IRC.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include "Replies.hpp"
#include <iostream>
#include <sstream>

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

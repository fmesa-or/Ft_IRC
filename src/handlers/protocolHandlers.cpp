#include "CommandDispatcher.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include "Replies.hpp"

 /****************************************************************************
 * Handles the CAP (Capability Negotiation) command.                         *
 *                                                                           *
 * CAP is used by modern IRC clients to negotiate extended protocol features *
 * before registration. This server supports no capabilities.                *
 *                                                                           *
 * Subcommands handled:                                                      *
 *  - LS  : Lists available capabilities → responds with empty list.         *
 *  - LIST: Lists active capabilities    → responds with empty list.         *
 *  - REQ : Requests a capability        → always rejected with NAK.         *
 *  - END : Ends negotiation             → no response needed.               *
 *                                                                           *
 * Any other subcommand or missing params is silently ignored.               *
 ****************************************************************************/
void CommandDispatcher::handleCap(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.empty())
		return;

	const std::string &subcommand = cmd.params[0];
	if (subcommand == "END")
		return;

	if (subcommand == "LS")
	{
		server.sendToClient(client.getFd(), ":ft_irc CAP * LS :\r\n");
		return;
	}

	if (subcommand == "LIST")
	{
		server.sendToClient(client.getFd(), ":ft_irc CAP * LIST :\r\n");
		return;
	}

	if (subcommand == "REQ")
	{
		if (cmd.params.size() < 2)
			return;
		server.sendToClient(client.getFd(), ":ft_irc CAP * NAK :" + cmd.params[1] + "\r\n");
		return;
	}
}

/******************************************
 * Checks if the connections stills alive *
 *****************************************/
void CommandDispatcher::handlePing(Server &server, Client &client, const Command &cmd) {
	const std::string token = cmd.params.empty() ? "ft_irc" : cmd.params[0];
	server.sendToClient(client.getFd(), ":ft_irc PONG ft_irc :" + token + "\r\n");
}

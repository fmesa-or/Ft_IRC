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
	/***********************************
	 * Checks the name of the channel. *
	 * IT MUST START BY #              *
	 **********************************/
	bool isValidChannelName(const std::string &name)
	{
		return !name.empty() && name[0] == '#';
	}

	
	// Builds the NAMES list sent to the client
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


	 // Checks whether the channel exists
	
	Channel*	channelExist(const std::string &target, Server &server, Client &client) {
		Channel*	channel = server.findChannel(target);
		if (!channel) {
			server.sendToClient(client.getFd(), Replies::noSuchChannel (client, target));
			return NULL;
		}
		return channel;
	}
}

/****************************
 * Handles the JOIN command *
 ***************************/
void	CommandDispatcher::handleJoin(Server &server, Client &client, const Command &cmd)
{
	if (cmd.params.empty())
	{
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "JOIN"));
		return;
	}

	if (!isValidChannelName(cmd.params[0]))
	{
		server.sendToClient(client.getFd(), Replies::badChannelMask(client, cmd.params[0]));
		return;
	}

	// Creates a new channel if it doesn't exist. Otherwise, returns the existing channel.
	Channel* channel = server.addChannel(cmd.params[0]);
	const std::string key = (cmd.params.size() > 1) ? cmd.params[1] : "";

	if (!channel->handleJoin(server, client, key))
		return;

	server.sendToChannel(*channel, Replies::joinMsg(client, channel->getName()));

	// Sends the topic of the channel
	if (channel->getTopic().empty())
		server.sendToClient(client.getFd(), Replies::noTopicSet(client, channel->getName()));

	else
		server.sendToClient(client.getFd(), Replies::topic(client, channel->getName(), channel->getTopic()));

	// Sends the NAMES list to the client.
	server.sendToClient(client.getFd(), Replies::nameReply(client, channel->getName(), buildNamesList(*channel)));

	// Sends the end of the NAMES list reply
	server.sendToClient(client.getFd(), Replies::endOfNames(client, channel->getName()));
}

/********************************************
 * Inputs: /mod <#channel> <mode> <argmnts> *
 *******************************************/
void CommandDispatcher::handleMode(Server &server, Client &client, const Command &cmd)
{
	// Checks parameters
	if (cmd.params.empty()) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "MODE"));
		return;
	}

	// Checks whether the channel is valid
	if (!isValidChannelName(cmd.params[0])) {
		server.sendToClient(client.getFd(), Replies::badChannelMask(client, cmd.params[0]));
		return;
	}

	// Checks whether the channel exists
	Channel*	channel;
	if (!(channel = channelExist(cmd.params[0], server, client)))
		return;

	// // If only the channel is provided, checks all active modes
	if (cmd.params.size() == 1) {
		std::string modeStr = "+";
		if (channel->getInviteOnly())     modeStr += "i";
		if (channel->getTopicRestricted()) modeStr += "t";
		if (!channel->getKey().empty())   modeStr += "k";
		if (channel->getUserLimit() > 0)  modeStr += "l";

		server.sendToClient(client.getFd(), Replies::channelModeIs(client, channel->getName(), modeStr));
		return;
	}

	// Checks if it has a proper mode
	if (cmd.params.size() < 2)
		return;
	if (cmd.params[1].size() < 2)
		return;

	// Sends the command to the specific mode handler
	char		mode = cmd.params[1][1]; // Ignoring the +/- symbol


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
		default:
			server.sendToClient(client.getFd(), Replies::unknownMode(client, cmd.params[1]));
	}
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
		server.sendToClient(client.getFd(), Replies::endOfWho(client, "*"));
		return;
	}

	// Checks whether the channel exists
	Channel*	channel;
	if (!(channel = channelExist(cmd.params[0], server, client)))
		return;

	const std::string	target = cmd.params[0];
	const std::set<Client*>& members = channel->getMembers();
	for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it) {
		std::string prefix = channel->isOperator(**it) ? "@" : "";
		// 352: RPL_WHOREPLY
		// :server 352 <requestor> <channel> <user> <host> <server> <nick> H[@] :<hopcount> <realname>
		server.sendToClient(client.getFd(),  Replies::whoReply(client, target, **it, prefix));
	}

	server.sendToClient(client.getFd(), Replies::endOfWho(client, target));
}

/*********************************************************
 * Handles the TOPIC command.                            *
 * It could be for asking topic, change it or delete it. *
 ********************************************************/
void	CommandDispatcher::handleTopic(Server &server, Client &client, const Command &cmd) {

	// Checks parameters
	if (cmd.params.empty()) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "TOPIC"));
		return;
	}

	// Checks whether the channel exists
	Channel*	channel;
	if (!(channel = channelExist(cmd.params[0], server, client)))
		return;

	// Checks whether a new topic was provided and can be overwritten by the client
	const std::string	target = cmd.params[0];
	if (cmd.params.size() > 1) {
		if (channel->getTopicRestricted() && !channel->isOperator(client)) {
			server.sendToClient(client.getFd(), Replies::notChannelOperator(client, target));
			return;
		}
		std::string newTopic = cmd.params[1];

		if (newTopic == "remove")
			newTopic = "";

		channel->setTopic(newTopic);

		if (newTopic.empty()) {
				// Broadcasts the topic change to all channel members
				server.sendToChannel(*channel, Replies::removeTopicMsg(client, target));

				// Sends RPL_NOTOPIC (331) to the client
				server.sendToClient(client.getFd(), Replies::noTopicSet(client, target));
		} 
		else {
			server.sendToChannel(*channel, Replies::topicMsg(client, target, newTopic));
		}
		return;
	}

	// Sends current topic
	if (channel->getTopic().empty()) {
		server.sendToClient(client.getFd(), Replies::noTopicSet(client, target));
	} 
	else {
		server.sendToClient(client.getFd(), Replies::topic(client, target, channel->getTopic()));
	}

}

/*********************************
 * Kicks a client from a channel *
 ********************************/
void CommandDispatcher::handleKick(Server &server, Client &client, const Command &cmd)
{
	// Checks parameters
	if (cmd.params.size() < 2) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "KICK"));
		return;
	}

	// Checks whether the channel exists
	Channel*	channel;
	if (!(channel = channelExist(cmd.params[0], server, client)))
		return;

	// Checks operator credentials
	if (!channel->isOperator(client)) {
		server.sendToClient(client.getFd(), Replies::notChannelOperator(client, cmd.params[0]));
		return;
	}

	// Finds target by nickname
	Client* target = server.findClientByNick(cmd.params[1]);
	if (!target || !channel->hasMember(*target)) {
		server.sendToClient(client.getFd(), Replies::userNotInChannel(client, cmd.params[1], cmd.params[0]));
		return;
	}

	// Cannot kick yourself if last member
	if (target == &client) {
		server.sendToClient(client.getFd(),
			":ft_irc 482 " + client.getNickname() + " " + channel->getName() +
			" :You cannot kick yourself\r\n");
		return;
	}

	// If there isn't any reason, use operator nick.
	const std::string reason = (cmd.params.size() > 2) ? cmd.params[2] : client.getNickname();

	// Broadcast Kick to everyone in channel
	server.sendToChannel(*channel, Replies::kickMsg(client, channel->getName(), target->getNickname(), reason));

	// Internal server kick
	channel->handleKick(client, *target);
}

/*****************************
 * Invites Client to channel *
 ****************************/
void CommandDispatcher::handleInvite(Server &server, Client &client, const Command &cmd) {
	// Checks parameters
	if (cmd.params.size() < 2) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "INVITE"));
		return;
	}

	const std::string& targetNick = cmd.params[0];
	const std::string& channelName = cmd.params[1];
	
	// Checks whether the channel exists
	Channel*	channel;
	if (!(channel = channelExist(channelName, server, client)))
		return;

	// Checks whether the client is a member
	if (!channel->hasMember(client)) {
		server.sendToClient(client.getFd(), Replies::notOnChannel(client, channelName));
		return;
	}

	// Checks if inviteOnly is active && If is NOT an operator
	if (channel->getInviteOnly() && !channel->isOperator(client)) {
		server.sendToClient(client.getFd(), Replies::notChannelOperator(client, channelName));
		return;
	}

	// Finds target in server
	Client* target = server.findClientByNick(targetNick);
	if (!target) {
		server.sendToClient(client.getFd(), Replies::noSuchNick(client, targetNick));
		return;
	}


	// Already a member
	if (channel->hasMember(*target)) {
		server.sendToClient(client.getFd(), Replies::userOnChannel(client, targetNick, channelName));
		return;
	} 
	// Already invited
	if (channel->hasInvited(*target)) {
		server.sendToClient(client.getFd(), Replies::alreadyInvited(client, targetNick, channelName));
		return;
	}

	channel->handleInvite(client, *target);

	// Notifies the inviter that the invitation was sent
	server.sendToClient(client.getFd(), Replies::inviting(client, targetNick, channelName));

	// Notifies the invited user
	server.sendToClient(target->getFd(), Replies::invite(client, targetNick, channelName));
}

/*****************************************
 * Makes client leave a channel.         *
 * If no one is left removes the channel *
 *                                       *
 * /part <nickname> <reason>             *
 ****************************************/
void CommandDispatcher::handlePart(Server &server, Client &client, const Command &cmd)
{
	// Checks parameters
	if (cmd.params.empty()) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, "PART"));
		return;
	}

	// Checks whether the channel exists
	Channel* channel;
	if (!(channel = channelExist(cmd.params[0], server, client)))
		return;

	// Checks whether the client is a member
	if (!channel->hasMember(client)) {
		server.sendToClient(client.getFd(), Replies::notOnChannel(client, cmd.params[0]));
		return;
	}

	// Optional reason
	const std::string reason = (cmd.params.size() > 1) ? cmd.params[1] : "";

	// Broadcast PART to everyone before leaving
	if (reason.empty()) {
    	server.sendToChannel(*channel, Replies::partMsg(client, channel->getName()));
	}
	else {
    	server.sendToChannel(*channel, Replies::partReasonMsg(client, channel->getName(), reason));
}

	channel->handlePart(client);

	// Assigns a new operator if the channel becomes orphaned
	if (channel->getMembers().empty()) {
		server.removeChannel(cmd.params[0]);
	} 
	else if (channel->getOperators().empty()) {
		Client* newOp = *channel->getMembers().begin();
		channel->addOperator(*newOp);
		server.sendToChannel(*channel, Replies::modeNewOperator(channel->getName(), newOp->getNickname()));
	}
}

/*************************************************************************************
 * Handles QUIT command.                                                             *
 * Broadcast the quit message to every channel containing @param client and leaves it *
 ************************************************************************************/
void CommandDispatcher::handleQuit(Server &server, Client &client, const Command &cmd)
{
	const std::string reason = cmd.params.empty() ? "Client quit" : cmd.params[0];

	const std::string quitMsg = Replies::quitMsg(client, reason);

	// Notify every channel before parting
	server.removeClientFromAllChannels(server, client, quitMsg);

	// Sends ERROR before disconnecting the client
	server.sendToClient(client.getFd(), Replies::closingLinkMsg(client, reason));

	// Disconnects from server
	server.disconnectClientByFd(client.getFd());
}

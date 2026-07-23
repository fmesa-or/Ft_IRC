/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:09:04 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/07/23 19:40:30 by fmesa-or         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"
#include "Server.hpp"
#include "Replies.hpp"
#include <cstdlib>

/***************
 * Constructor *
 **************/
Channel::Channel(void)
	: _name("default"), _topic(""), _key(""), _inviteOnly(false),
	 _topicRestricted(false), _userLimit(0){}

 /********************
  * Name Constructor *
  *******************/
Channel::Channel(const std::string& name)
	: _name(name) , _topic(""), _key(""), _inviteOnly(false),
	 _topicRestricted(false), _userLimit(0){
		std::cout << "Channel " << _name << " created" << std::endl;
	 }

	/************
	 * IDENTITY *
	 ***********/

/******************************************
 * Returns @param _name from Channel Class*
 *****************************************/
const std::string& Channel::getName() const {
	return _name;
}

	/**************
	 * MEMBERSHIP *
	 *************/

/**********************************************************************
 * Agregates a new @param client to the @param _members set container *
 *********************************************************************/
void	Channel::addMember(Client& client) {
	addToContainer(client, _members);
	LOG_DEBUG("Client: " << client.getNickname() << " added to " << getName());
}

/*******************************************************************
 * Wipe out a @param client from the @param _members set container *
 ******************************************************************/
void	Channel::removeMember(Client& client) {
	if (isOperator(client)) {
		removeFromContainer(client, _operators);
	}

	removeFromContainer(client, _members);
}

/*****************************************************************************
 * Checks if a @param client exists inside the @param _members set container *
 ****************************************************************************/
bool	Channel::hasMember(const Client& client) const {
	return hasInContainer(client, _members);
}

/*************************************************************
 * Returns a reference of the full @param _members container *
 ************************************************************/
const std::set<Client*>&	Channel::getMembers() const {
	return _members;
}

	/*************
	 * OPERATORS *
	 ************/

/************************************************************************
 * Agregates a new @param client to the @param _operators set container *
 *	only if it is already in @param _members                            *
 ***********************************************************************/
bool	Channel::addOperator(Client& client) {
	if (!hasMember(client)) {
		LOG_DEBUG("Error: Couldn't add operator. Is not a member.");
		return false;
	}
	addToContainer(client, _operators);
	return true;
}

/*********************************************************************
 * Wipe out a @param client from the @param _operators set container *
 *	but not from member.                                             *
 * If @param client is the only one operator, it doesn't removes it. *
 ********************************************************************/
bool	Channel::removeOperator(Client& client) {
	if (_operators.size() <= 1) {
		LOG_DEBUG("Error: Couldn't remove operator. Always must be one.");
		return false;
	}
	removeFromContainer(client, _operators);
	return true;
}

/***************************************************************************
 * Checks if a @param client is inside the @param _operators set container *
 **************************************************************************/
bool	Channel::isOperator(const Client& client) const {
	return hasInContainer(client, _operators);
}

/***************************************************************
 * Returns a reference of the full @param _operators container *
 **************************************************************/
const std::set<Client*>&	Channel::getOperators() const {
	return _operators;
}

	/***********
	 * INVITED *
	 **********/

/**********************************************************************
 * Agregates a new @param client to the @param _invited set container *
 * For std::pair we can use .first & .second.                         *
 *	first	-> Iterator pointing to the new element.                  *
 *	second	-> True if added, False if not.                           *
 *********************************************************************/
void	Channel::addInvited(Client& client) {
	addToContainer(client, _invited);
}

/*******************************************************************
 * Wipe out a @param client from the @param _invited set container *
 ******************************************************************/
void	Channel::removeInvited(Client& client) {
	removeFromContainer(client, _invited);
}

/*****************************************************************************
 * Checks if a @param client exists inside the @param _invited set container *
 ****************************************************************************/
bool	Channel::hasInvited(const Client& client) const {
	return hasInContainer(client, _invited);
}

/*************************************************************
 * Returns a reference of the full @param _invited container *
 ************************************************************/
const std::set<Client*>&	Channel::getInvited() const {
	return _invited;
}

	/*******************
	 * CHANNEL SUPPORT *
	 ******************/

/************************************************
 * Checks if @param client can join the channel *
 ***********************************************/
bool Channel::canJoin(Server& server, const Client& client, const std::string& key) const {
	// Already a member
	if (hasMember(client)) {
		server.sendToClient(client.getFd(),
			":ft_irc 443 " + client.getNickname() + " " + getName() +
			" :is already on channel\r\n");
		return false;
	}
	// Full channel
	if (_userLimit > 0 && _members.size() >= _userLimit) {
		server.sendToClient(client.getFd(),
			":ft_irc 471 " + client.getNickname() + " " + getName() +
			" :Cannot join channel (+l)\r\n");
		return false;
	}
	// Invite only
	if (_inviteOnly && !hasInvited(client)) {
		server.sendToClient(client.getFd(),
			":ft_irc 473 " + client.getNickname() + " " + getName() +
			" :Cannot join channel (+i)\r\n");
		return false;
	}
	// Wrong password
	if (!_key.empty() && key != _key) {
		server.sendToClient(client.getFd(),
			":ft_irc 475 " + client.getNickname() + " " + getName() +
			" :Cannot join channel (+k)\r\n");
		return false;
	}
	return true;
}

/*****************************************************************
 * Joins a @param client in the channel.                         *
 *                                                               *
 *	Checks if @param client can join.                            *
 *	                                                             *
 *	Checks if it's the creator of the channel (the first member) *
 *		and adds it to the operators container.                  *
 *	Checks if there is an invitation mode                        *
 *		and removes the user from the invitation list.           *
 ****************************************************************/
bool	Channel::handleJoin(Server& server, Client& client, const std::string& key) {
	if (!canJoin(server, client, key)) {
		return false;
	}
	bool isFirst = _members.empty();
	addMember(client);
	if (isFirst) {
		addOperator(client);
		server.sendToChannel(*this,
					":ft_irc MODE " + this->getName() +
					" +o " + client.getNickname() + "\r\n");
	}
	if (_inviteOnly) {
		removeInvited(client); // Remove the invitation for this user
	}
	return true;
}

/*********************************
 * Overload when inserted no key *
 ********************************/
bool	Channel::handleJoin(Server& server, Client& client) {
	if (!canJoin(server, client, "")) {
		return false;
	}
	bool isFirst = _members.empty();
	addMember(client);
	if (isFirst) {
		addOperator(client);
		server.sendToChannel(*this,
					":ft_irc MODE " + this->getName() +
					" +o " + client.getNickname() + "\r\n");
	}
	if (_inviteOnly) {
		removeInvited(client); // Remove the invitation for this user
	}
	return true;
}

/*************************************************
 * Removes @param client itself from the channel *
 * 
 * <expand when rol A made new parts>
 ************************************************/
void	Channel::handlePart(Client& client) {
	removeMember(client);
}

/************************************************************************************
 * Checks if the @param kicker has the permissions to remove the @param target user *
 ***********************************************************************************/
void	Channel::handleKick(Client& kicker, Client& target) {
	if (isOperator(kicker)) {
		removeMember(target);
	}
}

/*****************************************************************************
 * Checks if the invitation mode is on and if the inviter has the permission *
 ****************************************************************************/
void	Channel::handleInvite(Client& inviter, Client& target) {
	if (_inviteOnly && isOperator(inviter)) {
		addInvited(target);
	}
}

/***********************************************
 * Checks if the invitator is an Operator      *
 * 	Then checks the action add(+) or remove(-) *
 **********************************************/
void Channel::handleOperatorinator(Server& server, Client& client, Command cmd) {
	if (cmd.params.size() < 3) {
		server.sendToClient(client.getFd(),
			":ft_irc 461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
		return;
	}

	// Client not found or not member of channel
	Client* target = server.findClientByNick(cmd.params[2]);
	if (!target) {
		server.sendToClient(client.getFd(), Replies::noSuchNick(client, client.getNickname()));
		return;
	}
	if (!hasMember(*target)) {
		server.sendToClient(client.getFd(), Replies::userNotInChannel(client, client.getNickname(), getName()));
		return;
	}

	if (!isOperator(client)) {
		server.sendToClient(client.getFd(), Replies::notChannelOperator(client, getName()));
		return;
	}

	if (cmd.params[1][0] == '+') {
		if (!addOperator(*target)) {
			server.sendToClient(client.getFd(), Replies::userNotInChannel(client, client.getNickname(), getName()));
			return;
		}
		server.sendToChannel(*this,
			":" + client.getNickname() + "!" + client.getUsername() +
			"@localhost MODE " + getName() + " +o " + cmd.params[2] + "\r\n");
	} else if (cmd.params[1][0] == '-') {
		if (!isOperator(*target)) {
			server.sendToClient(client.getFd(), Replies::userNotInChannel(client, client.getNickname(), getName()));
		} else if (!removeOperator(*target)) {
			server.sendToClient(client.getFd(), Replies::notChannelOperator(client, getName()));
		} else {
			server.sendToChannel(*this,
				":" + client.getNickname() + "!" + client.getUsername() +
				"@localhost MODE " + getName() + " -o " + cmd.params[2] + "\r\n");
		}
	} else {
		server.sendToClient(client.getFd(), Replies::unknownMode(client, cmd.params[1]));
		return;
	}
}

/***********************************************
 * Changes Topic to a new one.                 *
 * If restricted only Operators can change it. *
 **********************************************/
void	Channel::setTopic(std::string topic) {
	_topic = topic;
}

	/*****************
	 * CHANNEL MODES *
	 ****************/

/*************************************************
 * Changes inviteOnly mode if client is operator *
 ************************************************/
void	Channel::setInvitedOnly(Server& server, Client& client, Command cmd) {
	if (!isOperator(client)){
		server.sendToClient(client.getFd(), Replies::notChannelOperator(client, getName()));
		return;
	}
	if (cmd.params[1][0] == '+') {
		_inviteOnly = true;
		server.sendToChannel(*this,
			":" + client.getNickname() + "!" + client.getUsername() +
			"@localhost MODE " + this->getName() + " " +
			cmd.params[1] + "\r\n");
	} else if (cmd.params[1][0] == '-'){
		_inviteOnly = false;
		server.sendToChannel(*this,
			":" + client.getNickname() + "!" + client.getUsername() +
			"@localhost MODE " + this->getName() + " " +
			cmd.params[1] + "\r\n");
	} else {
		server.sendToClient(client.getFd(), Replies::unknownMode(client, cmd.params[1]));
		return;
	}
}

/******************************************************
 * Changes topicRestricted mode if client is operator *
 *****************************************************/
void	Channel::setTopicRestricted(Server& server, Client& client, Command cmd) {
	if (!(isOperator(client))){
		server.sendToClient(client.getFd(), Replies::notChannelOperator(client, getName()));
		return;
	}
	
	if (cmd.params[1][0] == '+') {
		_topicRestricted = true;
		server.sendToChannel(*this,
			":" + client.getNickname() + "!" + client.getUsername() +
			"@localhost MODE " + this->getName() + " " +
			cmd.params[1] + "\r\n");
	} else if (cmd.params[1][0] == '-'){
		_topicRestricted = false;
		server.sendToChannel(*this,
			":" + client.getNickname() + "!" + client.getUsername() +
			"@localhost MODE " + this->getName() + " " +
			cmd.params[1] + "\r\n");
	} else {
		server.sendToClient(client.getFd(), Replies::unknownMode(client, cmd.params[1]));
		return;
	}
}

/******************************************
 * Changes password if client is operator *
 *****************************************/
void	Channel::setKey(Server &server, Client &client, Command cmd) {
	if (cmd.params.size() < 3) {
		server.sendToClient(client.getFd(),
			":ft_irc 461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
		return;
	}
	if (!isOperator(client)){
		server.sendToClient(client.getFd(), Replies::notChannelOperator(client, getName()));
		return;
	}
	// Check if adds or removes key
	if (cmd.params[1][0] == '+') {
		_key = cmd.params[2];
		LOG_DEBUG("PASWORD ADDED: " << cmd.params[2]);
	} else if (cmd.params[1][0] == '-') {
		if (cmd.params[2] != _key) {
			LOG_DEBUG("Wrong PASWORD");
			server.sendToClient(client.getFd(), Replies::noticeWrongPassword(client.getNickname()));
			return;
		} else {
			_key = "";
		}
	} else {
		server.sendToClient(client.getFd(), Replies::unknownMode(client, cmd.params[1]));
		return;
	}

	server.sendToChannel(*this, ":" + client.getNickname() + "!" + client.getUsername() +
	"@localhost MODE " + this->getName() + " " +
	cmd.params[1] + " " + cmd.params[2] + "\r\n");
}

/*******************************************
 * Changes userLimit if client is operator *
 ******************************************/
void	Channel::setUserLimit(Server &server, Client &client, Command cmd) {
	if (!isOperator(client)) {
		server.sendToClient(client.getFd(), Replies::notChannelOperator(client, getName()));
		return;
	}
	if (cmd.params[1][0] == '+') {
		if (cmd.params.size() < 3) {
		server.sendToClient(client.getFd(), Replies::needMoreParams(client, cmd.name));
		return;
		}
		int limit = atoi(cmd.params[2].c_str());
		if (limit <= 0) {
			server.sendToClient(client.getFd(),
				":ft_irc 696 " + client.getNickname() + " " + getName() +
				" l :Invalid limit value\r\n");
			return;
		}
		_userLimit = static_cast<size_t>(limit);
		server.sendToChannel(*this,
			":" + client.getNickname() + "!" + client.getUsername() +
			"@localhost MODE " + this->getName() + " " +
			cmd.params[1] +
			(cmd.params.size() > 2 ? " " + cmd.params[2] : "") + "\r\n");
	} else if (cmd.params[1][0] == '-') {
		_userLimit = 0;
		server.sendToChannel(*this,
			":" + client.getNickname() + "!" + client.getUsername() +
			"@localhost MODE " + getName() + " -l\r\n");
	} else {
		server.sendToClient(client.getFd(), Replies::unknownMode(client, cmd.params[1]));
	}
}

	/*********************
	 * SETTERS & GETTERS *
	 ********************/

// SETTERS

void	Channel::setName(std::string name) {
	_name = name;
}


// GETTERS

std::string&	Channel::getTopic() {
	return _topic;
}

std::string&	Channel::getKey() {
	return _key;
}

bool			Channel::getInviteOnly() {
	return _inviteOnly;
}

bool			Channel::getTopicRestricted() {
	return _topicRestricted;
}

size_t			Channel::getUserLimit() {
	return _userLimit;
}

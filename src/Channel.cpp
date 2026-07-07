/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:09:04 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/07/07 18:05:19 by fmesa-or         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/***************
 * Constructor *
 **************/
Channel::Channel(void) : _name("default") {}

 /********************
  * Name Constructor *
  *******************/
Channel::Channel(const std::string& name) : _name(name) {}

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
void Channel::addMember(Client& client) {
	addToContainer(client, _members);
}

/*******************************************************************
 * Wipe out a @param client from the @param _members set container *
 ******************************************************************/
void Channel::removeMember(Client& client) {
	if (isOperator(client)) {
		removeFromContainer(client, _operators);
	}
	
	removeFromContainer(client, _members);

	// Comprobar si canal queda vacío y marcar para que sea eliminado por rol A
}

/*****************************************************************************
 * Checks if a @param client exists inside the @param _members set container *
 ****************************************************************************/
bool Channel::hasMember(const Client& client) const {
	return hasInContainer(client, _members);
}

/*************************************************************
 * Returns a reference of the full @param _members container *
 ************************************************************/
const std::set<Client*>& Channel::getMembers() const {
	return _members;
}

	/*************
	 * OPERATORS *
	 ************/

/************************************************************************
 * Agregates a new @param client to the @param _operators set container *
 *	only if it is already in @param _members                            *
 * For std::pair we can use .first & .second.                           *
 *	first	-> Iterator pointing to the new element.                    *
 *	second	-> True if added, False if not.                             *
 ***********************************************************************/
void Channel::addOperator(Client& client) {
	if (!hasMember(client)) {
		// No puede ser añadido como operador si no existe previamente como miembro
		return; // Puede que aquí añada un throw
	}
	addToContainer(client, _operators);
}

/*********************************************************************
 * Wipe out a @param client from the @param _operators set container *
 *	but not from member.                                             *
 * If @param client is the only one operator, it doesn't removes it. *
 ********************************************************************/
void Channel::removeOperator(Client& client) {
	// Allways has to be at least one operator
	if (_operators.size() > 1) {
		removeFromContainer(client, _operators);
	}
	else if (_operators.size() == 1 && _members.size() == 1) {
		removeFromContainer(client, _operators);
		removeFromContainer(client, _members);
	}
	else {
		// Mensaje: solo puede quedar uno (operador)
	}
}

/***************************************************************************
 * Checks if a @param client is inside the @param _operators set container *
 **************************************************************************/
bool Channel::isOperator(const Client& client) const {
	return hasInContainer(client, _operators);
}

/***************************************************************
 * Returns a reference of the full @param _operators container *
 **************************************************************/
const std::set<Client*>& Channel::getOperators() const {
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
void Channel::addInvited(Client& client) {
	addToContainer(client, _invited);
}

/*******************************************************************
 * Wipe out a @param client from the @param _invited set container *
 ******************************************************************/
void Channel::removeInvited(Client& client) {
	removeFromContainer(client, _invited);

	// Comprobar si canal queda vacío y marcar para que sea eliminado por rol A
}

/*****************************************************************************
 * Checks if a @param client exists inside the @param _invited set container *
 ****************************************************************************/
bool Channel::hasInvited(const Client& client) const {
	return hasInContainer(client, _invited);
}

/*************************************************************
 * Returns a reference of the full @param _invited container *
 ************************************************************/
const std::set<Client*>& Channel::getInvited() const {
	return _invited;
}

	/*********
	 * OTHER *
	 ********/

	// SETTERS

	void	Channel::setName(std::string name) {
		_name = name;
	}

	void	Channel::setTopic(std::string topic) {
		_topic = topic;
	}

	void	Channel::setKey(const std::string key) {
		_key = key;
	}

	void	Channel::setInvitedOnly(bool inviteOnly) {
		_inviteOnly = inviteOnly;
	}

	void	Channel::setTopicRestricted(bool topicRestricted) {
		_topicRestricted = topicRestricted;
	}

	void	Channel::setUserLimit(size_t userLimit) {
		_userLimit = userLimit;
	}

	// GETTERS

	std::string&	Channel::getName() {
		return _name;
	}

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

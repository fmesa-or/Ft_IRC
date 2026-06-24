/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:09:04 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/06/24 18:45:16 by fmesa-or         ###   ########.fr       */
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
	addContainer(client, _members);
}

/*******************************************************************
 * Wipe out a @param client from the @param _members set container *
 ******************************************************************/
void Channel::removeMember(Client& client) {
	if (isOperator(client)) {
		removeOperator(client);
	}
	
	_members.erase(&client);

	// Comprobar si canal queda vacío y marcar para que sea eliminado por rol A
}

/*****************************************************************************
 * Checks if a @param client exists inside the @param _members set container *
 ****************************************************************************/
bool Channel::hasMember(const Client& client) const {
	std::set<Client*>::const_iterator it = _members.find(const_cast<Client*>(&client));
	if (it != _members.end()) {
		return 1;
	}
	return 0;
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
	addContainer(client, _operators);
}

/*********************************************************************
 * Wipe out a @param client from the @param _operators set container *
 *	but not from member.                                             *
 * If @param client is the only one operator, it doesn't removes it. *
 ********************************************************************/
void Channel::removeOperator(Client& client) {
	// Allways has to be at least one operator
	if (_operators.size() > 1) {
		_operators.erase(&client);
	}
	else if (_operators.size() == 1 && _members.size() == 1) {
		_operators.erase(&client);
		_members.erase(&client);
	}
	else {
		// Mensaje: solo puede quedar uno (operador)
	}
}

/***************************************************************************
 * Checks if a @param client is inside the @param _operators set container *
 **************************************************************************/
bool Channel::isOperator(const Client& client) const {
	std::set<Client*>::const_iterator it = _operators.find(const_cast<Client*>(&client));
	if (it != _operators.end()) {
		return 1;
	}
	return 0;
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
	addContainer(client, _invited);
}

/*******************************************************************
 * Wipe out a @param client from the @param _invited set container *
 ******************************************************************/
void Channel::removeInvited(Client& client) {
	_invited.erase(&client);

	// Comprobar si canal queda vacío y marcar para que sea eliminado por rol A
}

/*****************************************************************************
 * Checks if a @param client exists inside the @param _invited set container *
 ****************************************************************************/
bool Channel::hasInvited(const Client& client) const {
	std::set<Client*>::const_iterator it = _invited.find(const_cast<Client*>(&client));
	if (it != _invited.end()) {
		return 1;
	}
	return 0;
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
	/*
	void	setName(std::string name);
	void	setTopic(std::string topic);
	void	setKey(const std::string key);
	void	setInvitedOnly(bool inviteOnly);
	void	setTopicRestricted(bool topicRestricted);
	void	setUserLimit(size_t userLimit);

	std::string&	getName();
	std::string&	getTopic();
	std::string&	getKey();
	bool			getInvitedOnly();
	bool			getTopicRestricted();
	size_t			getUserLimit();*/


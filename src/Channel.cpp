/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:09:04 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/06/22 20:46:05 by fmesa-or         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/***************
 * Constructor *
 **************/
Channel::Channel(void) {
	std::cout << "Class Channel() created" << std::endl;
}

 /********************
  * Name Constructor *
  *******************/
Channel::Channel(const std::string& name) : _name(name);

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
 * For std::pair we can use .first & .second.                         *
 *	first	-> Iterator pointing to the new element.                  *
 *	second	-> True if added, False if not.                           *
 *********************************************************************/
void addMember(Client& client) {
	std::pair<std::set<Client>::iterator, bool> res = _members.insert(client);

	if (!res.second) {
		// Cliente repetido, no se ha insertado
	}

	// Revisar si .insert() puede lanzar excepciones de algún tipo
	// Añadir más adelante funcionalidad para +l (limite) y +i (invitación)
}

/*******************************************************************
 * Wipe out a @param client from the @param _members set container *
 ******************************************************************/
void removeMember(Client& client) {
	if (hasOperator(client)) {
		removeOperator(client);
	}
	
	_members.erase(client)

	// Comprobar si canal queda vacío y marcar para que sea eliminado por rol A
}

/*****************************************************************************
 * Checks if a @param client exists inside the @param _members set container *
 ****************************************************************************/
bool hasMember(const Client& client) const {
	return _members.find(client);
}

/*************************************************************
 * Returns a reference of the full @param _members container *
 ************************************************************/
const std::set<Client*>& getMembers() const {
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
void addOperator(Client& client) {
	if (!hasMember(client)) {
		// No puede ser añadido como operador si no existe previamente como miembro
		return; // Puede que aquí añada un throw
	}

	std::pair<std::set<Client>::iterator, bool> res = _operators.insert(client);

	if (!res.second) {
		// Cliente ya es operador, no se ha insertado
	}

	// Revisar si .insert() puede lanzar excepciones de algún tipo
}

/*********************************************************************
 * Wipe out a @param client from the @param _operators set container *
 *	but not from member.                                             *
 * If @param client is the only one operator, it doesn't removes it. *
 ********************************************************************/
void removeOperator(Client& client) {
	// Allways has to be at least one operator
	if (_operators.size() > 1) {
		_operators.erase(client);
	} else {
		// Mensaje: solo puede quedar uno (operador)
	}
}

/***************************************************************************
 * Checks if a @param client is inside the @param _operators set container *
 **************************************************************************/
bool isOperator(const Client& client) const {
	return _operators.find(client);
}

/***************************************************************
 * Returns a reference of the full @param _operators container *
 **************************************************************/
const std::set<Client*>& getOperators() const {
	return _operators;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 18:09:04 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/06/22 18:54:41 by fmesa-or         ###   ########.fr       */
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

/***************************************************************
 * Wipe a @param client from the @param _members set container *
 **************************************************************/
void removeMember(Client& client) {
	_members.erase(client)

	// Revisar si está en operador, entonces eliminarlo también
	// Comprobar si canal queda vacío y marcar para que sea eliminado por rol A
}

/**
 * Checks if a @param client exists inside the @param _members set container *
 * Also returns how many times shows up in the container.
 * 	If apears more than once                     <to fullfil>
 */
bool hasMember(const Client& client) const {
	size_t	amount = _members.count(client);

	switch (amount) {
		case 0:
			// No aparece
			break;
		case 1:
			// Aparece una única vez
			break;
		default:
			// Aparece más de una vez. | Gestionar objeto Cliente repetido
	}

	return amount;
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


void addOperator(Client& client);
void removeOperator(Client& client);

bool isOperator(const Client& client) const;


const std::set<Client*>& getOperators() const;
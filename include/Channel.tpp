/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.tpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 18:26:26 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/07/07 18:06:27 by fmesa-or         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/************************************************************************
 * We made this so, if we want to expand the variety of containers,     *
 *	we just aply the templates.                                         *
 *                                                                      *
 * Also if it's needed to change anything in the main logical structure *
 *	just need to change the template                                    *
 ***********************************************************************/

/***********************************************************************
 * Agregates a new @param client to the @param container set container *
 **********************************************************************/
template <typename Container>
void	Channel::addToContainer(Client& client, Container& container) {
	std::pair<std::set<Client*>::iterator, bool> res = container.insert(&client);

	if (!res.second) {
		// Cliente repetido, no se ha insertado
	}

	// Revisar si .insert() puede lanzar excepciones de algún tipo
	// Añadir más adelante funcionalidad para +l (limite) y +i (invitación)
}

/********************************************************************
 * Wipe out a @param client from the @param container set container *
 *******************************************************************/
template <typename Container>
void	Channel::removeFromContainer(Client& client, Container& container) {
	container.erase(&client);
}

/******************************************************************************
 * Checks if a @param client exists inside the @param container set container *
 *****************************************************************************/
template <typename Container>
bool	Channel::hasInContainer(const Client& client, Container& container) const {
	std::set<Client*>::const_iterator it = container.find(const_cast<Client*>(&client));
	if (it != container.end()) {
		return 1;
	}
	return 0;
}



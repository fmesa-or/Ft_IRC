/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.tpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 18:26:26 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/06/25 17:47:26 by fmesa-or         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/**
 * We made this so, if we want to expand the variety of containers, *
 *	we just aply the templates.
 * 
 * Also if it's needed to change anything in the main logical structure *
 *	just need to change the template                                    *
 */

/**
 * 
 */
template <typename Container>
void	Channel::addContainer(Client& client, Container& container) {
	std::pair<std::set<Client*>::iterator, bool> res = container.insert(&client);

	if (!res.second) {
		// Cliente repetido, no se ha insertado
	}

	// Revisar si .insert() puede lanzar excepciones de algún tipo
	// Añadir más adelante funcionalidad para +l (limite) y +i (invitación)
}

/**
 * 
 */
template <typename Container>
void	Channel::removeContainer(Client& client, Container& container) {
	container.erase(&client);
}

/**
 * 
 */
template <typename Container>
bool	Channel::hasContainer(const Client& client, Container& container) const {
	std::set<Client*>::const_iterator it = container.find(const_cast<Client*>(&client));
	if (it != container.end()) {
		return 1;
	}
	return 0;
}



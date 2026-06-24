/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.tpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 18:26:26 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/06/24 18:41:34 by fmesa-or         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

template <typename Container>
void	Channel::addContainer(Client& client, Container& container) {
	std::pair<std::set<Client*>::iterator, bool> res = container.insert(&client);

	if (!res.second) {
		// Cliente repetido, no se ha insertado
	}

	// Revisar si .insert() puede lanzar excepciones de algún tipo
	// Añadir más adelante funcionalidad para +l (limite) y +i (invitación)
}

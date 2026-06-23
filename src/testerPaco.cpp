/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testerPaco.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:04:44 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/06/23 18:33:53 by fmesa-or         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

class Client {
	private:
		std::string	_name;
	public:
		Client(std::string& name) : _name(name){};
};


/**********************************************
 * UNITARY TESTS                              *
 *                                            *
 * Hide Client.hpp to use the following tests *
 *********************************************/
int	main() {
		
/*
	try {
		// Constructors
		Channel ch1;
		Channel ch2("MálagaFC");

		// Identity
		std::cout << ch1.getName() << " | " << ch2.getName() << std::endl;

		/ Membership
		std::string	name1 = "Miguel";
		Client cliente1(name1);
		ch1.addMember(cliente1);
		ch2.addMember(cliente1);
		if (ch1.hasMember(cliente1) && ch2.hasMember(cliente1)) {
			std::cout << "CHECK" << std::endl;
		}

		ch2.removeMember(cliente1);
		std::cout << "ch2 members amount: " << ch2.getMembers().size() << std::endl;

		// Operators
		ch1.addOperator(cliente1);
		if (ch1.isOperator(cliente1)) {
			std::cout << "CHECK op1" << std::endl;
		}

		ch2.addOperator(cliente1);
		if (!(ch2.isOperator(cliente1))) {
			std::cout << "CHECK op2" << std::endl;
		}
		
		ch1.removeOperator(cliente1);
		std::cout << "ch1 members amount: " << ch1.getOperators().size() << std::endl;
		

	}
	catch(const std::exception& ex) {
		std::cout << "Exception: " << ex.what() << std::endl;
	}
*/


	return 0;
}

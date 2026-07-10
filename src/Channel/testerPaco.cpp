/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testerPaco.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 17:04:44 by fmesa-or          #+#    #+#             */
/*   Updated: 2026/07/10 14:50:54 by fmesa-or         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

static void registerClient(Client& client, const std::string& nickname, const std::string& username)
{
	client.setNickname(nickname);
	client.setHasNickname(true);
	client.setUsername(username);
	client.setHasUsername(true);
	client.setHasPassword(true);
}

/**********************************************
 * UNITARY TESTS                              *
 *                                            *
 *********************************************/
int	main() {


	try {
		// Constructors
		std::cout << "****************" 
				  << "* CONSTRUCTORS *"
				  << "****************" << std::endl;
		Channel ch1;
		Channel ch2("MálagaFC");

		// Identity
		std::cout << std::endl;
		std::cout << "************" 
				  << "* IDENTITY *"
				  << "************" << std::endl;
		std::cout << ch1.getName() << " | " << ch2.getName() << std::endl;

		// Membership
		std::cout << std::endl;
		std::cout << "**************" 
				  << "* MEMBERSHIP *"
				  << "**************" << std::endl;
		std::string	name1 = "Miguel";
		Client cliente1;
		registerClient(cliente1, name1, "miguel");
		ch1.addMember(cliente1);
		ch2.addMember(cliente1);
		if (ch1.hasMember(cliente1) && ch2.hasMember(cliente1)) {
			std::cout << "CHECK" << std::endl;
		}

		ch2.removeMember(cliente1);
		std::cout << "ch2 members amount: " << ch2.getMembers().size() << std::endl;

		// Operators
		std::cout << std::endl;
		std::cout << "*************" 
				  << "* OPERATORS *"
				  << "*************" << std::endl;
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
		
		// Invited
		std::cout << std::endl;
		std::cout << "*************" 
				  << "** INVITED **"
				  << "*************"<< std::endl;
		Channel	ch3("Pokefans");
		std::string	name2 = "PikachuFan<33";
		Client	client2;
		registerClient(client2, name2, "pikachufan");

		ch3.addInvited(client2);
		if (ch3.hasInvited(client2)) {
			std::cout << "Added to Invited" << std::endl;
		}
		ch3.getInvited();
		ch3.removeInvited(client2);
		if (ch3.hasInvited(client2)) {
			std::cout << "Removed from Invited" << std::endl;
		} else {
			std::cout << "Couldn't be removed from Invited" << std::endl;
		}

		// Channel Support
		std::cout << std::endl;
		std::cout << "***********" 
				  << "  CHANNEL SUPORT "
				  << "***********"<< std::endl;
		std::string	name3 = "VaporeonMaster76";
		Client	client3;
		registerClient(client3, name3, "vaporeonmaster");

		// First user must be Operator
		ch3.handleJoin(client2);
		std::cout << "_member size = " << ch3.getMembers().size()
			<< "\n_operator size = " << ch3.getOperators().size() 
			<< std::endl;

		if (ch3.getOperators().size() == 1) {
			std::set<Client*>::iterator it = ch3.getOperators().begin();
			std::cout << "Operator is " << (*it)->getNickname() << std::endl;
		} else {
			std::cout << "Failed to create first operator" << std::endl;
		}

		// Test normal JOIN
		std::cout << std::endl;
		std::cout << "*************" 
				  << "*** JOIN ****"
				  << "*************"<< std::endl;
		ch3.handleJoin(client3);
		std::cout << "Number of members(2) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;

		// Test PART
		std::cout << std::endl;
		std::cout << "*************" 
				  << "**** PART ***"
				  << "*************" << std::endl;
		ch3.handlePart(client3);
		std::cout << "Number of members(1) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;

		// KEY
		std::cout << std::endl;
		std::cout << "*************" 
				  << "**** KEY ****"
				  << "*************" << std::endl;
		ch3.setKey("PokeTruco");
			// NO KEY
		std::cout << "* ------No KEY------ *" << std::endl;

		ch3.handleJoin(client3);
		std::cout << "Number of members(1) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;
			// WRONG KEY
		std::cout << "* ------Wrong KEY------ *" << std::endl;

		ch3.handleJoin(client3, "abracadabra");
		std::cout << "Number of members(1) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;
			// GOOD KEY
		std::cout << "* ------Good KEY------ *" << std::endl;
		ch3.handleJoin(client3, "PokeTruco");
		std::cout << "Number of members(2) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;

		// INVITE
		std::cout << std::endl;
		std::cout << "*************" 
				  << "** INVITE ***"
				  << "*************" << std::endl;

		ch3.setInvitedOnly(true);
		std::string	name4 = "XxSnorlaxLifterxX";
		Client	client4;
		registerClient(client4, name4, "snorlaxlifter");
			// No
		std::cout << std::endl;
		std::cout << "* ------No INVITED------ *" << std::endl;

		ch3.handleJoin(client4);
		std::cout << "Number of invited(0) = " <<  ch3.getInvited().size() << std::endl;
		std::cout << "Number of members(2) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;
			// Yes
		std::cout << std::endl;
		std::cout << "* ------Yes INVITED BUT WRONG OPERATOR------ *" << std::endl;

		ch3.handleInvite(client4, client4);

		std::cout << "Number of invited(0) = " <<  ch3.getInvited().size() << std::endl;
		std::cout << "Number of members(2) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;

		std::cout << "* ------Yes INVITED------ *" << std::endl;

		ch3.handleInvite(client2, client4);

		std::cout << "Number of invited(1) = " <<  ch3.getInvited().size() << std::endl;
		std::cout << "Number of members(2) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;

		ch3.handleJoin(client4, "PokeTruco");

		std::cout << "\nJOIN INVITED" << std::endl;
		std::cout << "Number of invited(0) = " <<  ch3.getInvited().size() << std::endl;
		std::cout << "Number of members(3) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;

		// Canal lleno
		std::cout << std::endl;
		std::cout << "*************" 
				  << "*** LIMIT ***"
				  << "*************" << std::endl;

		ch3.setInvitedOnly(false);
		std::cout << "Setting _userLimit to 4" << std::endl;
		std::string	name5 = "Satoshi";
		Client	client5;
		registerClient(client5, name5, "satoshi");
		std::string	name6 = "Red";
		Client	client6;
		registerClient(client6, name6, "red");
		ch3.setUserLimit(4);

		ch3.handleJoin(client5, "PokeTruco");
		std::cout << "Number of members(4) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;
		
		std::cout << "\n->Forcing limit" << std::endl;
		ch3.handleJoin(client6, "PokeTruco");
		std::cout << "Number of members(4) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;

		
		// Test KICK
		std::cout << std::endl;
		std::cout << "*************" 
				  << "*** KICK ****"
				  << "*************" << std::endl;
		ch3.handleKick(client2, client3);

		std::cout << "KICK client3" << std::endl;
		std::cout << "Number of members(3) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;
		
		std::cout << "JOIN again client3" << std::endl;
		ch3.handleJoin(client3, "PokeTruco");
		std::cout << "Number of members(4) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;

		// Test Already Member
		std::cout << std::endl;
		std::cout << "*************" 
				  << "* ALREADY MEMBER *"
				  << "*************" << std::endl;
		std::cout << "Try to add an already member" << std::endl;
		ch3.handleJoin(client3, "PokeTruco");
		std::cout << "Number of members(4) = " <<  ch3.getMembers().size() << std::endl;
		std::cout << "Number of operators(1) = " <<  ch3.getOperators().size() << std::endl;


	}
	catch(const std::exception& ex) {
		std::cout << "Exception: " << ex.what() << std::endl;
	}



	return 0;
}

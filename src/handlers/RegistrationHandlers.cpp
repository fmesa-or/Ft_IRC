#include "CommandDispatcher.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include "Replies.hpp"


/* ************************************************************************** */
/*                            User registration                               */
/* ************************************************************************** */

void CommandDispatcher::handlePass(Server &server, Client &client, const Command &cmd) {
	if (client.isRegistered()) {
		server.sendToClient(client.getFd(), Replies::alreadyRegistered(client));
		return;
	}
	if (cmd.params.empty()) {
        server.sendToClient(client.getFd(), Replies::needMoreParams(client, "PASS"));
        return;
    }

    if (cmd.params[0] != server.getPassword()) {
        server.sendToClient(client.getFd(), Replies::passwordIncorrect());
        return;
    }

    client.setHasPassword(true);

   // tryRegister(server, client);
}

void CommandDispatcher::handleNick(Server &server, Client &client, const Command &cmd) {
	if (cmd.params.empty()) {
    server.sendToClient(client.getFd(), Replies::noNicknameGiven());
        return;
    }

    const std::string &nick = cmd.params[0];

    Client *other = server.findClientByNick(nick);

    if (other && other != &client) {
        server.sendToClient(client.getFd(), Replies::nicknameInUse(client, nick));
        return;
    }

    client.setNickname(nick);
    client.setHasNickname(true);

   // tryRegister(server, client);
}

void CommandDispatcher::handleUser(Server &server, Client &client, const Command &cmd) {
	if (client.isRegistered()) {
        server.sendToClient(client.getFd(), Replies::alreadyRegistered(client));
		return;
	}
	if (cmd.params.size() < 4) {
        server.sendToClient(client.getFd(), Replies::needMoreParams(client, "USER"));
        return;
    }
    client.setUsername(cmd.params[0]);
    client.setHasUsername(true);

    //tryRegister(server, client);
}

/*void CommandDispatcher::tryRegister(Server &server, Client &client) {
	if (client.isRegistered())
		return;
	if (!client.hasPassword())
		return;
	if (!client.hasNickname())
		return;
	if (!client.hasUsername())
        return;

    client.setRegistered(true);

    server.sendToClient(client, Replies::welcome(client));
}*/

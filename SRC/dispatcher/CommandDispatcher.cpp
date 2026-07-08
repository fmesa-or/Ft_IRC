#include "CommandDispatcher.hpp"
#include "Replies.hpp"


CommandDispatcher::CommandDispatcher(){
	registerHandlers();
}

void CommandDispatcher::dispatch(Server &server, Client &client, const Command &cmd) {
    HandlerIterator it = _handlers.find(cmd.name);

    if (it == _handlers.end())
    {
        server.sendToClient(server, client, "421 " + cmd.name + " :Unknown command");
        return;
    }

    Handler h = it->second;

    (this->*h)(server, client, cmd);
}



void CommandDispatcher::handlePass(Server &server, Client &client, const Command &cmd) {
	    if (cmd.params.empty())
    {
        server.sendToClient(client, Replies::needMoreParams(client, "PASS"));
        return;
    }

    if (cmd.params[0] != server.getPassword())
    {
        sendError(server, client, "464 :Password incorrect\r\n");
        return;
    }

    client.setHasPassword(true);

    tryRegister(server, client);
}

void CommandDispatcher::handleNick(Server &server, Client &client, const Command &cmd) {
    if (cmd.params.empty()) {
        sendError(server, client, "431 :No nickname given\r\n");
        return;
    }

    const std::string &nick = cmd.params[0];

    Client *other = server.findClientByNick(nick);

    if (other && other != &client) {
        //sendError(server, client, "433 " + nick + " :Nickname is already in use\r\n");
        return;
    }

    client.setNickname(nick);
    client.setHasNickname(true);

    tryRegister(server, client);
}

void CommandDispatcher::handleUser(Server &server, Client &client, const Command &cmd) {
	if (cmd.params.size() < 4) {
        //sendError(server, client, "461 USER :Not enough parameters\r\n");
        return;
    }

    client.setUsername(cmd.params[0]);
    client.setHasUsername(true);

    tryRegister(server, client);
}

void CommandDispatcher::tryRegister(Server &server, Client &client) {
	if (client.isRegistered)
		return;
	if (!client._hasPassword())
		return;
	if (!client._hasNickname())
		return;
	if (!client.hasUsername())
        return;

    client.setRegistered(true);

    server.sendToClient(client, ":ft_irc 001 " + client.getNickname() + " :Welcome to the Internet Relay Network\r\n");
}

/* ************************************************************************** *
/
/*                                Channels                                    */
/* ************************************************************************** */

void CommandDispatcher::handleJoin(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;

    // TODO
}

void CommandDispatcher::handlePart(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;

    // TODO
}

void CommandDispatcher::handleQuit(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;

    // TODO
}

/* ************************************************************************** */
/*                               Messaging                                    */
/* ************************************************************************** */

void CommandDispatcher::handlePrivmsg(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;

    // TODO
}

void CommandDispatcher::handleNotice(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;

    // TODO
}

/* ************************************************************************** */
/*                                Protocol                                    */
/* ************************************************************************** */

void CommandDispatcher::handlePing(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;

    // TODO
}

void CommandDispatcher::handlePong(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;

    // TODO
}

/* ************************************************************************** */
/*                     Advanced channel management                            */
/* ************************************************************************** */

/*
void CommandDispatcher::handleMode(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;
}

void CommandDispatcher::handleTopic(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;
}

void CommandDispatcher::handleInvite(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;
}

void CommandDispatcher::handleKick(Server &server, Client &client, const Command &cmd) {
    (void)server;
    (void)client;
    (void)cmd;
}
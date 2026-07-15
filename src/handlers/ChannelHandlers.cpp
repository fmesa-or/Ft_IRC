#include "CommandDispatcher.hpp"
#include "IRC.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include "Replies.hpp"
#include <iostream>


void CommandDispatcher::handleJoin(Server &server, Client &client, const Command &cmd)
{
	// Creates a new channel and returns it, if does exits, just returns the channel
	Channel* channel = server.addChannel(cmd.params[0]);

	// Comprobar que el cliente se puede unir
	channel->handleJoin(client);

	// Unir al cliente al canal



}
/*
void CommandDispatcher::handlePart(Server &, Client &, const Command &)
{
    TODO();
}

void CommandDispatcher::handleQuit(Server &, Client &, const Command &)
{
    TODO();
}
*/

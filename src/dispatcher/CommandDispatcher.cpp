#include "CommandDispatcher.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Replies.hpp"


CommandDispatcher::CommandDispatcher(){
	registerHandlers();
}

CommandDispatcher::~CommandDispatcher() {
}

void CommandDispatcher::dispatch(Server &server, Client &client, const Command &cmd) {
    HandlerIterator it = _handlers.find(cmd.name);

    if (it == _handlers.end())
    {
        server.sendToClient(client.getFd(), "421 " + cmd.name + " :Unknown command");
        return;
    }

	if (!client.isRegistered() && cmd.name != "PASS" && cmd.name != "NICK" && cmd.name != "USER" && cmd.name!= "JOIN") {
        server.sendToClient(client.getFd(), Replies::notRegistered(client));
        return;
    }

    Handler h = it->second;

    (this->*h)(server, client, cmd);
}

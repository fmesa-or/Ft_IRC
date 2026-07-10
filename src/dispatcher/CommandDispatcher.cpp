#include "CommandDispatcher.hpp"
#include "Replies.hpp"


CommandDispatcher::CommandDispatcher(){
	registerHandlers();
}

void CommandDispatcher::dispatch(Server &server, Client &client, const Command &cmd) {
    HandlerIterator it = _handlers.find(cmd.name);

    if (it == _handlers.end())
    {
        server.sendToClient(client, "421 " + cmd.name + " :Unknown command");
        return;
    }

	if (!client.isRegistered() && cmd.name != "PASS" && cmd.name != "NICK" && cmd.name != "USER") {
        server.sendToClient(client, Replies::notRegistered(client));
        return;
    }

    Handler h = it->second;

    (this->*h)(server, client, cmd);
}

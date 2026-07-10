#include "CommandDispatcher.hpp"
#include "Server.hpp"
#include "Replies.hpp"

/* ************************************************************************** */
/*                              Registration                                  */
/* ************************************************************************** */
 
void CommandDispatcher::registerHandlers() {

	// Registration
	_handlers["PASS"] = &CommandDispatcher::handlePass;
	_handlers["NICK"] = &CommandDispatcher::handleNick;
	_handlers["USER"] = &CommandDispatcher::handleUser;


    // Channels
    _handlers["JOIN"] = &CommandDispatcher::handleJoin;
    _handlers["PART"] = &CommandDispatcher::handlePart;
    _handlers["QUIT"] = &CommandDispatcher::handleQuit;
    
    // Messaging
    _handlers["PRIVMSG"] = &CommandDispatcher::handlePrivmsg;
    _handlers["NOTICE"]  = &CommandDispatcher::handleNotice;
    
    // Protocol
    _handlers["PING"] = &CommandDispatcher::handlePing;
    _handlers["PONG"] = &CommandDispatcher::handlePong;
}

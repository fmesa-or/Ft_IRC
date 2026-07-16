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
	_handlers["CAP"] = &CommandDispatcher::handleCap;

	// Channels
	_handlers["JOIN"] = &CommandDispatcher::handleJoin;
	_handlers["MODE"] = &CommandDispatcher::handleMode;
	/*
    _handlers["PART"] = &CommandDispatcher::handlePart;
    _handlers["QUIT"] = &CommandDispatcher::handleQuit;
	// KICK
	// INVITE Nickname #channel (+i only operators / -i anyone can)
	// TOPIC
    
    // Messaging
    _handlers["PRIVMSG"] = &CommandDispatcher::handlePrivmsg;
    _handlers["NOTICE"]  = &CommandDispatcher::handleNotice;
    
    // Protocol
    _handlers["PING"] = &CommandDispatcher::handlePing;
    _handlers["PONG"] = &CommandDispatcher::handlePong;
	*/
}

#ifndef COMMANDDISPATCHER_HPP
# define COMMANDDISPATCHER_HPP

#include <string>
#include <map>

class Server;
class Client;
struct Command;

// class Server;
// class Client;


class CommandDispatcher {
	public:
    	CommandDispatcher();
    	~CommandDispatcher();

    	void dispatch(Server &server, Client &client, const Command &cmd);

	private:

		// Pointer to member function
    	typedef	void (CommandDispatcher::*Handler)(Server &server, Client &client, const Command &cmd);

		// Iterator
		typedef	std::map<std::string, Handler>::iterator HandlerIterator;

		// Command table
    	std::map<std::string, Handler> _handlers;

		//  REGISTRATION
    	void registerHandlers();
		void handlePass(Server &server, Client &client, const Command &cmd);
    	void handleNick(Server &server, Client &client, const Command &cmd);
    	void handleUser(Server &server, Client &client, const Command &cmd);
		void tryRegister(Server &server, Client &client);

    	// ---- CHANNELS ----
		void handleJoin(Server &server, Client &client, const Command &cmd);
		void handleMode(Server &server, Client &client, const Command &cmd);
		void handleTopic(Server &server, Client &client, const Command &cmd);
		void handleKick(Server &server, Client &client, const Command &cmd);
		void handleInvite(Server &server, Client &client, const Command &cmd);
		void handlePart(Server &server, Client &client, const Command &cmd);
		void handleQuit(Server &server, Client &client, const Command &cmd);


    	// ---- MESSAGING ----
    //	void handlePrivmsg(Server &server, Client &client, const Command &cmd);
    //	void handleNotice(Server &server, Client &client, const Command &cmd);

    	// ---- PROTOCOL ----
		//Envía un mensaje IRC de error al cliente que mandó un comando desconocido.
		void handleCap(Server &server, Client &client, const Command &cmd);
		void handlePing(Server &server, Client &client, const Command &cmd);
		void handleWho(Server &server, Client &client, const Command &cmd);

    //	void handlePong(Server &server, Client &client, const Command &cmd);
};

#endif

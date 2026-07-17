#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <iostream>
# include "Command.hpp"

class Server;
class Client;

class Channel
{
private:

	std::string			_name;
	std::string			_topic;

	std::set<Client*>	_members;
	std::set<Client*>	_operators;	// +o

	std::string			_key;		// +k
	std::set<Client*>	_invited;

	bool				_inviteOnly;	// +i
	bool				_topicRestricted;	// +t
	size_t				_userLimit;	// +l | 0 = Limitless

	/* Templates */
	template <typename Container>
	void	addToContainer(Client& client, Container& container);

	template <typename Container>
	void	removeFromContainer(Client& client, Container& container);

	template <typename Container>
	bool	hasInContainer(const Client& client, Container& container) const ;

public:

	Channel();
	Channel(const std::string& name);

	/* Identity */

	const std::string&	getName() const;

	/* Membership */

	void	addMember(Client& client);
	void	removeMember(Client& client);
	bool	hasMember(const Client& client) const;
	const	std::set<Client*>&	getMembers() const;

	/* Operators */

	void	addOperator(Client& client);
	void	removeOperator(Client& client);
	bool	isOperator(const Client& client) const;
	const std::set<Client*>&	getOperators() const;

	/* Invited */
	void	addInvited(Client& client);
	void	removeInvited(Client& client);
	bool	hasInvited(const Client& client) const;
	const	std::set<Client*>&	getInvited() const;

	/* Channel Support */
	bool	canJoin(const Client& client, const std::string& key) const;
	void	handleJoin(Client& client, const std::string& key);  // El primer usuario que crea el canal debe ser operador.
	void	handleJoin(Client& client); // Overload
	void	handlePart(Client& client);
	void	handleKick(Client& kicker, Client& target);
	void	handleInvite(Client& inviter, Client& invited);
	void	handleOperatorinator(Server& server, Client& client, Command cmd);

	/* Channel Modes */
	void	setTopic(std::string topic);
	void	setInvitedOnly(Server& server, Client& client, Command cmd);
	void	setTopicRestricted(Server& server, Client& client, Command cmd);
	void	setKey(Server &server, Client &client, Command cmd);
	void	setUserLimit(Server &server, Client &client, Command cmd);

	/* Setters & Getters */
	void	setName(std::string name);

	std::string&	getTopic();
	std::string&	getKey();
	bool			getInviteOnly();
	bool			getTopicRestricted();
	size_t			getUserLimit();

};

# include "Channel.tpp"

#endif

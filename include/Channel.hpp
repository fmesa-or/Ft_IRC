#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>

class Client;

class Channel
{
private:

	std::string			_name;
	std::string			_topic;

	std::set<Client*>	_members;
	std::set<Client*>	_operators;

	std::string			_key;		//+k
	std::set<Client*>	_invited;	//+i

	bool				_inviteOnly;
	bool				_topicRestricted;
	size_t				_userLimit;	//+l | 0 = Limitless

public:

	Channel();
	Channel(const std::string& name);

	/* Identity */

	const std::string& getName() const;

	/* Membership */

	void addMember(Client& client);
	void removeMember(Client& client);
	bool hasMember(const Client& client) const;

	const std::set<Client*>& getMembers() const;

	/* Operators */

	bool isOperator(const Client& client) const;

	void addOperator(Client& client);
	void removeOperator(Client& client);

	const std::set<Client*>& getOperators() const;

	/* Channel Creation Support */
//	handleJoin(); // El primer usuario que crea el canal debe ser operador.
};

#endif

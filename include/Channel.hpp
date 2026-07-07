#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <iostream>


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

	/* Others */
	void	setName(std::string name);
	void	setTopic(std::string topic);
	void	setKey(const std::string key);
	void	setInvitedOnly(bool inviteOnly);
	void	setTopicRestricted(bool topicRestricted);
	void	setUserLimit(size_t userLimit);

	std::string&	getName();
	std::string&	getTopic();
	std::string&	getKey();
	bool			getInviteOnly();
	bool			getTopicRestricted();
	size_t			getUserLimit();

	/* Channel Creation Support */
//	handleJoin(); // El primer usuario que crea el canal debe ser operador.
};

# include "Channel.tpp"

#endif

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>

class Client;

class Channel
{
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

private:

	std::string       _name;

	std::set<Client*> _members;
	std::set<Client*> _operators;
};

#endif

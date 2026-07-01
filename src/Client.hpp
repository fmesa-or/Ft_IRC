#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client
{
public:

	Client();
	Client(int fd);

	/* Connection */

	int getFd() const;

	/* Identity */

	const std::string& getNickname() const;
	const std::string& getUsername() const;

	void setNickname(const std::string& nickname);
	void setUsername(const std::string& username);

	/* Registration */

	bool hasPassword() const;
	bool hasNickname() const;
	bool hasUsername() const;

	void setHasPassword(bool value);
	void setHasNickname(bool value);
	void setHasUsername(bool value);

	bool isRegistered() const;

	/* Incoming data */

	std::string& getRecvBuffer();

	/* Outgoing data */

	void queueMessage(const std::string& message);

	std::string& sendBuffer();

private:

	int         _fd;

	std::string _nickname;
	std::string _username;

	bool        _has_password;
	bool        _has_nickname;
	bool        _has_username;

	std::string _recv_buffer;
	std::string _send_buffer;
};

#endif

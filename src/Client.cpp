#include "Client.hpp"

Client::Client() :
	_fd(-1),
	_has_password(false),
	_has_nickname(false),
	_has_username(false),
	_registration_completed(false) {}


Client::Client(int fd) :
	_fd(fd),
	_has_password(false),
	_has_nickname(false),
	_has_username(false),
	_registration_completed(false) {}


std::string& Client::getRecvBuffer() {
	return _recv_buffer;
}

int Client::getFd() const {
	return _fd;
}


const std::string& Client::getNickname() const {
	return _nickname;
}

const std::string& Client::getUsername() const {
	return _username;
}


void Client::setNickname(const std::string& nickname) {
	_nickname = nickname;
}

void Client::setUsername(const std::string& username) {
	_username = username;
}


bool Client::hasPassword() const {
	return _has_password;
}

bool Client::hasNickname() const {
	return _has_nickname;
}

bool Client::hasUsername() const {
	return _has_username;
}


bool Client::isRegistered() const {
	if (_has_password && _has_nickname && _has_username) {
		return true;
	} else {
		return false;
	}
}


void Client::setHasPassword(bool value) {
	_has_password = value;
}

void Client::setHasNickname(bool value) {
	_has_nickname = value;
}

void Client::setHasUsername(bool value) {
	_has_username = value;
}

bool Client::getRegistrationCompleted() const {
	return (_registration_completed);
} 

void Client::setRegistrationCompleted(bool value) {
	_registration_completed = value;
}

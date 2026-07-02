#include "Client.hpp"

Client::Client() {}

Client::Client(int fd) : _fd(fd) {}

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

void Client::setHasPassword(bool value) {
	_has_password = value;
}

void Client::setHasNickname(bool value) {
	_has_nickname = value;
}

void Client::setHasUsername(bool value) {
	_has_username = value;
}

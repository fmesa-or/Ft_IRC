#include "Client.hpp"

Client::Client() {}

Client::Client(int fd) : _fd(fd) {}

std::string& Client::getRecvBuffer() {
	return _recv_buffer;
}

int Client::getFd() const {
	return _fd;
}

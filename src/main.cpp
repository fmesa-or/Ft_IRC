#include "Command.hpp"
#include "IRC.hpp"
#include "Server.hpp"
#include <cstring>
#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <cctype>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <sstream>


typedef struct Input_Data {
	long port;
	char *password;
} Input_Data;

bool validateInput(int argc, char **argv, Input_Data *input_data) {
	if (argc != 3) {
		return ERROR("format: ircserv <port> <password>");
	}

	char *port_str = argv[1];
	char *port_str_end = NULL;
	long port = std::strtol(port_str, &port_str_end, 10);

	if (port < 1024 || port > 65535) {
		return ERROR("Only port numbers 1024 - 65535 are valid.");
	}

	if (*port_str_end != '\0') {
		return ERROR("Couldn't parse port argument. Invalid character \'" << *port_str_end << "\' found.");
	}

	char *password = argv[2];
	int i = 0;
	for (; password[i] != '\0'; i++) {
		if (std::isspace(static_cast<int>(password[i]))) {
			return ERROR("Password cannot contain whitespace.");
		}
	}
	if (i == 0) {
		return ERROR("Password cannot be empty.");
	}

	LOG_DEBUG("Successfully parsed input.");

	input_data->port = port;
	input_data->password = password;

	return true;
}

void close_socket_and_exit_with_failure(int socket_fd) {
	close(socket_fd);
	exit(EXIT_FAILURE);
}

std::string getIpStringFromNetwork(const sockaddr_in& socket_address) {
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &socket_address.sin_addr, ip, INET_ADDRSTRLEN);

	uint16_t port = ntohs(socket_address.sin_port);

	std::stringstream address_string_stream;
	address_string_stream << ip << ":" << port;

	return address_string_stream.str();
}

int main(int argc, char **argv) {
	Input_Data input_data;
	bool ok = validateInput(argc, argv, &input_data);
	if (!ok) {
		return EXIT_FAILURE;
	}

	Server server(input_data.port, input_data.password);

	int listening_socket_fd = server.createConfigureAndSetUpListeningSocket();
	server.continuouslyPollSockets(listening_socket_fd);

	return 0;
}

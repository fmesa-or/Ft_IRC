#include "IRC.hpp"
#include "Server.hpp"

#include <cstring>
#include <stdint.h>
#include <cstdlib>
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
#include <csignal>

volatile sig_atomic_t global_signal = 0;

void handleSignal(int signal) {
	global_signal = signal;
}

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

int main(int argc, char **argv) {
	signal(SIGINT, &handleSignal);

	Input_Data input_data;
	bool ok = validateInput(argc, argv, &input_data);
	if (!ok) {
		return EXIT_FAILURE;
	}

	Server server(input_data.port, input_data.password);

	int listening_socket_fd = server.createConfigureAndSetUpListeningSocket();
	server.continuouslyPollSockets(listening_socket_fd);

	server.cleanup();
	return EXIT_FAILURE;
}

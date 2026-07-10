#include <string>
#include <sstream>
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

std::string getIpStringFromNetwork(const sockaddr_in& socket_address) {
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &socket_address.sin_addr, ip, INET_ADDRSTRLEN);

	uint16_t port = ntohs(socket_address.sin_port);

	std::stringstream address_string_stream;
	address_string_stream << ip << ":" << port;

	return address_string_stream.str();
}

void close_socket_and_exit_with_failure(int socket_fd) {
	close(socket_fd);
	exit(EXIT_FAILURE);
}

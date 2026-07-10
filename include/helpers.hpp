#ifndef HELPERS_HPP
# define HELPERS_HPP

# include <string>
# include <arpa/inet.h>

std::string getIpStringFromNetwork(const sockaddr_in& socket_address);
void close_socket_and_exit_with_failure(int socket_fd);

#endif

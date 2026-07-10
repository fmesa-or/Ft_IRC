#ifndef IRC_HPP
# define IRC_HPP

# define BUFFER_SIZE 512
# define MAX_RECV_BUFFER_SIZE 8192

#define TERMINAL_COLOR_RED "\033[31m"
#define TERMINAL_COLOR_GREEN "\033[32m"
#define TERMINAL_COLOR_YELLOW "\033[33m"
#define TERMINAL_COLOR_RESET "\033[0m"

#define ERROR(x) (std::cerr << TERMINAL_COLOR_RED "ERROR: " TERMINAL_COLOR_RESET << x << std::endl, false)
#define LOG_DEBUG(x) (std::cerr << TERMINAL_COLOR_YELLOW "DEBUG: " TERMINAL_COLOR_RESET << x << std::endl)
#define LOG(x) (std::cerr << x << std::endl)

# include <iostream>
# include <cstdlib>

# define TODO() \
	do { \
		std::cerr << "TODO: " << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")\n"; \
		std::abort(); \
	} while (0)

#endif

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <vector>

/**
 * @param name the command name
 * @param params all the parameters for the comand
 */
struct Command
{
	std::string              name;
	std::vector<std::string> params;
};

#endif

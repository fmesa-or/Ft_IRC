#ifndef PARSER_HPP
# define PARSER_HPP

#include "Command.hpp"
#include <string>
#include <vector>

class	Parser {
	private:
		std::vector<std::string>	split(const std::string &str);
		Command						parseLine(const std::string &line);
};

#endif

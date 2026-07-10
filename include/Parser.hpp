#ifndef PARSER_HPP
# define PARSER_HPP

#include "Command.hpp"
#include <string>
#include <vector>

class	Parser {
	public:
		Command						parseLine(const std::string &line);

	private:
		std::vector<std::string>	split(const std::string &str);
};

#endif

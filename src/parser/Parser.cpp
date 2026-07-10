#include "Parser.hpp"
#include "Command.hpp"

std::vector<std::string>	Parser::split(const std::string &line){
	std::vector<std::string>	tokens;
	std::string	work_line = line;

	while (!work_line.empty()) {
		size_t	start = work_line.find_first_not_of(' ');
		if (start == std::string::npos)
			break;
		work_line.erase(0, start);
		size_t	space = work_line.find(' ');
		if (space == std::string::npos) {
			tokens.push_back(work_line);
			break;
		}
		tokens.push_back(work_line.substr(0, space));
		work_line.erase(0, space + 1);
	}
	return tokens;
}

Command	Parser::parseLine(const std::string &line) {
	Command		cmd;
	std::string	work_line = line;
	
	//erase '\r' and '\n'
	if (!work_line.empty() && work_line[work_line.size() - 1] == '\n')
		work_line.erase(work_line.size() - 1);
	if (!work_line.empty() && work_line[work_line.size() - 1] == '\r')
		work_line.erase(work_line.size() - 1);

	// separate trailing part (after ':')
	std::string	left;
	std::string trailing;
	size_t		colon = work_line.find(":");

	 if (colon != std::string::npos){
		left 		= work_line.substr(0, colon);
		trailing	= work_line.substr(colon + 1);
	 }
	 else
	 	left = work_line;
	std::vector<std::string> tokens = split(left);
	if (tokens.empty())
	 	return cmd;

	// command and params
	 cmd.name = tokens[0];
	 for (size_t i = 1; i < tokens.size(); i++)
	 	cmd.params.push_back(tokens[i]);
	 if (!trailing.empty())
	 	cmd.params.push_back(trailing);
	return cmd;
}

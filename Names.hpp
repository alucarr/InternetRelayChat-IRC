#ifndef NAMES_HPP
#define NAMES_HPP

#include "Command.hpp"  // Ensure this includes Commands

class Names : public Command // Inherit from Commands
{
public:
	Names();
	void execute(int client_fd);
	std::string getName() const;
	std::string description() const;
};

#endif

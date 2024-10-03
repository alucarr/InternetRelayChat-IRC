#ifndef PART_HPP
#define PART_HPP

#include "Command.hpp"  // Ensure this includes Commands

class Part : public Command // Inherit from Commands
{
public:
	Part();
	void execute(int client_fd);
	std::string getName() const;
	std::string description() const;
};

#endif

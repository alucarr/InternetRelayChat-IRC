#ifndef KICK_HPP
#define KICK_HPP

#include "Command.hpp"  // Ensure this includes Commands

class Kick : public Command // Inherit from Commands
{
public:
	Kick();
	void execute(int client_fd);
	std::string getName() const;
	std::string description() const;
};

#endif

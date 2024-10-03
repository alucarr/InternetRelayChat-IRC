#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "Command.hpp"  // Ensure this includes Commands

class Privmsg : public Command // Inherit from Commands
{
public:
	Privmsg();
	void execute(int client_fd);
	std::string getName() const;
	std::string description() const;
};

#endif

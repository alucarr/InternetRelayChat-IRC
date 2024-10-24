#ifndef WHO_HPP
#define WHO_HPP

#include "Command.hpp"  // Ensure this includes Commands

class Who : public Command // Inherit from Commands
{
public:
	Who();
	void execute(int client_fd);
	std::string getName() const;
	std::string description() const;
};

#endif

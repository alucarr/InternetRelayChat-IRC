#include "Nick.hpp"

Nick::Nick()
{
}

void Nick::execute(int client_fd)
{
}

std::string Nick::getName() const
{
	return "nick";
}

std::string Nick::description() const
{
	return   "NICK: Sets or changes the user's nickname.";
}

#include "Kick.hpp"

Kick::Kick()
{
}

void Kick::execute(int client_fd)
{
}

std::string Kick::getName() const
{
	return "kick";
}

std::string Kick::description() const
{
	return  "KICK: Removes a user from a specific channel";
}

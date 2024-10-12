#include "Privmsg.hpp"

Privmsg::Privmsg()
{
}

void Privmsg::execute(int client_fd)
{
}

std::string Privmsg::getName() const
{
	return "PRIVMSG";
}

std::string Privmsg::description() const
{
	return  "PRIVMSG: Sends a message to a specific user or channel";
}

#include "Join.hpp"

Join::Join()
{
}

void Join::execute(int client_fd)
{
	
}

std::string Join::getName() const
{
	return "join";
}

std::string Join::description() const
{
	return  "JOIN: Used to add users to the channel";
}

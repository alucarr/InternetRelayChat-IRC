#include "Names.hpp"

Names::Names()
{
}

void Names::execute(int client_fd)
{
}

std::string Names::getName() const
{
	return "NAMES";
}

std::string Names::description() const
{
	return  "NAMES: Shows the list of users in a channel";
}

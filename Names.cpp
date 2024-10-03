#include "Names.hpp"

Names::Names()
{
}

void Names::execute(int client_fd)
{
}

std::string Names::getName() const
{
	return "names";
}

std::string Names::description() const
{
	return RED "Names 	: Shows the list of users in a channel";
}

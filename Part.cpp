#include "Part.hpp"

Part::Part()
{
}

void Part::execute(int client_fd)
{
}

std::string Part::getName() const
{
	return "part";
}

std::string Part::description() const
{
	return RED "Part 	: Allows the user to leave a channel";
}

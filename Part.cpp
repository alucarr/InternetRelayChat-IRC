#include "Part.hpp"

Part::Part()
{
}

void Part::execute(int client_fd)
{
}

std::string Part::getName() const
{
	return "PART";
}

std::string Part::description() const
{
	return  "PART: Allows the user to leave a channel";
}

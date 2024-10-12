#include "Notice.hpp"

Notice::Notice()
{
}

void Notice::execute(int client_fd)
{
}

std::string Notice::getName() const
{
	return "NOTICE";
}

std::string Notice::description() const
{
	return  "NOTICE: Sends a notification message to the target (user or channel)";
}

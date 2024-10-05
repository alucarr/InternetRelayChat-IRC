#include "List.hpp"

List::List()
{
}

void List::execute(int client_fd)
{
}

std::string List::getName() const
{
	return "list";
}

std::string List::description() const
{
	return  "LIST: Shows the list of channels available on the server";
}

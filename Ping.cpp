#include "Ping.hpp"

Ping::Ping()
{
}

void Ping::execute(int client_fd)
{
}

std::string Ping::getName() const
{
	return "ping";
}

std::string Ping::description() const
{
	return BLUE "Ping 	: It pings the server and checks if the connection is live";
}

#include "Ping.hpp"

Ping::Ping()
{
}

void Ping::execute(int client_fd)
{
}

std::string Ping::getName() const
{
	return "PING";
}

std::string Ping::description() const
{
	return  "PING: It pings the server and checks if the connection is live";
}

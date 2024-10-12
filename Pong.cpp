#include "Pong.hpp"

Pong::Pong()
{
}

void Pong::execute(int client_fd)
{
}

std::string Pong::getName() const
{
	return "PONG";
}

std::string Pong::description() const
{
	return  "PONG: It responds to the ping message from the server";
}

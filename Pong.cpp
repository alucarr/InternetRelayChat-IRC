#include "Pong.hpp"

Pong::Pong()
{
}

void Pong::execute(int client_fd)
{
}

std::string Pong::getName() const
{
	return "pong";
}

std::string Pong::description() const
{
	return RED "Pong	: It responds to the ping message from the server";
}

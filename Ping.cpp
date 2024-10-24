#include "Ping.hpp"
#include "Commands.hpp"

Ping::Ping()
{
}

void Ping::execute(int client_fd)
{
	if(_args.size() < 2)
	{
		_server->sendError(client_fd,"Usage: PING/PONG <message> \n");
		return;
	}
	
	Commands *pongcommand = _server->getCommands();
	// // ping aldığında dair mesaj gönder send ile
	pongcommand->commandFinder("PONG " + _args[1], _users);
}

std::string Ping::getName() const
{
	return "PING";
}

std::string Ping::description() const
{
	return  "PING: It pings the server and checks if the connection is live";
}

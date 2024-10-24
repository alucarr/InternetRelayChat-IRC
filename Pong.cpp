#include "Pong.hpp"

Pong::Pong()
{
}

void Pong::execute(int client_fd)
{
	// std::cout << _users->getNickName() << "mustmustumsutmustmustu"<<std::endl; 
	// :AS31S!asdasd@localhost PONG :AS31S
	if(_args.size() < 2)
	{
		_server->sendError(client_fd, "Usage: PING/PONG <message>\r\n");
		return;
	}
	std::string message = ":" + _users->getNickName()+ "!"+ _users->getName() + "@" + _server->getHost()+ " PONG :" + _args[1] + "\r\n";
	_server->sendMessage(client_fd, message);
	std::cout << message << std::endl;
	_args.erase(_args.begin(), _args.end());
}

std::string Pong::getName() const
{
	return "PONG";
}

std::string Pong::description() const
{
	return  "PONG: It responds to the ping message from the server";
}

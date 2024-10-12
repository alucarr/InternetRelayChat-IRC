#include "Quit.hpp"

Quit::Quit()
{
}

void Quit::execute(int client_fd)
{
	// _server->sendMessage(client_fd,":" + _users->getNickName()+"!"+_users->getName()+"@"+ _server->getHost()+"\r\n");
	_server->removeUserAndFd(client_fd);
}

std::string Quit::getName() const
{
	return "QUIT";
}

std::string Quit::description() const
{
	return  "QUIT: Allows you to exit the program";
}

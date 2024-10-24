#include "Quit.hpp"
#include <iostream>

Quit::Quit()
{
}

void Quit::execute(int client_fd)
{
	Commands *partcommand = _server->getCommands();
	std::vector<std::string> userChannels = _users->getChannelName();
	for (std::vector<std::string>::iterator it = userChannels.begin(); it != userChannels.end(); ++it)
	{
		if((*it).data() && (*it).data()[0] != '\0') //stringe çevirilip bakılabilir char* a bakarak yapıyorum.
		{
			//PARTA gönderip kanallardan çıkıcak sonra quit
			partcommand->commandFinder("PART " + *it, _users);
		}
	}
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

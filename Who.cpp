#include "Who.hpp"

Who::Who()
{
}

void Who::execute(int client_fd)
{
	bool channelExists = 0;
	//server get users ile kullanıcı alıp tüm kullanıcılar için kanal ismi eşleşiyorsa eşleşen kullanıcıları yazdır.
	if (_args.size() == 2)
	{
		std::vector<User *> users = _server->getUsers();
		for (std::vector<User *>::iterator us_it = users.begin(); us_it != users.end(); us_it++)
		{
			if(_args[1][0] == '#')
			{
				std::vector<std::string> userChannels = (*us_it)->getChannelName();
				for (std::vector<std::string>::iterator it = userChannels.begin(); it != userChannels.end(); ++it)
				{
					std::cout << (*it).data() << _args[1] << std::endl;
					if((*it).data()[0] != '\0' && (*it).data() && (*it) == _args[1])
					{
						_server->sendMessage(client_fd, "Nick: " + (*us_it)->getNickName() + "\n");
						channelExists = 1;
					}
				}
			}
			else if(_args[1] == (*us_it)->getNickName())
			{
				_server->sendMessage(client_fd, "INFO: Nick: " + (*us_it)->getNickName() + " Username: " + (*us_it)->getName());
				break;
			}
			else if((us_it+1) == users.end())
			{
				_server->sendError(client_fd, "User not found\n");
				return;
			}
			if(!channelExists && us_it+1 == users.end())
			{
				_server->sendError(client_fd, "Channel not found\n");
				return;
			}
		}
		_server->sendMessage(client_fd,": End of WHO list\n");
	}
	else
		_server->sendError(client_fd, "Usage: WHO <channel> / <user>\n");
}


std::string Who::getName() const
{
	return "WHO";
}

std::string Who::description() const
{
	return "WHO: Allows you to see the users in the channel";
}
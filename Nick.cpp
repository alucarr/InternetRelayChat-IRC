#include "Nick.hpp"

Nick::Nick()
{
}

void Nick::execute(int client_fd)
{
	std::vector<User*> users = _server->getUsers();
	std::vector<Channel*> channels = _server->getChannel();
	if(_args.size() == 2)
	{
		for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it)
		{
		    if ((*it)->getNickName() == _args[1])
		    {
		        _server->sendMessage(client_fd, "Error : Nickname is already in use \n");
		        return;
		    }
		}

		if (_users->getClientfd() == client_fd && !_args[1].empty())
		{
			std::string oldNick = _users->getNickName();
			for(std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
			{
				if(oldNick == (*it)->getAdminName())
				{
					(*it)->setAdminName(_args[1]);
				}
			}
		    _users->setNickName(_args[1]);
			std::string name = ":" + oldNick + "!" + _users->getName() + _server->getHost() + " NICK " + ":" + _args[1] + "\n";
			_server->sendMessage(client_fd, name);
			std::vector<std::string> channel = _users->getChannelName();
			for(std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it)
			{

				for (size_t i = 0; i < channel.size(); ++i)
				{
					if(_users->getNickName() != (*it)->getNickName())
					{
						std::string message = ":" + oldNick + " NICK " + _args[1] + "\n";
						_server->sendMessage((*it)->getClientfd(), message);
					}
				}
			}
		}
	}
	else
		_server->sendError(client_fd, "Usage: /NICK <nickname>\n");
}

std::string Nick::getName() const
{
	return "NICK";
}

std::string Nick::description() const
{
	return   "NICK: Sets or changes the user's nickname.";
}

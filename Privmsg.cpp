#include "Privmsg.hpp"

Privmsg::Privmsg()
{
}

void Privmsg::execute(int client_fd)
{
	for (size_t i = 0; i < _args.size(); i++)
	{
		std::cout<< i << " " << _args[i] << std::endl;
	}
	
    if (_args.size() >= 2)
    {
		std::string mes = ":" + _users->getNickName() + " PRIVMSG " + _args[1] + " : ";
		if (_args.size() >= 3 && _args[2].front() != ':')
			mes+= ":";
		if (_args.size() == 2 && _args[1].back() != ':')
			mes+= ":";

		for (size_t i = 2; i < _args.size(); i++)
		{
			if(i != 2)
				mes += " ";
			mes += _args[i];
		}
		std::vector<User*> users = _server->getUsers();
		std::vector<Channel*> channel = _server->getChannel();
        if (_args[1][0] == '#')
        {
            std::string channelName = _args[1];
			std::vector<std::string> nemo = _users->getChannelName();
			int d = 0;
			for(std::vector<std::string>::iterator it = nemo.begin(); it != nemo.end(); ++it)
			{
				if((*it).data() != _args[1])
					d++;
			}
			if(d == nemo.size())
			{
				_server->sendError(client_fd, " You cannot send messages to this channel\n");
				return;
			}
 			for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it)
 			{
 			    std::vector<std::string> userChannels = (*it)->getChannelName();

 			    for (size_t k = 0; k < userChannels.size(); k++) 
 			    {
 			        if (userChannels[k] == channelName && (*it)->getClientfd() != client_fd) 
 			        {
						std::string userName = (*it)->getNickName();
 			            _server->sendMessage((*it)->getClientfd(), mes + "\n");
 			        }
 			    }
 			}
        }
        else
        {
            for (size_t i = 0; i < users.size(); i++)
            {
                if (users[i]->getNickName() == _args[1] && _users->getNickName() != _args[1])
                {
                    _server->sendMessage(users[i]->getClientfd(), mes + "\n");
                    return;
                }
                if(_users->getNickName() == _args[1])
                {
                    _server->sendError(client_fd, "Please enter a valid user\n");
                    return;
                }
                if (i == users.size() - 1)
                {
                    _server->sendError(client_fd,  " User not found\n");
                }
            }
        }
    }
    else
    {
        _server->sendError(client_fd, " Argument missing \n");
    }
	_args.clear();
}


std::string Privmsg::getName() const
{
	return "PRIVMSG";
}

std::string Privmsg::description() const
{
	return  "PRIVMSG: Sends a message to a specific user or channel";
}
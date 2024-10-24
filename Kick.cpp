#include "Kick.hpp"

Kick::Kick()
{
}

void Kick::execute(int client_fd)
{
	bool flag = false;
	if(_args.size() > 2)
	{
		std::string mes;
		for (size_t i = 3; i < _args.size(); i++)
		{
			mes += " ";
			mes += _args[i];
		}
		std::vector<Channel*> channels = _server->getChannel();
		std::vector<User*> allUsers = _server->getUsers();
		for(std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if(_users->getNickName() == (*it)->getAdminName() && (*it)->getChannelName() == _args[1])
			{
				for (std::vector<User*>::iterator user_it = allUsers.begin(); user_it != allUsers.end(); ++user_it)
        		{
            		if ((*user_it)->getNickName() == _args[2])
            		{
            		    std::vector<std::string> userChannels = (*user_it)->getChannelName();

            		    for (std::vector<std::string>::iterator ch_it = userChannels.begin(); ch_it != userChannels.end(); ++ch_it)
            		    {
            		        if (*ch_it == _args[1])
            		        {
								flag = true;
            		            (*user_it)->removeUser(_args[1]); 
            		            _server->sendMessage(client_fd , ":" + _users->getNickName() + "!" + _users->getName() + "@" + _server->getHost() + " KICK " + _args[1] + " " + _args[2] + " :" + mes +"\n\r");
								for (std::vector<User*>::iterator it = allUsers.begin(); it != allUsers.end(); ++it)
								{
									if(client_fd != (*it)->getClientfd())
									{
										std::cout << "naber mudur ---------" << std::endl;
										_server->sendMessage((*it)->getClientfd() , ":" + _users->getNickName() + "!" + _users->getName() + "@" + _server->getHost() + " KICK " + _args[1] + " " + _args[2] + " :" + mes +"\n\r");
									}
								}
            		        }
							break;
            		    }
            		}
					else if((user_it + 1 ) == allUsers.end() && !flag)
						_server->sendError(client_fd, " User not found\n");
        		}
			}
			else if(_users->getNickName() != (*it)->getAdminName() && (*it)->getChannelName() == _args[1])
				_server->sendError(client_fd, " Only admin can use this command\n");
		}
	}
	else
		_server->sendError(client_fd, "Usage: </kick> <#Channel Name> <User>\n");
}

std::string Kick::getName() const
{
	return "KICK";
}

std::string Kick::description() const
{
	return  "KICK: Removes a user from a specific channel";
}

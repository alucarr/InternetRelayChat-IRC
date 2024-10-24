#include "Part.hpp"

Part::Part()
{
}

void Part::execute(int client_fd)
{
    if (_args.size() == 2 || _args.size() == 7)
    {
		bool flag = false;
        std::vector<User*> allUsers = _server->getUsers();
        for (std::vector<User*>::iterator user_it = allUsers.begin(); user_it != allUsers.end(); ++user_it)
        {
            if ((*user_it)->getClientfd() == client_fd)
            {
                std::vector<std::string> userChannels = (*user_it)->getChannelName();
                
                for (std::vector<std::string>::iterator ch_it = userChannels.begin(); ch_it != userChannels.end(); ++ch_it)
                {
                    if (*ch_it == _args[1])
                    {
						flag = true;
                        (*user_it)->removeUser(_args[1]); 
                        _server->sendMessage(client_fd , ":" + (*user_it)->getNickName() + "!" + (*user_it)->getName() + "@" + _server->getHost() + " PART " + _args[1] + "\n\r");
						for (std::vector<User*>::iterator it = allUsers.begin(); it != allUsers.end(); ++it) 
						{
							if(client_fd != (*it)->getClientfd())
								_server->sendMessage((*it)->getClientfd() , ":" + (*user_it)->getNickName() + "!" + (*user_it)->getName() + "@" + _server->getHost() + " PART " + _args[1] + "\n\r");
    					}
                        break;
                    }
                }
            }
        }
		if (!flag)
        {
            std::string errorMessage = " Channel " + _args[1] + " not found.\n";
            _server->sendError(client_fd, errorMessage);
            return;
        }
    }
	else
		_server->sendError(client_fd, "Usage: /PART #<channel name>\n");
}

std::string Part::getName() const
{
	return "PART";
}

std::string Part::description() const
{
	return  "PART: Allows the user to leave a channel";
}

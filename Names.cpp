#include "Names.hpp"

Names::Names()
{
}

void Names::execute(int client_fd)
{
	if(_args.size() == 2)
	{
	    std::vector<Channel*> channels = _server->getChannel();
	    std::vector<User*> allUsers = _server->getUsers();
	
		bool flag = false;
	    for(std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	    {
	        if((*it)->getChannelName() == _args[1])
	        {
				flag = true;
	            _server->sendMessage(client_fd, "Channel " + (*it)->getChannelName() + " Users:\n");
	            for(std::vector<User*>::iterator user_it = allUsers.begin(); user_it != allUsers.end(); ++user_it)
	            {
	                std::vector<std::string> userChannels = (*user_it)->getChannelName();
	                for(std::vector<std::string>::iterator ch_it = userChannels.begin(); ch_it != userChannels.end(); ++ch_it)
	                {
	                    if(*ch_it == (*it)->getChannelName())
	                        _server->sendMessage(client_fd, (*user_it)->getNickName() + "\n");
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
		 _server->sendError(client_fd, "Usage: /names <channel>\n");
}

std::string Names::getName() const
{
	return "names";
}

std::string Names::description() const
{
	return  "NAMES: Shows the list of users in a channel";
}

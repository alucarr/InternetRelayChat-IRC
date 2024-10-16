#include "Names.hpp"

Names::Names()
{
}

void Names::execute(int client_fd)
{
	if(_args.size() <= 2)
	{
	    std::vector<Channel*> channels = _server->getChannel();
	    std::vector<User*> allUsers = _server->getUsers();
	
	    if(_args.size() > 1 && !_args[1].empty())
	    {
	        for(std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	        {
	            if((*it)->getChannelName() == _args[1])
	            {
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
	    }
	    else
	    {
	        for(std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	        {
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
	}

}

std::string Names::getName() const
{
	return "NAMES";
}

std::string Names::description() const
{
	return  "NAMES: Shows the list of users in a channel";
}

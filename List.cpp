#include "List.hpp"

List::List()
{
}

void List::execute(int client_fd)
{
	if (_args.size() <= 2)
	{
	    std::vector<Channel*> channels = _server->getChannel();
	    std::vector<User*> allUsers = _server->getUsers();
	
	    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	    {
	        int userCount = 0;
	        for (std::vector<User*>::iterator user_it = allUsers.begin(); user_it != allUsers.end(); ++user_it)
	        {
	            std::vector<std::string> userChannels = (*user_it)->getChannelName();
	            for (std::vector<std::string>::iterator ch_it = userChannels.begin(); ch_it != userChannels.end(); ++ch_it)
	            {
				
	                if (*ch_it == (*it)->getChannelName())
	                {
	                    userCount++;
	                    break;
	                }
	            }
	        }
	        std::string message = "Channel " + (*it)->getChannelName() + " Users: " + std::to_string(userCount) + "\n";
	        _server->sendMessage(client_fd, message);
	    }
	}
	else
	{
	    _server->sendMessage(client_fd, "Error: hatalı giriş \n");
	}
}

std::string List::getName() const
{
	return "LIST";
}

std::string List::description() const
{
	return  "LIST: Shows the list of channels available on the server";
}

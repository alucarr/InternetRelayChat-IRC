#include "Topic.hpp"

Topic::Topic()
{
}

void Topic::execute(int client_fd)
{
	std::string mes;
    for (size_t i = 2; i < _args.size(); i++)
    {
        mes += " ";
        mes += _args[i];
    }
	if (_args.size() > 2)
	{
		bool flag = false;
		std::vector<Channel*> channels = _server->getChannel();
		std::vector<User*> users = _server->getUsers();
		for(std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if((*it)->getChannelName() == _args[1])
			{
				flag = true;
				(*it)->setTopicName(_args[2]);
			}
		}
		if (!flag)
        {
            std::string errorMessage = "Error: Channel " + _args[1] + " not found.\n";
            _server->sendError(client_fd, errorMessage);
            return;
        }
		for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it)
 		{
 		    std::vector<std::string> userChannels = (*it)->getChannelName();
 		    for (size_t k = 0; k < userChannels.size(); k++) 
 		    {
 		        if (userChannels[k] == _args[1]) 
 		        {
					std::string message = ":" + _users->getNickName() + " TOPIC " + _args[1] + mes + "\n";
 		            _server->sendMessage((*it)->getClientfd(), message);
 		        }
 		    }
 		}
	}
	else
		_server->sendError(client_fd, "Usage: /topic <channel name> <Topic Title>\n");
}

std::string Topic::getName() const
{
	return "TOPIC";
}

std::string Topic::description() const
{
	return  "Topic: Sets or shows the topic of a channel";
}

#include "Join.hpp"

Join::Join()
{
}

void Join::execute(int client_fd)
{
	if(_args.size() != 2)
	{
		_server->sendError(client_fd,"Usage: /join #Channel name.\n");
		return;
	}
	std::vector<std::string> userChannels = _users->getChannelName();
	for (std::vector<std::string>::iterator it = userChannels.begin(); it != userChannels.end(); ++it)
	{
		if(*it == _args[1])
		{
			_server->sendError(client_fd,"You're already joined the channel.\n");
			return;
		}
	}
	if (_args[1][0] != '#')
	{
		_server->sendError(client_fd,"Usage: /join #Channel name.\n");
		return;
	}
	Channel *channel = _server->getChannel(_args[1]);

    if(!channel)
    {
        channel = new Channel(_args[1]);
        _server->createChannel(channel);
		channel->setAdminName(_users->getNickName());
		_users->setChannelName(_args[1]);
		_server->addToChannel(channel, _users,_args[1],_users->getClientfd());
    }
	else
	{
		_users->setChannelName(_args[1]);//new 2 eklencek
		_server->addToChannel(channel, _users,_args[1],_users->getClientfd());
	}
}

std::string Join::getName() const
{
	return "JOIN";
}

std::string Join::description() const
{
	return  "JOIN: Used to add users to the channel";
}

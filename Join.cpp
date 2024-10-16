#include "Join.hpp"

Join::Join()
{
}

void Join::execute(int client_fd)
{
	for (size_t i = 0; i < _args.size(); i++)
	{
		if(i>2)
		{
			_server->sendError(client_fd,"Too many parameters for join. Please enter '/join' 'channel name' 'password' .\n");
			return;
		}
	}
	// if (_args[1][0] == '#')
	// 	_args[1] = _args[1].substr(1);
	
	
	Channel *channel = _server->getChannel(_args[1]);
    if(!channel)
    {
		std::cout << "SŞUGDFPISUDFGUISDFPIUGSDGIPUFVSGIUPDFIGPUSIUFGSDIPGYFGIPYSDFIGPUYSDIFGUSOGUPIDFPIGUSDFIPGUASIDGPUFAISGUYFIPGUYDS"<< std::endl;
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

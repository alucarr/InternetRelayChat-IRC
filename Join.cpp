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
	

	
	_server->setChannel(_args);
	// _server->addTo
	std::string str = ":" + _users->getNickName() + "!" + _users->getName() + "@"+ _server->getHost() + " JOIN " + _args[1]+ "\r\n";
	_server->sendMessage(_users->getClientfd(), str);
	//TODO: MOD VERME YERİ
	//str = "MODE " + _args[1] + " +o " + _users->getNickName() + "/r/n";
	//_server->sendMessage(_users->getClientfd(), str);

}

std::string Join::getName() const
{
	return "JOIN";
}

std::string Join::description() const
{
	return  "JOIN: Used to add users to the channel";
}

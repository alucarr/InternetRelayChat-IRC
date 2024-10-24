#include "Command.hpp"

Command::Command() : _server(NULL), _users(NULL) {}

void Command::setServer(Server *server)
{
	_server = server;
}

void Command::setUser(User *users)
{
	_users = users;
}

void Command::setUserArgs(std::vector<std::string> args)
{
	if (!_args.empty())
		_args.clear();

	_args = args;
}

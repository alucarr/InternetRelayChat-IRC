#include "Help.hpp"
#include "Utils.hpp"
#include "Commands.hpp"
#include "Server.hpp"

Help::Help()
{
}

void Help::execute(int client_fd)
{
	Commands cmdManager(_server);

	std::vector<Command*> commands = cmdManager.getCommends();
	for(size_t i = 0; i < commands.size(); i++)
	{
		_server->sendMessage(client_fd, commands[i]->description()+"\n");
	}
}

std::string Help::getName() const
{
	return "HELP";
}

std::string Help::description() const
{
	return  "HELP: Shows descriptions of commands" ;
}
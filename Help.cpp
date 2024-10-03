#include "Help.hpp"
#include "Utils.hpp"
#include "Commands.hpp"
#include "Server.hpp"

Help::Help()
{
}

void Help::execute(int client_fd)
{
	Commands cmdManager;

	std::vector<Command*> commands = cmdManager.getCommends();
	for(size_t i = 0; i < commands.size(); i++)
	{
		send(client_fd, commands[i]->description().c_str(),commands[i]->description().length(), 0);
		send(client_fd, "\n", 1, 0);
	}
}

std::string Help::getName() const
{
	return "help";
}

std::string Help::description() const
{
	return RED "Help 	: Shows descriptions of commands" RESET;
}
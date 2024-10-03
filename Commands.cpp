#include "Commands.hpp"  // Include Commands header
#include "Command.hpp"
#include "Help.hpp"      // Include Help header to recognize Help class
#include "Quit.hpp"
#include "Join.hpp"
#include "Nick.hpp"
#include "Part.hpp"
#include "Privmsg.hpp"
#include "Ping.hpp"
#include "Pong.hpp"
#include "Notice.hpp"
#include "Topic.hpp"
#include "Kick.hpp"
#include "List.hpp"
#include "Names.hpp"
#include "User.hpp"
#include <sys/socket.h>

// Constructor implementation
Commands::Commands() {
    _commands.push_back(new Help());  // Add Help command in constructor
	_commands.push_back(new Quit());
	_commands.push_back(new Join());
	_commands.push_back(new Nick());
	_commands.push_back(new Part());
	_commands.push_back(new Privmsg());
	_commands.push_back(new Notice());
	_commands.push_back(new Ping());
	_commands.push_back(new Pong());
	_commands.push_back(new Topic());
	_commands.push_back(new Kick());
	_commands.push_back(new List());
	_commands.push_back(new Names());
}

// Destructor implementation
Commands::~Commands() {
    for (size_t i = 0; i < _commands.size(); ++i) {
        delete _commands[i];  // Clean up allocated memory
    }
}

Command* Commands::commandFinder(const std::string &cmdName, User *it)
{
	for(size_t i = 0; i < _commands.size(); i++)
	{
		if(_commands[i]->getName() == cmdName)
		{
			// fdyi ya userdan ya da direk clientfd olarak parametre olarak alacak bir fonksiyon gelecek buraya sebebi bütün commandlerin executuna göndermemek için
			_commands[i]->execute((*it).getClientfd());
			break;
		}
		if(i == _commands.size() -1&& cmdName != "")
		{
			std::string str = "command not found\n";
			send((*it).getClientfd(), str.c_str(), str.length(), 0);
		}
	}

	return 0;
}

std::vector<Command *> Commands::getCommends() const
{
	return _commands;
}

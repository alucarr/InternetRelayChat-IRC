#include "Server.hpp"
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
#include <sstream>

// Constructor implementation
Commands::Commands(Server* server) : _server(server) { 
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
	std::vector<std::string> args = setArgs(cmdName);
	for(size_t i = 0; i < _commands.size(); i++)
	{
		std::cout << "burada mı" << std::endl;
		if(cmdName != "" && _commands[i]->getName() == args[0])
		{

			_commands[i]->setServer(_server);
			_commands[i]->setUser(it);
			_commands[i]->setUserArgs(args);
			std::cout << "args içi --------------\n";

			for(std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
				std::cout <<(*it).data() << std::endl;
			}
			std::cout << " args sonu--------------\n";
			
			// fdyi ya userdan ya da direk clientfd olarak parametre olarak alacak bir fonksiyon gelecek buraya sebebi bütün commandlerin executuna göndermemek için
			_commands[i]->execute((*it).getClientfd());
			break;
		}
		if(cmdName != "" && i == _commands.size() -1)
			_server->sendError((*it).getClientfd(),"Command not found\n");
	}

	return 0;
}

std::vector<Command *> Commands::getCommends() const
{
	return _commands;
}

std::vector<std::string> Commands::setArgs(const std::string &msg) {
    std::vector<std::string> cpyArg;
    std::stringstream ss(msg);
    std::string arg;

    // Mesajı boşluklara göre ayır
    while (std::getline(ss, arg, ' ')) {
        if (!arg.empty()) { // Boş argümanları ekleme
            cpyArg.push_back(arg);
        }
    }

    return cpyArg;
}


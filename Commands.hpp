#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Utils.hpp"  
#include <vector>
#include "User.hpp"
#include "Server.hpp"  // Include Server.hpp to use Server class

class Command;  // Forward declaration of Command

class Commands {
private:
    Server* _server;
    std::vector<Command*> _commands;  // Vector to hold command pointers
public:
    Commands(Server* server);  // Constructor
    virtual ~Commands();  // Destructor declaration
    Command* commandFinder(const std::string& cmdName, User* it);
    std::vector<Command*> getCommends() const;
    std::vector<std::string> setArgs(const std::string &msg);
};

#endif // COMMANDS_HPP

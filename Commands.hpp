#if !defined(COMMANDS_HPP)
#define COMMANDS_HPP

#include "Utils.hpp"  // Assuming this doesn't include Help or Commands
#include <vector>
#include "User.hpp"

class Command;

class Commands  // Forward declaration of Commands
{
private:
    std::vector<Command*> _commands;  // Vector to hold command pointers
public:
    Commands();  // Constructor
    virtual ~Commands();  // Destructor declaration
	Command* commandFinder(const std::string& cmdName, User* it);
	std::vector<Command*> getCommends() const;
};

#endif
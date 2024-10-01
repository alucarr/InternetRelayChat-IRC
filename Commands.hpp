#if !defined(COMMANDS_HPP)
#define COMMANDS_HPP

#include "Utils.hpp"  // Assuming this doesn't include Help or Commands
#include <vector>

class Commands  // Forward declaration of Commands
{
private:
    std::vector<Commands*> _commands;  // Vector to hold command pointers
public:
    Commands();  // Constructor
    virtual ~Commands();  // Destructor declaration
};

#endif

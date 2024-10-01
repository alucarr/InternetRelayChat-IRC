#include "Commands.hpp"  // Include Commands header
#include "Help.hpp"      // Include Help header to recognize Help class

// Constructor implementation
Commands::Commands() {
    _commands.push_back(new Help());  // Add Help command in constructor
}

// Destructor implementation
Commands::~Commands() {
    for (size_t i = 0; i < _commands.size(); ++i) {
        delete _commands[i];  // Clean up allocated memory
    }
}

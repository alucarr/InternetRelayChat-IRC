#include "Server.hpp"
int main(int argc, char **argv) {
    try
    {
        if (argc != 3)
        {
            std::cerr << "Please enter 3 arguments: <program name>, <port>, <password>." << std::endl;
            return EXIT_FAILURE; 
        }
        else
        {
            Server ircServer("127.0.0.1", argv[1], argv[2]);

            ircServer.start();
        }
        
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

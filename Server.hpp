
#define MAX_CONNECTIONS 42
#if !defined(SERVER_HPP)
#define SERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <fcntl.h>
#include <vector>
#include <poll.h>
#include <stdexcept>
#include "User.hpp"
#include "Commands.hpp"

class Server
{
private:
  std::string   _host;
  std::string	_port;
  std::string	_password;
  int					_serverSocket;
  std::vector<struct pollfd> _pollfds; //giriş çıkış ve fd olaylarını yönetmek için kull. poll fd veri türüdür.
  std::vector<User *> _users;
    int                 setupSocket();          

public:
    //Server(/* args */);
    Server(const std::string host, const std::string port, const std::string password);
    ~Server();

    void start();
    void addUser(int client_fd,char *host, int port);
    void handleEvents();
};


#endif // SERVER_HPP


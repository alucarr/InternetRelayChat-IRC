#ifndef SERVER_HPP
#define SERVER_HPP
#define MAX_CONNECTIONS 42

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
#include <ctype.h>
#include <cctype>
#include <string>
#include "User.hpp"
#include "Channel.hpp"

class Commands;  // Forward declaration of Commands

class Server {
private:
    std::string _host;
    std::string _port;
    std::string _password;
    int _serverSocket;
    std::vector<struct pollfd> _pollfds; // Poll file descriptors for I/O events
    std::vector<User*> _users;
    std::vector<Channel*> _channel;
    Commands* _commands;  // Pointer to Commands

    int setupSocket();          

public:
    Server(const std::string host, const std::string port, const std::string password);
    ~Server();

    void forRegister(std::string &message, int clientSock, User *us);
    void start();
    void addUser(int client_fd, char *host, int port);
    void handleEvents();
    void sendError(int clientSock, const std::string &message);
    bool isUserNameTaken(const std::string &nickname);
    bool splitMessage(const std::string &message, std::string &part1, std::string &part2, std::string &part3);
    void forRegisterFromClient(std::string &message, int clientSock, User *us);
    void removeUserAndFd(int client_fd);
    void sendMessage(int clientSock, const std::string &message);
    std::string getHost();
    Channel* getChannel(std::string chname);
    void createChannel(Channel* channel);
    void addToChannel(Channel *channel, User *users,std::string & chname,int clfd);
	std::vector<User*> getUsers();
	std::vector<Channel*> getChannel();
	int getServerSocket() {return _serverSocket;}

};

std::string trim(const std::string &s);
bool isOnlyWhitespace(const std::string &str);

#endif // SERVER_HPP

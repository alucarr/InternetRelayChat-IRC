#include "Server.hpp"
#include "Commands.hpp"
#include <algorithm>

void printSocketInfo(int sock_fd) {
    sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    
    if (getsockname(sock_fd, (sockaddr *)&addr, &addrlen) == -1) {
        std::cerr << "Error getting socket name: " << strerror(errno) << std::endl;
        return;
    }

    std::cout << "Socket is bound to IP: " << inet_ntoa(addr.sin_addr)
              << " on port: " << ntohs(addr.sin_port) << std::endl;
}


int Server::setupSocket()
{
    addrinfo hints, *serverInfo, *cp;
    int sock_fd;
    int optval = 1;

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(_host.c_str(), _port.c_str(), &hints, &serverInfo) != 0)
        throw std::runtime_error("Error while getting address info!");

    for (cp = serverInfo; cp != NULL; cp = cp->ai_next)
    {
        sock_fd = socket(cp->ai_family, cp->ai_socktype, cp->ai_protocol);
        if (sock_fd < 0) {
            continue;
        }

        if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0) {
            close(sock_fd);
      freeaddrinfo(serverInfo);
            throw std::runtime_error("Error while setting socket options!");
        }

        if (bind(sock_fd, cp->ai_addr, cp->ai_addrlen) < 0) {
            close(sock_fd);
            continue;
        }
        printSocketInfo(sock_fd);
        break;
    }

    freeaddrinfo(serverInfo);

    if (cp == NULL)
        throw std::runtime_error("Failed to bind the socket!");

    if (listen(sock_fd, MAX_CONNECTIONS) < 0)
        throw std::runtime_error("Error while listening on the socket!");

    return sock_fd;

}

Server::Server(const std::string host, const std::string port, const std::string passwd)
{
    _host = host;
    _port = port;
    _password = passwd;
    _serverSocket = setupSocket();
}

Server::~Server()
{
}

bool isOnlyWhitespace(const std::string& str) {
    return str.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
}

void Server::forRegisterFromClient(std::string &message, int clientSock, User *us) {
    size_t pos, lastPos;
    std::string charactersToFind = "\r\n"; // Satır sonu karakterleri

    
    if (message.find("PASS") == 0) {
        lastPos = message.find_first_of(charactersToFind);
        std::string password = message.substr(6, lastPos - 6);
        
        if (_password != password) {
            sendError(clientSock, "Incorrect password! Please try again.\nEnter: <Password>, <Name>, <Nickname>\n");
            return;
        }
        else
        {
                if ((pos = message.find("NICK "))) {
                    lastPos = message.find(charactersToFind, pos);
                    std::string nickname = message.substr(pos + 5, lastPos - (pos + 5)); // Düzeltildi

                    if (isUserNameTaken(nickname)) {
                        sendError(clientSock, "Nickname is already taken! Please try another one.\nEnter: <Password>, <Name>, <Nickname>\n");
                        return;
                    }
                    us->setNickName(nickname);
                }
                if ((pos = message.find("USER "))) {

                    lastPos = message.find(' ', pos + 5);
                    std::string name = message.substr(pos + 5, lastPos - (pos + 5));
                    us->setName(name);
                } 
                if (!us->getName().empty())
                {
                    std::string str = ":" + _host + " 001 " + us->getNickName() + " :Welcome to the Internet Relay Network " \
                                    + us->getNickName() +"!"+ us->getName() + "@" +_host+"\r\n";
                    sendMessage(clientSock, str);
                    us->setRegister(true);
                    std::cout << "name: " << us->getName() << " nickname: " << us->getNickName() << " is Register: " << us->didRegister() << std::endl;
                }
                else
                    sendError(clientSock,"Register error.");

        }
    } 
}

void Server::removeUserAndFd(int client_fd)
{
    for (std::vector<pollfd>::iterator poll_it = _pollfds.begin(); poll_it != _pollfds.end(); ++poll_it)
    {
        if (poll_it->fd == client_fd)
        {
            close((*poll_it).fd);
			_pollfds.erase(poll_it);
			break ;

        }
    }
    for(std::vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
    {

        if (client_fd == (*it)->getClientfd())
        {
            delete (*it);
            _users.erase(it);
            break ;
        }
    }

}


void Server::addToChannel(Channel *channel, User *users, std::string &chname, int clfd) {

    std::string message = ":" + users->getNickName() + "!" + users->getName() + "@" + getHost() + " JOIN " + channel->getChannelName() + "\r\n";

    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
        User* user = *it;
        std::vector<std::string> userChannels = user->getChannelName();
        
        if (std::find(userChannels.begin(), userChannels.end(), chname) != userChannels.end()) {
            sendMessage(user->getClientfd(), message);
        }
    }

    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
        User* user = *it;
        std::vector<std::string> userChannels = user->getChannelName();

        if (std::find(userChannels.begin(), userChannels.end(), chname) != userChannels.end() && user->getClientfd() != clfd) {
            sendMessage(clfd, ":" + user->getNickName() + "!" + user->getName() + "@" + getHost() + " JOIN " + channel->getChannelName() + "\r\n");
        } else if (std::find(userChannels.begin(), userChannels.end(), chname) != userChannels.end() && user->getNickName() == channel->getAdminName()) {
            std::string str = "MODE " + chname + " +o " + channel->getAdminName() + "\r\n";
            sendMessage(user->getClientfd(), str);
        }
    }

    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
        User* user = *it;

        if (user->getNickName() != channel->getAdminName() && clfd == user->getClientfd()) {
            std::string str = "MODE " + chname + " +o " + channel->getAdminName() + "\r\n";
            sendMessage(user->getClientfd(), str);
        }
    }

    //userın içinde bulunduğu kanalları yazdırma for u
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
    User* user = *it;
    std::vector<std::string> userChannels = user->getChannelName();
    
    for (std::vector<std::string>::iterator chanIt = userChannels.begin(); chanIt != userChannels.end(); ++chanIt) {
        std::cout << "Kullanıcı: " << user->getNickName() << ", Kanal: " << *chanIt << std::endl;
    }
}

}

Channel* Server::getChannel(std::string chname)
{
    for(std::vector<Channel *>::iterator it = _channel.begin(); it != _channel.end(); ++it)
    {
        if(chname == (*it)->getChannelName())
            return *it;
    }
    return NULL;
}
void Server::createChannel(Channel *channel)
{
    _channel.push_back(channel);
}

std::string Server::getHost()
{
	return _host;
}

void Server::forRegister(std::string &message, int clientSock, User *us) {
    std::string part1, part2, part3;
    
    size_t argCount = std::count(message.begin(), message.end(), ' ') + 1;
    if (message.find("\r\n") != std::string::npos)
    {
        return; //TODO: burayı başktaki CAP LS için kullandık ama ben buraya şerh düşüyorum
    }
 
    if (argCount > 3) {
        sendError(clientSock, "Too many arguments! Please enter exactly 3\nEnter: <Password>, <Name>, <Nickname>\n");
        return;
    }
    if (!splitMessage(message, part1, part2, part3)) {
        sendError(clientSock, "Please fill in entry information!!\nEnter: <Password>, <Name>, <Nickname>\n");
        return;
    }
    
    if (_password != part1) {
        sendError(clientSock, "password is incorrect!!\nEnter: <Password>, <Name>, <Nickname>\n");
        return;
    }

    if (!part3.empty() && isUserNameTaken(part3)) {
        sendError(clientSock, "This nickname is already in use!\nEnter: <Password>, <Name>, <Nickname>\n");
        return;
    }

    us->setName(part2);
    us->setNickName(part3);
    std::string str = ":" + _host + " 001 " + us->getNickName() + " :Welcome to the Internet Relay Network " \
        + us->getNickName() +"!"+ us->getName() + "@" +_host+"\r\n";
    sendMessage(clientSock, str);
    
    us->setRegister(true);
    std::cout <<"name:" <<  us->getName() << " nickname " <<  us->getNickName() <<"is Register " << us->didRegister() << std::endl;
}


// Mesajı parçalara ayıran fonksiyon
bool Server::splitMessage(const std::string &message, std::string &part1, std::string &part2, std::string &part3) {
    size_t first_space = message.find(' ');
    if (first_space != std::string::npos) {
        part1 = message.substr(0, first_space); 
        size_t second_space = message.find(' ', first_space + 1);
        if (second_space != std::string::npos) {
            part2 = message.substr(first_space + 1, second_space - first_space - 1);
            part3 = message.substr(second_space + 1); 
        } else {
            part2 = message.substr(first_space + 1);
            part3.clear();
        }
        return !isOnlyWhitespace(part1) && !isOnlyWhitespace(part2) && !isOnlyWhitespace(part3);
    }
    return false; // Mesajda boşluk yok
}

bool Server::isUserNameTaken(const std::string &nickname) {
    for (std::vector<User *>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
        if ((*it)->getNickName() == nickname) {
            return true;
        }
    }
    return false;
}

void Server::sendError(int clientSock, const std::string &message) {
    std::string fullMessage = "ERROR " + message; 
    send(clientSock, fullMessage.c_str(), fullMessage.length(), 0);
}
void Server::sendMessage(int clientSock, const std::string &message) {
    send(clientSock, message.c_str(), message.length(), 0);
}

std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \n\r\t\f\v");
    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    return (start == std::string::npos || end == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

void Server::start()
{
    struct pollfd newPollfd;
    newPollfd.fd = _serverSocket;
    newPollfd.events = POLLIN;

    if(fcntl(_serverSocket, F_SETFL,O_NONBLOCK)== -1)
        throw std::runtime_error("Error while setting socket non-blocking!");

    _pollfds.push_back(newPollfd);
    std::cout<< "Server Started" << std::endl;

    _commands = new Commands(this);
    

    while(true)
    {                                                      //timeout eklenebilir
        int numReady = poll(&_pollfds[0], _pollfds.size(), -1);
        if(numReady == -1)
            throw std::runtime_error("Error: while polling!");
        this->handleEvents();
    }
}

void Server::handleEvents()
{
    for (unsigned long i = 0; i < _pollfds.size(); ++i) {
        std::cout << "\r Poll fd size : "<<_pollfds.size() << std::endl;
        struct pollfd& pfd = _pollfds[i];
        if((pfd.revents & POLLHUP) == POLLHUP){
            if (_users.empty())
			{
				break;
			}
            // if (_pollfds.size() < 2) //TODO: BİLİYOZ.
            //     break;
            std::cout << "arabadan atladi" << std::endl;
            removeUserAndFd(pfd.fd);
            break;
        }
        if ((pfd.revents & POLLIN) == POLLIN) {
            if (pfd.fd == _serverSocket) {
                sockaddr_in clientAddr;
                socklen_t addrlen = sizeof(clientAddr);
                int clientSock = accept(_serverSocket, (sockaddr*)&clientAddr, &addrlen);
                
                if (clientSock >= 0) {

                    addUser(clientSock, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

                    pollfd client_fd;
                    client_fd.fd = clientSock;
                    client_fd.events = POLLIN;
                    _pollfds.push_back(client_fd);

                    if(fcntl(clientSock, F_SETFL,O_NONBLOCK) == -1)
                        throw std::runtime_error("Error while setting client socket non-blocking!");
                    sendMessage(clientSock,"@ :ServerMessage Enter : <Password>, <Name>, <Nickname>\n");
                    break;
                }
            } else {
                char buffer[1024] = {0};
                std::string accumulated_message;
                bool end_of_message = false;
                while (!end_of_message) {
                    ssize_t bytes_received = recv(pfd.fd, buffer, sizeof(buffer) - 1, 0);
                    if (bytes_received == 0) {
                        std::cout << "Connection closed by client on fd: " << pfd.fd << std::endl;
                        removeUserAndFd(pfd.fd);
                        break;
                    }
                    for (ssize_t i = 0; i < bytes_received; i++) {
                        if (buffer[i] == '\n') {
                            accumulated_message += buffer[i];
                            end_of_message = true;
                            if(buffer[i-1] == '\r')
                                continue;
                            else
                                break;
                        } else {
                            accumulated_message += buffer[i];
                        }
                    }
                }
                std::cout <<"---123"<< accumulated_message << "---123"<<std::endl;
                // Mesajı işleme
                if (end_of_message) {
                    if (!accumulated_message.empty()) {
                        accumulated_message = trim(accumulated_message);
                        if(accumulated_message.empty())
                            break;
                       for(std::vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it) {
                        if((*it)->getClientfd() == pfd.fd && accumulated_message.find("\r\n") && !(*it)->didRegister())
                        {
                            forRegisterFromClient(accumulated_message,pfd.fd,*it);
                        }
                        if ((*it)->getClientfd() == pfd.fd && !(*it)->didRegister()) {
                            forRegister(accumulated_message, pfd.fd, *it);
                            break;
                        }
                        else
                        {
                            if ((*it)->getClientfd() == pfd.fd && (*it)->didRegister())
                            {
                                 _commands->commandFinder(accumulated_message, *it);
                                break;
                            }
                        }
                        if (_users.empty())
                            break;
                    }
                    }
                }
            }

        }
    }
}

void Server::addUser(int client_fd,char *host, int port)
{
	User* newUser = new User(client_fd, host, port);
	_users.push_back(newUser);
	std::cout << "User added: " << "host:" << host <<  "\tport:" << port <<std::endl;
}

std::vector<User *> Server::getUsers()
{
	if (_users.empty()) {
        std::cerr << "No users in the vector." << std::endl;
    }
	return _users;
}

std::vector<Channel *> Server::getChannel()
{
	return _channel;
}

User *Server::findUserByNick(const std::string &nickName)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
        User* user = *it;
        if ((*it)->getNickName() == nickName) {
            return (*it);
        }
    }
    return nullptr;
}

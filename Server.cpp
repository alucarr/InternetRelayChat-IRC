#include "Server.hpp"

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

    // ip bilgilerini set eder
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
                                                        //burada optval kullanmamızın sebebi int* bekliyor olması, işlevi sadece aktif etmek(1);
        if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0) {
            close(sock_fd);
      freeaddrinfo(serverInfo);
            throw std::runtime_error("Error while setting socket options!");
        }

        if (bind(sock_fd, cp->ai_addr, cp->ai_addrlen) < 0) { //socketi adres ve port ile ilişkilendiriyor atıyorum localhost:9000 ile soketi bağlıyor 
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
    // hata kontrolü olabilir
}

Server::~Server()
{
}

void Server::forRegister(std::string &message,int clientSock, User *us)
{
    std::string part1,part2,part3;
    std::string str;
    size_t first_space = message.find(' ');
    if (first_space != std::string::npos) {
        part1 = message.substr(0, first_space);  // İlk boşluğa kadar olan kısım

        size_t second_space = message.find(' ', first_space + 1);
        if (second_space != std::string::npos) {
            part2 = message.substr(first_space + 1, second_space - first_space - 1);  // İkinci boşluğa kadar olan kısım
            part3 = message.substr(second_space + 1);  // Geri kalan kısım
        }
    }
    if (_password != part1)
    {
        str = "password is incorrect!!\nEnter: <Password>, <Name>, <Nickname>\n";
        send(clientSock, str.c_str(), str.length(), 0);
    }
    else if(part1.empty() || part2.empty() || part3.empty())
    {
        str = "Please fill in entry information!!\nEnter: <Password>, <Name>, <Nickname>\n";
        send(clientSock, str.c_str(), str.length(), 0);   
    }
    else
    {
        us->setName(part2);
        us->setNickName(part3);
        str =  "Welcome : " + us->getName() +"\n";
        send(clientSock, str.c_str(), str.length(), 0);
        us->registered(); // burayı düzenlicez 0 ya da 1 alcak bool çünkü; adı da setRegister olcak;
    }
    //std::cout << us.didRegister() << "        " << std::endl;
    
}

   
void Server::start()
{
    struct pollfd newPollfd;
    newPollfd.fd = _serverSocket;
    newPollfd.events = POLLIN;

    if(fcntl(_serverSocket, F_SETFL,O_NONBLOCK)== -1) // fdlerin özelliklerini ayarlar
        throw std::runtime_error("Error while setting socket non-blocking!");

    _pollfds.push_back(newPollfd);
    std::cout<< "Server Started" << std::endl;

    while(true)
    {                                                      //timeout eklenebilir
        int numReady = poll(&_pollfds[0], _pollfds.size(), -1);//poll fd olaylarını izler
        if(numReady == -1)
            throw std::runtime_error("Error: while polling!");
        this->handleEvents();
    }
}

void Server::handleEvents()
{
    pollfd client_fd;
    for (unsigned long i = 0; i < _pollfds.size(); ++i) {
        struct pollfd& pfd = _pollfds[i]; // pfd burada tanımlanıyor
        if (pfd.revents & POLLIN) {
            if (pfd.fd == _serverSocket) {
                std::cout << "\033[35m" << "rainbow mustafa" << "\033[0m" << std::endl;
                sockaddr_in clientAddr;
                socklen_t addrlen = sizeof(clientAddr);
                int clientSock = accept(_serverSocket, (sockaddr*)&clientAddr, &addrlen);
                
                if (clientSock >= 0) {
                    std::string str;
                    str = "Enter: <Password>, <Name>, <Nickname>\n";
                    client_fd.fd = clientSock;    // Yeni kabul edilen client soketini ekliyoruz
                    client_fd.events = POLLIN;    // Veri bekle
                    _pollfds.push_back(client_fd);
                    if(fcntl(clientSock, F_SETFL,O_NONBLOCK)== -1) // fdlerin özelliklerini ayarlar client için
                        throw std::runtime_error("Error while setting client socket non-blocking!");
                    send(clientSock, str.c_str(), str.length(), 0);
                    addUser(clientSock, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                    break;
                }
            }
        }
            else {
                char message[1024] = {0};
                ssize_t bytes_received = recv(client_fd.fd, message, sizeof(message), 0);
                std::cout << message << std::endl;
                std::string message_str(message);
                for(std::vector<User *>::const_iterator it = _users.begin(); it != _users.end(); ++it)
                {
                    if((*it)->didRegister() == 0)
                        forRegister(message_str, client_fd.fd, *it);
                }
            }
                
        // if (pfd.revents & POLLHUP) {
        //     std::cout << "Bağlantı kapatıldı!" << std::endl;
        //     // Burada kaynakları serbest bırakabiliriz
        //     close(pfd.fd); // Socket'i kapat
        //     //exit(1);
        // } 
    }
}
void Server::addUser(int client_fd,char *host, int port)
{
	User* newUser = new User(client_fd, host, port);
	_users.push_back(newUser);
	std::cout << "User added: " << "host:" << host <<  "\tport:" << port <<std::endl;
}
#if !defined(USER_HPP)
#define USER_HPP

#include <iostream>
#include <string> // std::string kullanabilmek için ekledik

class User {
private:
    std::string _name;
    std::string _nickName;
    int _client_fd;
    char *_host;
    int _port;
    bool _didRegister;

public:
	User();
    User(int client_fd, char *host, int port);
    ~User();
    
    std::string getName() { return _name; }
    std::string getNickName() { return _nickName; }
    int getPort() {return _port; }
    int getClientfd() {return _client_fd;}
    bool didRegister();
    void setRegister(bool status);
    void setName(std::string &name);
    void setNickName(std::string &nickname);
};

#endif // USER_HPP

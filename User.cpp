#include "User.hpp"

User::User()
{
}

User::User(int client_fd, char *host, int port)
{
	_client_fd = client_fd;
	_host = host;
	_port = port;
	_didRegister = 0;
	
}

User::~User()
{
}

bool User::didRegister()
{
	return (_didRegister);
}

void User::setRegister(bool status){ _didRegister = status;}

void User::setName(std::string &name)
{
	_name = name;
}

void User::setNickName(std::string &nickname)
{
	_nickName.clear();
	_nickName = nickname;
}
void User::setChannelName(std::string &ch)
{
	_channelName.push_back(ch);
}

void User::removeUser(const std::string& channelName) {
    // Kanal adını bul ve sil
    for (std::vector<std::string>::iterator it = _channelName.begin(); it != _channelName.end(); ++it) {
        if (*it == channelName) {
            _channelName.erase(it); // Kanal adını sil
            break; // Kanal silindikten sonra döngüden çık
        }
    }
}
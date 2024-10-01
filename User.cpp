#include "User.hpp"

User::User()
{
}

User::User(int client_fd, char *host, int port)
{
	_client_fd = client_fd;
	_host = host;
	_port = port;
}

User::~User()
{
}

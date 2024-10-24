#include "Notice.hpp"
#include <sstream>
Notice::Notice()
{
}

void Notice::execute(int client_fd)
{
    if (_args.size() < 2)
    {
        _server->sendMessage(client_fd, "Usage: notice <Target> <Message>\n");
        return;
    }

    // if (_args[2].find("LAGCHECK") != std::string::npos)
    //     return;

    User* targetUser = _server->findUserByNick(_args[1]);
    if (!targetUser)
    {
        _server->sendError(client_fd, "No such user\n");
        return;
    }

    std::ostringstream oss;
    oss << targetUser->getPort();
    std::string portStr = oss.str();

    std::string str = ":" + _users->getNickName() + "!" + _users->getName() + "@" + portStr + " NOTICE " + targetUser->getNickName() + " :";
    if (_args.size() >= 3 && _args[2].front() != ':')
		str+= ":";
	if (_args.size() == 2 && _args[1].back() != ':')
		str+= ":";

    for (size_t i = 2; i < _args.size(); i++)
    {
        if(i != 2)
            str += " ";
        str += _args[i];
    }

    str += "\r\n";
    _server->sendMessage(targetUser->getClientfd(), str);
}



std::string Notice::getName() const
{
	return "NOTICE";
}

std::string Notice::description() const
{
	return  "NOTICE: Sends a notification message to the target (user or channel)";
}

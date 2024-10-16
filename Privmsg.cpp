#include "Privmsg.hpp"

Privmsg::Privmsg()
{
}

void Privmsg::execute(int client_fd)
{
    std::string mes = ":" + _users->getNickName() + " PRIVMSG " + _args[1] + " : ";
    for (size_t i = 2; i < _args.size(); i++)
    {
        mes += " ";
        mes += _args[i];
    }

    std::vector<User*> users = _server->getUsers();

    if (!_args[2].empty())
    {
        if (_args[1][0] == '#')
        {
            std::vector<Channel*> channels = _server->getChannel();

            std::string channelName = _args[1];

            for (size_t i = 0; i < channels.size(); i++)
            {
                if (channels[i]->getChannelName() == channelName) 
                {
                    for (size_t j = 0; j < users.size(); j++)
                    {
                        User* user = users[j];
                        std::vector<std::string> userChannels = user->getChannelName();
                        
                        for (size_t k = 0; k < userChannels.size(); k++) 
                        {
                            if (userChannels[k] == channelName && user->getClientfd() != client_fd) 
                            {
                                _server->sendMessage(user->getClientfd(), mes + "\n");
                            }
                        }
                    }
                    return;
                }
            }
        }
        else
        {
            for (size_t i = 0; i < users.size(); i++)
            {
                if (users[i]->getNickName() == _args[1])
                {
                    _server->sendMessage(users[i]->getClientfd(), mes + "\n");
                    return;
                }
                if (i == users.size() - 1)
                {
                    _server->sendMessage(client_fd, "Error : hata \n");
                }
            }
        }
    }
    else
    {
        _server->sendMessage(client_fd, "Error : arguman eksik \n");
    }
}


std::string Privmsg::getName() const
{
	return "PRIVMSG";
}

std::string Privmsg::description() const
{
	return  "PRIVMSG: Sends a message to a specific user or channel";
}

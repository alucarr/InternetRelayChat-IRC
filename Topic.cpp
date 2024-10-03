#include "Topic.hpp"

Topic::Topic()
{
}

void Topic::execute(int client_fd)
{
}

std::string Topic::getName() const
{
	return "topic";
}

std::string Topic::description() const
{
	return BLUE "Topic 	: Sets or shows the topic of a channel";
}

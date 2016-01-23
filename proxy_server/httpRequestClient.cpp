#include "httpRequestClient.h"

void httpRequestClient::parseFirstLine()
{
	std::string line = str.substr(0, str.find("\r\n"));
	int i = str.find(" ");
	method = line.substr(0, i);
	line = line.substr(i+1);
	i = str.find(" ");
	path = line.substr(0, i);
	version = line.substr(i+1);
}

std::string httpRequestClient::getMethod()
{
	return method;
}

std::string httpRequestClient::getPath()
{
	return path;
}
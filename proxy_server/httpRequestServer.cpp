#include "httpRequestServer.h"

void httpRequestServer::parseFirstLine()
{
	std::string line = str.substr(0, str.find("\r\n"));
	int i = line.find(" ");
	vertion = line.substr(0, i);
	line = line.substr(i + 1);
	i = line.find(" ");
	code = line.substr(0, i);
	info = line.substr(i + 1);
}

std::string httpRequestServer::getCode()
{
	return code;
}

std::string httpRequestServer::getInfo()
{
	return info;
}

std::string httpRequestServer::getVertion()
{
	return vertion;
}
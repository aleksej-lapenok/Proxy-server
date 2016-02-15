#include "httpRequestServer.h"
#include <algorithm>

void httpRequestServer::parseFirstLine()
{
	std::string line = str.substr(0, str.find("\r\n"));
	int i = line.find(" ");
	vertion = line.substr(0, i);
//	std::transform(vertion.begin(), vertion.end(), vertion.begin(), toupper);
	toUpper(&vertion);
	line = line.substr(i + 1);
	i = line.find(" ");
	code = line.substr(0, i);
	std::transform(code.begin(), code.end(), code.begin(), toupper);
	info = line.substr(i + 1);
	if (vertion != "HTTP/1.1" && vertion != "HTTP/1.0")
	{
		stat = BAD;
	}
	if (code.find_first_not_of("0123456789") != std::string::npos)
	{
		stat = BAD;
	}
	if (info == "")
	{
		stat = BAD;
	}
	
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
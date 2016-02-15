#include "httpRequestClient.h"
#include <algorithm>

void httpRequestClient::parseFirstLine()
{
	std::string line = str.substr(0, str.find("\r\n"));
	int i = str.find(" ");
	method = line.substr(0, i);
//	std::transform(method.begin(), method.end(), method.begin(), toupper);
	toUpper(&method);
	line = line.substr(i+1);
	i = line.find(" ");
	path = line.substr(0, i);
	
	version = line.substr(i+1);
//	std::transform(version.begin(), version.end(), version.begin(), toupper);
	toUpper(&version);
	if (method == "" || path == "" || version == "")
	{
		stat = BAD;
	}
	if (method != "GET" && method != "CONNECT")
	{
		stat = BAD;
	}
	if (version != "HTTP/1.1" && version != "HTTP/1.0")
	{
		stat = BAD;
	}
}

std::string httpRequestClient::getMethod()
{
	return method;
}

std::string httpRequestClient::getPath()
{
	return path;
}
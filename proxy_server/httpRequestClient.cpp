#include "httpRequestClient.h"
#include <algorithm>
#include <iostream>

void httpRequestClient::parseFirstLine()
{
	std::string line = str.substr(0, str.find("\r\n"));
	size_t i = str.find(" ");
	method = line.substr(0, i);
	toUpper(&method);
	line = line.substr(i+1);
	i = line.find(" ");
	path = line.substr(0, i);
	version = line.substr(i+1);
	toUpper(&version);
	i = path.find("http://");
	if (i == std::string::npos)
	{
		i = 0;
	}
	else
	{
		i = 7;
	}
	url = path.substr(i);
	i = url.find_first_of("/");
	if (i != std::string::npos)
	{
		path = url.substr(i);
		url = url.substr(0, i);
	}
	else
	{
		path = "/";
	}
	i = url.find_last_of(":");
	if (i == std::string::npos)
	{
		port = 80;
	}
	else
	{
		std::string p = url.substr(i + 1);
		url = url.substr(0, i);
		if (p.find_first_not_of("0123456789") != std::string::npos)
		{
			stat = BAD;
		}
		else
		{
			port = std::stoi(p);
		}
	}
	if (method == "" || path == "" || version == "" || url=="")
	{
		stat = BAD;
	}
	if (method != "GET" && method != "CONNECT" && method != "POST")
	{
		stat = BAD;
	}
	if (version != "HTTP/1.1" && version != "HTTP/1.0")
	{
		stat = BAD;
	}
	/*std::cout << url << std::endl;
	std::cout << port << std::endl;
	std::cout << path << std::endl;*/
}

std::string httpRequestClient::makeRequest()
{
	return str;
}

std::string httpRequestClient::getUrl()
{
	return url;
}

int httpRequestClient::getPort()
{
	return port;
}

std::string httpRequestClient::getMethod()
{
	return method;
}
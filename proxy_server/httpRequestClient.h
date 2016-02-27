#pragma once

#include "httpRequest.h"

struct httpRequestClient :httpRequest
{
	httpRequestClient():httpRequest()
	{}
	std::string makeRequest();
	std::string getUrl();
	std::string getMethod();
	int getPort();

protected:
	virtual void parseFirstLine();
	std::string method;
	std::string path;
	std::string version;
	std::string url;
	int port;
};
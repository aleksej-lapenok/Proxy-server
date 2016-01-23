#pragma once

#include "httpRequest.h"

struct httpRequestClient :httpRequest
{
	httpRequestClient():httpRequest()
	{}
	std::string getMethod();
	std::string getPath();
protected:
	virtual void parseFirstLine();
	std::string method;
	std::string path;
	std::string version;
};
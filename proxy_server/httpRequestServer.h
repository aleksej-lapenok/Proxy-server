#pragma once

#include "httpRequest.h"

struct httpRequestServer:httpRequest
{
	httpRequestServer():httpRequest()
	{}
	std::string getCode();
	std::string getInfo();
	std::string getVertion();
protected:
	virtual void parseFirstLine();
	std::string vertion;
	std::string code;
	std::string info;
};
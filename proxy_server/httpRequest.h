#pragma once

#include <string>

struct httpRequest
{
	httpRequest();
	httpRequest(std::string&);
	httpRequest(httpRequest const&);
	httpRequest operator=(httpRequest const&);
	void setStr(std::string&);
	std::string toString();
	bool isEmpty();
private:
	std::string str = "";
};
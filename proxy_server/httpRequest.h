#pragma once

#include <string>
#include <map>

struct httpRequest
{
	httpRequest();
	httpRequest(std::string&);
	httpRequest(httpRequest const&);
	void setStr(std::string);
	std::string toString();
	std::string getMethod(std::string const&);
	bool isEmpty();
	virtual ~httpRequest() {}
protected:
	virtual void parseFirstLine() = 0;
	std::map<std::string, std::string> params;
	std::string type;
	std::string str = "";
	void parse();
};


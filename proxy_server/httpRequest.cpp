#include "httpRequest.h"

httpRequest::httpRequest()
{

}

httpRequest::httpRequest(std::string& str1) :str(str1)
{
	parse();
}

httpRequest::httpRequest(httpRequest const& other) : str(other.str), params(other.params)
{

}

void httpRequest::setStr(std::string str1)
{
	str = str1;
	params.clear();
	parse();
}

bool httpRequest::isEmpty()
{
	return str.size() == 0;
}

std::string httpRequest::toString()
{
	return str;
}

void httpRequest::parse()
{
	parseFirstLine();
	size_t k = str.find("\r\n")+2;
	size_t i = str.find(": ",k);
	size_t j = str.find("\r\n",k);
	std::string method = "", value = "";
	while (i != std::string::npos && j!=std::string::npos)
	{
		method = str.substr(k, i - k);
		value = str.substr(i + 2, j - 2 - i);
		params.insert(std::pair<std::string, std::string>(method, value));
		k = j + 2;
		i = str.find(": ",k);
		j = str.find("\r\n",k);
	}
	method = str.substr(k,i-k);
	value = str.substr(i + 2, j - 2 - i);
	params.insert(std::pair<std::string, std::string>(method, value));
}

std::string httpRequest::getMethod(std::string const& method)
{
	auto i=params.find(method);
	if (i != params.end())
	{
		return i->second;
	}
	else
	{
		return "";
	}
}
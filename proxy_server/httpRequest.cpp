#include "httpRequest.h"
#include <algorithm>

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

void httpRequest::setStr(std::string const& str1)
{
	str = str1;
	params.clear();
	parse();
}


std::string httpRequest::toString()
{
	return str;
}

void httpRequest::parse()
{
	parseFirstLine();
	if (stat == BAD)
	{
		return;
	}
	size_t k = str.find("\r\n")+2;
	size_t i = str.find(": ",k);
	size_t j = str.find("\r\n",k);
	std::string method = "", value = "";
	while (i != std::string::npos && j!=std::string::npos)
	{
		if (!(k < i && i < j))
		{
			stat = BAD;
			return;
		}
		method = str.substr(k, i - k);
		value = str.substr(i + 2, j - 2 - i);
		params.insert(std::pair<std::string, std::string>(method, value));
		k = j + 2;
		i = str.find(": ",k);
		j = str.find("\r\n",k);
	}
	if (!(k < i && i < j) && !(i==j && j==std::string::npos))
	{
		stat = BAD;
		return;
	}
	method = str.substr(k,i-k);
	value = str.substr(i + 2, j - 2 - i);
	params.insert(std::pair<std::string, std::string>(method, value));
	stat = GOOD;
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

void httpRequest::toUpper(std::string* str)
{
	std::transform(str->begin(), str->end(), str->begin(), toupper);
}
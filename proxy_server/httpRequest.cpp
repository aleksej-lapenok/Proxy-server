#include "httpRequest.h"

httpRequest::httpRequest()
{

}

httpRequest::httpRequest(std::string& str1) :str(str1)
{

}

httpRequest::httpRequest(httpRequest const& other) : str(other.str)
{

}

httpRequest httpRequest::operator=(httpRequest const& other)
{
	return httpRequest(other);
}

void httpRequest::setStr(std::string& str1)
{
	str = str1;
}

bool httpRequest::isEmpty()
{
	return str.size() == 0;
}

std::string httpRequest::toString()
{
	return str;
}
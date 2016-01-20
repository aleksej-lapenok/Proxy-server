#include "Exceptions.h"
#include <string>

ExceptionBind::ExceptionBind(int port)
{
	std::string str = "Bild faild: port ¹" + std::to_string(port) + " in use";
	//exception(str);
}

ExceptionListen::ExceptionListen()
{
	exception("Listen faild");
}

ExceptionInvalidSocket::ExceptionInvalidSocket()
{
	exception("Socket is invalid");
}

OutOfLen::OutOfLen()
{
	exception("OutOfLen_Exception");
}
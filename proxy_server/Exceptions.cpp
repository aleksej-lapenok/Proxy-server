#include "Exceptions.h"
#include <string>

ExceptionBild::ExceptionBild(int port)
{
	std::string str = "Bild faild: port �" + std::to_string(port) + " in use";
	//exception(str);
}

ExceptionListen::ExceptionListen()
{
	exception("Listen faild");
}

ExceptionInvalidSocket::ExceptionInvalidSocket()
{
	exception("Scoket is invalid");
}
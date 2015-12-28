#pragma once
#include <exception>

struct ExceptionBind:std::exception
{
	ExceptionBind(int port);
};

struct ExceptionListen:std::exception
{
	ExceptionListen();
};

struct ExceptionInvalidSocket :std::exception
{
	ExceptionInvalidSocket();
};

struct OutOfLen:std::exception
{
	OutOfLen();
};

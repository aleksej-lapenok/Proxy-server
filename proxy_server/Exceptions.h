#pragma once
#include <exception>

struct ExceptionBild:std::exception
{
	ExceptionBild(int port);
};

struct ExceptionListen:std::exception
{
	ExceptionListen();
};

struct ExceptionInvalidSocket :std::exception
{
	ExceptionInvalidSocket();
};
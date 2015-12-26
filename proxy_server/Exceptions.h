#pragma once
#include <exception>

// TODO: s/Bild/Bind/
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

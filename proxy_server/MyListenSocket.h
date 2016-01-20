#pragma once
#include "Clients.h"

struct MyListenSocket
{
private:
	Clients cl;
protected:
	virtual MySocketPair* onAccept(MySocket*);
public:
	MyListenSocket();
	MyListenSocket(int port);
	void myAccept();
};

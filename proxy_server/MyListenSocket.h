#pragma once
#include "Clients.h"

struct MyListenSocket
{
private:
	Clients cl;
protected:
	virtual MySocketPair* onAccept(MySocket*);
	int timeout;
public:
	MyListenSocket(int);
	MyListenSocket(int,int port);
	void myAccept();
};

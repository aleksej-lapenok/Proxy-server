#pragma once
#include "Clients.h"

struct MyListenSocket
{
protected:
	virtual void onAccept(MySocket*);
	int timeout;
	Clients cl;
public:
	MyListenSocket(int);
	MyListenSocket(int,int port);
	void myAccept();
};

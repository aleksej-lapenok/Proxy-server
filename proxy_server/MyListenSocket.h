#pragma once
#include "Clients.h"

struct MyListenSocket
{
private:
	//std::queue<MySocketPair> clients;
	int timeout;
	Clients cl;
public:
	MyListenSocket(long timeout);
	MyListenSocket(int port,long timeout);
	MySocketPair onAccept(MySocket&);
	void myAccept();
};

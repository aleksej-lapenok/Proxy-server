#pragma once
#include "MySocketPair.h"
#include <queue>
#include <Windows.h>

struct MyListenSocket
{
private:
	MySocket mySocket;
	std::queue<MySocketPair> clients;
	int timeout;
public:
	MyListenSocket(long timeout);
	MyListenSocket(int port,long timeout);
	MySocketPair onAccept(MySocket&);
	void myAccept();
	void listenClients();
	void Destroy(); // TODO: remove or make private
	~MyListenSocket();
};

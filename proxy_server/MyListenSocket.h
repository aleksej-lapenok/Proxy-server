#pragma once
#include "MySocketPair.h"
#include <queue>
#include <Windows.h>

struct MyListenSocket
{
	MySocket mySocket;
	std::queue<MySocketPair> clients;
	MyListenSocket();
	MyListenSocket(int port);
	MySocketPair onAccept(MySocket&);
	void myAccept();
	void listenClients();
	void Destroy();
	~MyListenSocket();
};
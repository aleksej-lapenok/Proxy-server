#pragma once
#include "MySocket.h"
#include "timer.h"

struct MySocketPair
{

	MySocket client;
	MySocket server;
	bool canBeRead = false;
	timer time;
public:
	MySocketPair(long timeout);
	MySocketPair(long timeout,SOCKET client, SOCKET server);
	MySocketPair(long timeout,MySocket, MySocket);
	void Destroy();
	void ReadAndWrite();
};
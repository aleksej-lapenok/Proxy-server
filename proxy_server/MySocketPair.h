#pragma once
#include "MySocket.h"

struct MySocketPair
{
	MySocket client;
	MySocket server;
	bool canBeRead = false;
	MySocketPair();
	MySocketPair(SOCKET client, SOCKET server);
	MySocketPair(MySocket, MySocket);
	void Destroy();
	void ReadAndWrite();
};
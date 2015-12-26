#pragma once

#include "MySocket.h"

struct MySocketPair
{
	MySocket client;
	MySocket server;
private:
	bool canBeRead = false;
public:
	MySocketPair();
	MySocketPair(SOCKET client, SOCKET server);
	MySocketPair(MySocket, MySocket);
	void ReadAndWrite();
	void Destroy();
};
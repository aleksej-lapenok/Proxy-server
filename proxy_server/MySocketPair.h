#pragma once

#include "MySocket.h"

struct MySocketPair
{
	MySocket* client;
	MySocket* server;
private:
	bool canBeRead = false;
	void Destroy();
public:
	bool is_close = false;
	MySocketPair();
	MySocketPair(SOCKET client, SOCKET server);
	MySocketPair(MySocket*, MySocket*);
	void ReadAndWrite();
	~MySocketPair();
};

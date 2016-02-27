#pragma once


#include "MySocket.h"
#include <string>

struct MySocketPair
{

	MySocket* client;
	MySocket* server;
	bool canBeRead = false;
	int timeout;
private:
	int max_timer;
	void Destroy();
protected:
	virtual void onReadClient();
	virtual void onReadServer();
	virtual void onConnected();
	
public:
	bool is_close = false;
	MySocketPair(int);
	MySocketPair(int,SOCKET client, SOCKET server);
	MySocketPair(int,MySocket*, MySocket*);
	void updateTimer();
	void ReadAndWrite();
	virtual ~MySocketPair();
};

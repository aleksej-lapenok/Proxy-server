#pragma once


#include "MySocket.h"
#include <string>

struct MySocketPair
{
	std::string file_name_server = "server";
	std::string file_name_client = "client";

	MySocket* client;
	MySocket* server;
	bool canBeRead = false;
private:
	void Destroy();
protected:
	virtual void onReadClient();
	virtual void onReadServer();
	
public:
	bool is_close = false;
	MySocketPair();
	MySocketPair(SOCKET client, SOCKET server);
	MySocketPair(MySocket*, MySocket*);
	void ReadAndWrite();
	virtual ~MySocketPair();
};

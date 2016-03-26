#pragma once

#include "httpRequestClient.h"
#include "httpRequestServer.h"
#include <string>
#include <iostream>

// TODO: split into 2 classes
template<bool flage>
struct httpSocket :MySocket
{
	httpSocket(SOCKET client, int WSAevent,int timeout);

	httpRequest* request; // TODO: unique_ptr

	virtual ~httpSocket();
protected:
	virtual void onRead();
	virtual void onConnected();

private:
	bool findRequest(std::string& str);
private:
	std::string begin = "";
};

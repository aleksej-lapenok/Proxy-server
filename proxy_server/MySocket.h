#pragma once

#include <winsock2.h>

struct MySocket
{
	static int const LEN = 1024*4;
	MySocket* other;

	bool events[9];
	SOCKET Socket;
private:
	MySocket(MySocket const&);
	void Destroy();
protected:
	virtual void onRead();
	virtual void onConnected();

	int len_buffer = 0;
public:
	WSAEVENT WSAEvent;
	char buffer[LEN];
	MySocket();
	MySocket(SOCKET);
	MySocket(SOCKET, long events);
	MySocket(SOCKET, WSAEVENT);
	bool checkEvent();
	void ReadAndWrite();
	virtual ~MySocket();
};

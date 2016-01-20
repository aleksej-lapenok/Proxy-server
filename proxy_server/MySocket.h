#pragma once

#include <winsock2.h>

struct MySocket
{
	static int const LEN = 4096*2;
	int len_buffer = 0;
	bool events[9];
	SOCKET Socket;
private:
	MySocket(MySocket const&);
	void Destroy();
public:
	WSAEVENT WSAEvent;
	char buffer[LEN];
	MySocket();
	MySocket(SOCKET);
	MySocket(SOCKET, long events);
	MySocket(SOCKET, WSAEVENT);
	bool checkEvent();
	~MySocket();
};

#pragma once

#include <winsock2.h>
#include "MyArray.h"

struct MySocket
{
	// TODO: make members private

	static int const LEN = 4096*2;
	int len_buffer = 0;
	//MyArray<bool, 9> events;
	bool events[9];
	SOCKET Socket;
private:
	MySocket(MySocket const&);
	void Destroy();
	//MySocket operator=(MySocket const&);
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

#pragma once

#include <winsock2.h>

struct MySocket
{
	static int const LEN = 1024*4;
	MySocket* other;
	int timeout;
	bool events[9];
	SOCKET Socket;
private:
	int maxTimeout;
	MySocket(MySocket const&);
	void Destroy();
protected:
	virtual void onRead();
	virtual void onConnected();

	int len_buffer = 0;
public:
	WSAEVENT WSAEvent;
	char buffer[LEN];
	MySocket(int);
	MySocket(SOCKET,int);
	MySocket(SOCKET, long events,int);
	MySocket(SOCKET, WSAEVENT,int);
	bool checkEvent();
	void ReadAndWrite();
	void updateTimer();
	virtual ~MySocket();
};

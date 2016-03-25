#include "MySocket.h"
#include <iostream>
#include <assert.h>

using namespace std;


MySocket::MySocket(int timeout):maxTimeout(timeout)
{
	this->timeout = maxTimeout;
	Socket = INVALID_SOCKET;
	for (int i = 0; i < 9; i++)
	{
		events[i] = false;
	}
	other = 0;
}

MySocket::MySocket(SOCKET sock,int timeout):MySocket(timeout)
{
	Socket = sock;
}

MySocket::MySocket(SOCKET sock, long events,int timeout):MySocket(sock,timeout)
{
	WSAEvent = WSACreateEvent();
	WSAEventSelect(Socket, WSAEvent, events | FD_CLOSE);
}

MySocket::MySocket(SOCKET sock, WSAEVENT even,int timeout):MySocket(sock,timeout)
{
	WSAEvent = even;
}

MySocket::MySocket(MySocket const& )
{
	Socket = INVALID_SOCKET;
	for (int i = 0; i < 9; i++)
	{
		events[i] = false;
	}
	other = 0;
}

void MySocket::Destroy()
{
	if (Socket != INVALID_SOCKET)
	{
		closesocket(Socket);
		Socket = INVALID_SOCKET;
		WSACloseEvent(WSAEvent);
	}
}

bool MySocket::checkEvent()
{
	bool flage = false;
	WSANETWORKEVENTS WSANetWork;
	WSAEnumNetworkEvents(Socket, WSAEvent, &WSANetWork);
	if ((WSANetWork.lNetworkEvents & FD_WRITE) && WSANetWork.iErrorCode[FD_WRITE_BIT] == 0 && !events[FD_WRITE_BIT])
	{
		events[FD_WRITE_BIT] = true;
		flage = true;
	}
	if ((WSANetWork.lNetworkEvents & FD_READ) && WSANetWork.iErrorCode[FD_READ_BIT] == 0)
	{
		events[FD_READ_BIT] = true;
		flage = true;
	}
	if ((WSANetWork.lNetworkEvents & FD_CONNECT) && WSANetWork.iErrorCode[FD_CONNECT_BIT] == 0)
	{
		events[FD_CONNECT_BIT] = true;
		flage = true;
	}
	if ((WSANetWork.lNetworkEvents&FD_ACCEPT) && WSANetWork.iErrorCode[FD_ACCEPT_BIT] == 0)//3
	{
		events[FD_ACCEPT_BIT] = true;
		flage = true;
	}
	if ((WSANetWork.lNetworkEvents & FD_CLOSE) && WSANetWork.iErrorCode[FD_CLOSE_BIT] == 0)
	{
		events[FD_CLOSE_BIT] = true;
		flage = true;
	}
	return flage;
}

void MySocket::ReadAndWrite()
{
	if (Socket == INVALID_SOCKET || other->Socket == INVALID_SOCKET)
		return;
	//if (server->checkEvent() || client->checkEvent())
	{
		if (other->other != this)
		{
			events[FD_CLOSE_BIT] = true;
			return;
		}
		bool flage = true;

		if (other->events[FD_CONNECT_BIT])
		{
			onConnected();
			flage = false;
		}

		if (events[FD_CONNECT_BIT])
		{
			onConnected();
			flage = false;
		}

		if ((other->events[FD_READ_BIT] || other->len_buffer > 0) && /*canBeRead &&*/ events[FD_WRITE_BIT])
		{
			other->onRead();
			flage = false;
		}
		if (events[FD_CLOSE_BIT])//close=5;
			return;
		if ((events[FD_READ_BIT] || len_buffer > 0) && /*canBeRead &&*/ other->events[FD_WRITE_BIT])
		{
			onRead();
			flage = false;
		}
		if (flage && (other->events[FD_CLOSE_BIT] || events[FD_CLOSE_BIT]))
		{
			events[FD_CLOSE_BIT] = true;
			other->events[FD_CLOSE_BIT] = true;
		}
	}
}

void MySocket::onConnected()
{
	std::cout << "client connected" << std::endl;
	events[FD_CONNECT_BIT] = false;
}

void MySocket::onRead()//client = this; server=other
{
	if (len_buffer == 0)
	{
		len_buffer = recv(Socket, buffer, LEN, 0);
	}
	int lenSend = send(other->Socket, buffer, len_buffer, 0);
	std::cout << "client to server: " << std::endl/* << std::string(server->buffer + 0, server->buffer + server->len_buffer) << std::endl << std::endl*/;
	events[FD_READ_BIT] = false;
	if (WSAGetLastError() == 10035 || lenSend == -1)
	{
		other->events[FD_WRITE_BIT] = false;
	}
	else
	{
		assert(len_buffer == lenSend);
		other->len_buffer = 0;
	}
}

void MySocket::updateTimer()
{
	timeout = maxTimeout;
	if (other)
	{
		other->timeout = other->maxTimeout;
	}
}

MySocket::~MySocket()
{
	Destroy();
}

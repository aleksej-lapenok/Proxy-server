#include "MySocket.h"

using namespace std;


MySocket::MySocket()
{
	Socket = INVALID_SOCKET;
	for (int i = 0; i < 9; i++)
	{
		events[i] = false;
	}
}

MySocket::MySocket(SOCKET sock)
{
	Socket = sock;
	for (int i = 0; i < 9; i++)
	{
		events[i] = false;
	}
}

MySocket::MySocket(SOCKET sock, long events)
{
	Socket = sock;
	WSAEvent = WSACreateEvent();
	WSAEventSelect(Socket, WSAEvent, events | FD_CLOSE);
	for (int i = 0; i < 9; i++)
	{
		this->events[i] = false;
	}
}

MySocket::MySocket(SOCKET sock, WSAEVENT even)
{
	Socket = sock;
	WSAEvent = even;
	for (int i = 0; i < 9; i++)
	{
		events[i] = false;
	}
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

MySocket::~MySocket()
{
	//TODO: ???
	//closesocket(Socket);
}

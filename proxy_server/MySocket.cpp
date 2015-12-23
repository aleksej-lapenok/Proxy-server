#include "MySocket.h"

using namespace std;


MySocket::MySocket()
{
	Socket = INVALID_SOCKET;
}

MySocket::MySocket(SOCKET sock)
{
	Socket = sock;
}

MySocket::MySocket(SOCKET sock, long events)
{
	Socket = sock;
	WSAEvent = WSACreateEvent();
	WSAEventSelect(Socket, WSAEvent, events | FD_CLOSE);
}

MySocket::MySocket(SOCKET sock, WSAEVENT even)
{
	Socket = sock;
	WSAEvent = even;
}

MySocket::MySocket(MySocket const& other)
{
	Socket = other.Socket;
	WSAEvent = other.WSAEvent;
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

MySocket::~MySocket()
{
	//closesocket(Socket);
}
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "MyListenSocket.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;


MyListenSocket::MyListenSocket(int port) :mySocket(socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))
{
	sockaddr_in inetAddr;
	inetAddr.sin_family = AF_INET;
	inetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	inetAddr.sin_port = htons(port);
	if (bind(mySocket.Socket, (SOCKADDR*)&inetAddr, sizeof(inetAddr)))
		throw ExceptionBild(port);

	mySocket.WSAEvent = WSACreateEvent();
	WSAEventSelect(mySocket.Socket, mySocket.WSAEvent, FD_ACCEPT | FD_CLOSE);
	if (listen(mySocket.Socket, 10))
		throw ExceptionListen();
}

MyListenSocket::MyListenSocket()
{
	MyListenSocket(7777);
}

MySocketPair MyListenSocket::onAccept(MySocket& client1)
{
	SOCKADDR_IN inetAddr;
	inetAddr.sin_family = AF_INET;
	inetAddr.sin_addr.s_addr = inet_addr("77.234.212.58");
	inetAddr.sin_port = htons(80);
	MySocket client2(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP),FD_READ|FD_WRITE|FD_CONNECT);
	connect(client2.Socket, (struct sockaddr*)&inetAddr, sizeof(inetAddr));
	MySocketPair back(client1, client2);
	return back;
}
void MyListenSocket::myAccept()
{
	int index = WSAWaitForMultipleEvents(1, &mySocket.WSAEvent, true, 0, false);
	while(index != WSA_WAIT_FAILED && index != WSA_WAIT_TIMEOUT)
	{
		WSANETWORKEVENTS wsaNetwork;
		WSAEnumNetworkEvents(mySocket.Socket, mySocket.WSAEvent, &wsaNetwork);
		if ((wsaNetwork.lNetworkEvents & FD_ACCEPT) && (wsaNetwork.iErrorCode[FD_ACCEPT_BIT] == 0))
		{
			MySocket client(accept(mySocket.Socket, NULL, NULL));
			client.WSAEvent = WSACreateEvent();
			WSAEventSelect(client.Socket, client.WSAEvent, FD_READ|FD_WRITE|FD_CONNECT);
			MySocketPair Client = onAccept(client);
			clients.push(Client);
			cout << "Client accepted" << endl;
		}
		index = WSAWaitForMultipleEvents(1, &mySocket.WSAEvent, true, 0, false);
	}
}
void MyListenSocket::listenClients()
{
	int len = clients.size();
	for (int i = 0; i < len; i++)
	{
		MySocketPair client = clients.front();
		clients.pop();
		client.ReadAndWrite();
		if (client.client.Socket != INVALID_SOCKET && client.server.Socket != INVALID_SOCKET)
		{
			clients.push(client);
		}
		else
		{
			cout << "Client closed" << endl;
		}
	}
}
void MyListenSocket::Destroy()
{
	while (clients.size() != 0)
	{
		clients.front().Destroy();
		clients.pop();
	}
	mySocket.Destroy();
	WSACleanup();
}

MyListenSocket::~MyListenSocket()
{
	Destroy();
}
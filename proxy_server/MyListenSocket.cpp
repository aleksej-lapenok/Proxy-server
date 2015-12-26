#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "MyListenSocket.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;


MyListenSocket::MyListenSocket(int port,long timeout) 
{
	MySocket mySocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	sockaddr_in inetAddr;
	inetAddr.sin_family = AF_INET;
	inetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	inetAddr.sin_port = htons(port);
	if (bind(mySocket.Socket, (SOCKADDR*)&inetAddr, sizeof(inetAddr)))
		throw ExceptionBind(port);

	mySocket.WSAEvent = WSACreateEvent();
	WSAEventSelect(mySocket.Socket, mySocket.WSAEvent, FD_ACCEPT | FD_CLOSE);
	if (listen(mySocket.Socket, 10))
		throw ExceptionListen();
	this->timeout = timeout;
	MySocketPair listenSocket(mySocket, mySocket);
	cl.Add(listenSocket);
}

MyListenSocket::MyListenSocket(long timeout)
{
	MyListenSocket(7777,timeout);
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
	std::pair<bool, MySocketPair&> ev = cl.WaitMultyEvent();
	if (ev.first)
	{
		MySocketPair& pair = ev.second;
		if (pair.server.Socket == ev.second.client.Socket && (pair.client.events[FD_ACCEPT_BIT] || pair.server.events[FD_ACCEPT_BIT]))
		{
			MySocket client(accept(ev.second.client.Socket, NULL, NULL));
			client.WSAEvent = WSACreateEvent();
			WSAEventSelect(client.Socket, client.WSAEvent, FD_READ | FD_WRITE | FD_CONNECT);
			MySocketPair Client = onAccept(client);
			cl.Add(Client);
			cout << "Client accepted" << endl;
		}
		else
		{
			pair.ReadAndWrite();
			if (pair.client.Socket == INVALID_SOCKET || pair.server.Socket == INVALID_SOCKET)
			{
				cl.Delete(pair);
				cout << "Client closed" << endl;
			}
		}
	}
	/*int index = WSAWaitForMultipleEvents(1, &mySocket.WSAEvent, true, 0, false);
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
			clients.Add(Client);
			cout << "Client accepted" << endl;
		}
		index = WSAWaitForMultipleEvents(1, &mySocket.WSAEvent, true, 0, false);
	}*/
}
void MyListenSocket::listenClients()
{

	/*
	int len = clients.size();
	for (int i = 0; i < len; i++)
	{
		MySocketPair client = clients.front();
		clients.pop();
		client.ReadAndWrite();
		if (client.client.Socket != INVALID_SOCKET && client.server.Socket != INVALID_SOCKET && !client.time.check())
		{
			clients.push(client);
		}
		else
		{
			client.Destroy();
			cout << "Client closed" << endl;
		}
	}*/
}
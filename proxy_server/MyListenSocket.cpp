#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "MyListenSocket.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;


MyListenSocket::MyListenSocket(int t,int port)
{
	timeout = t;
	MySocket* mySocket=new MySocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	sockaddr_in inetAddr;
	inetAddr.sin_family = AF_INET;
	inetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	inetAddr.sin_port = htons(port);
	if (bind(mySocket->Socket, (SOCKADDR*)&inetAddr, sizeof(inetAddr)))
		throw ExceptionBind(port);

	mySocket->WSAEvent = WSACreateEvent();
	WSAEventSelect(mySocket->Socket, mySocket->WSAEvent, FD_ACCEPT | FD_CLOSE);
	if (listen(mySocket->Socket, 10))
		throw ExceptionListen();
	MySocketPair* listenSocket=new MySocketPair(timeout,mySocket, mySocket);
	cl.Add(listenSocket);
}

MyListenSocket::MyListenSocket(int t)
{
	MyListenSocket(t,7777);
}

MySocketPair* MyListenSocket::onAccept(MySocket* client1)
{
	SOCKADDR_IN inetAddr;
	inetAddr.sin_family = AF_INET;
	inetAddr.sin_addr.s_addr = inet_addr("188.165.141.151");
	inetAddr.sin_port = htons(80);
	MySocket* client2=new MySocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP),FD_READ|FD_WRITE|FD_CONNECT);
	connect(client2->Socket, (struct sockaddr*)&inetAddr, sizeof(inetAddr));
	MySocketPair* back=new MySocketPair(timeout,client1, client2);
	return back;
}
void MyListenSocket::myAccept()
{
	std::pair<bool, MySocketPair*> ev = cl.WaitMultyEvent();
	if (ev.first)
	{
		MySocketPair* pair = ev.second;
		if (pair->server->Socket == pair->client->Socket && (pair->client->events[FD_ACCEPT_BIT] || pair->server->events[FD_ACCEPT_BIT]))
		{
			MySocket* client=new MySocket(accept(pair->client->Socket, NULL, NULL), FD_READ | FD_WRITE | FD_CONNECT);
			MySocketPair* Client = onAccept(client);
			cl.Add(Client);
			cout << "Client accepted" << endl;
		}
		else
		{
			pair->ReadAndWrite();
			if (pair->is_close)
			{
				cl.Delete(pair);
				cout << "Client closed" << endl;
			}
		}
	}
	else
	{
		//cl.DeleteAll();
	}
}

#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "MyListenSocket.h"
#include "Exceptions.h"
#include <iostream>
#include <memory>

using namespace std;


MyListenSocket::MyListenSocket(int t,int port)
{
	timeout = t;
	MySocket* mySocket=new MySocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP),timeout);
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
	cl.Add(mySocket);
}

MyListenSocket::MyListenSocket(int t)
{
	MyListenSocket(t,7777);
}

void MyListenSocket::onAccept(MySocket* listenSocket)
{
	SOCKADDR_IN inetAddr;
	inetAddr.sin_family = AF_INET;
	inetAddr.sin_addr.s_addr = inet_addr("188.165.141.151");
	inetAddr.sin_port = htons(80);
	MySocket* client1 = new MySocket(accept(listenSocket->Socket, NULL, NULL),FD_READ|FD_WRITE, timeout);
	MySocket* client2=new MySocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP),FD_READ|FD_WRITE|FD_CONNECT,timeout);
	connect(client2->Socket, (struct sockaddr*)&inetAddr, sizeof(inetAddr));
	client1->other = client2;
	client2->other = client1;
	cl.Add(client1);
	cl.Add(client2);
}
void MyListenSocket::myAccept()
{
	std::pair<bool, MySocket*> ev = cl.WaitMultyEvent();
	if (ev.first)
	{
		MySocket* pair = ev.second;
		if (pair->events[FD_ACCEPT_BIT])
		{
			//MySocket* client=new MySocket(accept(pair->Socket, NULL, NULL), FD_READ | FD_WRITE | FD_CONNECT);
			onAccept(pair);
			//cl.Add(client);
			//cout << "Client accepted" << endl;
		}
		else
		{
			pair->ReadAndWrite();
			if (pair->events[FD_CLOSE_BIT])
			{
				cl.Delete(pair);
				//cout << "Client closed" << endl;
			}
		}
	}
	else
	{
		//cl.DeleteAll();
	}
}

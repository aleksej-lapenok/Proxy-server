#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "httpListenSocket.h"

MySocketPair* httpListenSocket::onAccept(MySocket* client1)
{
	SOCKADDR_IN inetAddr;
	inetAddr.sin_family = AF_INET;
	inetAddr.sin_addr.s_addr = inet_addr("188.165.141.151");
	inetAddr.sin_port = htons(80);
	MySocket* client2 = new MySocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), FD_READ | FD_WRITE | FD_CONNECT);
	connect(client2->Socket, (struct sockaddr*)&inetAddr, sizeof(inetAddr));
	MySocketPair* back = new httpSocketPair(client1, client2);
	return back;
}
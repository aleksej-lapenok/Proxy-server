#include "httpListenSocket.h"

void httpListenSocket::onAccept(MySocket* listenSocket)
{
	MySocket* client1 = new httpSocket<true>(accept(listenSocket->Socket, NULL, NULL), FD_READ | FD_WRITE,timeout);
	MySocket* client2 = new httpSocket<false>(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), FD_READ | FD_WRITE | FD_CONNECT,timeout);
	client1->other = client2;
	client2->other = client1;
	cl.Add(client2);
	cl.Add(client1);
}
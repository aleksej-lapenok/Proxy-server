#include "httpListenSocket.h"

MySocketPair* httpListenSocket::onAccept(MySocket* client1)
{
	MySocket* client2 = new MySocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), FD_READ | FD_WRITE | FD_CONNECT);
	MySocketPair* back = new httpSocketPair(client1, client2);
	return back;
}
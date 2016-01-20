#pragma once

#include "MyListenSocket.h"
#include "httpSocketPair.h"

struct httpListenSocket :public MyListenSocket
{
	httpListenSocket() :MyListenSocket(){}
	httpListenSocket(int port):MyListenSocket(port){}
protected:
	virtual MySocketPair* onAccept(MySocket*);
};
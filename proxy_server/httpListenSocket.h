#pragma once

#include "MyListenSocket.h"
#include "httpSocket.h"

struct httpListenSocket :public MyListenSocket
{
	httpListenSocket(int t) :MyListenSocket(t){}
	httpListenSocket(int t,int port):MyListenSocket(t,port){}
protected:
	virtual void onAccept(MySocket*);
};
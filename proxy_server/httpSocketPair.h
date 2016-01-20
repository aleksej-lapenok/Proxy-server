#pragma once

#include "MySocketPair.h"
#include "httpRequest.h"
#include <string>
#include <iostream>

struct httpSocketPair :MySocketPair
{
	httpSocketPair(MySocket* first, MySocket* second):MySocketPair(first,second){}
protected:
	virtual void onReadClient();
	virtual void onReadServer();
private:
	httpRequest requestS,requestC;
	bool findRequst(std::string&,bool);
};
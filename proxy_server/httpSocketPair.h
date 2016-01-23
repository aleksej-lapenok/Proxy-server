#pragma once

#include "MySocketPair.h"
#include "httpRequestClient.h"
#include "httpRequestServer.h"
#include <string>
#include <iostream>

struct httpSocketPair :MySocketPair
{
	httpSocketPair(MySocket* first, MySocket* second);
protected:
	virtual void onReadClient();
	virtual void onReadServer();
private:
	httpRequestClient requestC;
	httpRequestServer requestS;
	std::string beginS = "", beginC = "";
	bool findRequst(std::string&,bool);
};
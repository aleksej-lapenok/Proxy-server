#pragma once
#include "MySocketPair.h"
#include <vector>

struct Clients
{
private:
	std::vector<MySocketPair*> clients;
	//std::vector<MySocket> servers;
	std::vector<WSAEVENT> events;
	void pushback(int index);
public:
	Clients();
	Clients(MySocketPair*);
	void Add(MySocketPair*);
	void Delete(MySocketPair*);
	std::pair<bool, MySocketPair*> Clients::WaitMultyEvent();
	~Clients();
};
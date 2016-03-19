#pragma once
#include "MySocketPair.h"
#include <vector>

struct Clients
{
private:
	std::vector<MySocketPair*> clients;
	//std::vector<MySocket> servers;
	std::vector<WSAEVENT> events;
	void pushback(MySocketPair*);
	int cout_clients = 0;
public:
	Clients();
	Clients(MySocketPair*);
	void Add(MySocketPair*);
	void Delete(MySocketPair*);
	std::pair<bool, MySocketPair*> WaitMultyEvent();
	~Clients();
};

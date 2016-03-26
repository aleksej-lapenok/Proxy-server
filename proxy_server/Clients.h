#pragma once
#include "MySocket.h"
#include <memory>
#include <vector>

struct Clients
{
private:
	std::vector<std::unique_ptr<MySocket>> clients;
	//std::vector<MySocket> servers;
	const int TIMEOUT = 17 * 1000;
	std::vector<WSAEVENT> events;
	size_t cout_clients = 0;

	void CheckAndDelete(MySocket*);
	void pushback(MySocket*);
	void Add(std::unique_ptr<MySocket>);
public:
	Clients();
	Clients(MySocket*);
	void Add(MySocket*);
	void Delete(MySocket*);
	std::pair<bool, MySocket*> WaitMultyEvent();
	~Clients();
};

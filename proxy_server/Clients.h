#include "MySocket.h"
#include <list>

using namespace std;

struct Clients
{
	struct iterator
	{
		list<MySocket>::iterator its;
		iterator(list<MySocket>::iterator);
		MySocket& operator*() const;
		MySocket& operator->() const;
		iterator operator--();
		iterator operator++();
		iterator operator--(int);
		iterator operator++(int);
	};
	list<MySocket> clients;
	Clients();
	Clients(MySocket&);
	void Add(MySocket&);
	void Delete(MySocket&);
	pair<WSANETWORKEVENTS,MySocket&> WaitMultyEvent();

};
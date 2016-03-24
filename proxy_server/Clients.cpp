#include "Clients.h"
#include <assert.h>
#include <iostream>
#include <ctime>


Clients::Clients():clients()
{
}

Clients::Clients(MySocket* client) : clients()
{
	Add(client);
}

void Clients::Add(MySocket* client)
{
	std::unique_ptr<MySocket> client2(client);
	Add(std::move(client2));
}

void Clients::Add(std::unique_ptr<MySocket> client)
{
	events.push_back(client->WSAEvent);
	clients.push_back(std::move(client));
}

template<typename T>
bool operator==(std::unique_ptr<T> const& ptr1, T* ptr2)
{
	return ptr1.get() == ptr2;
}

template<typename T>
bool operator==(T* ptr2, std::unique_ptr<T> const& ptr1)
{
	return ptr1 == ptr2;
}

void Clients::Delete(MySocket* forDel)
{
	if (forDel == forDel->other || !forDel->other)
	{
		pushback(forDel);
		return;
	}
	std::vector<std::unique_ptr<MySocket>>::iterator first = find(clients.begin(), clients.end(), forDel);
	MySocket* other = forDel->other;
	if (first != clients.end())
	{
		std::vector<WSAEVENT>::iterator firstEvent = find(events.begin(), events.end(), forDel->WSAEvent);
		clients.erase(first);
		events.erase(firstEvent);
	}
	std::vector<std::unique_ptr<MySocket>>::iterator second = find(clients.begin(), clients.end(), forDel->other);
	if (second != clients.end())
	{
		std::vector<WSAEVENT>::iterator secondEvent = find(events.begin(), events.end(), (*second)->WSAEvent);
		clients.erase(second);
		events.erase(secondEvent);
	}
}

std::pair<bool, MySocket*> Clients::WaitMultyEvent()
{
	WSAEVENT* array_event = events.data();
	if (cout_clients != clients.size() - 1)
	{
		std::cout << "count of clients: " << clients.size() - 1 << std::endl;
		cout_clients = clients.size() - 1;
	}
	clock_t start = clock();
	int index = WSAWaitForMultipleEvents(min(events.size(),WSA_MAXIMUM_WAIT_EVENTS-1), array_event, false, TIMEOUT, false);
	if (index != WSA_WAIT_FAILED && index != WSA_WAIT_TIMEOUT)
	{
		index -= WSA_WAIT_EVENT_0;
		MySocket* pair = clients[index].get();
		/*if (index != 0 && pair->client != pair->server)
		{
			clients[0]->timeout -= (clock() - start);
			if (clients[0].get() != pair && clients[0]->timeout < 0)
			{
				Delete(clients[0].get());
			}
		}*/
		//pair->updateTimer();
		bool flage = false;
		flage = pair->checkEvent();
		pushback(pair);
		return std::pair<bool, MySocket*>(flage, pair);
	}
	if (index == WSA_WAIT_TIMEOUT)
	{
		DeleteGlobal();//TODO: write to delete first WSA_MAXIMUM_WAIT_EVENTS
		return std::pair<bool, MySocket*>(false, clients[0].get());
	}
	std::cout << WSAGetLastError()<<std::endl;
	return std::pair<bool, MySocket*>(false, clients[0].get());
}

void Clients::pushback(MySocket* forDel)
{
	std::vector<WSAEVENT>::iterator j = events.begin();
	std::unique_ptr<MySocket> forDel_ptr;

	std::vector<std::unique_ptr<MySocket>>::iterator client = std::find(clients.begin(), clients.end(), forDel);
	std::vector<WSAEVENT>::iterator wsaevents = std::find(events.begin(), events.end(), forDel->WSAEvent);
	forDel_ptr = std::move(*client);
	clients.erase(client);
	events.erase(wsaevents);
	this->Add(std::move(forDel_ptr));
}

void Clients::DeleteGlobal()
{
	int i = 0;
	for (std::vector<std::unique_ptr<MySocket>>::iterator j = clients.begin(); j != clients.end() && i < WSA_MAXIMUM_WAIT_EVENTS; i++)
	{
		MySocket* forDel = j->get();
		if (forDel == forDel->other || !forDel->other)
		{
			j++;
			continue;
		}
		MySocket* other = forDel->other;
		if (j != clients.end())
		{
			std::vector<WSAEVENT>::iterator firstEvent = find(events.begin(), events.end(), forDel->WSAEvent);
			j=clients.erase(j);
			events.erase(firstEvent);
		}
		std::vector<std::unique_ptr<MySocket>>::iterator second = find(clients.begin(), clients.end(), forDel->other);
		if (j == second)
			j++;
		if (second != clients.end())
		{
			std::vector<WSAEVENT>::iterator secondEvent = find(events.begin(), events.end(), (*second)->WSAEvent);
			clients.erase(second);
			events.erase(secondEvent);
		}
	}
}

Clients::~Clients()
{
	clients.clear();
}

#include "Clients.h"
#include <assert.h>
#include <iostream>
#include <ctime>


Clients::Clients():clients()
{
}

Clients::Clients(MySocketPair* client) : clients()
{
	Add(client);
}

void Clients::Add(MySocketPair* client)
{
	clients.push_back(client);
	events.push_back(client->server->WSAEvent);
	events.push_back(client->client->WSAEvent);
}

void Clients::Delete(MySocketPair* forDel)
{
	if (forDel->client == forDel->server || forDel->timeout > 0)
	{
		pushback(forDel);
		return;
	}
	std::vector<WSAEVENT>::iterator j = events.begin();
	for (std::vector<MySocketPair*>::iterator i = clients.begin();  i != clients.end(); i++,j++,j++)
	{
		if (*i == forDel)
		{
			clients.erase(i);
			j=events.erase(j);
			events.erase(j);
			delete forDel;
			break;
		}
	}
}

std::pair<bool,MySocketPair*> Clients::WaitMultyEvent()
{
	WSAEVENT* array_event = events.data();
	if (cout_clients != clients.size() - 1)
	{
		std::cout << "count of clients: " << clients.size() - 1 << std::endl;
		cout_clients = clients.size() - 1;
	}
	clock_t start = clock();
	int index = WSAWaitForMultipleEvents(min(events.size(),WSA_MAXIMUM_WAIT_EVENTS-1), array_event, false, clients[0]->timeout, false);
	if (index != WSA_WAIT_FAILED && index != WSA_WAIT_TIMEOUT)
	{
		index -= WSA_WAIT_EVENT_0;
		MySocketPair* pair = clients[index >> 1];
		if (index != 0 && pair->client != pair->server)
		{
			clients[0]->timeout -= (clock() - start);
			if (clients[0] != pair && clients[0]->timeout < 0)
			{
				Delete(clients[0]);
			}
		}
		pair->updateTimer();
		bool flage = false;
		if (index % 2 == 0)
		{
			flage = pair->server->checkEvent();
		}
		else
		{
			flage = pair->client->checkEvent();
		}
		pushback(pair);
		return std::pair<bool, MySocketPair*>(flage, pair);
	}
	if (index == WSA_WAIT_TIMEOUT)
	{
		Delete(clients[0]);
		return std::pair<bool, MySocketPair*>(false, NULL);
	}
	return std::pair<bool, MySocketPair*>(false, clients[0]);
}

void Clients::pushback(MySocketPair* forDel)
{
	std::vector<WSAEVENT>::iterator j = events.begin();
	for (std::vector<MySocketPair*>::iterator i = clients.begin(); i != clients.end(); i++, j++, j++)
	{
		if (*i == forDel)
		{
			clients.erase(i);
			j = events.erase(j);
			events.erase(j);
			break;
		}
	}
	this->Add(forDel);
}
Clients::~Clients()
{
	for (std::vector<MySocketPair*>::iterator i = clients.begin(); i != clients.end();)
	{
		delete *i;
		i = clients.erase(i);
	}
}
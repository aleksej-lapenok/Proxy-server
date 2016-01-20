#include "Clients.h"
#include <assert.h>


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
	int index = WSAWaitForMultipleEvents(min(events.size(),WSA_MAXIMUM_WAIT_EVENTS), array_event, false, WSA_INFINITE, false);
	if (index != WSA_WAIT_FAILED && index != WSA_WAIT_TIMEOUT)
	{
		index -= WSA_WAIT_EVENT_0;
		MySocketPair* pair = clients[index >> 1];
		bool flage = false;
		if (index % 2 == 0)
		{
			flage=pair->server->checkEvent();
		}
		else
		{
			flage = pair->client->checkEvent();
		}
		pushback(index);
		return std::pair<bool, MySocketPair*>(flage, pair);
	}
	return std::pair<bool, MySocketPair*>(false, clients[0]);
}

void Clients::pushback(int index)
{
	MySocketPair* forDel = clients[index >> 1];
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
#include "Clients.h"


Clients::Clients():clients()
{
}

Clients::Clients(MySocketPair& client) : clients()
{
	Add(client);
}

void Clients::Add(MySocketPair& client)
{
	clients.push_back(client);
	//servers.push_back(client.server);
	events.push_back(client.server.WSAEvent);
	events.push_back(client.client.WSAEvent);
}

void Clients::Delete(MySocketPair& forDel)
{
	std::vector<WSAEVENT>::iterator j = events.begin();
	for (std::vector<MySocketPair>::iterator i = clients.begin();  i != clients.end(); i++,j++,j++)
	{
		if (i->client.Socket == forDel.client.Socket)
		{
			clients.erase(i);
			j=events.erase(j);
			events.erase(j);
			break;
		}
	}
}

std::pair<bool,MySocketPair&> Clients::WaitMultyEvent()
{
	WSAEVENT* array_event = events.data();
	int index = WSAWaitForMultipleEvents(events.size(), array_event, false, 30*1000, false);
	if (index != WSA_WAIT_FAILED && index != WSA_WAIT_TIMEOUT)
	{
		MySocketPair& pair = clients[index >> 1];
		index -= WSA_WAIT_EVENT_0;
		bool flage = false;
		if (index % 2 == 0)
		{
			flage=pair.server.checkEvent();
		}
		else
		{
			flage = pair.client.checkEvent();
		}
		//pushback(index);
		return std::pair<bool, MySocketPair&>(flage, pair);
	}
	return std::pair<bool, MySocketPair&>(false, clients[0]);
}

void Clients::pushback(int index)
{
	MySocketPair& pair = clients[index >> 1];
	std::vector<WSAEVENT>::iterator j = events.begin();
	for (std::vector<MySocketPair>::iterator i = clients.begin(); i != clients.end(); i++, j++, j++)
	{
		if (i->client.Socket == pair.client.Socket)
		{
			clients.erase(i);
			j = events.erase(j);
			events.erase(j);
			break;
		}
	}
	this->Add(pair);
}
Clients::~Clients()
{
	for (std::vector<MySocketPair>::iterator i = clients.begin(); i != clients.end();)
	{
		i->Destroy();
		i = clients.erase(i);
	}
}
/*
pair<WSANETWORKEVENTS, MySocket&> Clients::WaitMultyEvent()
{
	WSANETWORKEVENTS wsaNetwork;
	for (vector<MySocket>::iterator i = clients.begin(); i != clients.end(); i++)
	{
		int index = WSAWaitForMultipleEvents(1, &i->WSAEvent, true, 0, false);
		if (index != WSA_WAIT_FAILED && index != WSA_WAIT_TIMEOUT)
		{
			WSAEnumNetworkEvents(i->Socket, i->WSAEvent, &wsaNetwork);
			return pair<WSANETWORKEVENTS, MySocket&>(wsaNetwork, *i);
		}
	}
	return pair<WSANETWORKEVENTS, MySocket&>(wsaNetwork, MySocket());
}*/
/*
Clients::iterator(list<MySocket>::iterator iterat)
{
	
}
MySocket Clients::iterator::operator*() const
{
	return *its;
}
MySocket Clients::iterator::operator->() const
{
	return *its;
}
Clients::iterator Clients::iterator::operator--()
{
	return --its;
}
Clients::iterator Clients::iterator::operator++()
{
	return ++its;
}
Clients::iterator Clients::iterator::operator--(int)
{
	return its--;
}
Clients::iterator Clients::iterator::operator++(int)
{
	return its++;
}*/
#include "Clients.h"


Clients::Clients()
{

}
Clients::Clients(MySocket& client)
{
	Add(client);
}
void Clients::Add(MySocket& client)
{
	clients.push_back(client);
}
void Clients::Delete(MySocket& forDel)
{
	for (list<MySocket>::iterator i = clients.begin(); i != clients.end(); i++)
	{
		if (i->Socket == forDel.Socket)
		{
			i->Destroy();
			clients.erase(i);
			break;
		}
	}
}

pair<WSANETWORKEVENTS, MySocket&> Clients::WaitMultyEvent()
{
	WSANETWORKEVENTS wsaNetwork;
	for (list<MySocket>::iterator i = clients.begin(); i != clients.end(); i++)
	{
		int index = WSAWaitForMultipleEvents(1, &i->WSAEvent, true, 0, false);
		if (index != WSA_WAIT_FAILED && index != WSA_WAIT_TIMEOUT)
		{
			WSAEnumNetworkEvents(i->Socket, i->WSAEvent, &wsaNetwork);
			return pair<WSANETWORKEVENTS, MySocket&>(wsaNetwork, *i);
		}
	}
	return pair<WSANETWORKEVENTS, MySocket&>(wsaNetwork, MySocket());
}
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
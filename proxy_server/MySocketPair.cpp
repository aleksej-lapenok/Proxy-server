#include "MySocketPair.h"
#include <iostream>
#include <string>
#include <assert.h>


MySocketPair::MySocketPair(long timeout) :client(), server(), time(timeout)
{
}

MySocketPair::MySocketPair(long timeout,SOCKET client, SOCKET server) :client(client,FD_WRITE|FD_READ), server(server,FD_WRITE|FD_READ),time(timeout)
{
}

MySocketPair::MySocketPair(long timeout, MySocket client1, MySocket server1) : client(client1), server(server1),time(timeout)
{
}

void MySocketPair::Destroy()
{
	client.Destroy();
	server.Destroy();
}

void MySocketPair::ReadAndWrite()
{
	if (client.Socket == INVALID_SOCKET || server.Socket == INVALID_SOCKET)
		return;
	if (server.checkEvent() || client.checkEvent())
	{
		if (server.Socket != client.Socket)
		{
			time.update();
			bool flage = true;
			
			if (server.events[FD_CONNECT_BIT])
			{
				std::cout << "client connected" << std::endl;
				server.events[FD_CONNECT_BIT] = false;
				canBeRead = true;
			}
			
			if (server.events[FD_READ_BIT] && canBeRead /*&& client.events[FD_WRITE_BIT] */)
			{
				int lenRecv = recv(server.Socket, server.buffer, server.LEN, 0);
				int lenSend = send(client.Socket, server.buffer, lenRecv, 0);
				std::cout << "server to client: " << std::endl/* << std::string(server.buffer + 0, server.buffer + lenRecv) << std::endl << std::endl*/;
				server.events[FD_READ_BIT] = false;
				client.events[FD_WRITE_BIT] = false;
				flage = false;
				assert(lenRecv == lenSend);
			}
			if (client.events[FD_READ_BIT] && canBeRead /*&& server.events[FD_WRITE_BIT]*/)
			{
				int lenRecv = recv(client.Socket, client.buffer, client.LEN, 0);
				int lenSend = send(server.Socket, client.buffer, lenRecv, 0);
				std::cout << "client to server: " << std::endl /*<< std::string(client.buffer + 0, client.buffer + lenRecv) << std::endl << std::endl*/;
				flage = false;
				client.events[FD_READ_BIT] = false;
				assert(lenRecv == lenSend);
			}
			if (flage && (server.events[FD_CLOSE_BIT] || client.events[FD_CLOSE_BIT]))
			{
				Destroy();
			}
		}
		else
		{
			WSANETWORKEVENTS WSANetWorkServer;
			WSAEnumNetworkEvents(client.Socket, client.WSAEvent, &WSANetWorkServer);
			if (((WSANetWorkServer.lNetworkEvents & FD_CLOSE) && WSANetWorkServer.iErrorCode[FD_CLOSE_BIT] == 0))
			{
				Destroy();
				return;
			}
			if ((WSANetWorkServer.lNetworkEvents & FD_READ) && WSANetWorkServer.iErrorCode[FD_READ_BIT] == 0)
			{
				int lenRecv = recv(server.Socket, server.buffer, server.LEN, 0);
				send(client.Socket, server.buffer, lenRecv, 0);
				std::cout << "client to client: " << std::endl << std::string(server.buffer + 0, server.buffer + lenRecv) << std::endl << std::endl;
			}
		}
	}
}
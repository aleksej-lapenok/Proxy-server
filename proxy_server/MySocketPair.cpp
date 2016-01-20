#include "MySocketPair.h"
#include <iostream>
#include <string>
#include <assert.h>



MySocketPair::MySocketPair() :client(), server()
{
}

MySocketPair::MySocketPair(SOCKET client, SOCKET server)
{
	this->client = new MySocket(client, FD_WRITE | FD_READ);
	this->server = new MySocket(server, FD_WRITE | FD_READ);
}

MySocketPair::MySocketPair(MySocket* client1, MySocket* server1) 
{
	client = client1;
	server = server1;
}

void MySocketPair::Destroy()
{
	if (client != server)
	{
		delete client;
		delete server;
	}
	else
	{
		delete server;
	}
}

void MySocketPair::onReadClient()
{
	FILE* file_client;
	fopen_s(&file_client, file_name_client.c_str(), "a");
	if (server->len_buffer == 0)
	{
		server->len_buffer = recv(client->Socket, server->buffer, client->LEN, 0);
	}
	int lenSend = send(server->Socket, server->buffer, server->len_buffer, 0);
	std::cout << "client to server: " << std::endl/* << std::string(server->buffer + 0, server->buffer + server->len_buffer) << std::endl << std::endl*/;
	if(file_client)
		fwrite(server->buffer, sizeof(char), server->len_buffer, file_client);
	client->events[FD_READ_BIT] = false;
	if (WSAGetLastError() == 10035 || lenSend == -1)
	{
		server->events[FD_WRITE_BIT] = false;
	}
	else
	{
		assert(server->len_buffer == lenSend);
		server->len_buffer = 0;
	}
	fclose(file_client);
}

void MySocketPair::onReadServer()
{
	FILE* file_server;
	fopen_s(&file_server, file_name_server.c_str(), "a");
	if (client->len_buffer == 0)
	{
		client->len_buffer = recv(server->Socket, client->buffer, server->LEN, 0);
	}
	int lenSend = send(client->Socket, client->buffer, client->len_buffer, 0);
	std::cout << "server to client: " << std::endl/* << std::string(client->buffer + 0, client->buffer + client->len_buffer) << std::endl << std::endl*/;
	fwrite(client->buffer, sizeof(char), client->len_buffer, file_server);
	server->events[FD_READ_BIT] = false;
	if (WSAGetLastError() == 10035 || lenSend == -1)
	{
		client->events[FD_WRITE_BIT] = false;
	}
	else
	{
		assert(client->len_buffer == lenSend);
		client->len_buffer = 0;
	}
	fclose(file_server);
}

void MySocketPair::ReadAndWrite()
{
	
	if (client->Socket == INVALID_SOCKET || server->Socket == INVALID_SOCKET)
		return;
	//if (server->checkEvent() || client->checkEvent())
	{
		if (server->Socket != client->Socket)
		{
			bool flage = true;
			
			if (server->events[FD_CONNECT_BIT])
			{
				std::cout << "client connected" << std::endl;
				server->events[FD_CONNECT_BIT] = false;
				canBeRead = true;
			}
			
			if (server->events[FD_READ_BIT] && /*canBeRead &&*/ client->events[FD_WRITE_BIT])
			{
				onReadServer();
				flage = false;
			}
			if (client->events[FD_READ_BIT] && /*canBeRead &&*/ server->events[FD_WRITE_BIT])
			{
				onReadClient();
				flage = false;
			}
			if (flage && (server->events[FD_CLOSE_BIT] || client->events[FD_CLOSE_BIT]))
			{
				is_close = true;
				//Destroy();
			}
		}
		else
		{
			WSANETWORKEVENTS WSANetWorkServer;
			WSAEnumNetworkEvents(client->Socket, client->WSAEvent, &WSANetWorkServer);
			if (((WSANetWorkServer.lNetworkEvents & FD_CLOSE) && WSANetWorkServer.iErrorCode[FD_CLOSE_BIT] == 0))
			{
				Destroy();
				return;
			}
			if ((WSANetWorkServer.lNetworkEvents & FD_READ) && WSANetWorkServer.iErrorCode[FD_READ_BIT] == 0)
			{
				int lenRecv = recv(server->Socket, server->buffer, server->LEN, 0);
				send(client->Socket, server->buffer, lenRecv, 0);
				std::cout << "client to client: " << std::endl << std::string(server->buffer + 0, server->buffer + lenRecv) << std::endl << std::endl;
			}
		}
	}
}

MySocketPair::~MySocketPair()
{
	Destroy();
}
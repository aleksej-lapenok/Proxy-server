#include "httpSocketPair.h"
#include <iostream>
#include <assert.h>

void httpSocketPair::onReadClient()
{
	
	if (server->len_buffer == 0)
	{
		server->len_buffer = recv(client->Socket, server->buffer, client->LEN, 0);
	}
	if (requestC.isEmpty())
	{
		std::string begin = std::string(server->buffer + 0, server->buffer + server->len_buffer);
		if (findRequst(begin,true))
		{
			begin = "";
			FILE* file_client;
			fopen_s(&file_client, file_name_client.c_str(), "w");
			if (file_client)
				fprintf(file_client,"%s", requestC.toString().c_str());
			fclose(file_client);
		}
	}
	
	int lenSend = send(server->Socket, server->buffer, server->len_buffer, 0);
	std::cout << "client to server" << std::endl/* << std::string(server->buffer + 0, server->buffer + server->len_buffer) << std::endl << std::endl*/;
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
}

void httpSocketPair::onReadServer()
{
	if (client->len_buffer == 0)
	{
		client->len_buffer = recv(server->Socket, client->buffer, server->LEN, 0);
	}
	if (requestS.isEmpty())
	{
		std::string begin = std::string(client->buffer + 0, client->buffer + client->len_buffer);
		if (findRequst(begin, false))
		{
			begin = "";
			FILE* file_server;
			fopen_s(&file_server, file_name_server.c_str(), "w");
			if (file_server)
				fprintf(file_server, "%s", requestS.toString().c_str());
			fclose(file_server);
		}
	}
	int lenSend = send(client->Socket, client->buffer, client->len_buffer, 0);
	std::cout << "server to client" << std::endl/* << std::string(client->buffer + 0, client->buffer + client->len_buffer) << std::endl << std::endl*/;
	
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
}

bool httpSocketPair::findRequst(std::string& str,bool flage)
{
	if (flage)
	{
		if (!requestC.isEmpty())
		{
			return true;
		}
	}
	else
	{
		if (!requestS.isEmpty())
		{
			return true;
		}
	}
	size_t i = str.find("\r\n\r\n");
	if (i == std::string::npos)
	{
		return false;
	}
	if (flage)
	{
		requestC.setStr(str.substr(0, i));
	}
	else
	{
		requestS.setStr(str.substr(0, i));
	}
	return true;
}
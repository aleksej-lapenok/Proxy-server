#include "MySocketPair.h"
#include <iostream>
#include <string>


MySocketPair::MySocketPair() :client(), server()
{
}

MySocketPair::MySocketPair(SOCKET client, SOCKET server) :client(client,FD_WRITE|FD_READ), server(server,FD_WRITE|FD_READ)
{
}

MySocketPair::MySocketPair(MySocket client1, MySocket server1) : client(client1), server(server1)
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
	int indexServer = WSAWaitForMultipleEvents(1, &server.WSAEvent, true, 0, false);
	int indexClient = WSAWaitForMultipleEvents(1, &client.WSAEvent, true, 0, false);
	if ((indexServer != WSA_WAIT_FAILED && indexServer != WSA_WAIT_TIMEOUT) || (indexClient != WSA_WAIT_FAILED && indexClient != WSA_WAIT_TIMEOUT))
	{
		if (server.Socket != client.Socket)
		{
			WSANETWORKEVENTS WSANetWorkServer, WSANetWorkClient;
			WSAEnumNetworkEvents(server.Socket, server.WSAEvent, &WSANetWorkServer);
			WSAEnumNetworkEvents(client.Socket, client.WSAEvent, &WSANetWorkClient);
			if ((WSANetWorkClient.lNetworkEvents & FD_CONNECT) && WSANetWorkClient.iErrorCode[FD_CONNECT_BIT]==0 || (WSANetWorkServer.lNetworkEvents & FD_CONNECT) && WSANetWorkServer.iErrorCode[FD_CONNECT_BIT]==0)
			{
				std::cout << "client connected"<<std::endl;
				canBeRead = true;
			}
			
			if ((WSANetWorkServer.lNetworkEvents & FD_READ) && WSANetWorkServer.iErrorCode[FD_READ_BIT] == 0 && canBeRead /*&& (WSANetWorkClient.lNetworkEvents & FD_WRITE) && WSANetWorkClient.iErrorCode[FD_WRITE_BIT] == 0*/)
			{
				int lenRecv = recv(server.Socket, server.buffer, server.LEN, 0);
				if (lenRecv != -1)
				{
					int LenSend = send(client.Socket, server.buffer, lenRecv, 0);
					std::cout << "server to client: " << std::endl << std::string(server.buffer + 0, server.buffer + lenRecv) << std::endl << std::endl;
				}
			}
			if (/*(WSANetWorkServer.lNetworkEvents & FD_WRITE) && WSANetWorkServer.iErrorCode[FD_WRITE_BIT] == 0 && */(WSANetWorkClient.lNetworkEvents & FD_READ) && WSANetWorkClient.iErrorCode[FD_READ_BIT] == 0 && canBeRead)
			{
				int lenRecv = recv(client.Socket, client.buffer, client.LEN, 0);
				int LenSend = send(server.Socket, client.buffer, lenRecv, 0);
				std::cout << "client to server: " << std::endl << std::string(client.buffer + 0,client.buffer+lenRecv)  << std::endl << std::endl;
			}
			if (((WSANetWorkServer.lNetworkEvents & FD_CLOSE) && WSANetWorkServer.iErrorCode[FD_CLOSE_BIT] == 0) || ((WSANetWorkClient.lNetworkEvents & FD_CLOSE) && WSANetWorkClient.iErrorCode[FD_CLOSE_BIT] == 0))
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
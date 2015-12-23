#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <stdio.h>
#include <iostream>
#include <list>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

typedef struct _PerData
{
	int len = 0;
	int  ClientAddrLen;
	SOCKET  ClientSocket;
	char chBuf[1024];
	WSABUF  Buf;
	int time = 0;
	bool flage = false;
	WSAOVERLAPPED ol;
	DWORD NumBytes;
	~_PerData()
	{
	}
} PerData;
HANDLE ComPort;
/*
DWORD _stdcall ServerWorkThread(LPVOID)
{
	return 0;
}*/
void main()
{
	WSADATA Wsadata;

	SYSTEM_INFO Info;

	
	SOCKET ListenSocket;
	HANDLE ComPort;
	
	SOCKADDR_IN ClientAddr;

	LPFN_ACCEPTEX lpfnAcceptEx = NULL;
	LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockaddrs = NULL;

	GUID  GuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
	GUID  GuidAcceptEx = WSAID_ACCEPTEX;

	
	DWORD NumBytes;
	DWORD flag;
	int rc, i;

	char *ip;
	u_short port;
	hostent *thisHost;

	if (WSAStartup(MAKEWORD(2, 2), &Wsadata))
	{
		cout << " !" << endl;
		getchar();
	}
	ComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NULL);
	if (ComPort == INVALID_HANDLE_VALUE)
	{
		cout << " " << endl;
		getchar();
	}

	GetSystemInfo(&Info);
	/*for (i = 0; i<Info.dwNumberOfProcessors * 2; i++)
	{
		HANDLE Handle;
		Handle = CreateThread(NULL, 0, ServerWorkThread, ComPort, 0, NULL);
		CloseHandle(Handle);
	}*/

	ListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << " ! ：" << WSAGetLastError() << endl;
		exit(0);
	}

	port = 27015;
	thisHost = gethostbyname("");
	ip = inet_ntoa(*(struct in_addr *) *thisHost->h_addr_list);

	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_port = htons(port);
	ClientAddr.sin_addr.s_addr = inet_addr(ip);


	if (bind(ListenSocket, (SOCKADDR*)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR)
	{
		cout << " ! ：" << WSAGetLastError() << endl;
		getchar();
		return;
	}

	rc = WSAIoctl(ListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidAcceptEx,
		sizeof(GuidAcceptEx),
		&lpfnAcceptEx,
		sizeof(lpfnAcceptEx),
		&NumBytes,
		NULL,
		NULL);
	if (rc == SOCKET_ERROR)
	{
		cout << " lpfnAcceptEx ! ：" << WSAGetLastError() << endl;
		getchar();
		return;
	}
	if (listen(ListenSocket, 5) == SOCKET_ERROR)
	{
		cout << " ! ：" << WSAGetLastError() << endl;
		getchar();
		return;
	}

	cout << "Listening on address: " << ip << ":" << port<<endl;
		
	list<PerData*> sockets;
	while (1)
	{
		PerData* PD=new PerData();
		memset(&PD->ol, 0, sizeof(WSAOVERLAPPED));
		PD->ol.hEvent = WSACreateEvent();
		DWORD tran = -1;
		PD->ClientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (PD->ClientSocket == INVALID_SOCKET)
		{
			cout << " ! ：" << WSAGetLastError() << endl;
			exit(0);
		}
		PD->Buf.buf = PD->chBuf;
		char* buf = PD->Buf.buf;
		for (int i = 0; i < 1024; i++)
		{
			buf[i] = 0;
		}
		PD->Buf.len = 10;
		lpfnAcceptEx(ListenSocket,
			PD->ClientSocket,
			PD->Buf.buf, 10,
			sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16,
			&PD->NumBytes,
			(WSAOVERLAPPED*)&PD->ol);
		
		sockets.push_back(PD);
		for (list<PerData*>::iterator i = sockets.begin(); i != sockets.end();)
		{
			int index = WSAWaitForMultipleEvents(1, &(*i)->ol.hEvent, 1, 1, 0);
			WSAResetEvent((*i)->ol.hEvent);
			WSAGetOverlappedResult((*i)->ClientSocket, &(*i)->ol, &tran, 0, &flag);
			//cout << WSAGetLastError() << endl;
			bool flage = false;
			if (tran)
			{
				(*i)->Buf.buf[tran] = 0;
				WSASend((*i)->ClientSocket, &(*i)->Buf, 1, &tran, 0, &(*i)->ol, NULL);
				
				char* buf = (*i)->Buf.buf;
				for (int i = 0; i < 1024; i++)
				{
					if (buf[i] != 0)
						flage = true;
					buf[i] = 0;
				}
				if (flage)
				{
					(*i)->flage = true;
					(*i)->time = 0;
				}
				
			}
			if (!flage)
			{
				if ((*i)->len == -1)
				{
					(*i)->time++;
				}
				if (!(*i)->flage)
				{
					(*i)->time += 1000;
				}
				if ((*i)->time > 1 * 60 * 1000)
				{
					closesocket((*i)->ClientSocket);
					delete *i;
					i = sockets.erase(i);
					continue;
				}
			}
			
			(*i)->len = WSARecv((*i)->ClientSocket, &(*i)->Buf, 1, &tran, &flag, &(*i)->ol, NULL);
			i++;
		}
	}
}
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <winsock2.h>
#include <iostream>
#include <thread>
#include "Clients.h"
#include "MyListenSocket.h"

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

bool flage = true;

int server()
{
	WSADATA wsadata;

	WSAStartup(MAKEWORD(2, 2), &wsadata);

	MyListenSocket server(2725,60*1000);
	cout << "Server started" << endl << endl;
	while (flage)
	{
		server.myAccept();
	}
	cout << "Server stopped";
	return 0;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	thread thr(server);
	int a;
	cin >> a;
	flage = false;
	thr.join();
	return a;
}


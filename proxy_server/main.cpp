#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <winsock2.h>
#include <iostream>
#include <thread>
#include "httpListenSocket.h"
#include <atomic>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

// TODO: use std::atomic<bool>
std::atomic<bool> flage = true;

int server()
{
	WSADATA wsadata;

	WSAStartup(MAKEWORD(2, 2), &wsadata);

	httpListenSocket server(1000*3,2725);
	cout << "Server started" << endl;
	while (flage)
	{
		server.myAccept();
	}
	cout << "Server stopped"<<endl;
	return 0;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	thread thr(server);
	bool isContinue = true;
	while (isContinue)
	{
		int a;
		cin >> a;
		switch (a)
		{
		case 0:
			flage = false;
			thr.join();
			isContinue = false;
			break;
		case 1:
			flage = false;
			thr.join();
			isContinue = true;
			flage = true;
			thr = std::thread(server);
			break;
		default:
			cout << "invalid code: " << endl << "		0 to stop server" << endl << "		1 to restart server"<<endl;
			break;
		}
	}
	return 0;
}


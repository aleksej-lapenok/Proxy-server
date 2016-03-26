#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "httpSocket.h"
#include <iostream>
#include <assert.h>
#include <WinSock2.h>
#include <stdio.h>
#include <assert.h>

httpSocket<true>::httpSocket(SOCKET client, int WSAevent,int timeout) : MySocket(client, WSAevent,timeout)
{
	request = new httpRequestClient();
}

httpSocket<false>::httpSocket(SOCKET client, int WSAevent,int timeout) : MySocket(client, WSAevent,timeout)
{
	events[FD_WRITE_BIT] = true;
	request = new httpRequestServer();
}

void httpSocket<true>::onRead()
{
	httpSocket<true>* other = static_cast<httpSocket<true>*>(this->other);
	httpRequestClient* request = static_cast<httpRequestClient*>((this->request));
	if (len_buffer <= 0 || request->stat==request->NON)
	{
		len_buffer = recv(Socket, buffer, LEN, 0);
		events[FD_READ_BIT] = false;
	}
	if (len_buffer == -1)
	{
		return;
	}
	if (request->stat == httpRequest::NON)
	{
		begin = begin + std::string(buffer + 0, buffer + len_buffer);
		len_buffer = 0;
		if (findRequest(begin))
		{
			if (request->stat == httpRequest::BAD)
			{
				events[FD_CLOSE_BIT] = true;
				other->events[FD_CLOSE_BIT] = true;
				return;
			}
			begin = "";
			SOCKADDR_IN inetAddr;
			inetAddr.sin_family = AF_INET;
			/*if (requestC.getMethod() == "CONNECT")
			{
				inetAddr.sin_addr.s_addr = inet_addr("93.171.164.251");
				inetAddr.sin_port = htons(3128);
			}
			else
			{*/
			/*this->is_close = true;
			return;*/
			try
			{
				struct hostent* res = gethostbyname(request->getUrl().c_str());
				if (!res)
					throw std::exception("Can't find this host");
				inetAddr.sin_addr.s_addr = *((unsigned long*)res->h_addr);
			}
			catch (...)
			{
				events[FD_CLOSE_BIT] = true;
				other->events[FD_CLOSE_BIT] = true;
				return;
			}

			inetAddr.sin_port = htons(request->getPort());
			//}

			connect(other->Socket, (struct sockaddr*)&inetAddr, sizeof(inetAddr));
			len_buffer = request->toString().size() + 4;
			std::string str_request = request->makeRequest();
			if (request->getMethod() != "CONNECT")
			{
				for (size_t i = 0; i < str_request.size(); i++)
				{
					buffer[i] = str_request[i];
				}
				for (int i = len_buffer - 4; i < len_buffer; i += 2)
				{
					buffer[i] = '\r';
					buffer[i + 1] = '\n';
				}
				other->events[FD_WRITE_BIT] = false;
				events[FD_READ_BIT] = true;
			}
			else
			{
				events[FD_CLOSE_BIT] = true;
				other->events[FD_CLOSE_BIT] = true;
			}
		}
		else
		{
			return;
		}
	}
	if(events[FD_WRITE_BIT])
	{
		int lenSend = send(other->Socket, buffer, len_buffer, 0);
		//std::cout << "client to server" << std::endl;
		if (WSAGetLastError() == 10035 || lenSend == -1)
		{
			other->events[FD_WRITE_BIT] = false;
			events[FD_READ_BIT] = true;
		}
		else
		{
			assert(len_buffer == lenSend);
			len_buffer = 0;
		}
	}
}

void httpSocket<false>::onRead()
{
	httpRequestServer* request = static_cast<httpRequestServer*>((this->request));
	if (len_buffer <= 0)
	{
		len_buffer = recv(Socket, buffer, LEN, 0);
		events[FD_READ_BIT] = false;
	}
	if (len_buffer == -1)
	{
		return;
	}
	if (request->stat == request->NON)
	{
		begin = begin + std::string(buffer + 0, buffer + len_buffer);//len_buffer> size(buffer) bug!!!!
		if (findRequest(begin))
		{
			begin = "";
			/*if (request.getMethod() == "CONNECT")
			{
				std::cout << requestC.toString() << std::endl;
				std::cout << requestS.toString() << std::endl;
				std::cout << std::endl;
			}*/
		}
	}
	if (events[FD_WRITE_BIT])
	{
		int lenSend = send(other->Socket, buffer, len_buffer, 0);
		if (WSAGetLastError() == 10035 || lenSend == -1)
		{
			events[FD_READ_BIT] = true;
			other->events[FD_WRITE_BIT] = false;
		}
		else
		{
			assert(len_buffer == lenSend);
			len_buffer = 0;
		}
	}
}

void httpSocket<false>::onConnected()
{
	httpSocket<true>* other = static_cast<httpSocket<true>*>(this->other);
	httpRequestClient& requestC= *dynamic_cast<httpRequestClient*>((other->request));
	if (requestC.getMethod() == "CONNECT")
	{
		std::string answer = "HTTP/1.1 200 Connection established\r\n\r\n";
		len_buffer = answer.size();
		for (size_t i = 0; i < answer.size(); i++)
		{
			buffer[i] = answer[i];
		}
		onRead();
	}
}

void httpSocket<true>::onConnected()
{
	//throw new std::exception("You are very strange person");
}

template<bool flage>
bool httpSocket<flage>::findRequest(std::string& str)
{
	if (request->stat != request->NON)
	{
		return true;
	}
	size_t i = str.find("\r\n\r\n");
	if (i == std::string::npos)
	{
		return false;
	}
	request->setStr(str.substr(0, i));
	return true;
}

template<bool flage>
httpSocket<flage>::~httpSocket()
{
	delete request;
}

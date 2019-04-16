//server
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <vector>

using namespace std;

vector<SOCKET> connections;

void clientHandlerThread(int client)
{
	char buffer[256];

	while (true)
	{
		recv(connections[client], buffer, sizeof(buffer), NULL);

		for (int i = 0 ; i < connections.size(); i++)
		{
			if (i != client) //skip sending message to user who sent it
			{
				send(connections[i], buffer, sizeof(buffer), NULL);
			}
		}
	}
}

int main()
{
	//start up winsock
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) //if anything else than zero, startup fails
	{
		MessageBoxA(NULL, "Winsock startup failed!\n", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr; //address that we will bind our listening socket to
	int addrlen = sizeof(addr);//length of addrs (required to accept call)
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");//broadcast locally
	addr.sin_port = htons(1111);//port
	addr.sin_family = AF_INET; //IPv4 Socket

	cout << "Listening...\n";
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);//create socket to listen
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));//bind address to socket
	listen(sListen, SOMAXCONN);//socket is now listening for connections, somaxconn limits to max connections


	//handles new connections
	while (true)
	{
		SOCKET newConnection;//socket to hold the client's connection
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);

		if (newConnection == 0)
		{
			cout << "\nFailed to accept the clients connection.\n";
		}
		else //if client connected properly
		{
			//send message
			connections.push_back(newConnection);//store connection
			cout << "\nClient connected!\nNumber of clients is: " << connections.size();
			char MOTD[256] = "Welcome! *Insert Message of the Day here*.";
			send(newConnection, MOTD, sizeof(MOTD), NULL);//sizeof returns the number of bytes, char = 1, int  = 4 etc, make sure to use accordingly
			CreateThread(NULL,NULL, (LPTHREAD_START_ROUTINE)clientHandlerThread, (LPVOID)(connections.size() - 1), NULL, NULL);
		}
	}
	


	system("pause");
	return 0;
}
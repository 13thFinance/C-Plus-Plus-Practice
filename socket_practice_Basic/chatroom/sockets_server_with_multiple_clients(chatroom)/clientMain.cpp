#define _WINSOCK_DEPRECATED_NO_WARNINGS

//client

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

using namespace std;

SOCKET connection;

//receives messages from other users
void clientHandlerThread()
{
	char buffer[256];

	while (true)
	{
		recv(connection, buffer, sizeof(buffer), NULL);
		cout << buffer << endl;
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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");//connecting to self  locally
	addr.sin_port = htons(1111);//port
	addr.sin_family = AF_INET; //IPv4 Socket

	connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		MessageBoxA(NULL, "Failed to establish connection!\n", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	cout << "Connected!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandlerThread, NULL, NULL, NULL);

	char buffer[256];

	//keep getting client input and sending it to server
	//server will handle distribuiting it to other users
	while (true)
	{
		cin.getline(buffer, sizeof(buffer));
		send(connection, buffer, sizeof(buffer), NULL);
		Sleep(10);//not super necessary
	}




	system("pause");

	return 0;
}
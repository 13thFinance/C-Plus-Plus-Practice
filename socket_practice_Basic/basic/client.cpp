#define _WINSOCK_DEPRECATED_NO_WARNINGS

//client

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

using namespace std;

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

	SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		MessageBoxA(NULL, "Failed to establish connection!\n", "Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	cout << "Connected!\n";

	char MOTD[256];

	recv(connection, MOTD, sizeof(MOTD), NULL);;
	cout << "MOTD: " << MOTD << endl;



	system("pause");

	return 0;
}
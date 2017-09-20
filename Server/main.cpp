
#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>

#include "lib_SK5_BASE.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	std::cout << "SK5 BASE\n" << std::endl;

	// Hey! Windows je fais du reseau !
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);

	// Variables Parametres
	int port_listen_SK5 = 40001;   // SK5 LAN/WAN
	int port_listen_CLIENT = 9082; // LAN/WAN

	// Affichage des ports
	std::cout << "---------------------" << std::endl;
	std::cout << "| SK5 PORT: 40001   |" << std::endl;
	std::cout << "| CLIENT PORT: 9082 |" << std::endl;
	std::cout << "| HOSTNAME FOR EXIT |" << std::endl;
	std::cout << "| www.exitsoft.sk5  |" << std::endl;
	std::cout << "---------------------\n" << std::endl;

	SOCKET      s_CLIENT;
	SOCKET      serverSocket_CLIENT;
	SOCKADDR_IN addr_CLIENT;
	INT         addrSize_CLIENT;
	if(!StartServer(port_listen_CLIENT, &serverSocket_CLIENT, &addr_CLIENT, &addrSize_CLIENT))
		return 1;

	SOCKET      s_SK5;
	SOCKET      serverSocket_SK5;
	SOCKADDR_IN addr_SK5;
	INT         addrSize_SK5;
	if(!StartServer(port_listen_SK5, &serverSocket_SK5, &addr_SK5, &addrSize_SK5))
		return 2;

	threaddata d;
	int ok = 0;

	while(1)
	{
		ok = 0;
		s_SK5 = accept(serverSocket_SK5, (SOCKADDR *) &addr_SK5, &addrSize_SK5);
		if(s_SK5 != INVALID_SOCKET)
		{
			ok++;
			printf("SK5 Connect OK\n");
		}

		s_CLIENT = accept(serverSocket_CLIENT, (SOCKADDR *) &addr_CLIENT, &addrSize_CLIENT);
		if(s_CLIENT != INVALID_SOCKET)
		{
			ok++;
			printf("Client Connect OK\n");
		}

		if(ok == 2)
		{
			printf("Client/SK5 Connect OK\n");
			d.x = s_CLIENT;
			d.y = s_SK5;
			create_sock_thread(d);
		}
	}



	system("\n\npause");
	return 0;
}
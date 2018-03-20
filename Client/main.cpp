
#include <iostream>
#include <string>
#include <sstream>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "lib_SK5.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	//FreeConsole();
	std::cout << "SK5 CLIENT COM/DECOM\n\n" << std::endl;

	// Hey! Windows je fais du reseau !
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	const char *ip_SK5 = "XXX.XXX.XXX.XXX";
	int port_SK5 = 40001; // 40001

	bool one_time = TRUE;

	while (1)
	{
		SOCKET sock;
		SOCKADDR_IN sin;
		sin.sin_family = AF_INET;

		InetPton(AF_INET, ip_SK5, &sin.sin_addr);

		sin.sin_port = htons(port_SK5);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
			return -1;

		if (connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR) // SK5 BASE CONNECTION OK
		{
			if(one_time)
			{
				printf("Connection OK with SK5 - BASE \n");
				one_time = TRUE;
			}

			create_sock_thread(sock);
		}

	}

	system("\n\npause");
	return 0;
}

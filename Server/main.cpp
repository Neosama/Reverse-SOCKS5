
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

#include <winsock2.h>
#include <ws2tcpip.h>

// Lib for Compress
#include "lz4.h"

#include "lib_SK5_BASE.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	std::cout << "SK5 SERVER V2\n" << std::endl;
	std::cout << "With LZ4 & IPv6 compatibility\n" << std::endl;

	/*
	const char* text = "GET / TCP www.google.com www.facebook.com www.secret.gov";
	std::cout << "Text size: " << strlen(text) << std::endl;

	int size_x = LZ4_compressBound(strlen(text));
	char *compressedData = new char[size_x];
	int compressedSize = LZ4_compress_default(text, compressedData, strlen(text), size_x);

	std::cout << "Compressed size: " << compressedSize << std::endl;
	std::cout << "Compressed DATA : " << compressedData << std::endl;

	if (compressedSize > strlen(text)) {
		int uu = compressedSize - strlen(text);
		std::cout << "DIFF SIZE : +" << uu << std::endl;
	}
	else {
		int cc = strlen(text) - compressedSize;
		std::cout << "DIFF SIZE : -" << cc << std::endl;
	}

	char tmp_buff[1024 * 4];
	memset(tmp_buff, 0, sizeof(tmp_buff));
	LZ4_decompress_safe(text, tmp_buff, strlen(text), sizeof(tmp_buff));

	printf("\nDECOMPRESSED (SIZE = %i) = \"%s\"\n", strlen(text), text);
	system("PAUSE");
	*/

	// Configuration
	int port_listen_SK5 = 40001;   // SK5 LAN/WAN
	int port_listen_CLIENT = 9082; // Proxifier

	// Check Configuration
	printf("Configuration :\n");
	printf("Port SK5 : %i\n", port_listen_SK5);
	printf("Port CLIENT : %i\n", port_listen_CLIENT);

	// Hey! Windows je fais du reseau !
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	// SOCKET PROXIFIER
	SOCKET      s_CLIENT;
	SOCKET      serverSocket_CLIENT;
	SOCKADDR_IN addr_CLIENT;
	INT         addrSize_CLIENT;
	if (!StartServer(port_listen_CLIENT, &serverSocket_CLIENT, &addr_CLIENT, &addrSize_CLIENT))
		return 1;

	SOCKET      s_SK5;
	SOCKET      serverSocket_SK5;
	SOCKADDR_IN addr_SK5;
	INT         addrSize_SK5;
	if (!StartServer(port_listen_SK5, &serverSocket_SK5, &addr_SK5, &addrSize_SK5))
		return 2;

	threaddata d;

	while (1)
	{
		int nb_connect = 0;

		// Connections client/SK5
		s_SK5 = accept(serverSocket_SK5, (SOCKADDR *)&addr_SK5, &addrSize_SK5);
		s_CLIENT = accept(serverSocket_CLIENT, (SOCKADDR *)&addr_CLIENT, &addrSize_CLIENT);

		if (s_CLIENT != INVALID_SOCKET && s_SK5 != INVALID_SOCKET)
		{
			printf("\n\nClient/SK5 Connect\n");

			d.x = s_CLIENT;
			d.y = s_SK5;

			create_sock_thread(d);
		}

	}

	system("\n\npause");
	return 0;
}

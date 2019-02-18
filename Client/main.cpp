
// CLIENT_SK5_V2

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <sstream>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "lib_SK5.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	printf("CLIENT_SK5_V2\n");

	// Configuration variables
	BOOL VERBOSE_mode = TRUE; // If FALSE only [!] is show
	const char *ip_SK5 = "127.0.0.1";
	int port_SK5 = 40001;
	int buf_size = 512; // If changed, line 202 in lib_SK5.h is a change too
	int timeout_sec = 30; // seconds
	int max_connect_failed = 1; // 1 for infinite

	// Initiates Winsock
	WSADATA WSAData;
	int ret = WSAStartup(MAKEWORD(2, 0), &WSAData);
	if (ret != 0)
	{
		printf("[!] WSAStartup error: %i\n", ret);
		return -1;
	}
	else
		if (VERBOSE_mode) printf("[+] WSAStartup success\n");

	int connect_failed = 0;

	while (TRUE)
	{
		SOCKET sock;
		SOCKADDR_IN sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = inet_addr(ip_SK5);
		sin.sin_port = htons(port_SK5);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			printf("[!] sock INVALID_SOCKET\n");
			return -1;
		}

		if (connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
		{
			if (VERBOSE_mode) printf("[+] Connect to SK5 success\n");
			connect_failed = 0;

			fd_set readfds;
			int result, nfds = sock + 1;
			FD_ZERO(&readfds);
			FD_SET(sock, &readfds);

			if ((result = select(nfds, &readfds, 0, 0, 0)) > 0)
			{
				if (FD_ISSET(sock, &readfds))
				{
					threaddata qq;
					qq.x = sock;
					qq.VERBOSE_mode = VERBOSE_mode;
					qq.timeout_sec = timeout_sec;
					qq.buf_size = buf_size;

					create_sock_thread((void*)&qq);
				}
			}
		}
		else
		{
			connect_failed++;
			printf("[!] Connect to SK5 (%i) error\n", connect_failed);
		}

		if (connect_failed >= max_connect_failed && max_connect_failed != 1)
			return -2;
	}

	getchar();
	return 0;
}


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
	std::cout << "SK5 V2 Client\n" << std::endl;

	// Variables Parameters
	char *ip_SK5 = "127.0.0.1";
    int port_SK5 = 40001;
	BOOL VERBOSE_mode = TRUE;
	int timeout_sec = 30;
	int buf_size = 512; // MAX 1024 * 10

	// Initiates Winsock
	WSADATA WSAData;
	int ret = WSAStartup(MAKEWORD(2, 0), &WSAData);
	if (ret != 0)
	{
		printf("[ERROR] WSAStartup error: %i\n", ret);
		return -1;
	}
	else
		if (VERBOSE_mode) printf("[+] WSAStartup success\n");
	
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

		if(connect(sock,(SOCKADDR*)&sin,sizeof(sin)) != SOCKET_ERROR) // Connect to SK5_BASE
		{
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

					if (VERBOSE_mode) printf("[+] Connect to SK5_BASE success\n");
					create_sock_thread(qq);
				}
			}
		}
		else
			printf("[ERROR] Connect to SK5_BASE error\n", ret);
	}

	system("\n\npause");
	return 0;
}



#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>

#include "lib_SK5_BASE.h"

#pragma comment(lib, "ws2_32.lib")

int main()
{
	// Variables Parameters
	int port_listen_SK5 = 40001;   // SK5 CLIENT
	int port_listen_CLIENT = 9082; // PROXIFIER
	int buf_size = 512; // MAX 1024 * 10
	BOOL VERBOSE_mode = TRUE; // If FALSE only [ERROR] is show
	BOOL OLDMODEM_mode = TRUE; // Get your 56k modem ! Limit to ~2KB/sec with buf_size = 512
	int timeout_sec = 30;

	printf("  -ymNMMMMdy/     `mm/       /hmmo`  .=Neosama=:                         :ydmmmmy+` \n");
	printf(" yMNs:-.-:oNMd.   .MM+    `/mMNs.    yMMsoooooo.                       `hMNy/:/omMN:\n");
	printf(".MMs       .mms   .MM+  `+mMNs.     `NMd                 .::.      -::`:mm/     `NMh\n");
	printf("`hMNs/-.``        .MM+`+mMm+.       +MMhshhhy+.          .NMd`    .NMd          /MMs\n");
	printf(" `/ymNNNmdys/.    .MMdmMNMm:        dNNds++sdMNo          /MMo   `dMN.        .sMNy`\n");
	printf("     .-:+shmMN+   .MMMd/.yNMy.      ---`     oMM/          yMM-  oMM+      `:yNNh:  \n");
	printf("hhs        `yMM.  .MMs`   :dMm/     ..`      -MMo          `dMd`-NMd`    `+dMms-    \n");
	printf("yMM+`      .hMN.  .MM+     `oNMh.  .mNh`    `yMN:           -NMsdMN-    /mMd/.      \n");
	printf("`omMmhs++shmMd:   .MM+       -hMN+` /mMdsoosdMd:             +MMMM+    /MMMhhhhhhhhh\n");
	printf("  `:osyyyys+-`    `ss-        `+so:  `:Server:`               +sso     /ssssssssssss\n");
	
	printf("\n================================================\n");
	printf("         Configuration\n");
	printf("Port listen for SK5 : %i\n", port_listen_SK5);
	printf("Port listen for Proxifier : %i\n", port_listen_CLIENT);
	printf("BUFFER SIZE : %i\n", buf_size);
	printf("TIMEOUT (sec) : %i\n", timeout_sec);
	printf("Verbose : %s\n", (VERBOSE_mode ? "ENABLE" : "DISABLE"));
	printf("OLDMODEM_mode : %s\n\n", (OLDMODEM_mode ? "ENABLE" : "DISABLE"));
	printf("         Specials URL\n");
	printf("exitsoft.sk5 : Close program SK5_Client\n");
	printf("tetris.sk5 : Play Tetris music on SK5_Client\n");
	printf("================================================\n\n");

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

	SOCKET      s_CLIENT;
	SOCKET      serverSocket_CLIENT;
	SOCKADDR_IN addr_CLIENT;
	INT         addrSize_CLIENT;
	if (!StartServer(port_listen_CLIENT, &serverSocket_CLIENT, &addr_CLIENT, &addrSize_CLIENT))
	{
		printf("[ERROR] StartServer s_CLIENT\n");
		return 1;
	}
	else
		if (VERBOSE_mode) printf("[+] StartServer s_CLIENT success\n");

	SOCKET      s_SK5;
	SOCKET      serverSocket_SK5;
	SOCKADDR_IN addr_SK5;
	INT         addrSize_SK5;
	if (!StartServer(port_listen_SK5, &serverSocket_SK5, &addr_SK5, &addrSize_SK5))
	{
		printf("[ERROR] StartServer s_SK5\n");
		return 2;
	}
	else
		if (VERBOSE_mode) printf("[+] StartServer s_SK5 success\n");

	while(1)
	{
		s_SK5 = accept(serverSocket_SK5, (SOCKADDR *) &addr_SK5, &addrSize_SK5);
		if (s_SK5 == INVALID_SOCKET)
		{
			printf("[ERROR] s_SK5 INVALID_SOCKET!\n");
		}
		else
			if(VERBOSE_mode) printf("[+] s_SK5 Connected\n");

		s_CLIENT = accept(serverSocket_CLIENT, (SOCKADDR *) &addr_CLIENT, &addrSize_CLIENT);
		if (s_CLIENT == INVALID_SOCKET)
		{
			printf("[ERROR] s_Client INVALID_SOCKET!\n");
		}
		else
			if (VERBOSE_mode) printf("[+] s_Client Connected\n");

		// Pass parameters to thread with threaddata
		threaddata d;
		d.x = s_CLIENT;
		d.y = s_SK5;
		d.VERBOSE_mode = VERBOSE_mode;
		d.OLDMODEM_mode = OLDMODEM_mode;
		d.buf_size = buf_size;

		create_sock_thread(d);
	}

	system("\n\npause");
	return 0;
}

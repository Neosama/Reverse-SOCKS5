
/*

LIB SK5 - BASE

*/

static BOOL StartServer(INT port, SOCKET *s, SOCKADDR_IN *addr, INT *addrSize)
{
   WSADATA wsa;
   *addrSize = sizeof(*addr);
   if(WSAStartup(MAKEWORD(2, 2), &wsa))
      return FALSE;
   if((*s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
      return FALSE;

   (*addr).sin_family      = AF_INET;
   (*addr).sin_addr.s_addr = INADDR_ANY;
   (*addr).sin_port        = htons(port);

   if(bind(*s, (sockaddr *) addr, *addrSize) == SOCKET_ERROR)
      return FALSE;
   if(listen(*s, SOMAXCONN) == SOCKET_ERROR)
      return FALSE;

   getsockname(*s, (SOCKADDR *) addr, addrSize);
   return TRUE;
}

void set_fds(int sock1, int sock2, fd_set *fds) {
    FD_ZERO (fds);
    FD_SET (sock1, fds); 
    FD_SET (sock2, fds); 
}


typedef struct 
{
    SOCKET x;
	SOCKET y;
} threaddata;

unsigned long WINAPI Fonction1(void* params)
{

	threaddata* sub = (threaddata*)params;

	SOCKET s_CLIENT = sub->x;
	SOCKET s_SK5 = sub->y;

	char buffer_connect[1024];
	memset(buffer_connect, 0, sizeof(buffer_connect));

	fd_set readfds; 
	int result, nfds = max(s_CLIENT, s_SK5)+1;
	set_fds(s_CLIENT, s_SK5, &readfds);
	while((result = select(nfds, &readfds, 0, 0, 0)) > 0)  // Choix sens (C_Firefox <-> Server.dest)
	{
		if (FD_ISSET (s_CLIENT, &readfds)) // C_Firefox -> Server.dest
		{
			int recvd = recv(s_CLIENT, buffer_connect, 256, 0); // Recv de Client_firefox
			if(recvd <= 0)
			{
				closesocket(s_CLIENT);
				return 3;
			}
			send(s_SK5, buffer_connect, recvd, 0); // Send au Server.dest
		}

		if (FD_ISSET (s_SK5, &readfds))  // Server.dest -> C_Firefox
		{
			int recvd = recv(s_SK5, buffer_connect, 256, 0);
			if(recvd <= 0)
			{
				closesocket(s_CLIENT);
				return 4;
			}
			send(s_CLIENT, buffer_connect, recvd, 0);
		}
		set_fds(s_CLIENT, s_SK5, &readfds);
	}

	shutdown(s_CLIENT, 2);
	closesocket(s_CLIENT);
	closesocket(s_SK5);
	return 0;
}

static int create_sock_thread(threaddata f)
{
	HANDLE handle;
	if (!(handle = CreateThread(NULL, 0, Fonction1, (void *)&f, 0, 0)))
		return 0;
	CloseHandle(handle);
	
	return 1;
}
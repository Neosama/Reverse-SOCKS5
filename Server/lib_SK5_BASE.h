
/*

LIB SK5 - BASE

*/

std::string string_hex(std::string str, const bool capital = false)
{
	std::string hexstr = "";
	hexstr.resize(str.size() * 2);
	static const char a = capital ? 0x40 : 0x60;
	for (size_t i = 0; i < str.size(); i++)
	{
		char c = (str[i] >> 4) & 0xF;
		hexstr[i * 2] = c > 9 ? (c - 9) | a : c | '0';
		hexstr[i * 2 + 1] = (str[i] & 0xF) > 9 ? (str[i] - 9) & 0xF | a : str[i] & 0xF | '0';
	}
	return hexstr;
}

std::string char_to_string(char x[], int size_recv)
{
	int num_car = 0;
	int stop_while = size_recv;
	std::string output = "";

	while (stop_while > 0)
	{
		output += x[num_car];
		if (num_car < size_recv) { num_car++; }

		stop_while--;
	}

	return output;
}

static BOOL StartServer(INT port, SOCKET *s, SOCKADDR_IN *addr, INT *addrSize)
{
	WSADATA wsa;
	*addrSize = sizeof(*addr);
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		return FALSE;
	if ((*s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return FALSE;

	(*addr).sin_family = AF_INET;
	(*addr).sin_addr.s_addr = INADDR_ANY;
	(*addr).sin_port = htons(port);

	if (bind(*s, (sockaddr *)addr, *addrSize) == SOCKET_ERROR)
		return FALSE;
	if (listen(*s, SOMAXCONN) == SOCKET_ERROR)
		return FALSE;

	getsockname(*s, (SOCKADDR *)addr, addrSize);
	return TRUE;
}

void set_fds(int sock1, int sock2, fd_set *fds) {
	FD_ZERO(fds);
	FD_SET(sock1, fds);
	FD_SET(sock2, fds);
}

typedef struct
{
	SOCKET x;
	SOCKET y;
	BOOL VERBOSE_mode;
	BOOL OLDMODEM_mode;
	int timeout_sec;
	int buf_size;
} threaddata;

unsigned long WINAPI Fonction1(void* params)
{
	threaddata* sub = (threaddata*)params;

	SOCKET s_CLIENT = sub->x;
	SOCKET s_SK5 = sub->y;
	BOOL VERBOSE_mode = sub->VERBOSE_mode;
	BOOL OLDMODEM_mode = sub->OLDMODEM_mode;
	int timeout_sec = sub->timeout_sec;
	int buf_size = sub->buf_size;

	char buf[1024 * 10];

	fd_set readfds;
	int result, nfds = max(s_CLIENT, s_SK5) + 1;
	set_fds(s_CLIENT, s_SK5, &readfds);

	// Set timeout for select
	timeval tv;
	tv.tv_sec = timeout_sec;
	tv.tv_usec = 0;

	memset(buf, 0, sizeof(buf));

	while ((result = select(nfds, &readfds, 0, 0, &tv)) > 0)  // s_Client <-> s_SK5
	{
		if (FD_ISSET(s_CLIENT, &readfds)) // s_Client -> s_SK5
		{
			int recvd_A = recv(s_CLIENT, buf, buf_size, 0);
			if (recvd_A <= 0)
			{
				printf("[ERROR] s_Client -> s_SK5 recvd (%i) <= 0\n", recvd_A);
				closesocket(s_CLIENT);
				return 3;
			}

			int sendd_A = send(s_SK5, buf, recvd_A, 0);
		}

		if (FD_ISSET(s_SK5, &readfds))  // s_SK5-> s_Client
		{
			int recvd_B = 0;

			recvd_B = recv(s_SK5, buf, buf_size, 0);

			if (recvd_B <= 0)
			{
				printf("[ERROR] s_SK5-> s_Client recvd (%i) <= 0\n", recvd_B);
				closesocket(s_CLIENT);
				return 4;
			}
			int sendd_B = send(s_CLIENT, buf, recvd_B, 0);
		}
		set_fds(s_CLIENT, s_SK5, &readfds);

		if (OLDMODEM_mode)
			Sleep(250);
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

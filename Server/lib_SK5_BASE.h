
// LIB SK5_BASE_2

std::string hex_string(std::string hexstr)
{
	std::string str = "";
	str.resize((hexstr.size() + 1) / 2);
	for (size_t i = 0, j = 0; i < str.size(); i++, j++)
	{
		char at = '@';
		str[i] = (hexstr[j] & at ? hexstr[j] + 9 : hexstr[j]) << 4, j++;
		str[i] |= (hexstr[j] & at ? hexstr[j] + 9 : hexstr[j]) & 0xF;
	}
	return str;
}

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
} threaddata;

unsigned long WINAPI Fonction1(void* params)
{
	threaddata* sub = (threaddata*)params;

	SOCKET s_CLIENT = sub->x;
	SOCKET s_SK5 = sub->y;

	char buffer_connect[1024];
	memset(buffer_connect, 0, sizeof(buffer_connect));

	fd_set readfds;
	int result, nfds = max(s_CLIENT, s_SK5) + 1;
	set_fds(s_CLIENT, s_SK5, &readfds);
	while ((result = select(nfds, &readfds, 0, 0, 0)) > 0)  // PROXIFIER <-> SK5-CLIENT
	{
		if (FD_ISSET(s_CLIENT, &readfds)) // PROXIFIER -> SK5-CLIENT
		{
			int recvd = recv(s_CLIENT, buffer_connect, 256, 0); // Recv PROXFIER
			if (recvd <= 0)
			{
				closesocket(s_CLIENT);
				return 3;
			}
			send(s_SK5, buffer_connect, recvd, 0); // Send SK5-CLIENT
		}

		if (FD_ISSET(s_SK5, &readfds))  // SK5-CLIENT -> PROXIFIER
		{
			int recvd = recv(s_SK5, buffer_connect, 256, 0);
			if (recvd <= 0)
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



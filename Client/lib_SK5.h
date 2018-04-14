
/*

LIB SK5

*/

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

long int hex_to_dec(std::string x)
{
	return strtol(x.data(), NULL, 16);
}

void tetris_sound()
{
	// From github.com/newtpuandre/MusicToBeep
	Beep(658, 125);
	Beep(1320, 500);
	Beep(990, 250);
	Beep(1056, 250);
	Beep(1188, 250);
	Beep(1320, 125);
	Beep(1188, 125);
	Beep(1056, 250);
	Beep(990, 250);
	Beep(880, 500);
	Beep(880, 250);
	Beep(1056, 250);
	Beep(1320, 500);
	Beep(1188, 250);
	Beep(1056, 250);
	Beep(990, 750);
	Beep(1056, 250);
	Beep(1188, 500);
	Beep(1320, 500);
	Beep(1056, 500);
	Beep(880, 500);
	Beep(880, 500);
	Sleep(250);
	Beep(1188, 500);
	Beep(1408, 250);
	Beep(1760, 500);
	Beep(1584, 250);
	Beep(1408, 250);
	Beep(1320, 750);
	Beep(1056, 250);
	Beep(1320, 500);
	Beep(1188, 250);
	Beep(1056, 250);
	Beep(990, 500);
	Beep(990, 250);
	Beep(1056, 250);
	Beep(1188, 500);
	Beep(1320, 500);
	Beep(1056, 500);
	Beep(880, 500);
	Beep(880, 500);
	Sleep(500);
	Beep(1320, 500);
	Beep(990, 250);
	Beep(1056, 250);
	Beep(1188, 250);
	Beep(1320, 125);
	Beep(1188, 125);
	Beep(1056, 250);
	Beep(990, 250);
	Beep(880, 500);
	Beep(880, 250);
	Beep(1056, 250);
	Beep(1320, 500);
	Beep(1188, 250);
	Beep(1056, 250);
	Beep(990, 750);
	Beep(1056, 250);
	Beep(1188, 500);
	Beep(1320, 500);
	Beep(1056, 500);
	Beep(880, 500);
	Beep(880, 500);
	Sleep(250);
	Beep(1188, 500);
	Beep(1408, 250);
	Beep(1760, 500);
	Beep(1584, 250);
	Beep(1408, 250);
	Beep(1320, 750);
	Beep(1056, 250);
	Beep(1320, 500);
	Beep(1188, 250);
	Beep(1056, 250);
	Beep(990, 500);
	Beep(990, 250);
	Beep(1056, 250);
	Beep(1188, 500);
	Beep(1320, 500);
	Beep(1056, 500);
	Beep(880, 500);
	Beep(880, 500);
	Sleep(500);
	Beep(660, 1000);
	Beep(528, 1000);
	Beep(594, 1000);
	Beep(495, 1000);
	Beep(528, 1000);
	Beep(440, 1000);
	Beep(419, 1000);
	Beep(495, 1000);
	Beep(660, 1000);
	Beep(528, 1000);
	Beep(594, 1000);
	Beep(495, 1000);
	Beep(528, 500);
	Beep(660, 500);
	Beep(880, 1000);
	Beep(838, 2000);
	Beep(660, 1000);
	Beep(528, 1000);
	Beep(594, 1000);
	Beep(495, 1000);
	Beep(528, 1000);
	Beep(440, 1000);
	Beep(419, 1000);
	Beep(495, 1000);
	Beep(660, 1000);
	Beep(528, 1000);
	Beep(594, 1000);
	Beep(495, 1000);
	Beep(528, 500);
	Beep(660, 500);
	Beep(880, 1000);
	Beep(838, 2000);
}

typedef struct
{
	SOCKET x;
	SOCKET y;
	BOOL VERBOSE_mode;
	int timeout_sec;
	int buf_size;
} threaddata;

void set_fds(int sock1, int sock2, fd_set *fds) {
	FD_ZERO(fds);
	FD_SET(sock1, fds);
	FD_SET(sock2, fds);
}

// MSG SOCKS5
char msg_auth_ok[] = { 0X05, 0X00 }; // VERSION SOCKS, AUTH MODE, OK
char msg_ipv6_nok[] = { 0X05, 0X08, 0X00, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }; // IPv6 not compt
char msg_request_co_ok[] = { 0X05, 0X00, 0X00, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }; // Request connect OK

DWORD WINAPI sock5_gen(void *param)
{
	threaddata* sub = (threaddata*)param;

	SOCKET sock_SK5 = sub->x;
	BOOL VERBOSE_mode = sub->VERBOSE_mode;
	int timeout_sec = sub->timeout_sec;
	int buf_size = sub->buf_size;

	// SIZE DATA RECV
	int SIZE_DATA_RECV = 256 * 4;

	char buf[1024*10];
	int version_sock = 0; // 5, 4, 0, ??

	// Phase A = Version Socks, Send Msg Auth
	memset(buf, 0, sizeof(buf));

	// Get Version Socks
	int size_recv_PA = 0;

	size_recv_PA = recv(sock_SK5, buf, sizeof(buf), 0);

	if (size_recv_PA <= 0)
	{
		printf("[ERROR] size_recv_PA <= 0\n");
		closesocket(sock_SK5);
		return 1;
	}

	version_sock = buf[0];

	if (version_sock != 5)
	{
		printf("[ERROR] version_sock != 5\n");
		closesocket(sock_SK5);
		return 1;
	}

	// Send msg_auth_ok
	int size_send_PA = 0;

	size_send_PA = send(sock_SK5, msg_auth_ok, sizeof(msg_auth_ok), 0);

	if (size_send_PA <= 0)
	{
		printf("[ERROR] size_send_PA <= 0\n");
		closesocket(sock_SK5);
		return 1;
	}

	// Phase B = Mode Connect & Type Addr & Addr/Port
	memset(buf, 0, sizeof(buf));
	int size_recv_PB = 0;

	size_recv_PB = recv(sock_SK5, buf, sizeof(buf), 0);

	if (size_recv_PB <= 0)
	{
		printf("[ERROR] size_recv_PB <= 0\n");
		closesocket(sock_SK5);
		return 1;
	}

	int mode_cmd = buf[1]; // Connect = 1, Bind = 2, UDP = 3
	int type_addr = buf[3]; // IPv4 = 01, Domain Name = 03, IPv6 = 04

	if (mode_cmd != 1)
	{
		printf("[ERROR] Bind/UDP not supported\n");
		closesocket(sock_SK5);
		return 1;
	}

	std::string dest_ip = ""; // IP dest
	int dest_port = 0; // Port dest

	if (type_addr == 1) // IPv4
	{
		std::string raw_demand = char_to_string(buf, size_recv_PB); // convert to string
		std::string str_ip_dest = raw_demand.substr(4, 11); // get IPv4
		std::string str_port_dest = raw_demand.substr(8, 10); // get Port

		sockaddr_in svr = { 0 };
		svr.sin_family = AF_INET;
		svr.sin_addr.s_addr = MAKELONG(MAKEWORD((buf[4] & 0xff), (buf[5] & 0xff)), MAKEWORD((buf[6] & 0xff), (buf[7] & 0xff)));

		dest_ip = inet_ntoa(svr.sin_addr); // IPv4 dest in string
		dest_port = hex_to_dec(string_hex(str_port_dest)); // port dest in long
	}

	if (type_addr == 3) // Domain Name
	{
		std::string raw_demand = char_to_string(buf, size_recv_PB); // convert to string
		int size_domain_name = buf[4]; // Get size Domain Name
		std::string str_domain_name;
		std::string str_port_dest;
		
		try
		{
			str_domain_name = raw_demand.substr(5, size_domain_name); // get domain_name
			str_port_dest = raw_demand.substr(5 + size_domain_name, 5 + size_domain_name + 2); // get Port
			dest_port = hex_to_dec(string_hex(str_port_dest)); // port dest in long
		}
		catch (const std::out_of_range& e) 
		{
			printf("[ERROR] Out of Range\n");
		}

		// Hostname EXIT
		if (std::string(str_domain_name).find("exitsoft.sk5") != std::string::npos)
		{
			closesocket(sock_SK5);
			exit(12);
		}

		// Hostname Tetris
		if (std::string(str_domain_name).find("tetris.sk5") != std::string::npos)
		{
			tetris_sound();
			closesocket(sock_SK5);
		}

		struct hostent *he_a;
		he_a = gethostbyname(str_domain_name.c_str());
		if (he_a == NULL) // Error gethostbyname
		{
			printf("[ERROR] gethostbyname\n");
			closesocket(sock_SK5);
			return 12;
		}
		else // Ok gethostbyname
		{
			dest_ip = inet_ntoa(*((struct in_addr *) he_a->h_addr_list[0]));
		}
	}

	if (type_addr == 4) // IPv6
	{
		printf("[ERROR] IPv6 not supported\n");

		int size_send_IPNOK = 0;

		size_send_IPNOK = send(sock_SK5, msg_ipv6_nok, sizeof(msg_ipv6_nok), 0);

		if (size_send_IPNOK <= 0) // Send msg_ipv6_nok
		{
			closesocket(sock_SK5);
			return 1;
		}
		closesocket(sock_SK5);
		return 1;
	}

	// Phase C = Send Request Addr OK & Recv Request & Connect to TARGET SERVER
	int size_send_PC = 0;

	size_send_PC = send(sock_SK5, msg_request_co_ok, sizeof(msg_request_co_ok), 0);

	if (size_send_PC <= 0) // Send msg_request_co_ok
	{
		printf("[ERROR] size_send_PC <= 0\n");
		closesocket(sock_SK5);
		return 1;
	}

	memset(buf, 0, sizeof(buf));

	int size_recv_PC = 0;

	size_recv_PC = recv(sock_SK5, buf, sizeof(buf), 0);

	if (size_recv_PC <= 0)
	{
		printf("[ERROR] size_recv_PC <= 0\n");
		closesocket(sock_SK5);
		return 1;
	}

	SOCKET client_TARGET; // Socket Connection for TARGET
	struct sockaddr_in serveraddra_a;

	if ((client_TARGET = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) //open socket
		printf("[ERROR] socket() failed\n");

	//connect
	memset(&serveraddra_a, 0, sizeof(serveraddra_a));
	serveraddra_a.sin_family = AF_INET;
	serveraddra_a.sin_addr.s_addr = inet_addr(dest_ip.c_str());
	serveraddra_a.sin_port = htons((unsigned short)dest_port);

	if (connect(client_TARGET, (struct sockaddr *) &serveraddra_a, sizeof(serveraddra_a)) == SOCKET_ERROR)
	{
		printf("[ERROR] connect() failed\n");
		closesocket(sock_SK5);
		return 1;
	}

	if (send(client_TARGET, buf, size_recv_PC, 0) != size_recv_PC)
		printf("[ERROR] send() sent a different number of bytes than expected\n");

	fd_set readfds;
	int result, nfds = max(sock_SK5, client_TARGET) + 1;
	set_fds(sock_SK5, client_TARGET, &readfds);

	// Set timeout for select
	timeval tv;
	tv.tv_sec = timeout_sec;
	tv.tv_usec = 0;

	memset(buf, 0, sizeof(buf));

	while ((result = select(nfds, &readfds, 0, 0, &tv)) > 0)  // SK5_BASE <-> SERVER_TARGET
	{
		if (FD_ISSET(sock_SK5, &readfds)) // SK5_BASE -> SERVER_TARGET
		{
			int recvd_A = 0;

			recvd_A = recv(sock_SK5, buf, buf_size, 0);

			if (recvd_A <= 0)
			{
				printf("[ERROR] recvd_A (%i) <= 0\n", recvd_A);
				closesocket(sock_SK5);
				return 1;
			}

			int sendd_A = 0;
			send(client_TARGET, buf, recvd_A, 0); // SERVER_TARGET

			if (VERBOSE_mode)
				printf("[+] A DATA RECV = %i | DATA SEND = %i\n", recvd_A, sendd_A);
		}

		if (FD_ISSET(client_TARGET, &readfds))  // SERVER_TARGET -> SK5_BASE
		{
			int recvd_B = recv(client_TARGET, buf, buf_size, 0);
			if (recvd_B <= 0)
			{
				printf("[ERROR] recvd_B (%i) <= 0\n", recvd_B);
				closesocket(sock_SK5);
				return 1;
			}

			int sendd_B = 0;

			sendd_B = send(sock_SK5, buf, recvd_B, 0);

			if (VERBOSE_mode)
				printf("[+] B DATA RECV = %i | DATA SEND = %i\n", recvd_B, sendd_B);
		}
		set_fds(sock_SK5, client_TARGET, &readfds);
	}

	shutdown(client_TARGET, 2);
	closesocket(client_TARGET);
	closesocket(sock_SK5);

	return 0;
}

static int create_sock_thread(threaddata s)
{
	HANDLE handle;
	if (!(handle = CreateThread(NULL, 0, sock5_gen, (void *)&s, 0, 0)))
		return 0;
	CloseHandle(handle);

	return 1;
}

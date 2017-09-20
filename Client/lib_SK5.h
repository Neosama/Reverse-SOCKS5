
/*

	LIB SK5

*/

// Fonctions conversion

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

	while(stop_while > 0)
	{
		output += x[num_car];
		if(num_car < size_recv){num_car++;}

		stop_while--;
	}

	return output;
}

long int hex_to_dec(std::string x)
{
	return strtol(x.data(), NULL, 16);
}

// Fonctions sur Network

void set_fds(int sock1, int sock2, fd_set *fds) {
    FD_ZERO (fds);
    FD_SET (sock1, fds); 
    FD_SET (sock2, fds); 
}

// Message SOCKS5
char msg_auth_ok[] = {0X05, 0X00}; // VERSION SOCKS, AUTH MODE, OK
char msg_ipv6_nok[] = {0X05, 0X08, 0X00, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00}; // IPv6 not compt
char msg_request_co_ok[] = {0X05, 0X00, 0X00, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00}; // Request connect OK

DWORD WINAPI sock5_gen(void *param)
{

	SOCKET sock_client = (SOCKET)param; // Get param -> socket

	char request_client_socks[1024]; // Request du Client Socks
	int version_sock = 0; // 5, 4, 0, ??

	// Phase = Version Socks, Send Msg Auth

	memset(request_client_socks, 0, sizeof(request_client_socks)); // Reset 0 de request_client_socks
	if(recv(sock_client, request_client_socks, sizeof(request_client_socks), 0) <= 0) // Recv de la request du client socks5
	{
		closesocket(sock_client);
		return 1;
	}

	version_sock = request_client_socks[0]; // Get Ver Socks

	if(version_sock != 5)
	{
		printf("Not Version 5!\n");
		std::cout << "request_client_socks =\n" << request_client_socks << std::endl;
		closesocket(sock_client);
		return 1;
	}

	if(send(sock_client, msg_auth_ok, sizeof(msg_auth_ok), 0) <= 0) // Send msg_auth_ok
	{
		closesocket(sock_client);
		return 1;
	}

	// Phase = Mode Connect & Type Addr & AddrV4/Port

	memset(request_client_socks, 0, sizeof(request_client_socks)); // Reset 0 de request_client_socks
	int recv_size = recv(sock_client, request_client_socks, sizeof(request_client_socks), 0); // Recv de la request du client socks5
	if(recv_size <= 0)
	{
		closesocket(sock_client);
		return 1;
	}

	int mode_cmd = request_client_socks[1]; // Connect = 1, Bind = 2, UDP = 3
	int type_addr = request_client_socks[3]; // IPv4 = 01, Nom de domaine = 03, IPv6 = 04
	
	if(mode_cmd != 1) // Si autre que Connect
	{
		printf("Bind/UDP Pas pris en charge !\n");
		closesocket(sock_client);
		return 1;
	}

	std::string dest_ipv4 = ""; // IPv4 dest
	int dest_port = 0; // Port dest

	if(type_addr == 1) // IPv4
	{
		std::string raw_demand = char_to_string(request_client_socks, recv_size); // convert to string
		std::string str_ip_dest = raw_demand.substr(4,11); // get IPv4
		std::string str_port_dest = raw_demand.substr(8,10); // get Port

		sockaddr_in svr = {0};
		svr.sin_family = AF_INET;
		svr.sin_addr.s_addr = MAKELONG(MAKEWORD((request_client_socks[4]&0xff),(request_client_socks[5]&0xff)), MAKEWORD((request_client_socks[6]&0xff),(request_client_socks[7]&0xff))) ;

		dest_ipv4 =  inet_ntoa(svr.sin_addr); // IPv4 dest in string
		dest_port = hex_to_dec(string_hex(str_port_dest)); // port dest in long
	}

	if(type_addr == 3) // Domain Name
	{
		std::string raw_demand = char_to_string(request_client_socks, recv_size); // convert to string
		int size_domain_name = request_client_socks[4]; // Get size nom de domaine

		std::string str_domain_name = raw_demand.substr(5, size_domain_name); // get domain_name
		std::string str_port_dest = raw_demand.substr(5 + size_domain_name , 5 + size_domain_name + 2); // get Port
		dest_port = hex_to_dec(string_hex(str_port_dest)); // port dest in long

		// Hostname EXIT
		if(std::string(str_domain_name).find("www.exitsoft.sk5") != std::string::npos)
		{
			closesocket(sock_client);
			exit(12);
		}

		struct hostent *he_a;
		he_a = gethostbyname(str_domain_name.c_str());
		if (he_a == NULL) // Error gethostbyname
		{
			printf("Error gethostbyname!\n");
		}

		else // Ok gethostbyname
		{
			dest_ipv4 = inet_ntoa (*((struct in_addr *) he_a->h_addr_list[0]));
		}
	}

	if(type_addr == 4) // IPv6
	{
		printf("IPv6 Pas en charge !\n");
		if(send(sock_client, msg_ipv6_nok, sizeof(msg_ipv6_nok), 0) <= 0) // Send msg_ipv6_nok
		{
			closesocket(sock_client);
			return 1;
		}
		closesocket(sock_client);
		return 1;
	}

	std::cout << "IPv4: " << dest_ipv4 << "\nPort: " << dest_port << "\n" << std::endl;

	// Phase = Send Request Addr OK & Recv Request & Connection au server cible

	if(send(sock_client, msg_request_co_ok, sizeof(msg_request_co_ok), 0) <= 0) // Send msg_request_co_ok
	{
		closesocket(sock_client);
		return 1;
	}

	memset(request_client_socks, 0, sizeof(request_client_socks)); // Reset 0 de request_client_socks
	recv_size = recv(sock_client, request_client_socks, sizeof(request_client_socks), 0); // Recv de la request du client socks5
	if(recv_size <= 0)
	{
		closesocket(sock_client);
		return 1;
	}

	std::string request_tcp = char_to_string(request_client_socks, recv_size); // request_client_socks -> string

	SOCKET client_dest; // Socket Connection Server Cible
	struct sockaddr_in serveraddra_a;

    if ((client_dest = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)	//open socket
        printf("socket() failed");

	//connect
    memset(&serveraddra_a, 0, sizeof(serveraddra_a));
    serveraddra_a.sin_family      = AF_INET;
    serveraddra_a.sin_addr.s_addr = inet_addr(dest_ipv4.c_str());
    serveraddra_a.sin_port        = htons((unsigned short) dest_port);

	UINT TimeOut = 30000; // 1000 = 1 sec / Par defaut 1000 ms
	setsockopt(client_dest,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut)); // TimeOut

	if(connect(client_dest, (struct sockaddr *) &serveraddra_a, sizeof(serveraddra_a)) == SOCKET_ERROR)
    {
		printf("\aconnect() failed");
		closesocket(sock_client);
		return 1;
	}
	
	if(send(client_dest, request_tcp.c_str(), request_tcp.length(), 0) != request_tcp.length())
        printf("send() sent a different number of bytes than expected");

	char buffer_connect[1024];
	memset(buffer_connect, 0, sizeof(buffer_connect));

	fd_set readfds; 
	int result, nfds = max(sock_client, client_dest)+1;
	set_fds(sock_client, client_dest, &readfds);
	while((result = select(nfds, &readfds, 0, 0, 0)) > 0)  // Choix sens (C_Firefox <-> Server.dest)
	{
		if (FD_ISSET (sock_client, &readfds)) // C_Firefox -> Server.dest
		{
			int recvd = recv(sock_client, buffer_connect, 256, 0); // Recv de Client_firefox
			if(recvd <= 0)
			{
				closesocket(sock_client);
				return 1;
			}
			send(client_dest, buffer_connect, recvd, 0); // Send au Server.dest
		}

		if (FD_ISSET (client_dest, &readfds))  // Server.dest -> C_Firefox
		{
			int recvd = recv(client_dest, buffer_connect, 256, 0);
			if(recvd <= 0 || recvd == SOCKET_ERROR)
			{
				closesocket(sock_client);
				return 1;
			}
			send(sock_client, buffer_connect, recvd, 0);
		}
		set_fds(sock_client, client_dest, &readfds);
	}

	shutdown(client_dest, 2);
    closesocket(client_dest);
	closesocket(sock_client);

	return 0;
}

static int create_sock_thread(SOCKET sz)
{
	HANDLE handle;
	if (!(handle = CreateThread(NULL, 0, sock5_gen, (void *)s, 0, 0)))
		return 0;
	CloseHandle(handle);
	
	return 1;
}
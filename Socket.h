#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#ifndef __WIN_CPP_SOCKET_H__
#define __WIN_CPP_SOCKET_H__

#pragma comment(lib, "Ws2_32.lib")

class Socket {
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	addrinfo *result = NULL;
	addrinfo *ptr = NULL;
	addrinfo hints;
	bool status;
public:

private:

public:
	Socket();
	~Socket();

	bool state();
	bool abort();
	bool connectToHost(byte *ip, int ip_len, int port, int exceed_time = 10000);
	bool write(byte *data, int data_len);
	bool waitForReadyRead(int exceed_time = 10000);
	int read(byte* data, int len);
};

#endif
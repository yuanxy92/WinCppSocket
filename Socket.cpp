#include "Socket.h"
#include <thread>
#include <chrono>

Socket::Socket() {
	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		exit(-1);
	}
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	status = false;
}

Socket::~Socket() {}

bool Socket::connectToHost(byte *ip, int ip_len, int port,
	int exceed_time) {
	// Connect to server.
	char port_str[10];
	itoa(port, port_str, 10);
	int iResult = getaddrinfo((PCSTR)ip, (PCSTR)port_str, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return false;
	}
	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;
	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}
	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}
	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return false;
	}
	status = true;
	return true;
}

bool Socket::state() {
	return status;
}

bool Socket::write(byte *data, int data_len) {
	int iResult;
	// Send an initial buffer
	iResult = send(ConnectSocket, (const char*)data, data_len, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return false;
	}
	return true;
}

bool Socket::waitForReadyRead(int exceed_time) {
	u_long argp = 0;
	for (int i = 0; i < exceed_time; i += 5) {
		ioctlsocket(ConnectSocket, SIOCATMARK, &argp);
		if (argp > 0)
			return true;
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	printf("Over time!\n");
	return false;
}

int Socket::read(byte* data, int len) {
	// Receive data until the server closes the connection
	int iResult;
	iResult = recv(ConnectSocket, (char*)data, len, 0);
	return len;
}

bool Socket::abort() {
	int iResult;
	iResult = shutdown(ConnectSocket, SD_BOTH);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return false;
	}
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
	return true;
}
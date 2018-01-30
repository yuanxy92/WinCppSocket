#include <cstdio>
#include <cstdlib>

#include "Socket.h"

int main(int argc, char* argv[]) {
	char p[] = "10.8.5.188";
	char b[100];
	memset(b, 1, sizeof(b));
	int send = 1;

	Socket socket;
	socket.connectToHost((byte*)p, sizeof(p), 54321, 10000);
	socket.write((byte*)(&send), 4);
	//int readbytes = socket.read((unsigned char*)b, 4);
	if (socket.waitForReadyRead(1000))
	{
		int readbytes = socket.read((unsigned char*)b, 4);
		printf("%d %d", readbytes, b[0]);
	}
	system("pause");
	return 0;
}
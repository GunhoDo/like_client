#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

#define PACKET_LENGTH 4

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

int GetFileSize(string fileName);


// tcp/ip 프로토콜은 2기가도 버틴다 그에 비해 udp 1000도 못버틸 정도로 한번에 큰 용량을 보낼 수는 없다

int main()
{
	int ssock;
	int clen;
	struct sockaddr_in server_addr;

	int nFileSize = GetFileSize("read.png");
	if (nFileSize == 0)
	{
		return 1;
	}

	int nPacketSize = nFileSize + PACKET_LENGTH;

	// 파일 크기 만큼 메모리 할당
	char* buf = new char[nPacketSize]; // scanf로 받은 걸 넣으면 정적인데 동적할당처럼 해서 오류나는 것
	// 파일을 읽어서 pBuf 에 저장
	ifstream ifStream("read.png", std::ifstream::binary); //binary는 파일 형식에 구애받지 않음 text인 경우 binary를 없애면 그만인데 그럴경우 text에 추가적인 이익이 있음

	memcpy(&buf[0], &nFileSize, PACKET_LENGTH); //memcpy(buf, &nFileSize, 4);

	ifStream.read(buf + PACKET_LENGTH, nFileSize); //ifStream.read(&buf[4], nFileSize);

    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    int nErrorStatus;

    nErrorStatus = WSAStartup(wVersionRequested, &wsaData);

	if (nErrorStatus != 0)
	{
		delete[] buf;

		return 0;
	}

	if ((ssock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		delete[] buf;

		return 0;
	}

	clen = sizeof(server_addr);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(3317);

	if (connect(ssock, (struct sockaddr *)&server_addr, clen) < 0) // 3웨이 핸드쉐이킹
	{
		closesocket(ssock); //fin 주고 받고 받는게 30초 동안 응답없으면 closewait
	}
	//memset(buf, 0, MAXBUF);
	if (send(ssock, buf, nPacketSize, 0) <= 0)
	{
		closesocket(ssock);
	}
	/*
	if (recv(ssock, buf, nFileSize, 0) <= 0)
	{
		closesocket(ssock);
	}
	*/

	Sleep(1000);

	WSACleanup();
	
	delete[] buf;

	return 0;
}

int GetFileSize(string fileName)
{
	struct stat statFile;
	int nStat = stat(fileName.c_str(), &statFile);
	int nSize = 0;
	if (nStat == 0)
	{
		nSize = statFile.st_size;
	}

	return nSize;
}

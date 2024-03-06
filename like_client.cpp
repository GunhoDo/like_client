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


// tcp/ip ���������� 2�Ⱑ�� ��ƾ�� �׿� ���� udp 1000�� ����ƿ ������ �ѹ��� ū �뷮�� ���� ���� ����

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

	// ���� ũ�� ��ŭ �޸� �Ҵ�
	char* buf = new char[nPacketSize]; // scanf�� ���� �� ������ �����ε� �����Ҵ�ó�� �ؼ� �������� ��
	// ������ �о pBuf �� ����
	ifstream ifStream("read.png", std::ifstream::binary); //binary�� ���� ���Ŀ� ���ֹ��� ���� text�� ��� binary�� ���ָ� �׸��ε� �׷���� text�� �߰����� ������ ����

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

	if (connect(ssock, (struct sockaddr *)&server_addr, clen) < 0) // 3���� �ڵ彦��ŷ
	{
		closesocket(ssock); //fin �ְ� �ް� �޴°� 30�� ���� ��������� closewait
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

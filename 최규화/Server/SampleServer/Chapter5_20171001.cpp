
#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996)
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>  
#include <sys/stat.h>
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512
//send������ �ִ�ũ��� 8MB(8000000)
//���� �Լ� ���� ��� �� ����

#pragma pack(1)
struct Protocol01 {
	long fileSize;
	char fileName[BUFSIZE];
};
#pragma pack()


void err_quit(char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

//����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;
	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}
	return (len - left);


}
int _recv_ahead(SOCKET s, char* p) {
	__declspec(thread) static int nbytes = 0;
	__declspec(thread) static char buf[1024];
	__declspec(thread) static char* ptr;

	if (nbytes == 0 || nbytes == SOCKET_ERROR) {
		nbytes = recv(s, buf, sizeof(buf), 0);
		if (nbytes == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (nbytes == 0)
			return 0;
		ptr = buf;
	}
	--nbytes;
	*p = *ptr++;
	return 1;
}

int recvline(SOCKET s, char* buf, int maxlen) {
	int n, nbytes;
	char c;
	char* ptr = buf;
	for (n = 1; n < maxlen; ++n) {
		nbytes = _recv_ahead(s, &c);
		if (nbytes == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;
		}
		else if (nbytes == 0) {
			*ptr = 0;
			return n - 1;
		}
		else
			return SOCKET_ERROR;
	}
	*ptr = 0;
	return n;
}

int main(int argc, char* argv[]) {

	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	int retval;
	FILE* fileReader;
	int fileReader2;
	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");
	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	Protocol01 recvProtocol;
	
	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen, file_read_len;
	int read_Len;
	char buf[BUFSIZE + 1];
	char buf2[BUFSIZE];

	int len;

	


	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		ZeroMemory(&recvProtocol, sizeof(recvProtocol));
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		read_Len = recvn(client_sock, (char*)&recvProtocol, sizeof(recvProtocol), 0);
		//fileReader2 = open(buf2, O_WRONLY | O_CREAT | O_EXCL, 0700);//_S_IREAD |_S_IWRITE);


		 fileReader = fopen(recvProtocol.fileName, "wb");
		if (fileReader == NULL) {
			printf("���� ���� �Ұ���\n");
			return 1;
		}
		// Ŭ���̾�Ʈ�� ������ ���
		int totalsize=0;
		while (read_Len) {
			char file_name[BUFSIZE];
			memset(buf, NULL, BUFSIZE);

			read_Len = recvn(client_sock, (char*)&len, sizeof(len), 0);
			totalsize += len;
			printf("���������� ���� �õ� ��\n");
			if (read_Len == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (read_Len == 0)
			{
				printf("���� ���� 0 - ����\n");
				break;
			}
			memset(buf, 0x00, BUFSIZE);
			read_Len = recvn(client_sock, buf, len, 0);
			float tempPercentage = (float)((float)totalsize * 100) / (float)recvProtocol.fileSize;
			printf("���������� ���� �õ� �� - �� %d����Ʈ �� %d����Ʈ ����Ϸ� [��������: %.2f%%]\n",recvProtocol.fileSize,totalsize,tempPercentage);
			if (read_Len == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (read_Len == 0)
			{
				break;
			}



			//write(fileReader2, buf, read_Len);


			fwrite(buf,sizeof(char) ,read_Len,fileReader);
			printf("���� �Է� ��..\n");

			if (read_Len == EOF || read_Len == 0)
			{
				printf("finish file\n");
				break;
			}
		}




		fclose(fileReader);
		// closesocket()
		closesocket(client_sock);

		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	//close(fileReader2);
	
	closesocket(listen_sock);
	WSACleanup();
	return 0;
}


#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996)
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <vector>
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512
//send버퍼의 최대크기는 8MB(8000000)
//소켓 함수 오류 출력 후 종료

#pragma pack(1)
struct Protocol01 {
	u_short posx;
	u_short posy;
	u_short turn;

};
 
#pragma pack()
#pragma pack(1)
struct Protocol02 {
	u_short zone[15][15];

};

#pragma pack()

int omok_Zone[15][15];
int turn = 0;
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

//사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;
	return 0;
}
DWORD WINAPI ProcessClient(LPVOID arg) {
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN client_addr;
	int addrlen;
	char buf[BUFSIZE + 1];
	
	
	Protocol01 omok;
	Protocol02 info;

	addrlen = sizeof(client_addr);
	getpeername(client_sock, (SOCKADDR*)&client_addr, &addrlen);
	while (1) {
		

			retval = recv(client_sock, (char*)&omok, sizeof(omok), 0);
			turn++;
			system("cls");
			printf("%d %d\n", omok.posx, omok.posy);
			if (turn % 2 == 0 && turn != 0) {
				omok_Zone[omok.posx][omok.posy] = 1;
			}
			else if (turn % 2 == 1 && turn != 0) {
				omok_Zone[omok.posx][omok.posy] = 2;
			}
			for (int i = 0; i < 15; ++i) {
				printf("\n");
				for (int j = 0; j < 15; ++j) {
					if (omok_Zone[i][j] == 1)
						printf("○");
					else if (omok_Zone[i][j] == 2)
						printf("●");
					else
						printf("┼");
					info.zone[i][j] = omok_Zone[i][j];
				}
			}
			
			retval = send(client_sock, (char*)&info, sizeof(info),0 );
			
			


		}
	closesocket(client_sock);
	return 0;
}



int main(int argc, char* argv[]) {
	WSADATA wsa;
	SOCKET listen_sock;
	SOCKET client_sock1;
	SOCKET client_sock2;

	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr1;
	SOCKADDR_IN clientaddr2;
	HANDLE hThread;
	int addrlen;
	
	int read_Len;
	int turn=0;
	Protocol01 omok;
	ZeroMemory(omok_Zone, sizeof(omok_Zone));
	
	int retval;
	if (WSAStartup(MAKEWORD(2, 2),&wsa) != 0) {
		err_quit("winsock()");		
	}
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");
	printf("%d", sizeof(u_short));
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");
	addrlen = sizeof(clientaddr1);
	while (1) {
		client_sock1 = accept(listen_sock, (SOCKADDR*)&clientaddr1, &addrlen);
		printf("\n[TCP 서버] 클라이언트 접속 : Ip 주소=%s,포트번호=%d\n", inet_ntoa(clientaddr1.sin_addr), ntohs(clientaddr1.sin_port));
		ZeroMemory(&omok, sizeof(omok));
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock1, 0, NULL);
		if (hThread == NULL)
			closesocket(client_sock1);
		else
			CloseHandle(hThread);
					
				
		Sleep(1000);

	}
	
	closesocket(listen_sock);
	
	WSACleanup();
	return 0;
}
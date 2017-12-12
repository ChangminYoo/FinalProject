#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#define SERVERPORT 9000
#define BUFSIZE    512


//소켓 함수 오류 출력 후 중지
void err_quit(char *msg)
{
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


// 소켓 함수 오류 출력
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

BOOL IsLittleEndian(const u_short&);
BOOL IsBigEndian(const u_short&);

int main(int argc, char *argv[])
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, "윈도우소켓생성성공", "알림", MB_OK);

	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	u_short v = 0x1212;
	

	printf("If my Computer's system is Big Endian : 0x%x -> in Memory : 0x%x\n",v,v);
	printf("Else if my Computer's system is Little Endian : 0x%x -> in Memory : 0x%x\n", v, ntohs(v));

	if (IsLittleEndian(v)) {

		printf("Your System is Little Endian\n");
	}
	if (IsBigEndian(v)) {
		printf("Your System is Big Endian\n");
	}

	if (tcp_sock == INVALID_SOCKET)
		err_quit("socket()");
	MessageBox(NULL, "TCP 소켓 생성 성공", "알림", MB_OK);

	/*
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, TCPServer4, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, TCPServer6, NULL, 0, NULL);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	*/
	closesocket(tcp_sock);
	// 윈속 종료
	WSACleanup();
	return 0;
}

BOOL IsLittleEndian(const u_short& val) {
	char* temp0 = (char*)&val;
	char frontBite = *temp0;
	char backBite= *(temp0+1);

	if (backBite == 0x00) {
		printf("비교할 수 없는 수를 주었습니다.");
		return FALSE;
	}
	
	if ((char)(val>>8)==backBite) {
		printf("당신의 수 0x%x의 저장스타일은 0x%x || 0x%x \n", val,*temp0, *(temp0 + 1));
		return TRUE;
	}
	else {
		
		return FALSE;
	}
}
BOOL IsBigEndian(const u_short& val) {
	char* temp0 = (char*)&val;
	char frontBite = *temp0;
	char backBite = *(temp0 + 1);
	
	if (backBite == 0x00) {
		printf("비교할 수 없는 수를 주었습니다.");
		return FALSE;
	}
	if ((char)(val << 8) == backBite) {
		printf("당신의 수 0x%x의 저장스타일은 0x%x || 0x%x \n", val, *temp0, *(temp0 + 1));
		return TRUE;
	}
	else {
		
		return FALSE;
	}
}
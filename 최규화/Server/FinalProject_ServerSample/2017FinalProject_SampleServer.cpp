#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996)
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include "stdafx_sample.h"
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

//소켓 함수 오류 출력 후 종료
//--------------------------------전역변수 모음 시작--------------------------------------------------------------------------------------------------------------------------------------------------------
HANDLE hPlayer1Event;
HANDLE hPlayer2Event;

int sendid = -1;
CharacterInfo g_cinfo[2];
//float g_PrevTime = 0.0f;
//float g_CurTime = 0.0f;
//float g_ElaspedTime = 0.0f;

CRITICAL_SECTION cs;

//--------------------------------전역변수 모음 끝--------------------------------------------------------------------------------------------------------------------------------------------------------
//bool IsCollide(CharacterInfo, CarObjectInfo);
//void MoveCar(CarObjectInfo);


//에러 발생 시 메시지 출력 및 종료 함수
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
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
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;
	while (left > 0) 
	{
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

//쓰레드 1함수
DWORD WINAPI Player1Thread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;

	//클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	int tempid = 0;
	int count = 0;

	EnterCriticalSection(&cs);
	sendid++;
	LeaveCriticalSection(&cs);

	CharacterInfo cinfo;
	cinfo.char_pos.x = INIT_LEFT_CHAR_POSX;
	cinfo.char_pos.y = INIT_CHAR_POSY;
	g_cinfo[1].char_pos = { INIT_RIGHT_CHAR_POSX, INIT_CHAR_POSY };
	g_cinfo[1].id = 1;

	CharID charID;
	charID.id = tempid;

	while (1)
	{
		EnterCriticalSection(&cs);
		count = (count + 1) % MAX_COUNT;
		LeaveCriticalSection(&cs);

		if (count == EXCUTE_COUNT)
		{
			//EnterCriticalSection(&cs);

			//g_PrevTime = timeGetTime();
			//자신의 캐릭터 정보 수신
			retval = send(client_sock, (char*)&charID, sizeof(charID), 0);


			//1. 캐릭터의 정보를 받는다.
			retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
			if (retval == SOCKET_ERROR) { err_display("send()"); break;}
			
			if (tempid == 0) 
			{
				g_cinfo[0].char_pos.x = cinfo.char_pos.x;
				g_cinfo[0].char_pos.y = cinfo.char_pos.y;
			}
			else 
			{
				g_cinfo[1].char_pos.x = cinfo.char_pos.x;
				g_cinfo[1].char_pos.y = cinfo.char_pos.y;
			}
			printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);

			//2. 상대방캐릭터에 대한 정보를 보낸다.
			if (tempid == 0)
				retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
			else
				retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
			if (retval == SOCKET_ERROR) { err_display("send()"); break; }

		
			if (tempid == 0)
				printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
			else
				printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);

			//EnterCriticalSection(&cs);
			//g_CurTime = timeGetTime();
			//g_ElaspedTime += (g_CurTime - g_PrevTime) / 1000.0f;
			//LeaveCriticalSection(&cs);

		}
	}

	//closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소 =%s,포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}
DWORD WINAPI Player2Thread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;

	//클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);

	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);


	int tempid = 1;
	int count = 0;
	EnterCriticalSection(&cs);
	sendid++;
	LeaveCriticalSection(&cs);

	CharacterInfo cinfo;
	cinfo.char_pos.x = INIT_RIGHT_CHAR_POSX;
	cinfo.char_pos.y = INIT_CHAR_POSY;
	g_cinfo[0].char_pos = { INIT_LEFT_CHAR_POSX, INIT_CHAR_POSY };
	g_cinfo[0].id = 1;

	CharID charID;
	charID.id = tempid;

	while (1)
	{
		
		EnterCriticalSection(&cs);
		count = (count + 1) % MAX_COUNT;
		LeaveCriticalSection(&cs);
		
		if (count == EXCUTE_COUNT)
		{
			//자신의 캐릭터 정보 수신
			retval = send(client_sock, (char*)&charID, sizeof(charID), 0);	


			retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
			//아이디 값에 따라 캐릭터 정보 전역변수에 갱신
			if (tempid == 0)
			{
				g_cinfo[0].char_pos.x = cinfo.char_pos.x;
				g_cinfo[0].char_pos.y = cinfo.char_pos.y;
			}
			else
			{
				g_cinfo[1].char_pos.x = cinfo.char_pos.x;
				g_cinfo[1].char_pos.y = cinfo.char_pos.y;
			}
			printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);

			//아이디 값에 따라 상대방 캐릭터 정보값 발신
			if (tempid == 0)
				retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
			else
				retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}

			if (tempid == 0)
				printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
			else
				printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);

			//시간 정보 갱신
			//EnterCriticalSection(&cs);
			//g_CurTime = timeGetTime();
			//g_ElaspedTime += (g_CurTime - g_PrevTime) / 1000.0f;
			//
			//LeaveCriticalSection(&cs);
		}

	}

	//closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소 =%s,포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

int main(int argc, char* argv[])
{
	//소켓관련함수를 실행할 때 return value를 출력해주는 함수
	int retval;

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	//socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");

	SOCKADDR_IN clientaddr;

	SOCKET client_sock;
	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");
	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	ZeroMemory(&clientaddr, sizeof(clientaddr));

	int addrlen;
	HANDLE hThread;

	int usercount = 0;
	int count = 0;
	CharacterInfo cinfo;
	CharacterInfo cinfo2;

	InitializeCriticalSection(&cs);
	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) { err_display("accept()"); break; }
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));


		if (usercount == 0)
		{
			hThread = CreateThread(NULL, 0, Player1Thread, (LPVOID)client_sock, 0, NULL);
			if (hThread == NULL) { closesocket(client_sock); }
			else { CloseHandle(hThread);}
		}
		else if(usercount==1)
		{
			hThread = CreateThread(NULL, 0, Player2Thread, (LPVOID)client_sock, 0, NULL);
			if (hThread == NULL) { closesocket(client_sock);}
			else { CloseHandle(hThread);}
		}

		usercount = usercount + 1;;
	}
	DeleteCriticalSection(&cs);

	closesocket(sock);
	WSACleanup();
	return 0;
}

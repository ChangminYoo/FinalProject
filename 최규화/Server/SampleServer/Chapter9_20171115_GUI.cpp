#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996)
#include <stdlib.h>
#ifndef _WINSOCK2API_
#include <WINSOCK2.H>
#include <windows.h>
#endif
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 512

//������ ���ν���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//���� ��Ʈ�� ��� �Լ�
void DisplayText(char* fmt, ...);
void err_quit(char* msg);
void err_display(char* msg);
//���� ��� ������ �Լ�
DWORD WINAPI ServerMain(LPVOID arg);
DWORD WINAPI ProcessClient(LPVOID arg);

HINSTANCE hInst; // �ν��Ͻ� �ڵ�
HWND hEdit; //���� ��Ʈ��
CRITICAL_SECTION cs; //�Ӱ迵��

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstancs, LPSTR lpCmdLine, int nCmdShow) 
{
	hInst = hInstance;
	InitializeCriticalSection(&cs);

	//������ Ŭ���� ���
	WNDCLASS WndClass;
	//g_hInst = hInstance;

	HACCEL hAcc;
	//memset(&Message, 0, sizeof(Message));
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_NO); //IDC_NO ���콺Ŀ���� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	
	WndClass.lpszClassName = "MyWndClass";
	if (RegisterClass(&WndClass)) return 1;
	
	HWND hWnd = CreateWindow("MyWndClass", "TCP ����", WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, (HMENU)NULL, hInstance, NULL);
	if (hWnd == NULL) return 1;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	CreateThread(NULL, 0, ServerMain, NULL, 0, NULL);
	//�޽��� ����
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DeleteCriticalSection(&cs);
	return msg.wParam;

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//�������� Ŭ��� ����
	case WM_CREATE:
		hEdit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY, 0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL);
		return 0;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);

}

void DisplayText(char* fmt, ...) 
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[BUFSIZE+256];
	vsprintf(cbuf, fmt, arg);
	EnterCriticalSection(&cs);
	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	LeaveCriticalSection(&cs);
	va_end(arg);
}

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


//TCP ���� ���� �κ�

DWORD WINAPI ServerMain(LPVOID arg)
{
	int retval;
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen, file_read_len;
	HANDLE hThread;

	while (1)
	{
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}
		DisplayText("\r\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� =%s, ��Ʈ ��ȣ=%d\r\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		//������ ����
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL)
		{
			closesocket(client_sock);
		}
		else
		{
			CloseHandle(hThread);
		}
	}
	//closesocket()
	closesocket(listen_sock);
	WSACleanup();
	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	//Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
	while (1)
	{
		//������ �ޱ�
		retval = recv(client_sock, buf, BUFSIZE,0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		//���� ������ ���
		buf[retval] = '\0';
		DisplayText("\r\n[TCP \%s:%d]%s\r\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port),buf);
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
	}
	//closesocket()
	closesocket(client_sock);
	DisplayText("\r\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� =%s, ��Ʈ ��ȣ=%d\r\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	return 0;
}
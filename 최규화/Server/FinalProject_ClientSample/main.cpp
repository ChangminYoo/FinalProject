#pragma comment(lib,"winmm.lib")
#pragma warning(disable : 4996)
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#pragma comment(lib, "ws2_32")
#include "stdafx.h"
#include "InGameScene.h"
#include "CClient.h"

//===================
CInGameScene* g_InGame = NULL; // 인게임 씬 생성.

CClient *main_client = nullptr;
bool g_IsActive_InGame(false); // 씬이 활성화 되어있는지 판별

int MySelectedChar, OppSelectedChar;

int g_current_Scene = IN_GAME_SCENE; // 현재 씬이 무엇인지 판별
//===================

HINSTANCE g_hInst;
HWND g_hwnd;
LPCTSTR lpszClass = TEXT("2017 FinalProject_SampleServerClient");
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
UINT wDeviceID = 0;
bool bQuit = false;

DWORD elapsedTime = 0;
DWORD prevTime = 0, currTime = 0;
POINT mouse;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char szAppName[] = "FP_Sample";
	char szTitle[] = "Final Project Sample";
	main_client = new CClient();
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	
	HACCEL hAcc;
	memset(&Message, 0, sizeof(Message));

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;// MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = NULL;//CS_HREDRAW | CS_VREDRAW;
	hAcc = LoadAccelerators(g_hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	RegisterClass(&WndClass);
	g_hwnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOWSIZE_WIDTH, WINDOWSIZE_HEIGHT,
		NULL, (HMENU)NULL, hInstance, NULL);

	CreateThread(NULL, 0, main_client->ProcessClient, main_client, 0, NULL);
	//=========================================

	if (NULL != g_hwnd)
	{
		ShowWindow(g_hwnd, nCmdShow);
		while (!bQuit)
		{
			if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
			{
				if (Message.message == WM_QUIT) break;
				if (!TranslateAccelerator(Message.hwnd, hAcc, &Message))
				{
					TranslateMessage(&Message);
					DispatchMessage(&Message);
				}
			}
			else
			{
				currTime = timeGetTime();
				elapsedTime = currTime - prevTime;
				switch (g_current_Scene)
				{
				case IN_GAME_SCENE:
					if (!g_IsActive_InGame) // 인게임이 초기화 되지 않았다면 초기화 수행.
					{
						g_InGame = new CInGameScene();
						g_InGame->Initializing(MySelectedChar, OppSelectedChar, g_hwnd, g_hInst);
						g_IsActive_InGame = true;
					}

					// 인게임 내용 수행.
					g_InGame->FrameAdvance(elapsedTime, main_client, main_client->m_charInfo, main_client->m_opcharInfo);
							
					break;
				}
				prevTime = currTime;
			}
		}
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		GetCursorPos(&mouse);
		ScreenToClient(hwnd, &mouse);
		break;
	case WM_KEYDOWN:
		switch (wParam) 
		{
		case VK_ESCAPE: // ESC로 종료
			PostQuitMessage(0);
			return 0;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;
		break;

	case WM_PAINT:
		break;

	case WM_DESTROY:
		if (wDeviceID)
			mciSendCommand(wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
		if (g_InGame != NULL)
		{
			g_InGame->DestroyObjects();
			delete g_InGame;
			g_InGame = NULL;
		}
		PostQuitMessage(0);
		bQuit = true;
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

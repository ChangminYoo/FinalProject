
#include "stdafx.h"
#include "InGameScene.h"
#include "CharSelectScene.h"
#include "WaitRoomScene.h"
#include "CClient.h"
#pragma comment(lib,"winmm.lib")
#pragma warning(disable : 4996)
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#pragma comment(lib, "ws2_32")


//===================
CCharSelectScene* g_CharSel = NULL; // 캐릭터 선택 씬 생성.
CWaitRoomScene* g_WaitRoom = NULL; // 대기방 씬 생성.
CInGameScene* g_InGame = NULL; // 인게임 씬 생성.
//추가 규화 코드 . 통신을 위한 클래스
CClient *main_client = nullptr;

bool g_IsActive_CharSel(false); // 씬이 활성화 되어있는지 판별
bool g_IsActive_WaitRoom(false); // 씬이 활성화 되어있는지 판별
bool g_IsActive_InGame(false); // 씬이 활성화 되어있는지 판별

int MySelectedChar, OppSelectedChar;

int g_current_Scene = CHAR_SELECT_SCENE; // 현재 씬이 무엇인지 판별
//===================

HINSTANCE g_hInst;
HWND g_hwnd;
LPCTSTR lpszClass = TEXT("2017 NGP Term Project");
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
UINT wDeviceID = 0;
bool bQuit = false;


DWORD elapsedTime = 0;
DWORD prevTime = 0, currTime = 0;
POINT mouse;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	char szAppName[] = "NGP";
	char szTitle[] = "Network Game Programing Term Project";
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
				case CHAR_SELECT_SCENE:
					if (!g_IsActive_CharSel) // 캐릭터 선택창이 초기화 되지 않았다면 초기화 수행.
					{
						g_CharSel = new CCharSelectScene();
						g_CharSel->Initializing(g_hwnd, g_hInst);
						g_IsActive_CharSel = true;
					}

					g_CharSel->FrameAdvance(elapsedTime); // 캐릭터 선택창 내용 수행.

					if (g_CharSel->GetSceneSwap()) // 씬 전환 명령이 나오면
					{
						MySelectedChar = g_CharSel->GetMySC(); // 내가 선택한 캐릭터 받아오기
						OppSelectedChar = g_CharSel->GetOppSC(); // 상대방이 선택한 캐릭터 받아오기

						g_CharSel->DestroyObjects(); // 지우는 작업 1
						delete g_CharSel; // 지우는 작업 2
						g_CharSel = NULL; // 지우는 작업 3

						g_current_Scene = WAIT_ROOM_SCENE; // 씬 전환 준비
						g_IsActive_CharSel = false; // 캐릭터 선택 씬 비활성화
					}
					break;

				case WAIT_ROOM_SCENE:
					if (!g_IsActive_WaitRoom) // 대기방이 초기화 되지 않았다면 초기화 수행.
					{
						g_WaitRoom = new CWaitRoomScene();
						g_WaitRoom->Initializing(g_hwnd, g_hInst, MySelectedChar, OppSelectedChar);
						g_IsActive_WaitRoom = true;
					}
					//g_WaitRoom->FrameAdvance(elapsedTime);
					g_WaitRoom->FrameAdvance(elapsedTime,main_client->m_waitThreadInWait); // 대기방 내용 수행.

					if (g_WaitRoom->GetSceneSwap()) // 씬 전환 명령이 나오면
					{
						g_WaitRoom->DestroyObjects(); // 씬 지우는 작업 1
						delete g_WaitRoom; // 씬 지우는 작업 2
						g_WaitRoom = NULL; // 씬 지우는 작업 3

						g_current_Scene = IN_GAME_SCENE; // 씬 전환 준비
						g_IsActive_WaitRoom = false; // 대기방 씬 비활성화
					}
					break;

				case IN_GAME_SCENE:
					if (!g_IsActive_InGame) // 인게임이 초기화 되지 않았다면 초기화 수행.
					{
						g_InGame = new CInGameScene();
						g_InGame->Initializing(MySelectedChar, OppSelectedChar, g_hwnd, g_hInst);
						g_IsActive_InGame = true;
					}

					g_InGame->FrameAdvance(elapsedTime,main_client, main_client->m_carinfo, main_client->m_charInfo,main_client->m_opcharInfo); // 인게임 내용 수행.

					break;
				}
				prevTime = currTime;
			}
		}
		if (g_CharSel != NULL)
			g_CharSel->DestroyObjects();
		if (g_InGame != NULL)
			g_InGame->DestroyObjects();
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		GetCursorPos(&mouse);
		ScreenToClient(hwnd, &mouse);
		if (g_IsActive_CharSel)
			g_CharSel->Select(mouse,main_client); // 캐릭터 선택
		if (g_IsActive_WaitRoom)
			g_WaitRoom->Ready(mouse, main_client); // 준비 완료
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		/*case VK_UP:
		if (g_InGame != NULL)
			main_client->MovePlayerInfo(0);
			break;
		case VK_DOWN:
		if (g_InGame != NULL)
			main_client->MovePlayerInfo(1);
			break;
		case VK_LEFT:
		if (g_InGame != NULL)
			main_client->MovePlayerInfo(2);
			break;
		case VK_RIGHT:
		if (g_InGame != NULL)
			main_client->MovePlayerInfo(3);
			break;
			*/
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
		if (g_CharSel != NULL)
			g_CharSel->DestroyObjects();
		if (g_InGame != NULL)
			g_InGame->DestroyObjects();
		PostQuitMessage(0);
		bQuit = true;
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

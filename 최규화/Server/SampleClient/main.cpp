
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
CCharSelectScene* g_CharSel = NULL; // ĳ���� ���� �� ����.
CWaitRoomScene* g_WaitRoom = NULL; // ���� �� ����.
CInGameScene* g_InGame = NULL; // �ΰ��� �� ����.
//�߰� ��ȭ �ڵ� . ����� ���� Ŭ����
CClient *main_client = nullptr;

bool g_IsActive_CharSel(false); // ���� Ȱ��ȭ �Ǿ��ִ��� �Ǻ�
bool g_IsActive_WaitRoom(false); // ���� Ȱ��ȭ �Ǿ��ִ��� �Ǻ�
bool g_IsActive_InGame(false); // ���� Ȱ��ȭ �Ǿ��ִ��� �Ǻ�

int MySelectedChar, OppSelectedChar;

int g_current_Scene = CHAR_SELECT_SCENE; // ���� ���� �������� �Ǻ�
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
					if (!g_IsActive_CharSel) // ĳ���� ����â�� �ʱ�ȭ ���� �ʾҴٸ� �ʱ�ȭ ����.
					{
						g_CharSel = new CCharSelectScene();
						g_CharSel->Initializing(g_hwnd, g_hInst);
						g_IsActive_CharSel = true;
					}

					g_CharSel->FrameAdvance(elapsedTime); // ĳ���� ����â ���� ����.

					if (g_CharSel->GetSceneSwap()) // �� ��ȯ ����� ������
					{
						MySelectedChar = g_CharSel->GetMySC(); // ���� ������ ĳ���� �޾ƿ���
						OppSelectedChar = g_CharSel->GetOppSC(); // ������ ������ ĳ���� �޾ƿ���

						g_CharSel->DestroyObjects(); // ����� �۾� 1
						delete g_CharSel; // ����� �۾� 2
						g_CharSel = NULL; // ����� �۾� 3

						g_current_Scene = WAIT_ROOM_SCENE; // �� ��ȯ �غ�
						g_IsActive_CharSel = false; // ĳ���� ���� �� ��Ȱ��ȭ
					}
					break;

				case WAIT_ROOM_SCENE:
					if (!g_IsActive_WaitRoom) // ������ �ʱ�ȭ ���� �ʾҴٸ� �ʱ�ȭ ����.
					{
						g_WaitRoom = new CWaitRoomScene();
						g_WaitRoom->Initializing(g_hwnd, g_hInst, MySelectedChar, OppSelectedChar);
						g_IsActive_WaitRoom = true;
					}
					//g_WaitRoom->FrameAdvance(elapsedTime);
					g_WaitRoom->FrameAdvance(elapsedTime,main_client->m_waitThreadInWait); // ���� ���� ����.

					if (g_WaitRoom->GetSceneSwap()) // �� ��ȯ ����� ������
					{
						g_WaitRoom->DestroyObjects(); // �� ����� �۾� 1
						delete g_WaitRoom; // �� ����� �۾� 2
						g_WaitRoom = NULL; // �� ����� �۾� 3

						g_current_Scene = IN_GAME_SCENE; // �� ��ȯ �غ�
						g_IsActive_WaitRoom = false; // ���� �� ��Ȱ��ȭ
					}
					break;

				case IN_GAME_SCENE:
					if (!g_IsActive_InGame) // �ΰ����� �ʱ�ȭ ���� �ʾҴٸ� �ʱ�ȭ ����.
					{
						g_InGame = new CInGameScene();
						g_InGame->Initializing(MySelectedChar, OppSelectedChar, g_hwnd, g_hInst);
						g_IsActive_InGame = true;
					}

					g_InGame->FrameAdvance(elapsedTime,main_client, main_client->m_carinfo, main_client->m_charInfo,main_client->m_opcharInfo); // �ΰ��� ���� ����.

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
			g_CharSel->Select(mouse,main_client); // ĳ���� ����
		if (g_IsActive_WaitRoom)
			g_WaitRoom->Ready(mouse, main_client); // �غ� �Ϸ�
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
		case VK_ESCAPE: // ESC�� ����
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

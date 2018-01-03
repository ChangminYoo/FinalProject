
//	Press T to toggle 텍스쳐 바꿈
//	Press 1 for 2D view.
//	Press 2 for 3D view.

#include "Window.h"
#include "Scene.h"
#include <windowsx.h> 
using namespace std;
using namespace graphics;
using namespace window;

static const LPCWSTR	appName = L"Directx 12 Terrain Renderer";
static const int		WINDOW_HEIGHT = 1080;
static const int		WINDOW_WIDTH = 1920;
//static const int		WINDOW_HEIGHT = 2160;	
//static const int		WINDOW_WIDTH = 3840;

static const bool		FULL_SCREEN = false;
static Scene*			pScene = nullptr;
static int				lastMouseX = -1;
static int				lastMouseY = -1;

static void KeyUp(UINT key)
{
	switch (key)
	{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return;
	}
}

static void KeyDown(UINT key)
{
	switch (key) {
		case VK_SPACE:
		case _W:
		case _S:
		case _A:
		case _D:
		case _Q:
		case _Z:
		case _1:
		case _2:
		case _T:
		case _L:
			pScene->HandleKeyboardInput(key);
			break;
		case VK_ESCAPE:
			exit(1);
			break;
	}
}

static void HandleMouseMove(LPARAM lp)
{
	int x = GET_X_LPARAM(lp);
	int y = GET_Y_LPARAM(lp);

	if (lastMouseX == -1) 
	{ 
	}
	
	else
	{ 
		//마지막으로 업데이트 한 시간부터 얼마나 지났는지 계산하고 해당 정보를 장면에 전달
		//값을 클램프
		//마우스가 창 밖으로 나간 다음 처리		
		int moveX = lastMouseX - x;	
		moveX = moveX > 20 ? 20 : moveX;
		moveX = moveX < -20 ? -20 : moveX;
		int moveY = lastMouseY - y;
		moveY = moveY > 20 ? 20 : moveY;
		moveY = moveY < -20 ? -20 : moveY;
		pScene->HandleMouseInput(moveX, moveY);
	}
	
	// update our last mouse coordinates;
	lastMouseX = x;
	lastMouseY = y;
}

static LRESULT CALLBACK WndProc(HWND win , UINT msg, WPARAM wp, LPARAM lp) 
{
	switch (msg) 
	{
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_KEYUP:
			KeyUp((UINT)wp);
			break;
		case WM_KEYDOWN:
			KeyDown((UINT)wp);
			break;
		case WM_MOUSEMOVE:
			HandleMouseMove(lp);
			break;
		default:
			return DefWindowProc(win, msg, wp, lp);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow) 
{
	try {
		Window WIN(appName, WINDOW_HEIGHT, WINDOW_WIDTH, WndProc, FULL_SCREEN);
		Device DEV(WIN.GetWindow(), WIN.Height(), WIN.Width());
		Scene S(WIN.Height(), WIN.Width(), &DEV);
		pScene = &S; 

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (1) 
		{
			if (PeekMessage(&msg, WIN.GetWindow(), 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} 
			if (msg.message == WM_QUIT) { 
				pScene = nullptr;
				return 1;
			}

			S.Update();
		}

		pScene = nullptr;
		return 2;
	} catch (GFX_Exception& e) {
		OutputDebugStringA(e.what());
		pScene = nullptr;
		return 3;
	} catch (Window_Exception& e) {
		OutputDebugStringA(e.what());
		pScene = nullptr;
		return 4;
	}
}
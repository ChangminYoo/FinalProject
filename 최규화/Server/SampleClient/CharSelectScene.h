#pragma once
#include "stdafx.h"
#include "Object.h"
#include "CClient.h"
#define MAXCHAR 3

#define ANIMATE_COOLTIME 0.2f
class CClient;
class CCharSelectScene
{
private:

	Object* m_pChar[MAXCHAR];
	CClient *m_Client;
	HWND m_hWnd;
	HINSTANCE m_hInst;


	HDC m_hDC, m_hBackMemdc, m_hMemdc, m_hBackBuffer;
	PAINTSTRUCT m_paintstruct;
	HBRUSH m_hBrush, m_hOldBrush;
	RECT rect;

	HBITMAP m_hBitmap_Back, m_hBitmap_Copy;
	HBITMAP m_hBitmap_Background;
	HBITMAP m_hBitmap_Selected_Char[MAXCHAR];

	int m_MySC = -1;
	int m_OppSC = -2;

	bool m_IsMySelected = false;
	bool m_IsOppSelected = true;
	bool m_bSceneSwap = false;

	float m_AnimateCoolTime = 0.0f;
//규화 추가 - 인스턴스 활용
protected:
	CPacketInfo *m_Packet = nullptr;
public:
	CCharSelectScene();
	~CCharSelectScene();

	// hWnd
	HWND GethWnd() const { return m_hWnd; };
	void SethWnd(HWND hw) { m_hWnd = hw; };

	// hInst
	HINSTANCE GethInst() const { return m_hInst; };
	void SethInst(HINSTANCE hi) { m_hInst = hi; };

	// select
	int GetMySC() { return m_MySC; };
	int GetOppSC() { return m_OppSC; };
	void SetMySC(int s) { m_MySC = s; };
	void SetOppSC(int s) { m_OppSC = s; };

	void SetIsMySelected(bool p1s) { m_IsMySelected = p1s; };
	void SetIsOppSelected(bool p2s) { m_IsOppSelected = p2s; };

	bool GetSceneSwap() { return m_bSceneSwap; };
	void SetSceneSwap() { if (m_Packet->g_swapScene) m_bSceneSwap = true; };
	void RotateObjects(CClient *Client);

	void Initializing(HWND hwnd, HINSTANCE inst);
	void UpdateScene();
	void DrawScene();

	//성일 수정
	void Select(POINT mouse, CClient* client);

	void FrameAdvance(float elapsedTime);

	void DestroyObjects();
};


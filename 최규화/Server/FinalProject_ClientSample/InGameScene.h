#pragma once
#include "stdafx.h"
#include "Object.h"
#include "CPacketInfo.h"
#include "CClient.h"

#define MAXLEFTCAR 4
#define MAXRIGHTCAR 4
#define MAXITEM 4
#define MAXOBSTACLE 3

// CoolTimes
#define ANIMATE_COOLTIME 0.1f
#define MOVE_COOLTIME 0.01f
#define SCORE_COOLTIME 0.05f
#define ITEM_COOLTIME 15.0f
#define OBSTACLE_COOLTIME 5.0f

class CInGameScene
{
private:
	Object* m_pObjectMyPlayer;
	Object* m_pObjectOppPlayer;
	
	HWND m_hWnd;
	HINSTANCE m_hInst;

	HDC m_hDC, m_hBackMemdc, m_hMemdc, m_hBackBuffer;
	PAINTSTRUCT m_paintstruct;
	HBRUSH m_hBrush, m_hOldBrush;
	RECT rect;

	HBITMAP m_hBitmap_Back, m_hBitmap_Copy;
	HBITMAP m_hBitmap_Background;
	HBITMAP m_hBitmap_MyPlayerOrigin, m_hBitmap_OppPlayerOrigin;
	HBITMAP m_hBitmap_MyPlayerStuck, m_hBitmap_OppPlayerStuck;

	HBITMAP m_hBitmap_YouWin, m_hBitmap_YouLose;
	BLENDFUNCTION m_BlendFunction;

	float m_AnimateCoolTime = 0.0f;
	float m_MoveCoolTime = 0.0f;

protected:
	CPacketInfo *m_Packet = nullptr;
public:
	CInGameScene();
	~CInGameScene();

	//===========
	// Get & Set
	//===========

	// hWnd
	HWND GethWnd() const { return m_hWnd; };
	void SethWnd(HWND hw) { m_hWnd = hw; };

	// hInst
	HINSTANCE GethInst() const { return m_hInst; };
	void SethInst(HINSTANCE hi) { m_hInst = hi; };

	// ===============

	void Initializing(int psc, int p2sc, HWND hwnd, HINSTANCE inst);

	void BuildObjects(int objectType);

	void UpdateObjects(CClient* cl, CharacterInfo charinfo, CharacterInfo c);
	void DrawObjects();

	void GetKeyInput();

	void FrameAdvance(float elapsedTime, CClient* cl, CharacterInfo b, CharacterInfo c);
	void DestroyObjects();
	
};


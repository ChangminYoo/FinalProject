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

class CClient;
class CInGameScene
{
private:

	
	Object* m_pObjectMyPlayer;
	Object* m_pObjectOppPlayer;
	Object* m_pObjectLeftLineCars[MAXLEFTCAR];
	Object* m_pObjectRightLineCars[MAXRIGHTCAR];
	Object* m_pObjectItemtemp;
	
	Object* m_pObjectItem[MAXITEM];

	Object* m_pObjectObstacle[MAXOBSTACLE];

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

	// 선택한 캐릭터의 비트맵 번호를 구하기 위한 변수
	int m_MySelectedChar;
	int m_OppSelectedChar;

	// 아이템
	int m_curItemCount = -1;
	
	// 장애물
	int m_curObstacleCount = -1;
	bool firstSet = false;

	// 점수
	int m_MyScore = 0;
	int m_OppScore = 0;
	char m_MyScore_Text[24];
	char m_OppScore_Text[24];
	
	float m_AnimateCoolTime = 0.0f;
	float m_MoveCoolTime = 0.0f;
	float m_ScoreCoolTime = 0.0f;
	float m_ItemCoolTime = 0.0f;
	float m_ObstacleCoolTime = 0.0f;
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

	// score
	int GetMyScore() const { return m_MyScore; };
	void SetMyScore(int score) { m_MyScore = score; };
	void PlusMyScore(int score) { m_MyScore += score; };

	int GetOppScore() const { return m_OppScore; };
	void SetOppScore(int score) { m_OppScore = score; };

	


	// ===============

	void Initializing(int psc, int p2sc, HWND hwnd, HINSTANCE inst);

	void BuildObjects(int objectType);

	void UpdateObjects(CarObjectInfo car, CClient* cl, CharacterInfo charinfo, CharacterInfo c);
	void DrawObjects();

	void GetKeyInput();

	void FrameAdvance(float elapsedTime, CClient* cl, CarObjectInfo a, CharacterInfo b, CharacterInfo c);

	void DestroyObjects();
	
};


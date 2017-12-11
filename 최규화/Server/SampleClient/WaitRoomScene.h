#pragma once
#include "stdafx.h"
#include "Object.h"
#include "CClient.h"
#include "CPacketInfo.h"

class CClient;
class CWaitRoomScene
{
private:
	Object* m_pPlayer[2];

	HWND m_hWnd;
	HINSTANCE m_hInst;

	HDC m_hDC, m_hBackMemdc, m_hMemdc, m_hBackBuffer;
	PAINTSTRUCT m_paintstruct;
	HBRUSH m_hBrush, m_hOldBrush;
	RECT rect;

	HBITMAP m_hBitmap_Back, m_hBitmap_Copy;
	HBITMAP m_hBitmap_Background;
	HBITMAP m_hBitmap_My_ReadyButton, m_hBitmap_My_ReadyIcon;
	HBITMAP m_hBitmap_Opp_ReadyButton, m_hBitmap_Opp_ReadyIcon;

	//=====================
	// 플레이어가 선택한 캐릭터가 몇번인가?
	// 그림을 정해주기 위한것이므로 차라리 서버에서 따로 보내준다면
	// 이 변수를 사용할 필요가 없다.
	int m_My_Selected_Char;
	int m_Opp_Selected_Char;
	//=====================

	//=====================
	// 각 플레이어를 어느 위치에 그릴지 결정해줄 좌표 변수들.

	int m_My_Char_PosX; // 이 클라이언트의 좌표
	int m_My_Char_PosY; // 이 클라이언트의 좌표

	int m_Opp_Char_PosX; // 상대 클라이언트의 좌표
	int m_Opp_Char_PosY; // 상대 클라이언트의 좌표

	int m_Char_SizeX; // 사이즈는 동일하므로 구분하지않음..
	int m_Char_SizeY;

	int m_My_RButton_PosX; // 이 클라이언트의 버튼 좌표
	int m_My_RButton_PosY; // 이 클라이언트의 버튼 좌표
	
	int m_Opp_RButton_PosX; // 상대 클라이언트의 버튼 좌표
	int m_Opp_RButton_PosY; // 상대 클라이언트의 버튼 좌표

	int m_RButton_SizeX; // 사이즈는 동일하므로 구분하지않음..
	int m_RButton_SizeY;
	//=====================

	bool m_My_Ready; // 이 클라이언트의 준비상태
	bool m_Opp_Ready; // 상대 클라이언트의 준비상태
	bool m_bSceneSwap; // 씬 전환 준비가 되었는가?
protected:
	CPacketInfo *m_Packet = nullptr;
public:
	CWaitRoomScene();
	~CWaitRoomScene();

	// hWnd
	HWND GethWnd() const { return m_hWnd; };
	void SethWnd(HWND hw) { m_hWnd = hw; };

	// hInst
	HINSTANCE GethInst() const { return m_hInst; };
	void SethInst(HINSTANCE hi) { m_hInst = hi; };

	void Initializing(HWND hwnd, HINSTANCE inst, int p1Char, int p2Char);
	void DrawScene();

	void Ready(POINT mouse, CClient* c);

	void set_My_Ready(bool r) { m_My_Ready = r; };
	void set_Opp_Ready(bool r) { m_Opp_Ready = r; };
	void set_My_Ready_Button(HBITMAP rb) { m_hBitmap_My_ReadyButton = rb; };
	void set_Opp_Ready_Button(HBITMAP rb) { m_hBitmap_Opp_ReadyButton = rb; };

	bool GetSceneSwap() { return m_bSceneSwap; };
	void SetSceneSwap(WaitThread w) { if (w.isReady&&m_Opp_Ready) m_bSceneSwap = true; };
	void SetSceneSwap() { m_bSceneSwap = true; };

	void FrameAdvance(float elapsedTime);

	void FrameAdvance(float elapsedTime, WaitThread w);
	void SetCharInfo();
	void DestroyObjects();
};


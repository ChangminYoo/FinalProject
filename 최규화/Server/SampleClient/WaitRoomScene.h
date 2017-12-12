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
	// �÷��̾ ������ ĳ���Ͱ� ����ΰ�?
	// �׸��� �����ֱ� ���Ѱ��̹Ƿ� ���� �������� ���� �����شٸ�
	// �� ������ ����� �ʿ䰡 ����.
	int m_My_Selected_Char;
	int m_Opp_Selected_Char;
	//=====================

	//=====================
	// �� �÷��̾ ��� ��ġ�� �׸��� �������� ��ǥ ������.

	int m_My_Char_PosX; // �� Ŭ���̾�Ʈ�� ��ǥ
	int m_My_Char_PosY; // �� Ŭ���̾�Ʈ�� ��ǥ

	int m_Opp_Char_PosX; // ��� Ŭ���̾�Ʈ�� ��ǥ
	int m_Opp_Char_PosY; // ��� Ŭ���̾�Ʈ�� ��ǥ

	int m_Char_SizeX; // ������� �����ϹǷ� ������������..
	int m_Char_SizeY;

	int m_My_RButton_PosX; // �� Ŭ���̾�Ʈ�� ��ư ��ǥ
	int m_My_RButton_PosY; // �� Ŭ���̾�Ʈ�� ��ư ��ǥ
	
	int m_Opp_RButton_PosX; // ��� Ŭ���̾�Ʈ�� ��ư ��ǥ
	int m_Opp_RButton_PosY; // ��� Ŭ���̾�Ʈ�� ��ư ��ǥ

	int m_RButton_SizeX; // ������� �����ϹǷ� ������������..
	int m_RButton_SizeY;
	//=====================

	bool m_My_Ready; // �� Ŭ���̾�Ʈ�� �غ����
	bool m_Opp_Ready; // ��� Ŭ���̾�Ʈ�� �غ����
	bool m_bSceneSwap; // �� ��ȯ �غ� �Ǿ��°�?
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


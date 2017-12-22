#include "InGameScene.h"

random_device randomDevice;
default_random_engine dre(randomDevice());
uniform_int_distribution<> ui_randomPosX(ITEM_POSITION_MIN_X, ITEM_POSITION_MAX_X);
uniform_int_distribution<> ui_randomPosY(ITEM_POSITION_MIN_Y, ITEM_POSITION_MAX_Y);

CInGameScene::CInGameScene()
{
}

CInGameScene::~CInGameScene()
{
}

void CInGameScene::Initializing(int psc, int p2sc, HWND hwnd, HINSTANCE inst)
{
	SethWnd(hwnd);
	SethInst(inst);

	GetClientRect(m_hWnd, &rect);
	m_hBitmap_Background = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP_BACKG_ROAD));

	m_BlendFunction.AlphaFormat = 0;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.SourceConstantAlpha = 100;

	m_pObjectMyPlayer = NULL;
	m_pObjectOppPlayer = NULL;
	
	if (m_Packet->g_myID == 0)
	{
		m_pObjectMyPlayer = new Object
		(
			//m_Packet->g_charinfo.char_pos.x, m_Packet->g_charinfo.char_pos.y,
			INIT_LEFT_CHAR_POSX, INIT_CHAR_POSY,
			CHAR_DIR_X_INIT, CHAR_DIR_Y_INIT,
			CHAR_SPEED_INIT,
			CHAR_SIZE_X_INIT, CHAR_SIZE_Y_INIT,
			CHAR_SRC_SIZE_X_INIT, CHAR_SRC_SIZE_Y_INIT,
			CHAR_SRC_ANI_SIZE_X_INIT,
			IDB_BITMAP_CHAR_1,
			CHAR_MAX_ANI_COUNT_INIT,
			m_hInst);

		m_pObjectOppPlayer = new Object
		(
			OPP_POS_X_INIT, OPP_POS_Y_INIT,
			CHAR_DIR_X_INIT, CHAR_DIR_Y_INIT,
			CHAR_SPEED_INIT,
			CHAR_SIZE_X_INIT, CHAR_SIZE_Y_INIT,
			CHAR_SRC_SIZE_X_INIT, CHAR_SRC_SIZE_Y_INIT,
			CHAR_SRC_ANI_SIZE_X_INIT,
			IDB_BITMAP_CHAR_2,
			CHAR_MAX_ANI_COUNT_INIT,
			m_hInst);
	}
	else
	{
		m_pObjectMyPlayer = new Object
		(
			OPP_POS_X_INIT, OPP_POS_Y_INIT,
			CHAR_DIR_X_INIT, CHAR_DIR_Y_INIT,
			CHAR_SPEED_INIT,
			CHAR_SIZE_X_INIT, CHAR_SIZE_Y_INIT,
			CHAR_SRC_SIZE_X_INIT, CHAR_SRC_SIZE_Y_INIT,
			CHAR_SRC_ANI_SIZE_X_INIT,
			IDB_BITMAP_CHAR_2,
			CHAR_MAX_ANI_COUNT_INIT,
			m_hInst);

		m_pObjectOppPlayer = new Object
		(
			//m_Packet->g_charinfo.char_pos.x, m_Packet->g_charinfo.char_pos.y,
			INIT_LEFT_CHAR_POSX, INIT_CHAR_POSY,
			CHAR_DIR_X_INIT, CHAR_DIR_Y_INIT,
			CHAR_SPEED_INIT,
			CHAR_SIZE_X_INIT, CHAR_SIZE_Y_INIT,
			CHAR_SRC_SIZE_X_INIT, CHAR_SRC_SIZE_Y_INIT,
			CHAR_SRC_ANI_SIZE_X_INIT,
			IDB_BITMAP_CHAR_1,
			CHAR_MAX_ANI_COUNT_INIT,
			m_hInst);
	}

	BuildObjects(OBJECT_PLAYER);

};

void CInGameScene::BuildObjects(int objectType) // 캐릭터 선택에 따른 비트맵 변경을 위해 인자를 추가해주자.
{
	//switch(objectType)
	//{
	//case OBJECT_PLAYER:
	//	
	//	//m_pObjectMyPlayer = new Object(60, 400, 0, 0, 1, 70, 70, 122, 124, 121, IDB_BITMAP_CHAR_1 + m_MySelectedChar, 3, m_hInst);
	//
	//	// 이 비트맵들은 함정에 걸렸을 때 깜빡이는 효과를 주기 위한것..
	//	m_hBitmap_MyPlayerOrigin = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(m_MySelectedChar - 10));
	//	m_hBitmap_MyPlayerStuck = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(m_MySelectedChar - 10));
	//
	//	//m_pObjectOppPlayer = new Object(640, 400, 0, 0, 1, 70, 70, 122, 124, 121, IDB_BITMAP_CHAR_1 + m_OppSelectedChar, 3, m_hInst);
	//
	//	m_hBitmap_OppPlayerOrigin = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(m_OppSelectedChar - 10));
	//	m_hBitmap_OppPlayerStuck = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(m_OppSelectedChar - 10));
	//	break;
	//}
};


void CInGameScene::UpdateObjects(CClient* cl, CharacterInfo charinfo, CharacterInfo opcharinfo)
{
	// Char 
	if (m_Packet->g_myID == 0)
	{
		m_Packet->g_charinfo.char_pos.x = m_pObjectMyPlayer->GetPositionX();
		m_Packet->g_charinfo.char_pos.y = m_pObjectMyPlayer->GetPositionY();
	}
	else
	{
		m_Packet->g_opcharinfo.char_pos.x = m_pObjectMyPlayer->GetPositionX();
		m_Packet->g_opcharinfo.char_pos.y = m_pObjectMyPlayer->GetPositionY();
	}

	m_pObjectOppPlayer->SetPositionX(opcharinfo.char_pos.x);
	m_pObjectOppPlayer->SetPositionY(opcharinfo.char_pos.y);

	// MyPlayerBehavior
	if (m_pObjectMyPlayer != NULL)
	{
		if (m_AnimateCoolTime >= ANIMATE_COOLTIME)
		{
			// 애니메이트
			m_pObjectMyPlayer->Animate();
		}
	}

	// OppPlayerBehavior
	if (m_pObjectOppPlayer != NULL)
	{
		if (m_AnimateCoolTime >= ANIMATE_COOLTIME)
		{
			// 애니메이트
			m_pObjectOppPlayer->Animate();
		}
	}

	InvalidateRect(m_hWnd, NULL, FALSE);
	ShowWindow(m_hWnd, TRUE);
};

void CInGameScene::DrawObjects()
{
	m_hDC = BeginPaint(m_hWnd, &m_paintstruct);
	m_hBackMemdc = CreateCompatibleDC(m_hDC);
	m_hMemdc = CreateCompatibleDC(m_hDC);
	m_hBitmap_Back = CreateCompatibleBitmap(m_hDC, rect.right, rect.bottom); //도화지 준비!
	m_hBitmap_Copy = (HBITMAP)SelectObject(m_hBackMemdc, m_hBitmap_Back); //도화지 세팅

	FillRect(m_hBackMemdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	SelectObject(m_hMemdc, m_hBitmap_Background);
	StretchBlt(m_hBackMemdc, 0, 0, rect.right, rect.bottom - 50, m_hMemdc, 0, 0, 800, 600, SRCCOPY);

	m_pObjectMyPlayer->Draw(m_hBackMemdc, m_hMemdc);
	m_pObjectOppPlayer->Draw(m_hBackMemdc, m_hMemdc);

	m_hOldBrush = (HBRUSH)SelectObject(m_hDC, m_hBrush);
	SelectObject(m_hBackMemdc, m_hOldBrush);
	SetBkMode(m_hBackMemdc, TRANSPARENT);
	BitBlt(m_hDC, 0, 0, rect.right, rect.bottom, m_hBackMemdc, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(m_hBackMemdc, m_hBitmap_Copy));
	DeleteDC(m_hBackMemdc);
	DeleteDC(m_hMemdc);

	EndPaint(m_hWnd, &m_paintstruct);
};

void CInGameScene::GetKeyInput() 
{
	static UCHAR pKeyBuffer[256];
	if (GetKeyboardState(pKeyBuffer))
	{
		if (pKeyBuffer[VK_UP] & 0xF0)
		{
			m_pObjectMyPlayer->SetDirectionX(0);
			m_pObjectMyPlayer->SetDirectionY(-1);
			m_pObjectMyPlayer->Move();
		}
		if (pKeyBuffer[VK_DOWN] & 0xF0) 
		{
			m_pObjectMyPlayer->SetDirectionX(0);
			m_pObjectMyPlayer->SetDirectionY(1);
			m_pObjectMyPlayer->Move();
		}
		if (pKeyBuffer[VK_LEFT] & 0xF0) 
		{
			m_pObjectMyPlayer->SetDirectionX(-1);
			m_pObjectMyPlayer->SetDirectionY(0);
			m_pObjectMyPlayer->Move();
		}
		if (pKeyBuffer[VK_RIGHT] & 0xF0) 
		{
			m_pObjectMyPlayer->SetDirectionX(1);
			m_pObjectMyPlayer->SetDirectionY(0);
			m_pObjectMyPlayer->Move();
		}
 		if ((pKeyBuffer[VK_SPACE] & 0xF0) && !m_pObjectMyPlayer->GetIsJump())
		{
			m_pObjectMyPlayer->SetIsJump(true);
		}
	}
};

void CInGameScene::FrameAdvance(float elapsedTime, CClient* client, CharacterInfo charinfo, CharacterInfo opcharinfo)
{
	// 쿨타임 증가
	float coolTime = elapsedTime / 1000.0f;
	m_MoveCoolTime += coolTime;
	m_AnimateCoolTime += coolTime;

	GetKeyInput();
	UpdateObjects(client, charinfo, opcharinfo);
	// 쿨타임 초기화
	if (m_AnimateCoolTime >= ANIMATE_COOLTIME)
		m_AnimateCoolTime = 0.0f;
	if (m_MoveCoolTime >= MOVE_COOLTIME)
		m_MoveCoolTime = 0.0f;

		
	//SetSceneSwap();
	DrawObjects();

};

void CInGameScene::DestroyObjects() 
{
	if (m_pObjectMyPlayer != NULL)
	{
		delete m_pObjectMyPlayer;
		m_pObjectMyPlayer = NULL;
	}

	if (m_pObjectOppPlayer != NULL)
	{
		delete m_pObjectOppPlayer;
		m_pObjectOppPlayer = NULL;
	}
};
#include "WaitRoomScene.h"



CWaitRoomScene::CWaitRoomScene()
{
}


CWaitRoomScene::~CWaitRoomScene()
{
}

void CWaitRoomScene::Initializing(HWND hwnd, HINSTANCE inst, int p1Char, int p2Char)
{
	SethWnd(hwnd);
	SethInst(inst);
	GetClientRect(m_hWnd, &rect);
	m_hBitmap_Background = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP_BACKGROUND_WAITROOM));

	m_hBitmap_My_ReadyButton = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP_READY_BUTTON_F));
	m_hBitmap_My_ReadyIcon = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP_READY_DISPLAY));
	m_hBitmap_Opp_ReadyIcon = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP_READY_DISPLAY));



	m_My_Ready = false;
	m_Opp_Ready = false; // 일단은 서버적용을 안했기 때문에 true로 설정해둠.
	m_bSceneSwap = false;



	//====================
	// 캐릭터와 버튼을 그려줄 좌표 변수들.
	// 서버에서 받아오자.
	m_My_Char_PosX = 110;
	m_My_Char_PosY = 340;

	m_Opp_Char_PosX = 340;
	m_Opp_Char_PosY = 340;

	m_Char_SizeX = 150;
	m_Char_SizeY = 150;

	m_My_RButton_PosX = 625;
	m_My_RButton_PosY = 220;

	m_RButton_SizeX = 150;
	m_RButton_SizeY = 63;
	//====================



	//====================
	// 이 부분에서 서버에서 받아온 값을 바로 대입 해주자.
	// ex) m_P1_Selected_Char = 서버에서 받은 이 클라이언트의 캐릭터의 비트맵 값;
	// m_P2_Selected_Char = 서버에서 받은 상대 캐릭터의 비트맵 값;
	if (p1Char == 0) m_My_Selected_Char = IDB_BITMAP_CHAR_1_SEL;
	else if (p1Char == 1) m_My_Selected_Char = IDB_BITMAP_CHAR_2_SEL;
	else m_My_Selected_Char = IDB_BITMAP_CHAR_3_SEL;

	if (p2Char == 0) m_Opp_Selected_Char = IDB_BITMAP_CHAR_1_SEL;
	else if (p2Char == 1) m_Opp_Selected_Char = IDB_BITMAP_CHAR_2_SEL;
	else m_Opp_Selected_Char = IDB_BITMAP_CHAR_3_SEL;
	//====================



	// 캐릭터 생성
	m_pPlayer[0] = new Object(m_My_Char_PosX, m_My_Char_PosY, 0, 0, 0, m_Char_SizeX, m_Char_SizeY, 30, 29, 32, m_My_Selected_Char, 0, m_hInst);
	m_pPlayer[1] = new Object(m_Opp_Char_PosX, m_Opp_Char_PosY, 0, 0, 0, m_Char_SizeX, m_Char_SizeY, 30, 29, 32, m_Opp_Selected_Char, 0, m_hInst);
};

void CWaitRoomScene::DrawScene()
{
	m_hDC = BeginPaint(m_hWnd, &m_paintstruct);
	m_hBackMemdc = CreateCompatibleDC(m_hDC);
	m_hMemdc = CreateCompatibleDC(m_hDC);
	m_hBitmap_Back = CreateCompatibleBitmap(m_hDC, rect.right, rect.bottom); //도화지 준비!
	m_hBitmap_Copy = (HBITMAP)SelectObject(m_hBackMemdc, m_hBitmap_Back); //도화지 세팅

	SelectObject(m_hMemdc, m_hBitmap_Background); // 배경
	StretchBlt(m_hBackMemdc, 0, 0, rect.right, rect.bottom, m_hMemdc, 10, 10, 790, 590, SRCCOPY);
	SelectObject(m_hMemdc, m_hBitmap_My_ReadyButton); // P1 준비 버튼
	TransparentBlt(m_hBackMemdc, m_My_RButton_PosX, m_My_RButton_PosY, m_RButton_SizeX, m_RButton_SizeY, m_hMemdc, 0, 0, 282, 120, RGB(255, 122, 255));

	if (m_My_Ready)
	{
		SelectObject(m_hMemdc, m_hBitmap_My_ReadyIcon); // 내가 준비 완료시 띄울 로고
		TransparentBlt(m_hBackMemdc, m_Packet->g_waitRoomCharInfo.charPos[m_Packet->g_myID].x + 70, m_My_Char_PosY - 80, m_RButton_SizeX, m_RButton_SizeY, m_hMemdc, 0, 0, 295, 132, RGB(255, 122, 255));
	}

	if (m_Opp_Ready)
	{
		SelectObject(m_hMemdc, m_hBitmap_Opp_ReadyIcon); // 상대가 준비 완료시 띄울 로고
		if (m_Packet->g_myID == 0) {
			TransparentBlt(m_hBackMemdc, m_Packet->g_waitRoomCharInfo.charPos[1].x + 70, m_Opp_Char_PosY - 80, m_RButton_SizeX, m_RButton_SizeY, m_hMemdc, 0, 0, 295, 132, RGB(255, 122, 255));
		}
		else {
			TransparentBlt(m_hBackMemdc, m_Packet->g_waitRoomCharInfo.charPos[0].x + 70, m_Opp_Char_PosY - 80, m_RButton_SizeX, m_RButton_SizeY, m_hMemdc, 0, 0, 295, 132, RGB(255, 122, 255));
		}
		
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pPlayer[i]->Draw(m_hBackMemdc, m_hMemdc); // 캐릭터
	}

	BitBlt(m_hDC, 0, 0, rect.right, rect.bottom, m_hBackMemdc, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(m_hBackMemdc, m_hBitmap_Copy));
	DeleteDC(m_hBackMemdc);
	DeleteDC(m_hMemdc);
};

void CWaitRoomScene::Ready(POINT mouse,CClient* cClient)
{
	// 준비버튼을 눌렀다면
	if (m_My_RButton_PosX < mouse.x && m_My_RButton_PosX + m_RButton_SizeX > mouse.x
		&& m_My_RButton_PosY < mouse.y && m_My_RButton_PosY + m_RButton_SizeY > mouse.y)
	{
		m_Packet->g_charSelectedWait = true;
		set_My_Ready(true); // 준비상태로 전환.
		cClient->m_waitThreadInWait.isReady = true;
		
		m_hBitmap_My_ReadyButton = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP_READY_BUTTON_T));	// 버튼도 바꿈.
	}
};

void CWaitRoomScene::FrameAdvance(float elapsedTime)
{
	InvalidateRect(m_hWnd, NULL, FALSE);
	ShowWindow(m_hWnd, TRUE);

	// 이 부분에서 상대편의 준비상태를 받아오도록 하자
	// ...

	
	

	//SetSceneSwap(); // 씬전환 명령 대기..

	DrawScene();
};

void CWaitRoomScene::FrameAdvance(float elapsedTime, WaitThread w)
{
	InvalidateRect(m_hWnd, NULL, FALSE);
	ShowWindow(m_hWnd, TRUE);

	// 이 부분에서 상대편의 준비상태를 받아오도록 하자
	// ...


	SetCharInfo();

	SetSceneSwap(w); // 씬전환 명령 대기..

	DrawScene();
};


void CWaitRoomScene::DestroyObjects()
{
	if (m_pPlayer != NULL)
	{
		for (int i = 0; i < 2; ++i)
		{
			delete m_pPlayer[i];
			m_pPlayer[i] = NULL;
		}
	}
};

void CWaitRoomScene::SetCharInfo()
{
	//cout << "내캐릭터위치: " << m_Packet->g_waitRoomCharInfo.charPos[1].x << " , ";
	//cout << m_Packet->g_waitRoomCharInfo.charPos[1].y << endl;
	if(m_Packet->g_myID==0)
		m_Opp_Ready = m_Packet->g_waitRoomCharInfo.isready[1];
	else if (m_Packet->g_myID == 1)
		m_Opp_Ready = m_Packet->g_waitRoomCharInfo.isready[0];

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (m_Packet->g_waitRoomCharInfo.joinRoom[i] != -1) {
			m_pPlayer[i] = new Object(m_Packet->g_waitRoomCharInfo.charPos[i].x, m_Packet->g_waitRoomCharInfo.charPos[i].y,
				0, 0, 0,
				150, 150,
				30, 29, 32, m_Packet->g_waitRoomCharInfo.myBitMapNum[i], 3, m_hInst);
		}
		//cout << "캐릭터 위치: " << m_Packet->g_waitRoomCharInfo.charPos[i].x << " , " << m_Packet->g_waitRoomCharInfo.charPos[i].y << endl;
		//cout << "캐릭터 사이즈: " << m_Packet->g_waitRoomCharInfo.size[i] << endl;
	}


	//Sleep(500);
}

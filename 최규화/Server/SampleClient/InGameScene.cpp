#include "InGameScene.h"

random_device randomDevice;
default_random_engine dre(randomDevice());
uniform_int_distribution<> ui_randomPosX(140, 600);
uniform_int_distribution<> ui_randomPosY(100, 400);

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

	m_MySelectedChar = m_Packet->g_waitRoomCharInfo.myBitMapNum[m_Packet->g_myID];
	if(m_Packet->g_myID==0)
		m_OppSelectedChar = m_Packet->g_waitRoomCharInfo.myBitMapNum[1];
	else
		m_OppSelectedChar = m_Packet->g_waitRoomCharInfo.myBitMapNum[0];
	GetClientRect(m_hWnd, &rect);
	m_hBitmap_Background = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP_BACKG_ROAD));

	m_pObjectMyPlayer = NULL;
	m_pObjectOppPlayer = NULL;
	m_pObjectItemtemp = NULL;
	for (int i = 0; i < MAXLEFTCAR; ++i)
		m_pObjectLeftLineCars[i] = NULL;

	for (int i = 0; i < MAXRIGHTCAR; ++i)
		m_pObjectRightLineCars[i] = NULL;

	for (int i = 0; i < MAXITEM; ++i)
		m_pObjectItem[i] = NULL;

	for (int i = 0; i < MAXOBSTACLE; ++i)
		m_pObjectObstacle[i] = NULL;
	m_pObjectMyPlayer = new Object(m_Packet->g_charinfo.char_pos.x, m_Packet->g_charinfo.char_pos.y, 0, 0, 1, 70, 70, 122, 124, 121, m_MySelectedChar - 10, 3, m_hInst);
	m_pObjectOppPlayer = new Object(640, 400, 0, 0, 1, 70, 70, 122, 124, 121, m_OppSelectedChar - 10, 3, m_hInst);
	BuildObjects(OBJECT_PLAYER);
	BuildObjects(OBJECT_LEFTCAR);
	BuildObjects(OBJECT_RIGHTCAR);
	BuildObjects(OBJECT_ITEM2);
	BuildObjects(OBJECT_OBSTACLE);
};

void CInGameScene::BuildObjects(int objectType) // 캐릭터 선택에 따른 비트맵 변경을 위해 인자를 추가해주자.
{
	switch(objectType)
	{
	case OBJECT_PLAYER:
		
		

		// 이 비트맵들은 함정에 걸렸을 때 깜빡이는 효과를 주기 위한것..
		m_hBitmap_MyPlayerOrigin = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE( m_MySelectedChar-10));
		m_hBitmap_MyPlayerStuck = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(m_MySelectedChar-10));

		

		m_hBitmap_OppPlayerOrigin = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(m_OppSelectedChar-10));
		m_hBitmap_OppPlayerStuck = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(m_OppSelectedChar-10));
		break;

	case OBJECT_LEFTCAR:
		m_pObjectLeftLineCars[0] = new Object(140, 100, 0, 1, 3, 50, 80, 261, 440, 310, IDB_BITMAP_L_CAR2, 1, m_hInst);
		m_pObjectLeftLineCars[1] = new Object(200, 100, 0, 1, 4, 50, 80, 261, 440, 620, IDB_BITMAP_L_CAR1, 1, m_hInst);
		m_pObjectLeftLineCars[2] = new Object(260, 100, 0, 1, 2, 50, 80, 261, 440, 0, IDB_BITMAP_L_CAR2, 1, m_hInst);
		m_pObjectLeftLineCars[3] = new Object(320, 100, 0, 1, 1, 50, 80, 261, 440, 0, IDB_BITMAP_L_CAR1, 1, m_hInst);
		break;

	case OBJECT_RIGHTCAR:
		m_pObjectRightLineCars[0] = new Object(410, 100, 0, -1, 1, 50, 80, 261, 440, 0, IDB_BITMAP_R_CAR1, 1, m_hInst);
		m_pObjectRightLineCars[1] = new Object(470, 100, 0, -1, 2, 50, 80, 261, 440, 0, IDB_BITMAP_R_CAR2, 1, m_hInst);
		m_pObjectRightLineCars[2] = new Object(530, 100, 0, -1, 2, 50, 80, 261, 440, 0, IDB_BITMAP_R_CAR1, 1, m_hInst);
		m_pObjectRightLineCars[3] = new Object(600, 100, 0, -1, 5, 50, 80, 261, 440, 0, IDB_BITMAP_R_CAR2, 1, m_hInst);
		break;

	case OBJECT_ITEM:
		++m_curItemCount;
		//m_pObjectItem[m_curItemCount] = new Object(ui_randomPosX(dre), ui_randomPosY(dre), 0, 0, 0, 50, 50, 27, 27, 0, IDB_BITMAP_ITEM, 1, m_hInst);
		break;
	case OBJECT_ITEM2:
		m_pObjectItemtemp = new Object(m_Packet->g_iteminfo.item_pos.x, m_Packet->g_iteminfo.item_pos.y, 0, 0, 0, 50, 50, 27, 27, 0, IDB_BITMAP_ITEM, 1, m_hInst);
		break;
	case OBJECT_OBSTACLE:
		//++m_curObstacleCount;
		
		m_pObjectObstacle[0] = new Object(m_Packet->g_obsinfo.item_pos.x, m_Packet->g_obsinfo.item_pos.y, 0, 0, 0, 50, 50, 300, 300, 0, IDB_BITMAP_OBSTACLE, 1, m_hInst);
		m_pObjectObstacle[1] = new Object(m_Packet->g_obsinfo2.item_pos.x, m_Packet->g_obsinfo2.item_pos.y, 0, 0, 0, 50, 50, 300, 300, 0, IDB_BITMAP_OBSTACLE, 1, m_hInst);
		m_pObjectObstacle[2] = new Object(m_Packet->g_obsinfo3.item_pos.x, m_Packet->g_obsinfo3.item_pos.y, 0, 0, 0, 50, 50, 300, 300, 0, IDB_BITMAP_OBSTACLE, 1, m_hInst);

		break;
	}
};

void CInGameScene::UpdateObjects(CarObjectInfo car, CClient* cl, CharacterInfo charinfo, CharacterInfo opcharinfo)
{
	// Car Move
	m_Packet->g_charinfo.char_pos.x = m_pObjectMyPlayer->GetPositionX();
	m_Packet->g_charinfo.char_pos.y = m_pObjectMyPlayer->GetPositionY();
	
	if (m_MoveCoolTime >= MOVE_COOLTIME)
	{
		if (m_pObjectLeftLineCars != NULL)
		{
			for (int i = 0; i < MAXLEFTCAR; ++i)
			{/*
				if (m_pObjectLeftLineCars[i]->GetPositionY() > 400)
					m_pObjectLeftLineCars[i]->SetPositionY(0);
				m_pObjectLeftLineCars[i]->Move();*/
				//성일 수정
				m_pObjectLeftLineCars[i]->SetPositionY(car.posY[i]);
			}
		}
		if (m_pObjectRightLineCars != NULL)
		{
			for (int i = 0; i < MAXRIGHTCAR; ++i)
			{
				/*
				if (m_pObjectRightLineCars[i]->GetPositionY() < 10)
					m_pObjectRightLineCars[i]->SetPositionY(420);
				m_pObjectRightLineCars[i]->Move();*/
				//성일 수정
				m_pObjectRightLineCars[i]->SetPositionY(car.posY[i + 4]);
			}
		}
		//m_pObjectMyPlayer->SetPositionX(charinfo.char_pos.x);
		//m_pObjectMyPlayer->SetPositionY(charinfo.char_pos.y);
		m_pObjectOppPlayer->SetPositionX(opcharinfo.char_pos.x);
		m_pObjectOppPlayer->SetPositionY(opcharinfo.char_pos.y);
		m_pObjectItemtemp->SetPositionX(m_Packet->g_iteminfo.item_pos.x);
		m_pObjectItemtemp->SetPositionY(m_Packet->g_iteminfo.item_pos.y);
		m_pObjectObstacle[0]->SetPositionX(m_Packet->g_obsinfo.item_pos.x);
		m_pObjectObstacle[0]->SetPositionY(m_Packet->g_obsinfo.item_pos.y);
		m_pObjectObstacle[1]->SetPositionX(m_Packet->g_obsinfo2.item_pos.x);
		m_pObjectObstacle[1]->SetPositionY(m_Packet->g_obsinfo2.item_pos.y);
		m_pObjectObstacle[2]->SetPositionX(m_Packet->g_obsinfo3.item_pos.x);
		m_pObjectObstacle[2]->SetPositionY(m_Packet->g_obsinfo3.item_pos.y);

	}
	
	//if (!firstSet) {
		
	//	firstSet = true;
	//}
	//m_Packet->m_charInfo.char_pos.x;
	//m_Packet->m_charInfo.char_pos.y;

	
		

	// MyPlayerBehavior
	if (m_pObjectMyPlayer != NULL && m_pObjectMyPlayer->GetIsAlive())
	{

		//if (m_pObjectMyPlayer->GetHealth() <= 0)
		//{ // 플레이어 죽음 판정..
		//	m_pObjectMyPlayer->SetIsAlive(false);
		//}


		if (m_AnimateCoolTime >= ANIMATE_COOLTIME)
		{
			// 애니메이트
			m_pObjectMyPlayer->Animate();

			// Cars와 충돌체크
			for (int j = 0; j < MAXLEFTCAR; ++j)
			{
				if (m_pObjectMyPlayer->IsCollide(m_pObjectLeftLineCars[j]))
				{
					m_pObjectMyPlayer->Damaged(1);
				}
			}
			for (int j = 0; j < MAXRIGHTCAR; ++j)
			{
				if (m_pObjectMyPlayer->IsCollide(m_pObjectRightLineCars[j]))
				{
					m_pObjectMyPlayer->Damaged(1);
				}
			}

			if (m_pObjectItemtemp != NULL) {
				if (m_pObjectMyPlayer->IsCollide(m_pObjectItemtemp)) {
					PlusMyScore(100);
					m_pObjectItemtemp->SetIsAlive(false);
					m_Packet->g_iteminfo.is_alive = false;

					m_Packet->g_iteminfo.id = m_Packet->g_myID;					// 제거
					//m_pObjectItemtemp = NULL;

					//delete m_pObjectItemtemp;
				}

			}

			// Items와 충돌체크
			if (m_pObjectItem != NULL)
			{
				for (int j = 0; j <= m_curItemCount; ++j)
				{
					/*
					if (m_pObjectMyPlayer->IsCollide(m_pObjectItem[j]))
					{
						// 100점 얻음
						PlusMyScore(100);

						// 제거
						delete m_pObjectItem[j];
						m_pObjectItem[j] = NULL;
						for (int k = j; k <= m_curItemCount; ++k)
							m_pObjectItem[j] = m_pObjectItem[k];
						--m_curItemCount;
						--j;
					}
					*/
				}
			}


			// Obstacles와 충돌체크
			if (m_pObjectObstacle != NULL) {
				for (int j = 0; j < 3; ++j)
				{
					if (m_pObjectMyPlayer->IsCollide(m_pObjectObstacle[j]))
					{
						// 데미지 1 입고 스턱.
						m_pObjectMyPlayer->Damaged(1);
						m_pObjectMyPlayer->SetIsStuck(true);
						if (j == 0) {
							m_Packet->g_obsinfo.is_alive = false;
							m_pObjectObstacle[j]->SetIsAlive(false);
							m_Packet->g_obsinfo.id = m_Packet->g_myID;
						}
						else if (j == 1) {
							m_Packet->g_obsinfo2.is_alive = false;
							m_pObjectObstacle[j]->SetIsAlive(false);
							m_Packet->g_obsinfo2.id = m_Packet->g_myID;
						}
						else {
							m_Packet->g_obsinfo3.is_alive = false;
							m_pObjectObstacle[j]->SetIsAlive(false);
							m_Packet->g_obsinfo3.id = m_Packet->g_myID;
						}

						// 제거
						//delete m_pObjectObstacle[j];
						//m_pObjectObstacle[j] = NULL;
						//for (int k = j; k <= m_curObstacleCount; ++k)
							//m_pObjectObstacle[j] = m_pObjectObstacle[k];
						//--m_curObstacleCount;
						//--j;
					}
				}
			}
		}
		
		// 점프
		if (m_pObjectMyPlayer->GetIsJump())
			m_pObjectMyPlayer->Jump();

		// 점수 체크
		if (m_ScoreCoolTime >= SCORE_COOLTIME)
		{
			if (m_pObjectMyPlayer->GetPositionX() > 100 && m_pObjectMyPlayer->GetPositionX() < 600)
				m_MyScore += 1;
		}
	}




	// OppPlayerBehavior
	if (m_pObjectOppPlayer != NULL)
	{
		if (m_AnimateCoolTime >= ANIMATE_COOLTIME)
		{
			// 애니메이트
			m_pObjectOppPlayer->Animate();

			// 점수 체크
			//if (m_pObjectOppPlayer->GetPositionX() > 100 && m_pObjectOppPlayer->GetPositionX() < 600)
				//m_OppScore += 1;
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

	// Items
	if (m_Packet->g_iteminfo.is_alive) {
		m_pObjectItemtemp->Draw(m_hBackMemdc, m_hMemdc);

	}
	//for (int i = 0; i <= m_curItemCount; ++i)
	//	m_pObjectItem[i]->Draw(m_hBackMemdc, m_hMemdc);
	// Obstacles

	
	if(m_Packet->g_obsinfo.is_alive)
		m_pObjectObstacle[0]->Draw(m_hBackMemdc, m_hMemdc);
	if (m_Packet->g_obsinfo2.is_alive)
		m_pObjectObstacle[1]->Draw(m_hBackMemdc, m_hMemdc);
	if (m_Packet->g_obsinfo3.is_alive)
		m_pObjectObstacle[2]->Draw(m_hBackMemdc, m_hMemdc);

	// Cars
	for (int i = 0; i < MAXLEFTCAR; ++i)
		m_pObjectLeftLineCars[i]->Draw(m_hBackMemdc, m_hMemdc);
	for (int i = 0; i < MAXRIGHTCAR; ++i)
		m_pObjectRightLineCars[i]->Draw(m_hBackMemdc, m_hMemdc);

	// Players
	if (m_pObjectMyPlayer->GetIsStuck())
	{
		if ((int)(m_pObjectMyPlayer->GetStuckCoolTime() * 15) % 2 == 0)
			m_pObjectMyPlayer->SetBitmap(m_hBitmap_MyPlayerStuck);
		else m_pObjectMyPlayer->SetBitmap(m_hBitmap_MyPlayerOrigin);
	}

	m_pObjectMyPlayer->Draw(m_hBackMemdc, m_hMemdc);

	
	m_pObjectOppPlayer->Draw(m_hBackMemdc, m_hMemdc);

	


	m_hOldBrush = (HBRUSH)SelectObject(m_hDC, m_hBrush);
	SelectObject(m_hBackMemdc, m_hOldBrush);
	SetBkMode(m_hBackMemdc, TRANSPARENT);
	sprintf_s(m_MyScore_Text, "1P 현재 점수 : %5d 점", m_MyScore);
	sprintf_s(m_OppScore_Text, "2P 현재 점수 : %5d 점", m_OppScore);
	TextOut(m_hBackMemdc, (rect.left + 30), (rect.bottom - 30), m_MyScore_Text, strlen(m_MyScore_Text));
	TextOut(m_hBackMemdc, (rect.right - 200), (rect.bottom - 30), m_OppScore_Text, strlen(m_OppScore_Text));

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
		if (pKeyBuffer[VK_SPACE] & 0xF0) 
		{
			m_pObjectMyPlayer->SetIsJump(true);
		}
		
	}
};

void CInGameScene::FrameAdvance(float elapsedTime,CClient* client, CarObjectInfo car, CharacterInfo charinfo, CharacterInfo opcharinfo)
{
	// 쿨타임 증가
	float coolTime = elapsedTime / 1000.0f;
	m_MoveCoolTime += coolTime;
	m_AnimateCoolTime += coolTime;
	m_ScoreCoolTime += coolTime;
	m_ItemCoolTime += coolTime;
	m_ObstacleCoolTime += coolTime;
	m_pObjectMyPlayer->SetJumpCoolTime(coolTime);


	
	if (!m_pObjectMyPlayer->GetIsStuck())
		GetKeyInput();
	else // 플레이어 함정 발동 시
	{
		m_pObjectMyPlayer->PlusStuckCoolTime(coolTime);
		if (m_pObjectMyPlayer->GetStuckCoolTime() >= 2.0f) // 2초간 정지
		{
			m_pObjectMyPlayer->SetIsStuck(false);
			m_pObjectMyPlayer->SetStuckCoolTime(0.0f);
		}
	}

	

	UpdateObjects(car,client, charinfo, opcharinfo);

	DrawObjects();



	// 쿨타임 초기화
	if (m_AnimateCoolTime >= ANIMATE_COOLTIME)
		m_AnimateCoolTime = 0.0f;
	if (m_MoveCoolTime >= MOVE_COOLTIME)
		m_MoveCoolTime = 0.0f;
	if (m_ScoreCoolTime >= SCORE_COOLTIME)
		m_ScoreCoolTime = 0.0f;
	if (m_ItemCoolTime >= ITEM_COOLTIME)
	{
		m_ItemCoolTime = 0.0f;
		BuildObjects(OBJECT_ITEM2); // 5초마다 아이템 생성
	}
	if (m_ObstacleCoolTime >= OBSTACLE_COOLTIME)
	{
		m_ObstacleCoolTime = 0.0f;
		if (m_curObstacleCount < MAXOBSTACLE-1)
			BuildObjects(OBJECT_OBSTACLE); // 5초마다 장애물 생성
	}

};

void CInGameScene::DestroyObjects() 
{
	if (m_pObjectLeftLineCars != NULL)
	{
		for (int i = 0; i < MAXLEFTCAR; ++i)
		{
			delete m_pObjectLeftLineCars[i];
			m_pObjectLeftLineCars[i] = NULL;
		}
	}

	if (m_pObjectRightLineCars != NULL)
	{
		for (int i = 0; i < MAXRIGHTCAR; ++i)
		{
			delete m_pObjectRightLineCars[i];
			m_pObjectRightLineCars[i] = NULL;
		}
	}

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

	if (m_pObjectItem != NULL)
	{
		for (int i = 0; i < MAXITEM; ++i)
		{
			delete m_pObjectItem[i];
			m_pObjectItem[i] = NULL;
		}
	}
	if (m_pObjectItemtemp != NULL) {
		delete m_pObjectItemtemp;
		m_pObjectItemtemp = NULL;
	}

	if (m_pObjectObstacle != NULL)
	{
		for (int i = 0; i < MAXOBSTACLE; ++i)
		{
			delete m_pObjectObstacle[i];
			m_pObjectObstacle[i] = NULL;
		}
	}
};
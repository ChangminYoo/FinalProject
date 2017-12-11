#include "CharSelectScene.h"



CCharSelectScene::CCharSelectScene()
{
}


CCharSelectScene::~CCharSelectScene()
{
}

void CCharSelectScene::Initializing(HWND hwnd, HINSTANCE inst)
{
	SethWnd(hwnd);
	SethInst(inst);
	GetClientRect(m_hWnd, &rect);
	m_hBitmap_Background = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP_BACKGROUND_CHARSELECT));

	for (int i = 0; i < MAXCHAR; ++i)
	{
		m_pChar[i] = new Object(130 + i * 180, 370, 0, 0, 0, 150, 150, 30, 29, 32, IDB_BITMAP_CHAR_1_SEL + i, 3, m_hInst);
		m_hBitmap_Selected_Char[i] = (HBITMAP)LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP_GRAY_1_SEL + i));
	}
		

};

void CCharSelectScene::UpdateScene()
{
	if (m_AnimateCoolTime >= ANIMATE_COOLTIME)
	{
		for (int i = 0; i < MAXCHAR; ++i)
		{
			if (i != m_MySC && i != m_OppSC) // ���� ���� ���Ѱ͸� �����δ�.
				m_pChar[i]->Animate();
		}
	}
	InvalidateRect(m_hWnd, NULL, FALSE);
	ShowWindow(m_hWnd, TRUE);
};

void CCharSelectScene::DrawScene()
{
	m_hDC = BeginPaint(m_hWnd, &m_paintstruct);
	m_hBackMemdc = CreateCompatibleDC(m_hDC);
	m_hMemdc = CreateCompatibleDC(m_hDC);
	m_hBitmap_Back = CreateCompatibleBitmap(m_hDC, rect.right, rect.bottom); //��ȭ�� �غ�!
	m_hBitmap_Copy = (HBITMAP)SelectObject(m_hBackMemdc, m_hBitmap_Back); //��ȭ�� ����

	SelectObject(m_hMemdc, m_hBitmap_Background); // ���
	StretchBlt(m_hBackMemdc, 0, 0, rect.right, rect.bottom, m_hMemdc, 0, 0, 800, 600, SRCCOPY);
	
	for (int i = 0; i < MAXCHAR; ++i)
	{
		if (i == m_MySC || i == m_OppSC) // ������ � ĳ���͸� �����ߴٸ�
		{
			m_pChar[i]->SetBitmap(m_hBitmap_Selected_Char[i]);
		}
		m_pChar[i]->Draw(m_hBackMemdc, m_hMemdc); // �� ĳ���͸� ����ó���� �̹����� �׸���.
	}

	BitBlt(m_hDC, 0, 0, rect.right, rect.bottom, m_hBackMemdc, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(m_hBackMemdc, m_hBitmap_Copy));
	DeleteDC(m_hBackMemdc);
	DeleteDC(m_hMemdc);
};

void CCharSelectScene::Select(POINT mouse, CClient* Client)
{
	if (!m_IsMySelected) // ���� ���� ĳ���� ������ ������ ��
	{
		for (int i = 0; i < MAXCHAR; ++i)
		{
			// � ĳ������ ������ Ŭ��������
			if (m_pChar[i]->GetPositionX() < mouse.x && m_pChar[i]->GetPositionX() + m_pChar[i]->GetBitSize().first > mouse.x
				&& m_pChar[i]->GetPositionY() < mouse.y && m_pChar[i]->GetPositionY() + m_pChar[i]->GetBitSize().second > mouse.y)
			{
				// ������ �̹� ������ ĳ���Ͱ� �ƴ϶��
				if (i != m_OppSC)
				{
					m_MySC = i; // ���� ������ ĳ���Ͱ� �������� �����ϰ�
					//m_Packet->m_selectedInChar = i;
					m_IsMySelected = true; // ���� ĳ���� ������ �ߴٰ� �˸���.
					m_pChar[i]->SetCharID(i);
					m_Packet->g_selected= i;
					//RotateObjects(Client);
					//SetEvent(m_Packet->g_handle);
					m_Packet->g_charSelected = false;

				}
				
			}
		}
	}
};

void CCharSelectScene::FrameAdvance(float elapsedTime)
{
	m_AnimateCoolTime += elapsedTime / 1000.0f;
	UpdateScene();
	DrawScene();
	SetSceneSwap(); // ���� ��밡 ��� ĳ���� ������ �Ϸ��ߴٸ� ����ȯ ����.
	if (m_AnimateCoolTime >= ANIMATE_COOLTIME)
		m_AnimateCoolTime = 0.0f;
};


void CCharSelectScene::DestroyObjects()
{
	if (m_pChar != NULL)
	{
		for (int i = 0; i < MAXCHAR; ++i)
		{
			delete m_pChar[i];
			m_pChar[i] = NULL;
		}
	}
};

void CCharSelectScene::RotateObjects(CClient *Client)
{
	for (int i = 0; i < MAXCHAR; ++i)
	{
		if (m_pChar[i]->GetCharID() != -1)
		{
			Client->SetPlayerInfo(m_pChar[i]->GetCharID(), 150, 150, 1, 20, true, true);
			cout << Client->GetPlayerInfo() << endl;
			break;
		}
	}
}
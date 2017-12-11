#pragma once
#include "stdafx.h"
#include "Object.h"
#include "CPacketInfo.h"
#include "CharSelectScene.h"
//#include "InGameScene.h"

class CCharSelectScene;

class CClient : public CPacketInfo
{
private:
	int m_CurScene;
	SOCKET m_ClientSock;
	Object *m_Player = nullptr;
	CCharSelectScene *m_SelectScene = nullptr;
	vector<HANDLE> m_handleVec;
	//�߰� -R
	//CInGameScene *m_GameScene = nullptr;
protected:
	CPacketInfo *m_Packet = nullptr;
	
public:
	CClient();
	WaitRoomWhoIsSelected CClient::g_waitRoomSel = { { -1, -1 },{ false, false },{ IDB_BITMAP_READY_BUTTON_F, IDB_BITMAP_READY_BUTTON_F },{ IDB_BITMAP_READY_DISPLAY,IDB_BITMAP_READY_DISPLAY } };
	static bool IsReady;
	static DWORD WINAPI ProcessClient(LPVOID arg);
    //static DWORD WINAPI ProcessMainServer(LPVOID arg);

	void mainClientThread();

	void err_quit(char *msg);
	void err_display(char *msg);
	int recvn(SOCKET s, char *buf, int len, int flags);

	//���� ���� ���°� ��������
	void SetCurScene(int scene);
	int GetCurScene() const;

	void SetPlayerInfo(int id, int pos_x, int pos_y, int speed, int hp, bool alive, bool ready);
	Object* GetPlayerInfo();

	void MovePlayerInfo(int direction);//ĳ���͸� ������ �� ���� �޾ƿ��� �Լ�
	~CClient();
};


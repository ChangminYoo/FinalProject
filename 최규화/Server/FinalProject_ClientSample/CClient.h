#pragma once
#include "Object.h"
#include "CPacketInfo.h"
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

protected:
	CPacketInfo *m_Packet = nullptr;
	
public:
	CClient();
	static bool IsReady;
	static DWORD WINAPI ProcessClient(LPVOID arg);

	void mainClientThread();

	void err_quit(char *msg);
	void err_display(char *msg);
	int recvn(SOCKET s, char *buf, int len, int flags);

	//현재 씬의 상태가 무엇인지
	void SetCurScene(int scene);
	int GetCurScene() const;

	void SetPlayerInfo(int id, int pos_x, int pos_y, int speed, int hp, bool alive, bool ready);
	Object* GetPlayerInfo();

	void MovePlayerInfo(int direction);//캐릭터를 움직일 때 값을 받아오는 함수
	~CClient();
};


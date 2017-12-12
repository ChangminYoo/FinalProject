#pragma once
#include "stdafx.h"
class CPacketInfo 
{
public:
	PackInfo m_packInfo;
	CharacterInfo m_charInfo;
	CharacterInfo m_opcharInfo;

	Wait_Room m_waitInfo;
	Character_Select m_selInfo;
	WaitThread m_waitThread{ true };
	WaitThread m_waitThreadInWait{ false };
	GameStateInfo m_gameinfo{ STAGE_NORMAL_STATE };
	SceneState m_sceneState{ 0 };
	SceneChange m_sceneChange{ 0 };
	WhoIsSelected m_whoIsSelected{ -1 };
	WaitRoomCharInfo m_waitRoomCharInfo;
	ItemObjectInfo m_iteminfo;
	ObstacleObjectInfo m_obsinfo;
	ObstacleObjectInfo m_obsinfo2;
	ObstacleObjectInfo m_obsinfo3;

	int m_selectedInChar;
	//추가1
	WaitRoomWhoIsSelected m_waitRoomSel;
	CarObjectInfo m_carinfo;
	bool m_StopClient{ false };
	bool m_waitSelected;
public:
	static int g_myID;
	CPacketInfo();
	static int g_selectedInChar;
	static bool g_charSelected;
	static bool g_charSelectedWait;
	static bool g_swapScene;
	
	static int g_selected;
	static GameStateInfo g_gameinfo;
	static ItemObjectInfo g_iteminfo;
	static WaitRoomCharInfo g_waitRoomCharInfo;
	static CharacterInfo g_charinfo;
	static CharacterInfo g_opcharinfo;
	void WaitRoomCharInfoInit();
	void WaitRoomCharInfoInit(int id, Pos charPos, int charSize, Pos buttonPos, int buttonSize, int BitNum, int join);
	static ObstacleObjectInfo g_obsinfo;
	static ObstacleObjectInfo g_obsinfo2;
	static ObstacleObjectInfo g_obsinfo3;

	void WhoIsSelectedCount(int count) { m_whoIsSelected.selected = count; }

	void SceneStateInit(int scene) { m_sceneState.sceneState = scene; }

	void SceneChangeCountIncrease(int count) { m_sceneChange.changeCount += count; }
	//void SceneChangeCountIncrease(int count) { m_sceneState.sceneState += count; }

	void PacketInfoInit(PACK_TYPE type);

	void CharPacketInfoInit(long id);
	void WaitRoomWhoIsSelInit();
	void WaitRoomWhoIsSelInit(int id[], bool flag[], int logo[], int button[]);
	void WaitThreadPacketInit() { m_waitThread.isReady = true; m_waitThreadInWait.isReady = false; }

	void CharPacketInfoInit(Pos pos, int id, int life, int score);

	void WaitRoomPacketInfoInit(Pos pos, int resource, int id, bool ready, bool start);
	void GameScenePacketInfoInit();
	//ㅑ 귀결 ㅏ 증명 가능
	void SelectPacketInfoInit(long id, bool start);
	short GetCarPos(int i) { return m_carinfo.posY[i]; }
	~CPacketInfo();
};


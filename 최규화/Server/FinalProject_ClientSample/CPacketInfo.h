#pragma once
#include "stdafx.h"
class CPacketInfo 
{
public:
	CharacterInfo m_charInfo;
	CharacterInfo m_opcharInfo;
	CharID m_myID;
	//Ãß°¡1

public:
	//static int g_myID;
	CPacketInfo();
	
	static CharacterInfo g_charinfo;
	static CharacterInfo g_opcharinfo;
	static int g_myID;

	//void SceneChangeCountIncrease(int count) { m_sceneState.sceneState += count; }
	void CharPacketInfoInit(long id);
	void CharPacketInfoInit(Pos pos, int id, int life, int score);
	
	~CPacketInfo();
};


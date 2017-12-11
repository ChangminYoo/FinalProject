#pragma once
#pragma once
#include <windows.h>
#include <math.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <mmsystem.h>
#include <iostream>
#include <map>
#include <vector>
#include <random>

using namespace std;


#define MAX_PLAYER 2

struct Pos
{
	int x;
	int y;
};
//괄호 방식 func\n{} 지켜줄 것 

//캐릭터 정보 구조체

enum PACK_TYPE
{
	SELECT_ROOM,
	WAIT_ROOM,
	STAGE_TIME,
	CHARACTER_INFO,
	OBSTACLE_CAR,
	OBSTACLE_LASER,
	ITEM_CLICK,
	GAME_OVER,
	PACKET_INFO,
	MAIN_STAGE
};

enum GAME_TYPE
{
	STAGE_NORMAL_STATE,
	STAGE_ITEM_STATE,
	STAGE_TRAP_STATE,
	STAGE_ITEMTRAP_STATE
};
#pragma pack(1)
struct CharacterInfo
{

	PACK_TYPE packType = PACK_TYPE::CHARACTER_INFO;
	
	Pos char_pos;
	int score;
	int id;
	int life;
	bool isjump;
};
struct GameStateInfo
{
	GAME_TYPE type;

};

//자동차 장애물 정보 구조체

struct CarObjectInfo
{

	
	short posY[8];

};

struct ItemObjectInfo
{

	PACK_TYPE packType ;
	
		Pos item_pos;
		bool is_alive;
		int id;

};

struct ObstacleObjectInfo
{

	PACK_TYPE packType;
	//PACK_TYPE::OBSTACLE_LASER;
	
	bool is_alive;
	Pos item_pos;
	int id;
	//float clearTime;

	
};


//시간 정보 구조체

struct TimeInfo
{
	PACK_TYPE packType = PACK_TYPE::STAGE_TIME;
	double game_Play_Time;
};



//대기 방 상태 정보 구조체

struct Wait_Room
{
	PACK_TYPE packType = PACK_TYPE::WAIT_ROOM;
	
		bool game_ready;
		
		Pos char_pos;
		int resource_type;
		u_short id;
		
		bool game_start;
	

};


//캐릭터 선택 정보 구조체
/*
#pragma pack(1)
struct Character_Select
{
	// PacketInfo의 packetType = 7
	PACK_TYPE packType = PACK_TYPE::SELECT_ROOM;
	int character_type;
	bool go_to_wr;
	int character_left;
	u_short id;
};
#pragma pack()
*/

struct Character_Select
{
	// PacketInfo의 packetType = 7
	
	bool go_to_wr;
	
	long id;
};

struct WaitRoomCharInfo
{
	int id[MAX_PLAYER];
	Pos charPos[MAX_PLAYER];
	int size[MAX_PLAYER];
	Pos buttonPos[MAX_PLAYER];
	int buttonSize[MAX_PLAYER];
	int myBitMapNum[MAX_PLAYER];
	int joinRoom[MAX_PLAYER];
	bool isready[MAX_PLAYER];
};

struct WaitRoomIsReady
{
	int readyCnt{ 0 };
};

struct WaitRoomWhoIsSelected
{
	int id[MAX_PLAYER];
	bool selected[MAX_PLAYER];
	int logoBitmap[MAX_PLAYER];
	int buttonDownBitmap[MAX_PLAYER];
};

//승패 여부 판단 구조체

struct Game_Info
{
	PACK_TYPE packType = PACK_TYPE::GAME_OVER;
	int score;
	u_short id;
	bool will_replay;
};
#pragma pack()

class PacketClass {
public:
	CharacterInfo packet_Chr[2];
	CarObjectInfo packet_Car;
	ItemObjectInfo packet_Item;
	ObstacleObjectInfo packet_Obs;
	TimeInfo packet_Time;
	Wait_Room packet_Wr[2];
	Character_Select packet_Cs[2];
	Game_Info packet_GI;

	void InitializePacket() {
		packet_Chr[0].packType = PACK_TYPE::CHARACTER_INFO;
		packet_Chr[1].packType = PACK_TYPE::CHARACTER_INFO;
		packet_Chr[0].char_pos.x = 60;
		packet_Chr[1].char_pos.x = 640;
		packet_Chr[0].char_pos.y = 400;
		packet_Chr[1].char_pos.y = 400;
		packet_Chr[0].score = 0;
		packet_Chr[1].score = 0;
		packet_Chr[0].id = 0;
		packet_Chr[1].id = 0;
		packet_Chr[0].life = 10;
		packet_Chr[1].life = 10;
		//packet_Car.packType = PACK_TYPE::OBSTACLE_CAR;
		for (int i = 0; i < 8; ++i)
			packet_Car.posY[i] = 100;
		packet_Item.packType = PACK_TYPE::ITEM_CLICK;
		packet_Item.item_pos.x = 0;
		packet_Item.item_pos.y = 0;
		packet_Item.is_alive = false;
		packet_Item.id = 0;

		for (int i = 0; i<2; ++i) {
			packet_Wr[i].packType = PACK_TYPE::WAIT_ROOM;
			packet_Wr[i].game_ready = false;
			packet_Wr[i].game_start = false;
			packet_Wr[i].char_pos.x = 0;
			packet_Wr[i].char_pos.y = 0;
			packet_Wr[i].id = 0;
			packet_Wr[i].resource_type = 0;
			//packet_Cs[i].packType = PACK_TYPE::SELECT_ROOM;
			//packet_Cs[i].character_type = 0;
			packet_Cs[i].go_to_wr = false;
			//packet_Cs[i].character_left = 0;
			packet_Cs[i].id = 0;
		}


		packet_GI.packType = PACK_TYPE::GAME_OVER;
		packet_GI.score = 0;
		packet_GI.id = 0;
		packet_GI.will_replay = false;

	}
	void SetCharcterInfo(int num, int x, int y, int tempid, u_short i, int d) {
		if (num < 2) {


		}
	}
	void SetC_SInfo(int c, u_short ids) {
		if (ids-1 < 2) {
//			packet_Cs[ids-1].character_type = c;
//			if (c == 1)
//				packet_Cs[ids-1].character_left = 2;
	//		else if(c==2)
//				packet_Cs[ids-1].character_left = 1;

		}
	}
	void SetCharInfo(u_short ids, CharacterInfo chinfo) {
		if (ids < 3) {
			packet_Chr[ids - 1].score = chinfo.score;
			packet_Chr[ids - 1].char_pos.x = chinfo.char_pos.x;
			packet_Chr[ids - 1].char_pos.y = chinfo.char_pos.y;
			packet_Chr[ids - 1].life = chinfo.life;


		}

	}

};


//int carposx[8] = {140,200,260,320,410,470,530,600};
/*
bool IsCollide(Pos a, CarObjectInfo b) {
	
	for (int i = 0; i < 8; ++i)
	{

		bool col1 = (a.x + 35 > carposx[i] - 25);
		bool col2 = (a.x - 35 < carposx[i] + 25);
		bool col3 = (a.y + 35 > b.posY[i] - 40);
		bool col4 = (a.y - 35 < b.posY[i] + 40);
		if ((col1 && col2) && (col3 && col4))
		{
			cout << "충돌" << endl;
			return true;
		}

		else
			return false;
	}
	return false;
	
	//bool col5 = m_isAlive && other.GetIsAlive() && !m_isJump;

	
};

*/
#pragma once
#pragma warning(disable : 4996)
//����2.h�� ������.h �浹 �ذ���.
#ifndef _WINSOCK2API_
#include <WINSOCK2.H>
#include <windows.h>
#endif
#include <math.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <iostream>
#include <map>
#include <vector>

#include <random>
#include "resource1.h"
using namespace std;

#define OBJECT_PLAYER 0
#define OBJECT_LEFTCAR 1
#define OBJECT_RIGHTCAR 2
#define OBJECT_ITEM 3
#define OBJECT_ITEM2 33
#define OBJECT_OBSTACLE 4
#define JUMP_COOLTIME 0.4f
#define CHAR_SELECT_SCENE 0
#define WAIT_ROOM_SCENE 1
#define IN_GAME_SCENE 2

#define WINDOWSIZE_WIDTH 800
#define WINDOWSIZE_HEIGHT 600

#define MAX_PLAYER 2

#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"

struct Pos
{
	int x;
	int y;
};
//��ȣ ��� func\n{} ������ �� 

//ĳ���� ���� ����ü

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
struct WaitThread
{
	bool isReady;
};

struct PackInfo
{
	int size;
	PACK_TYPE packet;
};

struct GameStateInfo
{
	GAME_TYPE type;

};

struct SceneState
{
	int sceneState;
};

struct SceneChange
{
	int changeCount;
};

struct WhoIsSelected
{
	int selected;
};
struct WaitRoomWhoIsSelected
{
	int id[MAX_PLAYER];
	bool selected[MAX_PLAYER];
	int logoBitmap[MAX_PLAYER];
	int buttonDownBitmap[MAX_PLAYER];
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
struct CharacterInfo
{
	PACK_TYPE packType = PACK_TYPE::CHARACTER_INFO;

	Pos char_pos;
	int score;
	int id;
	int life;
	bool isjump;
};


//�ڵ��� ��ֹ� ���� ����ü

struct CarObjectInfo
{

	short posY[8];
};

struct ItemObjectInfo
{
	PACK_TYPE packType;
	Pos item_pos;
	bool is_alive;
	int id;
};

struct ObstacleObjectInfo
{
	PACK_TYPE packType;
	bool is_alive;
	Pos item_pos;
	int id;
	//float clearTime;
};


//�ð� ���� ����ü

struct TimeInfo
{
	PACK_TYPE packType = PACK_TYPE::STAGE_TIME;
	double game_Play_Time;
};



//��� �� ���� ���� ����ü

struct Wait_Room
{
	PACK_TYPE packType = PACK_TYPE::WAIT_ROOM;
	bool game_ready;
	Pos char_pos;
	int resource_type;
	int id;
	bool game_start;
};


//ĳ���� ���� ���� ����ü

struct Character_Select
{
	// PacketInfo�� packetType = 7
	//PACK_TYPE packType = PACK_TYPE::SELECT_ROOM;
	bool go_to_wr;
	int id;
};



//���� ���� �Ǵ� ����ü

struct Game_Info
{
	PACK_TYPE packType = PACK_TYPE::GAME_OVER;
	int score;
	int id;
	bool will_replay;
};
#pragma pack()

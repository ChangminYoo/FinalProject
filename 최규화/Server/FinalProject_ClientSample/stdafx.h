#pragma once
#include <windows.h>
#include <math.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <iostream>
#include <random>

#include "resource1.h"
using namespace std;

#define JUMP_COOLTIME 0.5f
#define DAMAGE_COOLTIME 0.1f

#define WINDOWSIZE_WIDTH 800
#define WINDOWSIZE_HEIGHT 600

#define OBJECT_PLAYER 0
#define OBJECT_LEFTCAR 1
#define OBJECT_RIGHTCAR 2
#define OBJECT_ITEM 3
#define OBJECT_OBSTACLE 4
#define OBJECT_ITEM2 33

//씬 타입
#define CHAR_SELECT_SCENE 0
#define WAIT_ROOM_SCENE 1
#define IN_GAME_SCENE 2

#define CHAR_POSITION_MIN_X 60
#define CHAR_POSITION_MAX_X 640
#define CHAR_POSITION_MIN_Y 0
#define CHAR_POSITION_MAX_Y 440

// 아이템 랜덤 위치 범위
#define ITEM_POSITION_MIN_X 140
#define ITEM_POSITION_MAX_X 600
#define ITEM_POSITION_MIN_Y 100
#define ITEM_POSITION_MAX_Y 400



#define MAX_COUNT 10
#define EXCUTE_COUNT 1
#define UNSELECTED -1
#define MAX_PLAYER 2

// ======== 오브젝트 생성시 필요한 인자 ===========

// ========CHARSELECT SCENE=========
#define CHARSEL_CHAR_POS_X_INIT 130
#define CHARSEL_CHAR_OFFSET_X_INIT 180
#define CHARSEL_CHAR_POS_Y_INIT 370
#define CHARSEL_CHAR_ANI_SIZE_X_INIT 32

// ========WAITROOM SCENE===========
#define WAITROOM_CHAR_SIZE_INIT 150
#define WAITROOM_CHAR_SRC_SIZE_X_INIT 30
#define WAITROOM_CHAR_SRC_SIZE_Y_INIT 29

// ========INGAME SCENE===========
// 캐릭터 속성 정보
#define CHAR_DIR_X_INIT 0
#define CHAR_DIR_Y_INIT 0
#define CHAR_SPEED_INIT 1
#define CHAR_SIZE_X_INIT 70
#define CHAR_SIZE_Y_INIT 70
#define CHAR_SRC_SIZE_X_INIT 122
#define CHAR_SRC_SIZE_Y_INIT 124
#define CHAR_SRC_ANI_SIZE_X_INIT 121
#define CHAR_MAX_ANI_COUNT_INIT 3

// 상대캐릭터 위치
#define OPP_POS_X_INIT 640
#define OPP_POS_Y_INIT 400


// ==============================================

// TransparentRGB
#define TRANSPARENT_R 255
#define TRANSPARENT_G 122
#define TRANSPARENT_B 255

// 게임오버 화면 속성
#define CHAR_SPEED 5
#define INIT_LEFT_CHAR_POSX 60
#define INIT_RIGHT_CHAR_POSX 640
#define INIT_CHAR_POSY 400
#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"

struct Pos
{
	int x;
	int y;
};

#pragma pack(1)
struct CharacterInfo
{
	Pos char_pos;
	int id;
};

struct CharID
{
	int id;
};
#pragma pack()

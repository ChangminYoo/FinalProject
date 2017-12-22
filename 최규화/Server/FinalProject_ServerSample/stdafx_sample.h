#pragma once
#include <windows.h>
#include <math.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <mmsystem.h>
#include <iostream>

using namespace std;

#define MAX_PLAYER 2

#define INIT_LEFT_CHAR_POSX 60
#define INIT_RIGHT_CHAR_POSX 640
#define INIT_CHAR_POSY 400

#define MAX_COUNT 10
#define EXCUTE_COUNT 1

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



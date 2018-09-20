#pragma once
//#pragma comment(lib, "d3dx12")
//#pragma comment(lib, "d3d12")

#include <iostream>
#include <unordered_map>
#include <unordered_set>

// boost asio 관련 헤더
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <mutex>
#include <queue>
#include <chrono>
#include "protocol.h"

//const char SERVERIP[] = "192.168.41.29";

const char SERVERIP[] = "127.0.0.1";
//const char SERVERIP[] = "192.168.22.106";
const unsigned short PORT = 31400;

//----------------------------맵 2층 오브젝트 수----------------------------//
#define MAX_SECOND_FLOOR_NUM 1
#define MAX_SECOND_SMALLWALL_NUM 8
#define MAX_SECOND_BOX_NUM 4

//----------------------------맵 1층 오브젝트 수----------------------------//
#define MAX_FIRST_MOVEOBJECT_NUM 4
#define MAX_FIRST_BOX_NUM (35 + 50)
#define MAX_FIRST_COLUMN_NUM 4
#define MAX_FIRST_BROKENCART_NUM 1
#define MAX_FIRST_BIGWALL_NUM 5
#define MAX_FIRST_SMALLWALL_NUM 15
#define MAX_FIRST_BULLDING_NUM 5
#define MAX_RIGIDBODY_NUM 12

#define TOTAL_SECOND_FLOOR_STATIC_OBJECT (MAX_SECOND_FLOOR_NUM + MAX_SECOND_SMALL_WALL_NUM + MAX_SECOND_BOX_NUM);
#define TOTAL_FIRST_FLOOR_STATIC_OBEJCT (MAX_FIRST_BOX_NUM + MAX_FIRST_COLUMN_NUM + MAX_FIRST_BROKENCART_NUM + MAX_FIRST_BIGWALL_NUM + MAX_FIRST_SMALLWALL_NUM + MAX_FIRST_BULLDING_NUM);
#define TOTAL_STATIC_OBJECT (TOTAL_SECOND_FLOOR_STATIC_OBJECT + TOTAL_FIRST_FLOOR_STATIC_OBEJCT);

#define TOTAL_RIGIDBODY_OBJECT (MAX_RIGIDBODY_NUM);
#define TOTAL_MOVE_OBJECT (MAX_FIRST_MOVEOBJECT_NUM);

#define GS_START	1//시작상태
#define GS_LOAD		2//로딩상태
#define GS_PLAY		3//플레이 상태
#define GS_END		4//엔딩 상태

using namespace std;
using namespace chrono;

#include "SkillInformation.h"
#include "PhysicsEngine\MyMiniPysicsEngine.h"
#include "GameObject\CMonoObject.h"
#include "CNpcObject.h"
#include "FSM\FSM.h"
#include "GameObject\CStaticObject.h"
#include "CPLAYEROBJECT.H"
#include "GameObject\CBulletObject.h"
#include "Timer\CTimerWorker.h"
#include "CAccpetPlayer.h"
#include "CPhysicEngineWorker.h"

extern CTimerWorker							g_timer_queue;
extern CPhysicEngineWorker					g_physics_worker;

extern vector<CPlayerObject*>				g_clients;
extern vector<CNpcObject*>				    g_npcs;

extern list<CBulletObject*>					g_bullets;
extern CBulletObject					   *g_bullobj;

extern unordered_set<CStaticObject*>	    g_staticobjs;
extern unordered_set<CRigidCubeObject*>     g_rigidobjs;
extern unordered_set<CMoveCubeObject*>      g_moveobjs;

extern CStaticObject					   *g_staticobj;

extern FSM								   *g_fsm;
extern CNpcObject					       *g_npcobj;

extern queue<char>					        g_pdir_queue;
//io_service는 IO와 통신을 위해서사용 - 전역 하나만 있어도 된다
extern boost::asio::io_service	            g_io_service;
//boost::asio::io_service						g_io_service;

extern bool									g_start_physics;
extern bool									g_start_oneGame;

static const int MAX_PLAYER_PER_GAME = 5;
//vector<int, int>				      g_RoomData;
//int								  g_currRoomCnt;
//extern bool						  g_start_timer;






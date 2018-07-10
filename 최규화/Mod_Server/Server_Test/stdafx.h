#pragma once
//#pragma comment(lib, "d3dx12")
//#pragma comment(lib, "d3d12")

#include <iostream>
#include <unordered_map>
#include <unordered_set>

// boost asio ���� ���
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <mutex>
#include <queue>
#include <chrono>
#include "protocol.h"

const char SERVERIP[] = "127.0.0.1";
const unsigned short PORT = 31400;

#define MAX_BOX_NUM 54
#define MAX_SMALL_WALL_NUM 15
#define MAX_BIG_WALL_NUM 5
#define MAX_BULLDING_NUM 3
#define MAX_RIGIDBODY_NUM 9
#define MAX_TOTAL_STATIC_OBJECT (MAX_BOX_NUM + MAX_SMALL_WALL_NUM + MAX_BIG_WALL_NUM + MAX_BULLDING_NUM + MAX_RIGIDBODY_NUM)

using namespace std;
using namespace chrono;

#include "PhysicsEngine\MyMiniPysicsEngine.h"
#include "GameObject\CMonoObject.h"
#include "GameObject\CStaticObject.h"
#include "CPLAYEROBJECT.H"
#include "GameObject\CBulletObject.h"
#include "Timer\CTimerWorker.h"
#include "CAccpetPlayer.h"
#include "CPhysicEngineWorker.h"
#include "GameObject/CSkillObject.h"

extern CTimerWorker g_timer_queue;
extern CPhysicEngineWorker g_physics_worker;

extern vector<CPlayerObject*> g_clients;

extern list<CBulletObject*> g_bullets;
extern CBulletObject		*g_bullobj;

extern unordered_set<CStaticObject*> g_staticobjs;
extern unordered_set<RigidCubeObject*> g_rigidobjs;
extern CStaticObject				*g_staticobj;


extern queue<char>					g_pdir_queue;
//io_service�� IO�� ����� ���ؼ���� - ���� �ϳ��� �־ �ȴ�
extern boost::asio::io_service g_io_service;







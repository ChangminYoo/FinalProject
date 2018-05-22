#pragma once
//#pragma comment(lib, "d3dx12")
//#pragma comment(lib, "d3d12")

#include <iostream>
#include <unordered_map>

// boost asio 관련 헤더
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
#define MAX_TOTAL_STATIC_OBJECT (MAX_BOX_NUM + MAX_SMALL_WALL_NUM + MAX_BIG_WALL_NUM + MAX_BULLDING_NUM)

using namespace std;
using namespace chrono;

#include "PhysicsEngine\MyMiniPysicsEngine.h"
#include "GameObject\CMonoObject.h"
#include "GameObject\CStaticObject.h"
#include "CPLAYEROBJECT.H"
#include "GameObject\CBulletObject.h"
#include "Timer\CTimerWorker.h"
#include "CAccpetPlayer.h"

extern CTimerWorker g_timer_queue;

extern vector<CPlayerObject*> g_clients;

extern list<CBulletObject*> g_bullets;
extern CBulletObject		*g_bullobj;

extern unordered_set<CStaticObject*> g_staticobjs;
extern CStaticObject				*g_staticobj;

//io_service는 IO와 통신을 위해서사용 - 전역 하나만 있어도 된다
extern boost::asio::io_service g_io_service;





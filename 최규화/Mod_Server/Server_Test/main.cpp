#include "stdafx.h"

boost::asio::io_service g_io_service;

CTimerWorker			g_timer_queue;
CPhysicEngineWorker     g_physics_worker;

vector<CPlayerObject*>	g_clients;
vector<CNpcObject*>	g_npcs;

list<CBulletObject*>	g_bullets;
CBulletObject			*g_bullobj;

unordered_set<CStaticObject*> g_staticobjs;
unordered_set<RigidCubeObject*> g_rigidobjs;
CStaticObject				 *g_staticobj;

FSM							*g_fsm;
CNpcObject					*g_npcobj;


queue<char>					g_pdir_queue;

int main()
{
	try
	{
		_wsetlocale(LC_ALL, L"korean");
		CAccpetPlayer server;
	}
	catch (const exception& e)
	{
		cout << "Main Thread Error: " << e.what() << endl;
	}
}
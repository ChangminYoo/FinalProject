#include "stdafx.h"

boost::asio::io_service g_io_service;

CTimerWorker			g_timer_queue;
CPhysicEngineWorker     g_physics_worker;

vector<CPlayerObject*>	g_clients;

list<CBulletObject*>	g_bullets;
CBulletObject			*g_bullobj;

unordered_set<CStaticObject*> g_staticobjs;
CStaticObject				 *g_staticobj;

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
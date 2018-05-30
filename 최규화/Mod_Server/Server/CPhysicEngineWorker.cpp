#include "stdafx.h"

CPhysicEngineWorker::CPhysicEngineWorker()
{
	countsPerSec = 3515623;
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

void CPhysicEngineWorker::Update()
{
	// 물리효과와 충돌처리를 계속 루프를 돌면서 cpu를 쉬지 않고 돌려가며 작업을 해야한다
	// 1. GravitySystem() - 중력에 대한 힘 처리
	// 2. Tick() - Integrate 처리 (가속도 및 중력적용)
	// 3. AfterGravity() - 중력 후처리 (y값이 0보다 작으면 그 위로 올려준다)
	// 4. Collision() - 충돌 처리 
	while (true)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_currtime);
		m_deltime = (m_currtime - m_prevtime) * mSecondsPerCount;
		m_prevtime = m_currtime;

		//for (auto client : g_clients)
		//{
		//	client->PlayerInput(client->GetPlayerDirection(), m_deltime);
		//	client->GravitySystem(m_deltime);
		//	client->Tick(m_deltime);
		//	client->AfterGravitySystem(m_deltime);
		//
		//	//client->Collision(m_deltime);
		//
		//	//임시
		//	//client->SetPlayerDirection(0);
		//	//client->SetPlayerAnimation(Ani_State::Idle);
		//}

		for (int i = 0; i < g_clients.size(); ++i)
		{
			//g_clients[i]->m_mtx.lock();
			//g_clients[i]->m_dir
			g_clients[i]->PlayerInput(g_clients[i]->GetPlayerDirection(), RegularPhysicsTime);
			g_clients[i]->GravitySystem(RegularPhysicsTime);
			g_clients[i]->Tick(RegularPhysicsTime);
			g_clients[i]->AfterGravitySystem(RegularPhysicsTime);

			g_clients[i]->SetChangedPlayerState();
			//g_clients[i]->SetPlayerDirection(0);
			//g_clients[i]->m_mtx.unlock();
		}

		for (auto bullet : g_bullets)
		{
			bullet->Tick(RegularPhysicsTime);
			bullet->AfterGravitySystem(RegularPhysicsTime);
		}

	}
}


CPhysicEngineWorker::~CPhysicEngineWorker()
{
}

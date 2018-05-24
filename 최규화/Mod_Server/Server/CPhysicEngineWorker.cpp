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

		for (auto client : g_clients)
		{
			client->GravitySystem(m_deltime);
			client->Tick(m_deltime);
			client->AfterGravitySystem(m_deltime);

			//client->Collision(m_deltime);
		}

	}
}


CPhysicEngineWorker::~CPhysicEngineWorker()
{
}

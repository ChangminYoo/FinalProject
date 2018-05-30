#include "stdafx.h"

CPhysicEngineWorker::CPhysicEngineWorker()
{
	countsPerSec = 3515623;
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

void CPhysicEngineWorker::Update()
{
	// ����ȿ���� �浹ó���� ��� ������ ���鼭 cpu�� ���� �ʰ� �������� �۾��� �ؾ��Ѵ�
	// 1. GravitySystem() - �߷¿� ���� �� ó��
	// 2. Tick() - Integrate ó�� (���ӵ� �� �߷�����)
	// 3. AfterGravity() - �߷� ��ó�� (y���� 0���� ������ �� ���� �÷��ش�)
	// 4. Collision() - �浹 ó�� 
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
		//	//�ӽ�
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

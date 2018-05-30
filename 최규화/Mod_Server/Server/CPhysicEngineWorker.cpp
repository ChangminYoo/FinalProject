#include "stdafx.h"

#include <chrono>
using namespace chrono;

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
		m_currtime = high_resolution_clock::now();
		m_deltime = duration_cast<microseconds>((m_currtime - m_prevtime)).count();
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
			g_clients[i]->PlayerInput(g_clients[i]->GetPlayerDirection(), m_deltime);
			g_clients[i]->GravitySystem(m_deltime);
			g_clients[i]->Tick(m_deltime);
			g_clients[i]->AfterGravitySystem(m_deltime);

			g_clients[i]->SetChangedPlayerState();
			//g_clients[i]->SetPlayerDirection(0);
			//g_clients[i]->m_mtx.unlock();
		}

		for (auto bullet : g_bullets)
		{
			bullet->Tick(m_deltime);
			bullet->AfterGravitySystem(m_deltime);
		}

	}
}


CPhysicEngineWorker::~CPhysicEngineWorker()
{
}

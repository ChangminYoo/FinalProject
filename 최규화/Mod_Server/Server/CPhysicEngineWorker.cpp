#include "stdafx.h"

CPhysicEngineWorker::CPhysicEngineWorker()
{

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
		m_currtime = high_resolution_clock::now();
		m_deltime = duration_cast<microseconds>(m_currtime - m_prevtime).count() / 1000.0f;
		m_prevtime = m_currtime;

		//cout << "Time : " << m_deltime << endl;
		//cout << "Delta time: " << duration_cast<microseconds>(m_currtime - m_prevtime).count() << endl;
		
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
			//cout << "Time : " << m_deltime << endl;
			//if (static_cast<int>(g_clients[i]->GetPlayerDirection()) != 0)
			g_clients[i]->PlayerInput(m_deltime);

			//cout << "PlayerInput Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - m_prevtime).count() / 1000.f << endl;
			g_clients[i]->GravitySystem(m_deltime);

			//cout << "GravitySystem Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - m_prevtime).count() / 1000.f << endl;
			g_clients[i]->Tick(m_deltime);

			//cout << "Tick Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - m_prevtime).count() / 1000.f << endl;
			g_clients[i]->AfterGravitySystem(m_deltime);

			//cout << "AfterGravitySystem Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - m_prevtime).count() / 1000.f << endl;

			g_clients[i]->SetChangedPlayerState();
		}

		for (auto bullet : g_bullets)
		{
			bullet->Tick(m_deltime);
			bullet->AfterGravitySystem(m_deltime);
		}

		CollisionSystem(m_deltime);

		//for (auto client : g_clients)
		//{
		//	client->SetChangedPlayerState();
		//}

	}
}

void CPhysicEngineWorker::CollisionSystem(float deltime)
{
	for (auto client : g_clients)
	{
		client->Collision(&g_clients, deltime);
		client->Collision(&g_staticobjs, deltime);
	}
}


CPhysicEngineWorker::~CPhysicEngineWorker()
{
}

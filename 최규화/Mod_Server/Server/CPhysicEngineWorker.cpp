#include "stdafx.h"

CPhysicEngineWorker::CPhysicEngineWorker()
{

}

//60fps -> 60 frame per second
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
		m_deltime = (float)(duration_cast<microseconds>(m_currtime - m_prevtime).count() / 1000000.0f); // 10�� -6
		m_prevtime = m_currtime;

		//cout << "Time : " << m_deltime << "\n";

		// 0.00000028
		//2838 

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

		if (!g_clients.empty())
		{
			for (int i = 0; i < g_clients.size(); ++i)
			{
				//double t_time = m_deltime / 1000000;

				g_clients[i]->PlayerInput(m_deltime);
				g_clients[i]->GravitySystem(m_deltime);
				g_clients[i]->Tick(m_deltime);
				g_clients[i]->AfterGravitySystem(m_deltime);
				g_clients[i]->SetChangedPlayerState();

				cout << "PosX: " << g_clients[i]->m_pdata.pos.x << "PosY: " << g_clients[i]->m_pdata.pos.y << "PosZ: " << g_clients[i]->m_pdata.pos.z << "\n";
			}

			auto data = g_clients[0]->m_pdata;
			cout << data.pos.y << endl;

		
			/*
			for (auto client : g_clients)
			{
				//cout << "Time : " << m_deltime << endl;
				//if (static_cast<int>(g_clients[i]->GetPlayerDirection()) != 0)
				client->PlayerInput(m_deltime);

				//cout << "PlayerInput Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - m_prevtime).count() / 1000.f << endl;
				client->GravitySystem(m_deltime);

				//cout << "GravitySystem Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - m_prevtime).count() / 1000.f << endl;
				client->Tick(m_deltime);

				//cout << "Tick Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - m_prevtime).count() / 1000.f << endl;
				client->AfterGravitySystem(m_deltime);

				//cout << "AfterGravitySystem Time: " << duration_cast<milliseconds>(high_resolution_clock::now() - m_prevtime).count() / 1000.f << endl;
				client->SetChangedPlayerState();
			}
			*/

		}

		/*
		for (auto bullet : g_bullets)
		{
			if (bullet->GetBulletIsAlive() == true)
			{
				auto temp_time = m_deltime / 1000000;
				bullet->Tick(temp_time);
				bullet->AfterGravitySystem();
			}

			cout << "Bullet ID: " << bullet->GetBulletID() << "Bullet MID: " << bullet->GetBulletMasterID() <<
				"Position: " << bullet->m_bulldata.pos4f.x << ", " << bullet->m_bulldata.pos4f.y << ", " << bullet->m_bulldata.pos4f.z <<
				"LifeTime: " << bullet->GetBulletLifeTime() << endl; 
		
		}
		*/
		

		//CollisionSystem(m_deltime);

		
		// alive �� false�� ������Ʈ�� �����ֱ� 
		
		/*
		for (auto iter = g_bullets.begin(); iter != g_bullets.end();)
		{
			if ((*iter)->GetBulletIsAlive() == false)
			{			
				STC_Attack stc_attack;	
				stc_attack.bull_data.pos4f = (*iter)->m_bulldata.pos4f;
				stc_attack.bull_data.rot4f = (*iter)->m_bulldata.rot4f;
				stc_attack.bull_data.endpoint = (*iter)->m_bulldata.endpoint;
				stc_attack.bull_data.master_id = (*iter)->m_bulldata.master_id;
				stc_attack.bull_data.my_id = (*iter)->m_bulldata.my_id;
				stc_attack.bull_data.type = (*iter)->m_bulldata.type;
				stc_attack.bull_data.alive = (*iter)->m_bulldata.alive;

				iter = g_bullets.erase(iter);

				for (auto client : g_clients)
				{
					client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
				}
			}
			else
			{
				++iter;
			}
		}
		*/
		


	}
}

void CPhysicEngineWorker::CollisionSystem(__int64 deltime)
{

	/*
	for (auto bullet : g_bullets)
	{
		if (bullet->GetBulletIsAlive() == true)
		{
			bullet->Collision(&g_clients, deltime);
			bullet->Collision(&g_staticobjs, deltime);
		}
	}
	*/

	for (auto client : g_clients)
	{
		if (client->GetPlayerIsAlive() == true)
		{
			client->Collision(&g_clients, deltime);
			client->Collision(&g_staticobjs, deltime);
		}
	}

}


CPhysicEngineWorker::~CPhysicEngineWorker()
{
}

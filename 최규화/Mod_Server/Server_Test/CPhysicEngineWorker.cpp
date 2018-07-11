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
		__int64 local_deltime = duration_cast<microseconds>(m_currtime - m_prevtime).count(); // 10�� -6
		m_deltime = local_deltime / 1000000.0;
		m_prevtime = m_currtime;

		// 0.00000028
		//2838 
		//for (auto rigidbody : )
	
		if (!g_clients.empty())
		{
			for (auto client : g_clients)
			{
				client->PlayerInput(m_deltime);
				client->GravitySystem(m_deltime);
				client->Tick(m_deltime);
				client->AfterGravitySystem(m_deltime);
				client->SetChangedPlayerState();

				//cout << "PosY: " << client->m_pdata.pos.y << "\n";
				//cout << "PosX: " << client->m_pdata.pos.x << "PosY: " << client->m_pdata.pos.y << "PosZ: " << client->m_pdata.pos.z << "\n";
			}
		}
	
		
		
		for (auto bullet : g_bullets)
		{
			if (bullet->GetBulletIsAlive() == true)
			{
				bullet->Tick(m_deltime);
				bullet->AfterGravitySystem();
			}

			//cout << "Bullet ID: " << bullet->GetBulletID() << "Bullet MID: " << bullet->GetBulletMasterID() <<
			//	"Position: " << bullet->m_bulldata.pos4f.x << ", " << bullet->m_bulldata.pos4f.y << ", " << bullet->m_bulldata.pos4f.z <<
			//	"LifeTime: " << bullet->GetBulletLifeTime() << endl; 
		   
		}
		
		for (auto rigid : g_rigidobjs)
		{
			rigid->GravitySystem(m_deltime);
			rigid->Tick(m_deltime);
			rigid->AfterGravitySystem(m_deltime);
			rigid->SetUpdatedRigidybodyObject();
		}


		CollisionSystem(m_deltime);

		
		// alive �� false�� ������Ʈ�� �����ֱ� 	
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
	}
}

void CPhysicEngineWorker::CollisionSystem(double deltime)
{

	
	for (auto bullet : g_bullets)
	{
		if (bullet->GetBulletIsAlive() == true)
		{
			bullet->Collision(&g_clients, deltime);
			bullet->Collision(&g_staticobjs, deltime);
		}
	}
	
	

	for (auto client : g_clients)
	{
		if (client->GetPlayerIsAlive() == true)
		{
			client->Collision(&g_clients, deltime);
			client->Collision(&g_staticobjs, deltime);
		}
	}

	for (auto rigid : g_rigidobjs)
	{
		rigid->Collision(&g_rigidobjs, deltime);
		rigid->Collision(&g_staticobjs, deltime);
		rigid->Collision(&g_clients, deltime);
		rigid->Collision(&g_bullets, deltime);
	}
}


CPhysicEngineWorker::~CPhysicEngineWorker()
{
}

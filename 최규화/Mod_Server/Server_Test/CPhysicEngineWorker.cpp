#include "stdafx.h"

CPhysicEngineWorker::CPhysicEngineWorker()
{

}

//60fps -> 60 frame per second
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
		__int64 local_deltime = duration_cast<microseconds>(m_currtime - m_prevtime).count(); // 10의 -6
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

		
		// alive 가 false인 오브젝트들 지워주기 	
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

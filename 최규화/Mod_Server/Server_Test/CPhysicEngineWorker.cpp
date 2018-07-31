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
		
		for (auto npc_monster : g_npcs)
		{
			if (!npc_monster->GetAlive()) continue;

			npc_monster->GravitySystem(m_deltime);
			npc_monster->Tick(m_deltime);
			npc_monster->AfterGravitySystem(m_deltime);

			npc_monster->UpdateLookvector();
			npc_monster->UpdateUpvector();

			npc_monster->SetMyBasicPacketData();
		}

	
		for (auto rigid : g_rigidobjs)
		{
			rigid->GravitySystem(m_deltime);
			//cout << "PosX: " << rigid->GetCenterPos4f().x << "PosY: " << rigid->GetCenterPos4f().y << "PosZ: " << rigid->GetCenterPos4f().z << "PosW: " << rigid->GetCenterPos4f().w << "\n";

			rigid->Tick(m_deltime);
			//cout << "PosX: " << rigid->GetCenterPos4f().x << "PosY: " << rigid->GetCenterPos4f().y << "PosZ: " << rigid->GetCenterPos4f().z << "PosW: " << rigid->GetCenterPos4f().w << "\n";
			
			rigid->AfterGravitySystem(m_deltime);
			//cout << "PosX: " << rigid->GetCenterPos4f().x << "PosY: " << rigid->GetCenterPos4f().y << "PosZ: " << rigid->GetCenterPos4f().z << "PosW: " << rigid->GetCenterPos4f().w << "\n";

			rigid->UpdateLookvector();
			rigid->UpdateUpvector();

			rigid->SetUpdatedRigidybodyObject();
			//cout << "PosX: " << rigid->GetCenterPos4f().x << "PosY: " << rigid->GetCenterPos4f().y << "PosZ: " << rigid->GetCenterPos4f().z << "PosW: " << rigid->GetCenterPos4f().w << "\n";
		}
		
		for (auto bullet : g_bullets)
		{
			if (!bullet->GetBulletIsAlive()) continue;

			bullet->Tick(m_deltime);
			bullet->AfterGravitySystem();

			//cout << "Bullet ID: " << bullet->GetBulletID() << "Bullet MID: " << bullet->GetBulletMasterID() <<
			//	"Position: " << bullet->m_bulldata.pos4f.x << ", " << bullet->m_bulldata.pos4f.y << ", " << bullet->m_bulldata.pos4f.z <<
			//	"LifeTime: " << bullet->GetBulletLifeTime() << endl; 
		   
		}
		
		CollisionSystem(m_deltime);

		// alive 가 false인 오브젝트들 지워주기 	
		for (auto iter = g_bullets.begin(); iter != g_bullets.end();)
		{
			if ((*iter)->GetBulletIsAlive() == false)
			{			
				if ((*iter)->GetObjectType() == protocol_NpcStoneBullet)
				{
					STC_NpcMonsterAttackStoneBullet stc_imp_bullet;
					stc_imp_bullet.npc_bulldata = (*iter)->GetChangedNPCBulletState();
					
					for (auto client : g_clients)
					{
						client->SendPacket(reinterpret_cast<Packet*>(&stc_imp_bullet));
					}
				}
				else
				{
					//서버->클라 불렛 구조체 
					//STC_BulletObject_Info stc_bullet;
					//stc_bullet.pos4f = (*iter)->m_bulldata.pos4f;
					//stc_bullet.rot4f = (*iter)->m_bulldata.rot4f;
					//stc_bullet.endpoint = (*iter)->m_bulldata.endpoint;
					//stc_bullet.master_id = (*iter)->m_bulldata.master_id;
					//stc_bullet.my_id = (*iter)->m_bulldata.my_id;
					//stc_bullet.type = (*iter)->m_bulldata.type;
					//stc_bullet.alive = (*iter)->m_bulldata.alive;
					//stc_bullet.degree = (*iter)->m_bulldata.degree;
					//

					if ((*iter)->m_bulldata.type == protocol_DiceBullet)
					{
						STC_SKILL_DICESTRIKE stc_skill_dicestrike;
						stc_skill_dicestrike.bull_data = move((*iter)->GetChangedBulletState());
						stc_skill_dicestrike.is_first = (*iter)->GetIsFirstBullet();
						stc_skill_dicestrike.lookvector = (*iter)->GetDicestrikeOffLookvector();

						for (auto client : g_clients)
						{
							client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_dicestrike));
						}
					}
					else
					{
						STC_Attack stc_attack;
						stc_attack.bull_data = move((*iter)->GetChangedBulletState());
						stc_attack.is_first = (*iter)->GetIsFirstBullet();

						for (auto client : g_clients)
						{
							client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
						}
					}
				}
				
				iter = g_bullets.erase(iter);
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
	for (auto client : g_clients)
	{
		if (client->GetPlayerIsAlive() == true)
		{
			client->Collision(&g_clients, deltime);
			client->Collision(&g_staticobjs, deltime);
			client->Collision_With_Waveshock();
		}
	}

	for (auto npc_monster : g_npcs)
	{
		if (!npc_monster->GetAlive()) continue;

		npc_monster->Collision(&g_clients, deltime);
		npc_monster->Collision(&g_staticobjs, deltime);
	}

	for (auto rigid : g_rigidobjs)
	{
		rigid->Collision(&g_rigidobjs, deltime);
		rigid->Collision(&g_staticobjs, deltime);
		rigid->Collision(&g_clients, deltime);
		rigid->Collision(&g_bullets, deltime);
	}

	for (auto bullet : g_bullets)
	{
		if (bullet->GetBulletIsAlive() == true)
		{
			bullet->Collision(&g_clients, deltime);
			bullet->Collision(&g_npcs, deltime);
			bullet->Collision(&g_staticobjs, deltime);
		}
	}
	
}


CPhysicEngineWorker::~CPhysicEngineWorker()
{
}

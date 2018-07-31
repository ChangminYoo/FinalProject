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

		// alive �� false�� ������Ʈ�� �����ֱ� 	
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
					//����->Ŭ�� �ҷ� ����ü 
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

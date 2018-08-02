#include "../stdafx.h"

CTimerWorker::CTimerWorker()
{
	
}

void CTimerWorker::TimerThread()
{
	//queue에 담겨진 이벤트들은 시간 순서대로 정렬되어 있는 상태여야함 -> min heap. 각 노드의 원소는 vector에 저장(배열기반)
	while (true)
	{
		Sleep(1);
		t_lock.lock();
		while (t_queue.empty() == false)
		{
			if (t_queue.top()->wakeup_time > GetTickCount())
				break;

			event_type *event_ptr = t_queue.top();

			t_queue.pop();
			t_lock.unlock();

			g_io_service.post([this, event_ptr]()
			{
				ProcessPacket(event_ptr);
				if (event_ptr != nullptr)
				{
					delete event_ptr;
				}
			}
			);
			t_lock.lock();
		}
		t_lock.unlock();
	}
}

void CTimerWorker::AddEvent(const unsigned int& id, const double& sec, TIMER_EVENT_TYPE type, bool is_ai, const unsigned int& master_id)
{
	event_type *event_ptr = new event_type;

	event_ptr->id = id;
	event_ptr->ai = is_ai;
	event_ptr->type = type;
	event_ptr->wakeup_time = (sec * 1000) + GetTickCount();		//sec으로 받아온 시간 *1000 + 현재시간 //ms
	event_ptr->master_id = master_id;

	t_lock.lock();
	t_queue.push(event_ptr);
	t_lock.unlock();
}

void CTimerWorker::ProcessPacket(event_type * et)
{
	switch (et->type)
	{
		case SKILL_SHIELD:
		{
			if (!g_clients[et->master_id]->GetShieldState())
				g_clients[et->master_id]->SetShieldState(true);
			
			if (g_clients[et->master_id]->GetShieldCurrtime() < SKILL_SHIELD_OP_TIME)
			{
				AddEvent(et->id, SKILL_SHIELD_OP_TIME, SKILL_SHIELD, true, et->master_id);
				g_clients[et->master_id]->SetShieldCurrtime(SKILL_SHIELD_OP_TIME);
			}
			else
			{
				g_clients[et->master_id]->SetShieldOnceFlag(true);
				g_clients[et->master_id]->SetShieldState(false);
				g_clients[et->master_id]->SetShieldCurrtime(0.0);


				STC_SKILL_SHIELD stc_skill_shield;
				stc_skill_shield.skill_data.alive = false;
				stc_skill_shield.skill_data.master_id = et->master_id;
				stc_skill_shield.skill_data.my_id = CHAR_SKILL::SHIELD;

				for (auto client : g_clients)
				{
					if (client->GetIsAI() == true || client->GetConnectState() == false) continue;

					client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_shield));
				}

			}

		}
		break;

		case SKILL_WAVESHOCK:
		{
			if (!g_clients[et->master_id]->GetWaveshockState())
				g_clients[et->master_id]->SetWaveshockState(true);

			if (g_clients[et->master_id]->GetWaveshockCurrtime() < SKILL_WAVESHOCK_OP_TIME)
			{
				AddEvent(et->id, SKILL_WAVESHOCK_OP_TIME, SKILL_WAVESHOCK, true, et->master_id);
				g_clients[et->master_id]->SetWaveshockCurrtime(SKILL_WAVESHOCK_OP_TIME);
			}
			else
			{
				g_clients[et->master_id]->SetWaveshockOnceFlag(true);
				g_clients[et->master_id]->SetWaveshockState(false);
				g_clients[et->master_id]->SetWaveshockCurrtime(0.0);

				STC_SKILL_WAVESHOCK stc_skill_waveshock;
				stc_skill_waveshock.skill_data.alive = false;
				stc_skill_waveshock.skill_data.master_id = et->master_id;
				stc_skill_waveshock.skill_data.my_id = CHAR_SKILL::WAVE_SHOCK;

				for (auto client : g_clients)
				{
					if (client->GetIsAI() == true || client->GetConnectState() == false) continue;
					
					client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_waveshock));
				}
			}
		}
		break;

		case DEAD_TO_ALIVE:
		{

		}
		break;

		case CHANGE_PLAYER_STATE:
		{

		}
		break;

		case REGULAR_PACKET_EXCHANGE:
		{
			// 1초에 20번 패킷을 정기적으로 보내줘야함 
			m_currtime = high_resolution_clock::now();
			__int64 local_deltime = duration_cast<microseconds>(m_currtime - m_prevtime).count();
			m_deltime = (local_deltime / 1000000.0);
			m_currtime = m_prevtime;

			//------------- 1초에 18번정도 보냄 - 20번x, 시간누적오차 때문에 ---------------//
			STC_MoveObject stc_move_object;
			for (const auto& mvobj : g_moveobjs)
			{
				stc_move_object.mvobj_data = move(mvobj->GetMoveObjectData());
				for (const auto& client : g_clients)
				{
					if (client->GetIsAI()) continue;
					if (!client->GetConnectState()) continue;
					client->SendPacket(reinterpret_cast<Packet*>(&stc_move_object));
				}
			}


			//0. 1초에 20번씩 서버에서 업데이트된 리즈드바디 오브젝트의 정보를 보냄 
			STC_RigidbodyObject stc_rigidbody_object;
			for (auto client : g_clients)
			{
				for (auto rigid : g_rigidobjs)
				{
					stc_rigidbody_object.rbobj_data = move(rigid->GetRigidbodyData());
					client->SendPacket(reinterpret_cast<Packet*>(&stc_rigidbody_object));

					//cout << "RigidybodyObject ID: " << rigid->m_stc_robjdata.id << "PosX: " << rigid->m_stc_robjdata.pos4f.x << "PosY: "
					//	<< rigid->m_stc_robjdata.pos4f.y << "PosZ: " << rigid->m_stc_robjdata.pos4f.z << "PosW: " << rigid->m_stc_robjdata.pos4f.w << "\n";
				}
			}


			//1. 1초에 20번씩 서버에서 업데이트된 클라이언트의 모든정보를 보냄
			STC_CharCurrState stc_char_state;
			for(auto myclient : g_clients)
			{
				if (myclient->GetIsAI() == true || myclient->GetConnectState() == false) continue;
				
				for (auto otherclient : g_clients)
				{
					stc_char_state.player_data = move(otherclient->m_pdata);
					myclient->SendPacket(reinterpret_cast<Packet*>(&stc_char_state));
				}
				
			}

			//2. 1초에 20번씩 서버에서 업데이트된 Monster NPC의 모든정보를 클라이언트로 보냄
			STC_NpcMonsterCurrState stc_mnpcs_state;
			for (auto npc_monster : g_npcs)
			{
				if (!npc_monster->GetMyBasicPacketData().connect) continue;
				if (!npc_monster->GetAlive()) continue;

				for (auto client : g_clients)
				{
					stc_mnpcs_state.npc_data = move(npc_monster->GetMyBasicPacketData());
					client->SendPacket(reinterpret_cast<Packet*>(&stc_mnpcs_state));
				}
			}


			//3. 불렛 데이터도 1초에 20번씩 클라이언트로 보냄 
			for (auto bullet : g_bullets)
			{
				//cout << "Bullet ID: " << bullet->GetBulletID() << "Bullet MID: " << bullet->GetBulletMasterID() <<
				//	"Position: " << bullet->m_bulldata.pos4f.x << ", " << bullet->m_bulldata.pos4f.y << ", " << bullet->m_bulldata.pos4f.z <<
				//	"LifeTime: " << bullet->GetBulletLifeTime() <<" " << "IsAlive:" << static_cast<int>(bullet->GetBulletIsAlive()) << endl;

				if (bullet->GetBulletIsAlive() == true)
				{
					//서버->클라 불렛 구조체 
					/*
					STC_BulletObject_Info stc_bullet;
					stc_bullet.pos4f = bullet->m_bulldata.pos4f;
					stc_bullet.rot4f = bullet->m_bulldata.rot4f;
					stc_bullet.endpoint = bullet->m_bulldata.endpoint;
					stc_bullet.master_id = bullet->m_bulldata.master_id;
					stc_bullet.my_id = bullet->m_bulldata.my_id;
					stc_bullet.type = bullet->m_bulldata.type;
					stc_bullet.alive = bullet->m_bulldata.alive;
					stc_bullet.degree = bullet->m_bulldata.degree;
					*/
					//

					if (bullet->GetObjectType() == protocol_NpcStoneBullet)
					{
						STC_NpcMonsterAttackStoneBullet stc_imp_bullet;
						stc_imp_bullet.npc_bulldata = bullet->GetChangedNPCBulletState();

						for (auto client : g_clients)
						{
							client->SendPacket(reinterpret_cast<Packet*>(&stc_imp_bullet));
						}
					}
					else
					{
						if (bullet->m_bulldata.type == protocol_DiceBullet && bullet->GetIsFirstBullet())
						{
							STC_SKILL_DICESTRIKE stc_skill_dicestrike;
							stc_skill_dicestrike.bull_data = move(bullet->GetChangedBulletState());
							stc_skill_dicestrike.is_first = bullet->GetIsFirstBullet();
							stc_skill_dicestrike.lookvector = bullet->GetDicestrikeOffLookvector();

							for (auto client : g_clients)
							{
								client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_dicestrike));
							}
						}
						else
						{
							STC_Attack stc_attack;
							stc_attack.bull_data = move(bullet->GetChangedBulletState());
							stc_attack.is_first = bullet->GetIsFirstBullet();

							for (auto client : g_clients)
							{
								client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
							}

						}

						if (bullet->GetIsFirstBullet())
							bullet->SetIsFirstBullet(false);
					}

				}
			}			

			AddEvent(0, RegularPacketExchangeTime , REGULAR_PACKET_EXCHANGE, true, 0);	
		}
		break;
	}

}



CTimerWorker::~CTimerWorker()
{
}

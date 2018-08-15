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
				
			}
		}
		break;

		case PLAYER_RESPAWN:
		{
			//캐릭터 m_alive = false가 되어 죽으면 5초뒤에 리스폰 상태를 다루는 이곳에 진입한다.
			auto my_id = et->master_id;

			g_clients[my_id]->SetCenterPos4f(g_clients[my_id]->GetOriginCenterPos4f());
			g_clients[my_id]->xmf4_pos = g_clients[my_id]->GetOriginCenterPos4f();
			g_clients[my_id]->UpdatePhysicsCenterPos();

			g_clients[my_id]->GetPhysicsPoint()->SetVelocity(0.f, 0.f, 0.f);
			g_clients[my_id]->SetMyAbility({ 100, 100, 10, 0, 1, 100.0 });
			g_clients[my_id]->SetMyAnimation(Ani_State::Idle);
			g_clients[my_id]->SetAlive(true);
			
			g_clients[my_id]->m_respawn_state = false;
			
			if (CPlayerObject::m_topRankerID == my_id)
			{
				g_clients[my_id]->GetPhysicsPoint()->SetHalfBox(4, 13, 4);			
				g_clients[my_id]->SetMyOriginalHP(700);								

				g_clients[my_id]->SetMyCurrSpeed(g_clients[my_id]->GetMyCurrSpeed() + 50);
				g_clients[my_id]->SetMyCurrHP(g_clients[my_id]->GetMyCurrHP() + 600);
			}
			else
			{
				g_clients[my_id]->GetPhysicsPoint()->SetHalfBox(3, 10, 3);
			}
					
			
			g_clients[my_id]->UpdateDataForPacket();
			
			STC_RESPAWN_DRAW stc_draw_state;
			stc_draw_state.stc_draw_state.drawobj = true;
			stc_draw_state.stc_draw_state.my_id = my_id;
			stc_draw_state.stc_draw_state.isTopRanker = g_clients[my_id]->GetIsTopRanker();

			//cout << "Respawn ID: " << static_cast<int>(my_id) << "\n";
			for (auto& client : g_clients)
			{ 
				//cout << "ID: " << client->GetID() << "  //  " << "Score: " << client->GetPlayerScore() << "Kill / Death: " << client->GetMyCharacterKillCount() << " , " << client->GetMyCharacterDeathCount() << "\n";
				//cout << "ID: " << client->GetID() << "RankScore: " << client->GetMyScoreForRanking() << "\n";
				//cout << "ID: " << client->GetID() << "IsTopRanker: " << static_cast<bool>(client->GetIsTopRanker()) << " , " << "Current Top Ranker ID: " << CPlayerObject::m_topRankerID << "\n";

				if (!client->GetConnectState()) continue;
				client->SendPacket(reinterpret_cast<Packet*>(&stc_draw_state));
			}
				
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

			//0. MoveObject 
			for (const auto& mvobj : g_moveobjs)
			{
				//if (!mvobj->GetIsCreateFirst())
				//{
					STC_MoveObject_NoCreate stc_move_object_no_create;
					stc_move_object_no_create.mvobj_data.id = mvobj->GetMoveObjectData().id;
					stc_move_object_no_create.mvobj_data.pos4f = mvobj->GetMoveObjectData().pos4f;
					stc_move_object_no_create.mvobj_data.rot4f = mvobj->GetMoveObjectData().rot4f;

					for (const auto& client : g_clients)
					{
						if (!client->GetConnectState()) continue;

						client->SendPacket(reinterpret_cast<Packet*>(&stc_move_object_no_create));
					}
				//}
										
			}

			//1. RigidbodyObject
			STC_RigidbodyObject stc_rigidbody_object;
			for (const auto& client : g_clients)
			{
				//클라이언트가 접속이 끊겼으면 정보를 보내지 않는다 (리즈드 바디 오브젝트)
				if (!client->GetConnectState()) continue;

				for (const auto& rigid : g_rigidobjs)
				{
					stc_rigidbody_object.rbobj_data = move(rigid->GetRigidbodyData());
					client->SendPacket(reinterpret_cast<Packet*>(&stc_rigidbody_object));

					//cout << "RigidybodyObject ID: " << rigid->m_stc_robjdata.id << "PosX: " << rigid->m_stc_robjdata.pos4f.x << "PosY: "
					//	<< rigid->m_stc_robjdata.pos4f.y << "PosZ: " << rigid->m_stc_robjdata.pos4f.z << "PosW: " << rigid->m_stc_robjdata.pos4f.w << "\n";
				}
			}


			//* All Clients 에 대한 Rank 올림차순으로 소트
			int local_rank = 0;
			vector<CPlayerObject*> temp_clients;
			temp_clients.reserve(g_clients.size());
			copy(g_clients.begin(), g_clients.end(), back_inserter(temp_clients));

			sort(temp_clients.begin(), temp_clients.end(), [&](CPlayerObject* c1, CPlayerObject* c2) {return c1->GetMyScoreForRanking() > c2->GetMyScoreForRanking(); });
			for (auto& temp_client : temp_clients)
				temp_client->SetMyCharacterCurrRank(++local_rank);


			int  topRankerID = -1;
			bool once_flag = false;
			for (auto& client : g_clients)
			{
				client->SetIsTopRanker(false);
				client->m_pdata.topRank = false;
				
				for (auto& temp_client : temp_clients)
				{
					if (client->GetID() == temp_client->GetID())
					{
						client->SetMyCharacterCurrRank(temp_client->GetMyCharacterCurrRank());
				
						//이 클라이언트의 등수가 1등이면 toprank를 true로, 단 가장 먼저 선언된 애가 1등, 점수는 30점이상일 때
						if (client->GetMyCharacterCurrRank() == 1 && client->GetPlayerScore() >= 10 && once_flag == false)
						{
							once_flag = true;
							//여기에는 1등만 들어옴, 그것도 첫번째 1등
							CPlayerObject::m_topRankerID = client->GetID();

							client->SetIsTopRanker(true);
							client->m_pdata.topRank = true;

							if (!client->GetIsBuffState())
							{
								client->SetIsBuffState(true);
								client->GetPhysicsPoint()->SetHalfBox(4, 13, 4);			//초기한번생성, 그 뒤로 안바뀜
								client->SetMyOriginalHP(700);								//초기한번생성, 그 뒤로 안바뀜

								client->SetMyCurrSpeed(client->GetMyCurrSpeed() + 50);		//초기한번생성, 그 뒤로 안바뀜
								client->SetMyCurrHP(client->GetMyCurrHP() + 600);			//초기한번생성, 그 뒤로 안바뀜

							}
						
						}

						client->m_pdata.rank = client->GetMyCharacterCurrRank();
						break;
					}
				}
			}

			//2. All Clients (Player)  // 캐릭터에게 스테이지 시간정보 및 리스폰 정보를 보냄
			STC_STAGE_TIMER   stc_stage_timer;
			STC_CharCurrState stc_char_state;
			for (const auto& my_client : g_clients)
			{
				//클라이언트가 접속이 끊겼으면 정보를 보내지 않는다(MOVE_OBJECT, CLIENT)
				if (!my_client->GetConnectState()) continue;
				if (CPlayerObject::m_topRankerID != my_client->GetID() && my_client->GetIsBuffState() == true)
					my_client->SetIsBuffState(false);

				//----------------------------------Character Respawn Data---------------------------------------//
		
				if (my_client->GetAlive() == false && my_client->m_respawn_state == false)
				{
					my_client->m_respawn_state = true;
					my_client->SetMyAnimation(Ani_State::Dead);

					//server to client packet data
					my_client->m_pdata.ani = Ani_State::Dead;
					my_client->m_pdata.respawn_cnt = 1;				//respawn_cnt 가 1일 때만 실행

					g_timer_queue.AddEvent(et->id, Player_RespawnTime, PLAYER_RESPAWN, false, my_client->GetID());
				}

				//--------------------My Client Data to Other Client (Including My Client data)------------------//

				for (const auto& other_client : g_clients)
				{
					stc_char_state.player_data = move(my_client->m_pdata);
					other_client->SendPacket(reinterpret_cast<Packet*>(&stc_char_state));
				}

				//---------------------------------------------------------------Move Object--------------------//

				if (!my_client->GetFirstMoveObjects())
				{
					for (const auto& mvobj : g_moveobjs)
					{
						STC_MoveObject stc_move_object;
						stc_move_object.mvobj_data = move(mvobj->GetMoveObjectData());
						my_client->SendPacket(reinterpret_cast<Packet*>(&stc_move_object));
					}
				}
				my_client->SetFirstMoveObjects(true);

				//-------------------------------------------------------------------Stage Timer----------------//

				stc_stage_timer.data.stage_time = m_deltime;
				my_client->SendPacket(reinterpret_cast<Packet*>(&stc_stage_timer));

				if (my_client->m_pdata.respawn_cnt == 1)
					my_client->m_pdata.respawn_cnt = 0;

			}

			//3. All NPCs (Non-Player)
			STC_NpcMonsterCurrState stc_mnpcs_state;
			for (const auto& npc_monster : g_npcs)
			{
				for (const auto& client : g_clients)
				{
					//클라이언트가 접속이 끊겼으면 정보를 보내지 않는다(NPC)
					if (!client->GetConnectState()) continue;

					stc_mnpcs_state.npc_data = move(npc_monster->GetMyBasicPacketData());
					client->SendPacket(reinterpret_cast<Packet*>(&stc_mnpcs_state));
				}
			}


			//4. All Bullets (LightBullet // HeavyBullet // DiceBullet)
			for (auto bullet : g_bullets)
			{
				//cout << "Bullet ID: " << bullet->GetBulletID() << "Bullet MID: " << bullet->GetBulletMasterID() <<
				//	"Position: " << bullet->m_bulldata.pos4f.x << ", " << bullet->m_bulldata.pos4f.y << ", " << bullet->m_bulldata.pos4f.z <<
				//	"LifeTime: " << bullet->GetBulletLifeTime() <<" " << "IsAlive:" << static_cast<int>(bullet->GetBulletIsAlive()) << endl;

				if (bullet->GetAlive())
				{
			
					//1. NPC Bullet
					if (bullet->GetObjectType() == protocol_NpcStoneBullet)
					{
						STC_NpcMonsterAttackStoneBullet stc_imp_bullet;
						stc_imp_bullet.npc_bulldata = bullet->GetChangedNPCBulletState();
						
						auto stone_rnpos = dynamic_cast<CStoneBulletObject*>(bullet)->GetOrgPlusPos();
						stc_imp_bullet.stone_rnpos = { stone_rnpos.x, stone_rnpos.y, stone_rnpos.z, stone_rnpos.w };

						/*
						cout << "NPC Bullet ID: " << stc_imp_bullet.npc_bulldata.my_id << ", " << "PosXYZW: " << stc_imp_bullet.npc_bulldata.pos4f.x << ", "
							<< stc_imp_bullet.npc_bulldata.pos4f.y << ", " << stc_imp_bullet.npc_bulldata.pos4f.z << ", " << stc_imp_bullet.npc_bulldata.pos4f.w << "\n";
						*/

						for (const auto& client : g_clients)
						{
							if (!client->GetConnectState()) continue;

							client->SendPacket(reinterpret_cast<Packet*>(&stc_imp_bullet));
						}
					}
					else
					{
						//2. Player Bullet
						if (bullet->GetObjectType() == protocol_HammerBullet)
						{
							STC_SKILL_HAMMERBULLET stc_skill_hammer_bullet;
							stc_skill_hammer_bullet.skill_data = bullet->GetChangedHammerBulletState();

							for (const auto& client : g_clients)
							{
								if (!client->GetConnectState()) continue;

								client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_hammer_bullet));
							}
						}
						else if (bullet->m_bulldata.type == protocol_DiceBullet && bullet->GetIsCreateFirst())
						{
							STC_SKILL_DICESTRIKE stc_skill_dicestrike;
							stc_skill_dicestrike.bull_data = move(bullet->GetChangedBulletState());
							stc_skill_dicestrike.is_first = bullet->GetIsCreateFirst();
							stc_skill_dicestrike.lookvector = bullet->GetDicestrikeOffLookvector();

							for (const auto& client : g_clients)
							{
								if (!client->GetConnectState()) continue;

								client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_dicestrike));
							}
						}
						else
						{
							STC_Attack stc_attack;
							stc_attack.bull_data = move(bullet->GetChangedBulletState());
							stc_attack.is_first = bullet->GetIsCreateFirst();

							for (const auto& client : g_clients)
							{
								if (!client->GetConnectState()) continue;

								client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
							}

						}
					}

					if (bullet->GetIsCreateFirst())
						bullet->SetIsCreateFirst(false);

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

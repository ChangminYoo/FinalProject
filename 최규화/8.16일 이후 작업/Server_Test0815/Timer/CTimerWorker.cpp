#include "../stdafx.h"

/*
CTimerWorker::CTimerWorker()
{
	
}
*/

bool CTimerWorker::m_tt_flag = false;
int CTimerWorker::m_tag_cnt = 0;

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
			stc_draw_state.stc_draw_state.drawobj = 1;
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

		case REGULAR_PACKET_EXCHANGE:
		{

			/*
			if (!m_tt_flag)
			{
				m_tt_flag = true;
				m_pt = high_resolution_clock::now();
				m_ct = m_pt;
			}

			if (m_tag_cnt == 0)
			{
				m_ct = high_resolution_clock::now();
				__int64 lt = duration_cast<microseconds>(m_ct - m_pt).count();
				double dt = lt / 1000000.0;

				m_tag_cnt = 1;
				//cout << "Timerthread Time: " << dt << "\n";
			}
			else
			{
				m_pt = high_resolution_clock::now();
				__int64 lt = duration_cast<microseconds>(m_pt - m_ct).count();
				double dt = lt / 1000000.0;

				m_tag_cnt = 0;
				//cout << "TimerThread Timer: " << dt << "\n";
			}
			*/

			//------------- 1초에 18번정도 보냄 - 20번x, 시간누적오차 때문에 ---------------//
			// 1초에 20번 패킷을 정기적으로 보내줘야함 
			m_currtime = high_resolution_clock::now();
			__int64 local_deltime = duration_cast<microseconds>(m_currtime - m_prevtime).count();
			m_deltime = (local_deltime / 1000000.0);
			m_currtime = m_prevtime;


			STC_STAGE_TIMER   stc_stage_timer;
			stc_stage_timer.data.stage_time = m_deltime;

			for (auto& client : g_clients)
			{
				if (!client->GetConnectState()) continue;

				client->SendPacket(reinterpret_cast<Packet*>(&stc_stage_timer));
			}
		
			//Timerthread에서 정기 패킷송신 작업을 처리하는 것이 아니라 이를 Workerthread로 던져줘야함
			//해당 Timerthread 는 싱글스레드로 할당되어 g_io_service.post를 할 때 strand로 wrapping 하지 않아도된다

			//strand와 lock과 기능은 같으나 strand는 외관적으로 멋있어보일 뿐이다.
			TTW_RegularPlayerData ttw_rpdata;
			auto rpdata = reinterpret_cast<Packet*>(&ttw_rpdata);
			g_io_service.post(m_strand.wrap([=]() { g_clients[0]->ProcessPacket(rpdata); }));

			AddEvent(0, RegularPacketExchangeTime , REGULAR_PACKET_EXCHANGE, true, 0);
		}
		break;
	}

}



CTimerWorker::~CTimerWorker()
{
}

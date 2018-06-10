//#include "CTimerWorker.h"
#include "../stdafx.h"

CTimerWorker::CTimerWorker()
{
	countsPerSec = 3515623;
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

void CTimerWorker::TimerThread()
{
	//queue�� ����� �̺�Ʈ���� �ð� ������� ���ĵǾ� �ִ� ���¿����� -> min heap. �� ����� ���Ҵ� vector�� ����(�迭���)
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

void CTimerWorker::AddEvent(const unsigned short & id, const float & sec, TIMER_EVENT_TYPE type, bool is_ai, const unsigned short & master_id)
{
	event_type *event_ptr = new event_type;

	event_ptr->id = id;
	event_ptr->ai = is_ai;
	event_ptr->type = type;
	event_ptr->wakeup_time = (sec * 1000) + GetTickCount();		//sec���� �޾ƿ� �ð� *1000 + ����ð� //ms
	event_ptr->curr_time = (sec * 1000);
	event_ptr->master_id = master_id;

	t_lock.lock();
	t_queue.push(event_ptr);
	t_lock.unlock();
}

void CTimerWorker::SetRegularCurrTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mRegularCurrTime);
}

void CTimerWorker::SetRegularPrevTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mRegularPrevTime);
}

void CTimerWorker::ProcessPacket(event_type * et)
{
	switch (et->type)
	{
		case DEAD_TO_ALIVE:
		{

		}
		break;

		case CHANGE_PLAYER_STATE:
		{

		}
		break;

		
		/*
		case LIGHT_BULLET:
		{
			for (auto& bull : g_bullets)
			{
				if (bull->GetBulletID() == et->id && bull->GetBulletMasterID() == et->master_id &&
					(bull->GetBulletCurrState() == true))
				{
					__int64 currTime;
					QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
					bull->m_currTime = currTime;

					bull->m_deltaTime = ((bull->m_currTime - bull->m_prevTime) * mSecondsPerCount);
					if (bull->m_deltaTime < 0.0) { bull->m_deltaTime = 0.0; }

					bull->m_prevTime = currTime;
					bull->SetBulletLifeTime(bull->m_deltaTime);

					if (bull->GetBulletLifeTime() >= MAX_LIGHT_BULLET_TIME)
					{
						//�Ѿ��� �����ֱⰡ ������ �� -> m_bulldata.alive = false�� 
						bull->DestroyBullet();
						AddEvent(et->id, 0, REGULAR_PACKET_EXCHANGE, true, et->master_id);
						//cout << "Light end " << "timeL: " << lbul->GetBulletLifeTime() << endl;
						//cout << "Pos: " << xmf4.x << "," << xmf4.y << "," << xmf4.z << "," << xmf4.w << endl;
						break;
					}

					bull->Tick(bull->m_deltaTime);
					bull->AfterGravitySystem(bull->m_deltaTime);
			
					// Ŭ���̾�Ʈ �ۼ��ſ� ��Ŷ�� ���� ���� ������Ʈ
					bull->SetChangedBulletState();

					if ((bull->GetBulletCurrState() == true))
						AddEvent(et->id, 0.025, LIGHT_BULLET, true, et->master_id);
				}
			}
		}
		break;

		case HEAVY_BULLET:
		{
			for (auto& bull : g_bullets)
			{
				if (bull->GetBulletID() == et->id && bull->GetBulletMasterID() == et->master_id &&
					(bull->GetBulletCurrState() == true))
				{
					__int64 currTime;
					QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
					bull->m_currTime = currTime;

					bull->m_deltaTime = ((bull->m_currTime - bull->m_prevTime) * mSecondsPerCount);
					if (bull->m_deltaTime < 0.0) { bull->m_deltaTime = 0.0; }

					bull->m_prevTime = currTime;
					bull->SetBulletLifeTime(bull->m_deltaTime);

					if (bull->GetBulletLifeTime() >= MAX_HEAVY_BULLET_TIME)
					{
						//�Ѿ��� �����ֱⰡ ������ �� -> m_bulldata.alive = false�� 
						bull->DestroyBullet();
						AddEvent(et->id, 0, REGULAR_PACKET_EXCHANGE, true, et->master_id);
						//cout << "Heavy end " << "timeL: " << lbul->GetBulletLifeTime() << endl;
						//cout << "Pos: " << xmf4.x << "," << xmf4.y << "," << xmf4.z << "," << xmf4.w << endl;
						break;
					}

					bull->Tick(bull->m_deltaTime);
					bull->AfterGravitySystem(bull->m_deltaTime);

					// Ŭ���̾�Ʈ �ۼ��ſ� ��Ŷ�� ���� ���� ������Ʈ
					bull->SetChangedBulletState();

					if ((bull->GetBulletCurrState() == true))
						AddEvent(et->id, 0.025, HEAVY_BULLET, true, et->master_id);
				}
			}
		}
		break;
		*/

		case REGULAR_PACKET_EXCHANGE:
		{
			// 1�ʿ� 20�� ��Ŷ�� ���������� ��������� 
			m_deltime += duration_cast<milliseconds>(high_resolution_clock::now() - m_prevtime).count() / 1000.f;
			m_currtime = m_prevtime;

			//1. 1�ʿ� 20���� �������� ������Ʈ�� Ŭ���̾�Ʈ�� ��������� ����
			STC_CharCurrState stc_char_state;
			for(auto client : g_clients)
			{
				if (client->GetIsAI() == true || client->GetConnectState() == false) continue;
				
				for (auto client : g_clients)
				{
					stc_char_state.player_data = move(client->m_pdata);		
					client->SendPacket(reinterpret_cast<Packet*>(&stc_char_state));
				}
				
				//cout << "Timer // ID : " << g_clients[i]->GetID() << "Pos[x, y, z, w]: " << g_clients[i]->m_pdata.pos.x << " , " << g_clients[i]->m_pdata.pos.y << " , " << g_clients[i]->m_pdata.pos.z << " , " << g_clients[i]->m_pdata.pos.w << endl;

				/*
				for (auto i = 0; i < 100000000; ++i)
				{
					int sum = 0;
					 sum += i;
				}

				cout << "ID : " << g_clients[i]->GetID() << "Pos[x, y, z, w]: " << g_clients[i]->m_pdata.pos.x << " , " << g_clients[i]->m_pdata.pos.y << " , " << g_clients[i]->m_pdata.pos.z << " , " << g_clients[i]->m_pdata.pos.w << endl;
				++m_tcnt;

				m_currtime = high_resolution_clock::now();
				if (m_flag)
				{
					m_prevtime = m_currtime;
					m_flag = false;
				}

				m_tdeltime += (duration_cast<milliseconds>(m_currtime - m_prevtime).count()) / 1000.f;
				m_prevtime = m_currtime;

				cout << "ī��Ʈ : " << m_tcnt << "�ð�: " << m_tdeltime << "ī��Ʈ_���: " << o_tcnt << endl;

				if (m_tdeltime >= 1.0f)
				{
					cout << "ī��Ʈ : " << m_tcnt << "�ð�: " << m_tdeltime << endl;
					system("pause");
				}
				*/
			}

			//2. �ҷ� �����͵� 1�ʿ� 20���� Ŭ���̾�Ʈ�� ���� 
			STC_Attack stc_attack;
			for (auto bullet : g_bullets)
			{
				if (bullet->GetBulletIsAlive() == true)
				{
					stc_attack.bull_data.pos4f = bullet->m_bulldata.pos4f;
					stc_attack.bull_data.rot4f = bullet->m_bulldata.rot4f;
					stc_attack.bull_data.endpoint = bullet->m_bulldata.endpoint;
					stc_attack.bull_data.master_id = bullet->m_bulldata.master_id;
					stc_attack.bull_data.my_id = bullet->m_bulldata.my_id;
					stc_attack.bull_data.type = bullet->m_bulldata.type;
					stc_attack.bull_data.alive = bullet->m_bulldata.alive;

					for (auto client : g_clients)
					{
						client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
					}
				}
			}

			AddEvent(0, RegularPacketExchangeTime , REGULAR_PACKET_EXCHANGE, true, 0);

			//cout << "����\n" << endl;

			//if (m_deltime >= 1.0f)
			//{
			//	cout << "����\n" << endl;
			//	m_deltime = 0.f;
			//}
	

			/*
			if (g_clients.size() > 0)
			{
				if (m_deltime >= RegularPacketExchangeTime)
				{
					cout << "Time :" << m_tcnt << "number: " << m_deltime << endl;

					m_tdeltime += m_deltime;
					if (m_tdeltime >= 1)
					{
						cout << "1�ʵ��� ī��Ʈ Ƚ��: " << m_tcnt << endl;
						system("pause");
					}

					m_deltime -= RegularPacketExchangeTime;
					AddEvent(0, 0, REGULAR_PACKET_EXCHANGE, true, 0);

				}
			}
			*/


			/*
			for (auto client : g_clients)
			{
				client->GravitySystem(mRegularDelTime);
				client->GetPhysicsPoint()->GravitySystem(mRegularDelTime, client->GetPhysicsPoint());
				client->Tick(mRegularDelTime);
				XMFLOAT4 xmf4{ client->m_pdata.pos.x,client->m_pdata.pos.y, client->m_pdata.pos.z, client->m_pdata.pos.w };
				client->GetPhysicsPoint()->integrate(mRegularDelTime, &xmf4);
				client->SetPlayerData_Pos(xmf4);
				client->AfterGravitySystem(mRegularDelTime);
				client->GetPhysicsEffect()->AfterGravitySystem(mRegularDelTime, client->GetPhysicsPoint(), OBJECT_TYPE::PLAYER,
					client->m_pdata.pos, client->m_pdata.airbone);

				client->SetChangedPlayerState();

				if (client->GetIsAI() == true || client->GetConnectState() == false) continue;

				stc_char_state.player_data = move(client->m_pdata);

				cout << "ID: " << stc_char_state.player_data.id << " " << "Pos: " << stc_char_state.player_data.pos.x << " "
					<< stc_char_state.player_data.pos.y << " " << stc_char_state.player_data.pos.z << " " << stc_char_state.player_data.pos.w 
					<< "  //  " << "Ani: " << stc_char_state.player_data.ani << endl;


				cout << "Timer ID: " << stc_char_state.player_data.id << " ��ȭ�� ȸ����: " << "[ x, y, z, w ]: "
					<< stc_char_state.player_data.rot.x << ", " << stc_char_state.player_data.rot.y << ", " << stc_char_state.player_data.rot.z << ", " << stc_char_state.player_data.rot.w << endl;

				client->SendPacket(reinterpret_cast<Packet*>(&stc_char_state));

			}
			*/

			/*
			for (auto bull = g_bullets.begin(); bull != g_bullets.end();)
			{
				STC_Attack stc_attack;
				stc_attack.bull_data = move((*bull)->GetBulletInfo());
				stc_attack.lifetime = (*bull)->GetBulletLifeTime();

				for (auto client : g_clients)
				{
					client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
				}

				// if((*bullet)->m_lock.try_lock()) (*bullet)->m_lock.lock();
				//(*bullet)->m_lock.lock();
				if (!(*bull)->GetBulletCurrState())
				{
					//�浹ó�� ������ ������������Ѵ�. �ȱ׷��� ���⼭ ������ �� �浹ó���� �õ��� ������ �� 
					//delete (*bullet);
					//(*bullet)->m_lock.unlock();

					//(*bullet)->m_lock.lock();
					bull = g_bullets.erase(bull);
					//(*bullet)->m_lock.unlock();
				}
				else
				{
					//(*bullet)->m_lock.unlock();
					++bull;
				}

			}*/


			//AddEvent(0, RegularPacketExchangeTime, REGULAR_PACKET_EXCHANGE, true, 0);
		
		}
		break;
	}

}



CTimerWorker::~CTimerWorker()
{
}

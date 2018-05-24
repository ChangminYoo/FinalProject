//#include "CTimerWorker.h"
#include "../stdafx.h"

CTimerWorker::CTimerWorker()
{
	countsPerSec = 3515623;
	mSecondsPerCount = 1.0 / (double)countsPerSec;
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

void CTimerWorker::AddEvent(const unsigned short & id, const float & sec, TIMER_EVENT_TYPE type, bool is_ai, const unsigned short & master_id)
{
	event_type *event_ptr = new event_type;

	event_ptr->id = id;
	event_ptr->ai = is_ai;
	event_ptr->type = type;
	event_ptr->wakeup_time = (sec * 1000) + GetTickCount();		//sec으로 받아온 시간 *1000 + 현재시간 //ms
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
						//총알의 생명주기가 다했을 때 -> m_bulldata.alive = false로 
						bull->DestroyBullet();
						AddEvent(et->id, 0, REGULAR_PACKET_EXCHANGE, true, et->master_id);
						//cout << "Light end " << "timeL: " << lbul->GetBulletLifeTime() << endl;
						//cout << "Pos: " << xmf4.x << "," << xmf4.y << "," << xmf4.z << "," << xmf4.w << endl;
						break;
					}

					bull->Tick(bull->m_deltaTime);
					bull->AfterGravitySystem(bull->m_deltaTime);
			
					// 클라이언트 송수신용 패킷에 담을 정보 업데이트
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
						//총알의 생명주기가 다했을 때 -> m_bulldata.alive = false로 
						bull->DestroyBullet();
						AddEvent(et->id, 0, REGULAR_PACKET_EXCHANGE, true, et->master_id);
						//cout << "Heavy end " << "timeL: " << lbul->GetBulletLifeTime() << endl;
						//cout << "Pos: " << xmf4.x << "," << xmf4.y << "," << xmf4.z << "," << xmf4.w << endl;
						break;
					}

					bull->Tick(bull->m_deltaTime);
					bull->AfterGravitySystem(bull->m_deltaTime);

					// 클라이언트 송수신용 패킷에 담을 정보 업데이트
					bull->SetChangedBulletState();

					if ((bull->GetBulletCurrState() == true))
						AddEvent(et->id, 0.025, HEAVY_BULLET, true, et->master_id);
				}
			}
		}
		break;

		case REGULAR_PACKET_EXCHANGE:
		{
			// 1초에 20번 패킷을 정기적으로 보내줘야함 
			QueryPerformanceCounter((LARGE_INTEGER*)&mRegularCurrTime);
			mRegularDelTime = (mRegularCurrTime - mRegularPrevTime) * mSecondsPerCount;
			mRegularPrevTime = mRegularCurrTime;

			STC_ChangedPos stc_pos;
			for (auto client : g_clients)
			{
				client->GravitySystem(mRegularDelTime);
				//client->GetPhysicsPoint()->GravitySystem(mRegularDelTime, client->GetPhysicsPoint());

				client->Tick(mRegularDelTime);
				//XMFLOAT4 xmf4{ client->m_pdata.pos.x,client->m_pdata.pos.y, client->m_pdata.pos.z, client->m_pdata.pos.w };
				//client->GetPhysicsPoint()->integrate(mRegularDelTime, &xmf4);
				//client->SetPlayerData_Pos(xmf4);
				client->AfterGravitySystem(mRegularDelTime);

				//client->GetPhysicsEffect()->AfterGravitySystem(mRegularDelTime, client->GetPhysicsPoint(), OBJECT_TYPE::PLAYER,
				//	client->m_pdata.pos, client->m_pdata.airbone);

				client->SetChangedPlayerState();

				stc_pos.id = client->m_pdata.id;
				stc_pos.ani_state = client->m_pdata.ani;
				stc_pos.pos = client->m_pdata.pos;

				cout << "ID: " << stc_pos.id << " " << "Pos: " << stc_pos.pos.x << " "
					<< stc_pos.pos.y << " " << stc_pos.pos.z << " " << stc_pos.pos.w << "  //  " << "Ani: " << stc_pos.ani_state << endl;

				client->SendPacket(reinterpret_cast<Packet*>(&stc_pos));
			}

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
					//충돌처리 다음에 삭제시켜줘야한다. 안그러면 여기서 지워진 뒤 충돌처리를 시도해 에러가 남 
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

			}


			AddEvent(0, RegularPacketExchangeTime, REGULAR_PACKET_EXCHANGE, true, 0);
		
		}
		break;
	}

}



CTimerWorker::~CTimerWorker()
{
}

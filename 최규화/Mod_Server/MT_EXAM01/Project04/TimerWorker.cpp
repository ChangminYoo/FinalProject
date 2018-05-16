#include "TimerWorker.h"
#include "Player_Session.h"
#include "BulletObject.h"

TimerWorker::TimerWorker()
{
	countsPerSec = 3515623;
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

void TimerWorker::TimerThread()
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

void TimerWorker::AddEvent(const unsigned short& id, const float& sec, TIMER_EVENT_TYPE type, bool is_ai, const unsigned short& master_id)
{
	event_type *event_ptr = new event_type;

	event_ptr->id = id;
	event_ptr->AI = is_ai;
	event_ptr->type = type;
	event_ptr->wakeup_time = (sec * 1000) + GetTickCount();		//sec으로 받아온 시간 *1000 + 현재시간 //ms
	event_ptr->curr_time = (sec * 1000);
	event_ptr->master_id = master_id;

	t_lock.lock();
	t_queue.push(event_ptr);
	t_lock.unlock();
}

void TimerWorker::ProcessPacket(event_type * et)
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
		//ex) 0.025초 뒤에 변화되야할 값 - 불렛 라이프타임. 불렛 위치. 물리효과 후처리. (충돌 처리)
		
		for (auto& lbul : Player_Session::m_bullobjs)
		{
			if (lbul->GetBulletID() == et->id && lbul->GetBulletMasterID() == et->master_id && 
				(lbul->GetBulletCurrState() == true))
			{
				__int64 currTime;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
				lbul->m_currTime = currTime;

				lbul->m_deltaTime = static_cast<float>((lbul->m_currTime - lbul->m_prevTime) * mSecondsPerCount);
				if (lbul->m_deltaTime < 0.0) { lbul->m_deltaTime = 0.0; }

				lbul->m_prevTime = currTime;
				lbul->SetBulletLifeTime(lbul->m_deltaTime);

				if (lbul->GetBulletLifeTime() >= MAX_LIGHT_BULLET_TIME)
				{
					//총알의 생명주기가 다했을 때 -> m_bulldata.alive = false로 
					lbul->DestroyBullet();
					AddEvent(et->id, 0, REGULAR_PACKET_EXCHANGE, true, et->master_id);
					//cout << "Light end " << "timeL: " << lbul->GetBulletLifeTime() << endl;
					//cout << "Pos: " << xmf4.x << "," << xmf4.y << "," << xmf4.z << "," << xmf4.w << endl;
					break;
				}

				XMFLOAT4 xmf4_rot;
				xmf4_rot.x = lbul->GetBulletInfo().Rotate_status.x;
				xmf4_rot.y = lbul->GetBulletInfo().Rotate_status.y;
				xmf4_rot.z = lbul->GetBulletInfo().Rotate_status.z;
				xmf4_rot.w = lbul->GetBulletInfo().Rotate_status.w;

				XMFLOAT4 xmf_rot = QuaternionMultiply(xmf4_rot, QuaternionRotation(lbul->GetLookvector(), MMPE_PI * lbul->m_deltaTime));

				lbul->SetBulletRotatevalue(xmf_rot);

				XMFLOAT4 xmf4 = { lbul->GetBulletInfo().pos.x, lbul->GetBulletInfo().pos.y,
								  lbul->GetBulletInfo().pos.z, lbul->GetBulletInfo().pos.w };

				//불렛 현재위치 = 0.5 * 가속도(0.5 * accel * time * time) + 속도 * 시간 + 이전 위치
				lbul->GetPhysicsPoint()->integrate(lbul->m_deltaTime, &xmf4);
				lbul->AfterGravitySystem();
				lbul->SetBulletNewPos(xmf4);

				if ((lbul->GetBulletCurrState() == true))
					AddEvent(et->id, 0.025, LIGHT_BULLET, true, et->master_id);
			}
		}
	}
	break;

	case HEAVY_BULLET:
	{
		for (auto& lbul : Player_Session::m_bullobjs)
		{
			if (lbul->GetBulletID() == et->id && lbul->GetBulletMasterID() == et->master_id &&
				(lbul->GetBulletCurrState() == true))
			{
				__int64 currTime;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
				lbul->m_currTime = currTime;

				lbul->m_deltaTime = static_cast<float>((lbul->m_currTime - lbul->m_prevTime) * mSecondsPerCount);
				if (lbul->m_deltaTime < 0.0) { lbul->m_deltaTime = 0.0; }

				lbul->m_prevTime = currTime;
				lbul->SetBulletLifeTime(lbul->m_deltaTime);

				if (lbul->GetBulletLifeTime() >= MAX_LIGHT_BULLET_TIME)
				{
					//총알의 생명주기가 다했을 때 -> m_bulldata.alive = false로 
					lbul->DestroyBullet();
					AddEvent(et->id, 0, REGULAR_PACKET_EXCHANGE, true, et->master_id);

					//cout << "Heavy end " << "timeL: " << lbul->GetBulletLifeTime() << endl;
					//cout << "Pos: " << xmf4.x << "," << xmf4.y << "," << xmf4.z << "," << xmf4.w << endl;
					break;
				}

				XMFLOAT4 xmf4_rot;
				xmf4_rot.x = lbul->GetBulletInfo().Rotate_status.x;
				xmf4_rot.y = lbul->GetBulletInfo().Rotate_status.y;
				xmf4_rot.z = lbul->GetBulletInfo().Rotate_status.z;
				xmf4_rot.w = lbul->GetBulletInfo().Rotate_status.w;

				XMFLOAT4 xmf_rot = QuaternionMultiply(xmf4_rot, QuaternionRotation(lbul->GetLookvector(), MMPE_PI / 6 * lbul->m_deltaTime));

				lbul->SetBulletRotatevalue(xmf_rot);

				XMFLOAT4 xmf4 = { lbul->GetBulletInfo().pos.x, lbul->GetBulletInfo().pos.y,
					lbul->GetBulletInfo().pos.z, lbul->GetBulletInfo().pos.w };

				//불렛 현재위치 = 0.5 * 가속도(0.5 * accel * time * time) + 속도 * 시간 + 이전 위치
				lbul->GetPhysicsPoint()->integrate(lbul->m_deltaTime, &xmf4);
				lbul->AfterGravitySystem();
				lbul->SetBulletNewPos(xmf4);

				//서버에서 사라질 때랑 클라에서 사라질 때 2초차이남. 서버가 2초 느림
				if ((lbul->GetBulletCurrState() == true))
					AddEvent(et->id, 0.025, HEAVY_BULLET, true, et->master_id);
			}
		}
	}
	break;

	case REGULAR_PACKET_EXCHANGE:
	{
		// 1초에 20번 패킷을 정기적으로 보내줘야함 
		for (auto client : Player_Session::m_clients)
		{

		}
		//문제 - m_bullobjs 가 없음 
		//1. 라이트 불렛이 있다면, 이 정보를 정기적으로 클라이언트에 보내줘야함 
		//2. 불렛 삭제관리 -> 불렛의 생명주기가 다하거나, 충돌하거나, y값이 0보다 작을 때 m_bulldata.alive = false로 된 상태를
		//   이곳에서 골라 삭제시킨다


		//Player_Session::m_lock.lock();
		//g_collworker.CollisionLock();


		//삭제될 것들을 지역변수로 설정. 따로 담은 뒤 다음에 몰아서 삭제?
		
		//외부에서 삭제하게 되면 문제가 발생이 될 수 있음.
		//1. 시간이 다했을 때 여기서 삭제 2. 총알이 캐릭터에 맞아 삭제될 때 여기서 삭제
		/*list<BulletObject*> t_bullet = Player_Session::m_bullobjs;

		for (auto bullet = t_bullet.begin(); bullet != t_bullet.end();)
		{
			STC_Attack stc_attack;
			stc_attack.bull_data = move((*bullet)->GetBulletInfo());
			stc_attack.lifetime = (*bullet)->GetBulletLifeTime();

			for (auto client : Player_Session::m_clients)
			{
				client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
			}

			if (!(*bullet)->GetBulletCurrState())
			{
				delete *bullet;
				bullet = t_bullet.erase(bullet);
			}
			else
			{
				++bullet;
			}
		}
		*/

		for (auto bullet = Player_Session::m_bullobjs.begin(); bullet != Player_Session::m_bullobjs.end();)
		{
			STC_Attack stc_attack;
			stc_attack.bull_data = move((*bullet)->GetBulletInfo());
			stc_attack.lifetime = (*bullet)->GetBulletLifeTime();

			for (auto client : Player_Session::m_clients)
			{
				client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
			}

		   // if((*bullet)->m_lock.try_lock()) (*bullet)->m_lock.lock();
			//(*bullet)->m_lock.lock();
			if (!(*bullet)->GetBulletCurrState())
			{
				//충돌처리 다음에 삭제시켜줘야한다. 안그러면 여기서 지워진 뒤 충돌처리를 시도해 에러가 남 
				//delete (*bullet);
				//(*bullet)->m_lock.unlock();

				//(*bullet)->m_lock.lock();
				bullet = Player_Session::m_bullobjs.erase(bullet);
				//(*bullet)->m_lock.unlock();
			}
			else 
			{ 
				//(*bullet)->m_lock.unlock();
				++bullet;
			}
			
		}
		

		AddEvent(0, RegularPacketExchangeTime, REGULAR_PACKET_EXCHANGE, true, 0);
	
	}
	break;

	default:
		break;
	}
}


TimerWorker::~TimerWorker()
{
}

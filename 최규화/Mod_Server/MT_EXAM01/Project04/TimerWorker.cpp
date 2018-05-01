#include "TimerWorker.h"
#include "Player_Session.h"
#include "BulletObject.h"

TimerWorker::TimerWorker()
{

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

void TimerWorker::AddEvent(const unsigned short& id, const float& sec, TIMER_EVENT_TYPE type, bool is_ai)
{
	event_type *event_ptr = new event_type;

	event_ptr->id = id;
	event_ptr->AI = is_ai;
	event_ptr->type = type;
	event_ptr->wakeup_time = (sec * 1000) + GetTickCount();		//sec으로 받아온 시간 *1000 + 현재시간 //ms
	event_ptr->curr_time = (sec * 1000);

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
		auto prevTime = high_resolution_clock::now();
		for (auto lbul : Player_Session::m_bullobjs)
		{
			if (lbul->GetBulletID() == et->id)
			{
				//auto currTime = high_resolution_clock::now();
				//auto durTime = currTime - prevTime;
				//auto milli = duration_cast<milliseconds>(durTime).count();
				//auto sec = duration_cast<seconds>(durTime);
				auto durTime = high_resolution_clock::now() - prevTime;
				auto durTime_ms = duration_cast<milliseconds>(durTime).count();
				float totaltime_sec = (et->curr_time + durTime_ms) / 1000;
				lbul->SetBulletLifeTime(totaltime_sec);

				XMFLOAT4 xmf4 = { lbul->GetBulletInfo().pos.x, lbul->GetBulletInfo().pos.y,
								  lbul->GetBulletInfo().pos.z, lbul->GetBulletInfo().pos.w };

				//불렛 현재위치 = 0.5 * 가속도(0.5 * accel * time * time) + 속도 * 시간 + 이전 위치
				lbul->GetPhysicsPoint()->integrate(totaltime_sec, &xmf4);
				lbul->AfterGravitySystem();

				lbul->SetBulletNewPos(xmf4);

				//서버에서 사라질 때랑 클라에서 사라질 때 2초차이남. 서버가 2초 느림
				if (lbul->GetBulletLifeTime() < MAX_LIGHTBULLET_LIFE_TIME )
					AddEvent(et->id, 0.025, LIGHT_BULLET, true);
				else
				{
					cout << "end " << "timeL: " << lbul->GetBulletLifeTime() << endl;
					cout << "10 Pos: " << xmf4.x << "," << xmf4.y << "," << xmf4.z << "," << xmf4.w << endl;
				}

				break;
			}
		}
	}
	break;

	default:
		break;
	}
}


TimerWorker::~TimerWorker()
{
}

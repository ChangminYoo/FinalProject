#include "TimerWorker.h"
#include "Player_Session.h"
#include "BulletObject.h"

TimerWorker::TimerWorker()
{

}

void TimerWorker::TimerThread()
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

void TimerWorker::AddEvent(const unsigned short& id, const float& sec, TIMER_EVENT_TYPE type, bool is_ai)
{
	event_type *event_ptr = new event_type;

	event_ptr->id = id;
	event_ptr->AI = is_ai;
	event_ptr->type = type;
	event_ptr->wakeup_time = (sec * 1000) + GetTickCount();		//sec���� �޾ƿ� �ð� *1000 + ����ð� //ms
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
		//ex) 0.025�� �ڿ� ��ȭ�Ǿ��� �� - �ҷ� ������Ÿ��. �ҷ� ��ġ. ����ȿ�� ��ó��. (�浹 ó��)
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

				//�ҷ� ������ġ = 0.5 * ���ӵ�(0.5 * accel * time * time) + �ӵ� * �ð� + ���� ��ġ
				lbul->GetPhysicsPoint()->integrate(totaltime_sec, &xmf4);
				lbul->AfterGravitySystem();

				lbul->SetBulletNewPos(xmf4);

				//�������� ����� ���� Ŭ�󿡼� ����� �� 2�����̳�. ������ 2�� ����
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

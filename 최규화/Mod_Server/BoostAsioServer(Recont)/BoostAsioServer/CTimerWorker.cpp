#include "CTimerWorker.h"

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

void CTimerWorker::AddEvent(const unsigned short & id, const double & sec, TIMER_EVENT_TYPE type, bool is_ai, const unsigned short & master_id)
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
		}
		break;
	}

}



CTimerWorker::~CTimerWorker()
{
}

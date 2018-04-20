#include "TimerWorker.h"
#include "Player_Session.h"

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
	event_ptr->wakeup_time = sec * 1000 + GetTickCount();		//sec���� �޾ƿ� �ð� *1000 + ����ð�

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

	case ATTACK_CYCLE:
		{
		
		}
	break;

	default:
		break;
	}
}


TimerWorker::~TimerWorker()
{
}

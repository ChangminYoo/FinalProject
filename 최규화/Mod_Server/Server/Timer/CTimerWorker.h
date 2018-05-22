#pragma once
//#include "../ShareHeader.h"

enum TIMER_EVENT_TYPE
{
	DEAD_TO_ALIVE = 1,
	CHANGE_PLAYER_STATE,
	LIGHT_BULLET,
	HEAVY_BULLET,
	REGULAR_PACKET_EXCHANGE
};

using event_type = struct Event_Type
{
	unsigned short id;
	unsigned short master_id;		//bullet의 경우에만
	double wakeup_time;
	double curr_time;
	char type;
	bool ai{ false };
};

class waketime_cmp
{
public:
	bool operator()(const event_type *lhs_val, const event_type *rhs_val) const
	{
		return (lhs_val->wakeup_time > rhs_val->wakeup_time);
	}
};

class CTimerWorker
{
private:
	mutex t_lock;

	__int64 mRegularPrevTime{ 0 };
	__int64 mRegularCurrTime{ 0 };

	__int64 countsPerSec;
	double mSecondsPerCount;

	double mRegularDelTime;

public:
	CTimerWorker();
	void lock() { t_lock.lock(); }
	void unlock() { t_lock.unlock(); }

	void TimerThread();

	void ProcessPacket(event_type* et);
	void AddEvent(const unsigned short& id, const double& sec, TIMER_EVENT_TYPE type, bool is_ai, const unsigned short& master_id);

	priority_queue<event_type*, vector<event_type*>, waketime_cmp> t_queue;
	~CTimerWorker();
};


#pragma once
#include "stdafx.h"

class Player_Session;
class BulletObject;

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
	float wakeup_time;
	float curr_time;
	char type;
	bool AI{ false };
};

class waketime_cmp
{
public:
	bool operator()(const event_type *lhs_val, const event_type *rhs_val) const
	{
		return (lhs_val->wakeup_time > rhs_val->wakeup_time);
	}
};

class TimerWorker
{
private:
	mutex t_lock;

	__int64 mPrevTime;
	__int64 mCurrTime;

	__int64 countsPerSec;
	double mSecondsPerCount;

public:
	TimerWorker();
	void lock() { t_lock.lock(); }
	void unlock() { t_lock.unlock(); }

	void TimerThread();
	void ProcessPacket(event_type* et);
	void AddEvent(const unsigned short& id, const float& sec, TIMER_EVENT_TYPE type, bool is_ai, const unsigned short& master_id);

	int m_tempcnt{ 0 };

	float m_sumprevtime{ 0.f };
	float m_sumtime{ 0.f };

	priority_queue<event_type*, vector<event_type*>, waketime_cmp> t_queue;
	~TimerWorker();
};


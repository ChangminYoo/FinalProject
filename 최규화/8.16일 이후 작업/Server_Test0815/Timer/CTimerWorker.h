#pragma once
//#include "../ShareHeader.h"

enum TIMER_EVENT_TYPE
{
	DEAD_TO_ALIVE = 1,
	CHANGE_PLAYER_STATE,
	LIGHT_BULLET,
	HEAVY_BULLET,
	REGULAR_PACKET_EXCHANGE,
	SKILL_SHIELD,
	SKILL_WAVESHOCK,
	SKILL_DICESTRIKE,
	PLAYER_RESPAWN
};

using event_type = struct Event_Type
{
	unsigned int id;
	unsigned int master_id;		//bullet의 경우에만
	double wakeup_time;
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
	boost::asio::io_service::strand m_strand;
	mutex t_lock;

	high_resolution_clock::time_point m_currtime, m_prevtime;

	double m_deltime;

	int m_tcnt{ 0 };
	float m_tdeltime;

	bool m_flag{ true };
public:
	CTimerWorker(boost::asio::io_service& io_service) : m_strand(io_service) {};
	void lock() { t_lock.lock(); }
	void unlock() { t_lock.unlock(); }

	void TimerThread();

	void ProcessPacket(event_type* et);
	void AddEvent(const unsigned int& id, const double& sec, TIMER_EVENT_TYPE type, bool is_ai, const unsigned int& master_id);

	priority_queue<event_type*, vector<event_type*>, waketime_cmp> t_queue;

	void CheckCurrTime() { m_currtime = high_resolution_clock::now(); }
	void CheckPrevTime() { m_prevtime = high_resolution_clock::now(); }
	~CTimerWorker();


	high_resolution_clock::time_point m_ct, m_pt;
	static bool m_tt_flag;
	static int m_tag_cnt;
};


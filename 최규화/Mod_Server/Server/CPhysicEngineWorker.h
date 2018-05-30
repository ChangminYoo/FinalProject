#pragma once
class CPhysicEngineWorker
{
private:
	high_resolution_clock::time_point	m_currtime, m_prevtime;

	__int64 countsPerSec;
	double mSecondsPerCount;
	 
	float m_deltime;
public:
	CPhysicEngineWorker();
	void CheckCurrTime() { QueryPerformanceCounter((LARGE_INTEGER*)&m_currtime); }
	void CheckPrevTime() { QueryPerformanceCounter((LARGE_INTEGER*)&m_prevtime); }
	void Update();
	~CPhysicEngineWorker();
};


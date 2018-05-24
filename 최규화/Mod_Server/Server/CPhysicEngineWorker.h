#pragma once
class CPhysicEngineWorker
{
private:
	__int64	m_currtime;
	__int64 m_prevtime;

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


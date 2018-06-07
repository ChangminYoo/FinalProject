#pragma once
class CPhysicEngineWorker
{
private:
	high_resolution_clock::time_point	m_currtime, m_prevtime;

	 
	float m_deltime;

	high_resolution_clock::time_point tt;

public:
	CPhysicEngineWorker();
	void CheckCurrTime() { m_currtime = high_resolution_clock::now(); }
	void CheckPrevTime() { m_prevtime = high_resolution_clock::now(); }
	void Update();
	void CollisionSystem(float deltime);
	~CPhysicEngineWorker();
};


#pragma once
class CPhysicEngineWorker
{
private:
	high_resolution_clock::time_point	m_currtime, m_prevtime;

<<<<<<< HEAD
	double m_deltime;
=======
	long long m_deltime;
>>>>>>> d49c29b133bfb755c81ee4c50a7fda69b21e920e
	

	high_resolution_clock::time_point tt;

public:
	CPhysicEngineWorker();
	void CheckCurrTime() { m_currtime = high_resolution_clock::now(); }
	void CheckPrevTime() { m_prevtime = high_resolution_clock::now(); }
	void Update();
	void CollisionSystem(double deltime);
	~CPhysicEngineWorker();
};


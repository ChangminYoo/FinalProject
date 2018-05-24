#include "stdafx.h"

CPhysicEngineWorker::CPhysicEngineWorker()
{
	countsPerSec = 3515623;
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

void CPhysicEngineWorker::Update()
{
	// ����ȿ���� �浹ó���� ��� ������ ���鼭 cpu�� ���� �ʰ� �������� �۾��� �ؾ��Ѵ�
	// 1. GravitySystem() - �߷¿� ���� �� ó��
	// 2. Tick() - Integrate ó�� (���ӵ� �� �߷�����)
	// 3. AfterGravity() - �߷� ��ó�� (y���� 0���� ������ �� ���� �÷��ش�)
	// 4. Collision() - �浹 ó�� 
	while (true)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_currtime);
		m_deltime = (m_currtime - m_prevtime) * mSecondsPerCount;
		m_prevtime = m_currtime;

		for (auto client : g_clients)
		{
			client->GravitySystem(m_deltime);
			client->Tick(m_deltime);
			client->AfterGravitySystem(m_deltime);

			//client->Collision(m_deltime);
		}

	}
}


CPhysicEngineWorker::~CPhysicEngineWorker()
{
}

#include "TimerWorker.h"
#include "Player_Session.h"
#include "BulletObject.h"

TimerWorker::TimerWorker()
{
	countsPerSec = 3515623;
	mSecondsPerCount = 1.0 / (double)countsPerSec;
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

void TimerWorker::AddEvent(const unsigned short& id, const float& sec, TIMER_EVENT_TYPE type, bool is_ai, const unsigned short& master_id)
{
	event_type *event_ptr = new event_type;

	event_ptr->id = id;
	event_ptr->AI = is_ai;
	event_ptr->type = type;
	event_ptr->wakeup_time = (sec * 1000) + GetTickCount();		//sec���� �޾ƿ� �ð� *1000 + ����ð� //ms
	event_ptr->curr_time = (sec * 1000);
	event_ptr->master_id = master_id;

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

	case LIGHT_BULLET:
	{
		//ex) 0.025�� �ڿ� ��ȭ�Ǿ��� �� - �ҷ� ������Ÿ��. �ҷ� ��ġ. ����ȿ�� ��ó��. (�浹 ó��)
		
		for (auto& lbul : Player_Session::m_bullobjs)
		{
			if (lbul->GetBulletID() == et->id && lbul->GetBulletMasterID() == et->master_id && 
				(lbul->GetBulletCurrState() == true))
			{
				__int64 currTime;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
				lbul->m_currTime = currTime;

				lbul->m_deltaTime = static_cast<float>((lbul->m_currTime - lbul->m_prevTime) * mSecondsPerCount);
				if (lbul->m_deltaTime < 0.0) { lbul->m_deltaTime = 0.0; }

				lbul->m_prevTime = currTime;
				lbul->SetBulletLifeTime(lbul->m_deltaTime);

				if (lbul->GetBulletLifeTime() >= MAX_LIGHT_BULLET_TIME)
				{
					//�Ѿ��� �����ֱⰡ ������ �� -> m_bulldata.alive = false�� 
					lbul->DestroyBullet();
					AddEvent(et->id, 0, REGULAR_PACKET_EXCHANGE, true, et->master_id);
					//cout << "Light end " << "timeL: " << lbul->GetBulletLifeTime() << endl;
					//cout << "Pos: " << xmf4.x << "," << xmf4.y << "," << xmf4.z << "," << xmf4.w << endl;
					break;
				}

				XMFLOAT4 xmf4_rot;
				xmf4_rot.x = lbul->GetBulletInfo().Rotate_status.x;
				xmf4_rot.y = lbul->GetBulletInfo().Rotate_status.y;
				xmf4_rot.z = lbul->GetBulletInfo().Rotate_status.z;
				xmf4_rot.w = lbul->GetBulletInfo().Rotate_status.w;

				XMFLOAT4 xmf_rot = QuaternionMultiply(xmf4_rot, QuaternionRotation(lbul->GetLookvector(), MMPE_PI * lbul->m_deltaTime));

				lbul->SetBulletRotatevalue(xmf_rot);

				XMFLOAT4 xmf4 = { lbul->GetBulletInfo().pos.x, lbul->GetBulletInfo().pos.y,
								  lbul->GetBulletInfo().pos.z, lbul->GetBulletInfo().pos.w };

				//�ҷ� ������ġ = 0.5 * ���ӵ�(0.5 * accel * time * time) + �ӵ� * �ð� + ���� ��ġ
				lbul->GetPhysicsPoint()->integrate(lbul->m_deltaTime, &xmf4);
				lbul->AfterGravitySystem();
				lbul->SetBulletNewPos(xmf4);

				if ((lbul->GetBulletCurrState() == true))
					AddEvent(et->id, 0.025, LIGHT_BULLET, true, et->master_id);
			}
		}
	}
	break;

	case HEAVY_BULLET:
	{
		for (auto& lbul : Player_Session::m_bullobjs)
		{
			if (lbul->GetBulletID() == et->id && lbul->GetBulletMasterID() == et->master_id &&
				(lbul->GetBulletCurrState() == true))
			{
				__int64 currTime;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
				lbul->m_currTime = currTime;

				lbul->m_deltaTime = static_cast<float>((lbul->m_currTime - lbul->m_prevTime) * mSecondsPerCount);
				if (lbul->m_deltaTime < 0.0) { lbul->m_deltaTime = 0.0; }

				lbul->m_prevTime = currTime;
				lbul->SetBulletLifeTime(lbul->m_deltaTime);

				if (lbul->GetBulletLifeTime() >= MAX_LIGHT_BULLET_TIME)
				{
					//�Ѿ��� �����ֱⰡ ������ �� -> m_bulldata.alive = false�� 
					lbul->DestroyBullet();
					AddEvent(et->id, 0, REGULAR_PACKET_EXCHANGE, true, et->master_id);

					//cout << "Heavy end " << "timeL: " << lbul->GetBulletLifeTime() << endl;
					//cout << "Pos: " << xmf4.x << "," << xmf4.y << "," << xmf4.z << "," << xmf4.w << endl;
					break;
				}

				XMFLOAT4 xmf4_rot;
				xmf4_rot.x = lbul->GetBulletInfo().Rotate_status.x;
				xmf4_rot.y = lbul->GetBulletInfo().Rotate_status.y;
				xmf4_rot.z = lbul->GetBulletInfo().Rotate_status.z;
				xmf4_rot.w = lbul->GetBulletInfo().Rotate_status.w;

				XMFLOAT4 xmf_rot = QuaternionMultiply(xmf4_rot, QuaternionRotation(lbul->GetLookvector(), MMPE_PI / 6 * lbul->m_deltaTime));

				lbul->SetBulletRotatevalue(xmf_rot);

				XMFLOAT4 xmf4 = { lbul->GetBulletInfo().pos.x, lbul->GetBulletInfo().pos.y,
					lbul->GetBulletInfo().pos.z, lbul->GetBulletInfo().pos.w };

				//�ҷ� ������ġ = 0.5 * ���ӵ�(0.5 * accel * time * time) + �ӵ� * �ð� + ���� ��ġ
				lbul->GetPhysicsPoint()->integrate(lbul->m_deltaTime, &xmf4);
				lbul->AfterGravitySystem();
				lbul->SetBulletNewPos(xmf4);

				//�������� ����� ���� Ŭ�󿡼� ����� �� 2�����̳�. ������ 2�� ����
				if ((lbul->GetBulletCurrState() == true))
					AddEvent(et->id, 0.025, HEAVY_BULLET, true, et->master_id);
			}
		}
	}
	break;

	case REGULAR_PACKET_EXCHANGE:
	{
		// 1�ʿ� 20�� ��Ŷ�� ���������� ��������� 
		for (auto client : Player_Session::m_clients)
		{

		}
		//���� - m_bullobjs �� ���� 
		//1. ����Ʈ �ҷ��� �ִٸ�, �� ������ ���������� Ŭ���̾�Ʈ�� ��������� 
		//2. �ҷ� �������� -> �ҷ��� �����ֱⰡ ���ϰų�, �浹�ϰų�, y���� 0���� ���� �� m_bulldata.alive = false�� �� ���¸�
		//   �̰����� ��� ������Ų��


		//Player_Session::m_lock.lock();
		//g_collworker.CollisionLock();


		//������ �͵��� ���������� ����. ���� ���� �� ������ ���Ƽ� ����?
		
		//�ܺο��� �����ϰ� �Ǹ� ������ �߻��� �� �� ����.
		//1. �ð��� ������ �� ���⼭ ���� 2. �Ѿ��� ĳ���Ϳ� �¾� ������ �� ���⼭ ����
		/*list<BulletObject*> t_bullet = Player_Session::m_bullobjs;

		for (auto bullet = t_bullet.begin(); bullet != t_bullet.end();)
		{
			STC_Attack stc_attack;
			stc_attack.bull_data = move((*bullet)->GetBulletInfo());
			stc_attack.lifetime = (*bullet)->GetBulletLifeTime();

			for (auto client : Player_Session::m_clients)
			{
				client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
			}

			if (!(*bullet)->GetBulletCurrState())
			{
				delete *bullet;
				bullet = t_bullet.erase(bullet);
			}
			else
			{
				++bullet;
			}
		}
		*/

		for (auto bullet = Player_Session::m_bullobjs.begin(); bullet != Player_Session::m_bullobjs.end();)
		{
			STC_Attack stc_attack;
			stc_attack.bull_data = move((*bullet)->GetBulletInfo());
			stc_attack.lifetime = (*bullet)->GetBulletLifeTime();

			for (auto client : Player_Session::m_clients)
			{
				client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
			}

		   // if((*bullet)->m_lock.try_lock()) (*bullet)->m_lock.lock();
			//(*bullet)->m_lock.lock();
			if (!(*bullet)->GetBulletCurrState())
			{
				//�浹ó�� ������ ������������Ѵ�. �ȱ׷��� ���⼭ ������ �� �浹ó���� �õ��� ������ �� 
				//delete (*bullet);
				//(*bullet)->m_lock.unlock();

				//(*bullet)->m_lock.lock();
				bullet = Player_Session::m_bullobjs.erase(bullet);
				//(*bullet)->m_lock.unlock();
			}
			else 
			{ 
				//(*bullet)->m_lock.unlock();
				++bullet;
			}
			
		}
		

		AddEvent(0, RegularPacketExchangeTime, REGULAR_PACKET_EXCHANGE, true, 0);
	
	}
	break;

	default:
		break;
	}
}


TimerWorker::~TimerWorker()
{
}

#include "CollisionCheck.h"
#include "Player_Session.h"
#include "StaticObject.h"
#include "BulletObject.h"

CollisionCheck::CollisionCheck()
{
}

void CollisionCheck::Update()
{
	auto prevTime = high_resolution_clock::now();
	while (true)
	{
		auto currTime = high_resolution_clock::now();
		auto durTime = currTime - prevTime;
		prevTime = currTime;

		auto deltaTime = duration_cast<milliseconds>(durTime).count() / 1000.0f;

		//for (auto client : Player_Session::m_clients)
		//{
		//	//client->Collision_Players(Player_Session::m_clients, deltaTime);
		//	//client->Collision_StaticObjects(Player_Session::m_sobjs, deltaTime);
		//}
		//for (auto client = Player_Session::m_clients.begin(); client != Player_Session::m_clients.end();)
		//{
			//(*client)->m_lock.lock();
			//Player_Data pdata;
			//pdata.AirBone = true;
			//(*client)->SetPlayerData(pdata);
			//(*client)->m_tempcount += 1;
			//cout << "Ä«¿îÆ®: " << (*client)->m_tempcount << endl;
		//}

		for (auto bullet = Player_Session::m_bullobjs.begin(); bullet != Player_Session::m_bullobjs.end();)
		{
			//(*bullet)->m_lock.lock();
			(*bullet)->Collision_Players(Player_Session::m_clients, deltaTime);
			//bullet->Collision_StaticObjects(Player_Session::m_sobjs, deltaTime);
			//(*bullet)->m_lock.unlock();

			if (Player_Session::m_bullobjs.empty())
				break;
		}


		
		//for (auto bullet : Player_Session::m_bullobjs)
		//{
		//	bullet->m_lock.lock();
		//	bullet->Collision_Players(Player_Session::m_clients, deltaTime);
		//	//bullet->Collision_StaticObjects(Player_Session::m_sobjs, deltaTime);
		//	bullet->m_lock.unlock();
		//
		//	if (Player_Session::m_bullobjs.empty())
		//		break;
		//}
	}
}


CollisionCheck::~CollisionCheck()
{
}

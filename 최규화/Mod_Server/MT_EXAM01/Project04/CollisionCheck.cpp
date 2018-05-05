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

		for (auto client : Player_Session::m_clients)
		{
			client->Collision_Players(Player_Session::m_clients, deltaTime);
			client->Collision_StaticObjects(Player_Session::m_sobjs, deltaTime);
		}

		for (auto bullet : Player_Session::m_bullobjs)
		{
			bullet->Collision_Players(Player_Session::m_clients, deltaTime);
			bullet->Collision_StaticObjects(Player_Session::m_sobjs, deltaTime);
		}
	}
}


CollisionCheck::~CollisionCheck()
{
}

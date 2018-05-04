#include "CollisionCheck.h"

CollisionCheck::CollisionCheck()
{
}

void CollisionCheck::Update()
{

	auto prevTime = high_resolution_clock::now();
	while (true)
	{
		for (auto client : Player_Session::m_clients)
		{
			client->Collision_Players()
		}
	}
}


CollisionCheck::~CollisionCheck()
{
}

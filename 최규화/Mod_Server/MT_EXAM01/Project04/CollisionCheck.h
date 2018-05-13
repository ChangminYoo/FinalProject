#pragma once
#include "stdafx.h"

class Player_Session;
class StaticObject;
class BulletObject;

class CollisionCheck
{
private:
	mutex m_colllock;

public:
	CollisionCheck();
	void Update();

	void CollisionLock() { m_colllock.lock(); }
	void CollisionUnLock() { m_colllock.unlock(); }
	~CollisionCheck();
};


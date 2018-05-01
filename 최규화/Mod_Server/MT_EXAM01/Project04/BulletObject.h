#pragma once
#include "PhysicalEffect.h"
#include "Player_Session.h"
#include "StaticObject.h"

class StaticObject;
class Player_Session;

#define MAX_LIGHTBULLET_LIFE_TIME 10

class BulletObject
{
private:
	float						 lifetime{ 0.f };
								 
	unsigned short				 origin_hp{ 1 };
	unsigned short				 cur_hp{ 1 };
	float						 Damage{ 10 };
	unsigned short				 Speed{ 50 };
	char						 GodMode{ false };
								 
	short						 myID{ -1 };

	BulletObject_Info			 m_bulldata;

	XMFLOAT3					 OffLookvector;
	XMFLOAT3					 OffRightvector;
								 
	XMFLOAT3					 Lookvector;
	XMFLOAT3					 Rightvector;
	XMFLOAT3					 Upvector;

	RigidBody					 *rb{ nullptr };
	PhysicsPoint				 *pp{ nullptr };
	PhysicalEffect				 *pe{ nullptr };

	RayCastObject				  rco;

	bool				          m_delobj{ false };

public:
	float						  prevTime{ 0.f };
	float						  currTime{ 0.f };


public:
	BulletObject(const unsigned short& master_id, const unsigned short& target_id,
				 const Position& pos, const Rotation& rot, float bulltime, const unsigned short& my_id,
				 Vel3f& vel);

	void AfterGravitySystem();

	void Update(float deltatime); //юс╫ц;

	float GetBulletLifeTime() const { return lifetime; }
	void SetBulletLifeTime(float time) { lifetime += time; }

	void Collision_StaticObjects(unordered_set<StaticObject*>& sobjs, float DeltaTime);
	void Collision_Players(vector<Player_Session*>& clients, float DeltaTime);

	BulletObject_Info GetBulletInfo() const { return m_bulldata; }
	short			  GetBulletID() const   { return myID; }
	PhysicsPoint*	  GetPhysicsPoint()     { return pp; }
	void			  SetBulletNewPos(float x, float y, float z, float w) { m_bulldata.pos.x = x; m_bulldata.pos.y = y; m_bulldata.pos.z = z; m_bulldata.pos.w = w; }
	void			  SetBulletNewPos(const XMFLOAT4& xmf4) { m_bulldata.pos.x = xmf4.x; m_bulldata.pos.y = xmf4.y; m_bulldata.pos.z = xmf4.z; m_bulldata.pos.w = xmf4.w; }

	~BulletObject();
};


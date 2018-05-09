#pragma once
#include "PhysicalEffect.h"
#include "Player_Session.h"
#include "StaticObject.h"

class StaticObject;
class Player_Session;

#define MAX_LIGHTBULLET_LIFE_TIME 10
#define MAX_LIGHT_BULLET_TIME 3.5

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

	//bool				          m_delobj{ false };

	unsigned char				  m_type;
	bool						  m_IsFirstCreate{ false };

public:
	__int64					      m_prevTime{ 0 };
	__int64					      m_currTime{ 0 };
	float						  m_deltaTime{ 0.f };
	mutex						  m_lock; 

public:
	BulletObject(const unsigned short& master_id, const unsigned short& my_id,
				 const Position& pos, const Rotation& rot, double bulltime,
				 Vel3f& vel, const unsigned char& type, const Position3D& endpt);

	void AfterGravitySystem();

	void Update(float deltatime); //юс╫ц;

	float GetBulletLifeTime() const { return lifetime; }
	void  SetBulletLifeTime(float time) { lifetime += time; }

	void Collision_StaticObjects(unordered_set<StaticObject*>& sobjs, float DeltaTime);
	void Collision_Players(vector<Player_Session*>& clients, float DeltaTime);

	BulletObject_Info GetBulletInfo() const { return m_bulldata; }
	short			  GetBulletID() const   { return m_bulldata.myID; }
	short			  GetBulletMasterID() const { return m_bulldata.Master_ID; }
	PhysicsPoint*	  GetPhysicsPoint()     { return pp; }
	void			  SetBulletNewPos(float x, float y, float z, float w) { m_bulldata.pos.x = x; m_bulldata.pos.y = y; m_bulldata.pos.z = z; m_bulldata.pos.w = w; }
	void			  SetBulletNewPos(const XMFLOAT4& xmf4) { m_bulldata.pos.x = xmf4.x; m_bulldata.pos.y = xmf4.y; m_bulldata.pos.z = xmf4.z; m_bulldata.pos.w = xmf4.w; }
	void			  DestroyBullet() { m_bulldata.alive = false; }
	char			  GetBulletCurrState() { return m_bulldata.alive; }

	void			  SetIsFirstCreate(bool flag) { m_IsFirstCreate = flag; }
	bool			  GetIsFirstCreate() { return m_IsFirstCreate; }

	void			  BulletLock() { m_lock.lock(); }
	void			  BulletUnLock() { m_lock.unlock(); }
	

	~BulletObject();
};


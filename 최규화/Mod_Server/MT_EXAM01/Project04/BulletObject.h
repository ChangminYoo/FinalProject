#pragma once
#include "PhysicalEffect.h"
#include "Player_Session.h"
#include "StaticObject.h"

class StaticObject;
class Player_Session;

class BulletObject
{
private:
	float		   lifetime;

	unsigned short  origin_hp{ 1 };
	unsigned short  cur_hp{ 1 };
	float			Damage{ 10 };
	unsigned short  Speed{ 50 };
	char		    GodMode{ true };
	char		    Ani{ Ani_State::Attack };

	CollisionBox  CollisionBox_Size;		//12
	bool		  alive{ true };

	short		  myID{ -1 };

	BulletObject_Info m_bulldata;

	XMFLOAT3 OffLookvector;
	XMFLOAT3 OffRightvector;

	XMFLOAT3 Lookvector;
	XMFLOAT3 Rightvector;
	XMFLOAT3 Upvector;

	RigidBody					 *rb{ nullptr };
	PhysicsPoint				 *pp{ nullptr };
	PhysicalEffect				 *pe{ nullptr };

	RayCastObject				  rco;

	bool						  m_delobj{ false };

public:
	BulletObject(const unsigned short& master_id, const unsigned short& target_id,
				 const Position& pos, const Rotation& rot, float bulltime, const unsigned short& my_id);

	void UpdateLookVector();
	void GetUpVector();

	void Update(float deltatime); //юс╫ц;

	float Get_LifeTime() const { return lifetime; }
	void Set_LifeTime(float time) { lifetime += time; }

	void Collision_StaticObjects(unordered_set<StaticObject*>& sobjs, float DeltaTime);
	void Collision_Players(vector<Player_Session*>& clients, float DeltaTime);

	~BulletObject();
};


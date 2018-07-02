#pragma once
//#include "CMonoObject.h"
#define MAX_LIGHT_BULLET_TIME 3.5
#define MAX_HEAVY_BULLET_TIME 5

class CBulletObject : public CMonoObject
{
private: 
	double m_lifetime{ 0 };
	
	short m_id{ -1 };
	unsigned short m_masterID;
	Position3D m_savept;
	Vel3f	   m_vel3f;

public:
	__int64					      m_prevTime{ 0 };
	__int64					      m_currTime{ 0 };
	//float						  m_deltaTime{ 0.f };
	CTS_BulletObject_Info         m_bulldata;

public:
	CBulletObject(const unsigned short& master_id, const unsigned short& my_id,
		const Position& pos, const Rotation& rot, float bulltime,
		Vel3f& vel, const unsigned char& type, const Position3D& endpt);

	virtual void AfterGravitySystem();
	virtual void Tick(__int64 deltime);
	virtual void Update(float deltime);
	void Collision(vector<CPlayerObject*>* clients, __int64 deltime);
	void Collision(unordered_set<CStaticObject*>* sobjs, __int64 deltime);

	float			  GetBulletLifeTime() const { return m_lifetime; }
	void			  SetBulletLifeTime(float time) { m_lifetime += time; }

	CTS_BulletObject_Info GetBulletInfo() { return m_bulldata; }
	Position		  GetBulletOldPos() const { return m_pos4f; }
	short			  GetBulletID() const { return m_id; }
	unsigned short    GetBulletMasterID() const { return m_masterID; }
	void			  SetBulletNewPos(float x, float y, float z, float w) { m_pos4f.x = x; m_pos4f.y = y; m_pos4f.z = z; m_pos4f.w = w; }
	void			  SetBulletNewPos(const XMFLOAT4& xmf4) { m_pos4f.x = xmf4.x; m_pos4f.y = xmf4.y; m_pos4f.z = xmf4.z; m_pos4f.w = xmf4.w; }
	void			  DestroyBullet() { m_alive = false; }
	char			  GetBulletIsAlive() { return static_cast<bool>(m_alive); }

	XMFLOAT3		  GetLookvector() const { return m_Lookvector; }
	Rotation		  GetBulletOldRot() const { return m_rot4f; }
	void			  SetBulletRotatevalue(const XMFLOAT4& xmf4);
	void			  SetChangedBulletState();

	~CBulletObject();
};


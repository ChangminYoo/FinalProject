#pragma once
#include "PhysicalEffect.h"
#include <unordered_map>
#include <unordered_set>

class StaticObject 
{
private:
	unordered_map<int, Position> m_boxPos;

	XMFLOAT3 OffLookvector;
	XMFLOAT3 OffRightvector;

	XMFLOAT3 Lookvector; 
	XMFLOAT3 Rightvector;
	XMFLOAT3 Upvector;

	StaticObject_Info m_sobjdata;

	//list<StaticObject*> m_staticObjs;

	unordered_set<StaticObject*> m_sobjs;

	//물리효과 처리
	PhysicsPoint		   *pp{ nullptr };
	PhysicalEffect		   *pe{ nullptr };

	CollisionBox			m_halfBox;

	bool					m_airbone{ false };					
	
public:
	StaticObject(){};
	~StaticObject();

	void							CycleStaticObjects();
	
	void							SetPosOfBox();
	void							InitBoxObjects();

	void							UpdatePPosCenterPos(XMFLOAT3& xmf3);
	void							AfterGravitySystem();

	//list<StaticObject*> GET_SObj_List() const { return m_staticObjs; } 
	
	unordered_set<StaticObject*>	GetSObjUdSet()      const { return m_sobjs; }
	unordered_set<StaticObject*>*	GetSObjUdSet_Collision()  { return &m_sobjs; }
	StaticObject_Info				GetInfoOfSObj()     const { return m_sobjdata; }

	PhysicsPoint*					GetPhysicsPoint()         { return pp; }
	XMFLOAT3						GetLookVector()     const { return Lookvector; }
	XMFLOAT3						GetRightVector()    const { return Rightvector; }
	XMFLOAT3						GetUpVector()		const { return Upvector; }

	void							SetAirBone(bool flag)	  { m_airbone = flag; }
	bool							GetIsStatic() { return m_sobjdata.Fixed; }
};


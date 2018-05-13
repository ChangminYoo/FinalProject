#pragma once
#include "PhysicalEffect.h"
#include <unordered_map>
#include <unordered_set>
#include <map>

class StaticObject 
{
private:
    map<int, StaticObjectBasicInfo> m_SobjPos;

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

	RayCastObject rco;
	
private:
	//여기다 물체 배치에 대한 상수들 넣는다.
	float BigWall_X1 = 300 * sinf(0.4f * MMPE_PI);
	float BigWall_Z1 = (600 * cosf(0.4f * MMPE_PI) + 600) / 2;

	float BigWall_X2 = ((600 * sinf(0.4f * MMPE_PI)) + (600 * sinf(0.8f * MMPE_PI))) / 2;
	float BigWall_Z2 = ((600 * cosf(0.4f * MMPE_PI)) + (-600 * cosf(0.2f * MMPE_PI))) / 2;

	float BigWall_Z3 = -600 * cosf(0.2f * MMPE_PI);

	float BigWall_Rad1 = MMPE_PI / 5; //36degree

public:
	StaticObject(){};
	~StaticObject();

	void							CycleStaticObjects();
	
	void							SetBasicInfoOfSObj();
	void							InitBoxObjects();
	void							InitSmallWallObjects();
	void							InitBigWallObjects();
	void							InitBuildingObjects();
	

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


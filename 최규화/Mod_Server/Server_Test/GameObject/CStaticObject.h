#pragma once
#include <map>
#include <unordered_set>

class CPlayerObject;
class CBulletObject;

struct BasicInfo
{
	Position  pos;
	double    degree;
	INSTALLED_OBJECT_TYPE type;
};

//CMonoObject: 기본 오브젝트 
//CStaticObject: 설치형 오브젝트
class CStaticObject : public CMonoObject
{
protected:
	double		        m_degree{ 0 };
	static map<int, BasicInfo> m_sobj_bdata;

public:
	CStaticObject();
	virtual ~CStaticObject();
	StaticObject_Info m_stc_sobjdata;

	void MakeStaticObjectBasicData();
	void CreateGameObject();
	void StaticObjectLoop();

public:
	//static object는 gravitysystem x // tick x
	//static object는 aftergravitysystem만 존재
	
};

class RigidCubeObject : public CStaticObject
{
public:
	RigidCubeObject(unsigned int id);
	void AmendObject(XMFLOAT3 axis, float radian, CMonoObject *obj);
	void RigidBodyCollisionPlane(XMFLOAT3 & Normal, float distance, double deltime, CMonoObject *obj);
	RigidbodyData m_stc_robjdata;

public:
	virtual void GravitySystem(double deltime) override;
	virtual void AfterGravitySystem(double deltime) override;
	virtual void Tick(double deltime) override;

	void Collision(unordered_set<RigidCubeObject*>* rbobjs, double deltime);
	void Collision(unordered_set<CStaticObject*>* sobjs, double deltime);
	void Collision(vector<CPlayerObject*>* clients, double deltime);
	void Collision(list<CBulletObject*>* bullets, double deltime);

	void SetUpdatedRigidybodyObject();
};

class NormalBoxObject : public CStaticObject
{
public:
	NormalBoxObject(unsigned int id);
	
};

class SmallWallObject : public CStaticObject
{
public:
	SmallWallObject(unsigned int id);

};

class BigWallObject : public CStaticObject
{
public:
	BigWallObject(unsigned int id);
	
};

class Building : public CStaticObject
{
public:
	Building(unsigned int id);
	
};

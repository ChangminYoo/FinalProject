#pragma once
#include <map>
#include <unordered_set>
//#include "CMonoObject.h"

struct BasicInfo
{
	Position  pos;
	double    degree;
	STATIC_OBJECT_TYPE type;
};

class CStaticObject : public CMonoObject
{
protected:
	double		        m_degree{ 0 };
	static map<int, BasicInfo> m_sobj_bdata;

public:
	StaticObject_Info m_stc_sobjdata;

public:
	CStaticObject();
	void MakeStaticObjectBasicData();
	void CreateGameObject();
	void StaticObjectLoop();
	virtual void AfterGravitySystem(float deltime);		//static object´Â gravitysystem x // tick x
	virtual void Collision();
	~CStaticObject();
};

class NormalBoxObject : public CStaticObject
{
public:
	NormalBoxObject(unsigned short id);
	//virtual void Collision();
};

class SmallWallObject : public CStaticObject
{
public:
	SmallWallObject(unsigned short id);
	//virtual void Collision();
};

class BigWallObject : public CStaticObject
{
public:
	BigWallObject(unsigned short id);
	//virtual void Collision();
};

class Building : public CStaticObject
{
public:
	Building(unsigned short id);
	//virtual void Collision();
};

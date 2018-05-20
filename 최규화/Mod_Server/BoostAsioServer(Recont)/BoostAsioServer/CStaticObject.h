#pragma once
#include <map>
#include <unordered_set>
#include "CMonoObject.h"

struct BasicInfo
{
	Position  pos;
	double    degree;
	STATIC_OBJECT_TYPE type;
};

map<int, BasicInfo> g_sobj_bdata;

void MakeStaticObjectBasicData();

class CStaticObject : CMonoObject
{
protected:
	StaticObject_Info m_stc_sobjdata;

public:
	CStaticObject();
	virtual void Initialize();
	virtual void Collision();
	~CStaticObject();
};

class NormalBoxObject : public CMonoObject
{
public:
	NormalBoxObject(unsigned short id);
	virtual void Collision();
};

class SmallWallObject : public CMonoObject
{
public:
	SmallWallObject(unsigned short id);
};

class BigWallObject : public CMonoObject
{
public:
	BigWallObject(unsigned short id);
};

class Building : public CMonoObject
{
public:
	Building(unsigned short id);
};

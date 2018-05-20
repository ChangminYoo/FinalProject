#pragma once
#include "CMonoObject.h"
class CBulletObject : public CMonoObject
{
private: 
	double m_lifetime{ 0 };
	
	unsigned short m_masterID;
	Position3D m_savept;

	BulletObject_Info m_bulldata;

public:
	CBulletObject(const unsigned short& master_id, const unsigned short& my_id,
		const Position& pos, const Rotation& rot, double bulltime,
		Vel3f& vel, const unsigned char& type, const Position3D& endpt);

	~CBulletObject();
};


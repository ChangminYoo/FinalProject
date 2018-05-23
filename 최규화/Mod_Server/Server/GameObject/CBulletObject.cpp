//#include "CBulletObject.h"
#include "../stdafx.h"

CBulletObject::CBulletObject(const unsigned short & master_id, const unsigned short & my_id, const Position & pos, const Rotation & rot, float bulltime, Vel3f & vel, const unsigned char & type, const Position3D & endpt)
{
	pp = new PhysicsPoint();

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	m_pos4f = pos;
	m_rot4f = rot;

	UpdateLookvector();
	UpdateUpvector();

	m_masterID = master_id;
	m_id = my_id;

	m_lifetime = bulltime;
	m_vel3f = vel;

	m_type = type;
	m_alive = true;

	m_savept = endpt;

	pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
	pp->SetVelocity(vel.x, vel.y, vel.z);

	pp->SetHalfBox(1, 1, 1);
	pp->SetDamping(1);
	pp->SetBounce(false);
	pp->SetVelocity(m_Lookvector.x * m_ability.speed, m_Lookvector.y * m_ability.speed, m_Lookvector.z * m_ability.speed);

	//--------------------------------------------------------------------------------------------
	
	m_bulldata.alive = m_alive;
	m_bulldata.endpoint = m_savept;
	m_bulldata.Master_ID = m_masterID;
	m_bulldata.myID = m_id;
	m_bulldata.pos = m_pos4f;
	m_bulldata.Rotate_status = m_rot4f;
	m_bulldata.type = type;
	m_bulldata.vel3f = vel;

	//--------------------------------------------------------------------------------------------
}

void CBulletObject::AfterGravitySystem(float deltime)
{
	if (m_pos4f.y <= 0)
		m_alive = false;
}

void CBulletObject::Tick(float deltime)
{
	XMFLOAT4 xmf4_rot{ m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	XMFLOAT4 xmf4_pos{ m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	
	xmf4_rot = QuaternionMultiply(xmf4_rot, QuaternionRotation(m_Lookvector, MMPE_PI * deltime));
	pp->integrate(deltime, &xmf4_pos);

}

void CBulletObject::Update(float deltime)
{
}

void CBulletObject::SetBulletRotatevalue(const XMFLOAT4 & xmf4)
{
	m_rot4f.x = xmf4.x;
	m_rot4f.y = xmf4.y;
	m_rot4f.z = xmf4.z;
	m_rot4f.w = xmf4.w;

	//m_bulldata.Rotate_status = m_rot4f;
}

void CBulletObject::SetChangedBulletState()
{
	m_bulldata.alive = m_alive;
	m_bulldata.endpoint = m_savept;
	m_bulldata.Master_ID = m_masterID;
	m_bulldata.myID = m_id;
	m_bulldata.pos = m_pos4f;
	m_bulldata.Rotate_status = m_rot4f;
	m_bulldata.type = m_type;
	m_bulldata.vel3f = m_vel3f;
}

CBulletObject::~CBulletObject()
{
	if (pp != nullptr)
		delete pp;
}

#include "CBulletObject.h"

CBulletObject::CBulletObject(const unsigned short & master_id, const unsigned short & my_id, const Position & pos, const Rotation & rot, double bulltime, Vel3f & vel, const unsigned char & type, const Position3D & endpt)
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
	m_bulldata.vel3f = vel;

	m_type = type;
	m_alive = true;

	m_savept = endpt;

	pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
	pp->SetVelocity(vel.x, vel.y, vel.z);
	pp->SetHalfBox(1, 1, 1);
	pp->SetDamping(1);
	pp->SetBounce(false);
	pp->SetVelocity(m_Lookvector.x * m_ability.speed, m_Lookvector.y * m_ability.speed, m_Lookvector.z * m_ability.speed);

}

CBulletObject::~CBulletObject()
{
	if (pp != nullptr)
		delete pp;
}

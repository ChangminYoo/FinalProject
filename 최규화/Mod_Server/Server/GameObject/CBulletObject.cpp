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

	pp->SetPosition(pos.x, pos.y, pos.z);
	pp->SetVelocity(vel.x, vel.y, vel.z);

	pp->SetHalfBox(1, 1, 1);
	pp->SetDamping(1);
	pp->SetBounce(false);

	m_ability.curHP = 0;
	m_ability.exp = 0;
	m_ability.level = 0;
	m_ability.orignHP = 0;
	m_ability.speed = 0;

	if (BULLET_TYPE::protocol_LightBullet == static_cast<int>(type)){
		m_ability.attack = 10;
	}
	else if (BULLET_TYPE::protocol_HeavyBullet == static_cast<int>(type)) {
		m_ability.attack = 30;
	}
	
	//--------------------------------------------------------------------------------------------
	
	m_bulldata.alive = m_alive;				
	m_bulldata.endpoint = m_savept;				//고정
	m_bulldata.master_id = m_masterID;			//고정
	m_bulldata.my_id = m_id;						//고정
	m_bulldata.pos4f = m_pos4f;
	m_bulldata.rot4f = m_rot4f;			
	m_bulldata.type = m_type;					//고정
	m_bulldata.vel3f = m_vel3f;					//고정

	//--------------------------------------------------------------------------------------------
}

void CBulletObject::AfterGravitySystem()
{
	if (m_pos4f.y <= 0 && m_alive == true)
	{
		m_alive = false;
		m_bulldata.alive = false;
	}
		
}

void CBulletObject::Tick(__int64 deltime)
{
	//deltime = deltime / 10000;

	cout << "Bullet DeltaTime: " << deltime << endl;

	XMFLOAT4 xmf4_rot{ m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	XMFLOAT4 xmf4_pos{ m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	
	pp->integrate(deltime, &xmf4_pos);

	xmf4_rot = QuaternionMultiply(xmf4_rot, QuaternionRotation(m_Lookvector, MMPE_PI * deltime));
	
	m_lifetime += deltime;

	m_lifetime = 0;
	if (m_lifetime >= MAX_LIGHT_BULLET_TIME && BULLET_TYPE::protocol_LightBullet == static_cast<int>(m_type))
	{
		m_alive = false;
		cout << "불렛 타임 초과" << endl;
	}
	else if (m_lifetime >= MAX_HEAVY_BULLET_TIME && BULLET_TYPE::protocol_HeavyBullet == static_cast<int>(m_type))
		m_alive = false;

	m_pos4f = { xmf4_pos.x , xmf4_pos.y, xmf4_pos.z, xmf4_pos.w };
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	m_bulldata.pos4f = { xmf4_pos.x, xmf4_pos.y, xmf4_pos.z, xmf4_pos.w };
	m_bulldata.rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };
	m_bulldata.alive = m_alive;

	cout << "IsAlive: " << static_cast<int>(m_bulldata.alive) << endl;

}

void CBulletObject::Update(float deltime)
{
}

void CBulletObject::Collision(vector<CPlayerObject*>* clients, __int64 deltime)
{

	for (auto iter = clients->begin(); iter != clients->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr && (*iter)->GetID() != m_masterID)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				(*iter)->GetDamaged(m_ability.attack);

				//고정된 물체가 아니면
				XMFLOAT3 cn;
				cn = Float3Add(pp->GetPosition(), (*(*iter)->GetPhysicsPoint()).GetPosition(), false);
				cn = Float3Normalize(cn);

				(*iter)->GetPhysicsPoint()->SetBounce(true);

				pp->ResolveVelocity(*(*iter)->GetPhysicsPoint(), cn, deltime);
				(*iter)->GetPhysicsPoint()->SetBounce(false);

				m_alive = false;
				m_bulldata.alive = false;
			}
		}
	}
	
}

void CBulletObject::Collision(unordered_set<CStaticObject*>* sobjs, __int64 deltime)
{

	for (auto iter = sobjs->begin(); iter != sobjs->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				//고정된 물체면
				XMFLOAT3 cn;
				cn = pp->pAxis;

				pp->ResolveVelocity(*(*iter)->GetPhysicsPoint(), cn, deltime);
				(*iter)->GetPhysicsPoint()->SetBounce(false);
				m_alive = false;
				m_bulldata.alive = false;
			}
		}
	}
	
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
	m_bulldata.master_id = m_masterID;
	m_bulldata.my_id = m_id;
	m_bulldata.pos4f = m_pos4f;
	m_bulldata.rot4f = m_rot4f;
	m_bulldata.type = m_type;
	m_bulldata.vel3f = m_vel3f;
}

CBulletObject::~CBulletObject()
{
	if (pp != nullptr)
		delete pp;
}

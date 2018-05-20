#include "CMonoObject.h"



CMonoObject::CMonoObject()
{
}

void CMonoObject::Initialize()
{
	pp = new PhysicsPoint();

	m_id = 0;
	m_fixed = true;
	m_alive = true;
	m_pos4f = { 0.f,0.f,0.f,0.f };
	m_rot4f = { 0.f,0.f,0.f,0.f };
	m_dir = 0;
	m_ai = true;
	m_godmode = false;
	m_airbone = false;
	m_type = STATIC_OBJECT_TYPE::BasicObject;
	m_degree = 0;

	m_ability.attack = 10;
	m_ability.speed = 50;
	m_ability.exp = 0;
	m_ability.level = 1;
	m_ability.orignHP = 100;
	m_ability.curHP = 100;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookvector();
	UpdateUpvector();

	pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
	pp->SetHalfBox(5, 5, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

}

void CMonoObject::Update(double deltime)
{
}

void CMonoObject::Collision()
{
}

void CMonoObject::AfterGravitySystem(double deltime, OBJECT_TYPE type)
{
	switch (type)
	{
	case PLAYER:
	case FIXED_OBJECT:
		{
			float ppy = pp->GetPosition().y;
			float hby = pp->GetHalfBox().y;
			if (ppy - hby < 0)
			{
				XMFLOAT3 gp = pp->GetPosition();
				gp.y += hby - ppy;
				pp->SetPosition(gp);
				UpdatePPosCenterPos();
				auto v = pp->GetVelocity();
				v.y = 0;
				pp->SetVelocity(v);
				m_airbone = false;
			}
		}
		break;
	case BULLET:
		{
			if (m_pos4f.y <= 0)
				m_alive = false;
		}
		break;
	default:
		break;
	}
}


void CMonoObject::UpdateUpvector()
{
	XMVECTOR l = XMLoadFloat3(&m_Lookvector);
	XMVECTOR r = XMLoadFloat3(&m_Rightvector);
	auto u = XMVector3Cross(l, r);

	XMFLOAT3 up;
	XMStoreFloat3(&up, u);
	up = Float3Normalize(up);

	m_Upvector = move(up);
}

void CMonoObject::UpdateLookvector()
{
	auto wmatrix = XMMatrixIdentity();
	
	XMFLOAT4 xmf4{ m_rot4f.x,m_rot4f.y,m_rot4f.z,m_rot4f.w };
	auto quater = XMLoadFloat4(&xmf4);
	wmatrix *= XMMatrixRotationQuaternion(quater);
	auto ol = XMLoadFloat3(&m_OffLookvector);
	auto orr = XMLoadFloat3(&m_OffRightvector);

	ol = XMVector4Transform(ol, wmatrix);
	orr = XMVector4Transform(orr, wmatrix);

	XMStoreFloat3(&m_Lookvector, ol);
	XMStoreFloat3(&m_Rightvector, orr);

	if (fabsf(m_Lookvector.x) < MMPE_EPSILON / 10)
		m_Lookvector.x = 0;
	if (fabsf(m_Lookvector.y) < MMPE_EPSILON / 10)
		m_Lookvector.y = 0;
	if (fabsf(m_Lookvector.z) < MMPE_EPSILON / 10)
		m_Lookvector.z = 0;


	if (fabsf(m_Rightvector.x) < MMPE_EPSILON / 10)
		m_Rightvector.x = 0;
	if (fabsf(m_Rightvector.y) < MMPE_EPSILON / 10)
		m_Rightvector.y = 0;
	if (fabsf(m_Rightvector.z) < MMPE_EPSILON / 10)
		m_Rightvector.z = 0;


	m_Lookvector = Float3Normalize(m_Lookvector);
	m_Rightvector = Float3Normalize(m_Rightvector);
}

void CMonoObject::UpdatePPosCenterPos()
{
	XMFLOAT4 xmf4;
	xmf4 = XMFloat3to4(pp->GetPosition());

	m_pos4f.x = xmf4.x; 
	m_pos4f.y = xmf4.y; 
	m_pos4f.z = xmf4.z; 
	m_pos4f.w = xmf4.w;
}


CMonoObject::~CMonoObject()
{
	if (pp != nullptr)
		delete pp;

}

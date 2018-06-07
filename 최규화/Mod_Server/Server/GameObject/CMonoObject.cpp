//#include "CMonoObject.h"
#include "../stdafx.h"


CMonoObject::CMonoObject()
{
}

void CMonoObject::Update(float deltime)
{
}

// 물리효과 순서 1.중력 2.가속도 및 중력 적용 3.중력 후처리 4.충돌
void CMonoObject::GravitySystem(float deltime)
{
}

void CMonoObject::Tick(float deltime)
{
}

void CMonoObject::AfterGravitySystem(float deltime)
{
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

#include "PhysicalEffect.h"



PhysicalEffect::PhysicalEffect()
{
}


PhysicalEffect::~PhysicalEffect()
{
}

void PhysicalEffect::GetUpVector(XMFLOAT3& Lv, XMFLOAT3& Rv, XMFLOAT3& Uv)
{
	XMVECTOR l = XMLoadFloat3(&Lv);
	XMVECTOR r = XMLoadFloat3(&Rv);
	auto u = XMVector3Cross(l, r);

	XMFLOAT3 up;
	XMStoreFloat3(&up, u);
	up = Float3Normalize(up);

	Uv = move(up);
}

void PhysicalEffect::UpdateLookVector(const XMFLOAT3& offLv, const XMFLOAT3& offRv, Rotation& orient, XMFLOAT3& Lv, XMFLOAT3& Rv)
{
	auto wmatrix = XMMatrixIdentity();

	//클라이언트에서 MouseMove를 통해 카메라를 회전할 때 마다 Rotate_status가 달라짐
	XMFLOAT4 orient_xmfloat4 =
	{	orient.x ,
		orient.y ,
		orient.z ,
		orient.w };

	auto quater = XMLoadFloat4(&orient_xmfloat4);
	wmatrix *= XMMatrixRotationQuaternion(quater);

#undef or
	//OffLookvector 와 OffRightvector는 플레이어타입(캐릭터, 불렛, 스테틱오브젝트 등에 따라 다름)
	auto ol = XMLoadFloat3(&offLv);
	auto or = XMLoadFloat3(&offRv);

	ol = XMVector4Transform(ol, wmatrix);
	or = XMVector4Transform(or , wmatrix);

	XMStoreFloat3(&Lv, ol);
	XMStoreFloat3(&Rv, or );

	if (fabsf(Lv.x) < MMPE_EPSILON / 10)
		Lv.x = 0;
	if (fabsf(Lv.y) < MMPE_EPSILON / 10)
		Lv.y = 0;
	if (fabsf(Lv.z) < MMPE_EPSILON / 10)
		Lv.z = 0;


	if (fabsf(Rv.x) < MMPE_EPSILON / 10)
		Rv.x = 0;
	if (fabsf(Rv.y) < MMPE_EPSILON / 10)
		Rv.y = 0;
	if (fabsf(Rv.z) < MMPE_EPSILON / 10)
		Rv.z = 0;

	Lv = Float3Normalize(Lv);
	Rv = Float3Normalize(Rv);
}

void PhysicalEffect::UpdatePPosCenterPos(XMFLOAT3& xmf3, Position& pos)
{
	XMFLOAT4 xmf4 = XMFloat3to4(xmf3);

	pos.x = xmf4.x; pos.y = xmf4.y; pos.z = xmf4.z; pos.w = xmf4.w;
}

void PhysicalEffect::GravitySystem(float time, PhysicsPoint *pp)
{
	GeneratorGravity gg;
	gg.SetGravityAccel(XMFLOAT3(0, -100, 0));

	gg.Update(time, *pp);
}

void PhysicalEffect::AfterGravitySystem(float time, PhysicsPoint *pp, OBJECT_TYPE type, Position& pos, bool& airbone)
{
	switch (type)
	{
	case OBJECT_TYPE::PLAYER:
	case OBJECT_TYPE::FIXED_OBJECT:
		{
			float ppy = pp->GetPosition().y;
			float hby = pp->GetHalfBox().y;
			if (ppy - hby < 0)
			{
				XMFLOAT3 gp = pp->GetPosition();
				gp.y += hby - ppy;
				pp->SetPosition(gp);
				UpdatePPosCenterPos(pp->GetPosition(), pos);
				auto v = pp->GetVelocity();
				v.y = 0;
				pp->SetVelocity(v);
				airbone = false;
			}
		}
		break;
	default:
		break;
	}
}

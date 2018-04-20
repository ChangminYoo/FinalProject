#pragma once
#include "PhysicsEngine\MyMiniPysicsEngine.h"
#include "stdafx.h"

class PhysicalEffect
{
public:
	PhysicalEffect();

	void GetUpVector(XMFLOAT3& Lv, XMFLOAT3& Rv, XMFLOAT3& Uv);

	void UpdateLookVector(const XMFLOAT3& offLv, const XMFLOAT3& offRv, Rotation& orient, XMFLOAT3& Lv, XMFLOAT3& Rv);

	void UpdatePPosCenterPos(XMFLOAT3& xmf3, Position& pos);

	void GravitySystem(float time, PhysicsPoint *pp);

	void AfterGravitySystem(float time, PhysicsPoint *pp, OBJECT_TYPE type, Position& pos, bool& airbone);

	~PhysicalEffect();
};


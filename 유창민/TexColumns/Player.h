#pragma once
#include <iostream>
#include "d3dApp.h"
#include "MathHelper.h"
#include "GeometryGenerator.h"
#include "GameTimer.h"
#include "FrameResource.h"

using namespace DirectX;


class Player
{
public:
	Player();
	~Player();

	void SetPosition(XMFLOAT3& xmf3Position) {}
	XMFLOAT3 GetPosition() { return xmf3Position; }


private:
	XMFLOAT3 xmf3Position;
	

};


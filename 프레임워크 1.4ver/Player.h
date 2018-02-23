#pragma once

#include"Camera.h"
#include"CGameObject.h"

class CPlayer
{
public:
	CPlayer(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist);
	~CPlayer();

	void PlayerInput(); //플레이어의 기본 정보


	CCamera Cam;
	CGameObject* PlayerObject;

private:
	int Skillnum; //사용 스킬 넘버

};


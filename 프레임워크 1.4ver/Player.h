#pragma once

#include"Camera.h"
#include"CGameObject.h"

class CPlayer
{
public:
	CPlayer(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist);
	~CPlayer();

	void PlayerInput(); //�÷��̾��� �⺻ ����


	CCamera Cam;
	CGameObject* PlayerObject;

private:
	int Skillnum; //��� ��ų �ѹ�

};


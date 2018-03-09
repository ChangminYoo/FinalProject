#pragma once
#include"CGameObject.h"
#include"Camera.h"
class CPlayer
{
public:
	CPlayer(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist, float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u);
	~CPlayer();

	void TPSCameraSystem(int mx,int my,float DeltaTime);
	void PlayerCameraReLocate();
	void SetPlayer(CGameObject* obj);
	void PlayerInput(float DeltaTime);


public:
	CCamera Camera;
	CGameObject* PlayerObject;


private:
	//카메라 회전관련 데이터들
	float ox = -1;//이전마우스 x좌표
	float oy = -1;//이전마우스 y좌표
	float xtheta = 0;
	float ytheta = 0;

};


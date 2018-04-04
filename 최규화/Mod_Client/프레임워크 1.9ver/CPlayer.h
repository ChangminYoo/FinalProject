#pragma once
#include"CGameObject.h"
#include"Camera.h"
#include"server\main_client\AsyncClient.h"

class AsyncClient;

class CPlayer
{
public:
	CPlayer(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist, float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u);
	~CPlayer();

	void TPSCameraSystem(int mx,int my,float DeltaTime);
	void PlayerCameraReLocate();
	void SetPlayer(CGameObject* obj);
	void PlayerInput(float DeltaTime);
	CCamera Camera;
	CGameObject* PlayerObject;

	//ī�޶� ȸ������ �����͵�
	float ox = -1;//�������콺 x��ǥ
	float oy = -1;//�������콺 y��ǥ
	float xtheta = 0;
	float ytheta = 0;

	//Player_Data* m_player_data;

	AsyncClient* m_async_client;
};


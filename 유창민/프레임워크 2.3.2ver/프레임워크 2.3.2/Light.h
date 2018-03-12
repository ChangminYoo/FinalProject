#pragma once
#include "Timer.h"
#include"UploadBuffer.h"

using namespace DirectX;
#define LIGHT_MAX 10

struct Light
{
	//�� ���� (Strength)
	XMFLOAT4 LightColor = { 0.5f, 0.5f, 0.5f ,1};	
	//����
	XMFLOAT4 Direction = { 0.0f, -1.0f, 0.0f,1 };                 
	//��ġ
	XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };  
	float SpecularPower = 64.0f;
};

struct LightData
{
	Light light[LIGHT_MAX];
	XMFLOAT4 AmbientLight = { 0.0f, 0.0f, 0.0f, 1.0f }; //�ֺ���
	int nLight=0;//����Ʈ���� ����Ʈ �����͸� ä�ﶧ 1�� �������Ѿ���.
	
};


class CLight
{
public:
	CLight();
	CLight(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist);
	~CLight();

	
	void UpdateConstantBuffer( ID3D12GraphicsCommandList* commandlist);
	UploadBuffer<LightData>* ConstBuffer = nullptr;
	LightData LData;

};


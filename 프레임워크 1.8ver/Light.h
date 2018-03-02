#pragma once
#include "Timer.h"
#include"UploadBuffer.h"

using namespace DirectX;
#define LIGHT_MAX 10

struct Light
{
	//빛 색상 (Strength)
	XMFLOAT3 LightColor = { 0.5f, 0.5f, 0.5f };	
	//방향
	XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };                 
	//위치
	XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };  
	float SpotPower = 64.0f;
};

struct LightData
{
	Light light[LIGHT_MAX];

	XMFLOAT4 AmbientLight = { 0.0f, 0.0f, 0.0f, 1.0f }; //주변광
	
	UploadBuffer<LightData>* ConstBuffer = nullptr;
};


class CLight
{
public:
	CLight();
	CLight(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist);
	~CLight();

	int nLight;//라이트갯수

	void UpdateConstantBuffer( ID3D12GraphicsCommandList* commandlist);

	LightData LData;

};


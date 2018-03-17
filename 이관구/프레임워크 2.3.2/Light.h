#pragma once
#include "Timer.h"
#include"UploadBuffer.h"

using namespace DirectX;
#define LIGHT_MAX 10

struct Light
{
	//빛 색상 (Strength)
	XMFLOAT4 LightColor = { 0.5f, 0.5f, 0.5f ,1};	
	//방향
	XMFLOAT4 Direction = { 0.0f, -1.0f, 0.0f,1 };                 
	//위치
	XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };  
	float SpecularPower = 64.0f;
};

struct LightData
{
	Light light[LIGHT_MAX];
	XMFLOAT4 AmbientLight = { 0.0f, 0.0f, 0.0f, 1.0f }; //주변광
	int nLight=0;//라이트갯수 라이트 데이터를 채울때 1씩 증가시켜야함.
	
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


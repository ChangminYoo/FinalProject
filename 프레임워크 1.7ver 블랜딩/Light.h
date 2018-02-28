#pragma once
#include "Timer.h"
#include"UploadBuffer.h"

using namespace DirectX;
#define LIGHT_MAX 10

struct Light
{
	//ºû »ö»ó
	XMFLOAT3 LightColor = { 0.5f, 0.5f, 0.5f };	
	
	XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };                 
	
	XMFLOAT3 Position = { 0.0f, 100.0f, 0.0f };  
	float SpotPower = 64.0f;
};

struct LightData
{
	Light light[LIGHT_MAX];

	XMFLOAT4 AmbientLight = { 0.0f, 0.0f, 0.0f, 1.0f }; //ÁÖº¯±¤
	
	UploadBuffer<LightData>* ConstBuffer = nullptr;
};


class CLight
{
public:
	CLight();
	CLight(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist);
	~CLight();

	int nLight;//¶óÀÌÆ®°¹¼ö

	void UpdateConstantBuffer( ID3D12GraphicsCommandList* commandlist);

	LightData lightData;

};


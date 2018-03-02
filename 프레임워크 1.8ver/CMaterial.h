#pragma once
#include"UploadBuffer.h"

using namespace DirectX;

struct MaterialData
{
	float Roughness = 0.25f;	//°ÅÄ¥±â

	UploadBuffer<MaterialData>* ConstBuffer = nullptr;
};


class CMaterial
{
public:
	CMaterial(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist);
	CMaterial();
	~CMaterial();

	void UpdateConstantBuffer(ID3D12GraphicsCommandList* commandlist);

	MaterialData MatData;
};


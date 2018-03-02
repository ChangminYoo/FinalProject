#pragma once
#include"UploadBuffer.h"

using namespace DirectX;

struct MaterialData
{
	XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };  // 빛 반사율
	XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };		//프레넬
	float Roughness = 0.25f;	//거칠기

	//텍스쳐 매핑에 사용.
	//XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();

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


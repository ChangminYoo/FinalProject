#include "Light.h"



CLight::CLight(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist)
{

	LData.ConstBuffer = new UploadBuffer<LightData>(Device, 1, true);
}

CLight::CLight()
{}

CLight::~CLight()
{
	if (LData.ConstBuffer != nullptr)
		delete LData.ConstBuffer;
}

void CLight::UpdateConstantBuffer(ID3D12GraphicsCommandList* commandlist)
{

	LData.light[0].LightColor = { 1.0f, 1.0f, 1.0f };
	LData.light[0].Direction = { 0.0f, -1.0f, 0.0f };
	LData.light[0].Position = { 0.0f, 20.0f, 0.0f };
	LData.light[0].SpotPower = 1.0f;

	LData.AmbientLight = { 0.25f, 0.25f, 0.25f, 1.0f };

	LData.ConstBuffer->CopyData(0, LData);
	commandlist->SetGraphicsRootConstantBufferView(4, LData.ConstBuffer->Resource()->GetGPUVirtualAddress());

}
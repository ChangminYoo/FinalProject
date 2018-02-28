#include "Light.h"



CLight::CLight(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist)
{

	lightData.ConstBuffer = new UploadBuffer<LightData>(Device, 1, true);
}

CLight::CLight()
{}

CLight::~CLight()
{
	if (lightData.ConstBuffer != NULL)
		delete lightData.ConstBuffer;
}

void CLight::UpdateConstantBuffer(ID3D12GraphicsCommandList* commandlist)
{
	lightData.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };

	lightData.light[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	lightData.light[0].LightColor = { 0.6f, 0.6f, 0.6f };

	lightData.ConstBuffer->CopyData(0, lightData);
	commandlist->SetGraphicsRootConstantBufferView(4, lightData.ConstBuffer->Resource()->GetGPUVirtualAddress());

}
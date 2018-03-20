#include "Light.h"



CLight::CLight(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist)
{

	ConstBuffer = new UploadBuffer<LightData>(Device, 1, true);
}

CLight::CLight()
{}

CLight::~CLight()
{
	if (ConstBuffer != nullptr)
		delete ConstBuffer;
}

void CLight::UpdateConstantBuffer(ID3D12GraphicsCommandList* commandlist)
{
	
	LData.light[0].LightColor = { 1.0f, 1.0f, 1.0f,1 };
	LData.light[0].Direction = { -3.0f, -3.0f, -10.0f,1 };
	LData.light[0].Position = { 100.0f, 100.0f, 0.0f };
	LData.light[0].SpecularPower = 32.0f;
	LData.nLight = 1;

	LData.AmbientLight = { 0.138f, 0.138f, 0.138f, 1.0f };

	ConstBuffer->CopyData(0, LData);
	commandlist->SetGraphicsRootConstantBufferView(4, ConstBuffer->Resource()->GetGPUVirtualAddress());

}
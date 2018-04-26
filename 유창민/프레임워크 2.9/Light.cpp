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
	
	LData.light[0].LightColor = { 0.8f, 0.8f, 0.8f,1 };
	LData.light[0].Direction = { 2.577f, -0.577f, -0.677f, 1.0f };
	LData.light[0].Position = { -400.0f, 220.0f, 10.0f };
	LData.light[0].SpecularPower = 32.0f;
	LData.nLight = 1;
	LData.light[1].LightColor = { 0.9f, 0.9f, 0.9f,1 };
	LData.light[1].Direction = { -2.577f, -0.577f, 0.577f, 1.0f };
	LData.light[1].Position = { 402.0f, 220.0f, -10.0f };
	LData.light[1].SpecularPower = 32.0f;
	LData.nLight = 2;
	LData.light[2].LightColor = { 0.7f, 0.7f, 0.7f,1 };
	LData.light[2].Direction = { -0.02f, -1.23f, 0.344f, 1.0f };
	LData.light[2].Position = { 0.0f, 220.0f, 0.0f };
	LData.light[2].SpecularPower = 32.0f;
	LData.nLight = 3;

	LData.AmbientLight = { 0.298f, 0.298f, 0.298f, 0.9f };

	ConstBuffer->CopyData(0, LData);
	commandlist->SetGraphicsRootConstantBufferView(5, ConstBuffer->Resource()->GetGPUVirtualAddress());

}
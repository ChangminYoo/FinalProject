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
	LData.light[0].Direction = { 0.7f,-1.5f,1.1f,1.0f };
	LData.light[0].Position = { -400.0f, 620.0f, 30.0f };
	LData.light[0].SpecularPower = 32.0f;
	LData.nLight = 1;

	//LData.light[1].LightColor = { 202.0f/255, 208.0f/255, 237.0f/255,1 };
	//LData.light[1].Direction = { -0.577f, -1.977f, 1.577f, 1.0f };
	//LData.light[1].Position = { 102.0f, 520.0f, -120.0f };
	//LData.light[1].SpecularPower = 32.0f;
	//LData.nLight = 2;

	
	LData.AmbientLight = { 0.288f, 0.289f, 0.284f, 0.8f };

	ConstBuffer->CopyData(0, LData);
	commandlist->SetGraphicsRootConstantBufferView(3, ConstBuffer->Resource()->GetGPUVirtualAddress());

}
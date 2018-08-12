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
	
	LData.light[0].LightColor = { 0.79f, 0.79f, 0.79f,1 };
	LData.light[0].Direction = { -5.7f,-10.5f,8.1f,1.0f };
	LData.light[0].Position = { -40.0f, 620.0f, 30.0f };
	LData.light[0].SpecularPower = 16.0f;
	LData.nLight = 1;

	LData.light[1].LightColor = { 3.9f,3.9f, 3.9f,1 };
	LData.light[1].Direction = { -0.577f, -1.977f, 1.577f, 1.0f };
	LData.light[1].Position = { 0.0f, 600.0f, -400.0f };
	LData.light[1].SpecularPower = 8.0f;
	LData.nLight = 2;
	
	LData.light[2].LightColor = { 3.70f, 3.7f, 3.07f,1 };
	LData.light[2].Direction = { 0.977f, -1.277f, -0.1577f, 1.0f };
	LData.light[2].Position = { -150.0f, 500.0f, 100.0f };
	LData.light[2].SpecularPower = 32.0f;
	LData.nLight =3;
	
	LData.light[3].LightColor = {0.90f,0.9f,0.90f,1 };
	LData.light[3].Direction = { 3.977f, -1.277f, -8.577f, 1.0f };
	LData.light[3].Position = { 00.0f, 120.0f, 00.0f };
	LData.light[3].SpecularPower =4.0f;
	LData.nLight = 4;
	//
	//LData.light[4].LightColor = { 0.90f, 0.9f, 0.90f,1 };
	//LData.light[4].Direction = { -3.977f, -1.277f, -6.577f, 1.0f };
	//LData.light[4].Position = { 1300.0f, 2500.0f, 1270.0f };
	//LData.light[4].SpecularPower = 32.0f;
	//LData.nLight = 5;
	//
	//LData.light[5].LightColor = { 0.8f, 0.80f, 0.8f,1 };
	//LData.light[5].Direction = { -7.577f, -8.977f, 7.577f, 1.0f };
	//LData.light[5].Position = { 600.0f, 2500.0f, -2570.0f };
	//LData.light[5].SpecularPower = 32.0f;
	//LData.nLight = 6;
	//
	//LData.light[6].LightColor = { 0.90f, 0.9f, 0.90f,1 };
	//LData.light[6].Direction = { 50.977f, -21.277f, -60.577f, 1.0f };
	//LData.light[6].Position = { 400.0f, 3000.0f, -970.0f };
	//LData.light[6].SpecularPower = 32.0f;
	//LData.nLight = 7;
	//
	//LData.light[7].LightColor = { 0.70f, 0.7f, 0.70f,1 };
	//LData.light[7].Direction = { 3.977f, -10.277f, 8.577f, 1.0f };
	//LData.light[7].Position = { -700.0f, 3200.0f, -570.0f };
	//LData.light[7].SpecularPower = 32.0f;
	//LData.nLight = 8;
	//
	//LData.light[8].LightColor = { 0.70f, 0.7f, 0.70f,1 };
	//LData.light[8].Direction = { -3.977f, -10.277f, -6.577f, 1.0f };
	//LData.light[8].Position = { 500.0f, 3000.0f, 190.0f };
	//LData.light[8].SpecularPower = 32.0f;
	//LData.nLight =9;
	//
	//LData.light[9].LightColor = { 0.670f, 0.67f, 0.670f,1 };
	//LData.light[9].Direction = { -3.977f, -10.277f, 6.577f, 1.0f };
	//LData.light[9].Position = { -500.0f, 3000.0f, -170.0f };
	//LData.light[9].SpecularPower = 32.0f;
	//LData.nLight = 10;
	
	LData.AmbientLight = { 0.5305248f, 0.5305249f, 0.5305244f, 0.8f };

	ConstBuffer->CopyData(0, LData);
	commandlist->SetGraphicsRootConstantBufferView(3, ConstBuffer->Resource()->GetGPUVirtualAddress());

}
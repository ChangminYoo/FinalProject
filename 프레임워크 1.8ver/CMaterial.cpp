#include "CMaterial.h"

CMaterial::CMaterial(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist)
{

}

CMaterial::CMaterial()
{}

CMaterial::~CMaterial()
{
	if (MatData.ConstBuffer != nullptr)
		delete MatData.ConstBuffer;
}

void CMaterial::UpdateConstantBuffer(ID3D12GraphicsCommandList * commandlist)
{
	

	MatData.ConstBuffer->CopyData(0, MatData);
	commandlist->SetGraphicsRootConstantBufferView(5, MatData.ConstBuffer->Resource()->GetGPUVirtualAddress());

}

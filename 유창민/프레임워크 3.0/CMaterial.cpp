#include "CMaterial.h"

CMaterial::CMaterial(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist)
{

}

CMaterial::CMaterial()
{}

CMaterial::~CMaterial()
{
	if (ConstBuffer != nullptr)
		delete ConstBuffer;
}

void CMaterial::UpdateConstantBuffer(ID3D12GraphicsCommandList * commandlist)
{
	

	ConstBuffer->CopyData(0, MatData);
	commandlist->SetGraphicsRootConstantBufferView(6, ConstBuffer->Resource()->GetGPUVirtualAddress());

}

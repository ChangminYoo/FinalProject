#include "CGameUIObject.h"

AimObject::AimObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 20.0f;
	ObjData.SpecularParamater = 0.5f;//스페큘러를 낮게준다.


	//게임관련 데이터들
	gamedata.GodMode = true;
	staticobject = true;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "AimTex", L"textures/ui/aim_circle.dds", false);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}

}

void AimObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	UINT numOfitem = 1;


	Mesh.SubResource = new CVertex;
	Mesh.nVertex = numOfitem;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT;
	Mesh.nindex = numOfitem;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);


	//여기서 좌표를 일괄적으로 설정 할 수 있다
	for (int i = 0; i < numOfitem; ++i)
	{

		Mesh.SubResource[i].V = XMFLOAT3(0, 0, 0);

		Mesh.Index[i] = i;
	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}


void AimObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//텍스처가 사이즈가 0 이상이면 연결
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["AimTex"].get()->Resource.Get(), false);
	//월드변환 업데이트 및 연결
	UpdateConstBuffer(commandlist);

	//애니메이션이 있으면 애니메이션또한 연결

	//메쉬를 렌더해야하는데 포인트리스트로 설정해야하므로 그냥 이렇게 함.
	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = Mesh.VertexBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = Mesh.nStride;
	vbv.SizeInBytes = Mesh.nStride *  Mesh.nVertex;

	commandlist->IASetVertexBuffers(0, 1, &vbv);

	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = Mesh.IndexBuffer->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = Mesh.nisize *  Mesh.nindex;

	commandlist->IASetIndexBuffer(&ibv);
	commandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	commandlist->DrawIndexedInstanced(Mesh.nindex, 1, Mesh.nioffset, Mesh.nOffset, 0);
}


CoolBarObject::CoolBarObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, float maxtime, CGameObject * master, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	MaxCoolTime = maxtime;
	ObjData.isAnimation = 0;
	ObjData.Scale = 80.0f;
	ObjData.SpecularParamater = 0.5f;//스페큘러를 낮게준다.
	ObjData.CustomData1.x = 1;

	Master = master;
	ObjData.CustomData1.y = 0;
	//게임관련 데이터들
	gamedata.HP = 1;
	gamedata.GodMode = true;
	staticobject = true;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "CoolBarTex", L"textures/ui/HP.dds", false);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}

}

void CoolBarObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	UINT numOfitem = 1;

	Mesh.SubResource = new CVertex;
	Mesh.nVertex = numOfitem;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT;
	Mesh.nindex = numOfitem;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);


	//여기서 좌표를 일괄적으로 설정 할 수 있다
	for (int i = 0; i < numOfitem; ++i)
	{
		Mesh.SubResource[i].V = XMFLOAT3(0, 0, 0);

		Mesh.Index[i] = i;
	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void CoolBarObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CoolBarTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);


	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = Mesh.VertexBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = Mesh.nStride;
	vbv.SizeInBytes = Mesh.nStride *  Mesh.nVertex;

	commandlist->IASetVertexBuffers(0, 1, &vbv);

	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = Mesh.IndexBuffer->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = Mesh.nisize *  Mesh.nindex;

	commandlist->IASetIndexBuffer(&ibv);
	commandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	commandlist->DrawIndexedInstanced(Mesh.nindex, 1, Mesh.nioffset, Mesh.nOffset, 0);

}


SkillUIObject::SkillUIObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, int SkillNum, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 80.0f;
	ObjData.SpecularParamater = 0.5f;//스페큘러를 낮게준다.
	ObjData.CustomData1.x = 3;

	//게임관련 데이터들
	gamedata.GodMode = true;
	staticobject = true;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "SkillBG", L"textures/ui/SkillBar.dds", false);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void SkillUIObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	UINT numOfitem = 1;


	Mesh.SubResource = new CVertex;
	Mesh.nVertex = numOfitem;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT;
	Mesh.nindex = numOfitem;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);


	//여기서 좌표를 일괄적으로 설정 할 수 있다
	for (int i = 0; i < numOfitem; ++i)
	{

		Mesh.SubResource[i].V = XMFLOAT3(0, 0, 0);

		Mesh.Index[i] = i;
	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void SkillUIObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//텍스처가 사이즈가 0 이상이면 연결
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["SkillBG"].get()->Resource.Get(), false);
	//월드변환 업데이트 및 연결
	UpdateConstBuffer(commandlist);

	//애니메이션이 있으면 애니메이션또한 연결

	//메쉬를 렌더해야하는데 포인트리스트로 설정해야하므로 그냥 이렇게 함.
	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = Mesh.VertexBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = Mesh.nStride;
	vbv.SizeInBytes = Mesh.nStride *  Mesh.nVertex;

	commandlist->IASetVertexBuffers(0, 1, &vbv);

	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = Mesh.IndexBuffer->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = Mesh.nisize *  Mesh.nindex;

	commandlist->IASetIndexBuffer(&ibv);
	commandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	commandlist->DrawIndexedInstanced(Mesh.nindex, 1, Mesh.nioffset, Mesh.nOffset, 0);
}


SelectBarObject::SelectBarObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 80.0f;
	ObjData.SpecularParamater = 0.5f;//스페큘러를 낮게준다.
	ObjData.CustomData1.x = 3;


	//게임관련 데이터들

	gamedata.GodMode = true;
	staticobject = true;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "SelectTex", L"textures/ui/SelectBar.dds", false);

		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void SelectBarObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	UINT numOfitem = 1;

	Mesh.SubResource = new CVertex;
	Mesh.nVertex = numOfitem;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT;
	Mesh.nindex = numOfitem;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);


	//여기서 좌표를 일괄적으로 설정 할 수 있다
	for (int i = 0; i < numOfitem; ++i)
	{
		Mesh.SubResource[i].V = XMFLOAT3(0, 0, 0);

		Mesh.Index[i] = i;
	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void SelectBarObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["SelectTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);


	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = Mesh.VertexBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = Mesh.nStride;
	vbv.SizeInBytes = Mesh.nStride *  Mesh.nVertex;

	commandlist->IASetVertexBuffers(0, 1, &vbv);

	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = Mesh.IndexBuffer->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = Mesh.nisize *  Mesh.nindex;

	commandlist->IASetIndexBuffer(&ibv);
	commandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	commandlist->DrawIndexedInstanced(Mesh.nindex, 1, Mesh.nioffset, Mesh.nOffset, 0);
}

//----------
BackGroundObject::BackGroundObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	//이녀석은 스케일을 x 길이로 CustomData1.x는 5로 설정해서 뭐하는녀석인지 선택하게 한후 CustomData1.y가 y길이로 하는 사각형이된다.

	ObjData.isAnimation = 0;
	ObjData.Scale = 800.0f;
	ObjData.CustomData1.y = 600;
	ObjData.SpecularParamater = 0.5f;//스페큘러를 낮게준다.
	ObjData.CustomData1.x = 5;

	//게임관련 데이터들
	gamedata.GodMode = true;
	staticobject = true;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "StartBG", L"textures/ui/Range.dds", false,2,0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "LoadBG", L"textures/ui/MainBar.dds", false,2,1);
		TextureName = "StartBG";

		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void BackGroundObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	UINT numOfitem = 1;


	Mesh.SubResource = new CVertex;
	Mesh.nVertex = numOfitem;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT;
	Mesh.nindex = numOfitem;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);


	//여기서 좌표를 일괄적으로 설정 할 수 있다
	for (int i = 0; i < numOfitem; ++i)
	{

		Mesh.SubResource[i].V = XMFLOAT3(0, 0, 0);

		Mesh.Index[i] = i;
	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void BackGroundObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//텍스처가 사이즈가 0 이상이면 연결
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), false,TexOff);
	//월드변환 업데이트 및 연결
	UpdateConstBuffer(commandlist);

	//애니메이션이 있으면 애니메이션또한 연결

	//메쉬를 렌더해야하는데 포인트리스트로 설정해야하므로 그냥 이렇게 함.
	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = Mesh.VertexBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = Mesh.nStride;
	vbv.SizeInBytes = Mesh.nStride *  Mesh.nVertex;

	commandlist->IASetVertexBuffers(0, 1, &vbv);

	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = Mesh.IndexBuffer->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = Mesh.nisize *  Mesh.nindex;

	commandlist->IASetIndexBuffer(&ibv);
	commandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	commandlist->DrawIndexedInstanced(Mesh.nindex, 1, Mesh.nioffset, Mesh.nOffset, 0);
}

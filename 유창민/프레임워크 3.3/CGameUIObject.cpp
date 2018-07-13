#include "CGameUIObject.h"

AimObject::AimObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 20.0f;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	MinDepth = true;

	//���Ӱ��� �����͵�
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

void AimObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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


	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
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
	//�ؽ�ó�� ����� 0 �̻��̸� ����
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["AimTex"].get()->Resource.Get());
	//���庯ȯ ������Ʈ �� ����
	UpdateConstBuffer(commandlist, false);

	//�ִϸ��̼��� ������ �ִϸ��̼Ƕ��� ����

	//�޽��� �����ؾ��ϴµ� ����Ʈ����Ʈ�� �����ؾ��ϹǷ� �׳� �̷��� ��.
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


CoolBarObject::CoolBarObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, float maxtime, CGameObject * master, XMFLOAT4 cp) :CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	MaxCoolTime = maxtime;
	ObjData.isAnimation = 0;
	ObjData.Scale = 80.0f;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 1;
	MinDepth = true;
	Master = master;
	ObjData.CustomData1.y = 0;
	//���Ӱ��� �����͵�
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

void CoolBarObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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


	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CoolBarTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);


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


SkillFrameUIObject::SkillFrameUIObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, int SkillNum, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 80.0f;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 3;
	MinDepth = true;
	//���Ӱ��� �����͵�
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

void SkillFrameUIObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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


	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
	for (int i = 0; i < numOfitem; ++i)
	{

		Mesh.SubResource[i].V = XMFLOAT3(0, 0, 0);

		Mesh.Index[i] = i;
	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void SkillFrameUIObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//�ؽ�ó�� ����� 0 �̻��̸� ����
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["SkillBG"].get()->Resource.Get());
	//���庯ȯ ������Ʈ �� ����
	UpdateConstBuffer(commandlist, false);

	//�ִϸ��̼��� ������ �ִϸ��̼Ƕ��� ����

	//�޽��� �����ؾ��ϴµ� ����Ʈ����Ʈ�� �����ؾ��ϹǷ� �׳� �̷��� ��.
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

SkillUIObject::SkillUIObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, int SkillNum, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 70.0f;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 3;
	MinDepth = true;
	//���Ӱ��� �����͵�
	gamedata.GodMode = true;
	staticobject = true;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill1", L"textures/ui/cube1.dds", false, 3, 0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill2", L"textures/ui/cube2.dds", false, 3, 1);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill3", L"textures/ui/tetrike.dds", false, 3, 2);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}

	if (SkillNum == 0)
		TextureName = "Skill1";
	else if (SkillNum == 1)
		TextureName = "Skill2";
	else if (SkillNum == 2)
		TextureName = "Skill3";
	else
		TextureName = "Skill1"; //���߿� ����

	TexOff = SkillNum;
}

void SkillUIObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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


	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
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
	//�ؽ�ó�� ����� 0 �̻��̸� ����
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), 0, TexOff);
	//���庯ȯ ������Ʈ �� ����
	UpdateConstBuffer(commandlist, false);

	//�ִϸ��̼��� ������ �ִϸ��̼Ƕ��� ����

	//�޽��� �����ؾ��ϴµ� ����Ʈ����Ʈ�� �����ؾ��ϹǷ� �׳� �̷��� ��.
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



SelectBarObject::SelectBarObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 80.0f;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 3;
	MinDepth = true;

	//���Ӱ��� �����͵�

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

void SelectBarObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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


	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["SelectTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);


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
BackGroundObject::BackGroundObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	//�̳༮�� �������� x ���̷� CustomData1.x�� 5�� �����ؼ� ���ϴ³༮���� �����ϰ� ���� CustomData1.y�� y���̷� �ϴ� �簢���̵ȴ�.

	ObjData.isAnimation = 0;
	ObjData.Scale = 800.0f;
	ObjData.CustomData1.y = 600;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 5;
	MinDepth = true;
	//���Ӱ��� �����͵�
	gamedata.GodMode = true;
	staticobject = true;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "StartBG", L"textures/ui/School.dds", false,2,0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "LoadBG", L"textures/ui/NowLoading.dds", false,2,1);
		TextureName = "StartBG";

		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void BackGroundObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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


	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
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
	//�ؽ�ó�� ����� 0 �̻��̸� ����
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), 0,TexOff);
	//���庯ȯ ������Ʈ �� ����
	UpdateConstBuffer(commandlist, false);

	//�ִϸ��̼��� ������ �ִϸ��̼Ƕ��� ����

	//�޽��� �����ؾ��ϴµ� ����Ʈ����Ʈ�� �����ؾ��ϹǷ� �׳� �̷��� ��.
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

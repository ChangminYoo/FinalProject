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


SkillFrameUIObject::SkillFrameUIObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, int SkillNum, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
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

SkillUIObject::SkillUIObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, int SkillNum, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
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

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill1", L"textures/ui/cube1.dds", false, 7, 0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill2", L"textures/ui/cube2.dds", false, 7, 1);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill3", L"textures/ui/tetrike.dds", false, 7, 2);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill4", L"textures/ui/dice.dds", false, 7, 3);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill5", L"textures/ui/padong.dds", false, 7, 4);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill6", L"textures/ui/shield.dds", false, 7, 5);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Skill7", L"textures/ui/hammer.dds", false, 7, 6);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}

	if (SkillNum == 0)
		TextureName = "Skill1";
	else if (SkillNum == 1)
		TextureName = "Skill2";
	else if (SkillNum == 2)
		TextureName = "Skill3";
	else if (SkillNum == 3)
		TextureName = "Skill4";
	else if (SkillNum == 4)
		TextureName = "Skill5";
	else if (SkillNum == 5)
		TextureName = "Skill6";
	else if (SkillNum == 6)
		TextureName = "Skill7";
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



SelectBarObject::SelectBarObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
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
BackGroundObject::BackGroundObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
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

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "StartBG", L"textures/ui/CharacterSelect.dds", false, 2, 0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "LoadBG", L"textures/ui/NowLoading.dds", false, 2, 1);
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

CharacterSelectObject::CharacterSelectObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{

	ObjData.isAnimation = 0;
	ObjData.Scale = 160.0f;
	ObjData.CustomData1.y = 200.0f;
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

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "CharacterSelect", L"textures/ui/CharacterSelectBar.dds", false);
		TextureName = "CharacterSelect";

		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
	CenterPos.x = -3000;
	CenterPos.y = -3000;
}

void CharacterSelectObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
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


void CharacterSelectObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//TexStart = ������ ����

	ObjData.TexClamp = XMFLOAT4(0.0f + (0.2f*TexStride), 0.2f + (0.2f*TexStride), 0, 0);

	//�ؽ�ó�� ����� 0 �̻��̸� ����
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get());
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



TimerObject1::TimerObject1(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 45.0f;
	ObjData.CustomData1.y = 45.0f;
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
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Time1", L"textures/ui/num.dds", false);
		TextureName = "Time1";
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void TimerObject1::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
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

void TimerObject1::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	ObjData.TexClamp = XMFLOAT4(0.0f + (0.1f*TexStride), 0.1f + (0.1f*TexStride), 0, 0);

	//�ؽ�ó�� ����� 0 �̻��̸� ����
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Time1"].get()->Resource.Get());
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

TimerObject2::TimerObject2(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 45.0f;
	ObjData.CustomData1.y = 45.0f;
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
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Time2", L"textures/ui/num.dds", false);
		TextureName = "Time2";
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void TimerObject2::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
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

void TimerObject2::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	ObjData.TexClamp = XMFLOAT4(0.0f + (0.1f*TexStride), 0.1f + (0.1f*TexStride), 0, 0);
	//�ؽ�ó�� ����� 0 �̻��̸� ����
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Time2"].get()->Resource.Get());
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

TimerObject3::TimerObject3(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 45.0f;
	ObjData.CustomData1.y = 45.0f;
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
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Time3", L"textures/ui/num.dds", false);
		TextureName = "Time3";
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void TimerObject3::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
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

void TimerObject3::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	ObjData.TexClamp = XMFLOAT4(0.0f + (0.1f*TexStride), 0.1f + (0.1f*TexStride), 0, 0);
	//�ؽ�ó�� ����� 0 �̻��̸� ����
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Time3"].get()->Resource.Get());
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


TitleObject::TitleObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 800.0f;
	ObjData.CustomData1.y = 600.0f;
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
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "TitleTex", L"textures/ui/title.dds", false);
		TextureName = "TitleTex";
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void TitleObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
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

void TitleObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	times += gt.DeltaTime();
	if (times >= 0.5f)
	{
		float t = 0.5f - times;
		TexStride += 1;
		times = t;
	}

	ObjData.TexClamp = XMFLOAT4(0.0f + (0.2f*TexStride), 0.2f + (0.2f*TexStride), 0, 0);


	//�ؽ�ó�� ����� 0 �̻��̸� ����
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["TitleTex"].get()->Resource.Get());
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

PointObject1::PointObject1(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{

	ObjData.isAnimation = 0;
	ObjData.Scale = 30.0f;
	ObjData.CustomData1.y = 30.0f;
	ObjData.SpecularParamater = 0.5f;
	ObjData.CustomData1.x = 5;
	MinDepth = true;

	gamedata.GodMode = true;
	staticobject = true;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Point1", L"textures/ui/num.dds", false);
		TextureName = "Point1";
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void PointObject1::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	Mesh.SubResource = new CVertex;
	Mesh.nVertex = 1;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT;
	Mesh.nindex = 1;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);


	Mesh.SubResource[0].V = XMFLOAT3(0, 0, 0);

	Mesh.Index[0] = 0;

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void PointObject1::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	ObjData.TexClamp = XMFLOAT4(0.0f + (0.1f*TexStride), 0.1f + (0.1f*TexStride), 0, 0);
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Point1"].get()->Resource.Get());

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

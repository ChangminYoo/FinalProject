#include "CGameObject.h"

extern UINT CbvSrvDescriptorSize;

CGameObject::CGameObject()
{
}


CGameObject::~CGameObject()
{
	if (ConstBuffer != NULL)
		delete ConstBuffer;
	if (DynamicVertexBuffer != NULL)
		delete DynamicVertexBuffer;
}

CGameObject::CGameObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist,bool animation,XMFLOAT4 cp)
{
	
	this->commandlist = commandlist;
	this->CenterPos = cp;//������ġ
	
	
	ThetaY = 0;

	

	SetWorldMatrix();	//���庯ȯ����

	
	this->Mesh = new CMesh;//�޽� ������� ����
	Mesh->Index = NULL;
	Mesh->SubResource = NULL;

	LoadTexture(m_Device, commandlist, "zombieTex", L"textures/zombie_diffuse.dds");

	SetMesh(m_Device,  commandlist);
	if (animation)
	{
		int a=sizeof(CVertex);
		//DynamicVertexBuffer = new UploadBuffer<CVertex>(m_Device, Mesh->nVertex, false);//�ִϸ��̼ǻ���Ҷ� �̰� ����
		
	}
	CreateConstBuffer(m_Device);//������ۻ���

	
}

void CGameObject::SetWorldMatrix()
{
	auto wmatrix = XMMatrixIdentity();
	auto pos = XMLoadFloat4(&CenterPos);
	wmatrix *= XMMatrixRotationY(ThetaY);
	wmatrix *= XMMatrixTranslationFromVector(pos);
	XMStoreFloat4x4(&ObjData.WorldMatrix, wmatrix);
}

void CGameObject::Render(ID3D12GraphicsCommandList * commandlist)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. ��������� ������Ʈ �ѵ� �����Ѵ��� �޽����� Drawindexedinstanceȣ���ϵ�����
	SetTexture(commandlist);
	UpdateConstBuffer(commandlist);
	
	if (DynamicVertexBuffer != NULL)
		Mesh->Render(commandlist, true, DynamicVertexBuffer->Resource());
	else
		Mesh->Render(commandlist);
}

void CGameObject::Tick(const GameTimer& gt)
{
	//ThetaY += 10 * gt.DeltaTime();
	//if (ThetaY > 360)
	//	ThetaY = 0;

	if (DynamicVertexBuffer != NULL)
	{
		if (TickValue >1)
		{
			TickValue = 0;

		}
		TickValue += 1;
	}
			UpdateMD5Model(commandlist, Mesh, this, gt.DeltaTime()*8, 0, animations, 0.15);

}

void CGameObject::CreateConstBuffer(ID3D12Device * m_Device)
{
	//������۸� ��Ʈ�Ķ���ͷ� Descriptor�� ����� ������� �並 ���鶧 ���� �ȸ��� �ȴ�.
	//������۸� ����� ���
	//UploadBuffer<����������>* buffer=new UploadBuffer<����������>(Device,1,true);//������ true�� ���� �����ϰ�� false��
	//��Ʈ�ñ״�ó �����׸����� ����(��Ŭ������ �ִ�.)
	
	//CD3DX12_ROOT_PARAMETER RootParameter[1];
	//RootParameter[0].InitAsConstantBufferView(0);
	//�̷��� �Ķ���͸� �����ؼ� �����. ����
	//UpdateConstBuffer���� CopyData�� ���ش���
	//commandlist->SetGraphicsRootConstantBufferView(��������ε���,GetGPUVIRTUALADDRESS()); �̷��� ���ָ鳡


	ConstBuffer = new UploadBuffer<ObjectData>(m_Device, 1, true);
	
}

void CGameObject::UpdateConstBuffer(ID3D12GraphicsCommandList * commandlist)
{
	SetWorldMatrix();//���� �����ǰ� �����̼������� ��������� �����.

	// �� �͵��� ī�޶� Ŭ������ ���� ó���ؾ��մϴ�.
	XMVECTOR pos = XMVectorSet(0, 183.5, -223.5, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMFLOAT4X4 mView;
	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);
	
	XMMATRIX world = XMLoadFloat4x4(&ObjData.WorldMatrix);
	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f*3.14, 800.0/600.0, 1.0f, 1000.0f);
	XMMATRIX worldViewProj = world * view*proj;


	XMStoreFloat4x4(&ObjData.WorldMatrix, XMMatrixTranspose(worldViewProj));
	ObjData.Scale = 0.5;
	//������� ������Ʈ
	ConstBuffer->CopyData(0, ObjData);


	//Ŀ�ǵ帮��Ʈ�� ������� ����
	
	commandlist->SetGraphicsRootConstantBufferView(1, ConstBuffer->Resource()->GetGPUVirtualAddress());//������Ŀ���
	commandlist->SetGraphicsRootConstantBufferView(2, Mesh->jarr->Resource()->GetGPUVirtualAddress());//����Ʈ�迭����
}

void CGameObject::SetMesh(ID3D12Device * m_Device,ID3D12GraphicsCommandList* commandlist)
{
	/* �׽�Ʈ�� �ﰢ��
	Mesh->SubResource = new CVertex[3];
	XMStoreFloat3(&Mesh->SubResource[0].V, XMVectorSet(0, 0.5, 1, 1));
	XMStoreFloat3(&Mesh->SubResource[1].V, XMVectorSet(0.5, -0.5, -1, 1));
	XMStoreFloat3(&Mesh->SubResource[2].V, XMVectorSet(-0.5, -0.5, -1, 1));

	Mesh->nOffset = 0;
	Mesh->nStride = sizeof(CVertex);
	Mesh->nVertex = 3;

	Mesh->Index = new UINT[3];
	Mesh->Index[0] = 0;
	Mesh->Index[1] = 1;
	Mesh->Index[2] = 2;

	Mesh->nindex = 3;
	Mesh->nioffset = 0;
	Mesh->nisize = sizeof(UINT);
	
	*/
	Mesh->jarr = new UploadBuffer<JointArr>(m_Device, 1, true);
	LoadMD5Model(L"dietzombie2.MD5MESH", Mesh, 0,1, 0.4);
	Mesh->CreateVertexBuffer(m_Device,commandlist);
	Mesh->CreateIndexBuffer(m_Device,commandlist);
	
	//�ִϸ��̼� �ε�
	LoadMD5Anim(m_Device, L"attack1.MD5ANIM", Mesh, this, animations);
}

void CGameObject::SetTexture(ID3D12GraphicsCommandList * commandlist)
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvDescriptorHeap.Get() };
	commandlist->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(0, CbvSrvDescriptorSize);

	commandlist->SetGraphicsRootDescriptorTable(0, tex);
}

void CGameObject::LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist,string texturename,wstring FileName)
{
	auto zombieTex = make_unique<CTexture>();
	zombieTex->Name = texturename;
	zombieTex->Filename = FileName;

	ThrowIfFailed(CreateDDSTextureFromFile12(device,
		commandlist, zombieTex->Filename.c_str(),
		zombieTex->Resource, zombieTex->UploadHeap));

	Textures[zombieTex->Name] = move(zombieTex);


	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&SrvDescriptorHeap)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	auto Tex = Textures[texturename]->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = Tex->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = Tex->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	device->CreateShaderResourceView(Tex.Get(), &srvDesc, hDescriptor);

}

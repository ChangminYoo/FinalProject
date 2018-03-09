#include "CGameObject.h"

extern UINT CbvSrvDescriptorSize;


CGameObject::CGameObject()
{
	
}


CGameObject::~CGameObject()
{
	if (ConstBuffer != NULL)
		delete ConstBuffer;

	if (pp != NULL)
		delete pp;
	if (jarr != NULL)
		delete jarr;
	
}

XMFLOAT3 CGameObject::GetUpvector()
{
	XMVECTOR l = XMLoadFloat3(&Lookvector);
	XMVECTOR r = XMLoadFloat3(&Rightvector);
	auto u=XMVector3Cross(l, r);

	XMFLOAT3 up;
	XMStoreFloat3(&up, u);
	up = Float3Normalize(up);
	return up;
}

CGameObject::CGameObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist,XMFLOAT4 cp)
{
	
	//���⼭�� �⺻���ΰ͵鸸 ó���Ѵ�. ��ġ�� ȸ���� �� �ʱ�ȭ
	this->commandlist = commandlist;
	this->CenterPos = cp;//������ġ
	ThetaY = 0;
	SetWorldMatrix();	//���庯ȯ����
	CreateConstBuffer(m_Device);//������ۻ���
}

void CGameObject::SetWorldMatrix()
{
	auto wmatrix = XMMatrixIdentity();
	auto pos = XMLoadFloat4(&CenterPos);
	wmatrix *= XMMatrixRotationY(ThetaY*MMPE_PI/180);
	wmatrix *= XMMatrixTranslationFromVector(pos);
	XMStoreFloat4x4(&ObjData.WorldMatrix, wmatrix);
}

void CGameObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	
}

void CGameObject::UpdatePPosCenterPos()
{
	CenterPos = XMFloat3to4(pp->GetPosition());
}

void CGameObject::UpdateLookVector()
{
	auto wmatrix = XMMatrixIdentity();
	wmatrix *= XMMatrixRotationY(ThetaY*MMPE_PI/180);
	auto ol=XMLoadFloat3(&OffLookvector);
	auto or = XMLoadFloat3(&OffRightvector);

	ol=XMVector4Transform(ol, wmatrix);
	or= XMVector4Transform(or, wmatrix);
	
	XMStoreFloat3(&Lookvector, ol);
	XMStoreFloat3(&Rightvector, or);

	Lookvector = Float3Normalize(Lookvector);
	Rightvector = Float3Normalize(Rightvector);
}

void CGameObject::SetAnimation(int n_Ani)
{
	if (n_Ani != n_Animation)
	{
		
		currAnimTime = 0;

	}
	n_Animation = n_Ani;
	
}

void CGameObject::Tick(const GameTimer& gt)
{

	
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
	//�׻� ȣ���Ұ�. �躤�Ϳ� ����Ʈ���͸� ������Ʈ���ش�.
	UpdateLookVector();
	UpdatePPosCenterPos();//������ �������� ������� ������. �ݵ�� �浹�ϰų� �ϸ� ó���ؾ���
	SetWorldMatrix();//���� �����ǰ� �����̼������� ��������� �����.

	XMMATRIX world = XMLoadFloat4x4(&ObjData.WorldMatrix);

	XMStoreFloat4x4(&ObjData.WorldMatrix, XMMatrixTranspose(world));

	
	//������� ������Ʈ
	ConstBuffer->CopyData(0, ObjData);

	commandlist->SetGraphicsRootConstantBufferView(1, ConstBuffer->Resource()->GetGPUVirtualAddress());//������Ŀ���
}




//---------------------------------------------------------
// DinamicObject
//---------------------------------------------------------

CCubeManObject::CCubeManObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, XMFLOAT4 cp) : CGameObject(m_Device,commandlist,cp)
{

	//���ӿ�����Ʈ �����ڿ��� �⺻���ΰ��� ó�������Ƿ� ���⼭��
	//�޽��� �ؽ�ó ���� �ҷ������ �ִϸ��̼� �ε���� ó���ؾ��Ѵ�.
	
	//����Ʈ�� ����� �迭.
	jarr = new UploadBuffer<JointArr>(m_Device, 1, true);

	if (CreateMesh == false)
	{
		
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
	
		LoadTexture(m_Device, commandlist,this, Textures,SrvDescriptorHeap,"ZombieTex", L"textures/human/Male White Wizard 05 Red.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;
		
	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, -1);
	OffRightvector = XMFLOAT3(-1, 0, 0);
	UpdateLookVector();
	ThetaY = 180;
	ObjData.isAnimation = true;
	ObjData.Scale =3;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	Speed = 60;
	

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(4, 0, 0);
	XMFLOAT3 ry(0, 9, 0);
	XMFLOAT3 rz(0, 0, 4);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(4, 9, 4);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.

	

}


void CCubeManObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{

	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\cIdle.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(true);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

	//�ִϸ��̼� �ε�
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cIdle.MD5ANIM",&Mesh, this, animations);//0
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cRunning.MD5ANIM", &Mesh, this, animations);//1
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cAttack.MD5ANIM", &Mesh, this, animations);//2
	
}

void CCubeManObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void CCubeManObject::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.
	pp->integrate(gt.DeltaTime(), &CenterPos);
	
	if (ObjData.isAnimation == true)
	{
		if (TickValue > 1)
		{
			//�ִϸ��̼� ������Ʈ
			UpdateMD5Model(commandlist, &Mesh, this, gt.DeltaTime() * 8, n_Animation, animations, jarr);

			TickValue = 0;
		}
	}
	TickValue += 1;

	
	
}

void CCubeManObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if(Texturing)
		SetTexture(commandlist,SrvDescriptorHeap);
	UpdateConstBuffer(commandlist);
	
	Mat.UpdateConstantBuffer(commandlist);

	//ƽ�Լ����� ������Ʈ�� �ִϸ��̼ǵ� ����Ʈ�� ������.
	commandlist->SetGraphicsRootConstantBufferView(2, jarr->Resource()->GetGPUVirtualAddress());
	//���� �׸���.

	Mesh.Render(commandlist);

}

//�浹��. �浹����� �浹�ؼҸ� �ô´�.
void CCubeManObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this)
		{
			
			bool test=pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector,(*i)->GetUpvector());

			if (test)//�浹������ �浹�ؼҸ� �ؾ��Ѵ�.
			{

				//�浹 ������ ���� (0,1,0) �̸� Airbone�� false�� �д�. �̴� ���� ���������� ��Ÿ����.
				if (pp->pAxis.y == 1)
					AirBone = false;
				//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
				//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
				if (pp->pAxis.y == -1)
					(*i)->AirBone = false;


				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);
				
					
					
				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{

				}
				
				

				//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime,false);//����� ƨ���� �ʴ´�.
				UpdatePPosCenterPos();
				(*i)->UpdatePPosCenterPos();
			}
		}
	}
}

void CCubeManObject::EndAnimation(int nAni)
{
	
	if (nAni == (int)Ani_State::Attack )//�����ϱ⿴����
	{
		SetAnimation((int)Ani_State::Idle);//�����·εд�.

	}
	
}



CZombieObject::CZombieObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, cp)
{

	//���ӿ�����Ʈ �����ڿ��� �⺻���ΰ��� ó�������Ƿ� ���⼭��
	//�޽��� �ؽ�ó ���� �ҷ������ �ִϸ��̼� �ε���� ó���ؾ��Ѵ�.

	//����Ʈ�� ����� �迭.
	jarr = new UploadBuffer<JointArr>(m_Device, 1, true);

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "ZombieTex", L"textures/zombie_diffuse.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, -1);
	OffRightvector = XMFLOAT3(-1, 0, 0);
	UpdateLookVector();
	ThetaY = 180;
	ObjData.isAnimation = true;
	ObjData.Scale = 0.1;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	Speed = 60;


	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(4, 0, 0);
	XMFLOAT3 ry(0, 8, 0);
	XMFLOAT3 rz(0, 0, 4);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(4,8,4);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.



}


void CZombieObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{

	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\dietzombie2.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

	//�ִϸ��̼� �ε�
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\attack1.MD5ANIM", &Mesh, this, animations);//0
	
}

void CZombieObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void CZombieObject::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.
	pp->integrate(gt.DeltaTime(), &CenterPos);

	if (ObjData.isAnimation == true)
	{
		if (TickValue > 1)
		{
			//�ִϸ��̼� ������Ʈ
			UpdateMD5Model(commandlist, &Mesh, this, gt.DeltaTime() * 8, n_Animation, animations, jarr);

			TickValue = 0;
		}
	}
	TickValue += 1;


}

void CZombieObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Texturing)
		SetTexture(commandlist, SrvDescriptorHeap);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//ƽ�Լ����� ������Ʈ�� �ִϸ��̼ǵ� ����Ʈ�� ������.
	commandlist->SetGraphicsRootConstantBufferView(2, jarr->Resource()->GetGPUVirtualAddress());
	//���� �׸���.

	Mesh.Render(commandlist);

}

//�浹��. �浹����� �浹�ؼҸ� �ô´�.
void CZombieObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this)
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ �浹�ؼҸ� �ؾ��Ѵ�.
			{

				//�浹 ������ ���� (0,1,0) �̸� Airbone�� false�� �д�. �̴� ���� ���������� ��Ÿ����.
				if (pp->pAxis.y == 1)
					AirBone = false;
				//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
				//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
				if (pp->pAxis.y == -1)
					(*i)->AirBone = false;


				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);



				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{

				}



				//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime, false);//����� ƨ���� �ʴ´�.
				UpdatePPosCenterPos();
				(*i)->UpdatePPosCenterPos();
			}
		}
	}
}

void CZombieObject::EndAnimation(int nAni)
{

	if (nAni == (int)Ani_State::Attack)//�����ϱ⿴����
	{
		SetAnimation((int)Ani_State::Idle);//�����·εд�.

	}

}

void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap)
{
	//�ؽ�ó�� ���̺��� �����̹Ƿ� ���� ���̺� �ΰ��� �����ؾ���.
	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvDescriptorHeap.Get() };
	commandlist->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(0, CbvSrvDescriptorSize);

	commandlist->SetGraphicsRootDescriptorTable(0, tex);
}

void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist,CGameObject* obj, unordered_map<string, unique_ptr<CTexture>>& Textures, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, string texturename, wstring FileName, bool isTexCube)
{
	obj->Texturing = true;
	auto Tex = make_unique<CTexture>();
	Tex->Name = texturename;
	Tex->Filename = FileName;

	ThrowIfFailed(CreateDDSTextureFromFile12(device,
		commandlist, Tex->Filename.c_str(),
		Tex->Resource, Tex->UploadHeap));

	Textures[Tex->Name] = move(Tex);


	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&SrvDescriptorHeap)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	auto Texs = Textures[texturename]->Resource;

	
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = Texs->GetDesc().Format;

		if (isTexCube) 
		{srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;}
		else 
		{srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;}

		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = Texs->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

		device->CreateShaderResourceView(Texs.Get(), &srvDesc, hDescriptor);

}




//---------------------------------------------------------
// StaticObject
//---------------------------------------------------------


CSphereObject::CSphereObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, cp)
{

	if (CreateMesh == false)
	{
		SetValue(0.5, 20, 20);

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "BrickTex", L"textures/bricks.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.

	OffLookvector = XMFLOAT3(0, 0, -1);
	OffRightvector = XMFLOAT3(-1, 0, 0);
	UpdateLookVector();
	ThetaY = 180;
	ObjData.isAnimation = false;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	Speed = 0;

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(0,0,0);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.


}

void CSphereObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;	
	Mesh.nVertex = ((stack - 2)* slice) + 2;

	int NumSphereVertices = Mesh.nVertex;
	int NumSphereFaces = ((stack - 3)*(slice) * 2) + (slice * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	if (Mesh.SubResource == NULL)
		Mesh.SubResource = new CVertex[NumSphereVertices];


	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMMATRIX Rotationx;
	XMMATRIX Rotationy;

	Mesh.SubResource[0].V.x = 0.0f;
	Mesh.SubResource[0].V.y = 0.0f;
	Mesh.SubResource[0].V.z = 1.0f;

	for (DWORD i = 0; i < stack - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (stack - 1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < slice; ++j)
		{
			sphereYaw = j * (6.28 / (slice));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			Mesh.SubResource[i*slice + j + 1].V.x = XMVectorGetX(currVertPos);
			Mesh.SubResource[i*slice + j + 1].V.y = XMVectorGetY(currVertPos);
			Mesh.SubResource[i*slice + j + 1].V.z = XMVectorGetZ(currVertPos);
		}
	}

	Mesh.SubResource[NumSphereVertices - 1].V.x = 0.0f;
	Mesh.SubResource[NumSphereVertices - 1].V.y = 0.0f;
	Mesh.SubResource[NumSphereVertices - 1].V.z = -1.0f;

	Mesh.nindex = NumSphereFaces * 3;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);

	if (Mesh.Index == NULL)
		Mesh.Index = new UINT[Mesh.nindex];

	int k = 0;
	for (DWORD l = 0; l < slice - 1; ++l)
	{
		Mesh.Index[k] = 0;
		Mesh.Index[k + 1] = l + 1;
		Mesh.Index[k + 2] = l + 2;
		k += 3;
	}

	Mesh.Index[k] = 0;
	Mesh.Index[k + 1] = slice;
	Mesh.Index[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < stack - 3; ++i)
	{
		for (DWORD j = 0; j < slice - 1; ++j)
		{
			Mesh.Index[k] = i*slice + j + 1;
			Mesh.Index[k + 1] = i*slice + j + 2;
			Mesh.Index[k + 2] = (i + 1)*slice + j + 1;
			Mesh.Index[k + 3] = (i + 1)*slice + j + 1;
			Mesh.Index[k + 4] = i*slice + j + 2;
			Mesh.Index[k + 5] = (i + 1)*slice + j + 2;

			k += 6; // next quad
		}

		Mesh.Index[k] = (i*slice) + slice;
		Mesh.Index[k + 1] = (i*slice) + 1;
		Mesh.Index[k + 2] = ((i + 1)*slice) + slice;
		Mesh.Index[k + 3] = ((i + 1)*slice) + slice;
		Mesh.Index[k + 4] = (i*slice) + 1;
		Mesh.Index[k + 5] = ((i + 1)*slice) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < slice - 1; ++l)
	{
		Mesh.Index[k] = NumSphereVertices - 1;
		Mesh.Index[k + 1] = (NumSphereVertices - 1) - (l + 1);
		Mesh.Index[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	Mesh.Index[k] = NumSphereVertices - 1;
	Mesh.Index[k + 1] = (NumSphereVertices - 1) - slice;
	Mesh.Index[k + 2] = NumSphereVertices - 2;



	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);


}

void CSphereObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);

	Mat.MatData.Roughness = 0.1f;
}

void CSphereObject::Tick(const GameTimer & gt)
{
}

void CSphereObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Texturing)
		SetTexture(commandlist, SrvDescriptorHeap);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	Mesh.Render(commandlist);

}

void CSphereObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
}

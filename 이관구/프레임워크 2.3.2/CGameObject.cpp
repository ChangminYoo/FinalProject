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
	if (rb != NULL)
		delete rb;
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
	XMStoreFloat4(&Orient, XMQuaternionIdentity());//������ �ʱ�ȭ �Ѵ�.
	SetWorldMatrix();	//���庯ȯ����
	CreateConstBuffer(m_Device);//������ۻ���
}

void CGameObject::SetWorldMatrix()
{
	auto wmatrix = XMMatrixIdentity();
	auto pos = XMLoadFloat4(&CenterPos);
	auto quater = XMLoadFloat4(&Orient);
	wmatrix *= XMMatrixRotationQuaternion(quater);
	wmatrix *= XMMatrixTranslationFromVector(pos);
	XMStoreFloat4x4(&ObjData.WorldMatrix, wmatrix);
}

void CGameObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	
}

void CGameObject::ToDamage(float Damage)
{
	if (gamedata.GodMode == false)
	{
		gamedata.HP -= fabsf(Damage);
		if (gamedata.HP <= 0)
			DelObj = true;
	}
}

void CGameObject::UpdatePPosCenterPos()
{
	CenterPos = XMFloat3to4(pp->GetPosition());
}

void CGameObject::UpdateLookVector()
{
	auto wmatrix = XMMatrixIdentity();
	auto quater = XMLoadFloat4(&Orient);
	wmatrix *= XMMatrixRotationQuaternion(quater);
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
	if (pp != NULL && rb==NULL)
		UpdatePPosCenterPos();//������ �������� ������� ������. �ݵ�� �浹�ϰų� �ϸ� ó���ؾ���
	SetWorldMatrix();//���� �����ǰ� �����̼������� ��������� �����.

	XMMATRIX world = XMLoadFloat4x4(&ObjData.WorldMatrix);

	XMStoreFloat4x4(&ObjData.WorldMatrix, XMMatrixTranspose(world));

	
	//������� ������Ʈ
	ConstBuffer->CopyData(0, ObjData);

	commandlist->SetGraphicsRootConstantBufferView(2, ConstBuffer->Resource()->GetGPUVirtualAddress());//������Ŀ���
}

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
	
		LoadTexture(m_Device, commandlist,this, Textures,SrvDescriptorHeap,"ZombieTex", L"textures/human/Male White Wizard 05 Red.dds",false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;
		
	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, -1);
	OffRightvector = XMFLOAT3(-1, 0, 0);
	auto q = XMLoadFloat4(&Orient);//������ 180�� ������ �Ѵ�.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, MMPE_PI);
	Orient = QuaternionMultiply(Orient, q2);
	UpdateLookVector();
	ObjData.isAnimation = true;
	ObjData.Scale =3;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	
	//���� ������ (����)�� ��´�. ĳ���ʹ� �������� ���� �ʰ�, źȯ�� �������� ����������.
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Speed = 60;
	
	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(3, 0, 0);
	XMFLOAT3 ry(0, 9, 0);
	XMFLOAT3 rz(0, 0, 3);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(3, 9, 3);//�浹 �ڽ��� x,y,z ũ��
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

	

	if(Textures.size()>0)
		SetTexture(commandlist,SrvDescriptorHeap,false);
	UpdateConstBuffer(commandlist);
	
	Mat.UpdateConstantBuffer(commandlist);

	//ƽ�Լ����� ������Ʈ�� �ִϸ��̼ǵ� ����Ʈ�� ������.
	commandlist->SetGraphicsRootConstantBufferView(3, jarr->Resource()->GetGPUVirtualAddress());
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
				if (pp->pAxis.y >0)
					AirBone = false;
				//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
				//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
				if (pp->pAxis.y <0)
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
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime);//����� ƨ���� �ʴ´�.
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

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "ZombieTex", L"textures/human/Male White Wizard 05 Red.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, -1);
	OffRightvector = XMFLOAT3(-1, 0, 0);
	auto q = XMLoadFloat4(&Orient);//������ 180�� ������ �Ѵ�.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, MMPE_PI);
	Orient = QuaternionMultiply(Orient, q2);


	UpdateLookVector();
	
	ObjData.isAnimation = true;
	ObjData.Scale =0.1;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Speed = 60;

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



	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//ƽ�Լ����� ������Ʈ�� �ִϸ��̼ǵ� ����Ʈ�� ������.
	commandlist->SetGraphicsRootConstantBufferView(3, jarr->Resource()->GetGPUVirtualAddress());
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
				if (pp->pAxis.y >0)
					AirBone = false;
				//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
				//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
				if (pp->pAxis.y <0)
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
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime);//����� ƨ���� �ʴ´�.
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


//---------------------------------------------------------------------------------------------------------------------------------

void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, bool isCubeMap)
{
	//�ؽ�ó�� ���̺��� �����̹Ƿ� ���� ���̺� �ΰ��� �����ؾ���.
	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvDescriptorHeap.Get() };
	commandlist->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	CD3DX12_GPU_DESCRIPTOR_HANDLE Skytex(SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	Skytex.Offset(0, CbvSrvDescriptorSize);

	if(isCubeMap)
		commandlist->SetGraphicsRootDescriptorTable(0, Skytex);
	else
		commandlist->SetGraphicsRootDescriptorTable(1, SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

}

void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist,CGameObject* obj, unordered_map<string, unique_ptr<CTexture>>& Textures, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, string texturename, wstring FileName, bool isCubeMap)
{
	
	auto Tex = make_unique<CTexture>();
	Tex->Name = texturename;
	Tex->Filename = FileName;

	ThrowIfFailed(CreateDDSTextureFromFile12(device,
		commandlist, Tex->Filename.c_str(),
		Tex->Resource, Tex->UploadHeap));

	Textures[Tex->Name] = move(Tex);


	//��ũ���� �� ����
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&SrvDescriptorHeap)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	auto Texs = Textures[texturename]->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	if (isCubeMap)
	{
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = Texs->GetDesc().MipLevels;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		srvDesc.Format = Texs->GetDesc().Format;
	}
	else
	{
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = Texs->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		srvDesc.Format = Texs->GetDesc().Format;
	}


	device->CreateShaderResourceView(Texs.Get(), &srvDesc, hDescriptor);
}


//---------------------------------------------------------------------------------------------------------------------------------


//------------------- �� �� ü -----------------------//

BulletCube::BulletCube(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist,CGameObject* master,XMFLOAT4& ori,CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "ZombieTex", L"textures/human/Male White Wizard 05 Red.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	//���ڷ� �߻�������� �躤�Ͱ� �������� ȸ���� ori���� �ް�, ���� ����(������ 0,0,0,1)�� �����ش�.
	Orient = QuaternionMultiply(Orient, ori);

	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 50;
	gamedata.GodMode = true;
	gamedata.Speed = 50;
	LifeTime = 10;
	Master = master;
	LockOn = lockon;

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(1, 1, 1);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//�躤�ͷ� ���ư�


}


void BulletCube::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{

	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\Cube.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

	
}

void BulletCube::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void BulletCube::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No�ִϸ��̼�!

	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	LifeTime -= gt.DeltaTime();
	if (LifeTime <= 0)
		DelObj = true;

}

void BulletCube::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap,false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}

//�浹��. �浹����� �浹�ؼҸ� �ô´�.
void BulletCube::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{
		
		if (*i != this && *i!=Master && (*i)->pp!=NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				(*i)->ToDamage(gamedata.Damage);
							
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

				//�浹�� �ӵ��� �����.
				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);
				//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
				DelObj = true;
				
			}
		}
	}
}


//---------------------- ����ƽ ������Ʈ -----------------------------//

SphereObject::SphereObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "MapTex", L"textures/sky/snowcube1024.dds",true);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

	//���Ӱ��� �����͵�
	gamedata.MAXHP = 100000;
	gamedata.HP = 100000;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;


	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(0, 0, 0);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//�躤�ͷ� ���ư�

}


void SphereObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\sphere.MD5MESH", &Mesh, 0, 1024);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void SphereObject::Tick(const GameTimer & gt)
{
}

void SphereObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, true);
	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}

void SphereObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
}




CubeObject::CubeObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "BoxTex", L"textures/WoodCrate.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.


	//���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;


	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(5, 0, 0);
	XMFLOAT3 ry(0, 5, 0);
	XMFLOAT3 rz(0, 0, 5);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(5, 5, 5);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(3);//������ ��ü�� ���԰� �����̴�.
	
	//�׽�Ʈ�� ������ �ٵ�
	rb = new RigidBody();
	rb->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	rb->SetHalfBox(5, 5, 5);//�浹 �ڽ��� x,y,z ũ��
	rb->SetDamping(0.8f,0.8f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	rb->SetBounce(false);//ƨ���� �ʴ´�.
	rb->SetMass(5);//������ ��ü�� ���԰� �����̴�.
	rb->SetIMoment(5, 5, 5);
	rb->SetOrient(&Orient);
	XMFLOAT3 testForce{ 0,0,1000 };
	XMFLOAT3 testPoint{ -15,0,-5 };
	rb->AddForcePoint(testForce,testPoint);
}

void CubeObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 1);

	//�� �ε�
	//LoadMD5Model(L".\\�÷��̾�޽���\\Cube.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void CubeObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void CubeObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//�׽�Ʈ�� ������ �ٵ� �ڵ�.
	rb->integrate(gt.DeltaTime());

	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap,false);
	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}

void CubeObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
}

void CreateCube(CMesh * Mesh, float size)
{
	float half = 0.5 * size;

	Mesh->SubResource = new CVertex[24];
	Mesh->nVertex = 24;
	Mesh->nStride = sizeof(CVertex);
	Mesh->nOffset = 0;

	//front
	Mesh->SubResource[0].V = XMFLOAT3(-half, -half, -half);
	Mesh->SubResource[1].V = XMFLOAT3(-half, +half, -half);
	Mesh->SubResource[2].V = XMFLOAT3(+half, +half, -half);
	Mesh->SubResource[3].V = XMFLOAT3(+half, -half, -half);
	
	Mesh->SubResource[0].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[1].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[2].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[3].Tex = XMFLOAT2(1, 1);
	
	//back
	Mesh->SubResource[4].V = XMFLOAT3(-half, -half, +half);
	Mesh->SubResource[5].V = XMFLOAT3(+half, -half, +half);
	Mesh->SubResource[6].V = XMFLOAT3(+half, +half, +half);
	Mesh->SubResource[7].V = XMFLOAT3(-half, +half, +half);

	Mesh->SubResource[4].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[5].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[6].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[7].Tex = XMFLOAT2(1, 1);

	//top
	Mesh->SubResource[8].V = XMFLOAT3(-half, +half, -half);
	Mesh->SubResource[9].V = XMFLOAT3(-half, +half, +half);
	Mesh->SubResource[10].V = XMFLOAT3(+half, +half, +half);
	Mesh->SubResource[11].V = XMFLOAT3(+half, +half, -half);

	Mesh->SubResource[8].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[9].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[10].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[11].Tex = XMFLOAT2(1, 1);


	//bottom
	Mesh->SubResource[12].V = XMFLOAT3(-half, -half, -half);
	Mesh->SubResource[13].V = XMFLOAT3(+half, -half, -half);
	Mesh->SubResource[14].V = XMFLOAT3(+half, -half, +half);
	Mesh->SubResource[15].V = XMFLOAT3(-half, -half, +half);

	Mesh->SubResource[12].Tex = XMFLOAT2(1, 1);
	Mesh->SubResource[13].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[14].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[15].Tex = XMFLOAT2(1, 0);

	//left
	Mesh->SubResource[16].V = XMFLOAT3(-half, -half, +half);
	Mesh->SubResource[17].V = XMFLOAT3(-half, +half, +half);
	Mesh->SubResource[18].V = XMFLOAT3(-half, +half, -half);
	Mesh->SubResource[19].V = XMFLOAT3(-half, -half, -half);
	
	Mesh->SubResource[16].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[17].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[18].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[19].Tex = XMFLOAT2(1, 1);

	//right
	Mesh->SubResource[20].V = XMFLOAT3(+half, -half, -half);
	Mesh->SubResource[21].V = XMFLOAT3(+half, +half, -half);
	Mesh->SubResource[22].V = XMFLOAT3(+half, +half, +half);
	Mesh->SubResource[23].V = XMFLOAT3(+half, -half, +half);

	Mesh->SubResource[20].Tex = XMFLOAT2(0, 1);
	Mesh->SubResource[21].Tex = XMFLOAT2(0, 0);
	Mesh->SubResource[22].Tex = XMFLOAT2(1, 0);
	Mesh->SubResource[23].Tex = XMFLOAT2(1, 1);

	Mesh->Index = new UINT[36];
	Mesh->nindex = 36;
	Mesh->nioffset = 0;
	Mesh->nisize = sizeof(UINT);

	Mesh->Index[0] = 0; Mesh->Index[1] = 1; Mesh->Index[2] = 2;
	Mesh->Index[3] = 0; Mesh->Index[4] = 2; Mesh->Index[5] = 3;

	Mesh->Index[6] = 4; Mesh->Index[7] = 5; Mesh->Index[8] = 6;
	Mesh->Index[9] = 4; Mesh->Index[10] = 6; Mesh->Index[11] = 7;

	Mesh->Index[12] = 8; Mesh->Index[13] = 9; Mesh->Index[14] = 10;
	Mesh->Index[15] = 8; Mesh->Index[16] = 10; Mesh->Index[17] = 11;

	Mesh->Index[18] = 12; Mesh->Index[19] = 13; Mesh->Index[20] = 14;
	Mesh->Index[21] = 12; Mesh->Index[22] = 14; Mesh->Index[23] = 15;
						 					  
	Mesh->Index[24] = 16; Mesh->Index[25] = 17; Mesh->Index[26] = 18;
	Mesh->Index[27] = 16; Mesh->Index[28] = 18; Mesh->Index[29] = 19;
						 					  
	Mesh->Index[30] = 20; Mesh->Index[31] = 21; Mesh->Index[32] = 22;
	Mesh->Index[33] = 20; Mesh->Index[34] = 22; Mesh->Index[35] = 23;


}


void CreateGrid(CMesh* Mesh, float width, float depth)
{
	int m = 301; int n = 301;
	UINT vertexCount = m*n;
	UINT faceCount = (m - 1)*(n - 1) * 2;

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);


	Mesh->SubResource = new CVertex[vertexCount];
	Mesh->nVertex = vertexCount;
	Mesh->nStride = sizeof(CVertex);
	Mesh->nOffset = 0;

	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			Mesh->SubResource[i*n + j].V = XMFLOAT3(x, 0.0f, z);

			// Stretch texture over grid.
			Mesh->SubResource[i*n + j].Tex = XMFLOAT2(j*du, i*dv);
		}
	}

	Mesh->Index = new UINT[faceCount*3];
	Mesh->nindex = faceCount*3;
	Mesh->nioffset = 0;
	Mesh->nisize = sizeof(UINT);

	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			Mesh->Index[k] = i*n + j;
			Mesh->Index[k + 1] = i*n + j + 1;
			Mesh->Index[k + 2] = (i + 1)*n + j;

			Mesh->Index[k + 3] = (i + 1)*n + j;
			Mesh->Index[k + 4] = i*n + j + 1;
			Mesh->Index[k + 5] = (i + 1)*n + j + 1;

			k += 6; // next quad
		}
	}

}
#include "CGameObject.h"

extern UINT CbvSrvDescriptorSize;

short CGameObject::myID = -1;
list<short> CGameObject::BulletIDList = list<short>();

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
	auto u = XMVector3Cross(l, r);

	XMFLOAT3 up;
	XMStoreFloat3(&up, u);
	up = Float3Normalize(up);
	return up;
}

CGameObject::CGameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp)
{

	//���⼭�� �⺻���ΰ͵鸸 ó���Ѵ�. ��ġ�� ȸ���� �� �ʱ�ȭ
	this->device = m_Device;
	this->commandlist = commandlist;
	this->CenterPos = cp;//������ġ
	this->ParticleList = Plist;

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
			ToDead();
	}
}

void CGameObject::ToDead()
{
	DelObj = true;
}

void CGameObject::UpdatePPosCenterPos()
{
	CenterPos = XMFloat3to4(pp->GetPosition());
}

#undef or
void CGameObject::UpdateLookVector()
{
	auto wmatrix = XMMatrixIdentity();
	auto quater = XMLoadFloat4(&Orient);
	wmatrix *= XMMatrixRotationQuaternion(quater);
	auto ol = XMLoadFloat3(&OffLookvector);
	auto orr = XMLoadFloat3(&OffRightvector);

	ol = XMVector4Transform(ol, wmatrix);
	orr = XMVector4Transform(orr , wmatrix);

	XMStoreFloat3(&Lookvector, ol);
	XMStoreFloat3(&Rightvector, orr );

	if (fabsf(Lookvector.x) < MMPE_EPSILON / 10)
		Lookvector.x = 0;
	if (fabsf(Lookvector.y) < MMPE_EPSILON / 10)
		Lookvector.y = 0;
	if (fabsf(Lookvector.z) < MMPE_EPSILON / 10)
		Lookvector.z = 0;


	if (fabsf(Rightvector.x) < MMPE_EPSILON / 10)
		Rightvector.x = 0;
	if (fabsf(Rightvector.y) < MMPE_EPSILON / 10)
		Rightvector.y = 0;
	if (fabsf(Rightvector.z) < MMPE_EPSILON / 10)
		Rightvector.z = 0;


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

void CGameObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
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
	if (pp != NULL && rb == NULL)
		UpdatePPosCenterPos();//������ �������� ������� ������. �ݵ�� �浹�ϰų� �ϸ� ó���ؾ���
	SetWorldMatrix();//���� �����ǰ� �����̼������� ��������� �����.

	XMMATRIX world = XMLoadFloat4x4(&ObjData.WorldMatrix);

	XMStoreFloat4x4(&ObjData.WorldMatrix, XMMatrixTranspose(world));

	//������� ������Ʈ
	ConstBuffer->CopyData(0, ObjData);

	commandlist->SetGraphicsRootConstantBufferView(2, ConstBuffer->Resource()->GetGPUVirtualAddress());//������Ŀ���
}


//========================================= �ؽ��� ���� =========================================================================

void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, ID3D12Resource* texture, bool isCubeMap, int Offset)
{

	commandlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));


	//�ؽ�ó�� ���̺��� �����̹Ƿ� ���� ���̺� �ΰ��� �����ؾ���.
	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvDescriptorHeap.Get() };
	commandlist->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	if (isCubeMap)
	{
		tex.Offset(0, CbvSrvDescriptorSize);
		commandlist->SetGraphicsRootDescriptorTable(0, tex);
	}
	else
	{
		tex.Offset(Offset, CbvSrvDescriptorSize);
		commandlist->SetGraphicsRootDescriptorTable(1, tex);
	}
	commandlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture,
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist, CGameObject* obj, unordered_map<string, unique_ptr<CTexture>>& Textures, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, string texturename, wstring FileName, bool isCubeMap, int NumDescriptors, int Offset)
{

	auto Tex = make_unique<CTexture>();
	Tex->Name = texturename;
	Tex->Filename = FileName;

	ThrowIfFailed(CreateDDSTextureFromFile12(device,
		commandlist, Tex->Filename.c_str(),
		Tex->Resource, Tex->UploadHeap));

	Textures[Tex->Name] = move(Tex);


	if (SrvDescriptorHeap.Get() == NULL)
	{
		//��ũ���� �� ����
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = NumDescriptors;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&SrvDescriptorHeap)));
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	hDescriptor.Offset(Offset, CbvSrvDescriptorSize);

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


//=================================================== ������Ʈ ���� ===============================================================

CCubeManObject::CCubeManObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	//���ӿ�����Ʈ �����ڿ��� �⺻���ΰ��� ó�������Ƿ� ���⼭��
	//�޽��� �ؽ�ó ���� �ҷ������ �ִϸ��̼� �ε���� ó���ؾ��Ѵ�.

	//����Ʈ�� ����� �迭.
	jarr = new UploadBuffer<JointArr>(m_Device, 1, true);

	int num = 10;
	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female Brown Casual",    L"textures/human/Female Brown Casual 03B.dds", false, num, 0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female Black Knight",    L"textures/human/Female Black Knight 04 Green.dds", false, num, 1);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female Brown Sorceress", L"textures/human/Female Brown Sorceress 03 White.dds", false, num, 2);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female White Knight",    L"textures/human/Female White Knight 04 Purple.dds", false, num, 3);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female White Barbarian", L"textures/human/Female White Barbarian 05 Red.dds", false, num, 4);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male Black Knight",      L"textures/human/Male Black Knight 07 White.dds", false, num, 5);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male White Wizard",      L"textures/human/Male White Wizard 06 White.dds", false, num, 6);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male Black Archer",      L"textures/human/Male Black Archer 05 Green.dds", false, num, 7);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male Fire",              L"textures/human/Male Fire 01 Orange.dds", false, num, 8);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male White King",        L"textures/human/Male White King 01 Red.dds", false, num, 9);


		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);

		CreateMesh = true;

	}
	select = rand() % num;
	if (select == 0)
		TextureName = "Female Brown Casual"; 
	else if (select == 1)
		TextureName = "Female Black Knight"; 
	else if (select == 2)
		TextureName = "Female Brown Sorceress"; 
	else if (select == 3)
		TextureName = "Female White Knight"; 
	else if (select == 4)
		TextureName = "Female White Barbarian"; 
	else if (select == 5)
		TextureName = "Male Black Knight"; 
	else if (select == 6)
		TextureName = "Male White Wizard"; 
	else if (select == 7)
		TextureName = "Male Black Archer"; 
	else if (select == 8)
		TextureName = "Male Fire"; 
	else if (select == 9)
		TextureName = "Male White King"; 
	TexOff = select;


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
	ObjData.Scale = 3;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

	obs = Dynamic;
	//���� ������ (����)�� ��´�. ĳ���ʹ� �������� ���� �ʰ�, źȯ�� �������� ����������.
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Speed = 50;

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(3, 0, 0);
	XMFLOAT3 ry(0, 10, 0);
	XMFLOAT3 rz(0, 0, 3);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(3, 10, 3);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.45);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.


						 //�Ӹ����� HP�� ���
	if (ParticleList != NULL)
	{
		Hpbar = new BarObject(m_Device, commandlist, ParticleList, this, XMFLOAT4(CenterPos.x, CenterPos.y + 25, CenterPos.z, 0));
		HPFrame = new BarFrameObject(m_Device, commandlist, ParticleList, this, XMFLOAT4(CenterPos.x, CenterPos.y + 25, CenterPos.z, 0));

		ParticleList->push_back(HPFrame);
		ParticleList->push_back(Hpbar);
	}

}

CCubeManObject::~CCubeManObject()
{
	if (Hpbar != NULL)
		Hpbar->DelObj = true;
	if (HPFrame != NULL)
		HPFrame->DelObj = true;

}


void CCubeManObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{

	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\cIdle.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(true);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

	//�ִϸ��̼� �ε�
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cIdle.MD5ANIM", &Mesh, this, animations);//0
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cRunning.MD5ANIM", &Mesh, this, animations);//1
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cAttack.MD5ANIM", &Mesh, this, animations);//2
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cDeath.MD5ANIM", &Mesh, this, animations);//2


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

		//�ִϸ��̼� ������Ʈ �ִϸ��̼��� 24���������� ������. ������ FPS�� 24�������� �ƴ�. �׺��� ū ������. ���� 24���������� �ش��������� ���� ����.
		if(n_Animation!=Attack)
			UpdateMD5Model(commandlist, &Mesh, this, gt.DeltaTime()*60.0 / 24.0, n_Animation, animations, jarr);
		else
		{

			UpdateMD5Model(commandlist, &Mesh, this, 2*gt.DeltaTime()*60.0 / 24.0, n_Animation, animations, jarr);
		}

	}


}

void CCubeManObject::ToDead()
{
	SetAnimation(Ani_State::Dead);
}

void CCubeManObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.


	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), false, TexOff);



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

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ �浹�ؼҸ� �ؾ��Ѵ�.
			{

				//�浹 ������ ���� (0,1,0) �̸� Airbone�� false�� �д�. �̴� ���� ���������� ��Ÿ����.
				if (pp->pAxis.y > 0)
				{
					pp->SetVelocity(pp->GetVelocity().x, 0, pp->GetVelocity().z);
					AirBone = false;


				}
				//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
				//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
				if (pp->pAxis.y < 0)
				{
					(*i)->pp->SetVelocity((*i)->pp->GetVelocity().x, 0, (*i)->pp->GetVelocity().z);
					(*i)->AirBone = false;
				}

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
					cn = pp->pAxis;
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

	if (nAni == (int)Ani_State::Attack)//�����ϱ⿴����
	{
		SetAnimation((int)Ani_State::Idle);//�����·εд�.

	}

	if (nAni == Ani_State::Dead)//�״¸���̾�����
	{
		DelObj = true;
	}
}

//------------------- �� �� ü -----------------------//

BulletCube::BulletCube(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon, XMFLOAT4 cp, bool IsMine) : CGameObject(m_Device, commandlist, Plist, cp)
{
	//Bullet Resoucre�� ��� Bullet�ʱ�ȭ�� �ѹ� ������ ó���� �����Ű�µ� �� ���� ������ �ʿ���� - firstBullet �÷��׸� �̿��ؼ� ������������
	bool firstBullet = true;
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "BulletTex", L"textures/effect/fire.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;
		firstBullet = false;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	//���ڷ� �߻�������� �躤�Ͱ� �������� ȸ���� ori���� �ް�, ���� ����(������ 0,0,0,1)�� �����ش�.
	Orient = QuaternionMultiply(Orient, ori);

	UpdateLookVector();

	obs = Bullet;

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.2f;//����ŧ���� �����ش�.

									 //���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 150;

	if (firstBullet)
	{
		//CreateBullet �Լ� ȣ�� - �� Ŭ���̾�Ʈ���� Ű�� ������ �ҷ��� �������� 
		if (IsMine)
		{
			++myID;
			BulletIDList.push_back(myID);

			m_bullet_data.myID = myID;
			m_bullet_data.Master_ID = master->m_player_data.ID;
		}
	}

	LifeTime = 3.5f;
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
	pp->SetMass(0.35f);

	if (ParticleList != NULL)
	{
		BulletParticles = new ParticleObject(m_Device, commandlist, ParticleList, this, 0.2f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles);
		BulletParticles2 = new ParticleObject(m_Device, commandlist, ParticleList, this, 0.3f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles2);
	}

}

BulletCube::~BulletCube()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;
	if (BulletParticles2 != NULL)
		BulletParticles2->DelObj = true;
}


void BulletCube::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{

	CreateCube(&Mesh, 2, 2, 2);
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
	Mat.MatData.Emissive = XMFLOAT4{ 0.75f, 0.23f, 0.13f, 0.7f };
}

void BulletCube::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.


	//���� �� ����
	//pp->integrate(gt.DeltaTime(), &CenterPos);

	//Orient = QuaternionMultiply(Orient, QuaternionRotation(Lookvector, MMPE_PI * gt.DeltaTime()));

	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	//LifeTime -= gt.DeltaTime();

	//if (LifeTime <= 0)
	//	DelObj = true;

}

void BulletCube::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["BulletTex"].get()->Resource.Get(), false);
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

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}
					//������ ��ü���ƴϸ� ��� �ٿ�� Ǯ��д�. �׷��� ƨ��ϱ�.
					(*i)->pp->SetBounce(true);

				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�� �ӵ��� �����.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);
				(*i)->pp->SetBounce(false);
				//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
				DelObj = true;


			}
		}
	}
}



HeavyBulletCube::HeavyBulletCube(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon, XMFLOAT4 cp, bool IsMine) : CGameObject(m_Device, commandlist, Plist, cp)
{
	//Bullet Resoucre�� ��� Bullet�ʱ�ȭ�� �ѹ� ������ ó���� �����Ű�µ� �� ���� ������ �ʿ���� - firstBullet �÷��׸� �̿��ؼ� ������������
	bool firstBullet = true;
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "BulletTex", L"textures/effect/fire.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;
		firstBullet = false;
	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	//���ڷ� �߻�������� �躤�Ͱ� �������� ȸ���� ori���� �ް�, ���� ����(������ 0,0,0,1)�� �����ش�.
	Orient = QuaternionMultiply(Orient, ori);

	UpdateLookVector();

	obs = Bullet;

	ObjData.isAnimation = 0;
	ObjData.Scale = 2.0;
	ObjData.SpecularParamater = 0.8f;//����ŧ���� �����ش�.

									 //���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 30;
	gamedata.GodMode = true;
	gamedata.Speed = 180;

	if (firstBullet)
	{
		//CreateBullet �Լ� ȣ�� - �� Ŭ���̾�Ʈ���� Ű�� ������ �ҷ��� �������� 
		if (IsMine)
		{
			++myID;
			BulletIDList.push_back(myID);

			m_bullet_data.myID = myID;
			m_bullet_data.Master_ID = master->m_player_data.ID;
		}
	}

	LifeTime = 3.5;
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
	pp->SetHalfBox(2, 2, 2);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//�躤�ͷ� ���ư�
	pp->SetMass(1);

	if (ParticleList != NULL)
	{
		BulletParticles = new ParticleObject(m_Device, commandlist, ParticleList, this, 0.2f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles);
		BulletParticles2 = new ParticleObject(m_Device, commandlist, ParticleList, this, 0.3f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles2);
	}

}

HeavyBulletCube::~HeavyBulletCube()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;
	if (BulletParticles2 != NULL)
		BulletParticles2->DelObj = true;
}


void HeavyBulletCube::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{


	CreateCube(&Mesh, 4, 4, 4);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);


}

void HeavyBulletCube::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);

	Mat.MatData.Emissive = XMFLOAT4{ 0.12f, 0.53f, 0.13f, 0.8f };
	Mat.MatData.Roughness = 0.3f;
}

void HeavyBulletCube::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.

	//pp->integrate(gt.DeltaTime(), &CenterPos);
	//
	////No�ִϸ��̼�!
	//
	////����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	//LifeTime -= gt.DeltaTime();
	//
	//Orient = QuaternionMultiply(Orient, QuaternionRotation(Lookvector, MMPE_PI / 6 * gt.DeltaTime()));
	//
	//if (LifeTime <= 0)
	//	DelObj = true;

}

void HeavyBulletCube::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["BulletTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}

//�浹��. �浹����� �浹�ؼҸ� �ô´�.
void HeavyBulletCube::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}
					//������ ��ü���ƴϸ� ��� �ٿ�� Ǯ��д�. �׷��� ƨ��ϱ�.
					(*i)->pp->SetBounce(true);

				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�� �ӵ��� �����.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);
				(*i)->pp->SetBounce(false);
				//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
				DelObj = true;


			}
		}
	}
}

//-------------------- ��Ʈ����ũ ---------------------------------//

Tetris1::Tetris1(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Tetris1Tex", L"textures/object/red.dds", false);
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
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

	obs = Bullet;
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 3;
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
	pp->SetHalfBox(4, 4, 4);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(0, -1 * gamedata.Speed, 0);
	pp->SetMass(2);



}

Tetris1::~Tetris1()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;
	if (BulletParticles2 != NULL)
		BulletParticles2->DelObj = true;
}


void Tetris1::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{


	CreateCube(&Mesh, 8, 8, 8);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);


}

void Tetris1::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void Tetris1::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.
	pp->AddForce(0, -300, 0);
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No�ִϸ��̼�!

	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetris1::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris1Tex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}

//�浹��. �浹����� �浹�ؼҸ� �ô´�.
void Tetris1::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}

				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�� �ӵ��� �����.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);

				//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
				DelObj = true;


			}
		}
	}
}

Tetris2::Tetris2(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Tetris2Tex", L"textures/object/green.dds", false);
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
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

	obs = Bullet;
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 15;
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
	pp->SetHalfBox(3, 9, 3);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(0, -1 * gamedata.Speed, 0);
	pp->SetMass(2);


}

Tetris2::~Tetris2()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;
	if (BulletParticles2 != NULL)
		BulletParticles2->DelObj = true;
}


void Tetris2::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{


	CreateCube(&Mesh, 6, 18, 6);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);


}

void Tetris2::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void Tetris2::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.
	pp->AddForce(0, -100, 0);
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No�ִϸ��̼�!

	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetris2::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris2Tex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}

//�浹��. �浹����� �浹�ؼҸ� �ô´�.
void Tetris2::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}


				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�� �ӵ��� �����.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);

				//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
				DelObj = true;


			}
		}
	}
}

Tetris3::Tetris3(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Tetris3Tex", L"textures/object/blue.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	obs = Bullet;
	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

									 //���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 5;
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
	pp->SetHalfBox(4, 8, 2);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(0, -1 * gamedata.Speed, 0);
	pp->SetMass(2);


}

Tetris3::~Tetris3()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;
	if (BulletParticles2 != NULL)
		BulletParticles2->DelObj = true;
}


void Tetris3::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{


	CreateTetrisL(&Mesh, 4, 16, 4);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);


}

void Tetris3::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void Tetris3::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.
	pp->AddForce(0, -180, 0);
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No�ִϸ��̼�!

	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetris3::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris3Tex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}

//�浹��. �浹����� �浹�ؼҸ� �ô´�.
void Tetris3::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}


				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�� �ӵ��� �����.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);
				DelObj = true;


			}
		}
	}
}


Tetris4::Tetris4(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Tetris4Tex", L"textures/object/yellow.dds", false);
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
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	obs = Bullet;
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 10;
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
	pp->SetHalfBox(4, 8, 2);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(0, -1 * gamedata.Speed, 0);
	pp->SetMass(2);

}

Tetris4::~Tetris4()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;
	if (BulletParticles2 != NULL)
		BulletParticles2->DelObj = true;
}


void Tetris4::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{


	CreateTetris��(&Mesh, 4, 16, 4);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);


}

void Tetris4::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void Tetris4::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.
	pp->AddForce(0, -150, 0);
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No�ִϸ��̼�!

	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetris4::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris4Tex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}

//�浹��. �浹����� �浹�ؼҸ� �ô´�.
void Tetris4::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}


				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�� �ӵ��� �����.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);

				DelObj = true;


			}
		}
	}
}

Tetrike::Tetrike(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*Bulletlist, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Tetrike", L"textures/object/Portal.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	CenterPos.y += 150;

	Blist = Bulletlist;
	ParticleList = Plist;

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	obs = Bullet;
	//���Ӱ��� �����͵�
	gamedata.GodMode = true;


	Master = master;
	LockOn = lockon;
	pp = NULL;



}

Tetrike::~Tetrike()
{

}


void Tetrike::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{


	CreateCube(&Mesh, 120, 0.01, 120);

	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);


}

void Tetrike::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void Tetrike::Tick(const GameTimer & gt)
{
	int c = abs((rand()*(int)(gt.TotalTime() * 1000))) % 10;
	c += 3;
	//No�ִϸ��̼�!

	if (ShotTime >= 0.5)
	{
		ShotTime = 0;
	}
	if (ShotTime == 0)
	{

		for (int i = 0; i < c; i++)
		{
			auto pos = CenterPos;
			float  n = abs((abs(rand())*(int)(fabsf(gt.TotalTime()) * 31430)) % 90) - 45;
			float n2 = abs((abs(rand())*(int)(fabsf(gt.TotalTime()) * 12340)) % 90) - 45;

			/*float b = sqrt(n*n + n2 * n2);

			if (b != 0)
			{
			n /= b;
			n2 /= b;
			n *= 60;
			n2 *= 60;

			}
			*/

			pos.x += n;
			pos.z += n2;
			pos.y += abs((abs(rand())*(int)(fabsf(gt.TotalTime()) * 12340)) % 5) - 5;

			int g = (rand()*(int)(gt.TotalTime() * 32524)) % 4;
			if (g == 0)
				Blist->push_back(new Tetris1(device, commandlist, ParticleList, Master, NULL, pos));
			else if (g == 1)
			{
				auto t = new Tetris2(device, commandlist, ParticleList, Master, NULL, pos);
				int k = (rand()*(int)(gt.TotalTime() * 32524)) % 2;

				if (k == 0)
					t->Orient = QuaternionMultiply(t->Orient, QuaternionRotation(XMFLOAT3(1, 0, 0), MMPE_PI / 2));
				Blist->push_back(t);
			}
			else if (g == 2)
			{
				auto t = new Tetris3(device, commandlist, ParticleList, Master, NULL, pos);
				int k = (rand()*(int)(gt.TotalTime() * 32524)) % 3;

				if (k == 0)
					t->Orient = QuaternionMultiply(t->Orient, QuaternionRotation(XMFLOAT3(0, 0, 1), MMPE_PI / 2));
				else if (k == 1)
					t->Orient = QuaternionMultiply(t->Orient, QuaternionRotation(XMFLOAT3(0, 0, 1), MMPE_PI));
				else if (k == 2)
					t->Orient = QuaternionMultiply(t->Orient, QuaternionRotation(XMFLOAT3(0, 0, 1), 3 * MMPE_PI / 2));
				Blist->push_back(t);
			}
			else if (g == 3)
			{
				auto t = new Tetris4(device, commandlist, ParticleList, Master, NULL, pos);
				int k = (rand()*(int)(gt.TotalTime() * 32524)) % 3;

				if (k == 0)
					t->Orient = QuaternionMultiply(t->Orient, QuaternionRotation(XMFLOAT3(0, 0, 1), MMPE_PI / 2));
				else if (k == 1)
					t->Orient = QuaternionMultiply(t->Orient, QuaternionRotation(XMFLOAT3(0, 0, 1), MMPE_PI));
				else if (k == 2)
					t->Orient = QuaternionMultiply(t->Orient, QuaternionRotation(XMFLOAT3(0, 0, 1), 3 * MMPE_PI / 2));
				Blist->push_back(t);
			}
		}
	}
	ShotTime += gt.DeltaTime();




	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetrike::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetrike"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}


//---------------------- ����ƽ ������Ʈ -----------------------------//

SphereObject::SphereObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "MapTex", L"textures/sky/snowcube1024.dds", true);
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
	ObjData.Scale = 1024.0f;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

	obs = Static;
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;


}


void SphereObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\sphere.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}


void SphereObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["MapTex"].get()->Resource.Get(), true);
	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}



//////////////

CubeObject::CubeObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "redTex", L"textures/object/red.dds", false, 7, 0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "orangeTex", L"textures/object/orange.dds", false, 7, 1);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "yellowTex", L"textures/object/yellow.dds", false, 7, 2);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "pinkTex", L"textures/object/pink.dds", false, 7, 3);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "whiteTex", L"textures/object/white.dds", false, 7, 4);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "blueTex", L"textures/object/blue.dds", false, 7, 5);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "greenTex", L"textures/object/green.dds", false, 7, 6);

		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);

		CreateMesh = true;

	}
	selectColor = rand() % 7 + 0;
	if (selectColor == 0)
		TextureName = "redTex";
	else if (selectColor == 1)
		TextureName = "orangeTex";
	else if (selectColor == 2)
		TextureName = "yellowTex";
	else if (selectColor == 3)
		TextureName = "pinkTex"; 
	else if (selectColor == 4)
		TextureName = "whiteTex";
	else if (selectColor == 5)
		TextureName = "blueTex"; 
	else if (selectColor == 6)
		TextureName = "greenTex";

	TexOff = selectColor;

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.46f;//����ŧ���� �����ش�.

	obs = Static;

	//���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;

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
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.


}

void CubeObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 1, 1, 1);

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
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size() > 0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), false, TexOff);

	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}

MoveCubeObject::MoveCubeObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, float rad, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "redTex", L"textures/object/red.dds", false, 7, 0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "orangeTex", L"textures/object/orange.dds", false, 7, 1);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "yellowTex", L"textures/object/yellow.dds", false, 7, 2);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "pinkTex", L"textures/object/pink.dds", false, 7, 3);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "whiteTex", L"textures/object/white.dds", false, 7, 4);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "blueTex", L"textures/object/blue.dds", false, 7, 5);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "greenTex", L"textures/object/green.dds", false, 7, 6);

		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}
	selectColor = rand() % 7 + 0;
	if (selectColor == 0)
		TextureName = "redTex"; 
	else if (selectColor == 1)
		TextureName = "orangeTex"; 
	else if (selectColor == 2)
		TextureName = "yellowTex"; 
	else if (selectColor == 3)
		TextureName = "pinkTex"; 
	else if (selectColor == 4)
		TextureName = "whiteTex";
	else if (selectColor == 5)
		TextureName = "blueTex"; 
	else if (selectColor == 6)
		TextureName = "greenTex"; 

	TexOff = selectColor;

	Rad = rad;

	n = rand() % 30 - 30;

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
	staticobject = true;

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(10, 0, 0);
	XMFLOAT3 ry(0, 5, 0);
	XMFLOAT3 rz(0, 0, 10);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(10, 5, 10);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.

}


void MoveCubeObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 2, 1, 2);

	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void MoveCubeObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void MoveCubeObject::Tick(const GameTimer & gt)
{
	n += gt.DeltaTime();

	CenterPos.x = Rad * cosf(MMPE_PI * n * 0.1f);
	CenterPos.z = Rad * sinf(MMPE_PI * n * 0.1f);

	pp->SetPosition(CenterPos);

}

void MoveCubeObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size() > 0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), false, TexOff);

	UpdateConstBuffer(commandlist);

	//���� �׸���.
	Mesh.Render(commandlist);
}

void MoveCubeObject::Collision(list<CGameObject*>* collist, float DeltaTime)
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
				if (pp->pAxis.y > 0)
				{
					pp->SetVelocity(pp->GetVelocity().x, 0, pp->GetVelocity().z);
					AirBone = false;

				}
				//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
				//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
				if (pp->pAxis.y < 0)
				{
					(*i)->pp->SetVelocity((*i)->pp->GetVelocity().x, 0, (*i)->pp->GetVelocity().z);
					(*i)->AirBone = false;
				}

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
					cn = pp->pAxis;
				}

				//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime);//����� ƨ���� �ʴ´�.
				UpdatePPosCenterPos();
				(*i)->UpdatePPosCenterPos();
			}
		}
	}
}

/////////////

GridObject::GridObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "GridTex", L"textures/object/tile1024.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.3f;//����ŧ���� �����ش�.

	obs = Static;
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;

}

void GridObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreatePentagon(&Mesh, 1200.0f);

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void GridObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void GridObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["GridTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}

///////////////////////////////////

//HPBar������Ʈ
BarObject::BarObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, CGameObject* master, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 1;

	Master = master;
	ObjData.CustomData1.y = Master->gamedata.HP;

	obs = UI;
	//���Ӱ��� �����͵�
	gamedata.HP = Master->gamedata.HP;
	gamedata.GodMode = true;
	staticobject = true;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "HPTex", L"textures/ui/HP.dds", false);

		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void BarObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
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

void BarObject::Tick(const GameTimer & gt)
{
	//hp�� �־��־ shader���� ������ũ�⸸ŭ �ٸ� �����. 	
	//HP�ٴ� Ŀ���ҵ����� y�� ����ϸ� ���⿡ ������ HP ������ ����.
	if (Master->gamedata.HP <= 0)
		Master->gamedata.HP = 0;

	ObjData.CustomData1.y = Master->gamedata.HP / Master->gamedata.MAXHP;
	CenterPos.x = Master->CenterPos.x; CenterPos.y = Master->CenterPos.y + 10; CenterPos.z = Master->CenterPos.z;
}

void BarObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["HPTex"].get()->Resource.Get(), false);
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


//HPbar Ʋ
BarFrameObject::BarFrameObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, CGameObject * master, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 4;

	Master = master;

	//���Ӱ��� �����͵�
	gamedata.HP = Master->gamedata.HP;
	gamedata.GodMode = true;
	staticobject = true;

	obs = UI;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "HPFrameTex", L"textures/ui/Bar.dds", false);

		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void BarFrameObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
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

void BarFrameObject::Tick(const GameTimer & gt)
{
	CenterPos.x = Master->CenterPos.x; CenterPos.y = Master->CenterPos.y + 10; CenterPos.z = Master->CenterPos.z;
}

void BarFrameObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["HPFrameTex"].get()->Resource.Get(), false);
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


DamageObject::DamageObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, float Damaged, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 15.0f;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

	damaged = Damaged;
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 0;
	gamedata.HP = 0;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 10;
	staticobject = true;

	obs = UI;

	ObjData.TexClamp = XMFLOAT4((damaged-10)*0.01f, damaged*0.01f, 0, 0);

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "DamageTex", L"textures/damage/damages.dds", false);


		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}

}

void DamageObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
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

void DamageObject::Tick(const GameTimer & gt)
{
	LifeTime -= gt.DeltaTime();

	if (LifeTime > 0.0f)
		CenterPos.y += gamedata.Speed * gt.DeltaTime();

	if (LifeTime <= 0)
		DelObj = true;
}

void DamageObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size() > 0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["DamageTex"].get()->Resource.Get(), false);

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

RigidCubeObject::RigidCubeObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	//�޽��� �ؽ�ó 

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "CubeTex", L"textures/object/DeepIce.dds", false);
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
	ObjData.Scale = 20.0f;
	ObjData.SpecularParamater =-1.f;//����ŧ���� �����ش�.

	obs = Rigid;


	//���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = false;


	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(12, 0, 0);
	XMFLOAT3 ry(0, 10, 0);
	XMFLOAT3 rz(0, 0, 12);
	rco.SetPlane(rx, ry, rz);

	//������ �ٵ�

	rb = new RigidBody();
	rb->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	rb->SetHalfBox(10, 10, 10);//�浹 �ڽ��� x,y,z ũ��
	rb->SetDamping(0.5f, 0.38f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	rb->SetBounce(false);//ƨ���� �ʴ´�.
	rb->SetMass(1.5);//������ ��ü�� ���԰� �����̴�.
	rb->SetIMoment(10, 10, 10);

	rb->SetOrient(&Orient);

	//�׽�Ʈ �ڵ�
	XMFLOAT3 testForce{ -5,-3,2 };
	XMFLOAT3 testPoint{ -15,5,-5 };
	//���� �ѹ��� ���Ѵ�. ���⼭ ���� �ѹ��� ���Ѵ��� ������ F�� 0.1�ʸ�ŭ ���ϴ°��� ���Ѵ�.
	//���ʿ� ���� �ѹ��� ���Ѵٶ�� �����Ҽ��� ������ ���� F��ŭ n�ʵ��� ���ϴ°� �´¸��̴�.
	rb->AddForcePoint(testForce, testPoint);
	rb->integrate(0.1f);

}

void RigidCubeObject::Tick(const GameTimer & gt)
{
	if (rb != NULL)
		rb->integrate(gt.DeltaTime());
}

void RigidCubeObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 1, 1, 1);

	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);


}

void RigidCubeObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;


}

void RigidCubeObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}

void RigidCubeObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	//���Ŀ� �߰��ؾ��Ѵ�.

	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this)
		{
			if ((*i)->rb != NULL)
			{
				bool test = rb->CollisionTest(*(*i)->rb, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

				if (test)//�浹������ �浹�ؼҸ� �ؾ��Ѵ�.
				{


					//�浹 ������ ���� (0,1,0) �̸� Airbone�� false�� �д�. �̴� ���� ���������� ��Ÿ����.
					if (rb->CollisionPointVector[0].pAxis.y > 0)
					{

						rb->SetVelocity(rb->GetVelocity().x, 0, rb->GetVelocity().z);
						AirBone = false;
					}
					//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
					//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
					if (rb->CollisionPointVector[0].pAxis.y < 0)
					{
						(*i)->rb->SetVelocity((*i)->rb->GetVelocity().x, 0, (*i)->rb->GetVelocity().z);
						(*i)->AirBone = false;
					}


					rb->ResolvePenetration(*(*i)->rb, DeltaTime);

				}
			}
			else
			{
				if ((*i)->pp != NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
				{

					RigidBody ppConvertrb;
					ppConvertrb.SetVelocity((*i)->pp->GetVelocity());
					ppConvertrb.SetPosition(&(*i)->CenterPos);
					ppConvertrb.SetMass((*i)->pp->GetMass(false));
					ppConvertrb.SetHalfBox((*i)->pp->GetHalfBox().x, (*i)->pp->GetHalfBox().y, (*i)->pp->GetHalfBox().z);
					ppConvertrb.SetE(1);
					ppConvertrb.SetDamping((*i)->pp->GetDamping(), 0);
					ppConvertrb.SetBounce((*i)->pp->GetBounce());
					ppConvertrb.SetAngularVelocity(0, 0, 0);
					ppConvertrb.SetAccel((*i)->pp->GetAccel());





					bool test = rb->CollisionTest(ppConvertrb, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

					if (test)//�浹������ �浹�ؼҸ� �ؾ��Ѵ�.
					{
						//�浹 ������ ���� ���� �Ʒ��� �������� Airbone�� false�� �д�. �̴� ���� �Ʒ��������� ��Ÿ����.
						//�� �÷��̾ �����ְ� ���� �Ʒ���.
						if (rb->CollisionPointVector[0].pAxis.y < 0)
						{

							ppConvertrb.SetVelocity(ppConvertrb.GetVelocity().x, 0, ppConvertrb.GetVelocity().z);
							(*i)->AirBone = false;
						}

						
						if ((*i)->obs == Bullet)
						{
							(*i)->DelObj = true;
							//�浹 ó��. ��ݷ��� ���ϰ� ��ħ�κ��ؼ�
							rb->CollisionResolve(ppConvertrb, XMFLOAT3(0, 0, 0), DeltaTime, 6000, 1400, 1.5);

							(*i)->pp->SetVelocity(ppConvertrb.GetVelocity());
							(*i)->pp->SetPosition(ppConvertrb.GetPosition());
							(*i)->pp->SetAccel(ppConvertrb.GetAccel());
							(*i)->UpdatePPosCenterPos();
						}
						else
						{
							//�浹ó�� ����� �׳� ��ħ�κи� �ؼ���

							rb->AmendTime = 0;
							rb->ResolvePenetration(ppConvertrb, DeltaTime);
							(*i)->pp->SetVelocity(ppConvertrb.GetVelocity());
							(*i)->pp->SetPosition(ppConvertrb.GetPosition());
							(*i)->pp->SetAccel(ppConvertrb.GetAccel());
							(*i)->UpdatePPosCenterPos();
						}
						

					}

				}
			}
		}
	}
}



//================================��������Ʈ=======================================

SmallWallObject::SmallWallObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, float dgree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "SmallWall", L"textures/object/IceWall.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	auto q = XMLoadFloat4(&Orient);//������ degree��ŭ ������ �Ѵ�.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, dgree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.80f;//����ŧ���� �����ش�.

	obs = Static;
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;

	XMFLOAT3 raxis{ 0,1,0 };
	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(20, 0, 0);
	auto rqx = QuaternionRotation(raxis, dgree);
	Orient = QuaternionMultiply(XMFLOAT4(rx.x, rx.y, rx.z, 0), rqx);
	rx.x = Orient.x; rx.y = Orient.y, rx.z = Orient.z;

	XMFLOAT3 ry(0, 10, 0);
	auto rqy = QuaternionRotation(raxis, dgree);
	Orient = QuaternionMultiply(XMFLOAT4(ry.x, ry.y, ry.z, 0), rqy);
	ry.x = Orient.x; ry.y = Orient.y, ry.z = Orient.z;

	XMFLOAT3 rz(0, 0, 5);
	auto rqz = QuaternionRotation(raxis, dgree);
	Orient = QuaternionMultiply(XMFLOAT4(ry.x, ry.y, ry.z, 0), rqz);
	rz.x = Orient.x; rz.y = Orient.y, rz.z = Orient.z;

	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(20, 10, 5);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.


}

void SmallWallObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 40, 20, 10);

	//�� �ε�
	//LoadMD5Model(L".\\�÷��̾�޽���\\Cube.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void SmallWallObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void SmallWallObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["SmallWall"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}




BigWallObject::BigWallObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, float dgree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Wall", L"textures/object/icewall.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}
	Blending = true;
	ObjData.BlendValue = 0.45;
	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	auto q = XMLoadFloat4(&Orient);//������ degree��ŭ ������ �Ѵ�.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, dgree);
	Orient = QuaternionMultiply(Orient, q2);

	obs = Static;
	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.2f;//����ŧ���� �����ش�.

									 //���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;


	XMFLOAT3 raxis{ 0,1,0 };
	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(350, 0, 0);
	auto rqx = QuaternionRotation(raxis, dgree);
	Orient = QuaternionMultiply(XMFLOAT4(rx.x, rx.y, rx.z, 0), rqx);
	rx.x = Orient.x; rx.y = Orient.y, rx.z = Orient.z;

	XMFLOAT3 ry(0, 50, 0);
	auto rqy = QuaternionRotation(raxis, dgree);
	Orient = QuaternionMultiply(XMFLOAT4(ry.x, ry.y, ry.z, 0), rqy);
	ry.x = Orient.x; ry.y = Orient.y, ry.z = Orient.z;

	XMFLOAT3 rz(0, 0, 5);
	auto rqz = QuaternionRotation(raxis, dgree);
	Orient = QuaternionMultiply(XMFLOAT4(ry.x, ry.y, ry.z, 0), rqz);
	rz.x = Orient.x; rz.y = Orient.y, rz.z = Orient.z;

	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(350, 50, 5);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.


}

void BigWallObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 700, 100, 10);

	//�� �ε�
	//LoadMD5Model(L".\\�÷��̾�޽���\\Cube.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void BigWallObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void BigWallObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Wall"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}


BuildingObject::BuildingObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, float dgree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "CubeTex", L"textures/object/bricks.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	auto q = XMLoadFloat4(&Orient);//������ degree��ŭ ������ �Ѵ�.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, dgree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.



									 //���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;
	obs = Static;
	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(15, 0, 0);
	XMFLOAT3 ry(0, 45, 0);
	XMFLOAT3 rz(0, 0, 15);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(15, 45, 15);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.

}

void BuildingObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 30, 90, 30);

	//�� �ε�
	//LoadMD5Model(L".\\�÷��̾�޽���\\Cube.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void BuildingObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void BuildingObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}

//���� ������Ʈ

RangeObject::RangeObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "CubeTex", L"textures/ui/range.dds", false);
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
	ObjData.BlendValue = 0.5f;
	Blending = true;
	obs = UI;


	//���Ӱ��� �����͵�
	gamedata.GodMode = true;

	staticobject = true;


	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = NULL;

}

void RangeObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 60, 0.01, 60);

	//�� �ε�
	//LoadMD5Model(L".\\�÷��̾�޽���\\Cube.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void RangeObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void RangeObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}



////////////////////

ParticleObject::ParticleObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, float LifeTime, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.3f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 2;
	Master = master;

	//���Ӱ��� �����͵�
	gamedata.MAXHP = 0;
	gamedata.HP = 0;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 10;
	staticobject = true;
	obs = UI;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "sparkTex", L"textures/effect/sparkTex.dds", false);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}

	Lookvector.x = -(Master->Lookvector.x); Lookvector.y = -(Master->Lookvector.y); Lookvector.z = -(Master->Lookvector.z);
	Lookvector = Float3Normalize(Lookvector);

	XMFLOAT3 Vel = XMFLOAT3(Lookvector.x * gamedata.Speed, Lookvector.y * gamedata.Speed, Lookvector.z * gamedata.Speed);
	ParticleTime = LifeTime;
	ObjData.Velocity = Vel;

	auto q = XMLoadFloat4(&Orient);
	XMFLOAT3 axis = { 0,1,0 };
	XMFLOAT4 q2;
	if (Lookvector.z >0)
		q2 = QuaternionRotation(axis, MMPE_PI / 2 * Lookvector.x);
	else if (Lookvector.z <0)
		q2 = QuaternionRotation(axis, MMPE_PI / 2 * -Lookvector.x);

	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
}

void ParticleObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	UINT numOfParticle = 40;

	Mesh.SubResource = new CVertex[numOfParticle];
	Mesh.nVertex = numOfParticle;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT[numOfParticle];
	Mesh.nindex = numOfParticle;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);

	float n = 0.1;
	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
	for (int i = 0; i < numOfParticle; ++i)
	{

		Mesh.SubResource[i].V.x = CenterPos.x * Lookvector.x + (cosf(MMPE_PI * n));
		Mesh.SubResource[i].V.y = CenterPos.y * Lookvector.y + (sinf(MMPE_PI * n));
		Mesh.SubResource[i].V.z = CenterPos.z * Lookvector.z;

		Mesh.Index[i] = i;
		n += 0.1;
	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void ParticleObject::Tick(const GameTimer & gt)
{
	LifeTime -= gt.DeltaTime();

	if (LifeTime <= 0.0f)
	{
		CenterPos = Master->CenterPos;
		LifeTime = ParticleTime;
	}


	ObjData.CustomData1.w = rand() % RAND_MAX;
	ObjData.PTime = gt.DeltaTime();

}

void ParticleObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["sparkTex"].get()->Resource.Get(), false);
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

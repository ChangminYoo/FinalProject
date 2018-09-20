#include "CGameObject.h"
#include"FSM.h"
#include"CPlayer.h"
extern UINT CbvSrvDescriptorSize;

int CGameObject::g_numofdice = 0;

int CGameObject::g_npcID = -1;
int CGameObject::g_npcBulletID = -1;

unsigned short CGameObject::myID = 0;
list<unsigned short> CGameObject::BulletIDList = list<unsigned short>();
bool CGameObject::m_IsBulletFirstCount = true;

unsigned short CGameObject::m_hmBulletID = 0;
bool CGameObject::m_hmBulletFirstCount = true;

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

CGameObject::CGameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp)
{

	//���⼭�� �⺻���ΰ͵鸸 ó���Ѵ�. ��ġ�� ȸ���� �� �ʱ�ȭ
	this->device = m_Device;
	this->commandlist = commandlist;
	this->CenterPos = cp;//������ġ
	this->ParticleList = Plist;
	this->Shadow = shadow;
	this->OrgPos = cp;
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

void CGameObject::SetShadowMatrix()
{
	auto wmatrix = XMMatrixIdentity();
	auto pos = XMLoadFloat4(&CenterPos);
	auto quater = XMLoadFloat4(&Orient);

	wmatrix *= XMMatrixRotationQuaternion(quater);
	//wmatrix *= XMMatrixRotationY(0.5f*MMPE_PI);
	wmatrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
	wmatrix *= XMMatrixTranslationFromVector(pos);


	XMStoreFloat4x4(&ObjData.WorldMatrix, wmatrix);

	XMFLOAT3 Direction = { 5.7f,-10.5f,8.1f }; //light[0].Direction

	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
	XMVECTOR toMainLight = -XMLoadFloat3(&Direction);
	XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
	XMMATRIX shadowOffsetY = XMMatrixTranslation(0.0f, 0.01f, 0.0f);

	XMStoreFloat4x4(&ObjData.WorldMatrix, wmatrix* S * shadowOffsetY);
}

void CGameObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{

}

void CGameObject::ToDamage(float Damage)
{
	if (Damage >= 100)
		Damage = 1;
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

void CGameObject::UpdateConstBuffer(ID3D12GraphicsCommandList * commandlist, bool isShadow)
{
	//�׻� ȣ���Ұ�. �躤�Ϳ� ����Ʈ���͸� ������Ʈ���ش�.
	UpdateLookVector();

	if (isShadow == false)
		SetWorldMatrix();//���� �����ǰ� �����̼������� ��������� �����.
	else
		SetShadowMatrix();

	XMMATRIX world = XMLoadFloat4x4(&ObjData.WorldMatrix);

	XMStoreFloat4x4(&ObjData.WorldMatrix, XMMatrixTranspose(world));
	

	//������� ������Ʈ
	ConstBuffer->CopyData(0, ObjData);

	commandlist->SetGraphicsRootConstantBufferView(1, ConstBuffer->Resource()->GetGPUVirtualAddress());//������Ŀ���
}


//========================================= �ؽ��� ���� =========================================================================

//texMap == 0 DiffuseMap
//texMap == 1 CubeMap
//texMap == 2 NormalMap
void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, ID3D12Resource* texture, int texMap, int Offset)
{

	commandlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));


	//�ؽ�ó�� ���̺��� �����̹Ƿ� ���� ���̺� �ΰ��� �����ؾ���.
	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvDescriptorHeap.Get() };
	commandlist->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	if (texMap == 1)
	{
		tex.Offset(0, CbvSrvDescriptorSize);
		commandlist->SetGraphicsRootDescriptorTable(5, tex);
	}
	else if (texMap == 2)
	{
		tex.Offset(0, CbvSrvDescriptorSize);
		commandlist->SetGraphicsRootDescriptorTable(7, tex);
	}
	else
	{
		tex.Offset(Offset, CbvSrvDescriptorSize);
		commandlist->SetGraphicsRootDescriptorTable(6, tex);
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

CCubeManObject::CCubeManObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{

	//���ӿ�����Ʈ �����ڿ��� �⺻���ΰ��� ó�������Ƿ� ���⼭��
	//�޽��� �ؽ�ó ���� �ҷ������ �ִϸ��̼� �ε���� ó���ؾ��Ѵ�.

	//����Ʈ�� ����� �迭.
	jarr = new UploadBuffer<JointArr>(m_Device, 1, true);

	int num = 20;
	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female Brown Casual",   L"textures/human/Female Brown Casual 03B.dds", false, num, 0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female Black Knight",   L"textures/human/Female Black Knight 04 Green.dds", false, num, 1);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female Brown Sorceress", L"textures/human/Female Brown Sorceress 03 White.dds", false, num, 2);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female White Knight",   L"textures/human/Female White Knight 04 Purple.dds", false, num, 3);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female White Barbarian", L"textures/human/Female White Barbarian 05 Red.dds", false, num, 4);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male Black Knight",    L"textures/human/Male Black Knight 07 White.dds", false, num, 5);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male White Wizard",    L"textures/human/Male White Wizard 06 White.dds", false, num, 6);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male Black Archer",    L"textures/human/Male Black Archer 05 Green.dds", false, num, 7);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male Fire",            L"textures/human/Male Fire 01 Orange.dds", false, num, 8);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male White King",      L"textures/human/Male White King 01 Red.dds", false, num, 9);
		
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female Brown Casual N", L"textures/human/Female Brown Casual N.dds", false, num, 10);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female Black Knight N", L"textures/human/Female Black Knight N.dds", false, num, 11);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female Brown Sorceress N", L"textures/human/Female Brown Sorceress N.dds", false, num, 12);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female White Knight N", L"textures/human/Female White Knight N.dds", false, num, 13);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Female White Barbarian N", L"textures/human/Female White Barbarian N.dds", false, num, 14);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male Black Knight N", L"textures/human/Male Black Knight N.dds", false, num, 15);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male White Wizard N", L"textures/human/Male White Wizard N.dds", false, num, 16);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male Black Archer N", L"textures/human/Male Black Archer N.dds", false, num, 17);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male Fire N", L"textures/human/Male Fire N.dds", false, num, 18);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Male White King N", L"textures/human/Male White King N.dds", false, num, 19);



		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
	
		CreateMesh = true;
	
	}
	select = 9;

	m_player_data.id = -1;
	
	TextureName = "Male White King";
	NTextureName = "Male White King N";
	TexOff = select;
	NTexOff = TexOff + 10;

	pointrank.DeathCount = 0;
	pointrank.KillCount = 0;
	pointrank.Point = 0;
	pointrank.TopMode = false;
	pointrank.Rank = MAX_PLAYER;

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
	ObjData.SpecularParamater = 2.0f;//����ŧ���� �����ش�.
	ObjData.CustomData1.w = 1234;

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
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(3, 10, 3);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.45);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	
	isShieldOn = false;
	
	//�Ӹ����� HP�� ���
	if (ParticleList != NULL)
	{
		Hpbar = new BarObject(m_Device, commandlist, ParticleList,NULL, this, 10, XMFLOAT4(CenterPos.x, 10, CenterPos.z, 0));
		HPFrame = new BarFrameObject(m_Device, commandlist, ParticleList, NULL, this, 10, XMFLOAT4(CenterPos.x, 10, CenterPos.z, 0));

		ParticleList->push_back(HPFrame);
		ParticleList->push_back(Hpbar);
	}
	if (Shadow != NULL)
	{
		s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(0, 0, 0), 0, MMPE_PI, CenterPos);
		s->ObjData.Scale = 2.0f;
		Shadow->push_back(s);
	}

}

CCubeManObject::~CCubeManObject()
{
	
	if (Hpbar != NULL)
		Hpbar->DelObj = true;
	if (HPFrame != NULL)
		HPFrame->DelObj = true;
	if (Rank1 != NULL)
		Rank1->DelObj = true;
	if (s != NULL)
		s->DelObj = true;
}


void CCubeManObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cDeath.MD5ANIM", &Mesh, this, animations);//3
	
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
	//pp->integrate(gt.DeltaTime());

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

	if (DrawObj)
	{
		if (Textures.size() > 0)
		{
			SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), 0, TexOff);
			SetTexture(commandlist, SrvDescriptorHeap, Textures[NTextureName].get()->Resource.Get(), 2, NTexOff);
		}


		UpdateConstBuffer(commandlist, false);

		Mat.UpdateConstantBuffer(commandlist);

		//ƽ�Լ����� ������Ʈ�� �ִϸ��̼ǵ� ����Ʈ�� ������.

		commandlist->SetGraphicsRootConstantBufferView(0, jarr->Resource()->GetGPUVirtualAddress());
		//���� �׸���.

		Mesh.Render(commandlist);
	}

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
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime);//����� ƨ���� �ʴ´�.
				
			}
		}
	}
}



void CCubeManObject::EndAnimation(int nAni)
{

	if (nAni == (int)Ani_State::Attack)//�����ϱ⿴����
	{
		SetAnimation((int)Ani_State::Idle);//�����·εд�.

		m_end_attack = true;


		//���Ⱑ ����
		//cout << "Client ID: " << m_player_data.id << "Attack Animation End \n";

	}

	if (nAni == Ani_State::Dead)//�״¸���̾�����
	{
		DrawObj = false;

		Hpbar->DrawObj = false;
		HPFrame->DrawObj = false;

		SetAnimation(static_cast<int>(Ani_State::Idle));
	}
}

//------------------- �� �� ü -----------------------//

BulletCube::BulletCube(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon, XMFLOAT4 cp, bool IsMine) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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
	gamedata.Speed = 250;
	LifeTime = 3.5f;
	Master = master;
	LockOn = lockon;

	if (firstBullet)
	{
		//CreateBullet �Լ� ȣ�� - �� Ŭ���̾�Ʈ���� Ű�� ������ �ҷ��� �������� 
		if (IsMine)
		{
			if (m_IsBulletFirstCount)
			{
				myID = 0; m_IsBulletFirstCount = false;
			}
			else { ++myID; }

			BulletIDList.push_back(myID);

			m_bullet_data.my_id = myID;
			m_bullet_data.master_id = master->m_player_data.id;
			m_bullet_data.alive = true;
		}
	}
	m_bullet_type = BULLET_TYPE::protocol_LightBullet;

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(1, 1, 1);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//�躤�ͷ� ���ư�
	pp->SetMass(0.35f);

	if (ParticleList != NULL)
	{
		BulletParticles = new ParticleObject(m_Device, commandlist, ParticleList,NULL, this, 0.2f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles);

	}

}

BulletCube::~BulletCube()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;

}


void BulletCube::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
	//���� - �� ��ġ���� ���� ��ġ ���� ����
	/*
	
	LifeTime -= gt.DeltaTime();

	Orient = QuaternionMultiply(Orient, QuaternionRotation(Lookvector, MMPE_PI / 6 * gt.DeltaTime()));
	*/
}

void BulletCube::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["BulletTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

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

				if ((*i)->isShieldOn) //��밡 �ǵ� on ���¸�
					gamedata.Damage = 100; //100 ������ 1���� �ؽ��� stride������ 100�̶�� �����ϰ� ToDamage���� 1�� ����

				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);

				bool isboss = false;
			
				//2. �������͸� �Ѿ˹����� ��������.
				if ((*i)->gamedata.MAXHP > 1000)//�������͸� �Ѿ� �������� �������ؾ���.
				{
					((ImpObject*)*i)->fsm->aidata.LastPosition = this->CenterPos;
					isboss = true;
				}

				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					(*i)->isHit1 = true;
					//���ӵ� ������ ���Ѵ�. A-B
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
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
				auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
				ParticleList->push_back(BulletParticles2);

				DelObj = true;


			}
		}
	}
}

void BulletCube::Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos)
{
	//isHit~ = Ÿ���� 
	switch (coll_type)
	{
		case AFTER_COLLISION_EFFECT::EMPTY:
		{
			isHit1 = false;
			//�ҷ��� ��� �浹�� �ƹ��� �߰�����Ʈ�� �߻����� ����
			//1. ���� ���� -> ����

		}
		break;

		case AFTER_COLLISION_EFFECT::BOOM:
		{
			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			//1. �÷��̾� ���� -> ����
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE:
		{
			isHit1 = true; //Ÿ�� ����1
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//3. ���� ���� -> �÷��̾�
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM:
		{
			isHit1 = true; //Ÿ�� ����1
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//1. �÷��̾� ���� -> �÷��̾�
			//2. �÷��̾� ���� -> ����
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}

			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

	default:
		break;
	}

	DelObj = true;
	//cout << "\n";
	//cout << "In Collision Bullet ID: " << static_cast<int>(m_bullet_data.my_id) << "Is Alive: " << static_cast<int>(m_bullet_data.alive) << "\n";
}



HeavyBulletCube::HeavyBulletCube(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon, XMFLOAT4 cp, bool IsMine) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
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
	LifeTime = 3.5;
	Master = master;
	LockOn = lockon;

	if (firstBullet)
	{
		//CreateBullet �Լ� ȣ�� - �� Ŭ���̾�Ʈ���� Ű�� ������ �ҷ��� �������� 
		if (IsMine)
		{
			if (m_IsBulletFirstCount)
			{
				myID = 0; m_IsBulletFirstCount = false;
			}
			else { ++myID; }

			BulletIDList.push_back(myID);

			m_bullet_data.my_id = myID;
			m_bullet_data.master_id = master->m_player_data.id;
			m_bullet_data.alive = true;
		}
	}
	m_bullet_type = BULLET_TYPE::protocol_HeavyBullet;

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(2, 2, 2);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//�躤�ͷ� ���ư�
	pp->SetMass(1);
	if (ParticleList != NULL)
	{
		BulletParticles = new ParticleObject(m_Device, commandlist, ParticleList,NULL, this, 0.2f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles);
	
	}

}

HeavyBulletCube::~HeavyBulletCube()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;
}


void HeavyBulletCube::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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

	//pp->integrate(gt.DeltaTime());

	//No�ִϸ��̼�!

	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	/*
	LifeTime -= gt.DeltaTime();

	Orient = QuaternionMultiply(Orient, QuaternionRotation(Lookvector, MMPE_PI / 6 * gt.DeltaTime()));

	if (LifeTime <= 0)
		DelObj = true;
	*/

}

void HeavyBulletCube::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["BulletTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

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
				if ((*i)->isShieldOn) //��밡 �ǵ� on ���¸�
					gamedata.Damage = 100; //100 ������ 1���� �ؽ��� stride������ 100�̶�� �����ϰ� ToDamage���� 1�� ����

				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);
				bool isboss = false;
			
				//2. �������͸� �Ѿ˹����� ��������.
				if ((*i)->gamedata.MAXHP > 1000)//�������͸� �Ѿ� �������� �������ؾ���.
				{
					((ImpObject*)*i)->fsm->aidata.LastPosition = this->CenterPos;
					isboss = true;
				}

				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList,NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
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

				auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList,NULL, this, 0.7f, 100, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
				ParticleList->push_back(BulletParticles2);
			}
		}
	}
}

void HeavyBulletCube::Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos)
{
	switch (coll_type)
	{
		case AFTER_COLLISION_EFFECT::EMPTY:
		{
			//�ҷ��� ��� �浹�� �ƹ��� �߰�����Ʈ�� �߻����� ����
			//1. ���� ���� -> ����

		}
		break;

		case AFTER_COLLISION_EFFECT::BOOM:
		{
			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			//1. �÷��̾� ���� -> ����
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//3. ���� ���� -> �÷��̾�
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//1. �÷��̾� ���� -> �÷��̾�
			//2. �÷��̾� ���� -> ����
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}

			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

	default:
		break;
	}

	DelObj = true;
	//cout << "\n";
	//cout << "In Collision Bullet ID: " << static_cast<int>(m_bullet_data.my_id) << "Is Alive: " << static_cast<int>(m_bullet_data.alive) << "\n";
}

//-------------------- ��Ʈ����ũ ---------------------------------//

Tetris1::Tetris1(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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

	m_bullet_data.alive = true;

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
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
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


void Tetris1::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
	pp->integrate(gt.DeltaTime());

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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris1Tex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

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
				if ((*i)->isShieldOn) //��밡 �ǵ� on ���¸�
					gamedata.Damage = 100; //100 ������ 1���� �ؽ��� stride������ 100�̶�� �����ϰ� ToDamage���� 1�� ����
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);

				bool isboss = false;

				//2. �������͸� �Ѿ˹����� ��������.
				if ((*i)->gamedata.MAXHP > 1000)//�������͸� �Ѿ� �������� �������ؾ���.
				{
					((ImpObject*)*i)->fsm->aidata.LastPosition = this->CenterPos;
					isboss = true;
				}

				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList,NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
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

				auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList,NULL, this, 0.7f, 100, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
				ParticleList->push_back(BulletParticles2);
			}
		}
	}
}

void Tetris1::Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos)
{
	switch (coll_type)
	{
		case AFTER_COLLISION_EFFECT::EMPTY:
		{
			//�ҷ��� ��� �浹�� �ƹ��� �߰�����Ʈ�� �߻����� ����
			//1. ���� ���� -> ����

		}
		break;

		case AFTER_COLLISION_EFFECT::BOOM:
		{
			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			//1. �÷��̾� ���� -> ����
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//3. ���� ���� -> �÷��̾�
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//1. �÷��̾� ���� -> �÷��̾�
			//2. �÷��̾� ���� -> ����
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}

			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

	default:
		break;
	}

	DelObj = true;
}

Tetris2::Tetris2(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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

	m_bullet_data.alive = true;

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
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
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


void Tetris2::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
	pp->integrate(gt.DeltaTime());

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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris2Tex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

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
				if ((*i)->isShieldOn) //��밡 �ǵ� on ���¸�
					gamedata.Damage = 100; //100 ������ 1���� �ؽ��� stride������ 100�̶�� �����ϰ� ToDamage���� 1�� ����
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);

				bool isboss = false;

				//2. �������͸� �Ѿ˹����� ��������.
				if ((*i)->gamedata.MAXHP > 1000)//�������͸� �Ѿ� �������� �������ؾ���.
				{
					((ImpObject*)*i)->fsm->aidata.LastPosition = this->CenterPos;
					isboss = true;
				}

				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList,NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
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
				auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList,NULL, this, 0.7f, 100, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
				ParticleList->push_back(BulletParticles2);

			}
		}
	}
}

void Tetris2::Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos)
{
	switch (coll_type)
	{
		case AFTER_COLLISION_EFFECT::EMPTY:
		{
			//�ҷ��� ��� �浹�� �ƹ��� �߰�����Ʈ�� �߻����� ����
			//1. ���� ���� -> ����

		}
		break;

		case AFTER_COLLISION_EFFECT::BOOM:
		{
			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			//1. �÷��̾� ���� -> ����
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//3. ���� ���� -> �÷��̾�
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//1. �÷��̾� ���� -> �÷��̾�
			//2. �÷��̾� ���� -> ����
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}

			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

	default:
		break;
	}

	DelObj = true;
}

Tetris3::Tetris3(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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

	m_bullet_data.alive = true;

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
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
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


void Tetris3::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
	pp->integrate(gt.DeltaTime());

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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris3Tex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

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
			
			if ((*i)->isShieldOn) //��밡 �ǵ� on ���¸�
				gamedata.Damage = 100; //100 ������ 1���� �ؽ��� stride������ 100�̶�� �����ϰ� ToDamage���� 1�� ����
			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);

				bool isboss = false;

				//2. �������͸� �Ѿ˹����� ��������.
				if ((*i)->gamedata.MAXHP > 1000)//�������͸� �Ѿ� �������� �������ؾ���.
				{
					((ImpObject*)*i)->fsm->aidata.LastPosition = this->CenterPos;
					isboss = true;
				}

				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList,NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}


				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�� �ӵ��� �����.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);
				DelObj = true;
				auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList,NULL ,this, 0.7f, 100, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
				ParticleList->push_back(BulletParticles2);

			}
		}
	}
}

void Tetris3::Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos)
{
	switch (coll_type)
	{
		case AFTER_COLLISION_EFFECT::EMPTY:
		{
			//�ҷ��� ��� �浹�� �ƹ��� �߰�����Ʈ�� �߻����� ����
			//1. ���� ���� -> ����

		}
		break;

		case AFTER_COLLISION_EFFECT::BOOM:
		{
			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			//1. �÷��̾� ���� -> ����
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//3. ���� ���� -> �÷��̾�
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//1. �÷��̾� ���� -> �÷��̾�
			//2. �÷��̾� ���� -> ����
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}

			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

	default:
		break;
	}

	DelObj = true;
}


Tetris4::Tetris4(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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

	m_bullet_data.alive = true;

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
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
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


void Tetris4::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
	pp->integrate(gt.DeltaTime());

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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris4Tex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

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

			if ((*i)->isShieldOn) //��밡 �ǵ� on ���¸�
				gamedata.Damage = 100; //100 ������ 1���� �ؽ��� stride������ 100�̶�� �����ϰ� ToDamage���� 1�� ����
			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);
				bool isboss = false;

				//2. �������͸� �Ѿ˹����� ��������.
				if ((*i)->gamedata.MAXHP > 1000)//�������͸� �Ѿ� �������� �������ؾ���.
				{
					((ImpObject*)*i)->fsm->aidata.LastPosition = this->CenterPos;
					isboss = true;
				}

				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList,NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}


				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�� �ӵ��� �����.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);

				DelObj = true;
				auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList,NULL, this, 0.7f, 100, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
				ParticleList->push_back(BulletParticles2);

			}
		}
	}
}

void Tetris4::Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos)
{
	switch (coll_type)
	{
		case AFTER_COLLISION_EFFECT::EMPTY:
		{
			//�ҷ��� ��� �浹�� �ƹ��� �߰�����Ʈ�� �߻����� ����
			//1. ���� ���� -> ����

		}
		break;

		case AFTER_COLLISION_EFFECT::BOOM:
		{
			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			//1. �÷��̾� ���� -> ����
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//3. ���� ���� -> �÷��̾�
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//1. �÷��̾� ���� -> �÷��̾�
			//2. �÷��̾� ���� -> ����
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}

			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

	default:
		break;
	}

	DelObj = true;
}

Tetrike::Tetrike(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, list<CGameObject*>*Bulletlist, CGameObject* master, CGameObject* lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "Tetrike", L"textures/effect/magicCircle.dds", false);
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


void Tetrike::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
				Blist->push_back(new Tetris1(device, commandlist, ParticleList,NULL, Master, NULL, pos));
			else if (g == 1)
			{
				auto t = new Tetris2(device, commandlist, ParticleList,NULL, Master, NULL, pos);
				int k = (rand()*(int)(gt.TotalTime() * 32524)) % 2;

				if (k == 0)
					t->Orient = QuaternionMultiply(t->Orient, QuaternionRotation(XMFLOAT3(1, 0, 0), MMPE_PI / 2));
				Blist->push_back(t);
			}
			else if (g == 2)
			{
				auto t = new Tetris3(device, commandlist, ParticleList,NULL, Master, NULL, pos);
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
				auto t = new Tetris4(device, commandlist, ParticleList,NULL, Master, NULL, pos);
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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetrike"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}


DiceStrike::DiceStrike(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, CGameObject * master, XMFLOAT4& ori, float degree, CGameObject * lockon, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	bool firstBullet = true;
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "DiceStrike", L"textures/effect/fire.dds", false);
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

	m_degree = degree;
	Orient = QuaternionMultiply(Orient, ori);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, degree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();

	obs = Bullet;

	ObjData.isAnimation = 0;
	ObjData.Scale = 2.0;
	ObjData.SpecularParamater = 0.2f;//����ŧ���� �����ش�.


	if (firstBullet)
	{
		if (m_IsBulletFirstCount)
		{
			myID = 0; m_IsBulletFirstCount = false;
		}
		else { ++myID; }
		BulletIDList.push_back(myID);

		m_bullet_data.my_id = myID;
		m_bullet_data.master_id = master->m_player_data.id;
		m_bullet_data.alive = true;
	}
	m_bullet_type = BULLET_TYPE::protocol_DiceBullet;
									 //���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 20;
	gamedata.GodMode = true;
	gamedata.Speed = 100;
	LifeTime = 5.5f;
	Master = master;
	LockOn = lockon;


	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(2, 0, 0);
	XMFLOAT3 ry(0, 2, 0);
	XMFLOAT3 rz(0, 0, 2);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(2, 2, 2);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//�躤�ͷ� ���ư�
	pp->SetMass(0.35f);

	if (ParticleList != NULL)
	{
		BulletParticles = new ParticleObject(m_Device, commandlist, ParticleList,NULL, this, 0.2f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles);
		BulletParticles2 = new ParticleObject(m_Device, commandlist, ParticleList,NULL, this, 0.3f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles2);
	}


}

DiceStrike::~DiceStrike()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;
	if (BulletParticles2 != NULL)
		BulletParticles2->DelObj = true;
}

void DiceStrike::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 2, 2, 2);

	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void DiceStrike::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);

	Mat.MatData.Emissive = XMFLOAT4{ 0.72f, 0.73f, 0.73f, 0.7f };
	Mat.MatData.Roughness = 0.2f;
}

void DiceStrike::Tick(const GameTimer & gt)
{
	/*
	pp->integrate(gt.DeltaTime());

	Orient = QuaternionMultiply(Orient, QuaternionRotation(Lookvector, MMPE_PI * gt.DeltaTime()));

	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	LifeTime -= gt.DeltaTime();

	if (LifeTime <= 0)
		DelObj = true;
	*/
}

void DiceStrike::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["DiceStrike"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}

void DiceStrike::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	//CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	
	/*
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);

				bool isboss = false;

				//2. �������͸� �Ѿ˹����� ��������.
				if ((*i)->gamedata.MAXHP > 1000)//�������͸� �Ѿ� �������� �������ؾ���.
				{
					((ImpObject*)*i)->fsm->aidata.LastPosition = this->CenterPos;
					isboss = true;
				}

				//���� ��밡 �׾��ٸ� ������ �߰��Ѵ�.
				if ((*i)->gamedata.HP <= 0)
				{
					if (((CCubeManObject*)Master)->player != NULL)
					{
						if (isboss)
							((CCubeManObject*)Master)->player->pointrank.Point += 100;
						else
							((CCubeManObject*)Master)->player->pointrank.Point += 10;
					}
				}

				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//���ӵ� ������ ���Ѵ�. A-B
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList,NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
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
				auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList,NULL, this, 0.7f, 100, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
				ParticleList->push_back(BulletParticles2);

			}
		}
	}
	*/
}

void DiceStrike::Collision(int coll_type, int damage, const XMFLOAT4 & bullet_pos, const XMFLOAT4 & target_pos)
{
	switch (coll_type)
	{
		case AFTER_COLLISION_EFFECT::EMPTY:
		{
			isHit1 = false;
			//�ҷ��� ��� �浹�� �ƹ��� �߰�����Ʈ�� �߻����� ����
			//1. ���� ���� -> ����

		}
		break;

		case AFTER_COLLISION_EFFECT::BOOM:
		{
			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			//1. �÷��̾� ���� -> ����
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE:
		{
			isHit1 = true; //Ÿ�� ����1
						   // ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
						   //3. ���� ���� -> �÷��̾�
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM:
		{
			isHit1 = true; //Ÿ�� ����1
						   // ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
						   //1. �÷��̾� ���� -> �÷��̾�
						   //2. �÷��̾� ���� -> ����
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}

			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

	default:
		break;
	}

	DelObj = true;
}



//---------------------- ����ƽ ������Ʈ -----------------------------//

SphereObject::SphereObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "MapTex", L"textures/sky/sunsetcube1024.dds", true);
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


void SphereObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["MapTex"].get()->Resource.Get(), 1);
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}



//////////////

CubeObject::CubeObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist,  list<CGameObject*>*shadow, float degree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
	   
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "cubeTex", L"textures/object/staticcubes.dds", false);

		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);

		CreateMesh = true;
		srand(time(NULL));
	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	auto q = XMLoadFloat4(&Orient);//������ degree��ŭ ������ �Ѵ�.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, degree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.Scale = 3.2f;
	ObjData.SpecularParamater = 0.2f;//����ŧ���� �����ش�.

	//ť�� ���� ���� ��ġ //ť�길 ����
	ObjData.CustomData1.w = rand() % 400 + 100;
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
	XMFLOAT3 rx(5, 0, 0);
	XMFLOAT3 ry(0, 5, 0);
	XMFLOAT3 rz(0, 0, 5);
	auto rqx = QuaternionRotation(raxis, degree);
	rx = Vector3Rotation(rx, rqx);

	
	auto rqy = QuaternionRotation(raxis, degree);
	ry = Vector3Rotation(ry, rqy);

	
	auto rqz = QuaternionRotation(raxis, degree);
	rz = Vector3Rotation(rz, rqz);

	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(5, 5, 5);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.

	
	if (Shadow != NULL)
	{
		s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(1, 1, 1), 1, degree, CenterPos);
		s->ObjData.Scale = 10.0f;
		Shadow->push_back(s);
	}
}

CubeObject::~CubeObject()
{
	if (s != NULL)
		s->DelObj = true;
}

void CubeObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	//CreateCube(&Mesh, 1, 1, 1);
	LoadMD5Model(L".\\�÷��̾�޽���\\staticcube1.MD5MESH", &Mesh, 0, 1);
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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["cubeTex"].get()->Resource.Get());

	
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}

MoveCubeObject::MoveCubeObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, float len, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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

	Len = len;

	n = rand() % 30;
	

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

	obs = Static;
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
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(10, 5, 10);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.
	
	if (Shadow != NULL)
	{
		s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(2, 1, 2), 7, 0, CenterPos);
		s->ObjData.Scale = 10.0f;
		Shadow->push_back(s);
	}
}

MoveCubeObject::~MoveCubeObject()
{
	if (s != NULL)
		s->DelObj = true;
}


void MoveCubeObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
	/*
	n += gt.DeltaTime();

	CenterPos.y = Len * sinf(MMPE_PI * n * 0.15f)+ 50;
	*/
}

void MoveCubeObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size() > 0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), 0, TexOff);

	UpdateConstBuffer(commandlist, false);

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
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime);//����� ƨ���� �ʴ´�.
				
			}
		}
	}
}

/////////////

GridObject::GridObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float size, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "GridTex", L"textures/object/1floor.dds", false,2,0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "GridNormalTex", L"textures/object/1floorN.dds", false,2,1);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	ObjData.isAnimation = 0;
	ObjData.Scale = size;
	ObjData.SpecularParamater = 0.4f;//����ŧ���� �����ش�.
	ObjData.CustomData1.w = 1234;
	obs = Static;
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;

}

void GridObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	CreatePentagon(&Mesh, 1200.0f, CenterPos.y);
	Mesh.SetNormal();
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

	if (Textures.size() > 0)
	{
		SetTexture(commandlist, SrvDescriptorHeap, Textures["GridTex"].get()->Resource.Get(),0,0);
		SetTexture(commandlist, SrvDescriptorHeap, Textures["GridNormalTex"].get()->Resource.Get(),2,1);
	}

	UpdateConstBuffer(commandlist, false);

	Mat.UpdateConstantBuffer(commandlist);
	//���� �׸���.

	Mesh.Render(commandlist);
}

///////////////////////////////////

//HPBar������Ʈ
BarObject::BarObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, CGameObject* master, float size, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = size;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 1;

	Master = master;
	ObjData.CustomData1.y = Master->gamedata.HP;
	YPos = CenterPos.y;

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

void BarObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
	CenterPos.x = Master->CenterPos.x; CenterPos.y = Master->CenterPos.y + YPos; CenterPos.z = Master->CenterPos.z;

	//cout << "CumstomData1.y: " << ObjData.CustomData1.y << "\n";
}

void BarObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (DrawObj)
	{
		if (Textures.size()>0)
			SetTexture(commandlist, SrvDescriptorHeap, Textures["HPTex"].get()->Resource.Get());
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
}


//HPbar Ʋ
BarFrameObject::BarFrameObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, CGameObject * master, float size, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = size;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 4;

	Master = master;
	YPos = CenterPos.y;

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

void BarFrameObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
	CenterPos.x = Master->CenterPos.x; CenterPos.y = Master->CenterPos.y + YPos; CenterPos.z = Master->CenterPos.z;
}

void BarFrameObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (DrawObj)
	{
		if (Textures.size()>0)
			SetTexture(commandlist, SrvDescriptorHeap, Textures["HPFrameTex"].get()->Resource.Get());
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
}


Rank1Object::Rank1Object(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, CGameObject * master, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 10;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 3;

	Master = master;

	//���Ӱ��� �����͵�
	gamedata.HP = 0;
	gamedata.GodMode = true;
	staticobject = true;


	obs = Static;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "rankTex", L"textures/ui/rank1.dds", false);

		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void Rank1Object::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
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

void Rank1Object::Tick(const GameTimer & gt)
{
	CenterPos.x = Master->CenterPos.x; CenterPos.y = Master->CenterPos.y + 20; CenterPos.z = Master->CenterPos.z;
}

void Rank1Object::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["rankTex"].get()->Resource.Get());
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

DiceObject::DiceObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, CGameObject * master, XMFLOAT3 goal, list<CGameObject*>* bulletlist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 5.0f;
	ObjData.SpecularParamater = 0.5f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 0;
	LifeTime = 3.0f;


	Device = m_Device;
	Commandlist = commandlist;
	plist = Plist;
	Master = master;
	Bulletlist = bulletlist;
	YPos = CenterPos.y;
	
	V = goal;

	//���Ӱ��� �����͵�
	gamedata.GodMode = true;
	staticobject = true;
	obs = UI;
	

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "DiceTex", L"textures/effect/dice.dds", false);

		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
}

void DiceObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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

void DiceObject::Tick(const GameTimer & gt)
{
	LifeTime -= gt.DeltaTime();

	if (LifeTime <= 0)
	{		
		if (TexStart == 0)
			Dicedata = 1;
		else if (TexStart == 1)
			Dicedata = 4;
		else if (TexStart == 2)
			Dicedata = 2;
		else if (TexStart == 3)
			Dicedata = 5;
		else if (TexStart == 4)
			Dicedata = 3;

		//�ù�, �̰� ���ߵŴµ�, �ù�, �̰Ŷ����� ��ð��̾�, �ù�
		//Master->SetAnimation(2);

		//���� �躤�Ϳ� ���ο� �躤�͸� �����ؼ� �������� ���Ѵ�.

		auto v = Float3Add(V, XMFloat4to3(Master->CenterPos), false);

		v = Float3Normalize(Master->Lookvector);//���ο� �躤��(�߻����)
							   //���⼭ �躤�Ͷ� ����, �÷��̾ �ƴ϶�, �Ѿ��� �躤�ʹ�. ��� ������Ʈ�� ���� �躤�ʹ� 0,0,1 �Ǵ� 0,0,-1 �ε�, ���ư��� ������ �ٶ󺸵���  
							   //�ؾ��ϹǷ� ���ο� �躤�͸� �ʿ�� �ϴ°��̴�.  


		XMFLOAT3 l{ 0,0,1 };
		XMVECTOR ol = XMLoadFloat3(&l);
		XMVECTOR nl = XMLoadFloat3(&v);
		auto axis = XMVector3Cross(ol, nl);
		//�������� �ϼ�.
		axis = XMVector3Normalize(axis);
		XMFLOAT3 Axis;
		XMStoreFloat3(&Axis, axis);
		//���� ȸ�������� ���ؾ��Ѵ�. ������ ���� ȸ�������� ���Ѵ�.

		auto temp = XMVector3Dot(ol, nl);

		float d;//���� �躤�Ϳ� ���ο� �躤�͸� ������ ���.
		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������

		auto ori = QuaternionRotation(Axis, d);

		//��¥ �躤�͸� �������� ���� ��¥ Right���͸� ���Ѵ�. ��¥ Up�� ��¥ ��� ��¥ ����Ʈ�� �������ϸ��.
		//ȸ���� �������ش�. ȸ���࿡�� �躤�͸� �����ϸ� ��¥ Right���Ͱ� ���´�.
		//�̶� ������ �ִ� RightVector�� ��¥ RightVector�� ���հ��� ����ϰ�
		//�躤�͸� ȸ�������� �����ش�. �� ȸ���࿡ ������ ����°�?
		//�����ϴ� ���� �躤�Ϳ� �߻������ �躤���� ȸ�����
		//���� RightVector�� �߻������ ȸ�������ϸ� ���� �ٸ��� ���´�.
		//������ ���� �׳� �̴�� �Ѿ�� �Ǹ� RightVector�� ��¥ RightVector�� ������ŭ ������ ����Ƿ�
		//�̷��ԵǸ� ���߿� Up�� ���Ҷ��� ������ �����.
		//��� X�� Y�� Z�� ������� �����ϸ� �̷��� ������ ���Ǿ�����, Ư������ ������� ȸ���Ҷ� ����� ������.

		auto wmatrix = XMMatrixIdentity();
		auto quater = XMLoadFloat4(&ori);
		wmatrix *= XMMatrixRotationQuaternion(quater);

		auto orr = XMVectorSet(1, 0, 0, 0);
		orr = XMVector4Transform(orr, wmatrix);//��¥ ����Ʈ ����
		orr = XMVector3Normalize(orr);
		auto RealRight = XMVector3Cross(axis, nl);//��¥ ����Ʈ����
		RealRight = XMVector3Normalize(RealRight);

		//��¥ ����Ʈ ���Ϳ� ��¥ ����Ʈ ���͸� ������.
		temp = XMVector3Dot(RealRight, orr);

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������
		auto ori2 = XMQuaternionRotationAxis(nl, d);//��¥ �躤�͸� ȸ�������� ��� ��¥����Ʈ�� ��¥����Ʈ�� ���հ���ŭȸ��

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//���� ȸ�� ����

		//cout << "CGamObject Master Lookvector xyz: " << Master->Lookvector.x << " , " << Master->Lookvector.y << " , " << Master->Lookvector.z << "\n";
		//cout << "\n";

		if (Dicedata == 1)
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, 0, NULL, Master->CenterPos));

		else if (Dicedata == 2)
		{
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, MMPE_PI / 18, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, -MMPE_PI / 18, NULL, Master->CenterPos));
		}
		else if (Dicedata == 3)
		{
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, 0, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, MMPE_PI / 9, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, -MMPE_PI / 9, NULL, Master->CenterPos));
		}
		else if (Dicedata == 4)
		{
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, MMPE_PI / 18, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, -MMPE_PI / 18, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, MMPE_PI / 9, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, -MMPE_PI / 9, NULL, Master->CenterPos));
		}
		else if (Dicedata == 5)
		{
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, 0, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, MMPE_PI / 18, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, -MMPE_PI / 18, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist,NULL, Master, ori, MMPE_PI / 9, NULL, Master->CenterPos));
			Bulletlist->push_back(new DiceStrike(Device, Commandlist, plist, NULL, Master,ori, -MMPE_PI / 9, NULL, Master->CenterPos));
		}

		DelObj = true;

		//�����߰�
		//�������� ���� Ȯ���� ���̽������� ����
		g_numofdice = Dicedata;
	}

	dTime = rand()%11;	
	ObjData.TexClamp = XMFLOAT4(0.0f + (0.2f*TexStart), 0.2f + (0.2f*TexStart), 0, 0);	
	CenterPos.x = Master->CenterPos.x; CenterPos.y = Master->CenterPos.y+20; CenterPos.z = Master->CenterPos.z;
	
	if (dTime % 3 == 0 && LifeTime >= 1.0f)
		TexStart++;
	
	if (TexStart > 4)
		TexStart = 0;
}

void DiceObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["DiceTex"].get()->Resource.Get());

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


DamageObject::DamageObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float Damaged, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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

void DamageObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["DamageTex"].get()->Resource.Get());

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

RigidCubeObject::RigidCubeObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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
	ObjData.SpecularParamater =1.f;//����ŧ���� �����ش�.

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

	//if (Shadow != NULL)
	//{
	//	s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(1, 1, 1), 1, CenterPos);
	//	s->ObjData.Scale = 20.0f;
	//	Shadow->push_back(s);
	//}
}

RigidCubeObject::~RigidCubeObject()
{
	//if (s != NULL)
	//	s->DelObj = true;
}

void RigidCubeObject::Tick(const GameTimer & gt)
{
	/*
	if (rb != NULL)
		rb->integrate(gt.DeltaTime());
	*/
}

void RigidCubeObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

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
							*(*i)->pp->CenterPos=ppConvertrb.GetPosition();
							(*i)->pp->SetAccel(ppConvertrb.GetAccel());
							auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList,NULL, this, 0.7f, 100, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
							ParticleList->push_back(BulletParticles2);
						}
						else
						{
							//�浹ó�� ����� �׳� ��ħ�κи� �ؼ���

							rb->AmendTime = 0;
							rb->ResolvePenetration(ppConvertrb, DeltaTime);
							(*i)->pp->SetVelocity(ppConvertrb.GetVelocity());
							*(*i)->pp->CenterPos=ppConvertrb.GetPosition();
							(*i)->pp->SetAccel(ppConvertrb.GetAccel());
							
						}
						

					}

				}
			}
		}
	}
}



//================================��������Ʈ=======================================

SmallWallObject::SmallWallObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "SmallWall", L"textures/object/smallWall.dds", false);
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
	auto q2 = QuaternionRotation(axis, degree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 5.5f;
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
	
	
	XMFLOAT3 ry(0, 10, 0);
	

	XMFLOAT3 rz(0, 0, 5);
	
	auto rqx = QuaternionRotation(raxis, degree);
	rx = Vector3Rotation(rx, rqx);


	auto rqy = QuaternionRotation(raxis, degree);
	ry = Vector3Rotation(ry, rqy);


	auto rqz = QuaternionRotation(raxis, degree);
	rz = Vector3Rotation(rz, rqz);

	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(20, 10, 5);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.

	if (Shadow != NULL)
	{
		s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(40, 20, 10), 4, degree, CenterPos);
		s->ObjData.Scale = 5.5f;
		Shadow->push_back(s);
	}
}

SmallWallObject::~SmallWallObject()
{
	if (s != NULL)
		s->DelObj = true;
}

void SmallWallObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	//CreateCube(&Mesh, 40, 20, 10);

	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\fence.MD5MESH", &Mesh, 0, 1);
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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["SmallWall"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}




BigWallObject::BigWallObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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
	auto q2 = QuaternionRotation(axis, degree);
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
	

	XMFLOAT3 ry(0, 50, 0);
	

	XMFLOAT3 rz(0, 0, 5);
	auto rqx = QuaternionRotation(raxis, degree);
	rx = Vector3Rotation(rx, rqx);


	auto rqy = QuaternionRotation(raxis, degree);
	ry = Vector3Rotation(ry, rqy);


	auto rqz = QuaternionRotation(raxis, degree);
	rz = Vector3Rotation(rz, rqz);

	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(350, 50, 5);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.

	//if (Shadow != NULL)
	//{
	//	s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(700, 100, 10), 1, Orient, CenterPos);
	//	s->ObjData.Scale = 2.0f;
	//	Shadow->push_back(s);
	//}
}

BigWallObject::~BigWallObject()
{
	/*if (s != NULL)
		s->DelObj = true;*/
}

void BigWallObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Wall"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}


ColumnObject::ColumnObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "CubeTex", L"textures/object/column.dds", false);
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
	auto q2 = QuaternionRotation(axis, degree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 6.0f;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

	//��ָ���
	ObjData.CustomData1.w = rand() % 400 + 100;

	//���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;
	obs = Static;


	XMFLOAT3 raxis{ 0,1,0 };
	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(15, 0, 0);
	

	XMFLOAT3 ry(0, 45, 0);
	

	XMFLOAT3 rz(0, 0, 15);
	auto rqx = QuaternionRotation(raxis, degree);
	rx = Vector3Rotation(rx, rqx);


	auto rqy = QuaternionRotation(raxis, degree);
	ry = Vector3Rotation(ry, rqy);


	auto rqz = QuaternionRotation(raxis, degree);
	rz = Vector3Rotation(rz, rqz);

	rco.SetPlane(rx, ry, rz);


	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(15, 45, 15);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.

	if (Shadow != NULL)
	{
		s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(30, 90, 30), 6, degree, CenterPos);
		s->ObjData.Scale = 6.0f;
		Shadow->push_back(s);
	}
}

ColumnObject::~ColumnObject()
{
	if (s != NULL)
		s->DelObj = true;
}

void ColumnObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	//CreateCube(&Mesh, 30, 90, 30);
	LoadMD5Model(L".\\�÷��̾�޽���\\mCol.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void ColumnObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void ColumnObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}


BuildingObject::BuildingObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "towerTex", L"textures/object/castle2.dds", false, 1, 0);
		
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
	auto q2 = QuaternionRotation(axis, degree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 22.0f;
	ObjData.SpecularParamater = 16.31f;//����ŧ���� �����ش�.
	ObjData.CustomData1.w = 1234;


									   //���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;
	obs = Static;


	XMFLOAT3 raxis{ 0,1,0 };
	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(15, 0, 0);
	

	XMFLOAT3 ry(0, 35, 0);
	

	XMFLOAT3 rz(0, 0, 15);
	auto rqx = QuaternionRotation(raxis, degree);
	rx = Vector3Rotation(rx, rqx);


	auto rqy = QuaternionRotation(raxis, degree);
	ry = Vector3Rotation(ry, rqy);


	auto rqz = QuaternionRotation(raxis, degree);
	rz = Vector3Rotation(rz, rqz);

	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(15, 35, 15);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.

	if (Shadow != NULL)
	{
		s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(30, 90, 30), 5, degree, CenterPos);
		s->ObjData.Scale = 22.0f;
		Shadow->push_back(s);
	}
}

BuildingObject::~BuildingObject()
{
	if (s != NULL)
	s->DelObj = true;
}

void BuildingObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{

	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\fcastle.MD5MESH", &Mesh, 0, 1);
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

	if (Textures.size() > 0)
	{
		SetTexture(commandlist, SrvDescriptorHeap, Textures["towerTex"].get()->Resource.Get(), 0, 0);
		//SetTexture(commandlist, SrvDescriptorHeap, Textures["towerNTex"].get()->Resource.Get(), 2, 1);
	}
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}
//�Ƕ�̵�

Rock1Object::Rock1Object(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "CubeTex", L"textures/object/whiterock2.dds", false);
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
	auto q2 = QuaternionRotation(axis, degree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater =-1.f;//����ŧ���� �����ش�.



									 //���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;
	obs = Static;
	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(5.5, 0, 0);
	XMFLOAT3 ry(0, 1.5, 0);
	XMFLOAT3 rz(0, 0, 5);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(5.5, 2, 5);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.

}

void Rock1Object::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	

	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\rock1.MD5MESH", &Mesh, 0, 5);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void Rock1Object::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void Rock1Object::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}




//���� ������Ʈ

RangeObject::RangeObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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

void RangeObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	CreateCube(&Mesh, 60, 0.01, 60);

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
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}


////////////////////
/*
���� ������ ��ƼŬ ���� : 1

�̸� : ParticleObject
���� : ������ �Һ����� ó�� �ణ ������ ���� ������� ���ư�. ����ü�� �����ϱ� ����.
������� : �ϴ� ����ü�� �躤���� �ݴ�������� ���ϴ°� �⺻���� �������� ��ƾ��Ѵ�.
���� �������� �� �������� ���ư� ������ ��������Ѵ�.
�̸����� ���� ���ؽ��� ��ְ����ٰ� ���ư� �ӵ��� �־�����. �� �̸��� ����ϻ� ���� ���ư� �ӵ���.
xyz�� cos�� sin, �������� �̿��� ���� ���ư� �������� ������������
w�� ��� ���۵Ǵ� ������ ������. ���Ҹ��ĸ� ��ƼŬ�� �� ��ġ�� ������ �ӵ� * �귯�� �ð����� ��ġ�� �ٲ۴�.
������ �귯�� �ð��� 0���� �����ϰԵǸ� �ϰ������� ������ġ�� �����ϹǷ� ���� ����ϰ� �ȴ�. ���� ����� ��ĭ�� �þ��...
�׷��� �귯���ð����ٰ� ������ �������� �־���. �׸��� �귯���ð��� frac�� ���ؼ� 0~1������ ������ ������.
�̷μ� �� ��ƼŬ�� 0~1������ ���� �ð��� ���Եǰ�, �� �ð��� ���� �ٸ��Ƿ� ����� 0�� ��ġ�� �ְ�, ����� �߰���ġ�� �ְ� �ϰ�
�ǹǷ� ���� �ڿ������� ��ƼŬ ǥ���� ��.

�� ��ƼŬ�� ������Ÿ���� ���� �ʿ����. �ֳ��ϸ� PTime�� frac�� �س��� �˾Ƽ� 0~1������ �����̵ȴ�.
�ٸ� �� �����Ӹ��� ������ �� �Ѿ��� �����̹Ƿ�, �� �Ѿ��� ��ġ�� �Ѿư����� CenterPos�� ���Ÿ� ���ش�.

*/
////////////////////
ParticleObject::ParticleObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, float LifeTime, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
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
	gamedata.Speed = 20;
	staticobject = true;
	obs = UI;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "sparkTex", L"textures/effect/fire.dds", false);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}

	Lookvector.x = -(Master->Lookvector.x); Lookvector.y = -(Master->Lookvector.y); Lookvector.z = -(Master->Lookvector.z);
	Lookvector = Float3Normalize(Lookvector);

	XMFLOAT3 Vel = XMFLOAT3(Lookvector.x * gamedata.Speed, Lookvector.y * gamedata.Speed, Lookvector.z * gamedata.Speed);
	ParticleTime = 0;
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

void ParticleObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	UINT numOfParticle = 200;

	Mesh.SubResource = new CVertex[numOfParticle];
	Mesh.nVertex = numOfParticle;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT[numOfParticle];
	Mesh.nindex = numOfParticle;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);


	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
	for (int i = 0; i < numOfParticle; ++i)
	{

		Mesh.SubResource[i].V.x = 0;
		Mesh.SubResource[i].V.y = 0;
		Mesh.SubResource[i].V.z = 0;

		Mesh.SubResource[i].N.x = 0 + (cosf(MMPE_PI / 180 * i*(float)(rand() % 10000)));
		Mesh.SubResource[i].N.y = 0 + (sinf(MMPE_PI / 180 * i*(float)(rand() % 10000)));
		Mesh.SubResource[i].N.z = (cosf(MMPE_PI / 180 * i*(float)(rand() % 10000)));
		Mesh.SubResource[i].N.w = (float)(rand() % 100);
		Mesh.Index[i] = i;

	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void ParticleObject::Tick(const GameTimer & gt)
{
	//�������Ӹ��� �������� ��ġ�� ����.

	CenterPos = Master->CenterPos;
	//���� �⺻��ƼŬ������Ʈ�� PTime�� ���� ��ġ�� �����.
	ObjData.PTime += gt.DeltaTime();

}

void ParticleObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["sparkTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist,false);


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


ParticleObject2::ParticleObject2(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, CGameObject * master, float lifeTime, float num, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	MinDepth = true;

	ObjData.isAnimation = 0;
	ObjData.Scale = 6.5f;
	ObjData.SpecularParamater = 0.3f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 10;
	Master = master;
	LifeTime = lifeTime;

	//���Ӱ��� �����͵�
	gamedata.MAXHP = 0;
	gamedata.HP = 0;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 20;
	staticobject = true;
	obs = UI;

	if (CreateMesh == false)	
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "sparkTex", L"textures/effect/Stars.dds", false);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}

	Lookvector = XMFLOAT3(0, 1, 0);
	Lookvector = Float3Normalize(Lookvector);

	XMFLOAT3 Vel = XMFLOAT3(Lookvector.x * gamedata.Speed, Lookvector.y * gamedata.Speed, Lookvector.z * gamedata.Speed);
	ParticleTime = 0;
	ObjData.Velocity = Vel;

	auto q = XMLoadFloat4(&Orient);
	XMFLOAT3 axis = { 0,1,0 };


	UpdateLookVector();
}

void ParticleObject2::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	UINT numOfParticle = 10;

	Mesh.SubResource = new CVertex[numOfParticle];
	Mesh.nVertex = numOfParticle;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT[numOfParticle];
	Mesh.nindex = numOfParticle;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);


	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
	for (int i = 0; i < numOfParticle; ++i)
	{

		Mesh.SubResource[i].V.x = 0;
		Mesh.SubResource[i].V.y = 0;
		Mesh.SubResource[i].V.z = 0;

		Mesh.SubResource[i].N.x = 0 + (cosf(MMPE_PI / 180 * i*(float)(rand() % 10000)));
		Mesh.SubResource[i].N.y = 0;// +(sinf(MMPE_PI / 180 * i*(float)(rand() % 10000)));
		Mesh.SubResource[i].N.z = (cosf(MMPE_PI / 180 * i*(float)(rand() % 10000)));
		Mesh.SubResource[i].N.w = (float)(rand() % 100);
		Mesh.Index[i] = i;

	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void ParticleObject2::Tick(const GameTimer & gt)

{
	if (LifeTime <= 0)
		DelObj = true;
	else
		LifeTime -= gt.DeltaTime();
	//���� �⺻��ƼŬ������Ʈ�� PTime�� ���� ��ġ�� �����.
	ObjData.PTime += gt.DeltaTime();
}


void ParticleObject2::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["sparkTex"].get()->Resource.Get());
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


ParticleObject3::ParticleObject3(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, CGameObject * master, float lifeTime, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 0.5f;
	ObjData.SpecularParamater = 0.3f;//����ŧ���� �����ش�.
	ObjData.CustomData1.x = 11;
	Master = master;
	LifeTime = lifeTime;

	//���Ӱ��� �����͵�
	gamedata.MAXHP = 0;
	gamedata.HP = 0;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 20;
	staticobject = true;
	obs = UI;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "sparkTex", L"textures/effect/fire.dds", false);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}

	Lookvector = XMFLOAT3(0, 1, 0);
	Lookvector = Float3Normalize(Lookvector);

	XMFLOAT3 Vel = XMFLOAT3(0, 0, 0);
	ParticleTime = 0;
	ObjData.Velocity = Vel;

	auto q = XMLoadFloat4(&Orient);
	XMFLOAT3 axis = { 0,1,0 };


	UpdateLookVector();
}

void ParticleObject3::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	UINT numOfParticle = 180;

	Mesh.SubResource = new CVertex[numOfParticle];
	Mesh.nVertex = numOfParticle;
	Mesh.nStride = sizeof(CVertex);
	Mesh.nOffset = 0;


	Mesh.Index = new UINT[numOfParticle];
	Mesh.nindex = numOfParticle;
	Mesh.nioffset = 0;
	Mesh.nisize = sizeof(UINT);


	//���⼭ ��ǥ�� �ϰ������� ���� �� �� �ִ�
	for (int i = 0; i < numOfParticle; ++i)
	{

		Mesh.SubResource[i].V.x = 0;
		Mesh.SubResource[i].V.y = 0;
		Mesh.SubResource[i].V.z = 0;

		Mesh.SubResource[i].N.x = 0 + (cosf(MMPE_PI / numOfParticle * i*(float)(rand() % 10000)));
		Mesh.SubResource[i].N.y = 0 + (sinf(MMPE_PI / numOfParticle * i*(float)(rand() % 10000)));
		Mesh.SubResource[i].N.z = (cosf(MMPE_PI / numOfParticle * i*(float)(rand() % 10000)));
		Mesh.SubResource[i].N.w = (float)(rand() % numOfParticle);
		Mesh.Index[i] = i;

	}

	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void ParticleObject3::Tick(const GameTimer & gt)
{
	CenterPos = Master->CenterPos;
	//���� �⺻��ƼŬ������Ʈ�� PTime�� ���� ��ġ�� �����.
	ObjData.PTime += gt.DeltaTime();
}

void ParticleObject3::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["sparkTex"].get()->Resource.Get());
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

ShieldArmor::ShieldArmor(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, CGameObject * master, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	if (CreateMesh)
		m_player_data.id = master->m_player_data.id;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "ShieldTex", L"textures/effect/shield.dds", false);
		SetMesh(m_Device, commandlist);
		CreateMesh = true;

	}
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.3f;//����ŧ���� �����ش�.
	Blending = true;
	ObjData.BlendValue = 0.3f;
	Master = master;

	//���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;
	obs = Static;

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(10, 0, 0);
	XMFLOAT3 ry(0, 10, 0);
	XMFLOAT3 rz(0, 0, 10);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(10, 10, 10);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.

	if(Master!=NULL)
		Master->isShieldOn = true;
}

void ShieldArmor::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	CreateCube(&Mesh, 20, 20, 20);

	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}


void ShieldArmor::Tick(const GameTimer & gt)
{
	/*
	LifeTime -= gt.DeltaTime();
	if (LifeTime <= 0)
	{
		Master->isShieldOn = false;
		DelObj = true;
	}
	*/

	Orient = QuaternionMultiply(Orient, QuaternionRotation(XMFLOAT3(0,1,0), MMPE_PI * gt.DeltaTime()));
	CenterPos = Master->CenterPos;
}

void ShieldArmor::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["ShieldTex"].get()->Resource.Get());

	UpdateConstBuffer(commandlist, false);

	Mesh.Render(commandlist);
}

ImpObject::ImpObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	//���ӿ�����Ʈ �����ڿ��� �⺻���ΰ��� ó�������Ƿ� ���⼭��
	//�޽��� �ؽ�ó ���� �ҷ������ �ִϸ��̼� �ε���� ó���ؾ��Ѵ�.

	//����Ʈ�� ����� �迭.
	jarr = new UploadBuffer<JointArr>(m_Device, 1, true);


	int num = 1;
	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "imp", L"textures/imp.dds", false, num, 0);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);

		CreateMesh = true;

	}
	TextureName = "imp";

	//���� ������Ʈ�� �߰��� ������ ���̵� ����

	isNPC = true;

	//���� NPC�� 65535�� ���� ������ �ȵ�
	this->myNPCID = ++g_npcID;
	this->m_npc_data.id = this->myNPCID;

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
	ObjData.Scale = 60;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.

	obs = Dynamic;
	//���� ������ (����)�� ��´�. ĳ���ʹ� �������� ���� �ʰ�, źȯ�� �������� ����������.
	gamedata.MAXHP = 5000;
	gamedata.HP = 5000;
	gamedata.Speed = 40;

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(10, 0, 0);
	XMFLOAT3 ry(0, 20, 0);
	XMFLOAT3 rz(0, 0, 10);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(10, 20, 10);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.25);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(500);

	//�Ӹ����� HP�� ���
	if (ParticleList != NULL)
	{
		Hpbar = new BarObject(m_Device, commandlist, ParticleList,NULL, this, 20, XMFLOAT4(CenterPos.x, 20, CenterPos.z, 0));
		HPFrame = new BarFrameObject(m_Device, commandlist, ParticleList,NULL, this, 20, XMFLOAT4(CenterPos.x, 20, CenterPos.z, 0));

		ParticleList->push_back(HPFrame);
		ParticleList->push_back(Hpbar);
	}
	if (Shadow != NULL)
	{
		s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(0, 0, 0), 2, 0, CenterPos);
		s->ObjData.Scale = 60.0f;
		Shadow->push_back(s);
	}
}

ImpObject::~ImpObject()
{
	if (Hpbar != NULL)
		Hpbar->DelObj = true;
	if (HPFrame != NULL)
		HPFrame->DelObj = true;
	if (s != NULL)
		s->DelObj = true;
}

void ImpObject::ToDead()
{
	SetAnimation(Ani_State::Dead);
}

void ImpObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\monster\\Idle.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

	//�ִϸ��̼� �ε�
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\monster\\Idle.MD5ANIM",    &Mesh, this, animations);//0
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\monster\\Run.MD5ANIM",     &Mesh, this, animations);//1
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\monster\\Attack1.MD5ANIM", &Mesh, this, animations);//2
	LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\monster\\Death.MD5ANIM",   &Mesh, this, animations);//2

}

void ImpObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{

	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void ImpObject::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.
	//pp->integrate(gt.DeltaTime());

	if (ObjData.isAnimation == true)
		UpdateMD5Model(commandlist, &Mesh, this, gt.DeltaTime()*60.0 / 24.0, n_Animation, animations, jarr);

	//if (fsm != NULL)
	//	fsm->Update(gt.DeltaTime());

}

void ImpObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), 0, TexOff);

	UpdateConstBuffer(commandlist, false);

	Mat.UpdateConstantBuffer(commandlist);

	//ƽ�Լ����� ������Ʈ�� �ִϸ��̼ǵ� ����Ʈ�� ������.

	commandlist->SetGraphicsRootConstantBufferView(0, jarr->Resource()->GetGPUVirtualAddress());
	//���� �׸���.

	Mesh.Render(commandlist);

}

void ImpObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	fsm->aidata.collisionmove = XMFLOAT3(0, 0, 0);
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
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				if (((*i)->obs == Obj_State::Static || (*i)->obs == Obj_State::Rigid) && fsm->aidata.curstateEnum == s_Trace)//�������°�, �浹����� ť����̸�
				{
					auto v = XMFloat4to3(Float4Add(fsm->aidata.LastPosition, CenterPos, false));
					v.y = 0;
					auto d = FloatLength(v);//�ӵ���ũ��
					v = Float3Normalize(v);//���� �ӵ� ����

					auto p = XMFloat4to3(Float4Add(CenterPos, (*i)->CenterPos, false));
					p.y = 0;
					p = Float3Normalize(p);

					if (Float3Cross(v, p).y > 0)//-80�������� p�� ȸ�����Ѿ���.
					{
						auto r = QuaternionRotation(XMFLOAT3(0, 1, 0), -MMPE_PI / 2.5);
						auto m = XMMatrixRotationQuaternion(XMLoadFloat4(&r));

						XMVECTOR pv = XMLoadFloat3(&p);
						pv = XMVector3Transform(pv, m);
						XMStoreFloat3(&fsm->aidata.collisionmove, pv);

					}
					else//80������ ȸ��
					{
						auto r = QuaternionRotation(XMFLOAT3(0, -1, 0), -MMPE_PI / 2.5);
						auto m = XMMatrixRotationQuaternion(XMLoadFloat4(&r));

						XMVECTOR pv = XMLoadFloat3(&p);
						pv = XMVector3Transform(pv, m);
						XMStoreFloat3(&fsm->aidata.collisionmove, pv);

					}


				}

				//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime);//����� ƨ���� �ʴ´�.

			}
		}
	}

}

void ImpObject::EndAnimation(int nAni)
{

	if (nAni == (int)Ani_State::Attack)//�����ϱ⿴����
	{
		SetAnimation((int)Ani_State::Idle);//�����·εд�.

		m_end_npc_attack = true;

	}

	if (nAni == Ani_State::Dead)//�״¸���̾�����
	{
		DelObj = true;

		m_end_npc_die = true;
	}
}

RingObject::RingObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4 cp):CGameObject(m_Device,commandlist,Plist,shadow,cp)
{
	if (CreateMesh)
		m_player_data.id = master->m_player_data.id;

	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "whiteTex", L"textures/object/white.dds", false, 1, 0);
		
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);

		CreateMesh = true;
	}

	TextureName = "whiteTex";

	
	TexOff = 0;

	Blending = true;
	ObjData.BlendValue = 0.45;

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.46f;//����ŧ���� �����ش�.
									  //ObjData.CustomData1.w = 1234;//CustomData1�� w�� 1234 �̸� ��ָ����� ���°�.

	obs = Static;
	DummyPos = XMFLOAT4(0, 100000, 0, 0);//�浹�˻縦 ���ϱ����ؼ� �Ϻη� �̻�����ġ�� pp�� �������� �д�.
	//���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(0, 0, 0);
	XMFLOAT3 ry(0, 0, 0);
	XMFLOAT3 rz(0, 0, 0);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&DummyPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(0, 0, 0);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.
}

void RingObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{

	LoadMD5Model(L".\\�÷��̾�޽���\\ring.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void RingObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void RingObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (times >= 0.125f)
	{
		DelObj = true;
	}
	else
	{
		times += gt.DeltaTime();
		ObjData.Scale += gt.DeltaTime() *1000;
	}

	//cout << "Times:" << times << "\n";

	if (Textures.size() > 0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), 0, TexOff);

	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}

ShadowObject::ShadowObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>*shadow,  CGameObject * master, XMFLOAT3 size, int kinds, float degree, XMFLOAT4 cp) :CGameObject(m_Device, commandlist, Plist,shadow, cp)
{
	//����Ʈ�� ����� �迭.
	jarr = new UploadBuffer<JointArr>(m_Device, 1, true);

	Master = master;
	Size = size;
	Kinds = kinds;
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	auto q = XMLoadFloat4(&Orient);//������ degree��ŭ ������ �Ѵ�.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, degree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();

	ObjData.BlendValue = 0.3f;


	if (Master != NULL)
	{
		if (Master->obs == Dynamic)
		{
			ObjData.isAnimation = true;
		}
	}

	if (CreateMesh == false)
	{
		cMesh.Index = NULL;
		cMesh.SubResource = NULL;
		oMesh.Index = NULL;
		oMesh.SubResource = NULL;
		iMesh.Index = NULL;
		iMesh.SubResource = NULL;
		smallwallMesh.Index = NULL;
		smallwallMesh.SubResource = NULL;
		towerMesh.Index = NULL;
		towerMesh.SubResource = NULL;
		columnMesh.Index = NULL;
		columnMesh.SubResource = NULL;
		elevatorMesh.Index = NULL;
		elevatorMesh.SubResource = NULL;


		SetMaterial(m_Device, commandlist);
		CreateMesh = true;
	}

	SetMesh(m_Device, commandlist);
	
}

void ShadowObject::SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	if (Kinds == Cubeman)
	{
		if (CreatecMesh == false)
		{
			LoadMD5Model(L".\\�÷��̾�޽���\\cIdle.MD5MESH", &cMesh, 0, 1);
			cMesh.SetNormal(true);
			cMesh.CreateVertexBuffer(m_Device, commandlist);
			cMesh.CreateIndexBuffer(m_Device, commandlist);

			LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cIdle.MD5ANIM", &cMesh, this, cAnimations);//0
			LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cRunning.MD5ANIM", &cMesh, this, cAnimations);//1
			LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cAttack.MD5ANIM", &cMesh, this, cAnimations);//2
			LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\cDeath.MD5ANIM", &cMesh, this, cAnimations);//3
			
			CreatecMesh = true;
		}
	}
	else if (Kinds == Cube)
	{
		if (CreateoMesh == false)
		{
			CreateCube(&oMesh, 1, 1, 1);
			oMesh.SetNormal(false);
			oMesh.CreateVertexBuffer(m_Device, commandlist);
			oMesh.CreateIndexBuffer(m_Device, commandlist);
			CreateoMesh = true;
		}
	}
	else if (Kinds == Boss)
	{
		if (CreateiMesh == false)
		{
			LoadMD5Model(L".\\�÷��̾�޽���\\monster\\Idle.MD5MESH", &iMesh, 0, 1);
			iMesh.SetNormal(false);
			iMesh.CreateVertexBuffer(m_Device, commandlist);
			iMesh.CreateIndexBuffer(m_Device, commandlist);

			LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\monster\\Idle.MD5ANIM", &iMesh, this, iAnimations);//0
			LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\monster\\Run.MD5ANIM", &iMesh, this, iAnimations);//1
			LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\monster\\Attack1.MD5ANIM", &iMesh, this, iAnimations);//2
			LoadMD5Anim(m_Device, L".\\�÷��̾�޽���\\monster\\Death.MD5ANIM", &iMesh, this, iAnimations);//3

			CreateiMesh = true;
		}
	}
	else if (Kinds == Meteor)
	{
		if (CreatemMesh == false)
		{
			LoadMD5Model(L".\\�÷��̾�޽���\\sphere.MD5MESH", &mMesh, 0, 1);
			mMesh.SetNormal(false);
			mMesh.CreateVertexBuffer(m_Device, commandlist);
			mMesh.CreateIndexBuffer(m_Device, commandlist);
			CreatemMesh = true;
		}
	}
	else if (Kinds == SmallWall)
	{
		if (CreateSmallwallMesh == false)
		{
			LoadMD5Model(L".\\�÷��̾�޽���\\fence.MD5MESH", &smallwallMesh, 0, 1);
			//
			smallwallMesh.SetNormal(false);
			smallwallMesh.CreateIndexBuffer(m_Device, commandlist);
			smallwallMesh.CreateVertexBuffer(m_Device, commandlist);
			CreateSmallwallMesh = true;		
		}


	}
	else if(Kinds == tower)
	{
		if (CreateTowerMesh == false)
		{
			//�� �ε�
			LoadMD5Model(L".\\�÷��̾�޽���\\fcastle.MD5MESH", &towerMesh, 0, 1);
			//
			towerMesh.SetNormal(false);
			towerMesh.CreateVertexBuffer(m_Device, commandlist);
			towerMesh.CreateIndexBuffer(m_Device, commandlist);

			CreateTowerMesh = true;
		}
	}
	else if(Kinds == column)
	{ 
		if (CreateColumnMesh == false)
		{
			LoadMD5Model(L".\\�÷��̾�޽���\\mCol.MD5MESH", &columnMesh, 0, 1);
			columnMesh.SetNormal(false);
			columnMesh.CreateVertexBuffer(m_Device, commandlist);
			columnMesh.CreateIndexBuffer(m_Device, commandlist);

			CreateColumnMesh = true;
		}
	}
	else if(Kinds == elevator)
	{
		if (CreateElevatorMesh == false)
		{
			CreateCube(&elevatorMesh, 2, 1, 2);

			elevatorMesh.SetNormal(false);
			elevatorMesh.CreateVertexBuffer(m_Device, commandlist);
			elevatorMesh.CreateIndexBuffer(m_Device, commandlist);

			CreateElevatorMesh = true;
		}
	}
}

void ShadowObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);
	
	Mat.MatData.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);

	Mat.MatData.Roughness = 0.1f;
}

void ShadowObject::Tick(const GameTimer & gt)
{
	if (Master->DelObj)
		DelObj = true;

	CenterPos = Master->CenterPos;
	

	if (ObjData.isAnimation == true)
	{
		if (CreatecMesh && Kinds == 0)
		{
			//�ִϸ��̼� ������Ʈ �ִϸ��̼��� 24���������� ������. ������ FPS�� 24�������� �ƴ�. �׺��� ū ������. ���� 24���������� �ش��������� ���� ����.
			if (Master->n_Animation != Attack)
			{
				UpdateMD5Model(commandlist, &cMesh, this, gt.DeltaTime()*60.0 / 24.0, Master->n_Animation, cAnimations, jarr);
			}
			else
			{
				UpdateMD5Model(commandlist, &cMesh, this, 2 * gt.DeltaTime()*60.0 / 24.0, Master->n_Animation, cAnimations, jarr);
			}
		}
		else if(CreateiMesh && Kinds == 2)
			UpdateMD5Model(commandlist, &iMesh, this, 0.8*gt.DeltaTime()*60.0 / 24.0, Master->n_Animation, iAnimations, jarr);

	}

	
}

void ShadowObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	UpdateConstBuffer(commandlist, true);

	Mat.UpdateConstantBuffer(commandlist);

	if(ObjData.isAnimation)
		commandlist->SetGraphicsRootConstantBufferView(0, jarr->Resource()->GetGPUVirtualAddress());

	if(CreatecMesh && Kinds == Cubeman && Master->DrawObj == true)
		cMesh.Render(commandlist);
	if (CreateoMesh && Kinds == Cube)
		oMesh.Render(commandlist);
	if (CreateiMesh && Kinds == Boss)
		iMesh.Render(commandlist);
	if (CreatemMesh && Kinds == Meteor)
		mMesh.Render(commandlist);
	if (CreateSmallwallMesh && Kinds == SmallWall)
		smallwallMesh.Render(commandlist);
	if (CreateTowerMesh && Kinds == tower)
		towerMesh.Render(commandlist);
	if (CreateColumnMesh && Kinds == column)
		columnMesh.Render(commandlist);
	if (CreateElevatorMesh && Kinds == elevator)
		elevatorMesh.Render(commandlist);
}

Floor2Object::Floor2Object(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, float size, XMFLOAT4 cp) :CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	if (CreateMesh == false)
	{
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "FloorTex", L"textures/object/tile1024.dds",false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;
	}
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	
	auto q = XMLoadFloat4(&Orient);//������ degree��ŭ ������ �Ѵ�.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, MMPE_PI*0.25);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = size;
	ObjData.SpecularParamater = 0.3f;//����ŧ���� �����ش�.
	ObjData.CustomData1.w = 1234;

	obs = Static;

	//���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;
	Blending = true;

	ObjData.BlendValue = 0.5f;

	XMFLOAT3 raxis{ 0,1,0 };
	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(90, 0, 0);


	XMFLOAT3 ry(0, 0.5f, 0);
	

	XMFLOAT3 rz(0, 0, 90);
	

	rco.SetPlane(rx, ry, rz);


	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(90, 0.5, 90);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.
}

void Floor2Object::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 180, 1, 180);

	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void Floor2Object::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void Floor2Object::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size() > 0)
	{
		SetTexture(commandlist, SrvDescriptorHeap, Textures["FloorTex"].get()->Resource.Get());
	}
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.
	Mesh.Render(commandlist);
}


BreakCartObject::BreakCartObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, float degree, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "CubeTex", L"textures/object/wood.dds", false);
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
	auto q2 = QuaternionRotation(axis, degree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 4.5f;
	ObjData.SpecularParamater = 0.0f;//����ŧ���� �����ش�.



									 //���Ӱ��� �����͵�
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;
	obs = Static;
	

	XMFLOAT3 raxis{ 0,1,0 };
	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(15, 0, 0);
	

	XMFLOAT3 ry(0, 8, 0);
	

	XMFLOAT3 rz(0, 0, 15);
	auto rqx = QuaternionRotation(raxis, degree);
	rx = Vector3Rotation(rx, rqx);


	auto rqy = QuaternionRotation(raxis, degree);
	ry = Vector3Rotation(ry, rqy);


	auto rqz = QuaternionRotation(raxis, degree);
	rz = Vector3Rotation(rz, rqz);

	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(15, 8, 15);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(0.5f);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetMass(INFINITY);//������ ��ü�� ���԰� �����̴�.
}

BreakCartObject::~BreakCartObject()
{
}

void BreakCartObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	//�� �ε�
	LoadMD5Model(L".\\�÷��̾�޽���\\cart.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void BreakCartObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.2f;
}

void BreakCartObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//���ӿ�����Ʈ�� �������� �����ϴ�. 
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist, false);

	//���� �׸���.

	Mesh.Render(commandlist);
}


StoneBullet::StoneBullet(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, CGameObject * master, XMFLOAT4 & ori, CGameObject * lockon, XMFLOAT4 cp, XMFLOAT4 opp) :CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "BulletTex", L"textures/effect/fire.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	if (g_npcBulletID >= 0xffff)
		g_npcBulletID = -1;

	this->myNPC_StoneBulletID = ++g_npcBulletID;
	m_bullet_data.alive = true;

	m_bullet_type = BULLET_TYPE::protocol_NpcStoneBullet;

	orgpluspos = opp;
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
	gamedata.Damage = 40;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	LifeTime = 10.0f;
	Master = master;
	LockOn = lockon;


	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(1, 1, 1);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(0, 0, 0);//Ʈ���������� �����ϰ���
	pp->SetMass(0.35f);

	if (ParticleList != NULL)
	{
		BulletParticles = new ParticleObject3(m_Device, commandlist, ParticleList, NULL, this, 0.2f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles);

	}

}

StoneBullet::~StoneBullet()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;

}

void StoneBullet::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 2, 2, 2);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void StoneBullet::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);

	Mat.MatData.Roughness = 0.3f;
	Mat.MatData.Emissive = XMFLOAT4{ 0.75f, 0.23f, 0.13f, 0.7f };

}

void StoneBullet::Tick(const GameTimer & gt)
{

	/*
	pp->integrate(gt.DeltaTime());

	Orient = QuaternionMultiply(Orient, QuaternionRotation(GetUpvector(), MMPE_PI * gt.DeltaTime()));

	CenterPos = orgpluspos;

	tempangle += MMPE_PI / 4 * gt.DeltaTime();
	XMFLOAT4 tempori = QuaternionRotation(XMFLOAT3(0, 1, 0), tempangle);

	XMVECTOR to = XMLoadFloat4(&tempori);
	XMVECTOR cp = XMLoadFloat4(&CenterPos);

	cp = XMVector4Transform(cp, XMMatrixRotationQuaternion(to));
	XMStoreFloat4(&CenterPos, cp);
	CenterPos = Float4Add(CenterPos, Master->CenterPos);
	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	LifeTime -= gt.DeltaTime();

	if (LifeTime <= 0)
		DelObj = true;
	*/
}

void StoneBullet::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["BulletTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}

void StoneBullet::Collision(list<CGameObject*>* collist, float DeltaTime)
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
				if ((*i)->isShieldOn) //��밡 �ǵ� on ���¸�
					gamedata.Damage = 100; //100 ������ 1���� �ؽ��� stride������ 100�̶�� �����ϰ� ToDamage���� 1�� ����



				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//1. ���� �������� �ش�.

					(*i)->ToDamage(gamedata.Damage);



					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}

					DelObj = true;

				}


			}
		}
	}

}

void StoneBullet::Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos)
{
	switch (coll_type)
	{
		case AFTER_COLLISION_EFFECT::EMPTY:
		{
			//�ҷ��� ��� �浹�� �ƹ��� �߰�����Ʈ�� �߻����� ����
			//1. ���� ���� -> ����

		}
		break;

		case AFTER_COLLISION_EFFECT::BOOM:
		{
			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			//1. �÷��̾� ���� -> ����
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//3. ���� ���� -> �÷��̾�
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//1. �÷��̾� ���� -> �÷��̾�
			//2. �÷��̾� ���� -> ����
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}

			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

	default:
		break;
	}

	DelObj = true;
}

MeteorObject::MeteorObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, list<CGameObject*>* shadow, CGameObject* master, XMFLOAT4& ori, XMFLOAT4 cp) :CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "MeteorTex", L"textures/object/Meteor.dds", false,2,0);
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "MeteorNTex", L"textures/object/MeteorN.dds", false,2,1);

		
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	UpdateLookVector();
	
	CenterPos.y += 1100.0f;
	
	obs = Bullet;
	
	ObjData.isAnimation = 0;
	ObjData.Scale = 30.0;
	ObjData.SpecularParamater = 0.1f;//����ŧ���� �����ش�.
	ObjData.CustomData1.w = 1234;
	staticobject = true;
	LifeTime = 20.0f;

	//���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 80;
	gamedata.GodMode = true;
	gamedata.Speed = 300;
	
	Master = master;

	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(30, 0, 0);
	XMFLOAT3 ry(0, 30, 0);
	XMFLOAT3 rz(0, 0, 30);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(30, 30, 30);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(0, -1 * gamedata.Speed, 0);
	pp->SetMass(1400);

	if (Shadow != NULL)
	{
		s = new ShadowObject(m_Device, commandlist, NULL, Shadow, this, XMFLOAT3(1, 1, 1), 3, 0, CenterPos);
		s->ObjData.Scale = 50.0f;
		Shadow->push_back(s);
	}

}

MeteorObject::~MeteorObject()
{
	if (s != NULL)
		s->DelObj = true;


}

void MeteorObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	//�� �ε�
	//CreateCube(&Mesh, 1, 1, 1);
	LoadMD5Model(L".\\�÷��̾�޽���\\sphere.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);
}

void MeteorObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);

	Mat.MatData.Roughness = 0.1f;
}

void MeteorObject::Tick(const GameTimer & gt)
{
	//���б�. ���б��? �� ƽ���� ��! �����ؼ� ��ȭ �Ǵ� ���ӵ�/�ӵ�/��ġ�� �����Ѵ�.
	//�̶� pp�� position�� CenterPos�� ��ġ���Ѿ��ϹǷ� CenterPos�� �����͸� ���ڷ� �Ѱܾ� �Ѵ�.
	pp->AddForce(0, -300, 0);
	pp->integrate(gt.DeltaTime());


	Orient = QuaternionMultiply(Orient, QuaternionRotation(XMFLOAT3(0,1,0), MMPE_PI * gt.DeltaTime()*2));


	LifeTime -= gt.DeltaTime();

	if (LifeTime <= 0)
		DelObj = true;
}

void MeteorObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//�ؽ�ó�� �����ϰ�, ��������� �����Ѵ�.

	if (Textures.size() > 0)
	{
		SetTexture(commandlist, SrvDescriptorHeap, Textures["MeteorTex"].get()->Resource.Get(),0,0);
		SetTexture(commandlist, SrvDescriptorHeap, Textures["MeteorNTex"].get()->Resource.Get(), 2, 1);
	}
	UpdateConstBuffer(commandlist, false);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);
}

void MeteorObject::Collision(list<CGameObject*>* collist, float DeltaTime)
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

				if ((*i)->isShieldOn) //��밡 �ǵ� on ���¸�
					gamedata.Damage = 10; //100 ������ 1���� �ؽ��� stride������ 100�̶�� �����ϰ� ToDamage���� 1�� ����

				//1. ���� �������� �ش�.
				(*i)->ToDamage(gamedata.Damage);

				bool isboss = false;

				//2. �������͸� �Ѿ˹����� ��������.
				if ((*i)->gamedata.MAXHP > 1000)//�������͸� �Ѿ� �������� �������ؾ���.
				{
					((ImpObject*)*i)->fsm->aidata.LastPosition = this->CenterPos;
					isboss = true;
				}

				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					(*i)->isHit1 = true;
					//���ӵ� ������ ���Ѵ�. A-B
					cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false));
					cn = Float3Normalize(cn);

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
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
				auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
				ParticleList->push_back(BulletParticles2);

				DelObj = true;


			}
		}
	}
}

HammerBullet::HammerBullet(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, list<CGameObject*>* bulletlist,int count, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon, XMFLOAT4 cp , XMFLOAT4 opp) :CGameObject(m_Device, commandlist, Plist, shadow, cp)
{
	bool firstBullet = true;
	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		BulletLists = bulletlist;
		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "BulletTex", L"textures/effect/fire.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;
		firstBullet = false;

	}

	if (firstBullet)
	{
		//���������ͷ� �ٲ��� �ʾƵ� �Ǵ� ��
		m_bullet_data.master_id = master->m_player_data.id;
		m_bullet_data.alive = true;

		//���� �����ͷ� �ٲ�� �Ǵ� ��
		if (m_hmBulletFirstCount)
		{
			m_hmBulletID = 0;
			m_hmBulletFirstCount = false;
		}
		else
		{ 
			++m_hmBulletID; 
		}

		m_bullet_data.my_id = m_hmBulletID;
	}

	m_bullet_type = BULLET_TYPE::protocol_HammerBullet;

	offsetPos = cp;
	orgpluspos = opp;
	//���ӿ�����Ʈ���� �躤�Ϳ� ����Ʈ���Ͱ� �ٸ��Ƿ� �ʱ� ������ ������ ���ش�.
	//���� �躤�� ���� ��� UpdateLookVector���� ó���ȴ�(����Ʈ���͵�) ���� Tick�Լ����� �ݵ�� ȣ���ؾ��Ѵ�.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	//���ڷ� �߻�������� �躤�Ͱ� �������� ȸ���� ori���� �ް�, ���� ����(������ 0,0,0,1)�� �����ش�.
	Orient = QuaternionMultiply(Orient, ori);
	
	UpdateLookVector();

	obs = Bullet;
	Count = count;//�п� ���� Ƚ��
	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.2f;//����ŧ���� �����ش�.

									 //���Ӱ��� �����͵�
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 50;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	LifeTime = 12.0f;
	Master = master;
	LockOn = lockon;


	//�����浹 �˻�� ����ü
	XMFLOAT3 rx(2, 0, 0);
	XMFLOAT3 ry(0, 2, 0);
	XMFLOAT3 rz(0, 0, 2);
	rco.SetPlane(rx, ry, rz);

	//����������Ʈ ���� �ʿ��� �����͵� ����
	pp = new PhysicsPoint();
	pp->SetPosition(&CenterPos);//�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(2, 2, 2);//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);//ƨ���� �ʴ´�.
	pp->SetVelocity(0, 0, 0);//Ʈ���������� �����ϰ���
	pp->SetMass(5.35f);

	if (ParticleList != NULL)
	{
		BulletParticles = new ParticleObject3(m_Device, commandlist, ParticleList, NULL, this, 0.2f, XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, 0));
		ParticleList->push_back(BulletParticles);

	}
}

HammerBullet::~HammerBullet()
{
	if (BulletParticles != NULL)
		BulletParticles->DelObj = true;

}

void HammerBullet::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	LoadMD5Model(L".\\�÷��̾�޽���\\hammer.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}

void HammerBullet::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);

	Mat.MatData.Roughness = 0.3f;
	Mat.MatData.Emissive = XMFLOAT4{ 0.75f, 0.23f, 0.13f, 0.7f };

}

void HammerBullet::Tick(const GameTimer & gt)
{
	/*Orient = QuaternionMultiply(Orient, QuaternionRotation(GetUpvector(), MMPE_PI * gt.DeltaTime()));

	CenterPos = orgpluspos;

	tempangle += MMPE_PI / 3 * gt.DeltaTime();
	XMFLOAT4 tempori = QuaternionRotation(XMFLOAT3(0, 1, 0), tempangle);

	XMVECTOR to = XMLoadFloat4(&tempori);
	XMVECTOR cp = XMLoadFloat4(&CenterPos);

	cp = XMVector4Transform(cp, XMMatrixRotationQuaternion(to));
	XMStoreFloat4(&CenterPos, cp);
	CenterPos = Float4Add(CenterPos, offsetPos);
	//����ü�� ���� �ֱⰡ �־�� �Ѵ�.
	LifeTime -= gt.DeltaTime();
	orgpluspos.z += gt.DeltaTime() * 7;

	if (LifeTime <= 0)
		DelObj = true;
	*/
}

void HammerBullet::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["BulletTex"].get()->Resource.Get());
	UpdateConstBuffer(commandlist, false);

	Mat.UpdateConstantBuffer(commandlist);

	//���� �׸���.

	Mesh.Render(commandlist);

}

void HammerBullet::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	/*
	CollisionList = collist;
	//�浹����Ʈ�� ��� ��ҿ� �浹�˻縦 �ǽ��Ѵ�.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp�� NULL�� �ƴϸ� ���� ������Ʈ�̴�.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//�浹������ pp�� ���� �׳� �������� �ְ� �ڽ��� ���ָ� ��. 
			{
				if ((*i)->isShieldOn) //��밡 �ǵ� on ���¸�
					gamedata.Damage = 100; //100 ������ 1���� �ؽ��� stride������ 100�̶�� �����ϰ� ToDamage���� 1�� ����



				XMFLOAT3 cn;
				//������ ��ü�� �ƴϸ�
				if ((*i)->staticobject == false)
				{
					//1. ���� �������� �ش�.

					(*i)->ToDamage(gamedata.Damage);
		
					bool isboss = false;

					//2. �������͸� �Ѿ˹����� ��������.
					if ((*i)->gamedata.MAXHP > 1000)//�������͸� �Ѿ� �������� �������ؾ���.
					{
						((ImpObject*)*i)->fsm->aidata.LastPosition = this->CenterPos;
						isboss = true;
					}

					//���� ��밡 �׾��ٸ� ������ �߰��Ѵ�.
					if ((*i)->gamedata.HP <= 0)
					{
						if (((CCubeManObject*)Master)->player != NULL)
						{
							if (isboss)
								((CCubeManObject*)Master)->player->pointrank.Point += 100;
							else
								((CCubeManObject*)Master)->player->pointrank.Point += 10;
						}
					}

					
					//�ظӴ� ���̳��Ϳ�����Ʈ�� �浹�� ������ȴ�.
					if (Count > 0)
					{
						HammerBullet* nh = new HammerBullet(Master->device, Master->commandlist, Master->ParticleList, NULL, NULL,Count-1, Master, XMFLOAT4(0, 0, 0, 1), NULL, Float4Add(CenterPos,XMFLOAT4(15,0,15,0)), XMFLOAT4(0, 0, 35, 0));
						BulletLists->push_back(((CGameObject*)nh));
						Count = 0;
					}

					// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}

					DelObj = true;

				}


			}
		}
	}
	*/
}

void HammerBullet::Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos)
{
	switch (coll_type)
	{
		case AFTER_COLLISION_EFFECT::EMPTY:
		{
			//�ҷ��� ��� �浹�� �ƹ��� �߰�����Ʈ�� �߻����� ����
			//1. ���� ���� -> ����

		}
		break;

		case AFTER_COLLISION_EFFECT::BOOM:
		{
			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			//1. �÷��̾� ���� -> ����
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//3. ���� ���� -> �÷��̾�
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}
		}
		break;

		case AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM:
		{
			// ��ƼŬ����Ʈ�� ������ ������Ʈ�� �����ؼ� ����. ��ƼŬ�� ����.
			//1. �÷��̾� ���� -> �÷��̾�
			//2. �÷��̾� ���� -> ����
			if (ParticleList != NULL)
			{
				ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, NULL, damage, XMFLOAT4(target_pos.x, target_pos.y + 11, target_pos.z, 0)));
			}

			//��ġ�� �κ��� �������ʿ䰡 ���°� ����ü�� ��ó�� ������ϱ�.
			auto BulletParticles2 = new ParticleObject2(device, commandlist, ParticleList, NULL, this, 0.7f, 100, XMFLOAT4(bullet_pos.x, bullet_pos.y, bullet_pos.z, 0));
			ParticleList->push_back(BulletParticles2);
		}
		break;

	default:
		break;
	}

	DelObj = true;
	//cout << "\n";
	//cout << "In Collision Bullet ID: " << static_cast<int>(m_bullet_data.my_id) << "Is Alive: " << static_cast<int>(m_bullet_data.alive) << "\n";
}

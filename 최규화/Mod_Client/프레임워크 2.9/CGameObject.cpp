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

	//여기서는 기본적인것들만 처리한다. 위치나 회전각 등 초기화
	this->device = m_Device;
	this->commandlist = commandlist;
	this->CenterPos = cp;//중점위치
	this->ParticleList = Plist;

	XMStoreFloat4(&Orient, XMQuaternionIdentity());//방향을 초기화 한다.
	SetWorldMatrix();	//월드변환생성
	CreateConstBuffer(m_Device);//상수버퍼생성
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
	//상수버퍼를 루트파라미터로 Descriptor를 쓸경우 상수버퍼 뷰를 만들때 힙을 안만들어도 된다.
	//상수버퍼를 만드는 방법
	//UploadBuffer<넣을데이터>* buffer=new UploadBuffer<넣을데이터>(Device,1,true);//마지막 true는 만약 정점일경우 false로
	//루트시그니처 생성항목으로 가서(씬클래스에 있다.)

	//CD3DX12_ROOT_PARAMETER RootParameter[1];
	//RootParameter[0].InitAsConstantBufferView(0);
	//이렇게 파라미터를 설정해서 만든다. 그후
	//UpdateConstBuffer에서 CopyData를 해준다음
	//commandlist->SetGraphicsRootConstantBufferView(상수버퍼인덱스,GetGPUVIRTUALADDRESS()); 이런거 해주면끝


	ConstBuffer = new UploadBuffer<ObjectData>(m_Device, 1, true);

}

void CGameObject::UpdateConstBuffer(ID3D12GraphicsCommandList * commandlist)
{
	//항상 호출할것. 룩벡터와 라이트벡터를 업데이트해준다.
	UpdateLookVector();
	if (pp != NULL && rb == NULL)
		UpdatePPosCenterPos();//질점의 중점으로 센터포즈를 변경함. 반드시 충돌하거나 하면 처리해야함
	SetWorldMatrix();//현재 포지션과 로테이션정보로 월드행렬을 만든다.

	XMMATRIX world = XMLoadFloat4x4(&ObjData.WorldMatrix);

	XMStoreFloat4x4(&ObjData.WorldMatrix, XMMatrixTranspose(world));

	//상수버퍼 업데이트
	ConstBuffer->CopyData(0, ObjData);

	commandlist->SetGraphicsRootConstantBufferView(2, ConstBuffer->Resource()->GetGPUVirtualAddress());//월드행렬연결
}


//========================================= 텍스쳐 세팅 =========================================================================

void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, ID3D12Resource* texture, bool isCubeMap, int Offset)
{

	commandlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));


	//텍스처는 테이블을 쓸것이므로 힙과 테이블 두개를 연결해야함.
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
		//디스크립터 힙 생성
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


//=================================================== 오브젝트 생성 ===============================================================

CCubeManObject::CCubeManObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	//게임오브젝트 생성자에서 기본적인것을 처리했으므로 여기서는
	//메쉬와 텍스처 사용시 불러오기와 애니메이션 로드등을 처리해야한다.

	//조인트가 저장될 배열.
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


	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, -1);
	OffRightvector = XMFLOAT3(-1, 0, 0);
	auto q = XMLoadFloat4(&Orient);//방향을 180도 돌리려 한다.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, MMPE_PI);
	Orient = QuaternionMultiply(Orient, q2);
	UpdateLookVector();
	ObjData.isAnimation = true;
	ObjData.Scale = 3;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.

	obs = Dynamic;
	//게임 데이터 (스텟)을 찍는다. 캐릭터는 데미지를 갖지 않고, 탄환이 데미지를 갖도록하자.
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Speed = 50;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(3, 0, 0);
	XMFLOAT3 ry(0, 10, 0);
	XMFLOAT3 rz(0, 0, 3);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(3, 10, 3);//충돌 박스의 x,y,z 크기
	pp->SetDamping(0.45);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.


						 //머리위에 HP바 띄움
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

	//모델 로드
	LoadMD5Model(L".\\플레이어메쉬들\\cIdle.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(true);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

	//애니메이션 로드
	LoadMD5Anim(m_Device, L".\\플레이어메쉬들\\cIdle.MD5ANIM", &Mesh, this, animations);//0
	LoadMD5Anim(m_Device, L".\\플레이어메쉬들\\cRunning.MD5ANIM", &Mesh, this, animations);//1
	LoadMD5Anim(m_Device, L".\\플레이어메쉬들\\cAttack.MD5ANIM", &Mesh, this, animations);//2
	LoadMD5Anim(m_Device, L".\\플레이어메쉬들\\cDeath.MD5ANIM", &Mesh, this, animations);//2


}

void CCubeManObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void CCubeManObject::Tick(const GameTimer & gt)
{
	//적분기. 적분기란? 매 틱마다 힘! 에의해서 변화 되는 가속도/속도/위치를 갱신한다.
	//이때 pp의 position과 CenterPos를 일치시켜야하므로 CenterPos의 포인터를 인자로 넘겨야 한다.
	pp->integrate(gt.DeltaTime(), &CenterPos);

	if (ObjData.isAnimation == true)
	{

		//애니메이션 업데이트 애니메이션은 24프레임으로 구성됨. 문제는 FPS가 24프레임이 아님. 그보다 큰 프레임. 따라서 24프레임으로 해당프레임을 나눠 보정.
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
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.


	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), false, TexOff);



	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//틱함수에서 업데이트한 애니메이션된 조인트를 연결함.

	commandlist->SetGraphicsRootConstantBufferView(3, jarr->Resource()->GetGPUVirtualAddress());
	//이후 그린다.

	Mesh.Render(commandlist);

}

//충돌기. 충돌검출과 충돌해소를 맡는다.
void CCubeManObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//충돌리스트의 모든 요소와 충돌검사를 실시한다.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this)
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//충돌했으면 충돌해소를 해야한다.
			{

				//충돌 했을때 축이 (0,1,0) 이면 Airbone을 false로 둔다. 이는 내가 위에있음을 나타낸다.
				if (pp->pAxis.y > 0)
				{
					pp->SetVelocity(pp->GetVelocity().x, 0, pp->GetVelocity().z);
					AirBone = false;


				}
				//충돌했을때  축이 (0,-1,0)이면 상대방 Airbone을 false로 둔다.  이는 상대가 내 위에있음을 나타낸다.
				//설사 상대 위에 다른 상대가 있어도 걱정말자. 자연스러운것임.
				if (pp->pAxis.y < 0)
				{
					(*i)->pp->SetVelocity((*i)->pp->GetVelocity().x, 0, (*i)->pp->GetVelocity().z);
					(*i)->AirBone = false;
				}

				XMFLOAT3 cn;
				//고정된 물체가 아니면
				if ((*i)->staticobject == false)
				{
					//상대속도 방향을 구한다. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

				}
				else//고정된 물체면 충돌한 평면의 노멀방향으로 cn을 설정할것.
				{
					cn = pp->pAxis;
				}

				//충돌해소 호출. 충돌해소 이후에 반드시 변경된 질점의 위치로 오브젝트위치를 일치시켜야한다.
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime);//좀비는 튕기지 않는다.
				UpdatePPosCenterPos();
				(*i)->UpdatePPosCenterPos();
			}
		}
	}
}



void CCubeManObject::EndAnimation(int nAni)
{

	if (nAni == (int)Ani_State::Attack)//공격하기였으면
	{
		SetAnimation((int)Ani_State::Idle);//대기상태로둔다.

	}

	if (nAni == Ani_State::Dead)//죽는모션이었으면
	{
		DelObj = true;
	}
}

//------------------- 투 사 체 -----------------------//

BulletCube::BulletCube(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon, XMFLOAT4 cp, bool IsMine) : CGameObject(m_Device, commandlist, Plist, cp)
{
	//Bullet Resoucre의 경우 Bullet초기화를 한번 무조건 처음에 실행시키는데 이 때의 정보는 필요없음 - firstBullet 플래그를 이용해서 삭제유무결정
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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	//인자로 발사방향으로 룩벡터가 될정도로 회전한 ori값을 받고, 현재 방향(아직은 0,0,0,1)과 곱해준다.
	Orient = QuaternionMultiply(Orient, ori);

	UpdateLookVector();

	obs = Bullet;

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.2f;//스페큘러를 낮게준다.

									 //게임관련 데이터들
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 150;

	if (firstBullet)
	{
		//CreateBullet 함수 호출 - 내 클라이언트에서 키를 눌러서 불렛을 생성했음 
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

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(1, 1, 1);//충돌 박스의 x,y,z 크기
	pp->SetDamping(1);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//룩벡터로 날아감
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
	//적분기. 적분기란? 매 틱마다 힘! 에의해서 변화 되는 가속도/속도/위치를 갱신한다.
	//이때 pp의 position과 CenterPos를 일치시켜야하므로 CenterPos의 포인터를 인자로 넘겨야 한다.


	//수정 및 삭제
	//pp->integrate(gt.DeltaTime(), &CenterPos);

	//Orient = QuaternionMultiply(Orient, QuaternionRotation(Lookvector, MMPE_PI * gt.DeltaTime()));

	//투사체는 생명 주기가 있어야 한다.
	//LifeTime -= gt.DeltaTime();

	//if (LifeTime <= 0)
	//	DelObj = true;

}

void BulletCube::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["BulletTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);

}

//충돌기. 충돌검출과 충돌해소를 맡는다.
void BulletCube::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//충돌리스트의 모든 요소와 충돌검사를 실시한다.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp가 NULL이 아니면 질점 오브젝트이다.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//충돌했으면 pp의 경우는 그냥 데미지를 주고 자신을 없애면 됨. 
			{
				//1. 먼저 데미지를 준다.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//고정된 물체가 아니면
				if ((*i)->staticobject == false)
				{
					//상대속도 방향을 구한다. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// 파티클리스트에 데미지 오브젝트를 생성해서 넣음. 파티클을 띄운다.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}
					//고정된 물체가아니면 잠깐 바운스를 풀어둔다. 그래야 튕기니까.
					(*i)->pp->SetBounce(true);

				}
				else//고정된 물체면 충돌한 평면의 노멀방향으로 cn을 설정할것.
				{
					cn = pp->pAxis;
				}

				//충돌후 속도를 계산함.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);
				(*i)->pp->SetBounce(false);
				//겹치는 부분을 제거할필요가 없는게 투사체는 어처피 사라지니까.
				DelObj = true;


			}
		}
	}
}



HeavyBulletCube::HeavyBulletCube(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon, XMFLOAT4 cp, bool IsMine) : CGameObject(m_Device, commandlist, Plist, cp)
{
	//Bullet Resoucre의 경우 Bullet초기화를 한번 무조건 처음에 실행시키는데 이 때의 정보는 필요없음 - firstBullet 플래그를 이용해서 삭제유무결정
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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	//인자로 발사방향으로 룩벡터가 될정도로 회전한 ori값을 받고, 현재 방향(아직은 0,0,0,1)과 곱해준다.
	Orient = QuaternionMultiply(Orient, ori);

	UpdateLookVector();

	obs = Bullet;

	ObjData.isAnimation = 0;
	ObjData.Scale = 2.0;
	ObjData.SpecularParamater = 0.8f;//스페큘러를 낮게준다.

									 //게임관련 데이터들
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 30;
	gamedata.GodMode = true;
	gamedata.Speed = 180;

	if (firstBullet)
	{
		//CreateBullet 함수 호출 - 내 클라이언트에서 키를 눌러서 불렛을 생성했음 
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

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(2, 2, 2);//충돌 박스의 x,y,z 크기
	pp->SetDamping(1);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//룩벡터로 날아감
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
	//적분기. 적분기란? 매 틱마다 힘! 에의해서 변화 되는 가속도/속도/위치를 갱신한다.
	//이때 pp의 position과 CenterPos를 일치시켜야하므로 CenterPos의 포인터를 인자로 넘겨야 한다.

	//pp->integrate(gt.DeltaTime(), &CenterPos);
	//
	////No애니메이션!
	//
	////투사체는 생명 주기가 있어야 한다.
	//LifeTime -= gt.DeltaTime();
	//
	//Orient = QuaternionMultiply(Orient, QuaternionRotation(Lookvector, MMPE_PI / 6 * gt.DeltaTime()));
	//
	//if (LifeTime <= 0)
	//	DelObj = true;

}

void HeavyBulletCube::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["BulletTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);

}

//충돌기. 충돌검출과 충돌해소를 맡는다.
void HeavyBulletCube::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//충돌리스트의 모든 요소와 충돌검사를 실시한다.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp가 NULL이 아니면 질점 오브젝트이다.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//충돌했으면 pp의 경우는 그냥 데미지를 주고 자신을 없애면 됨. 
			{
				//1. 먼저 데미지를 준다.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//고정된 물체가 아니면
				if ((*i)->staticobject == false)
				{
					//상대속도 방향을 구한다. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// 파티클리스트에 데미지 오브젝트를 생성해서 넣음. 파티클을 띄운다.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}
					//고정된 물체가아니면 잠깐 바운스를 풀어둔다. 그래야 튕기니까.
					(*i)->pp->SetBounce(true);

				}
				else//고정된 물체면 충돌한 평면의 노멀방향으로 cn을 설정할것.
				{
					cn = pp->pAxis;
				}

				//충돌후 속도를 계산함.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);
				(*i)->pp->SetBounce(false);
				//겹치는 부분을 제거할필요가 없는게 투사체는 어처피 사라지니까.
				DelObj = true;


			}
		}
	}
}

//-------------------- 테트라이크 ---------------------------------//

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);


	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.

	obs = Bullet;
	//게임관련 데이터들
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 3;
	LifeTime = 10;
	Master = master;
	LockOn = lockon;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(4, 4, 4);//충돌 박스의 x,y,z 크기
	pp->SetDamping(1);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
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
	//적분기. 적분기란? 매 틱마다 힘! 에의해서 변화 되는 가속도/속도/위치를 갱신한다.
	//이때 pp의 position과 CenterPos를 일치시켜야하므로 CenterPos의 포인터를 인자로 넘겨야 한다.
	pp->AddForce(0, -300, 0);
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No애니메이션!

	//투사체는 생명 주기가 있어야 한다.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetris1::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris1Tex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);

}

//충돌기. 충돌검출과 충돌해소를 맡는다.
void Tetris1::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//충돌리스트의 모든 요소와 충돌검사를 실시한다.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp가 NULL이 아니면 질점 오브젝트이다.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//충돌했으면 pp의 경우는 그냥 데미지를 주고 자신을 없애면 됨. 
			{
				//1. 먼저 데미지를 준다.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//고정된 물체가 아니면
				if ((*i)->staticobject == false)
				{
					//상대속도 방향을 구한다. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// 파티클리스트에 데미지 오브젝트를 생성해서 넣음. 파티클을 띄운다.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}

				}
				else//고정된 물체면 충돌한 평면의 노멀방향으로 cn을 설정할것.
				{
					cn = pp->pAxis;
				}

				//충돌후 속도를 계산함.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);

				//겹치는 부분을 제거할필요가 없는게 투사체는 어처피 사라지니까.
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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);


	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.

	obs = Bullet;
	//게임관련 데이터들
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 15;
	LifeTime = 10;
	Master = master;
	LockOn = lockon;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(3, 9, 3);//충돌 박스의 x,y,z 크기
	pp->SetDamping(1);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
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
	//적분기. 적분기란? 매 틱마다 힘! 에의해서 변화 되는 가속도/속도/위치를 갱신한다.
	//이때 pp의 position과 CenterPos를 일치시켜야하므로 CenterPos의 포인터를 인자로 넘겨야 한다.
	pp->AddForce(0, -100, 0);
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No애니메이션!

	//투사체는 생명 주기가 있어야 한다.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetris2::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris2Tex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);

}

//충돌기. 충돌검출과 충돌해소를 맡는다.
void Tetris2::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//충돌리스트의 모든 요소와 충돌검사를 실시한다.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp가 NULL이 아니면 질점 오브젝트이다.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//충돌했으면 pp의 경우는 그냥 데미지를 주고 자신을 없애면 됨. 
			{
				//1. 먼저 데미지를 준다.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//고정된 물체가 아니면
				if ((*i)->staticobject == false)
				{
					//상대속도 방향을 구한다. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// 파티클리스트에 데미지 오브젝트를 생성해서 넣음. 파티클을 띄운다.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}


				}
				else//고정된 물체면 충돌한 평면의 노멀방향으로 cn을 설정할것.
				{
					cn = pp->pAxis;
				}

				//충돌후 속도를 계산함.

				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);

				//겹치는 부분을 제거할필요가 없는게 투사체는 어처피 사라지니까.
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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	obs = Bullet;
	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.

									 //게임관련 데이터들
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 5;
	LifeTime = 10;
	Master = master;
	LockOn = lockon;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(4, 8, 2);//충돌 박스의 x,y,z 크기
	pp->SetDamping(1);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
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
	//적분기. 적분기란? 매 틱마다 힘! 에의해서 변화 되는 가속도/속도/위치를 갱신한다.
	//이때 pp의 position과 CenterPos를 일치시켜야하므로 CenterPos의 포인터를 인자로 넘겨야 한다.
	pp->AddForce(0, -180, 0);
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No애니메이션!

	//투사체는 생명 주기가 있어야 한다.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetris3::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris3Tex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);

}

//충돌기. 충돌검출과 충돌해소를 맡는다.
void Tetris3::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//충돌리스트의 모든 요소와 충돌검사를 실시한다.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp가 NULL이 아니면 질점 오브젝트이다.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//충돌했으면 pp의 경우는 그냥 데미지를 주고 자신을 없애면 됨. 
			{
				//1. 먼저 데미지를 준다.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//고정된 물체가 아니면
				if ((*i)->staticobject == false)
				{
					//상대속도 방향을 구한다. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// 파티클리스트에 데미지 오브젝트를 생성해서 넣음. 파티클을 띄운다.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}


				}
				else//고정된 물체면 충돌한 평면의 노멀방향으로 cn을 설정할것.
				{
					cn = pp->pAxis;
				}

				//충돌후 속도를 계산함.

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);


	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.
	obs = Bullet;
	//게임관련 데이터들
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 10;
	gamedata.GodMode = true;
	gamedata.Speed = 10;
	LifeTime = 10;
	Master = master;
	LockOn = lockon;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(4, 8, 2);//충돌 박스의 x,y,z 크기
	pp->SetDamping(1);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
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


	CreateTetrisㅗ(&Mesh, 4, 16, 4);
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
	//적분기. 적분기란? 매 틱마다 힘! 에의해서 변화 되는 가속도/속도/위치를 갱신한다.
	//이때 pp의 position과 CenterPos를 일치시켜야하므로 CenterPos의 포인터를 인자로 넘겨야 한다.
	pp->AddForce(0, -150, 0);
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No애니메이션!

	//투사체는 생명 주기가 있어야 한다.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetris4::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetris4Tex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);

}

//충돌기. 충돌검출과 충돌해소를 맡는다.
void Tetris4::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//충돌리스트의 모든 요소와 충돌검사를 실시한다.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this && *i != Master && (*i)->pp != NULL) // pp가 NULL이 아니면 질점 오브젝트이다.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//충돌했으면 pp의 경우는 그냥 데미지를 주고 자신을 없애면 됨. 
			{
				//1. 먼저 데미지를 준다.
				(*i)->ToDamage(gamedata.Damage);


				XMFLOAT3 cn;
				//고정된 물체가 아니면
				if ((*i)->staticobject == false)
				{
					//상대속도 방향을 구한다. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

					// 파티클리스트에 데미지 오브젝트를 생성해서 넣음. 파티클을 띄운다.
					if (ParticleList != NULL)
					{
						ParticleList->push_back(new DamageObject(device, commandlist, ParticleList, gamedata.Damage, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y + 11, (*i)->CenterPos.z, 0)));
					}


				}
				else//고정된 물체면 충돌한 평면의 노멀방향으로 cn을 설정할것.
				{
					cn = pp->pAxis;
				}

				//충돌후 속도를 계산함.

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	CenterPos.y += 150;

	Blist = Bulletlist;
	ParticleList = Plist;

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.
	obs = Bullet;
	//게임관련 데이터들
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
	//No애니메이션!

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




	//투사체는 생명 주기가 있어야 한다.
	LifeTime -= gt.DeltaTime();


	if (LifeTime <= 0)
		DelObj = true;

}

void Tetrike::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Tetrike"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);

}


//---------------------- 스태틱 오브젝트 -----------------------------//

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1024.0f;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.

	obs = Static;
	//게임관련 데이터들
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;


}


void SphereObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	//모델 로드
	LoadMD5Model(L".\\플레이어메쉬들\\sphere.MD5MESH", &Mesh, 0, 1);
	//
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

}


void SphereObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer & gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["MapTex"].get()->Resource.Get(), true);
	UpdateConstBuffer(commandlist);

	//이후 그린다.

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.46f;//스페큘러를 낮게준다.

	obs = Static;

	//게임관련 데이터들
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(5, 0, 0);
	XMFLOAT3 ry(0, 5, 0);
	XMFLOAT3 rz(0, 0, 5);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(5, 5, 5);//충돌 박스의 x,y,z 크기
	pp->SetDamping(0.5f);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
	pp->SetMass(INFINITY);//고정된 물체는 무게가 무한이다.


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
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size() > 0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures[TextureName].get()->Resource.Get(), false, TexOff);

	UpdateConstBuffer(commandlist);

	//이후 그린다.

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.


									 //게임관련 데이터들
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(10, 0, 0);
	XMFLOAT3 ry(0, 5, 0);
	XMFLOAT3 rz(0, 0, 10);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(10, 5, 10);//충돌 박스의 x,y,z 크기
	pp->SetDamping(0.5f);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
	pp->SetMass(INFINITY);//고정된 물체는 무게가 무한이다.

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

	//이후 그린다.
	Mesh.Render(commandlist);
}

void MoveCubeObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	CollisionList = collist;
	//충돌리스트의 모든 요소와 충돌검사를 실시한다.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this)
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//충돌했으면 충돌해소를 해야한다.
			{

				//충돌 했을때 축이 (0,1,0) 이면 Airbone을 false로 둔다. 이는 내가 위에있음을 나타낸다.
				if (pp->pAxis.y > 0)
				{
					pp->SetVelocity(pp->GetVelocity().x, 0, pp->GetVelocity().z);
					AirBone = false;

				}
				//충돌했을때  축이 (0,-1,0)이면 상대방 Airbone을 false로 둔다.  이는 상대가 내 위에있음을 나타낸다.
				//설사 상대 위에 다른 상대가 있어도 걱정말자. 자연스러운것임.
				if (pp->pAxis.y < 0)
				{
					(*i)->pp->SetVelocity((*i)->pp->GetVelocity().x, 0, (*i)->pp->GetVelocity().z);
					(*i)->AirBone = false;
				}

				XMFLOAT3 cn;
				//고정된 물체가 아니면
				if ((*i)->staticobject == false)
				{
					//상대속도 방향을 구한다. A-B
					cn = Float3Add(pp->GetPosition(), (*(*i)->pp).GetPosition(), false);
					cn = Float3Normalize(cn);

				}
				else//고정된 물체면 충돌한 평면의 노멀방향으로 cn을 설정할것.
				{
					cn = pp->pAxis;
				}

				//충돌해소 호출. 충돌해소 이후에 반드시 변경된 질점의 위치로 오브젝트위치를 일치시켜야한다.
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime);//좀비는 튕기지 않는다.
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
	ObjData.SpecularParamater = 0.3f;//스페큘러를 낮게준다.

	obs = Static;
	//게임관련 데이터들
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
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["GridTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);
}

///////////////////////////////////

//HPBar오브젝트
BarObject::BarObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, CGameObject* master, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.
	ObjData.CustomData1.x = 1;

	Master = master;
	ObjData.CustomData1.y = Master->gamedata.HP;

	obs = UI;
	//게임관련 데이터들
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


	//여기서 좌표를 일괄적으로 설정 할 수 있다
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
	//hp를 넣어주어서 shader에서 스케일크기만큼 바를 만든다. 	
	//HP바는 커스텀데이터 y를 사용하며 여기에 마스터 HP 비율을 넣음.
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


//HPbar 틀
BarFrameObject::BarFrameObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>* Plist, CGameObject * master, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{
	ObjData.isAnimation = 0;
	ObjData.Scale = 10.0f;
	ObjData.SpecularParamater = 0.5f;//스페큘러를 낮게준다.
	ObjData.CustomData1.x = 4;

	Master = master;

	//게임관련 데이터들
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


	//여기서 좌표를 일괄적으로 설정 할 수 있다
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
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.

	damaged = Damaged;
	//게임관련 데이터들
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


	//여기서 좌표를 일괄적으로 설정 할 수 있다
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
	//메쉬와 텍스처 

	if (CreateMesh == false)
	{

		Mesh.Index = NULL;
		Mesh.SubResource = NULL;

		LoadTexture(m_Device, commandlist, this, Textures, SrvDescriptorHeap, "CubeTex", L"textures/object/DeepIce.dds", false);
		SetMesh(m_Device, commandlist);
		SetMaterial(m_Device, commandlist);
		CreateMesh = true;

	}

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 20.0f;
	ObjData.SpecularParamater =-1.f;//스페큘러를 낮게준다.

	obs = Rigid;


	//게임관련 데이터들
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = false;


	//광선충돌 검사용 육면체
	XMFLOAT3 rx(12, 0, 0);
	XMFLOAT3 ry(0, 10, 0);
	XMFLOAT3 rz(0, 0, 12);
	rco.SetPlane(rx, ry, rz);

	//리지드 바디

	rb = new RigidBody();
	rb->SetPosition(&CenterPos);//이 값은 항상 갱신되야한다.
	rb->SetHalfBox(10, 10, 10);//충돌 박스의 x,y,z 크기
	rb->SetDamping(0.5f, 0.38f);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	rb->SetBounce(false);//튕기지 않는다.
	rb->SetMass(1.5);//고정된 물체는 무게가 무한이다.
	rb->SetIMoment(10, 10, 10);

	rb->SetOrient(&Orient);

	//테스트 코드
	XMFLOAT3 testForce{ -5,-3,2 };
	XMFLOAT3 testPoint{ -15,5,-5 };
	//힘을 한번만 가한다. 여기서 힘을 한번만 가한다의 단위는 F를 0.1초만큼 가하는것을 말한다.
	//애초에 힘을 한번만 가한다라고 정의할수는 없으며 힘은 F만큼 n초동안 가하는게 맞는말이다.
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

	//이후 그린다.

	Mesh.Render(commandlist);

}

void RigidCubeObject::Collision(list<CGameObject*>* collist, float DeltaTime)
{
	//이후에 추가해야한다.

	CollisionList = collist;
	//충돌리스트의 모든 요소와 충돌검사를 실시한다.
	for (auto i = CollisionList->begin(); i != CollisionList->end(); i++)
	{

		if (*i != this)
		{
			if ((*i)->rb != NULL)
			{
				bool test = rb->CollisionTest(*(*i)->rb, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

				if (test)//충돌했으면 충돌해소를 해야한다.
				{


					//충돌 했을때 축이 (0,1,0) 이면 Airbone을 false로 둔다. 이는 내가 위에있음을 나타낸다.
					if (rb->CollisionPointVector[0].pAxis.y > 0)
					{

						rb->SetVelocity(rb->GetVelocity().x, 0, rb->GetVelocity().z);
						AirBone = false;
					}
					//충돌했을때  축이 (0,-1,0)이면 상대방 Airbone을 false로 둔다.  이는 상대가 내 위에있음을 나타낸다.
					//설사 상대 위에 다른 상대가 있어도 걱정말자. 자연스러운것임.
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
				if ((*i)->pp != NULL) // pp가 NULL이 아니면 질점 오브젝트이다.
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

					if (test)//충돌했으면 충돌해소를 해야한다.
					{
						//충돌 했을때 축이 내가 아래로 내려가면 Airbone을 false로 둔다. 이는 내가 아래에있음을 나타낸다.
						//즉 플레이어가 위에있고 내가 아래임.
						if (rb->CollisionPointVector[0].pAxis.y < 0)
						{

							ppConvertrb.SetVelocity(ppConvertrb.GetVelocity().x, 0, ppConvertrb.GetVelocity().z);
							(*i)->AirBone = false;
						}

						
						if ((*i)->obs == Bullet)
						{
							(*i)->DelObj = true;
							//충돌 처리. 충격량을 가하고 겹침부분해소
							rb->CollisionResolve(ppConvertrb, XMFLOAT3(0, 0, 0), DeltaTime, 6000, 1400, 1.5);

							(*i)->pp->SetVelocity(ppConvertrb.GetVelocity());
							(*i)->pp->SetPosition(ppConvertrb.GetPosition());
							(*i)->pp->SetAccel(ppConvertrb.GetAccel());
							(*i)->UpdatePPosCenterPos();
						}
						else
						{
							//충돌처리 현재는 그냥 겹침부분만 해소중

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



//================================벽오브젝트=======================================

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	auto q = XMLoadFloat4(&Orient);//방향을 degree만큼 돌리려 한다.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, dgree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.80f;//스페큘러를 낮게준다.

	obs = Static;
	//게임관련 데이터들
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;

	XMFLOAT3 raxis{ 0,1,0 };
	//광선충돌 검사용 육면체
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

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(20, 10, 5);//충돌 박스의 x,y,z 크기
	pp->SetDamping(0.5f);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
	pp->SetMass(INFINITY);//고정된 물체는 무게가 무한이다.


}

void SmallWallObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 40, 20, 10);

	//모델 로드
	//LoadMD5Model(L".\\플레이어메쉬들\\Cube.MD5MESH", &Mesh, 0, 1);
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
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["SmallWall"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//이후 그린다.

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
	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	auto q = XMLoadFloat4(&Orient);//방향을 degree만큼 돌리려 한다.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, dgree);
	Orient = QuaternionMultiply(Orient, q2);

	obs = Static;
	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.2f;//스페큘러를 낮게준다.

									 //게임관련 데이터들
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;


	XMFLOAT3 raxis{ 0,1,0 };
	//광선충돌 검사용 육면체
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

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(350, 50, 5);//충돌 박스의 x,y,z 크기
	pp->SetDamping(0.5f);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
	pp->SetMass(INFINITY);//고정된 물체는 무게가 무한이다.


}

void BigWallObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 700, 100, 10);

	//모델 로드
	//LoadMD5Model(L".\\플레이어메쉬들\\Cube.MD5MESH", &Mesh, 0, 1);
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
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["Wall"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//이후 그린다.

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	auto q = XMLoadFloat4(&Orient);//방향을 degree만큼 돌리려 한다.
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, dgree);
	Orient = QuaternionMultiply(Orient, q2);

	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.



									 //게임관련 데이터들
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;
	staticobject = true;
	obs = Static;
	//광선충돌 검사용 육면체
	XMFLOAT3 rx(15, 0, 0);
	XMFLOAT3 ry(0, 45, 0);
	XMFLOAT3 rz(0, 0, 15);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(15, 45, 15);//충돌 박스의 x,y,z 크기
	pp->SetDamping(0.5f);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
	pp->SetMass(INFINITY);//고정된 물체는 무게가 무한이다.

}

void BuildingObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 30, 90, 30);

	//모델 로드
	//LoadMD5Model(L".\\플레이어메쉬들\\Cube.MD5MESH", &Mesh, 0, 1);
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
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);
}

//범위 오브젝트

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);


	UpdateLookVector();
	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.
	ObjData.BlendValue = 0.5f;
	Blending = true;
	obs = UI;


	//게임관련 데이터들
	gamedata.GodMode = true;

	staticobject = true;


	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = NULL;

}

void RangeObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 60, 0.01, 60);

	//모델 로드
	//LoadMD5Model(L".\\플레이어메쉬들\\Cube.MD5MESH", &Mesh, 0, 1);
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
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, Textures["CubeTex"].get()->Resource.Get(), false);
	UpdateConstBuffer(commandlist);

	//이후 그린다.

	Mesh.Render(commandlist);
}



////////////////////

ParticleObject::ParticleObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, list<CGameObject*>*Plist, CGameObject* master, float LifeTime, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, Plist, cp)
{

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.3f;//스페큘러를 낮게준다.
	ObjData.CustomData1.x = 2;
	Master = master;

	//게임관련 데이터들
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
	//여기서 좌표를 일괄적으로 설정 할 수 있다
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

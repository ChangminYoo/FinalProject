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
	
	//여기서는 기본적인것들만 처리한다. 위치나 회전각 등 초기화
	this->commandlist = commandlist;
	this->CenterPos = cp;//중점위치
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
	if (pp != NULL && rb==NULL)
		UpdatePPosCenterPos();//질점의 중점으로 센터포즈를 변경함. 반드시 충돌하거나 하면 처리해야함
	SetWorldMatrix();//현재 포지션과 로테이션정보로 월드행렬을 만든다.

	XMMATRIX world = XMLoadFloat4x4(&ObjData.WorldMatrix);

	XMStoreFloat4x4(&ObjData.WorldMatrix, XMMatrixTranspose(world));

	
	//상수버퍼 업데이트
	ConstBuffer->CopyData(0, ObjData);

	commandlist->SetGraphicsRootConstantBufferView(2, ConstBuffer->Resource()->GetGPUVirtualAddress());//월드행렬연결
}

CCubeManObject::CCubeManObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, XMFLOAT4 cp) : CGameObject(m_Device,commandlist,cp)
{

	//게임오브젝트 생성자에서 기본적인것을 처리했으므로 여기서는
	//메쉬와 텍스처 사용시 불러오기와 애니메이션 로드등을 처리해야한다.
	
	//조인트가 저장될 배열.
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
	ObjData.Scale =3;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.
	
	//게임 데이터 (스텟)을 찍는다. 캐릭터는 데미지를 갖지 않고, 탄환이 데미지를 갖도록하자.
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Speed = 60;
	
	//광선충돌 검사용 육면체
	XMFLOAT3 rx(3, 0, 0);
	XMFLOAT3 ry(0, 9, 0);
	XMFLOAT3 rz(0, 0, 3);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(3, 9, 3);//충돌 박스의 x,y,z 크기
	pp->SetDamping(0.5);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.

	

}


void CCubeManObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{

	//모델 로드
	LoadMD5Model(L".\\플레이어메쉬들\\cIdle.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(true);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

	//애니메이션 로드
	LoadMD5Anim(m_Device, L".\\플레이어메쉬들\\cIdle.MD5ANIM",&Mesh, this, animations);//0
	LoadMD5Anim(m_Device, L".\\플레이어메쉬들\\cRunning.MD5ANIM", &Mesh, this, animations);//1
	LoadMD5Anim(m_Device, L".\\플레이어메쉬들\\cAttack.MD5ANIM", &Mesh, this, animations);//2
	
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
		if (TickValue > 1)
		{
			//애니메이션 업데이트
			UpdateMD5Model(commandlist, &Mesh, this, gt.DeltaTime() * 8, n_Animation, animations, jarr);

			TickValue = 0;
		}
	}
	TickValue += 1;

	
	
}

void CCubeManObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	

	if(Textures.size()>0)
		SetTexture(commandlist,SrvDescriptorHeap,false);
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
			
			bool test=pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector,(*i)->GetUpvector());

			if (test)//충돌했으면 충돌해소를 해야한다.
			{

				//충돌 했을때 축이 (0,1,0) 이면 Airbone을 false로 둔다. 이는 내가 위에있음을 나타낸다.
				if (pp->pAxis.y >0)
					AirBone = false;
				//충돌했을때  축이 (0,-1,0)이면 상대방 Airbone을 false로 둔다.  이는 상대가 내 위에있음을 나타낸다.
				//설사 상대 위에 다른 상대가 있어도 걱정말자. 자연스러운것임.
				if (pp->pAxis.y <0)
					(*i)->AirBone = false;


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
	
	if (nAni == (int)Ani_State::Attack )//공격하기였으면
	{
		SetAnimation((int)Ani_State::Idle);//대기상태로둔다.

	}
	
}



CZombieObject::CZombieObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, XMFLOAT4 cp) : CGameObject(m_Device, commandlist, cp)
{

	//게임오브젝트 생성자에서 기본적인것을 처리했으므로 여기서는
	//메쉬와 텍스처 사용시 불러오기와 애니메이션 로드등을 처리해야한다.

	//조인트가 저장될 배열.
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
	ObjData.Scale =0.1;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.
	
	gamedata.MAXHP = 100;
	gamedata.HP = 100;
	gamedata.Speed = 60;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(4, 0, 0);
	XMFLOAT3 ry(0, 8, 0);
	XMFLOAT3 rz(0, 0, 4);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(4,8,4);//충돌 박스의 x,y,z 크기
	pp->SetDamping(0.5);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.



}


void CZombieObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{

	//모델 로드
	LoadMD5Model(L".\\플레이어메쉬들\\dietzombie2.MD5MESH", &Mesh, 0, 1);
	Mesh.SetNormal(false);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

	//애니메이션 로드
	LoadMD5Anim(m_Device, L".\\플레이어메쉬들\\attack1.MD5ANIM", &Mesh, this, animations);//0
	
}

void CZombieObject::SetMaterial(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	if (Mat.ConstBuffer == NULL)
		Mat.ConstBuffer = new UploadBuffer<MaterialData>(m_Device, 1, true);


	Mat.MatData.Roughness = 0.3f;
}

void CZombieObject::Tick(const GameTimer & gt)
{
	//적분기. 적분기란? 매 틱마다 힘! 에의해서 변화 되는 가속도/속도/위치를 갱신한다.
	//이때 pp의 position과 CenterPos를 일치시켜야하므로 CenterPos의 포인터를 인자로 넘겨야 한다.
	pp->integrate(gt.DeltaTime(), &CenterPos);

	if (ObjData.isAnimation == true)
	{
		if (TickValue > 1)
		{
			//애니메이션 업데이트
			UpdateMD5Model(commandlist, &Mesh, this, gt.DeltaTime() * 8, n_Animation, animations, jarr);

			TickValue = 0;
		}
	}
	TickValue += 1;



}

void CZombieObject::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.



	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, false);
	UpdateConstBuffer(commandlist);

	Mat.UpdateConstantBuffer(commandlist);

	//틱함수에서 업데이트한 애니메이션된 조인트를 연결함.
	commandlist->SetGraphicsRootConstantBufferView(3, jarr->Resource()->GetGPUVirtualAddress());
	//이후 그린다.

	Mesh.Render(commandlist);

}

//충돌기. 충돌검출과 충돌해소를 맡는다.
void CZombieObject::Collision(list<CGameObject*>* collist, float DeltaTime)
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
				if (pp->pAxis.y >0)
					AirBone = false;
				//충돌했을때  축이 (0,-1,0)이면 상대방 Airbone을 false로 둔다.  이는 상대가 내 위에있음을 나타낸다.
				//설사 상대 위에 다른 상대가 있어도 걱정말자. 자연스러운것임.
				if (pp->pAxis.y <0)
					(*i)->AirBone = false;


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

				}



				//충돌해소 호출. 충돌해소 이후에 반드시 변경된 질점의 위치로 오브젝트위치를 일치시켜야한다.
				pp->CollisionResolve(*(*i)->pp, cn, DeltaTime);//좀비는 튕기지 않는다.
				UpdatePPosCenterPos();
				(*i)->UpdatePPosCenterPos();
			}
		}
	}
}

void CZombieObject::EndAnimation(int nAni)
{

	if (nAni == (int)Ani_State::Attack)//공격하기였으면
	{
		SetAnimation((int)Ani_State::Idle);//대기상태로둔다.

	}

}


//---------------------------------------------------------------------------------------------------------------------------------

void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, bool isCubeMap)
{
	//텍스처는 테이블을 쓸것이므로 힙과 테이블 두개를 연결해야함.
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


	//디스크립터 힙 생성
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


//------------------- 투 사 체 -----------------------//

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);
	//인자로 발사방향으로 룩벡터가 될정도로 회전한 ori값을 받고, 현재 방향(아직은 0,0,0,1)과 곱해준다.
	Orient = QuaternionMultiply(Orient, ori);

	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.
	
	//게임관련 데이터들
	gamedata.MAXHP = 1;
	gamedata.HP = 1;
	gamedata.Damage = 50;
	gamedata.GodMode = true;
	gamedata.Speed = 50;
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
	pp->SetHalfBox(1, 1, 1);//충돌 박스의 x,y,z 크기
	pp->SetDamping(1);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//룩벡터로 날아감


}


void BulletCube::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{

	//모델 로드
	LoadMD5Model(L".\\플레이어메쉬들\\Cube.MD5MESH", &Mesh, 0, 1);
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
	//적분기. 적분기란? 매 틱마다 힘! 에의해서 변화 되는 가속도/속도/위치를 갱신한다.
	//이때 pp의 position과 CenterPos를 일치시켜야하므로 CenterPos의 포인터를 인자로 넘겨야 한다.
	pp->integrate(gt.DeltaTime(), &CenterPos);

	//No애니메이션!

	//투사체는 생명 주기가 있어야 한다.
	LifeTime -= gt.DeltaTime();
	if (LifeTime <= 0)
		DelObj = true;

}

void BulletCube::Render(ID3D12GraphicsCommandList * commandlist, const GameTimer& gt)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap,false);
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
		
		if (*i != this && *i!=Master && (*i)->pp!=NULL) // pp가 NULL이 아니면 질점 오브젝트이다.
		{

			bool test = pp->CollisionTest(*(*i)->pp, Lookvector, Rightvector, GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector());

			if (test)//충돌했으면 pp의 경우는 그냥 데미지를 주고 자신을 없애면 됨. 
			{
				(*i)->ToDamage(gamedata.Damage);
							
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

				}

				//충돌후 속도를 계산함.
				pp->ResolveVelocity(*(*i)->pp, cn, DeltaTime);
				//겹치는 부분을 제거할필요가 없는게 투사체는 어처피 사라지니까.
				DelObj = true;
				
			}
		}
	}
}


//---------------------- 스태틱 오브젝트 -----------------------------//

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

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookVector();

	ObjData.isAnimation = 0;
	ObjData.Scale = 1.0f;
	ObjData.SpecularParamater = 0.0f;//스페큘러를 낮게준다.

	//게임관련 데이터들
	gamedata.MAXHP = 100000;
	gamedata.HP = 100000;
	gamedata.Damage = 0;
	gamedata.GodMode = true;
	gamedata.Speed = 0;


	//광선충돌 검사용 육면체
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	//질점오브젝트 사용시 필요한 데이터들 설정
	pp = new PhysicsPoint();
	pp->SetPosition(CenterPos);//이 값은 항상 갱신되야한다.
	pp->SetHalfBox(0, 0, 0);//충돌 박스의 x,y,z 크기
	pp->SetDamping(1);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	pp->SetBounce(false);//튕기지 않는다.
	pp->SetVelocity(Lookvector.x*gamedata.Speed, Lookvector.y*gamedata.Speed, Lookvector.z*gamedata.Speed);//룩벡터로 날아감

}


void SphereObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	//모델 로드
	LoadMD5Model(L".\\플레이어메쉬들\\sphere.MD5MESH", &Mesh, 0, 1024);
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
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap, true);
	UpdateConstBuffer(commandlist);

	//이후 그린다.

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
	pp->SetMass(3);//고정된 물체는 무게가 무한이다.
	
	//테스트용 리지드 바디
	rb = new RigidBody();
	rb->SetPosition(&CenterPos);//이 값은 항상 갱신되야한다.
	rb->SetHalfBox(5, 5, 5);//충돌 박스의 x,y,z 크기
	rb->SetDamping(0.8f,0.8f);//마찰력 대신 사용되는 댐핑계수. 매 틱마다 0.5배씩 속도감속
	rb->SetBounce(false);//튕기지 않는다.
	rb->SetMass(5);//고정된 물체는 무게가 무한이다.
	rb->SetIMoment(5, 5, 5);
	rb->SetOrient(&Orient);
	XMFLOAT3 testForce{ 0,0,1000 };
	XMFLOAT3 testPoint{ -15,0,-5 };
	rb->AddForcePoint(testForce,testPoint);
}

void CubeObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist)
{
	CreateCube(&Mesh, 1);

	//모델 로드
	//LoadMD5Model(L".\\플레이어메쉬들\\Cube.MD5MESH", &Mesh, 0, 1);
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
	//테스트용 리지드 바디 코드.
	rb->integrate(gt.DeltaTime());

	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.

	if (Textures.size()>0)
		SetTexture(commandlist, SrvDescriptorHeap,false);
	UpdateConstBuffer(commandlist);

	//이후 그린다.

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
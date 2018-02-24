#include "CGameObject.h"

extern UINT CbvSrvDescriptorSize;





CGameObject::CGameObject()
{
	
}


CGameObject::~CGameObject()
{
	if (ConstBuffer != NULL)
		delete ConstBuffer;
	
}

CGameObject::CGameObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist,XMFLOAT4 cp)
{
	
	//여기서는 기본적인것들만 처리한다. 위치나 회전각 등 초기화
	this->commandlist = commandlist;
	this->CenterPos = cp;//중점위치
	ThetaY = 0;
	SetWorldMatrix();	//월드변환생성
	CreateConstBuffer(m_Device);//상수버퍼생성
}

void CGameObject::SetWorldMatrix()
{
	auto wmatrix = XMMatrixIdentity();
	auto pos = XMLoadFloat4(&CenterPos);
	wmatrix *= XMMatrixRotationY(ThetaY*MMPE_PI/180);
	wmatrix *= XMMatrixTranslationFromVector(pos);
	XMStoreFloat4x4(&ObjData.WorldMatrix, wmatrix);
}

void CGameObject::Render(ID3D12GraphicsCommandList * commandlist)
{
	
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

	SetWorldMatrix();//현재 포지션과 로테이션정보로 월드행렬을 만든다.

	XMMATRIX world = XMLoadFloat4x4(&ObjData.WorldMatrix);

	XMStoreFloat4x4(&ObjData.WorldMatrix, XMMatrixTranspose(world));

	
	//상수버퍼 업데이트
	ConstBuffer->CopyData(0, ObjData);

	commandlist->SetGraphicsRootConstantBufferView(1, ConstBuffer->Resource()->GetGPUVirtualAddress());//월드행렬연결
}

CZombieObject::CZombieObject(ID3D12Device * m_Device, ID3D12GraphicsCommandList * commandlist, XMFLOAT4 cp) : CGameObject(m_Device,commandlist,cp)
{

	//게임오브젝트 생성자에서 기본적인것을 처리했으므로 여기서는
	//메쉬와 텍스처 사용시 불러오기와 애니메이션 로드등을 처리해야한다.
	
	if (CreateMesh == false)
	{
		
		Mesh.Index = NULL;
		Mesh.SubResource = NULL;
		LoadTexture(m_Device, commandlist,this, Textures,SrvDescriptorHeap,"ZombieTex", L"textures/zombie_diffuse.dds");
		SetMesh(m_Device, commandlist);
		CreateMesh = true;
		
	}

	//게임오브젝트마다 룩벡터와 라이트벡터가 다르므로 초기 오프셋 설정을 해준다.
	//실제 룩벡터 등은 모두 UpdateLookVector에서 처리된다(라이트벡터도) 따라서 Tick함수에서 반드시 호출해야한다.
	OffLookvector = XMFLOAT3(0, 0, -1);
	OffRightvector = XMFLOAT3(-1, 0, 0);
	ThetaY = 180;
	ObjData.Scale = 0.1;
	Speed = 60;

	XMFLOAT3 rx(4, 0, 0);
	XMFLOAT3 ry(0, 8, 0);
	XMFLOAT3 rz(0, 0, 4);

	rco.SetPlane(rx, ry, rz);

	
}


void CZombieObject::SetMesh(ID3D12Device * m_Device, ID3D12GraphicsCommandList* commandlist)
{
	//조인트 배열 생성
	Mesh.jarr = new UploadBuffer<JointArr>(m_Device, 1, true);
	//모델 로드
	LoadMD5Model(L".\\플레이어메쉬들\\dietzombie2.MD5MESH", &Mesh, 0, 1, 0.4);
	Mesh.CreateVertexBuffer(m_Device, commandlist);
	Mesh.CreateIndexBuffer(m_Device, commandlist);

	//애니메이션 로드
	LoadMD5Anim(m_Device, L".\\플레이어메쉬들\\attack1.MD5ANIM",&Mesh, this, animations);
}

void CZombieObject::Tick(const GameTimer & gt)
{
	
	if (ObjData.isAnimation == true)
	{
		//애니메이션 업데이트
		if (TickValue > 3)
		{
			UpdateMD5Model(commandlist, &Mesh, this, gt.DeltaTime() * 8, n_Animation, animations);
			TickValue = 0;
		}
	}
	TickValue += 1;
}

void CZombieObject::Render(ID3D12GraphicsCommandList * commandlist)
{
	//게임오브젝트의 렌더링은 간단하다. 
	//텍스처를 연결하고, 월드행렬을 연결한다.


	if(Texturing)
		SetTexture(commandlist,SrvDescriptorHeap);
	UpdateConstBuffer(commandlist);
	
	//틱함수에서 업데이트한 애니메이션된 조인트를 연결함.
	commandlist->SetGraphicsRootConstantBufferView(2, Mesh.jarr->Resource()->GetGPUVirtualAddress());
	//이후 그린다.

	Mesh.Render(commandlist);

}

void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap)
{
	//텍스처는 테이블을 쓸것이므로 힙과 테이블 두개를 연결해야함.
	ID3D12DescriptorHeap* descriptorHeaps[] = { SrvDescriptorHeap.Get() };
	commandlist->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(0, CbvSrvDescriptorSize);

	commandlist->SetGraphicsRootDescriptorTable(0, tex);
}

void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist,CGameObject* obj, unordered_map<string, unique_ptr<CTexture>>& Textures, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, string texturename, wstring FileName)
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
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = Texs->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	device->CreateShaderResourceView(Texs.Get(), &srvDesc, hDescriptor);
}
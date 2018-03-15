#include "Scene.h"


Scene::Scene(ID3D12Device * m_Device, ID3D12GraphicsCommandList * m_DC, float cw, float ch)
{

	device = m_Device;
	commandlist = m_DC;

	mWidth = cw;
	mHeight = ch;
	
	XMFLOAT3 e(0, 60, -80);
	XMFLOAT3 a(0, 0, 0);
	XMFLOAT3 u(0, 1, 0);
	
	//루트시그니처와 쉐이더들을 생성한다.

	CreateRootSignature();
	nShader = 1;
	Shaders = new Shader;
	CreateShaderObject();
	Player = new CPlayer(m_Device, m_DC, cw / ch, e, a, u);
	Shaders->player = Player;//이제 플레이어도 설정되야 한다.
	light = new CLight(m_Device, m_DC);
	CreateGameObject();



}


Scene::Scene()
{
}


Scene::~Scene()
{
	if (Shaders != NULL)
	{
		
		delete Shaders;
	}
	while (DynamicObject.size())
	{
		delete DynamicObject.front();
		DynamicObject.pop_front();
	}
	while (BulletObject.size())
	{
		delete BulletObject.front();
		BulletObject.pop_front();
	}
	while (StaticObject.size())
	{
		delete StaticObject.front();
		StaticObject.pop_front();
	}
	if(SkyObject!=NULL)
		delete SkyObject;
	


	if (Player != NULL)
		delete Player;
	if (light != NULL)
		delete light;
}


void Scene::CreateRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE SkytexTable;
	SkytexTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0);


	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0);


	//연결될 리소스들 목록. 현재는 큐브의 월드변환행렬 하나만 넣을것.
	CD3DX12_ROOT_PARAMETER RootParameter[7];

	//파라메터의 종류는 3가진데  디스크립터쓸거임


	//파라메터목록에 해당 목록을 인자로 넣으면서 초기화함
	RootParameter[0].InitAsDescriptorTable(1, &SkytexTable, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParameter[1].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);

	RootParameter[2].InitAsConstantBufferView(0); //obj     //register b0
	RootParameter[3].InitAsConstantBufferView(1); //joint   //b1
	RootParameter[4].InitAsConstantBufferView(2); //camera  //b2
	RootParameter[5].InitAsConstantBufferView(3); //light	//b3
	RootParameter[6].InitAsConstantBufferView(4); //material	//b4


	auto staticSamplers = GetStaticSamplers();

	// 루트시그니처는 루트파라메터의 배열이다.
	//루트시그니처의 정보구조체 생성
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(7, RootParameter, (UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	// 루트시그니처를 만들 메모리 구조
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	//리소스들을 직렬화한다. 예를들어 버퍼가 2개들어가면 b0,b1 이렇게 순서대로 들어감.
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(device->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&rootsg)));


}

void Scene::CreateShaderObject()
{
	for (int i = 0; i < nShader; i++)
	{
		Shaders = new Shader;
		Shaders->CreateShader(device, rootsg.Get(), L"ShaderFile.hlsl", "VS", "vs_5_1", L"ShaderFile.hlsl", "PS", "ps_5_1");
		Shaders->SkyObject = &SkyObject;
		Shaders->DynamicObject = &DynamicObject;
		Shaders->BulletObject = &BulletObject;
		Shaders->StaticObject = &StaticObject;
	}
	
}

void Scene::CreateGameObject()
{

	//--------------- 메쉬와 텍스처 초기화 -------------//

	CGameObject* resource = NULL;
	resource = new CCubeManObject(device, commandlist, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new CZombieObject(device, commandlist, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new BulletCube(device, commandlist,NULL,XMFLOAT4(0,0,0,1),NULL, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new SphereObject(device, commandlist, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new CubeObject(device, commandlist, XMFLOAT4(0, 0, 0, 0));

	//--------------------------------------------------//
	
	SkyObject = new SphereObject(device, commandlist, XMFLOAT4(30, 0, 0, 0));

	DynamicObject.push_back(new CCubeManObject(device, commandlist,XMFLOAT4(0,-0,0,0)));
	DynamicObject.push_back(new CCubeManObject(device, commandlist, XMFLOAT4(0, 20, 0, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, XMFLOAT4(-20, 0, 0, 0)));

//	DynamicObject.push_back(new CCubeManObject(device, commandlist, XMFLOAT4(40,60, 0, 0)));
	//플레이어의 오브젝트 설정. 이건 나중에 바꿔야함.
	Player->SetPlayer(DynamicObject.front());
	Player->PlayerObject->Blending = false;



}

void Scene::Render(const GameTimer& gt)
{

	//나중에 카메라에 뷰포트와 씨저렉트를 갖도록하고 여기에서 연결하도록하자

	//루트시그니처연결. 이때 루트시그니처는 쉐이더 종류에 관계없이 동일해야함.
	commandlist->SetGraphicsRootSignature(rootsg.Get());


	//이제 쉐이더를 연결하고 PSO 연결후 쉐이더의 렌더함수호출. 그다음 게임오브젝트의 상수버퍼(시그니처에서 했던거)를 연결해야함
	//테이블의 경우 힙과 테이블을 연결해야함.
	if (nShader > 0)
	{
			Shaders->SetShader(commandlist);
			//여기까지 오면  정점버퍼, 인덱스버퍼 , 상수버퍼뷰만 연결하면 된다.

			//여기에 카메라행렬,프로젝션행렬을 추가한다.
			Player->Camera.UpdateConstantBuffer(commandlist);
			light->UpdateConstantBuffer(commandlist);

			
			//쉐이더가 보유한 그려야할 오브젝트 목록을 그린다.
			Shaders->Render(commandlist,gt);
			
		
	}
}

void Scene::Tick(const GameTimer & gt)
{
	//플레이어 입력처리 
	Player->PlayerInput(gt.DeltaTime(),this);

	//--------------------------------------------------------------
	//오브젝트 삭제
	//--------------------------------------------------------------
	//애니메이션있는 오브젝트
	for (auto i = DynamicObject.begin(); i != DynamicObject.end();)
	{

		if ((*i)->DelObj==true)
		{
			delete *i;//실제 게임오브젝트의 메모리 해제
			i = DynamicObject.erase(i);//리스트상에서 해당 요소를 지움


		}
		else
			i++;
	}
	//투사체
	for (auto i = BulletObject.begin(); i != BulletObject.end();)
	{

		if ((*i)->DelObj == true)
		{
			delete *i;//실제 게임오브젝트의 메모리 해제
			i = BulletObject.erase(i);//리스트상에서 해당 요소를 지움


		}
		else
			i++;
	}
	//스테틱 오브젝트
	for (auto i = StaticObject.begin(); i != StaticObject.end();)
	{
		if ((*i)->DelObj == true)
		{
			delete *i;//실제 게임오브젝트의 메모리 해제
			i = BulletObject.erase(i);//리스트상에서 해당 요소를 지움


		}
		else
			i++;
	}


	//--------------------------------------------------------------
	//오브젝트 틱함수 처리
	//--------------------------------------------------------------
	for (auto b = DynamicObject.begin(); b != DynamicObject.end(); b++)
		(*b)->Tick(gt);

	//불렛
	for (auto b = BulletObject.begin(); b != BulletObject.end(); b++)
		(*b)->Tick(gt);

	//스테틱 오브젝트
	for (auto b = StaticObject.begin(); b != StaticObject.end(); b++)
		(*b)->Tick(gt);


	//카메라 리 로케이트 
	Player->PlayerCameraReLocate();
}


array<const CD3DX12_STATIC_SAMPLER_DESC, 6> Scene::GetStaticSamplers()
{

	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}
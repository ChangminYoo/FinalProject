#include "Scene.h"
#include"FSM.h"

Scene::Scene(HWND hwnd,ID3D12Device * m_Device, ID3D12GraphicsCommandList * m_DC, float cw, float ch)
{
	hWnd = hwnd;
	device = m_Device;
	commandlist = m_DC;

	mWidth = cw;
	mHeight = ch;
	
	XMFLOAT3 e(0, 55, -65);
	XMFLOAT3 a(0, 13, 0);
	XMFLOAT3 u(0, 1, 0);
	
	//루트시그니처와 쉐이더들을 생성한다.

	CreateRootSignature();
	nShader = 1;
	Shaders = new Shader;
	CreateShaderObject();
	Player = new CPlayer(hWnd,m_Device, m_DC, cw / ch, e, a, u);
	Shaders->player = Player;//이제 플레이어도 설정되야 한다.
	light = new CLight(m_Device, m_DC);
	CreateUI();
	CreateGameObject();

	Sound = new CSound();
	Sound->Init();

}


Scene::Scene()
{
}


Scene::~Scene()
{
	if (AimUI != NULL)
		delete AimUI;
	if (BackGround != NULL)
		delete BackGround;
	for (int i = 0; i < 4; i++)
		if (SkillFrameUI[i] != NULL)
			delete SkillFrameUI[i];
	for (int i = 0; i < 4; i++)
		if (SkillUI[i] != NULL)
			delete SkillUI[i];
	if (SelectBar != NULL)
		delete SelectBar;

	if (EmptyObject != nullptr)
		delete EmptyObject;

	for (int i = 0; i < 4; i++)
		if(SkillCoolBar[i]!=NULL)
			delete SkillCoolBar[i];

	if (Shaders != NULL)
	{
		
		delete Shaders;
	}
	while (DynamicObject.size())
	{
		delete DynamicObject.front();
		DynamicObject.pop_front();
	}
	while (RigidObject.size())
	{
		delete RigidObject.front();
		RigidObject.pop_front();
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
	while (NoCollObject.size())
	{
		delete NoCollObject.front();
		NoCollObject.pop_front();
	}
	if(SkyObject!=NULL)
		delete SkyObject;
	while (BbObject.size())
	{
		delete BbObject.front();
		BbObject.pop_front();
	}
	while (LandObject.size())
	{
		delete LandObject.back();
		LandObject.pop_back();
	}

	if (Sound != NULL)
		Sound->DeleteSound();

	if (Player != NULL)
		delete Player;
	if (light != NULL)
		delete light;
}


void Scene::SceneState()
{
	if (GAMESTATE == GS_START)//시작시 생성자에서 UI등 기본적인것은 거기서 로드함.
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000&&GetFocus())
		{
			
			SetGameState(GS_LOAD);
		}

	}
	else if (GAMESTATE == GS_LOAD)
	{
		
		if (FirstLoad == true)
		{
			//CreateGameObject();
			FirstLoad = false;
			SetGameState(GS_PLAY);
			ShowCursor(false);

		/*	Sound->PlaySoundEffect(CSound::SoundType::TITLE);
			Sound->SetVolume(CSound::SoundType::TITLE, 0.5f);*/
		}
		else
		{
			//여기에서 로딩용 텍스처를 선택함.
			BackGround->TextureName = "LoadBG";
			BackGround->TexOff = 1;//다수의 텍스처이므로 TexOff를이용함.
			FirstLoad = true;

		}
	}
	else if (GAMESTATE == GS_END)
	{
		PostQuitMessage(0);
	}
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
		Shaders->CreateShader(device, rootsg.Get());
		Shaders->SkyObject = &SkyObject;
		Shaders->DynamicObject = &DynamicObject;
		Shaders->RigidObject = &RigidObject;
		Shaders->BulletObject = &BulletObject;
		Shaders->StaticObject = &StaticObject;
		Shaders->NoCollObject = &NoCollObject;
		Shaders->BbObject = &BbObject;
		Shaders->LandObject = &LandObject;
		Shaders->Shadows = &Shadows;

	}
	
}

void Scene::CreateGameObject()
{

	//--------------- 메쉬와 텍스처 초기화 -------------//

	CGameObject* resource = NULL;
	resource = new CCubeManObject(device, commandlist, &BbObject, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new BulletCube(device, commandlist,  &BbObject,NULL,XMFLOAT4(0,0,0,1),NULL, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new HeavyBulletCube(device, commandlist, &BbObject, NULL, XMFLOAT4(0, 0, 0, 1), NULL, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new SphereObject(device, commandlist,  &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new CubeObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new GridObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new RingObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new ShieldArmor(device, commandlist, &BbObject,NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;


	resource = new RangeObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new Tetrike(device, commandlist, &BbObject, NULL, NULL, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new Tetris1(device, commandlist, &BbObject, NULL, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new Tetris2(device, commandlist, &BbObject, NULL, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new Tetris3(device, commandlist, &BbObject, NULL, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new DiceStrike(device, commandlist, &BbObject, NULL, XMFLOAT4(0, 0, 0, 1), 0, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new DiceObject(device, commandlist, &BbObject, NULL, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new ShadowObject(device, commandlist, &BbObject, NULL, 0, XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0));
	delete resource;
	//--------------------------------------------------//
	
	SkyObject = new SphereObject(device, commandlist,  &BbObject, XMFLOAT4(0, 0, 0, 0));
	LandObject.push_back(new GridObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0)));

	DynamicObject.push_back(new CCubeManObject(device, commandlist,&BbObject, XMFLOAT4(0, 0, -240, 0)));
	Shadows.push_back(new ShadowObject(device, commandlist, &BbObject, DynamicObject.back(), 0, XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0)));

	DynamicObject.push_back(new CCubeManObject(device, commandlist,&BbObject, XMFLOAT4(100, 0, 110, 0)));
	Shadows.push_back(new ShadowObject(device, commandlist, &BbObject, DynamicObject.back(), 0, XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0)));


	CGameObject* imp = new ImpObject(device, commandlist, &BbObject, XMFLOAT4(-100, 0, 220, 0));
	((ImpObject*)imp)->fsm = new FSM(imp, &DynamicObject, &StaticObject);
	DynamicObject.push_back(imp);
	Shadows.push_back(new ShadowObject(device, commandlist, &BbObject, DynamicObject.back(), 2, XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0)));

	//DynamicObject.back()->pp->SetBounce(true);
	//DynamicObject.back()->pp->AddForce(-600, 0, 600);
	//DynamicObject.back()->pp->integrate(0.1f);//힘은 지속적으로 가해지는것이며 즉발적이려면 힘을 가한 시간을 통해 계산한다.
	
	//MoveCube
	/*
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, 155.0f, XMFLOAT4(0, 25, 155, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, -150.0f, XMFLOAT4(0, 52, -150, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, 145.0f, XMFLOAT4(145, 45, 0, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, -145.0f, XMFLOAT4(-145, 20, 0, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, -160.0f, XMFLOAT4(0, 17, -160, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, 140.0f, XMFLOAT4(140, 34, 0, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, 65.0f, XMFLOAT4(-65, 96, 0, 0)));
	*/

	//Cube
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(40, 18, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(50, 70, -80, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(35, 50, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(118, 58, 47, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(102, 69, 81, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(58, 39, 78, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(65, 35, -110, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(92, 61, 60, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(103, 36, 15, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(65, 17, -65, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(82, 11, 34, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(72, 53, 41, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(24, 44, 40, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-20, 33, 51, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(0, 24, 34, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(0, 58, -71, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-76, 53, 8, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-40, 10, 0, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-38, 48, -126, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-45, 30, -60, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-65, 40, -102, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-69, 62, 61, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-85, 42, -45, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-106, 40, 42, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-20, 5, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-20, 15, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-20, 25, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-20, 35, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-10, 35, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(0, 35, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(10, 35, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(10, 25, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(10, 15, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(10, 5, 210, 0)));


	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 5, -30, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 15, -30, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 25, -30, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 35, -30, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 35, -20, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 35, -10, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 35, 0, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 25, 0, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 15, 0, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(260, 5, 0, 0)));


	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-20, 5, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-20, 15, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-20, 25, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-20, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(-10, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(0, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(10, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(10, 25, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(10, 15, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, XMFLOAT4(10, 5, -280, 0)));

	//Smallwall
	//좌상 
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 200, 0, 480 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 140, 0, 400 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 80, 0, 320 * cosf(0.4f * MMPE_PI), 0)));

	//중상
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, MMPE_PI / 2, XMFLOAT4(0, 0, 530, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, MMPE_PI / 2, XMFLOAT4(0, 0, 460, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, MMPE_PI / 2, XMFLOAT4(0, 0, 390, 0)));

	//우상
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 200, 0, 480 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 140, 0, 400 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 80, 0, 320 * cosf(0.4f * MMPE_PI), 0)));

	//우하
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, MMPE_PI / 3, XMFLOAT4(520 * sinf(0.8f * MMPE_PI), 0, -500 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, MMPE_PI / 3, XMFLOAT4(470 * sinf(0.8f * MMPE_PI), 0, -440 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, MMPE_PI / 3, XMFLOAT4(420 * sinf(0.8f * MMPE_PI), 0, -380 * cosf(0.2f * MMPE_PI), 0)));

	//좌하
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, -MMPE_PI / 3, XMFLOAT4(-520 * sinf(0.8f * MMPE_PI), 0, -500 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, -MMPE_PI / 3, XMFLOAT4(-470 * sinf(0.8f * MMPE_PI), 0, -440 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, -MMPE_PI / 3, XMFLOAT4(-420 * sinf(0.8f * MMPE_PI), 0, -380 * cosf(0.2f * MMPE_PI), 0)));

	//BigWall
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, -BigWall_Rad1, XMFLOAT4(-BigWall_X1, 0, BigWall_Z1, 0)));//좌상
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, BigWall_Rad1, XMFLOAT4(BigWall_X1, 0, BigWall_Z1, 0)));//우상
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, -BigWall_Rad1 * 2, XMFLOAT4(BigWall_X2, 0, BigWall_Z2, 0)));//우하
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, 0, XMFLOAT4(0, 0, BigWall_Z3, 0)));//하
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, BigWall_Rad1 * 2, XMFLOAT4(-BigWall_X2, 0, BigWall_Z2, 0)));//좌하


	//building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, 0, XMFLOAT4(90, 0, 60, 0)));
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, 0, XMFLOAT4(-75, 0, 40, 0)));
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, 0, XMFLOAT4(0, 0, -100, 0)));


	//RigidObject
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(0, 200, 290, 0)));
	//RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(-270, 250, 60, 0)));
	//RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(270, 330, 60, 0)));
	//RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(-210, 390, -200, 0)));
	//RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(200, 180, -180, 0)));
	//RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(80, 310, -30, 0)));
	//RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(-31, 250, 160, 0)));
	//RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(90, 270, -340, 0)));
	//RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(-70, 220, -55, 0)));
	//RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(193, 160, 40, 0)));

	int player_num = 0;
	for (auto obj : DynamicObject)
	{
		if (obj->isNPC) continue;

		obj->m_player_data.id = player_num;
		++player_num;
	}

	player_num = 0;
	for (auto sobj : StaticObject)
	{
		sobj->m_sobj_data.ID = player_num;
		++player_num;
	}

	for (auto rbobj : RigidObject)
	{
		rbobj->m_rigidbody_data.id = player_num;
		++player_num;
	}
	player_num = 0;
	
	//플레이어의 오브젝트 설정. 이건 나중에 바꿔야함.
	Player->SetPlayer(DynamicObject.front());
	Player->PlayerObject->Blending = false;
}

void Scene::CreateUI()
{

	AimUI = new AimObject(device,commandlist,NULL);
	
	BackGround = new BackGroundObject(device, commandlist, NULL,XMFLOAT4(0, 0,0,0));

	SelectBar = new SelectBarObject(device, commandlist, NULL, XMFLOAT4(0 * 100 - 150, 0.9*-mHeight / 2, 0, 0));

	
	for (int i = 0; i < 4; i++)
	{
		float ct = 0;
		switch (Player->skilldata.Skills[i])//스킬넘버링
		{
		case 0://라이트큐브
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
			break;
		case 1://헤비큐브
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
			break;
		case 2://테트라이크
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
			break;
		case 3://다이스트라이크
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
			break;

		case 4: // 파동파
			ct = ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];

		case 5: //실드
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
		}

		SkillCoolBar[i] = new CoolBarObject(device, commandlist, NULL,ct, Player->PlayerObject, XMFLOAT4(i*100-150,0.98*-mHeight / 2, 0, 0));
		SkillFrameUI[i] = new SkillFrameUIObject(device, commandlist, NULL, Player->skilldata.Skills[i], XMFLOAT4(i * 100 - 150, 0.9*-mHeight / 2, 0, 0));
		SkillUI[i] = new SkillUIObject(device, commandlist, NULL, Player->skilldata.Skills[i], XMFLOAT4(i * 100 - 150, 0.9*-mHeight / 2, 0, 0));
	}

}

void Scene::UITick(const GameTimer & gt)
{
	if (Player->PlayerObject != NULL)
	{
		//스킬쿨타임 UI 커스텀데이터y값을 쿨타임에 맞게 변경
		for (int i = 0; i < 4; i++)
		{
			SkillCoolBar[i]->ObjData.CustomData1.y = Player->skilldata.SkillsCoolTime[i] / ((CoolBarObject*)SkillCoolBar[i])->MaxCoolTime;

			SelectBar->CenterPos = XMFLOAT4(Player->skilldata.SellectBulletIndex * mWidth / 8 - (mWidth / 8)*1.5, 0.9*-mHeight / 2, 0, 0);
		}
		
		if (resize)
		{
			SelectBar->ObjData.Scale = mWidth / 10;
			resize = false;
		}
	}
}

void Scene::Render(const GameTimer& gt)
{
	/*
		렌더하려면 루트시그니처/PSO/카메라와 샘플러 광원 과 같은 공유 리소스 / 메쉬/월드행렬/텍스처 를 연결하면 된다.
		루트시그니처의 경우 리소스를 연결하기 전에 반드시 연결할것. 보통 첫 시작에서 이걸 연결하면 편함.
	*/


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
			
			
			if (GetGameState() == GS_PLAY)
			{
				//쉐이더가 보유한 그려야할 오브젝트 목록을 그린다.
				Shaders->Render(commandlist, gt);

				//UI를 여기서 그린다. UI는 따로 리스트등이 없음.

				//먼저 루트시그니처는 그대로 사용해도 되고 카메라의 경우 UI는 직교투영을 써야하므로 기존 카메라행렬이 아닌
				//직교투영용 카메라 행렬을 연결한다. 이후 빌보드용 PSO를 연결한다. 루트/pso/공유리소스 까지 다 되었으니 월드행렬과 메쉬를 연결한다.
				Player->Camera.UpdateConstantBufferOrtho(commandlist);
				Shaders->SetBillboardShader(commandlist);
				AimUI->Render(commandlist, gt);

				for (int i = 0; i < 4; i++)
				{
					SkillCoolBar[i]->Render(commandlist, gt);
				}
				SkillUI[0]->Render(commandlist, gt);
				SkillUI[1]->Render(commandlist, gt);
				SkillUI[2]->Render(commandlist, gt);

				SelectBar->Render(commandlist, gt);
				for (int i = 0; i < 4; i++)
					SkillFrameUI[i]->Render(commandlist, gt);


			
				//다시 원상태로 바꿔줌. 이걸 안하면 피킹이 엉망이됨. 
				Player->Camera.UpdateConstantBuffer(commandlist);
			}
			else if (GetGameState() == GS_START || GetGameState()==GS_LOAD)
			{
				Player->Camera.UpdateConstantBufferOrtho(commandlist);
				Shaders->SetBillboardShader(commandlist);
				BackGround->Render(commandlist, gt);
				Player->Camera.UpdateConstantBuffer(commandlist);
			}
	}
}

void Scene::Tick(const GameTimer & gt)
{
	//플레이어 입력처리 
	Player->PlayerInput(gt.DeltaTime(),this);

	if (GetGameState() == GS_PLAY)
	{
		//--------------------------------------------------------------
		//오브젝트 삭제
		//--------------------------------------------------------------
		//애니메이션있는 오브젝트
		for (auto i = DynamicObject.begin(); i != DynamicObject.end();)
		{

			if ((*i)->DelObj == true)
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
				i = StaticObject.erase(i);//리스트상에서 해당 요소를 지움
			}
			else
				i++;
		}

		for (auto i = BbObject.begin(); i != BbObject.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//실제 게임오브젝트의 메모리 해제
				i = BbObject.erase(i);//리스트상에서 해당 요소를 지움
			}
			else
				i++;
		}
		//리지드바디 오브젝트
		for (auto i = RigidObject.begin(); i != RigidObject.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//실제 게임오브젝트의 메모리 해제
				i = RigidObject.erase(i);//리스트상에서 해당 요소를 지움
			}
			else
				i++;
		}
		for (auto i = NoCollObject.begin(); i != NoCollObject.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//실제 게임오브젝트의 메모리 해제
				i = NoCollObject.erase(i);//리스트상에서 해당 요소를 지움
			}
			else
				i++;
		}

		for (auto i = Shadows.begin(); i != Shadows.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//실제 게임오브젝트의 메모리 해제
				i = Shadows.erase(i);//리스트상에서 해당 요소를 지움
			}
			else
				i++;
		}
		//--------------------------------------------------------------
		//오브젝트 틱함수 처리
		//--------------------------------------------------------------
		for (auto b = DynamicObject.begin(); b != DynamicObject.end(); b++)
			(*b)->Tick(gt);

		for (auto b = StaticObject.begin(); b != StaticObject.end(); b++)
			(*b)->Tick(gt);

		//불렛
		for (auto b = BulletObject.begin(); b != BulletObject.end(); b++)
			(*b)->Tick(gt);


		for (auto b = BbObject.begin(); b != BbObject.end(); b++)
			(*b)->Tick(gt);

		//리지드 바디
		for (auto b = RigidObject.begin(); b != RigidObject.end(); b++)
			(*b)->Tick(gt);
		
		
		for (auto b = NoCollObject.begin(); b != NoCollObject.end(); b++)
			(*b)->Tick(gt);

		for (auto b = Shadows.begin(); b != Shadows.end(); b++)
			(*b)->Tick(gt);

		//DynamicObject가 1이면 게임 종료 상태로 만든다!!
		if (DynamicObject.size() == 1)
			GAMESTATE = GS_END;

		Player->Tick(gt.DeltaTime());


		UITick(gt);
		//카메라 리 로케이트 
		Player->PlayerCameraReLocate();


		//서버 추가
		//다이스트라이크 주사위 정보에 따른 불렛 데이터 서버로 전송(다이스트라이크 시전 시에만 호출)
		//Orient는 플레이어 각도에 따라 업데이트된 Orient임. 
		if (EmptyObject->g_numofdice != 0)
		{
			bool check_first{ true };
			for (auto iter = BulletObject.crbegin(); iter != BulletObject.crend(); ++iter)
			{
				if (EmptyObject->g_numofdice <= 0) break;

				CTS_BulletObject_Info bullet_info;
				bullet_info.alive = true;
				bullet_info.endpoint = { 0.f, 0.f, 0.f };
				bullet_info.master_id = Player->PlayerObject->m_player_data.id;
				bullet_info.pos4f = { Player->PlayerObject->CenterPos.x,  Player->PlayerObject->CenterPos.y,  Player->PlayerObject->CenterPos.z,  Player->PlayerObject->CenterPos.w };
				bullet_info.rot4f = { (*iter)->Orient.x, (*iter)->Orient.y, (*iter)->Orient.z, (*iter)->Orient.w };
				bullet_info.degree = (*iter)->m_degree;
				bullet_info.vel3f = { (*iter)->pp->GetVelocity().x, (*iter)->pp->GetVelocity().y, (*iter)->pp->GetVelocity().z };
				bullet_info.my_id = (*iter)->m_bullet_data.my_id;

				bullet_info.type = BULLET_TYPE::protocol_DiceBullet;

				--EmptyObject->g_numofdice;

				CTS_SKILL_DICESTRIKE cts_skill_dicestrike;
				cts_skill_dicestrike.bull_data = move(bullet_info);
				cts_skill_dicestrike.lookvector = { Player->PlayerObject->Lookvector.x, Player->PlayerObject->Lookvector.y, Player->PlayerObject->Lookvector.z };
				
				if (check_first)
				{
					cts_skill_dicestrike.is_firstdice = true;
					check_first = false;
				}
				else { cts_skill_dicestrike.is_firstdice = false; }
				
				
				Player->m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_skill_dicestrike));

			}

			EmptyObject->g_numofdice = 0;
		}
	}
}

Player_Data * Scene::Get_PlayerServerData(const unsigned int & id)
{
	for (auto GameObject : DynamicObject)
	{
		if (GameObject->m_player_data.id == id)
		{
			Get_GameObject = GameObject;
			return &(GameObject->m_player_data);
		}
	}

	return nullptr;
}

Player_Data * Scene::Get_MonsterServerData(const unsigned int & id)
{
	return nullptr;
}

void Scene::SET_PLAYER_BY_SEVER_DATA(const unsigned short & id, const Player_Data & playerdata, const unsigned char & packet_type)
{
	for (auto GameObject : DynamicObject)
	{
		if (GameObject->isNPC) continue;
		if (GameObject->m_player_data.id == id)
		{
			switch (packet_type)
			{
			case PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT:
			{
				GameObject->m_player_data = move(playerdata);

				GameObject->Orient = { playerdata.rot.x , playerdata.rot.y, playerdata.rot.z, playerdata.rot.w };
				GameObject->CenterPos = { playerdata.pos.x , playerdata.pos.y , playerdata.pos.z , playerdata.pos.w };
				GameObject->pp->SetPosition(&GameObject->CenterPos);

				GameObject->gamedata.Damage = move(playerdata.status.attack);
				GameObject->gamedata.HP = move(playerdata.status.cur_hp);
				GameObject->gamedata.MAXHP = move(playerdata.status.origin_hp);
				GameObject->gamedata.Speed = move(playerdata.status.speed);

				GameObject->AirBone = playerdata.airbone;

			}
			break;

			case PACKET_PROTOCOL_TYPE::INIT_CLIENT:
			{
				GameObject->m_player_data = move(playerdata);

				GameObject->Orient = { playerdata.rot.x , playerdata.rot.y, playerdata.rot.z, playerdata.rot.w };
				GameObject->CenterPos = { playerdata.pos.x , playerdata.pos.y , playerdata.pos.z , playerdata.pos.w };
				GameObject->pp->SetPosition(&GameObject->CenterPos);

				GameObject->gamedata.Damage = move(playerdata.status.attack);
				GameObject->gamedata.HP = move(playerdata.status.cur_hp);
				GameObject->gamedata.MAXHP = move(playerdata.status.origin_hp);
				GameObject->gamedata.Speed = move(playerdata.status.speed);

				GameObject->AirBone = playerdata.airbone;

				if (GameObject->m_player_data.id == my_ClientID)
					Player->SetPlayer(GameObject);
			}
			break;

			case PACKET_PROTOCOL_TYPE::PLAYER_CURR_STATE:
			{
				GameObject->m_player_data.ai = playerdata.ai;
				GameObject->m_player_data.airbone = playerdata.airbone;
				GameObject->m_player_data.connect = playerdata.connect;
				GameObject->m_player_data.dir = playerdata.dir;
				GameObject->m_player_data.godmode = playerdata.godmode;
				GameObject->m_player_data.id = playerdata.id;
				GameObject->m_player_data.pos = playerdata.pos;
				GameObject->m_player_data.status = playerdata.status;

				GameObject->CenterPos = { playerdata.pos.x , playerdata.pos.y , playerdata.pos.z , playerdata.pos.w };
				GameObject->pp->SetPosition(&GameObject->CenterPos);

				GameObject->gamedata.Damage = move(playerdata.status.attack);
				GameObject->gamedata.HP = move(playerdata.status.cur_hp);
				GameObject->gamedata.MAXHP = move(playerdata.status.origin_hp);
				GameObject->gamedata.Speed = move(playerdata.status.speed);

				GameObject->AirBone = playerdata.airbone;
			}
			break;

			case PACKET_PROTOCOL_TYPE::PLAYER_DISCONNECT:
			{
				//client 내 flag를 false로
			}
			break;

			case PACKET_PROTOCOL_TYPE::PLAYER_ROTATE:
			{
				GameObject->m_player_data.rot = move(playerdata.rot);

				GameObject->Orient = { playerdata.rot.x , playerdata.rot.y, playerdata.rot.z, playerdata.rot.w };
			}
			break;

			case PACKET_PROTOCOL_TYPE::PLAYER_ANIMATION:
			{
				//GameObject->ObjData.isAnimation = true;

				GameObject->currAnimTime = 0.f;
				
				GameObject->m_player_data.ani = playerdata.ani;

				GameObject->n_Animation = static_cast<int>(playerdata.ani);

			}
			break;

			default:
				break;

			}

		}
	}
}

void Scene::SET_NPC_BY_SERVER_DATA(const unsigned short & id, const Npc_Data & data, const unsigned char& monster_type, const unsigned char& packet_type)
{
	for (auto GameObject : DynamicObject)
	{
		if (!GameObject->isNPC) continue;

		if (GameObject->m_npc_data.id == id)
		{
			switch (packet_type)
			{
				case PACKET_PROTOCOL_TYPE::INIT_NPC:
				{
					GameObject->m_npc_data = move(data);

					GameObject->Orient = { data.rot.x , data.rot.y, data.rot.z, data.rot.w };
					GameObject->CenterPos = { data.pos.x , data.pos.y , data.pos.z , data.pos.w };
					GameObject->pp->SetPosition(&GameObject->CenterPos);

					GameObject->gamedata.Damage = move(data.status.attack);
					GameObject->gamedata.HP = move(data.status.cur_hp);
					GameObject->gamedata.MAXHP = move(data.status.origin_hp);
					GameObject->gamedata.Speed = move(data.status.speed);

					GameObject->AirBone = data.airbone;
				}
				break;

				case PACKET_PROTOCOL_TYPE::NPC_MONSTER_CURR_STATE:
				{
					GameObject->m_npc_data = move(data);

					GameObject->n_Animation = data.ani;
					GameObject->Orient = { data.rot.x , data.rot.y, data.rot.z, data.rot.w };
					GameObject->CenterPos = { data.pos.x , data.pos.y , data.pos.z , data.pos.w };
					GameObject->pp->SetPosition(&GameObject->CenterPos);

					GameObject->gamedata.Damage = move(data.status.attack);
					GameObject->gamedata.HP = move(data.status.cur_hp);
					GameObject->gamedata.MAXHP = move(data.status.origin_hp);
					GameObject->gamedata.Speed = move(data.status.speed);

					GameObject->AirBone = data.airbone;

				}
				break;


			default:
				break;
			}
		}
	}
}

void Scene::SET_NPC_ATTACK_BY_SERVER_DATA(const unsigned short & id, const NPC_BulletObject_Info & data, unsigned char type, bool first_bullet)
{
	switch (type)
	{
		case BULLET_TYPE::protocol_NpcStoneBullet:
		{
			if (first_bullet)
			{
				BulletObject.emplace_back(new )
			}

			for (const auto& bullet : BulletObject)
			{

			}
		}
		break;

	default:
		break;
	}
}

void Scene::SET_SOBJECT_BY_SERVER_DATA(const unsigned short & id, StaticObject_Info & sobjdata, const unsigned char & type)
{
	switch (type)
	{
	case INSTALLED_OBJECT_TYPE::Box:
	case INSTALLED_OBJECT_TYPE::SmallWall:
	case INSTALLED_OBJECT_TYPE::BigWall:
	case INSTALLED_OBJECT_TYPE::NormalBuilding:
	{
		//클라는 0 ~ 77까지의 스테틱오브젝트가 순서대로 ID가 저장된 상태
		//서버에서 오는 데이터는 뒤죽박죽 섞인 ID를 갖는 스테틱오브젝트정보
		for (auto sobj : StaticObject)
		{
			if (sobj->m_sobj_data.ID == sobjdata.ID)
			{
				sobj->m_sobj_data = move(sobjdata);

				sobj->Orient = { sobjdata.Rotate_status.x , sobjdata.Rotate_status.y , sobjdata.Rotate_status.z, sobjdata.Rotate_status.w };
				sobj->CenterPos = { sobjdata.Pos.x ,sobjdata.Pos.y , sobjdata.Pos.z, sobjdata.Pos.w };

				sobj->m_sobj_data.degree = sobjdata.degree;
				sobj->m_sobj_data.Fixed = sobjdata.Fixed;
				sobj->m_sobj_data.type = sobjdata.type;

				sobj->pp->SetPosition(&sobj->CenterPos);
				break;
			}
		}
	}
	break;
	default:
		break;
	}
}

void Scene::SET_RIGIDOBJECT_BY_SERVER_DATA(const unsigned int & id, RigidbodyData & rbobjdata, const unsigned char & type)
{
	switch (type)
	{
		case INSTALLED_OBJECT_TYPE::Rigidbodybox:
		{
			for (auto rigid : RigidObject)
			{
				if (rigid->m_rigidbody_data.id == rbobjdata.id)
				{
					rigid->m_rigidbody_data = move(rbobjdata);

					rigid->Orient = { rbobjdata.rot4f.x, rbobjdata.rot4f.y, rbobjdata.rot4f.z, rbobjdata.rot4f.w };
					rigid->CenterPos = { rbobjdata.pos4f.x, rbobjdata.pos4f.y, rbobjdata.pos4f.z, rbobjdata.pos4f.w };
					
					rigid->rb->SetPosition(&rigid->CenterPos);


					//cout << "RigidybodyObject ID: " << rigid->m_rigidbody_data.id << "PosX: " << rigid->m_rigidbody_data.pos4f.x << "PosY: "
					//	<< rigid->m_rigidbody_data.pos4f.y << "PosZ: " << rigid->m_rigidbody_data.pos4f.z << "PosW: " << rigid->m_rigidbody_data.pos4f.w << "\n";
					
					break;
				}
			}
		}
		break;
	}
}

void Scene::SET_BULLET_BY_SERVER_DATA(STC_BulletObject_Info & bulldata, const unsigned char & packet_type, bool first_bullet, const XMFLOAT3& OffLookvector)
{
	switch (packet_type)
	{
	case BULLET_TYPE::protocol_LightBullet:
	case BULLET_TYPE::protocol_HeavyBullet:
	case BULLET_TYPE::protocol_DiceBullet:
	{
		//클라에서 먼저 불렛이 죽고 서버에서 죽었다는 정보를 주게되면 아래 분기문이 실행됨 -> 버그
		//내 자신은 이걸 실행할 필요가없음

		//이동 전 첫번째 불렛이다 - 이 때만 Create로 불렛 생성
		if (first_bullet)
		{
			//1. 현재 클라이언트 아이디와 서버에서 건너온 불렛을 쏜 클라이언트 아이디가 다르다 -> CrateOtherClientBullet 실행
			//2. 현재 클라이언트가 서버에서 건너온 불렛을 쏜 경우 -> Create 스킵하고 데이터(이동위치, 회전값 등)만 업데이트 해준다
			if (bulldata.master_id != Player->PlayerObject->m_player_data.id)
			{
				if (bulldata.type == protocol_DiceBullet)
					Player->CreateOtherClientDicestrikeSkill(device, commandlist, bulldata.endpoint, nullptr, &BulletObject, bulldata, OffLookvector);
				else
					Player->CreateOtherClientBullet(device, commandlist, bulldata.endpoint, nullptr, &BulletObject, bulldata);
				
				//break;
			}
		}

		for (auto& lbul : BulletObject)
		{
			if (bulldata.master_id == lbul->m_bullet_data.master_id && bulldata.my_id == lbul->m_bullet_data.my_id)
			{
				//불렛이 소멸됨 -> 삭제
				if (!bulldata.alive)
				{
					lbul->DelObj = true;

					//cout << "Bullet ID: " << bulldata.my_id << "Bullet MID: " << bulldata.master_id <<
					//	"Position: " << bulldata.pos4f.x << ", " << bulldata.pos4f.y << ", " << bulldata.pos4f.z << ", " << bulldata.pos4f.w <<
					//	"IsAlive: " << static_cast<bool>(bulldata.alive) << endl;

					break;
				}

				lbul->m_bullet_data = move(bulldata);

				lbul->CenterPos = { bulldata.pos4f.x, bulldata.pos4f.y, bulldata.pos4f.z, bulldata.pos4f.w };
				lbul->pp->SetPosition(&lbul->CenterPos);

				lbul->Orient = { bulldata.rot4f.x, bulldata.rot4f.y, bulldata.rot4f.z, bulldata.rot4f.w };
				lbul->UpdateLookVector();


				//cout << "Bullet ID: " << lbul->myID << "Orient: " << lbul->Orient.x << ", " << lbul->Orient.y << ", " << lbul->Orient.z << ", " << lbul->Orient.w << "\n";
				//cout << "Bullet ID: " << lbul->myID << "Lookxyz: " << lbul->Lookvector.x << ", " << lbul->Lookvector.y << ", " << lbul->Lookvector.z << "\n";
				//cout << "Bullet ID: " << lbul->myID << "Rightxyz: " << lbul->Rightvector.x << ", " << lbul->Rightvector.y << ", " << lbul->Rightvector.z << "\n";


				break;
			}
		}

	}
	break;
	
	default:
		break;
	}
}

void Scene::SET_PLAYER_ANIMATION_BY_SERVER_DATA(const unsigned short & id, const unsigned char & ani)
{
	for (auto GameObject : DynamicObject)
	{
		if (GameObject->m_player_data.id == id)
		{
			GameObject->ObjData.isAnimation = true;
			GameObject->m_player_data.ani = ani;
			GameObject->n_Animation = static_cast<int>(ani);
			break;
		}
	}
}

void Scene::Set_RemovePlayerData(const unsigned int & id, Player_Data & playerdata)
{
	for (auto GameObject : DynamicObject)
	{
		if (GameObject->m_player_data.id == id)
		{
			delete GameObject;
		}
	}
}

void Scene::SET_PLAYER_STATE_BY_SERVER_DATA(const Player_Data & playerdata)
{
	for (auto GameObject : DynamicObject)
	{
		GameObject->m_player_data.ai = playerdata.ai;
		GameObject->m_player_data.airbone = playerdata.airbone;
		GameObject->m_player_data.connect = playerdata.connect;
		GameObject->m_player_data.dir = playerdata.dir;
		GameObject->m_player_data.godmode = playerdata.godmode;
		GameObject->m_player_data.id = playerdata.id;
		GameObject->m_player_data.pos = playerdata.pos;
		GameObject->m_player_data.status = playerdata.status;

		GameObject->CenterPos = { playerdata.pos.x , playerdata.pos.y , playerdata.pos.z , playerdata.pos.w };
		GameObject->pp->SetPosition(&GameObject->CenterPos);

		GameObject->gamedata.Damage = move(playerdata.status.attack);
		GameObject->gamedata.HP = move(playerdata.status.cur_hp);
		GameObject->gamedata.MAXHP = move(playerdata.status.origin_hp);
		GameObject->gamedata.Speed = move(playerdata.status.speed);

		GameObject->AirBone = playerdata.airbone;
	}
}

void Scene::SET_PLAYER_SKILL(const unsigned int & id, const STC_SkillData & playerdata)
{
	for (auto GameObject : DynamicObject)
	{
		// 사용스킬에 대한 패킷을 보낸 ID == 현재 이 클라이언트의 아이디
		if (id == GameObject->m_player_data.id)
		{
			switch (playerdata.my_id)
			{
				// 실드를 따로 생성해서 Centerpos를 패킷으로 받은 아이디를 가진 플레이어값으로 지정한다 
				case CHAR_SKILL::SHIELD:
				{
					if (playerdata.alive)
					{
						NoCollObject.push_back(new ShieldArmor(device, commandlist, &BbObject, GameObject, GameObject->CenterPos));
					}
					else
					{
						for (auto object : NoCollObject)
						{
							if (id == object->m_player_data.id)
							{
								object->DelObj = true;
								break;
							}
						}
					}
				}
				break;

			default:
				break;
			}
			
		}
	}
}


void Scene::SET_PLAYER_SKILL(const unsigned int& id, const STC_SkillData& playerdata, unsigned char texture_number)
{
	for (auto GameObject : DynamicObject)
	{
		// 사용스킬에 대한 패킷을 보낸 ID == 현재 이 클라이언트의 아이디
		if (id == GameObject->m_player_data.id)
		{
			switch (playerdata.my_id)
			{
				case CHAR_SKILL::WAVE_SHOCK:
				{
					if (playerdata.alive)
					{
						StaticObject.push_back(new RingObject(device, commandlist, &BbObject, GameObject->CenterPos));
						auto iter = StaticObject.back();
						switch (texture_number)
						{
						case 0:
							iter->TextureName = "redTex";
							break;
						case 1:
							iter->TextureName = "orangeTex";
							break;
						case 2:
							iter->TextureName = "yellowTex";
							break;
						case 3:
							iter->TextureName = "pinkTex";
							break;
						case 4:
							iter->TextureName = "whiteTex";
							break;
						case 5:
							iter->TextureName = "blueTex";
							break;
						case 6:
							iter->TextureName = "greenTex";
							break;
						default:
							break;
						}
					}
					else
					{
						for (auto object : StaticObject)
						{
							if (id == object->m_player_data.id)
							{
								object->DelObj = true;
								break;
							}
						}
					}
				}
				break;

			}
		}
	}
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
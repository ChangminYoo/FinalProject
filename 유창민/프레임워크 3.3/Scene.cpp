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
	
	//��Ʈ�ñ״�ó�� ���̴����� �����Ѵ�.

	CreateRootSignature();
	nShader = 1;
	Shaders = new Shader;
	CreateShaderObject();
	Player = new CPlayer(hWnd,m_Device, m_DC, cw / ch, e, a, u);
	Shaders->player = Player;//���� �÷��̾ �����Ǿ� �Ѵ�.
	light = new CLight(m_Device, m_DC);
	CreateUI();

	Sound = new CSound();

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
	if (CharacterSelect != NULL)
		delete CharacterSelect;
	for (int i = 0; i < 4; i++)
		if (SkillFrameUI[i] != NULL)
			delete SkillFrameUI[i];
	for (int i = 0; i < 4; i++)
		if (SkillUI[i] != NULL)
			delete SkillUI[i];
	if (SelectBar != NULL)
		delete SelectBar;

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
	while(Shadows.size())
	{
		delete Shadows.front();
		Shadows.pop_front();
	}

	if (Sound != NULL)
	{
		Sound->ReleaseSound();
		delete Sound;
	}
	if (Player != NULL)
		delete Player;
	if (light != NULL)
		delete light;
}


void Scene::SceneState()
{
	if (GAMESTATE == GS_START)//���۽� �����ڿ��� UI�� �⺻���ΰ��� �ű⼭ �ε���.
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && GetFocus())
		{
			SetGameState(GS_LOAD);
		}

		else if (GetAsyncKeyState(0x31) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = -320;
			CharacterSelect->CenterPos.y = 0;
			CharacterSelect->ObjData.CustomData1.z = 1;
		}
		else if (GetAsyncKeyState(0x32) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = -160;
			CharacterSelect->CenterPos.y = 0;
			CharacterSelect->ObjData.CustomData1.z = 2;
		}
		else if (GetAsyncKeyState(0x33) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0;
			CharacterSelect->CenterPos.y = 0;
			CharacterSelect->ObjData.CustomData1.z = 3;
		}
		else if (GetAsyncKeyState(0x34) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 160;
			CharacterSelect->CenterPos.y = 0;
			CharacterSelect->ObjData.CustomData1.z = 4;
		}
		else if (GetAsyncKeyState(0x35) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 320;
			CharacterSelect->CenterPos.y = 0;
			CharacterSelect->ObjData.CustomData1.z = 5;
		}
		else if (GetAsyncKeyState(0x36) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = -320;
			CharacterSelect->CenterPos.y = -200;
			CharacterSelect->ObjData.CustomData1.z = 6;
		}
		else if (GetAsyncKeyState(0x37) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = -160;
			CharacterSelect->CenterPos.y = -200;
			CharacterSelect->ObjData.CustomData1.z = 7;
		}
		else if (GetAsyncKeyState(0x38) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0;
			CharacterSelect->CenterPos.y = -200;
			CharacterSelect->ObjData.CustomData1.z = 8;
		}
		else if (GetAsyncKeyState(0x39) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 160;
			CharacterSelect->CenterPos.y = -200;
			CharacterSelect->ObjData.CustomData1.z = 9;
		}
		else if (GetAsyncKeyState(0x30) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 320;
			CharacterSelect->CenterPos.y = -200;
			CharacterSelect->ObjData.CustomData1.z = 10;
		}
	}
	else if (GAMESTATE == GS_LOAD)
	{
		
		if (FirstLoad == true)
		{
			CreateGameObject();
			FirstLoad = false;
			SetGameState(GS_PLAY);
			ShowCursor(false);

			Sound->PlaySoundBG();
		}
		else
		{
			//���⿡�� �ε��� �ؽ�ó�� ������.
			BackGround->TextureName = "LoadBG";
			BackGround->TexOff = 1;//�ټ��� �ؽ�ó�̹Ƿ� TexOff���̿���.
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
	
	CD3DX12_DESCRIPTOR_RANGE NormaltexTable;
	NormaltexTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2, 0);

	//����� ���ҽ��� ���. ����� ť���� ���庯ȯ��� �ϳ��� ������.
	CD3DX12_ROOT_PARAMETER RootParameter[8];

	//�Ķ������ ������ 3������  ��ũ���;�����


	//�Ķ���͸�Ͽ� �ش� ����� ���ڷ� �����鼭 �ʱ�ȭ��
	RootParameter[0].InitAsConstantBufferView(0); //joint   //b0
	RootParameter[1].InitAsConstantBufferView(1); //obj     //b1
	RootParameter[2].InitAsConstantBufferView(2); //camera  //b2
	RootParameter[3].InitAsConstantBufferView(3); //light	//b3
	RootParameter[4].InitAsConstantBufferView(4); //material	//b4

	RootParameter[5].InitAsDescriptorTable(1, &SkytexTable, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParameter[6].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParameter[7].InitAsDescriptorTable(1, &NormaltexTable, D3D12_SHADER_VISIBILITY_PIXEL);

	auto staticSamplers = GetStaticSamplers();

	// ��Ʈ�ñ״�ó�� ��Ʈ�Ķ������ �迭�̴�.
	//��Ʈ�ñ״�ó�� ��������ü ����
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(8, RootParameter, (UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	// ��Ʈ�ñ״�ó�� ���� �޸� ����
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	//���ҽ����� ����ȭ�Ѵ�. ������� ���۰� 2������ b0,b1 �̷��� ������� ��.
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

	//--------------- �޽��� �ؽ�ó �ʱ�ȭ -------------//

	CGameObject* resource = NULL;
	resource = new CCubeManObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new BulletCube(device, commandlist,  &BbObject, &Shadows,NULL,XMFLOAT4(0,0,0,1),NULL, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new HeavyBulletCube(device, commandlist, &BbObject, &Shadows, NULL, XMFLOAT4(0, 0, 0, 1), NULL, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new SphereObject(device, commandlist,  &BbObject, &Shadows, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new Floor2Object(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new GridObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new RingObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new ShieldArmor(device, commandlist, &BbObject, &Shadows,NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new RangeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new Tetrike(device, commandlist, &BbObject, &Shadows, NULL, NULL, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new Tetris1(device, commandlist, &BbObject, &Shadows, NULL, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new Tetris2(device, commandlist, &BbObject, &Shadows, NULL, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new Tetris3(device, commandlist, &BbObject, &Shadows, NULL, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new DiceStrike(device, commandlist, &BbObject, &Shadows, NULL, XMFLOAT4(0, 0, 0, 1), 0, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new DiceObject(device, commandlist, &BbObject, &Shadows, NULL, XMFLOAT3(0, 0, 0), NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new BuildingObject(device, commandlist, &BbObject, &Shadows, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new ColumnObject(device, commandlist, &BbObject, &Shadows, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new BreakCartObject(device, commandlist, &BbObject, &Shadows, NULL, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new MeteorObject(device, commandlist, &BbObject, &Shadows, NULL, XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new HammerBullet(device, commandlist, &BbObject, &Shadows,&BulletObject,0, NULL, XMFLOAT4(0, 0, 0, 1));
	delete resource;
	//--------------------------------------------------//
	
	SkyObject = new SphereObject(device, commandlist,  &BbObject, &Shadows, XMFLOAT4(0, 0, 0, 0));
	LandObject.push_back(new GridObject(device, commandlist, &BbObject, &Shadows,1, XMFLOAT4(0, -0.5f, 0, 0))); //1��

	DynamicObject.push_back(new CCubeManObject(device, commandlist,&BbObject, &Shadows, XMFLOAT4(0, 0, -240, 0)));
	DynamicObject.push_back(new CCubeManObject(device, commandlist,&BbObject, &Shadows, XMFLOAT4(100, 0, 110, 0)));

	CGameObject* imp = new ImpObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(0, 0, 220, 0));
	((ImpObject*)imp)->fsm = new FSM(imp, &DynamicObject, &StaticObject, &BulletObject);
	DynamicObject.push_back(imp);

	//MoveCube
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, &Shadows, 50.0f, XMFLOAT4(0, 25, 145, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, &Shadows, 50.0f, XMFLOAT4(0, 52, -150, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, &Shadows, 50.0f, XMFLOAT4(145, 34, 0, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, &Shadows, 50.0f, XMFLOAT4(-145, 96, 0, 0)));


	//Cube
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(40, 18, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(68, 70, -93, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(35, 50, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(128, 38, 27, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(102, 69, 81, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(58, 39, 78, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(65, 35, -110, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(92, 61, 60, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(103, 26, 35, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(65, 17, -65, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(82, 11, 34, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(72, 44, 41, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-32, 39, 51, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 24, 34, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 38, -71, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 15, 12, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(13, 33, -42, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-76, 53, 8, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-40, 10, -15, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-38, 48, -126, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-41, 50, -94, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-45, 30, -60, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-65, 40, -102, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-69, 54, 61, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-85, 42, -45, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-106, 40, 42, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-104, 64, -62, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-87, 70, 82, 0)));
	

	//door
	//�»� ~�ݽð���� ~���
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-170, 5,  200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-170, 15, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-170, 25, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-170, 35, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-167, 35, 212, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-165, 35, 223, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-163, 35, 234, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-163, 25, 234, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-163, 15, 234, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-163, 5,  234, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-270, 5, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-270, 15, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-270, 25, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-270, 35, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-265, 35, -80, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-260, 35, -90, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-255, 35, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-255, 25, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-255, 15, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-255, 5, -100, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-20, 5, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-20, 15, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-20, 25, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-20, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-10, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(10, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(10, 25, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(10, 15, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(10, 5, -280, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(270, 5, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(270, 15, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(270, 25, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(270, 35, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(265, 35, -80, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(260, 35, -90, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(255, 35, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(255, 25, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(255, 15, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(255, 5, -100, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(170, 5, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(170, 15, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(170, 25, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(170, 35, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(165, 35, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(160, 35, 220, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(155, 35, 230, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(155, 25, 230, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(155, 15, 230, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(155, 5, 230, 0)));


	//Column
	StaticObject.push_back(new ColumnObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(110, 0, 0, 0)));
	StaticObject.push_back(new ColumnObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-110, 0, 0, 0)));
	StaticObject.push_back(new ColumnObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 0, -110, 0)));
	StaticObject.push_back(new ColumnObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 0, 110, 0)));

	StaticObject.push_back(new BreakCartObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-60, 0, -100, 0)));


	//BigWall
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-BigWall_X1, 0, BigWall_Z1, 0)));//�»�
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(BigWall_X1, 0, BigWall_Z1, 0)));//���
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(BigWall_X2, 0, BigWall_Z2, 0)));//����
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 0, BigWall_Z3, 0)));//��
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-BigWall_X2, 0, BigWall_Z2, 0)));//����

	//SmallWall
	//�»� 
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 200, 0, 480 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 140, 0, 400 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 80, 0, 320 * cosf(0.4f * MMPE_PI), 0)));

	//Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 20, 0, 240 * cosf(0.4f * MMPE_PI), 0)));


	//�߻�
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 2, XMFLOAT4(0, 0, 530, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 2, XMFLOAT4(0, 0, 460, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 2, XMFLOAT4(0, 0, 390, 0)));

	//Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 2, XMFLOAT4(0, 0, 320, 0)));

	//���
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 200, 0, 480 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 140, 0, 400 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 80, 0, 320 * cosf(0.4f * MMPE_PI), 0)));

	//Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 20, 0, 240 * cosf(0.4f * MMPE_PI), 0)));


	//����
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 3, XMFLOAT4(520 * sinf(0.8f * MMPE_PI), 0, -500 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 3, XMFLOAT4(470 * sinf(0.8f * MMPE_PI), 0, -440 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 3, XMFLOAT4(420 * sinf(0.8f * MMPE_PI), 0, -380 * cosf(0.2f * MMPE_PI), 0)));

	//Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 3, XMFLOAT4(370 * sinf(0.8f * MMPE_PI), 0, -320 * cosf(0.2f * MMPE_PI), 0)));
 

	//����
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 3, XMFLOAT4(-520 * sinf(0.8f * MMPE_PI), 0, -500 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 3, XMFLOAT4(-470 * sinf(0.8f * MMPE_PI), 0, -440 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 3, XMFLOAT4(-420 * sinf(0.8f * MMPE_PI), 0, -380 * cosf(0.2f * MMPE_PI), 0)));

	//Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 3, XMFLOAT4(-370 * sinf(0.8f * MMPE_PI), 0, -320 * cosf(0.2f * MMPE_PI), 0)));


	//RigidObject
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(0, 50, 290, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(0, 100, 290, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(-270, 50, 60, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(270, 100, 60, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(-210, 390, -200, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(200, 440, -180, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(180, 310, -30, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(-231, 50, 160, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(-231, 100, 160, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(-190, 370, -340, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(-70, 40, -155, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(-70, 90, -155, 0)));
	


	//�÷��̾��� ������Ʈ ����. �̰� ���߿� �ٲ����.
	Player->SetPlayer(DynamicObject.front());
	Player->PlayerObject->Blending = false;

	//============2��=============

	StaticObject.push_back(new Floor2Object(device, commandlist, &BbObject, &Shadows, 1, XMFLOAT4(0, 91, 0, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI *0.25, XMFLOAT4(30, 100, 90, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI *0.25, XMFLOAT4(90, 100, 30, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI *0.25, XMFLOAT4(-30, 100, 90, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI *0.25, XMFLOAT4(-90, 100, 30, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI *0.25, XMFLOAT4(30, 100, -90, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI *0.25, XMFLOAT4(90, 100, -30, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI *0.25, XMFLOAT4(-30, 100, -90, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI *0.25, XMFLOAT4(-90, 100, -30, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(30, 101, -40, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-40, 101, 30, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(30, 111, -40, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-40, 111, 30, 0)));
}

void Scene::CreateUI()
{

	AimUI = new AimObject(device,commandlist,NULL, NULL);
	
	BackGround = new BackGroundObject(device, commandlist, NULL, NULL, XMFLOAT4(0, 0,0,0));

	CharacterSelect = new CharacterSelectObject(device, commandlist, NULL, NULL, XMFLOAT4(0, 0, 0, 0));

	SelectBar = new SelectBarObject(device, commandlist, NULL, NULL, XMFLOAT4(0 * 100 - 150, 0.9*-mHeight / 2, 0, 0));

	
	for (int i = 0; i < 4; i++)
	{
		float ct = 0;
		switch (Player->skilldata.Skills[i])//��ų�ѹ���
		{
		case 0://����Ʈť��
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
			break;
		case 1://���ť��
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
			break;
		case 2://��Ʈ����ũ
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
			break;
		case 3://���̽�Ʈ����ũ
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
			break;

		case 4: // �ĵ���
			ct = ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];

		case 5: //�ǵ�
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];

		case 6://�ظӺҷ�
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
		case 7://�ظӺҷ�
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
		}


		SkillCoolBar[i] = new CoolBarObject(device, commandlist, NULL, NULL, ct, Player->PlayerObject, XMFLOAT4(i*100-150,0.98*-mHeight / 2, 0, 0));
		SkillFrameUI[i] = new SkillFrameUIObject(device, commandlist, NULL, NULL, Player->skilldata.Skills[i], XMFLOAT4(i * 100 - 150, 0.9*-mHeight / 2, 0, 0));
		SkillUI[i] = new SkillUIObject(device, commandlist, NULL, NULL, Player->skilldata.Skills[i], XMFLOAT4(i * 100 - 150, 0.9*-mHeight / 2, 0, 0));
	}

}

void Scene::UITick(const GameTimer & gt)
{
	if (Player->PlayerObject != NULL)
	{
		//��ų��Ÿ�� UI Ŀ���ҵ�����y���� ��Ÿ�ӿ� �°� ����
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
		�����Ϸ��� ��Ʈ�ñ״�ó/PSO/ī�޶�� ���÷� ���� �� ���� ���� ���ҽ� / �޽�/�������/�ؽ�ó �� �����ϸ� �ȴ�.
		��Ʈ�ñ״�ó�� ��� ���ҽ��� �����ϱ� ���� �ݵ�� �����Ұ�. ���� ù ���ۿ��� �̰� �����ϸ� ����.
	*/


	//���߿� ī�޶� ����Ʈ�� ������Ʈ�� �������ϰ� ���⿡�� �����ϵ�������

	//��Ʈ�ñ״�ó����. �̶� ��Ʈ�ñ״�ó�� ���̴� ������ ������� �����ؾ���.
	commandlist->SetGraphicsRootSignature(rootsg.Get());


	//���� ���̴��� �����ϰ� PSO ������ ���̴��� �����Լ�ȣ��. �״��� ���ӿ�����Ʈ�� �������(�ñ״�ó���� �ߴ���)�� �����ؾ���
	//���̺��� ��� ���� ���̺��� �����ؾ���.
	if (nShader > 0)
	{
			Shaders->SetShader(commandlist);
			//������� ����  ��������, �ε������� , ������ۺ丸 �����ϸ� �ȴ�.

			//���⿡ ī�޶����,������������� �߰��Ѵ�.

			Player->Camera.UpdateConstantBuffer(commandlist);
			light->UpdateConstantBuffer(commandlist);
			
			
			if (GetGameState() == GS_PLAY)
			{
				//���̴��� ������ �׷����� ������Ʈ ����� �׸���.
				Shaders->Render(commandlist, gt);

				//UI�� ���⼭ �׸���. UI�� ���� ����Ʈ���� ����.

				//���� ��Ʈ�ñ״�ó�� �״�� ����ص� �ǰ� ī�޶��� ��� UI�� ���������� ����ϹǷ� ���� ī�޶������ �ƴ�
				//���������� ī�޶� ����� �����Ѵ�. ���� ������� PSO�� �����Ѵ�. ��Ʈ/pso/�������ҽ� ���� �� �Ǿ����� ������İ� �޽��� �����Ѵ�.
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
				SkillUI[3]->Render(commandlist, gt);

				SelectBar->Render(commandlist, gt);
				for (int i = 0; i < 4; i++)
					SkillFrameUI[i]->Render(commandlist, gt);


			
				//�ٽ� �����·� �ٲ���. �̰� ���ϸ� ��ŷ�� �����̵�. 
				Player->Camera.UpdateConstantBuffer(commandlist);
			}
			else if (GetGameState() == GS_START || GetGameState()==GS_LOAD)
			{
				Player->Camera.UpdateConstantBufferOrtho(commandlist);
				Shaders->SetBillboardShader(commandlist);
				CharacterSelect->Render(commandlist, gt);
				BackGround->Render(commandlist, gt);
				Player->Camera.UpdateConstantBuffer(commandlist);
			}
	}
}

void Scene::Tick(const GameTimer & gt)
{
	//�÷��̾� �Է�ó�� 
	Player->PlayerInput(gt.DeltaTime(),this);

	if (GetGameState() == GS_PLAY)
	{
		//--------------------------------------------------------------
		//������Ʈ ����
		//--------------------------------------------------------------
		//�ִϸ��̼��ִ� ������Ʈ
		for (auto i = DynamicObject.begin(); i != DynamicObject.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//���� ���ӿ�����Ʈ�� �޸� ����
				i = DynamicObject.erase(i);//����Ʈ�󿡼� �ش� ��Ҹ� ����
			}
			else
				i++;
		}
		//����ü
		for (auto i = BulletObject.begin(); i != BulletObject.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//���� ���ӿ�����Ʈ�� �޸� ����
				i = BulletObject.erase(i);//����Ʈ�󿡼� �ش� ��Ҹ� ����

			}
			else
				i++;
		}
		//����ƽ ������Ʈ
		for (auto i = StaticObject.begin(); i != StaticObject.end();)
		{
			if ((*i)->DelObj == true)
			{
				delete *i;//���� ���ӿ�����Ʈ�� �޸� ����
				i = StaticObject.erase(i);//����Ʈ�󿡼� �ش� ��Ҹ� ����
			}
			else
				i++;
		}

		for (auto i = BbObject.begin(); i != BbObject.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//���� ���ӿ�����Ʈ�� �޸� ����
				i = BbObject.erase(i);//����Ʈ�󿡼� �ش� ��Ҹ� ����
			}
			else
				i++;
		}
		//������ٵ� ������Ʈ
		for (auto i = RigidObject.begin(); i != RigidObject.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//���� ���ӿ�����Ʈ�� �޸� ����
				i = RigidObject.erase(i);//����Ʈ�󿡼� �ش� ��Ҹ� ����
			}
			else
				i++;
		}
		for (auto i = NoCollObject.begin(); i != NoCollObject.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//���� ���ӿ�����Ʈ�� �޸� ����
				i = NoCollObject.erase(i);//����Ʈ�󿡼� �ش� ��Ҹ� ����
			}
			else
				i++;
		}

		for (auto i = Shadows.begin(); i != Shadows.end();)
		{

			if ((*i)->DelObj == true)
			{
				delete *i;//���� ���ӿ�����Ʈ�� �޸� ����
				i = Shadows.erase(i);//����Ʈ�󿡼� �ش� ��Ҹ� ����
			}
			else
				i++;
		}
		//--------------------------------------------------------------
		//������Ʈ ƽ�Լ� ó��
		//--------------------------------------------------------------
		for (auto b = DynamicObject.begin(); b != DynamicObject.end(); b++)
		{
			if ((*b)->isHit1)
			{
				Sound->PlaySoundEffect(CSound::SoundType::HIT1);
				(*b)->isHit1 = false;
			}

			(*b)->Tick(gt);
		}

		for (auto b = StaticObject.begin(); b != StaticObject.end(); b++)
			(*b)->Tick(gt);

		//�ҷ�
		for (auto b = BulletObject.begin(); b != BulletObject.end(); b++)
			(*b)->Tick(gt);


		for (auto b = BbObject.begin(); b != BbObject.end(); b++)
			(*b)->Tick(gt);

		//������ �ٵ�
		for (auto b = RigidObject.begin(); b != RigidObject.end(); b++)
			(*b)->Tick(gt);
		
		
		for (auto b = NoCollObject.begin(); b != NoCollObject.end(); b++)
			(*b)->Tick(gt);

		for (auto b = Shadows.begin(); b != Shadows.end(); b++)
			(*b)->Tick(gt);

		//DynamicObject�� 1�̸� ���� ���� ���·� �����!!
		if (DynamicObject.size() == 1)
			GAMESTATE = GS_END;

		Player->Tick(gt.DeltaTime());

		

		UITick(gt);
		//ī�޶� �� ������Ʈ 
		Player->PlayerCameraReLocate();
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
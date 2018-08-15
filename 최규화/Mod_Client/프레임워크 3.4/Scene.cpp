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
	//while(CharacterSelect.size())
	//{
	//	delete CharacterSelect.back();
	//	CharacterSelect.pop_back();
	//}

	if (Title != NULL)
		delete Title;
	for (int i = 0; i < 4; i++)
		if (SkillFrameUI[i] != NULL)
			delete SkillFrameUI[i];
	for (int i = 0; i < 4; i++)
		if (SkillUI[i] != NULL)
			delete SkillUI[i];
	if (SelectBar != NULL)
		delete SelectBar;
	if (Time1 != NULL)
		delete Time1;
	if (Time2 != NULL)
		delete Time2;
	if (Time3 != NULL)
		delete Time3;

	if (MyPoint1 != NULL)
		delete MyPoint1;
	if (MyPoint2 != NULL)
		delete MyPoint2;
	if (MyPoint3 != NULL)
		delete MyPoint3;

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
	if (GAMESTATE == GS_TITLE)
	{
		if (Title->TexStride >= 4.0f)
		{
			SetGameState(GS_START);
		}

	}

	if (GAMESTATE == GS_START)//시작시 생성자에서 UI등 기본적인것은 거기서 로드함.
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && GetFocus())
		{
			//SetGameState(GS_LOAD);
			Player->m_async_client->m_myClientReady = true;
			Player->m_async_client->m_myTextureID = CharacterSelect->ObjData.CustomData1.z - 1;
			Player->m_async_client->m_selCharacterComplete = true;
			//return;

		}
		else if (GetAsyncKeyState(0x31) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0.8f * (-mWidth * 0.5f);
			CharacterSelect->CenterPos.y = 0;
			CharacterSelect->ObjData.CustomData1.z = 1;

		}
		else if (GetAsyncKeyState(0x32) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0.4f * (-mWidth * 0.5f);
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
			CharacterSelect->CenterPos.x = 0.4f * (mWidth* 0.5f);
			CharacterSelect->CenterPos.y = 0;
			CharacterSelect->ObjData.CustomData1.z = 4;

		}
		else if (GetAsyncKeyState(0x35) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0.8f * (mWidth* 0.5f);
			CharacterSelect->CenterPos.y = 0;
			CharacterSelect->ObjData.CustomData1.z = 5;


		}
		else if (GetAsyncKeyState(0x36) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0.8f * (-mWidth * 0.5f);
			CharacterSelect->CenterPos.y = (-mHeight * 1 / 3);
			CharacterSelect->ObjData.CustomData1.z = 6;

		}
		else if (GetAsyncKeyState(0x37) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0.4f * (-mWidth * 0.5f);
			CharacterSelect->CenterPos.y = (-mHeight * 1 / 3);
			CharacterSelect->ObjData.CustomData1.z = 7;
		}
		else if (GetAsyncKeyState(0x38) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0;
			CharacterSelect->CenterPos.y = (-mHeight * 1 / 3);
			CharacterSelect->ObjData.CustomData1.z = 8;
		}
		else if (GetAsyncKeyState(0x39) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0.4f * (mWidth* 0.5f);
			CharacterSelect->CenterPos.y = (-mHeight * 1 / 3);
			CharacterSelect->ObjData.CustomData1.z = 9;
		}
		else if (GetAsyncKeyState(0x30) & 0x8000 && GetFocus())
		{
			CharacterSelect->CenterPos.x = 0.8f * (mWidth* 0.5f);
			CharacterSelect->CenterPos.y = (-mHeight * 1 / 3);
			CharacterSelect->ObjData.CustomData1.z = 10;
		}

		Player->m_async_client->m_myTextureID = CharacterSelect->ObjData.CustomData1.z - 1;
	
		//Player->m_async_client->m_lobbyData는 구조체 vector
		//Player->m_async_client->m_lobbyData에 id 랑 texture_id(	CharacterSelect->ObjData.CustomData1.z (0 ~ 9) 값이 들어있음)
		
		/*if (!Player->m_async_client->m_lobbyData.empty())
		{
			for (auto& data : Player->m_async_client->m_lobbyData)
			{
				for (auto& select : CharacterSelect)
				{
					if (data.my_id == select->TexStride)
					{
						switch (data.textureID + 1)
						{
						case 1:
							select->CenterPos.x = 0.8f * (-mWidth * 0.5f);
							select->CenterPos.y = 0;
							break;
						}
					}
				}
			}
		}*/
		

	}
	else if (GAMESTATE == GS_LOAD)
	{
		if (FirstLoad == true)
		{
			CreateGameObject();
			/*
			for (auto& GameObject : DynamicObject)
			{
				if (GameObject->isNPC) continue;
				for (const auto& client : Player->m_async_client->m_clientsID)
				{
					if (client.my_id == GameObject->m_player_data.id)
					{
						GameObject->TexOff = client.textureID;
						break;
					}					
				}			
			}
			*/
			
			auto local_count = 0;
			for (auto& GameObject : DynamicObject)
			{
				if (GameObject->isNPC) continue;
				GameObject->m_player_data.id = Player->m_async_client->m_clientsID[local_count].my_id;
				GameObject->TexOff = Player->m_async_client->m_clientsID[local_count].textureID;

				++local_count;

				if (local_count == Player->m_async_client->m_clientsID.size())
					break;
			}
			

			//서버 추가
			STC_CHANGE_SCENE stc_change_scene;
			stc_change_scene.state.my_currScene = GS_LOAD;
			stc_change_scene.state.my_currSceneReady = true;
			stc_change_scene.state.my_id = Player->PlayerObject->m_player_data.id;

			Player->m_async_client->SendPacket(reinterpret_cast<Packet*>(&stc_change_scene));

			FirstLoad = false;
			SetGameState(GS_PLAY);
			ShowCursor(false);

			Sound->PlaySoundBG();
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
	
	CD3DX12_DESCRIPTOR_RANGE NormaltexTable;
	NormaltexTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2, 0);

	//연결될 리소스들 목록. 현재는 큐브의 월드변환행렬 하나만 넣을것.
	CD3DX12_ROOT_PARAMETER RootParameter[8];

	//파라메터의 종류는 3가진데  디스크립터쓸거임


	//파라메터목록에 해당 목록을 인자로 넣으면서 초기화함
	RootParameter[0].InitAsConstantBufferView(0); //joint   //b0
	RootParameter[1].InitAsConstantBufferView(1); //obj     //b1
	RootParameter[2].InitAsConstantBufferView(2); //camera  //b2
	RootParameter[3].InitAsConstantBufferView(3); //light	//b3
	RootParameter[4].InitAsConstantBufferView(4); //material	//b4

	RootParameter[5].InitAsDescriptorTable(1, &SkytexTable, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParameter[6].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParameter[7].InitAsDescriptorTable(1, &NormaltexTable, D3D12_SHADER_VISIBILITY_PIXEL);

	auto staticSamplers = GetStaticSamplers();

	// 루트시그니처는 루트파라메터의 배열이다.
	//루트시그니처의 정보구조체 생성
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(8, RootParameter, (UINT)staticSamplers.size(), staticSamplers.data(),
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
	resource = new RingObject(device, commandlist, &BbObject, &Shadows, NULL,  XMFLOAT4(0, 0, 0, 0));
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
	LandObject.push_back(new GridObject(device, commandlist, &BbObject, &Shadows,1, XMFLOAT4(0, -0.5f, 0, 0))); //1층

	DynamicObject.push_back(new CCubeManObject(device, commandlist,&BbObject, &Shadows, XMFLOAT4(0, 0, -240, 0)));
	DynamicObject.push_back(new CCubeManObject(device, commandlist,&BbObject, &Shadows, XMFLOAT4(100, 0, 110, 0)));

	CGameObject* imp = new ImpObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(0, 600, 0, 0));
	((ImpObject*)imp)->fsm = new FSM(imp, &DynamicObject, &StaticObject, &BulletObject);
	DynamicObject.push_back(imp);

	//============2층=============
	StaticObject.push_back(new Floor2Object(device, commandlist, &BbObject, &Shadows, 1, XMFLOAT4(0, 91, 0, 0)));						//0
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI *0.25, XMFLOAT4(30, 100, 90, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI *0.25, XMFLOAT4(90, 100, 30, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI *0.25, XMFLOAT4(-30, 100, 90, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI *0.25, XMFLOAT4(-90, 100, 30, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI *0.25, XMFLOAT4(30, 100, -90, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI *0.25, XMFLOAT4(90, 100, -30, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI *0.25, XMFLOAT4(-30, 100, -90, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI *0.25, XMFLOAT4(-90, 100, -30, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(50, 101, 50, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(50, 101, -50, 0)));					   //10
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-50, 111, 50, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-50, 111, -50, 0)));

	//============1층=============
	//MoveCube
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, &Shadows, 50.0f, XMFLOAT4(0, 25, 145, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, &Shadows, 50.0f, XMFLOAT4(0, 52, -150, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, &Shadows, 50.0f, XMFLOAT4(145, 34, 0, 0)));
	StaticObject.push_back(new MoveCubeObject(device, commandlist, &BbObject, &Shadows, 50.0f, XMFLOAT4(-145, 96, 0, 0)));


	//Cube
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(40, 18, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(68, 70, -93, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(35, 50, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(128, 38, 27, 0)));					    //20
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(102, 69, 81, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(58, 39, 78, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(65, 35, -110, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(92, 61, 60, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(103, 26, 35, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(65, 17, -65, 0))); //10
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(82, 11, 34, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(72, 44, 41, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-32, 39, 51, 0)));

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 24, 34, 0)));						//30
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 38, -71, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 15, 12, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(13, 33, -42, 0)));  //17

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-76, 53, 8, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-40, 10, -15, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-38, 48, -126, 0))); //20
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-41, 50, -94, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-45, 30, -60, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-65, 40, -102, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-69, 54, 61, 0)));						//40
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-85, 42, -45, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-106, 40, 42, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-104, 64, -62, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-87, 70, 82, 0))); //28					//44
	

	//door
	//좌상 ~반시계방향 ~우상
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-170, 5,  200, 0))); //29
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-170, 15, 200, 0))); //30
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-170, 25, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-170, 35, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-167, 35, 212, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-165, 35, 223, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-163, 35, 234, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-163, 25, 234, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-163, 15, 234, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-163, 5,  234, 0)));			//54

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-270, 5, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-270, 15, -70, 0)));  //40
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-270, 25, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-270, 35, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-265, 35, -80, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-260, 35, -90, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-255, 35, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-255, 25, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-255, 15, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-255, 5, -100, 0)));  //48		//64

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-20, 5, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-20, 15, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-20, 25, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-20, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-10, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(10, 35, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(10, 25, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(10, 15, -280, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(10, 5, -280, 0)));					 //58		//74

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(270, 5, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(270, 15, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(270, 25, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(270, 35, -70, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(265, 35, -80, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(260, 35, -90, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(255, 35, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(255, 25, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(255, 15, -100, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(255, 5, -100, 0)));  //68		//84

	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(170, 5, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(170, 15, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(170, 25, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(170, 35, 200, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(165, 35, 210, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(160, 35, 220, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(155, 35, 230, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(155, 25, 230, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(155, 15, 230, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(155, 5, 230, 0)));		  //78	     //94


	//Column
	StaticObject.push_back(new ColumnObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(110, 0, 0, 0)));					  //79
	StaticObject.push_back(new ColumnObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(-110, 0, 0, 0)));
	StaticObject.push_back(new ColumnObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 0, -110, 0)));
	StaticObject.push_back(new ColumnObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 0, 110, 0)));					  //82

	//BigWall
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1, XMFLOAT4(-BigWall_X1, 0, BigWall_Z1, 0)));//좌상							  //100
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1, XMFLOAT4(BigWall_X1, 0, BigWall_Z1, 0)));//우상
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, -BigWall_Rad1 * 2, XMFLOAT4(BigWall_X2, 0, BigWall_Z2, 0)));//우하
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, 0, XMFLOAT4(0, 0, BigWall_Z3, 0)));//하													  
	StaticObject.push_back(new BigWallObject(device, commandlist, &BbObject, &Shadows, BigWall_Rad1 * 2, XMFLOAT4(-BigWall_X2, 0, BigWall_Z2, 0)));//좌하						  //104			  

	//SmallWall
	//좌상 
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 200, 0, 480 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 140, 0, 400 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 80, 0, 320 * cosf(0.4f * MMPE_PI), 0)));
	//중상
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 2, XMFLOAT4(0, 0, 530, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 2, XMFLOAT4(0, 0, 460, 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 2, XMFLOAT4(0, 0, 390, 0)));
	//우상
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 200, 0, 480 * cosf(0.4f * MMPE_PI), 0)));				
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 140, 0, 400 * cosf(0.4f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 80, 0, 320 * cosf(0.4f * MMPE_PI), 0)));
	//우하
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 3, XMFLOAT4(520 * sinf(0.8f * MMPE_PI), 0, -500 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 3, XMFLOAT4(470 * sinf(0.8f * MMPE_PI), 0, -440 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 3, XMFLOAT4(420 * sinf(0.8f * MMPE_PI), 0, -380 * cosf(0.2f * MMPE_PI), 0)));
	//좌하
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 3, XMFLOAT4(-520 * sinf(0.8f * MMPE_PI), 0, -500 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 3, XMFLOAT4(-470 * sinf(0.8f * MMPE_PI), 0, -440 * cosf(0.2f * MMPE_PI), 0)));
	StaticObject.push_back(new SmallWallObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 3, XMFLOAT4(-420 * sinf(0.8f * MMPE_PI), 0, -380 * cosf(0.2f * MMPE_PI), 0))); 

																																													//좌상 Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 8, XMFLOAT4(-BigWall_X1 - 20, 0, 240 * cosf(0.4f * MMPE_PI), 0)));
	//중상 Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 2, XMFLOAT4(0, 0, 320, 0)));
	//우상 Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 8, XMFLOAT4(BigWall_X1 + 20, 0, 240 * cosf(0.4f * MMPE_PI), 0)));
	//우하 Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, MMPE_PI / 3, XMFLOAT4(370 * sinf(0.8f * MMPE_PI), 0, -320 * cosf(0.2f * MMPE_PI), 0)));
	//좌하 Building
	StaticObject.push_back(new BuildingObject(device, commandlist, &BbObject, &Shadows, -MMPE_PI / 3, XMFLOAT4(-370 * sinf(0.8f * MMPE_PI), 0, -320 * cosf(0.2f * MMPE_PI), 0)));	//131	//124번

	//RigidObject
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, &Shadows, XMFLOAT4(0, 200, 290, 0)));
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
	

	/*
	int player_num = 0;
	for (auto obj : DynamicObject)
	{
		if (obj->isNPC) continue;

		obj->m_player_data.id = player_num;
		++player_num;
	}
	*/

	int player_num = 0;
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
	
	
	//Peasant:  light, shield, padong, dice
	//Peggy : heavy, padong, shield, hammer
	//Ms.White : light, heavy, shield, dice
	//Purple : light, padong, dice, hammer
	//Lara : heavy, padong, shield, hammer
	//Jack : light, padong, shield, hammer
	//Miner : light, shield, dice, hammer
	//Robin : light, heavy, padong, dice
	//Blazer : heavy, shield, dice, hammer
	//Ronney : light, heavy, shield, hammer

	if (CharacterSelect->ObjData.CustomData1.z == 1)
	{
		Player->PlayerObject->TextureName = "Female Brown Casual";
		Player->PlayerObject->NTextureName = "Female Brown Casual N";
		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 0;
		Player->skilldata.Skills[1] = 5;
		Player->skilldata.Skills[2] = 4;
		Player->skilldata.Skills[3] = 3;

	}
	else if (CharacterSelect->ObjData.CustomData1.z == 2)
	{
		Player->PlayerObject->TextureName = "Female Black Knight";
		Player->PlayerObject->NTextureName = "Female Black Knight N";
		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 1;
		Player->skilldata.Skills[1] = 4;
		Player->skilldata.Skills[2] = 5;
		Player->skilldata.Skills[3] = 6;
	}
	else if (CharacterSelect->ObjData.CustomData1.z == 3)
	{
		Player->PlayerObject->TextureName = "Female Brown Sorceress";
		Player->PlayerObject->NTextureName = "Female Brown Sorceress N";
		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 0;
		Player->skilldata.Skills[1] = 1;
		Player->skilldata.Skills[2] = 5;
		Player->skilldata.Skills[3] = 3;

	}
	else if (CharacterSelect->ObjData.CustomData1.z == 4)
	{
		Player->PlayerObject->TextureName = "Female White Knight";
		Player->PlayerObject->NTextureName = "Female White Knight N";
		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 0;
		Player->skilldata.Skills[1] = 4;
		Player->skilldata.Skills[2] = 3;
		Player->skilldata.Skills[3] = 6;
	}
	else if (CharacterSelect->ObjData.CustomData1.z == 5)
	{
		Player->PlayerObject->TextureName = "Female White Barbarian";
		Player->PlayerObject->NTextureName = "Female White Barbarian N";

		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 1;
		Player->skilldata.Skills[1] = 4;
		Player->skilldata.Skills[2] = 5;
		Player->skilldata.Skills[3] = 6;
	}
	else if (CharacterSelect->ObjData.CustomData1.z == 6)
	{

		Player->PlayerObject->TextureName = "Male Black Knight";
		Player->PlayerObject->NTextureName = "Male Black Knight N";

		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 0;
		Player->skilldata.Skills[1] = 4;
		Player->skilldata.Skills[2] = 5;
		Player->skilldata.Skills[3] = 6;
	}
	else if (CharacterSelect->ObjData.CustomData1.z == 7)
	{
		Player->PlayerObject->TextureName = "Male White Wizard";
		Player->PlayerObject->NTextureName = "Male White Wizard N";

		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 0;
		Player->skilldata.Skills[1] = 5;
		Player->skilldata.Skills[2] = 3;
		Player->skilldata.Skills[3] = 6;

	}
	else if (CharacterSelect->ObjData.CustomData1.z == 8)
	{
		Player->PlayerObject->TextureName = "Male Black Archer";
		Player->PlayerObject->NTextureName = "Male Black Archer N";
		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 0;
		Player->skilldata.Skills[1] = 1;
		Player->skilldata.Skills[2] = 4;
		Player->skilldata.Skills[3] = 3;
	}
	else if (CharacterSelect->ObjData.CustomData1.z == 9)
	{
		Player->PlayerObject->TextureName = "Male Fire";
		Player->PlayerObject->NTextureName = "Male Fire N";
		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 1;
		Player->skilldata.Skills[1] = 5;
		Player->skilldata.Skills[2] = 3;
		Player->skilldata.Skills[3] = 6;
	}
	else if (CharacterSelect->ObjData.CustomData1.z == 10)
	{
		Player->PlayerObject->TextureName = "Male White King";
		Player->PlayerObject->NTextureName = "Male White King N";
		Player->PlayerObject->TexOff = CharacterSelect->ObjData.CustomData1.z - 1;
		Player->PlayerObject->NTexOff = Player->PlayerObject->TexOff + 10;

		Player->skilldata.Skills[0] = 0;
		Player->skilldata.Skills[1] = 1;
		Player->skilldata.Skills[2] = 5;
		Player->skilldata.Skills[3] = 6;
	}

	for (int i = 0; i < 4; i++)
	{
		SkillUI[i]->TexOff = Player->skilldata.Skills[i];
		((CoolBarObject*)SkillCoolBar[i])->MaxCoolTime = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
	}
}

void Scene::CreateUI()
{

	AimUI = new AimObject(device,commandlist,NULL, NULL);
	
	BackGround = new BackGroundObject(device, commandlist, NULL, NULL, XMFLOAT4(0, 0,0,0));

	CharacterSelect = new CharacterSelectObject(device, commandlist, NULL, NULL, XMFLOAT4(0, 0, 0, 0));

	SelectBar = new SelectBarObject(device, commandlist, NULL, NULL, XMFLOAT4(0 * 100 - 150, 0.9*-mHeight / 2, 0, 0));
	
	Title = new TitleObject(device, commandlist, NULL, NULL, XMFLOAT4(0, 0, 0, 0));

	
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
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
			break;
		case 5: //실드
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];

		case 6://해머불렛
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
		case 7://해머불렛
			ct = Player->skilldata.SkillsMaxCoolTime[Player->skilldata.Skills[i]];
		}


		SkillCoolBar[i] = new CoolBarObject(device, commandlist, NULL, NULL, ct, Player->PlayerObject, XMFLOAT4(i*100-150,0.98*-mHeight / 2, 0, 0));
		SkillFrameUI[i] = new SkillFrameUIObject(device, commandlist, NULL, NULL, Player->skilldata.Skills[i], XMFLOAT4(i * 100 - 150, 0.9*-mHeight / 2, 0, 0));
		SkillUI[i] = new SkillUIObject(device, commandlist, NULL, NULL, Player->skilldata.Skills[i], XMFLOAT4(i * 100 - 150, 0.9*-mHeight / 2, 0, 0));
		Time1 = new TimerObject1(device, commandlist, NULL, NULL, XMFLOAT4(-30, 0.90f*mHeight / 2, 0, 0));
		Time2 = new TimerObject2(device, commandlist, NULL, NULL, XMFLOAT4(10, 0.90f*mHeight / 2, 0, 0));
		Time3 = new TimerObject3(device, commandlist, NULL, NULL, XMFLOAT4(50, 0.90f*mHeight / 2, 0, 0));

		MyPoint1 = new PointObject1(device, commandlist, NULL, NULL, XMFLOAT4(-25 - (mWidth / 2)*0.625, -0.9f*mHeight / 2, 0, 0));
		MyPoint1->ObjData.Scale = mWidth / 30;
		MyPoint1->ObjData.CustomData1.y = mHeight / 10;
		MyPoint2 = new PointObject1(device, commandlist, NULL, NULL, XMFLOAT4(-(mWidth / 2)*0.625, -0.9f*mHeight / 2, 0, 0));
		MyPoint2->ObjData.Scale = mWidth / 30;
		MyPoint2->ObjData.CustomData1.y = mHeight / 10;

		MyPoint3 = new PointObject1(device, commandlist, NULL, NULL, XMFLOAT4(25 - (mWidth / 2)*0.625, -0.9f*mHeight / 2, 0, 0));
		MyPoint3->ObjData.Scale = mWidth / 30;
		MyPoint3->ObjData.CustomData1.y = mHeight / 10;

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
			CharacterSelect->ObjData.Scale = mWidth * 0.2f;
			resize = false;
		}

	}

	if (GetGameState() == GS_PLAY)
	{
		float crtime = static_cast<float>(Player->m_async_client->m_myClient_StageTime);

		((TimerObject3*)Time3)->TexStride = (static_cast<int>(crtime) % 100) % 10;
		((TimerObject2*)Time2)->TexStride = (static_cast<int>(crtime) % 100) / 10;
		((TimerObject1*)Time1)->TexStride = (static_cast<int>(crtime) / 100);

		/*
		if (((TimerObject3*)Time3)->times >= 1)
		{
			float t = 1 - ((TimerObject3*)Time3)->times;
			Time3->TexStride += 1;
			((TimerObject3*)Time3)->times = t;
		}
		if (Time3->TexStride > 9.9f)
		{
			Time3->TexStride = 0;
			Time2->TexStride += 1;
		}
		if (Time2->TexStride > 9.9f)
		{
			Time2->TexStride = 0;
			Time1->TexStride += 1;
		}
		*/



		//포인트 증가 
		MyPoint2->TexStride = (Player->PlayerObject->pointrank.Point % 100) / 10;
		MyPoint1->TexStride = (Player->PlayerObject->pointrank.Point / 100);
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
				SkillUI[3]->Render(commandlist, gt);

				SelectBar->Render(commandlist, gt);
				for (int i = 0; i < 4; i++)
					SkillFrameUI[i]->Render(commandlist, gt);


				Time1->Render(commandlist, gt);
				Time2->Render(commandlist, gt);
				Time3->Render(commandlist, gt);
			
				MyPoint1->Render(commandlist, gt);
				MyPoint2->Render(commandlist, gt);
				MyPoint3->Render(commandlist, gt);
				//다시 원상태로 바꿔줌. 이걸 안하면 피킹이 엉망이됨. 
				Player->Camera.UpdateConstantBuffer(commandlist);
			}
			else if (GetGameState() == GS_TITLE || GetGameState() == GS_START || GetGameState() == GS_LOAD)
			{
				Player->Camera.UpdateConstantBufferOrtho(commandlist);
				Shaders->SetBillboardShader(commandlist);

				if (GetGameState() == GS_START || GetGameState() == GS_LOAD)
				{
					if (GetGameState() == GS_START)
						CharacterSelect->Render(commandlist, gt);

					BackGround->Render(commandlist, gt);
				}
				if (GetGameState() == GS_TITLE)
					Title->Render(commandlist, gt);

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
				if ((*i)->isHit1)
				{
					Sound->PlaySoundEffect(CSound::SoundType::HIT1);
				}

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
		{
		

			(*b)->Tick(gt);

			if ((*b)->m_end_npc_attack)
			{
				STC_MyNPCAnim cts_npcanim;
				cts_npcanim.npc_anim = Ani_State::Idle;
				cts_npcanim.id = (*b)->m_npc_data.id;

				Player->m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_npcanim));
				(*b)->m_end_npc_attack = false;

			}

			/*
			if ((*b)->m_end_npc_die)
			{
				STC_MyNPCAnim cts_npcanim;
				cts_npcanim.npc_anim = Ani_State::Dead;
				cts_npcanim.id = (*b)->m_npc_data.id;

				Player->m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_npcanim));
				(*b)->m_end_npc_die = false;
			}
			*/

		}

		for (auto b = StaticObject.begin(); b != StaticObject.end(); b++)
			(*b)->Tick(gt);

		//불렛
		for (auto b = BulletObject.begin(); b != BulletObject.end(); b++)
		{
			(*b)->Tick(gt);
		}


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

void Scene::SET_STAGET_TIMER_BY_SERVER_DATA(const STC_StageTimer & timer)
{
	Player->m_async_client->m_myClient_StageTime = timer.stage_time;
}

void Scene::SET_SELECTED_CHARACTER_LOADSCENE_BY_SERVER_DATA(const unsigned short& id, const STC_ShowSelectCharacter & data)
{
	int find_cnt = 0;
	Player->m_lobbySceneMyID = id;

	if (Player->m_async_client->m_lobbyData.empty())
	{
		Player->m_async_client->m_lobbyData.push_back({ id, data.sel_id });
	}
	else
	{
		for (auto& lobby : Player->m_async_client->m_lobbyData)
		{
			if (lobby.my_id == id)
			{
				lobby.textureID = data.sel_id;
				++find_cnt;
			}

			//cout << "ID: " << lobby.my_id << " , " << "TextureID: " << lobby.textureID << "\n";
		}
	}

	if (find_cnt == 0)
	{
		Player->m_async_client->m_lobbyData.push_back({ id, data.sel_id });
	}

	//cout << "\n";
	//cout << "MyID: " << static_cast<int>(Player->m_lobbySceneMyID) << "\n";
}

void Scene::SET_DRAW_STATE_BY_DEATH_BY_SERVER_DATA(const unsigned short& id, const STC_DrawState & data)
{
	for (auto& GameObject : DynamicObject)
	{
		if (GameObject->isNPC) continue;

		if (id == GameObject->m_player_data.id)
		{
			GameObject->DrawObj = data.drawobj;
			GameObject->currAnimTime = 0.f;
			GameObject->n_Animation = Ani_State::Idle;

			dynamic_cast<CCubeManObject*>(GameObject)->Hpbar->DrawObj = true;
			dynamic_cast<CCubeManObject*>(GameObject)->HPFrame->DrawObj = true;

			if (!data.isTopRanker)
			{
				GameObject->pointrank.TopMode = false;
				GameObject->ObjData.Scale = 3;

				XMFLOAT3 rx(3, 0, 0);
				XMFLOAT3 ry(0, 10, 0);
				XMFLOAT3 rz(0, 0, 3);
				GameObject->rco.SetPlane(rx, ry, rz);

				dynamic_cast<CCubeManObject*>(GameObject)->s->ObjData.Scale = 2.0f;
				dynamic_cast<CCubeManObject*>(GameObject)->Hpbar->YPos = 10;
				dynamic_cast<CCubeManObject*>(GameObject)->HPFrame->YPos = 10;

			}

			//cout << "ID: " << static_cast<int>(id) << "DrawObject: " << GameObject->DrawObj << "\n";
			break;
		}

	}
}

void Scene::SET_PLAYER_BY_SEVER_DATA(const unsigned short & id, const Player_Data & playerdata, const unsigned char & packet_type)
{
	for (auto& GameObject : DynamicObject)
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

					//플레이어 스코어 , 킬 , 데스 , 포인트 관리
					GameObject->pointrank.DeathCount = playerdata.deathcount;
					GameObject->pointrank.KillCount = playerdata.killcount;
					GameObject->pointrank.Point = playerdata.score;
					GameObject->pointrank.Rank = playerdata.rank;

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

					//플레이어 스코어 , 킬 , 데스 , 포인트 관리
					GameObject->pointrank.DeathCount = playerdata.deathcount;
					GameObject->pointrank.KillCount = playerdata.killcount;
					GameObject->pointrank.Point = playerdata.score;
					GameObject->pointrank.Rank = playerdata.rank;
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
					GameObject->m_player_data.alive = playerdata.alive;

					GameObject->CenterPos = { playerdata.pos.x , playerdata.pos.y , playerdata.pos.z , playerdata.pos.w };
					GameObject->pp->SetPosition(&GameObject->CenterPos);

					GameObject->gamedata.Damage = playerdata.status.attack;
					GameObject->gamedata.HP = playerdata.status.cur_hp;
					GameObject->gamedata.MAXHP = playerdata.status.origin_hp;
					GameObject->gamedata.Speed = playerdata.status.speed;

					GameObject->AirBone = playerdata.airbone;

					//플레이어 스코어 , 킬 , 데스 , 포인트 관리
					GameObject->pointrank.DeathCount = playerdata.deathcount;
					GameObject->pointrank.KillCount = playerdata.killcount;
					GameObject->pointrank.Point = playerdata.score;
					GameObject->pointrank.Rank = playerdata.rank;

					if (playerdata.topRank)
					{
						GameObject->pointrank.TopMode = true;
						GameObject->ObjData.Scale = 4;
						XMFLOAT3 rx(4, 0, 0);
						XMFLOAT3 ry(0, 13, 0);
						XMFLOAT3 rz(0, 0, 4);
						GameObject->rco.SetPlane(rx, ry, rz);

						dynamic_cast<CCubeManObject*>(GameObject)->s->ObjData.Scale = 3.0f;
						dynamic_cast<CCubeManObject*>(GameObject)->Hpbar->YPos = 16;
						dynamic_cast<CCubeManObject*>(GameObject)->HPFrame->YPos = 16;

						//GameObject->gamedata.MAXHP = 700;
						//GameObject->gamedata.Speed = 75;
						//GameObject->gamedata.HP = GameObject->gamedata.HP + 600;

					}

					//캐릭터가 죽었을 때 단 한번만 실행이되어야한다
					if (playerdata.alive == false && playerdata.respawn_cnt == 1)
					{
						GameObject->currAnimTime = 0.f;
						GameObject->m_player_data.ani = playerdata.ani;
						GameObject->n_Animation = static_cast<int>(playerdata.ani);
					}
					
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

			break;
		}
	}

	//m_score_for_rankcheck = static_cast<float>(m_player_score) + static_cast<float>(m_killCount) * 3.8f - static_cast<float>(m_deathCount) * 1.8f;

	

	//	100번째 방 누가 바뀌었나 for (auto i = 0; v[100].size(); ++i) { ~ }  //이
}

void Scene::SET_NPC_BY_SERVER_DATA(const unsigned short & id, const Npc_Data & data, const unsigned char & monster_type, const unsigned char & packet_type)
{
	for (auto GameObject : DynamicObject)
	{
		if (!GameObject->isNPC) continue;

		if (GameObject->m_npc_data.id == id)	//dynamic_cast<ImpObject*>(GameObject)->myNPCID == id
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

				GameObject->n_Animation = static_cast<int>(data.ani);
			}
			break;

			case PACKET_PROTOCOL_TYPE::NPC_MONSTER_CURR_STATE:
			{
				if (!data.alive)
					GameObject->DelObj = true;
				

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

				GameObject->n_Animation = static_cast<int>(data.ani);

			}
			break;


			default:
				break;
			}
		}
	}
}

void Scene::SET_NPC_ATTACK_BY_SERVER_DATA(const unsigned short & id, const NPC_BulletObject_Info & bulldata, unsigned char type, bool first_bullet, const XMFLOAT4& xmf4_pos)
{
	switch (type)
	{
		case PACKET_PROTOCOL_TYPE::NPC_MONSTER_IMP_ATTACK_STONEBULLET:
		{
			//넘어온 NPC 임프의 스톤불렛이 첫 불렛일 때 = 생성작업
			if (first_bullet)
			{
				CGameObject * master_npc{ nullptr };
				for (const auto& find_npc : DynamicObject)
				{
					if (!find_npc->isNPC) continue;
					if (find_npc->m_npc_data.id == bulldata.master_id)
					{
						master_npc = find_npc;
						break;
					}
				}

				/*
				XMFLOAT4 t_pos4f = { data.pos4f.x ,data.pos4f.y, data.pos4f.z, data.pos4f.w };
				BulletObject.emplace_back(new StoneBullet(master_npc->device, master_npc->commandlist, master_npc->ParticleList,
								      NULL, master_npc, XMFLOAT4(0, 0, 0, 1), NULL, t_pos4f, xmf4_pos));
				*/
				BulletObject.emplace_back(new StoneBullet(master_npc->device, master_npc->commandlist, master_npc->ParticleList,
										 NULL, master_npc, XMFLOAT4(0, 0, 0, 1), NULL, master_npc->CenterPos, xmf4_pos));

				//cout << "StoneBullet ID: " << bulldata.my_id << "Create First \n";

			}

			for (auto& bullet : BulletObject)
			{
				//서버에서 날라온 불릿아이디와 클라관리 불릿아이디가 같고 그들의 주인 ID가 같을 때
				if (bullet->m_npc_data.id == bulldata.master_id && dynamic_cast<StoneBullet*>(bullet)->myNPC_StoneBulletID == bulldata.my_id)
				{
					if (!bulldata.alive)
					{
						bullet->DelObj = true;

						//불렛과 대상 충돌 후 파티클 작업
						bullet->Collision(bulldata.after_coll, bulldata.damage, XMFLOAT4(bulldata.pos4f.x, bulldata.pos4f.y, bulldata.pos4f.z, bulldata.pos4f.w), XMFLOAT4(bulldata.pos4f.x, bulldata.pos4f.y, bulldata.pos4f.z, bulldata.pos4f.w));		
						break;
					}

					bullet->m_bullet_data.alive = bulldata.alive;
					bullet->m_bullet_data.pos4f = bulldata.pos4f;
					bullet->m_bullet_data.rot4f = bulldata.rot4f;
					bullet->m_bullet_data.master_id = bulldata.master_id;
					bullet->m_bullet_data.my_id = bulldata.my_id;
					
					bullet->CenterPos = { bulldata.pos4f.x, bulldata.pos4f.y, bulldata.pos4f.z, bulldata.pos4f.w };
					bullet->pp->SetPosition(&bullet->CenterPos);

					bullet->Orient = { bulldata.rot4f.x, bulldata.rot4f.y, bulldata.rot4f.z, bulldata.rot4f.w };
					bullet->UpdateLookVector();

					break;
				}
				
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
		case INSTALLED_OBJECT_TYPE::SecondFloor:
		case INSTALLED_OBJECT_TYPE::Column:
		case INSTALLED_OBJECT_TYPE::BrokenCart:
		{
			//클라는 0 ~ 77까지의 스테틱오브젝트가 순서대로 ID가 저장된 상태
			//서버에서 오는 데이터는 뒤죽박죽 섞인 ID를 갖는 스테틱오브젝트정보
			for (auto sobj : StaticObject)
			{
				if (sobj->m_sobj_data.ID == sobjdata.ID)
				{
					//상자, 컬럼 오브젝트의 텍스쳐를 서버에서 관리함. 이 랜덤데이터를 받아 텍스쳐값재정의
					if(type == INSTALLED_OBJECT_TYPE::Box || INSTALLED_OBJECT_TYPE::Column)
						sobj->ObjData.CustomData1.w = sobjdata.TextureColor;

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

void Scene::SET_MOVEOBJECT_BY_SERVER_DATA(const unsigned int & id, MoveObjectData & mvobjdata)
{
	for (auto mvobj : StaticObject)
	{
		if (mvobj->m_sobj_data.ID == mvobjdata.id)
		{
			mvobj->m_mvobj_data = move(mvobjdata);

			mvobj->Orient = { mvobjdata.rot4f.x ,  mvobjdata.rot4f.y ,  mvobjdata.rot4f.z,  mvobjdata.rot4f.w };
			mvobj->CenterPos = { mvobjdata.pos4f.x,  mvobjdata.pos4f.y , mvobjdata.pos4f.z, mvobjdata.pos4f.w };

			mvobj->pp->SetPosition(&mvobj->CenterPos);

			dynamic_cast<MoveCubeObject*>(mvobj)->Len = mvobjdata.len;
			dynamic_cast<MoveCubeObject*>(mvobj)->n = mvobjdata.n;

			if (mvobjdata.texture_color == 0)
				mvobj->TextureName = "redTex";
			else if (mvobjdata.texture_color == 1)
				mvobj->TextureName = "orangeTex";
			else if (mvobjdata.texture_color == 2)
				mvobj->TextureName = "yellowTex";
			else if (mvobjdata.texture_color == 3)
				mvobj->TextureName = "pinkTex";
			else if (mvobjdata.texture_color == 4)
				mvobj->TextureName = "whiteTex";
			else if (mvobjdata.texture_color == 5)
				mvobj->TextureName = "blueTex";
			else if (mvobjdata.texture_color == 6)
				mvobj->TextureName = "greenTex";

			mvobj->TexOff = mvobjdata.texture_color;


			mvobj->UpdateLookVector();
			mvobj->GetUpvector();

			break;
		}
	}
}

void Scene::SET_MOVEOBJECT_NOCREATE_BY_SERVER_DATA(const unsigned int & id, MoveObjectData_NoCreate & nomvobjdata)
{
	for (auto mvobj : StaticObject)
	{
		if (mvobj->m_sobj_data.ID == nomvobjdata.id)
		{
			mvobj->m_mvobj_data.pos4f = nomvobjdata.pos4f;
			mvobj->m_mvobj_data.rot4f = nomvobjdata.rot4f;

			mvobj->Orient = { nomvobjdata.rot4f.x ,  nomvobjdata.rot4f.y ,  nomvobjdata.rot4f.z,  nomvobjdata.rot4f.w };
			mvobj->CenterPos = { nomvobjdata.pos4f.x,  nomvobjdata.pos4f.y , nomvobjdata.pos4f.z, nomvobjdata.pos4f.w };

			mvobj->pp->SetPosition(&mvobj->CenterPos);

			mvobj->UpdateLookVector();
			mvobj->GetUpvector();

			break;
		}
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

void Scene::SET_BULLET_BY_SERVER_DATA(STC_BulletObject_Info & bulldata, const unsigned char & packet_type, bool first_bullet, const XMFLOAT3 & OffLookvector)
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

		for (auto lbul : BulletObject)
		{
			if (bulldata.master_id == lbul->m_bullet_data.master_id && bulldata.my_id == lbul->m_bullet_data.my_id)
			{
				//불렛이 소멸됨 -> 삭제
				if (!bulldata.alive)
				{				
					lbul->m_bullet_data = move(bulldata);

					lbul->CenterPos = { bulldata.pos4f.x, bulldata.pos4f.y, bulldata.pos4f.z, bulldata.pos4f.w };
					lbul->pp->SetPosition(&lbul->CenterPos);

					lbul->Orient = { bulldata.rot4f.x, bulldata.rot4f.y, bulldata.rot4f.z, bulldata.rot4f.w };
					lbul->UpdateLookVector();

					lbul->m_particle_type = bulldata.after_coll;

					//cout << "Bullet Type: " << static_cast<int>(bulldata.type) << "\n";
					
					//불렛과 대상 충돌 후 파티클 작업
					//lbul->Collision(bulldata.after_coll, bulldata.damage, XMFLOAT4(bulldata.pos4f.x, bulldata.pos4f.y, bulldata.pos4f.z, bulldata.pos4f.w), XMFLOAT4(bulldata.pos4f.x, bulldata.pos4f.y, bulldata.pos4f.z, bulldata.pos4f.w));
				
					//lbul->DelObj = true;
					/*
					cout << "Bullet ID: " << bulldata.my_id << "Bullet MID: " << bulldata.master_id <<
						"Position: " << bulldata.pos4f.x << ", " << bulldata.pos4f.y << ", " << bulldata.pos4f.z << ", " << bulldata.pos4f.w <<
						"IsAlive: " << static_cast<bool>(bulldata.alive) << endl;
					
					*/
					
					break;
				}

				lbul->m_bullet_data = move(bulldata);
				lbul->m_bullet_data.alive = true;

				lbul->CenterPos = { bulldata.pos4f.x, bulldata.pos4f.y, bulldata.pos4f.z, bulldata.pos4f.w };
				lbul->pp->SetPosition(&lbul->CenterPos);

				lbul->Orient = { bulldata.rot4f.x, bulldata.rot4f.y, bulldata.rot4f.z, bulldata.rot4f.w };
				lbul->UpdateLookVector();

				/*
				cout << "Bullet ID: " << lbul->myID << "PosX: " << lbul->CenterPos.x << ", " << lbul->CenterPos.y << ", " << lbul->CenterPos.z << ", " << lbul->CenterPos.w << "\n";
				cout << "Bullet ID: " << lbul->myID << "Orient: " << lbul->Orient.x << ", " << lbul->Orient.y << ", " << lbul->Orient.z << ", " << lbul->Orient.w << "\n";
				cout << "Bullet ID: " << lbul->myID << "Lookxyz: " << lbul->Lookvector.x << ", " << lbul->Lookvector.y << ", " << lbul->Lookvector.z << "\n";
				cout << "Bullet ID: " << lbul->myID << "Rightxyz: " << lbul->Rightvector.x << ", " << lbul->Rightvector.y << ", " << lbul->Rightvector.z << "\n";
				*/

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
		//NPC 체크가 필요없으면 반드시 if(GameObject->isNPC) continue를 넣어준다 
		if (GameObject->isNPC) continue;

		// 사용스킬에 대한 패킷을 보낸 ID == 현재 이 클라이언트의 아이디
		if (id == GameObject->m_player_data.id)
		{
			switch (playerdata.my_id)
			{
				// 실드를 따로 생성해서 Centerpos를 패킷으로 받은 아이디를 가진 플레이어값으로 지정한다 
				case CHAR_SKILL::SHIELD:
				{
					//cout << "Using Skill Master ID: " << id << "\t" << "m_player_data ID: " << static_cast<int>(GameObject->m_player_data.id) << "\n";
					if (playerdata.alive)
					{
						NoCollObject.push_back(new ShieldArmor(device, commandlist, &BbObject, &Shadows, GameObject, GameObject->CenterPos));
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

void Scene::SET_PLAYER_SKILL(const unsigned int & id, const STC_SkillData & playerdata, unsigned char texture_number)
{
	for (auto GameObject : DynamicObject)
	{
		if (GameObject->isNPC) continue;

		// 사용스킬에 대한 패킷을 보낸 ID == 클라이언트 리스트의 아이디들
		if (id == GameObject->m_player_data.id)
		{
			//사용한 스킬의 아이디 = 파동파
			switch (playerdata.my_id)
			{
				case CHAR_SKILL::WAVE_SHOCK:
				{
					//파동파 스킬이 살아있는지 아닌지
					if (playerdata.alive)
					{
						//다른 클라이언트만 접근
						StaticObject.push_back(new RingObject(device, commandlist, &BbObject, &Shadows, GameObject, GameObject->CenterPos));			
					}
				}
				break;

			}
		}
	}
}

void Scene::SET_PLAYER_SKILL(const STC_HammerSkillInfo & hammer_bullet)
{
	STC_HammerSkillInfo hmdata = hammer_bullet;

	// 1.create_first = true 와 headBullet = true는 Create 할 필요없음 
	// 2.create_first = true 와 headBullet = false 로 날아올거임
	// 3.create_first = false 와 headBullet은 이제 상관없음 
	// [*] 내클라 -> 내 클라  
	// [*] 내클라 -> 니 클라 
	
	// 내클라 -> 니 클라 (create_first == true 처리랑 데이터 업데이트)
	// 죄다 create_first는 처음에 true로 옴
	if (hmdata.master_id != Player->PlayerObject->m_player_data.id)
	{
		if (hmdata.create_first)
		{
			for (const auto& client : DynamicObject)
			{
				if (client->isNPC) continue;

				if (client->m_player_data.id == hmdata.master_id)
				{
					XMFLOAT4 ori = XMFLOAT4(hmdata.rot4f.x, hmdata.rot4f.y, hmdata.rot4f.z, hmdata.rot4f.w);
					XMFLOAT4 CenterPos = XMFLOAT4(hmdata.pos4f.x, hmdata.pos4f.y, hmdata.pos4f.z, hmdata.pos4f.w);
					XMFLOAT4 opp = XMFLOAT4(hmdata.opp_pos4f.x, hmdata.opp_pos4f.y, hmdata.opp_pos4f.z, hmdata.opp_pos4f.w);

					CGameObject *hammer = new HammerBullet(device, commandlist, client->ParticleList, NULL, NULL, 0, client, ori, NULL, CenterPos, opp);
					BulletObject.push_back(hammer);

					//cout << "MyClient HammerBulet\n";
				}
			}
		}
	}
	
	if (hmdata.master_id == Player->PlayerObject->m_player_data.id)
	{
		//if (hmdata.create_first == true && hmdata.headBullet == true) 는 처리하지 않음 (내클라에서 내클라로 보내진 head hammer 데이터이므로)
		if (hmdata.create_first == true && hmdata.headBullet == false)
		{
			XMFLOAT4 ori = XMFLOAT4(hmdata.rot4f.x, hmdata.rot4f.y, hmdata.rot4f.z, hmdata.rot4f.w);
			XMFLOAT4 CenterPos = XMFLOAT4(hmdata.pos4f.x, hmdata.pos4f.y, hmdata.pos4f.z, hmdata.pos4f.w);
			XMFLOAT4 opp = XMFLOAT4(hmdata.opp_pos4f.x, hmdata.opp_pos4f.y, hmdata.opp_pos4f.z, hmdata.opp_pos4f.w);

			CGameObject *hammer = new HammerBullet(device, commandlist, Player->PlayerObject->ParticleList, NULL, NULL, 0, Player->PlayerObject, ori, NULL, CenterPos, opp);
			BulletObject.push_back(hammer);

			//cout << "MyClient to OtherClient HammerBulet\n";
		}
	}
	
	//같은 경우 - 불렛 데이터 업데이트
	for (auto bullet : BulletObject)
	{
		if (bullet->m_bullet_type == BULLET_TYPE::protocol_HammerBullet &&
			bullet->m_bullet_data.master_id == hmdata.master_id &&
			bullet->m_bullet_data.my_id == hmdata.my_id)
		{
			if (!hmdata.alive)
			{
				bullet->m_bullet_data.alive = false;
				bullet->m_bullet_data.damage = hmdata.damage;

				bullet->CenterPos = { hmdata.pos4f.x, hmdata.pos4f.y, hmdata.pos4f.z, hmdata.pos4f.w };
				bullet->pp->SetPosition(&bullet->CenterPos);

				bullet->Orient = { hmdata.rot4f.x, hmdata.rot4f.y, hmdata.rot4f.z, hmdata.rot4f.w };
				bullet->UpdateLookVector();

				bullet->m_particle_type = hmdata.after_coll;
				break;
			}

			bullet->m_bullet_data.alive = true;
			bullet->CenterPos = { hmdata.pos4f.x, hmdata.pos4f.y, hmdata.pos4f.z, hmdata.pos4f.w };
			bullet->pp->SetPosition(&bullet->CenterPos);

			bullet->Orient = { hmdata.rot4f.x, hmdata.rot4f.y, hmdata.rot4f.z, hmdata.rot4f.w };
			bullet->UpdateLookVector();
			break;
		}
	}
}

void Scene::SET_LOGIN_BY_SERVER_DATA(const STC_LoginData & playerdata)
{
	//이게 실행됐다는 것은 해당 방의 인원이 모두 레디를 눌러서 플레이를 한다는 것
	Player->m_async_client->m_mySkipLogin = true;

	Player->m_async_client->m_clientsID.push_back({ playerdata.my_id, playerdata.texture_id });

	SetGameState(GS_LOAD);

}



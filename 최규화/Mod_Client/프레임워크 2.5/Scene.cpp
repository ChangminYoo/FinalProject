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
	
	//��Ʈ�ñ״�ó�� ���̴����� �����Ѵ�.

	CreateRootSignature();
	nShader = 1;
	Shaders = new Shader;
	CreateShaderObject();
	Player = new CPlayer(m_Device, m_DC, cw / ch, e, a, u);
	Shaders->player = Player;//���� �÷��̾ �����Ǿ� �Ѵ�.
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


	//����� ���ҽ��� ���. ����� ť���� ���庯ȯ��� �ϳ��� ������.
	CD3DX12_ROOT_PARAMETER RootParameter[7];

	//�Ķ������ ������ 3������  ��ũ���;�����


	//�Ķ���͸�Ͽ� �ش� ����� ���ڷ� �����鼭 �ʱ�ȭ��
	RootParameter[0].InitAsDescriptorTable(1, &SkytexTable, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParameter[1].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);

	RootParameter[2].InitAsConstantBufferView(0); //obj     //register b0
	RootParameter[3].InitAsConstantBufferView(1); //joint   //b1
	RootParameter[4].InitAsConstantBufferView(2); //camera  //b2
	RootParameter[5].InitAsConstantBufferView(3); //light	//b3
	RootParameter[6].InitAsConstantBufferView(4); //material	//b4


	auto staticSamplers = GetStaticSamplers();

	// ��Ʈ�ñ״�ó�� ��Ʈ�Ķ������ �迭�̴�.
	//��Ʈ�ñ״�ó�� ��������ü ����
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(7, RootParameter, (UINT)staticSamplers.size(), staticSamplers.data(),
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
		Shaders->BbObject = &BbObject;
		Shaders->LandObject = &LandObject;
	}
	
}

void Scene::CreateGameObject()
{

	//--------------- �޽��� �ؽ�ó �ʱ�ȭ -------------//

	CGameObject* resource = NULL;
	resource = new CCubeManObject(device, commandlist, &BbObject, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new CZombieObject(device, commandlist, &BbObject, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new BulletCube(device, commandlist,  &BbObject,NULL,XMFLOAT4(0,0,0,1),NULL, XMFLOAT4(0, -0, 0, 0));
	delete resource;
	resource = new SphereObject(device, commandlist,  &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new CubeObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new GridObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new TreeObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;
	resource = new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(0, 0, 0, 0));
	delete resource;

	//--------------------------------------------------//
	
	SkyObject = new SphereObject(device, commandlist,  &BbObject, XMFLOAT4(0, 0, 0, 0));

	DynamicObject.push_back(new CCubeManObject(device, commandlist,&BbObject, XMFLOAT4(0, 0, -50, 0)));
	DynamicObject.push_back(new CCubeManObject(device, commandlist,&BbObject, XMFLOAT4(0, 0, -20, 0)));

	//DynamicObject.back()->pp->SetBounce(true);
	//DynamicObject.back()->pp->AddForce(-600, 0, 600);
	//DynamicObject.back()->pp->integrate(0.1f);//���� ���������� �������°��̸� ������̷��� ���� ���� �ð��� ���� ����Ѵ�.
	StaticObject.push_back(new CubeObject(device, commandlist,  &BbObject, XMFLOAT4(-20, 0, 0, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist,  &BbObject, XMFLOAT4(-40, 10, 0, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist,  &BbObject, XMFLOAT4(50, 0, -40, 0)));
	StaticObject.push_back(new CubeObject(device, commandlist,  &BbObject, XMFLOAT4(30, 0, 40, 0)));

	RigidObject.push_back(new RigidCubeObject(device, commandlist,  &BbObject, XMFLOAT4(25, 200, 10, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist,  &BbObject, XMFLOAT4(-11, 130, 10, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist,  &BbObject, XMFLOAT4(0.5, 50, 3, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist,  &BbObject, XMFLOAT4(13, 90, 7, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist,  &BbObject, XMFLOAT4(15, 40, 39, 0)));
	RigidObject.push_back(new RigidCubeObject(device, commandlist, &BbObject, XMFLOAT4(20, 100, 0, 0)));


	for (int i = -6; i <= 8; ++i)
	{
		for (int j = -6; j <= 8; ++j)
		{
			LandObject.push_back(new GridObject(device, commandlist,&BbObject, XMFLOAT4(j * 50, -1, i * 50, 0)));
			
		}
	}


	//BbObject.push_back(new TreeObject(device, commandlist, &BbObject, XMFLOAT4(0,0,0,0)));

	int player_num = 0;
	for (auto obj : DynamicObject)
	{
		obj->m_player_data.ID = player_num;
		++player_num;
	}

	//�÷��̾��� ������Ʈ ����. �̰� ���߿� �ٲ����.
	Player->SetPlayer(DynamicObject.front());
	Player->PlayerObject->Blending = false;

}

void Scene::Render(const GameTimer& gt)
{

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
			
			
			//���̴��� ������ �׷����� ������Ʈ ����� �׸���.
			Shaders->Render(commandlist,gt);
			
		
	}
}

void Scene::Tick(const GameTimer & gt)
{
	//�÷��̾� �Է�ó�� 
	Player->PlayerInput(gt.DeltaTime(),this);

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


	//--------------------------------------------------------------
	//������Ʈ ƽ�Լ� ó��
	//--------------------------------------------------------------
	for (auto b = DynamicObject.begin(); b != DynamicObject.end(); b++)
		(*b)->Tick(gt);

	//�ҷ�
	for (auto b = BulletObject.begin(); b != BulletObject.end(); b++)
		(*b)->Tick(gt);


	for (auto b = BbObject.begin(); b != BbObject.end(); b++)
		(*b)->Tick(gt);
	
	//������ �ٵ�
	for (auto b = RigidObject.begin(); b != RigidObject.end(); b++)
		(*b)->Tick(gt);

	
	//ī�޶� �� ������Ʈ 
	Player->PlayerCameraReLocate();
}

Player_Data* Scene::Get_PlayerServerData(const unsigned int& id)
{
	// map�� �̿��ؼ� first ���̵� ���� ���� ������ ã������?
	// �׳� list�� ����Ұ���?

	for (auto GameObject : DynamicObject)
	{
		if (GameObject->m_player_data.ID == id) //- MAX_MONSTER_NUM)
		{
			Get_GameObject = GameObject;
			return &(GameObject->m_player_data);
		}

	}

	return nullptr;
}


Player_Data* Scene::Get_MonsterServerData(const unsigned int& id)
{
	return nullptr;
}

void Scene::SET_PLAYER_BY_SEVER_DATA(const unsigned short& id, Player_Data& playerdata, const unsigned char& packet_type)
{
	for (auto GameObject : DynamicObject)
	{
		if (GameObject->m_player_data.ID == id)
		{
			switch (packet_type)
			{
				case PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT:
				{
					GameObject->m_player_data = move(playerdata);

					//GameObject->Orient = { playerdata.Rotate_status.x , playerdata.Rotate_status.y, playerdata.Rotate_status.z, playerdata.Rotate_status.w };
					GameObject->CenterPos = { playerdata.Pos.x , playerdata.Pos.y , playerdata.Pos.z , playerdata.Pos.w };

					GameObject->gamedata.Damage = move(playerdata.UserInfo.player_status.attack);
					GameObject->gamedata.HP = move(playerdata.UserInfo.cur_hp);
					GameObject->gamedata.MAXHP = move(playerdata.UserInfo.origin_hp);
					GameObject->gamedata.Speed = move(playerdata.UserInfo.player_status.speed);

				}
				break;

				case PACKET_PROTOCOL_TYPE::INIT_CLIENT:
				{
					GameObject->m_player_data = move(playerdata);

					//GameObject->Orient = { playerdata.Rotate_status.x , playerdata.Rotate_status.y, playerdata.Rotate_status.z, playerdata.Rotate_status.w };
					GameObject->CenterPos = { playerdata.Pos.x , playerdata.Pos.y , playerdata.Pos.z , playerdata.Pos.w };

					GameObject->gamedata.Damage = move(playerdata.UserInfo.player_status.attack);
					GameObject->gamedata.HP = move(playerdata.UserInfo.cur_hp);
					GameObject->gamedata.MAXHP = move(playerdata.UserInfo.origin_hp);
					GameObject->gamedata.Speed = move(playerdata.UserInfo.player_status.speed);

					if (GameObject->m_player_data.ID == my_ClientID)
						Player->SetPlayer(GameObject);
				}
				break;

				case PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION:
				{
					GameObject->m_player_data.Pos = move(playerdata.Pos);
					GameObject->m_player_data.Ani = move(playerdata.Ani);

					GameObject->CenterPos = { playerdata.Pos.x , playerdata.Pos.y , playerdata.Pos.z , playerdata.Pos.w };
					GameObject->n_Animation = playerdata.Ani;

				}
				break;

				case PACKET_PROTOCOL_TYPE::PLAYER_DISCONNECT:
				{
					//client �� flag�� false��
				}
				break;

				case PACKET_PROTOCOL_TYPE::PLAYER_ROTATE:
				{
					GameObject->m_player_data.Rotate_status = move(playerdata.Rotate_status);

					GameObject->Orient = { playerdata.Rotate_status.x , playerdata.Rotate_status.y, playerdata.Rotate_status.z, playerdata.Rotate_status.w };
				}
				break;

			break;

			}
			
			//GameObject->Orient = { playerdata.Rotate_status.x , playerdata.Rotate_status.y , playerdata.Rotate_status.z , playerdata.Rotate_status.w };

			//�޾ƿ� ���̵� �� Ŭ���̾�Ʈ ���̵��� ��
			
			//	GameObject->m_player->PlayerObject = move(GameObject);

			//���⿡�� ���� �ִϸ��̼� ������Ʈ �־��ְ� ��Ŷ�ְ���������� ��ȭ�Ǿ��� Ŭ�������� �־�����

		}
	}
}

void Scene::Set_RemovePlayerData(const unsigned int & id, Player_Data & playerdata)
{
	for (auto GameObject : DynamicObject)
	{
		if (GameObject->m_player_data.ID == id)
		{
			delete GameObject;
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
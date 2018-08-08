#include"mainFrameWork.h"

boost::asio::io_service g_io_service;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		MainFrameWork theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		thread* f_thread = new thread([]() {g_io_service.run(); });

		auto RunApp = theApp.Run();

		f_thread->join();
		delete f_thread;

		return theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

MainFrameWork::MainFrameWork(HINSTANCE hInstance) : FrameWork(hInstance)
{

}

MainFrameWork::~MainFrameWork()
{
}

bool MainFrameWork::Initialize()
{
	if (!FrameWork::Initialize())
		return false;

	scene->Player->m_async_client = new AsyncClient();
	scene->Player->m_async_client->Init(scene->Player->PlayerObject, scene);


	return true;
}

//�浹 �ý����� �ݵ�� ƽ�Լ� ���Ŀ� ó���Ǿ��Ѵ�. �ֳ��ϸ� ƽ�Լ����� ó���Ǹ� ������ �����ΰ� �Ǳ� �����̴�.
//�浹�� ������ ���Ŀ� �߻��Ѵ�.
//���� ������ ��ü�� ������ ��� ������Ʈ�� �˻��ؾ��Ѵ�.
//��� �ϳ��� ������ �ϴµ�, �����Ѵ���, �ش� ������Ʈ�� �˻��ؾ��� ����� ã�Ƽ� ���ڷ� �־��ش�.
//������Ʈ�� Collision�Լ��� ȣ���ϴµ� �Ʊ� ���ڷ� ���� ����Ʈ ����� �ָ��� ���鼭
//CollisionTest�Լ��� ȣ���Ѵ�. �̳༮�� ������ OBB�浹�˻縦 �ϴ� �༮�̴�.
//�浹�� ������ CollsionResolve �Լ��� ȣ���ؼ� �浹�� �ؼ����ش�.
//�߿������� �浹�ؼ��Ŀ� �ݵ��! UpdatePPosCenterPos�Լ��� ȣ���ؼ� PP�� �������� ���ӿ�����Ʈ�� ������ �����ؾ��Ѵ�.
void MainFrameWork::CollisionSystem(const GameTimer& gt)
{
	//�ƹ� �ϵ� ����

	
	//for (auto i = scene->RigidObject.begin(); i != scene->RigidObject.end(); i++)
	//{
	//	(*i)->Collision(&scene->RigidObject, gt.DeltaTime());
	//	(*i)->Collision(&scene->DynamicObject, gt.DeltaTime());
	//	(*i)->Collision(&scene->StaticObject, gt.DeltaTime());
	//	(*i)->Collision(&scene->BulletObject, gt.DeltaTime());
	//
	//}

	//�ݵ�� ����ü�� ������� ��븦 �˻��Ѵ�.
	for (auto i = scene->BulletObject.begin(); i != scene->BulletObject.end(); i++)
	{
		//FindCollisionObject �Լ��� ���߿� ��������. ����� �׳� DynamicObject�� �ش�.
		//�ش� ��ü�� �浹ó�� �˻縦 �� ����� ������.
		//�� ����� ������ �浹�˻縦 �ϵ�����.
		if ((*i)->DelObj) continue;

		if (!(*i)->m_bullet_data.alive)
			(*i)->Collision((*i)->m_particle_type, (*i)->m_bullet_data.damage, (*i)->CenterPos, (*i)->CenterPos);
	
		//����ü������ �˻� X ����ü�� �ݵ�� ���̳��Ϳ�����Ʈ ���� �˻��ؾ���.
		//(*i)->Collision(&scene->DynamicObject, gt.DeltaTime());
		//(*i)->Collision(&scene->StaticObject, gt.DeltaTime());
	}

	//������ ��ü�� ������ ��� ������Ʈ�� �浹�˻縦 �ϵ��� ��.
	//for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	//{
	//	//FindCollisionObject �Լ��� ���߿� ��������. ����� �׳� DynamicObject�� �ش�.
	//	//�ش� ��ü�� �浹ó�� �˻縦 �� ����� ������.
	//	//�� ����� ������ �浹�˻縦 �ϵ�����.
	//
	//	(*i)->Collision(&scene->DynamicObject, gt.DeltaTime());
	//	(*i)->Collision(&scene->StaticObject, gt.DeltaTime());
	//
	//}
	

}

void MainFrameWork::System(const GameTimer & gt)
{
	//�ƹ� �ϵ� ����

	//GravitySystem(gt);
}

void MainFrameWork::GravitySystem(const GameTimer & gt)
{
	//�ƹ� �ϵ� ����

	/*
	GeneratorGravity gg;
	gg.SetGravityAccel(XMFLOAT3(0, -100, 0));
	//������ ��ü�� ������ ��������Ʈ�� �߷��� ���Ѵ�. �� ����ü�� �����Ѵ�.
	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		if((*i)->staticobject==false)
		gg.Update(gt.DeltaTime(), *(*i)->pp);
	}
	for (auto i = scene->RigidObject.begin(); i != scene->RigidObject.end(); i++)
	{

		gg.Update(gt.DeltaTime(), *(*i)->rb);
	}
	*/

}

void MainFrameWork::AfterGravitySystem(const GameTimer & gt)
{
	//����ü�� y�� 0���� ������ ���ŵȴ�.
	for (auto i = scene->BulletObject.begin(); i != scene->BulletObject.end(); i++)
	{
		if ((*i)->CenterPos.y <= 0)
		{
			auto BulletParticles2 = new ParticleObject2(Device.Get(), mCommandList.Get(), &scene->BbObject, &scene->Shadows, NULL, 0.7f, 100, XMFLOAT4((*i)->CenterPos.x, (*i)->CenterPos.y, (*i)->CenterPos.z, 0));
			scene->BbObject.push_back(BulletParticles2);

			(*i)->DelObj = true;

		}
	}
	//�ƹ� �ϵ� ����

	/*
	static bool StaticProcess = false;

	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		//�� ���� ������ �ƴ� pp�� �������� ó���ϳĸ� ���������� �������� pp�� ������ �����̳�
		//pp�������� �����̰� ���������� �����̳� ������, UpdatePPosCenterPos�� ��������
		//pp�� �����̰� cp�� pp�� �����.
		float ppy = (*i)->pp->GetPosition().y;
		float hby = (*i)->pp->GetHalfBox().y;
		if (ppy - hby < 0)//pp�� ����y-�����ڽ��� y���� �Ѱ���� 0���� ������ ���Ʒ��� ������
		{
			XMFLOAT4 gp = (*i)->pp->GetPosition();
			gp.y += hby - ppy;//�׷��� �ݴ�� �����ڽ�y��-����y��ŭ �ø��� �ȴ�.
			*(*i)->pp->CenterPos=gp;
			
			auto v = (*i)->pp->GetVelocity();
			v.y = 0;//�߷¿� ���� �ӵ��� 0���� ����
			(*i)->pp->SetVelocity(v);
			(*i)->AirBone = false;
		}
	}

	if (StaticProcess == false)//������ ��ü�� �ѹ��� ����ε� ��ġ�� �÷��д�.
	{
		for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
		{

			//�� ���� ������ �ƴ� pp�� �������� ó���ϳĸ� ���������� �������� pp�� ������ �����̳�
			//pp�������� �����̰� ���������� �����̳� ������, UpdatePPosCenterPos�� ��������
			//pp�� �����̰� cp�� pp�� �����.
			float ppy = (*i)->pp->GetPosition().y;
			float hby = (*i)->pp->GetHalfBox().y;
			if (ppy - hby < 0)//pp�� ����y-�����ڽ��� y���� �Ѱ���� 0���� ������ ���Ʒ��� ������
			{
				XMFLOAT4 gp = (*i)->pp->GetPosition();
				gp.y += hby - ppy;//�׷��� �ݴ�� �����ڽ�y��-����y��ŭ �ø��� �ȴ�.
				*(*i)->pp->CenterPos = gp;

				auto v = (*i)->pp->GetVelocity();
				v.y = 0;//�߷¿� ���� �ӵ��� 0���� ����
				(*i)->pp->SetVelocity(v);
				(*i)->AirBone = false;
			}
		}
		StaticProcess = true;
	}
	for (auto i = scene->RigidObject.begin(); i != scene->RigidObject.end(); i++)
	{
		RigidBodyCollisionPlane(XMFLOAT3(0, 1, 0), 0, *i);
	}
	*/
}

void MainFrameWork::OnResize()
{
	FrameWork::OnResize();
}

void MainFrameWork::FrameAdvance(const GameTimer& gt)
{
	//�����Ӿ�庥���� ������ �޸𸮸��º��ʹ�.
	mDirectCmdListAlloc->Reset();
	mCommandList->Reset(mDirectCmdListAlloc.Get(), NULL);

	//����Ʈ ����
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);



	//�������۸� ����Ÿ������ ����
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//�ĸ���ۿ� ���̹��� �ʱ�ȭ
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// ���� ����۸� ����.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	//���� ������� ����� ������������ �������
	// RS ����Ʈ�� ������Ʈ , OM�� ����Ÿ�ٰ� �������ٽ� ��
	//������ �ؾ��Ұ�. ��Ʈ�ñ״�ó�� VS,PS�� PSO, ��������, �ε������� , ������ۺ�

	scene->SceneState();
	Update(gt);

	if (scene->Player->PlayerObject != nullptr)
		scene->Player->m_async_client->SendPacketRegular(*scene->Player->PlayerObject, gt);


	Draw(gt);

	//������� ������ PSO�� ���� �� ����Ǿ� �ְ�, �� �׷����ִ°��̴�.
	//���ҽ� ����� �б������ �����Ѵ�.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));


	// Ŀ�ǵ帮��Ʈ�� �ݴ´�.
	ThrowIfFailed(mCommandList->Close());

	// Ŀ�ǵ帮��Ʈ�� Ŀ�ǵ� ť�� ����ִ´�.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// �б������ ������Ʈ�Ѵ�
	ThrowIfFailed(SwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	//�÷���
	FlushCommandQueue();

}

void MainFrameWork::Update(const GameTimer& gt)
{
	
		//���� �ý��� Ŭ������ ������Ʈ �Ѵ�. �ý��� Ŭ������ ���� �������ΰ͵��� ������. �߷� ��
	if (scene->GetGameState() == GS_PLAY)
	{
		System(gt);
		//��Ŭ������ ������Ʈ�� ȣ���Ѵ�.
		scene->Tick(gt);
		
		//�߷� ��ó�� �ý����� ƽ�Լ� ���Ŀ� ó���ؾ���

		AfterGravitySystem(gt);
		//�浹 ó�� �ý����� ƽ�Լ� ���Ŀ� ó���ؾ��Ѵ�.

		CollisionSystem(gt);
	}
}

void MainFrameWork::Draw(const GameTimer& gt)
{
	scene->Render(gt);
}

void MainFrameWork::RigidBodyCollisionPlane(XMFLOAT3 & Normal, float distance, CGameObject * obj)
{


	if (obj->rb != NULL)
	{
		if (obj->rb->AmendTime > 0)
		{
			obj->rb->AmendTime -= mTimer.DeltaTime();

			if (obj->rb->AmendTime <= 0)
			{
				obj->rb->AmendTime = 0;
				obj->rb->SetAngularVelocity(0, 0, 0);
				obj->rb->SetVelocity(0, 0, 0);
			}

		}
		else
		{
			obj->rb->AmendTime = 0;

		}

		XMFLOAT4 arr[8];
		obj->rb->GetEightPoint(arr, obj->GetUpvector(), obj->Lookvector, obj->Rightvector);//���� 8 ���� ���� �����´�.
		std::vector<CollisionPoint> allpoint;
		std::vector<CollisionPoint> tempcollisionpoint;
		std::vector<CollisionPoint> contactpoint;



		for (int i = 0; i < 8; i++)
		{
			float temppenetration = arr[i].x*Normal.x + arr[i].y*Normal.y + arr[i].z*Normal.z;
			//�浹 ���� �����Ѵ��� �����Ѵ�.
			CollisionPoint cp;
			cp.Pos = arr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			allpoint.push_back(cp);
		}
		// 5) ���� ó����
		//penetration�� ���� ������ ����
		sort(allpoint.begin(), allpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
		{
			return a.penetration < b.penetration;
		});

		// 6) ���� ó����.
		//������ ���ĵ� �༮�� 1�������� �浹�� �� �༮���� tempcollisionpoint�� �����Ѵ�.
		for (auto i : allpoint)
		{
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0���ϸ� ������ �浹��
				tempcollisionpoint.push_back(i);

		}
		//���� ���� ���� �༮�� �������� �浹����Ʈ�� ���鼭 penetration�� ������ ����� ����� ������
		//�̳༮�� �о�ų� �������Ҷ� ���� �� �ִ� �浹���̴�. �� �͵��� �������� �浹���̴�.

		// 7) ���� ó����
		for (auto i : tempcollisionpoint)
		{
			//�����浹�� ù��° ���� - penetration���� ���ؼ� Epsilon�̸� ���� ���� �������� ����Ǵ� ���� �ִ´�.
			if (fabsf(tempcollisionpoint.front().penetration - i.penetration) <= 0.12)//���� 0.1������ ������ �浹 ���ɼ��� �ִٰ� �Ǵ��Ѵ�.
			{
				contactpoint.push_back(i);
			}

		}

		//------------- ���Ⱑ ���� �߿��� �б� -----------------//

		//------------- size �� 1/2/4/�׿� �϶� �� ������ ó�� ------------------//
		int size = contactpoint.size();
		if (size == 1)
		{
			//size�� 1�̸� ���ؾ��ұ�? �ٷ� ���� 2���� �ºε������ֵ��� ȸ�����Ѿ��Ѵ�.
			//�׷��� ���ؼ��� ������ ��ġ�� ��ݷ��� ���Ѵ�.
			//������ �̰� ��ȸ���� �Ͼ �� �ִ�.
			//���� ������ ȸ�������� �Ǹ� �� ������ �����Ѵ�. �ٸ� �ش� ��ݷ��� �ʹ� ũ�� �������� �����Ѵ�.

			//2������ - 1�������� �ؼ� 2�������� ���ϴ� ���͸� �����.
			//�̶� 2���� ���� ���ϴ� ������ Ư������ ���ϸ� �� ������ �����Ѵ�.

			//������ 2�������� ��������� �Ұ��ΰ� �̴�. ����Ǽ��� 2������. �����̳� �������� / �Ǵ� �밢�� ��.

			//���尡��� 2������.
			auto V1 = Float4Add(allpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			//�������� �밢���� ��ġ�� 2������
			auto sV1 = Float4Add(allpoint[3].Pos, contactpoint[0].Pos, false);
			sV1 = Float4Normalize(sV1);


			//���尡��� 2�������� ������ ���ϱ�
			float NdotV1 = Normal.x*V1.x + Normal.y*V1.y + Normal.z*V1.z;

			XMFLOAT3 ProjAB = Normal;
			ProjAB.x *= NdotV1;
			ProjAB.y *= NdotV1;
			ProjAB.z *= NdotV1;

			auto V2 = Float4Add(V1, XMFloat3to4(ProjAB), false);
			V2 = Float4Normalize(V2);




			auto tempdot = V1.x*V2.x + V1.y*V2.y + V1.z*V2.z;
			if (abs(tempdot) > 1)
			{
				if (tempdot > 0)
					tempdot = 1;
				else
					tempdot = -1;
			}
			//������ ����.  
			double theta = acos(tempdot);



			//�밢�� 2�������� ������ ���ϱ�
			float sNdotV1 = Normal.x*sV1.x + Normal.y*sV1.y + Normal.z*sV1.z;

			XMFLOAT3 sProjAB = Normal;
			sProjAB.x *= sNdotV1;
			sProjAB.y *= sNdotV1;
			sProjAB.z *= sNdotV1;

			auto sV2 = Float4Add(sV1, XMFloat3to4(sProjAB), false);
			sV2 = Float4Normalize(sV2);




			auto stempdot = sV1.x*sV2.x + sV1.y*sV2.y + sV1.z*sV2.z;
			if (abs(stempdot) > 1)
			{
				if (stempdot > 0)
					stempdot = 1;
				else
					stempdot = -1;
			}
			//������ ����.  
			double stheta = acos(stempdot);



			//��ݷ��� ����. ��ݷ��� Ư�� �� �����϶��� ��������.

			CollisionPoint fp;//��ݷ��� ���� ����
			fp.Pos = XMFLOAT4(0, 0, 0, 0);
			fp.pAxis = Normal;

			fp.Pos = contactpoint[0].Pos;
			fp.penetration = contactpoint[0].penetration;
			float impurse = obj->rb->CalculateImpulse(fp, NULL, 1);

			//�ִ����޽��� ���Ѵ�.
			if (impurse > obj->rb->GetMaxImpurse())
				impurse = obj->rb->GetMaxImpurse();


			//�ּ� ���޽��� ���Ѵ�.
			if (impurse < obj->rb->GetMinImpurse())
				impurse = obj->rb->GetMinImpurse();




			//���� ���հ��� Ư������ ���ϸ� ������Ų��. 
			//�� �̰� double�� �ص� 0�̾ƴѵ� 0�̳����°�찡 �����.
			//���� 0�ϰ�� �׳� ��ݷ��� ���ؼ� ������ �����Ų��.

			if (abs(stheta) <= MMPE_PI / 20 && abs(stheta) != 0 && abs(impurse) < obj->rb->GetMaxImpurse() && obj->rb->AmendTime <= 0)
			{
				//ȸ������ ���ϰ�..
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(sV1, sV2));
				mAxis = Float3Normalize(mAxis);
				//������ ��Ų��.
				AmendObject(mAxis, stheta, obj);

				//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
				//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
				obj->rb->SetAngularVelocity(0, 0, 0);

			}
			else if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && abs(impurse) < obj->rb->GetMaxImpurse() && obj->rb->AmendTime <= 0)//�뷫 5�� ���ϸ� ������Ŵ.
			{
				//ȸ������ ���ϰ�..
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V2));
				mAxis = Float3Normalize(mAxis);
				//������ ��Ų��.
				AmendObject(mAxis, theta, obj);

				//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
				//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
				obj->rb->SetAngularVelocity(0, 0, 0);


			}
			else//�ƴ϶�� ���� ��ݷ��� ������ ������ ���Ѵ�!
			{
				//���⿡ �Դٴ°��� ���̻� ������ ���� �ʰų� ������ ���� ���ʿ䰡 ���� ����.



				//Jm = J/M
				//���޽��� ������ ������.  ��. �Ϲ����� 1:1 ���迡�� 1.3: 0.3 ������ �д�.
				//������ 0.7�� �ҽǵ� �������� ġ��.
				if (obj->rb->AmendTime <= 0)
				{
					auto ratioImpurse = impurse * 0.3;

					auto Jm = Normal;



					Jm.x *= obj->rb->GetMass()*(impurse + ratioImpurse);
					Jm.y *= obj->rb->GetMass()*(impurse + ratioImpurse);
					Jm.z *= obj->rb->GetMass()*(impurse + ratioImpurse);



					//���ӵ� ���
					//W = ���� ���ӵ� + ((Q-P)Ximpurse)*InverseI
					auto W = obj->rb->GetAngularVelocity();
					XMVECTOR rxi = XMLoadFloat3(&XMFloat4to3(Float4Add(fp.Pos, obj->CenterPos, false)));
					rxi = XMVector3Cross(rxi, XMLoadFloat3(&Normal));
					rxi *= (ratioImpurse);
					rxi = XMVector3Transform(rxi, XMLoadFloat4x4(&obj->rb->GetIMoment()));

					XMFLOAT3 ia;
					XMStoreFloat3(&ia, rxi);

					W = Float3Add(W, ia);
					W = Float3Float(W, obj->rb->GetE() / 2);
					XMFLOAT3 lastvel = obj->rb->GetVelocity();
					obj->rb->SetVelocity(Float3Float(Float3Add(lastvel, Jm), obj->rb->GetE()));
					obj->rb->SetAngularVelocity(W);
				}

				//���� �ӵ��� ���ӵ��� ���� ������, ������ �κ� �ؼ�
				//���� �������� penetration(���� ����)��ŭ �ø��� �ȴ�.
				auto px = fabsf(contactpoint[0].penetration)*Normal.x;
				auto py = fabsf(contactpoint[0].penetration)*Normal.y;
				auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
				obj->CenterPos.x += px;
				obj->CenterPos.y += py;
				obj->CenterPos.z += pz;





			}


		}
		else if (size == 2)
		{
			//�߶վ��� �� 2���� ������ �ؾ��Ѵ�.
			//������ �ؾ��ϴ��� �˻縦 �ϱ� ���� ����� ��ְ� ������ �ΰ��� ���� ���Ͱ� ������ 0�� �������� �˻��Ѵ�.
			auto V1 = Float4Add(contactpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			float Dot = V1.x*Normal.x + V1.y*Normal.y + V1.z*Normal.z;

			if (fabsf(Dot) <= MMPE_EPSILON)
				Dot = 0;
			//����� 0�̸� ������ �ʿ䰡 ����.
			if (Dot == 0)
			{
				//���� 2���� �浹�� 4���� ���浹�� �ٲ� �� �ִ� ������ �� �� �ִ��� �˻��Ѵ�.
				//�׷��� ���ؼ� 3��° ��- ù��° �� �� �ؼ� 3��° ���� ���ϴ� ���͸� �����.
				//�� �� �̰� V1�� ������ 0 �� ������ ù��° ���� ����° ���� �밢���� �ƴ϶����̰�,
				//�ƴ϶�� �ι�° ���� ����°���� �̿��� ���͸� �������Ѵ�.
				auto V2 = Float4Add(allpoint[2].Pos, contactpoint[0].Pos, false);
				V2 = Float4Normalize(V2);
				float Dot2 = V1.x*V2.x + V1.y*V2.y + V1.z*V2.z;

				if (fabsf(Dot2) <= MMPE_EPSILON)
					Dot2 = 0;

				//ù��° ���� ����°���� ���� �ʴ°�� �ι�° ���� ����°������ �����.
				if (Dot2 != 0)
				{

					V2 = Float4Add(allpoint[2].Pos, contactpoint[1].Pos, false);
					V2 = Float4Normalize(V2);
				}
				//���� ���� �Դٴ°���, 4���� ���� ���� �� �ִ� �༮���� ã�� ���̴�.
				//�ٸ� �ٷ� �����ϸ� �ȵǰ�, �̰� ������� ���� ���� �� ���� �ȴ�.
				//�׷� �� ������ �����ΰ�??
				/*
				��  1)(V2)
				�� /
				��/
				�ѤѤ�2)(V3)

				���� V2�� �ٷ� 1) �̴�.
				�̰� ����� ��ְ� ������ 2) �� ���ͷ� ����ǵ�
				1)�� 2) ������ ��ٰ� ������ ���� �� 5~10 �� ���� �Ǹ� �׸�ŭ ȸ�� ��Ű�� ���̴�!
				�׷��� ���ؼ� 2)�� ���ؾ��Ѵ�. 2)�� ����� ����� A ���� V2�� B�� �ϰ� ProjAB �Ѱ���� V2�� ������ V2�� ���ȭ �ϸ� �ȴ�.
				*/

				float NdotV2 = Normal.x*V2.x + Normal.y*V2.y + Normal.z*V2.z;
				XMFLOAT3 ProjAB = Normal;
				ProjAB.x *= NdotV2;
				ProjAB.y *= NdotV2;
				ProjAB.z *= NdotV2;

				auto V3 = Float4Add(V2, XMFloat3to4(ProjAB), false);
				V3 = Float4Normalize(V3);
				//���� V2�� V3�� �ƴϱ� V2�� V3�� ���հ��� ���Ѵ��� V2���� V3�� �����ؼ� Axis�� ���Ѵ�.

				//���� ���հ����� ���Ѵ�.

				//�������� float���� �ߴµ�, 0�̾ƴϾ�� �ϴµ� 0�̳����°�찡 ����..


				auto tempdot = V2.x*V3.x + V2.y*V3.y + V2.z*V3.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//������ ����.  
				double theta = acos(tempdot);


				CollisionPoint fp;//��ݷ��� ���� ����
				fp.Pos = XMFLOAT4(0, 0, 0, 0);
				fp.pAxis = Normal;

				for (auto i : contactpoint)
				{
					fp.Pos = Float4Add(fp.Pos, i.Pos);
					fp.penetration += i.penetration;
				}

				fp.Pos.x /= contactpoint.size();
				fp.Pos.y /= contactpoint.size();
				fp.Pos.z /= contactpoint.size();

				fp.penetration /= contactpoint.size();

				float impurse = obj->rb->CalculateImpulse(fp, NULL, 1);

				//�ִ����޽��� ���Ѵ�.
				if (impurse > obj->rb->GetMaxImpurse())
					impurse = obj->rb->GetMaxImpurse();


				//�ּ� ���޽��� ���Ѵ�.
				if (impurse < obj->rb->GetMinImpurse())
					impurse = obj->rb->GetMinImpurse();


				//���� ���հ��� Ư������ ���ϸ� ������Ų��. 
				if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && abs(impurse) < obj->rb->GetMaxImpurse() && obj->rb->AmendTime <= 0)//�뷫 5�� ���ϸ� ������Ŵ.
				{
					//ȸ������ ���ϰ�
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
					mAxis = Float3Normalize(mAxis);
					//������ ��Ų��.
					AmendObject(mAxis, theta, obj);

					//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
					//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
					obj->rb->SetAngularVelocity(0, 0, 0);
					obj->rb->SetAccel(0, 0, 0);


				}
				else//�ƴ϶�� ���� ��ݷ��� ������ ������ ���Ѵ�!
				{
					//���⿡ �Դٴ°��� ���̻� ������ ���� �ʰų� ������ ���� ���ʿ䰡 ���� ����.

					//���� ���� �浹���� 2���̴� 2���� �浹���� ���� �� 2�� �����ָ� �浹���� �ϼ��̴�.
					//��ݷ��� ���Ŀ� �ٽ� ���ؾ߰�����, ����� 0,50,0 ������ ���ϰڴ�.

					//�ٵ� ���⼭ ����� ���� �����. �˴ٽ��� �밢�����·� ��Ȯ�ϰ� 45�� �������� ���ִ� ����Ǽ��� �����.
					//�̰��� ���ַ��� ���� �ش����� ���� ���ϴ����� �˻��ؾ��Ѵ�.

					//���� �� ����Ǽ��� �ذ��ؾ߰ڴ�. �˻縦 �Ϸ��� �¿� ��Ī������ ����ɰͰ���.
					//�׸��� ��Ī�̸� �����̵� �ƴϸ� ���������� ��ݷ��� �ణ ���ϴ� �ϸ� �ɰͰ���.

					//���� 2/3/4/5 �ε����� ���� ���ϸ鼭 �� ���̰� 0.0001���� ���̸� ���.
					float ComparePenetration = allpoint[2].penetration;
					bool pass = true;
					for (int i = 2; i < 6; i++)
					{
						if (fabsf(ComparePenetration - allpoint[i].penetration) > 0.25)
						{
							pass = false;
							break;
						}
					}



					//4���� ��� ���� ���̸� ������ �̷�°��̹Ƿ� ������ �μŹ�����.
					if (pass)
					{
						theta = -MMPE_PI / 18;
						XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
						mAxis = Float3Normalize(mAxis);
						//������ ��Ų��.
						AmendObject(mAxis, theta, obj);

						//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
						//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
						obj->rb->SetAngularVelocity(0, 0, 0);


						return;
					}



					//Jm = J/M

					//���޽��� ������ ������.  ��. �Ϲ����� 1:1 ���迡�� 1.3: 0.3 ������ �д�.

					if (obj->rb->AmendTime <= 0)
					{
						auto ratioImpurse = impurse * 0.3;

						auto Jm = Normal;



						Jm.x *= obj->rb->GetMass()*(impurse + ratioImpurse);
						Jm.y *= obj->rb->GetMass()*(impurse + ratioImpurse);
						Jm.z *= obj->rb->GetMass()*(impurse + ratioImpurse);



						//���ӵ� ���
						//W = ���� ���ӵ� + ((Q-P)Ximpurse)*InverseI
						auto W = obj->rb->GetAngularVelocity();
						XMVECTOR rxi = XMLoadFloat3(&XMFloat4to3(Float4Add(fp.Pos, obj->CenterPos, false)));
						rxi = XMVector3Cross(rxi, XMLoadFloat3(&Normal));
						rxi *= (ratioImpurse);
						rxi = XMVector3Transform(rxi, XMLoadFloat4x4(&obj->rb->GetIMoment()));

						XMFLOAT3 ia;
						XMStoreFloat3(&ia, rxi);

						W = Float3Add(W, ia);
						W = Float3Float(W, obj->rb->GetE() / 2);
						XMFLOAT3 lastvel = obj->rb->GetVelocity();
						obj->rb->SetVelocity(Float3Float(Float3Add(lastvel, Jm), obj->rb->GetE()));
						obj->rb->SetAngularVelocity(W);

					}
					//���� �ӵ��� ���ӵ��� ���� ������, ������ �κ� �ؼ�
					//���� �������� penetration(���� ����)��ŭ �ø��� �ȴ�.
					auto px = fabsf(contactpoint[0].penetration)*Normal.x;
					auto py = fabsf(contactpoint[0].penetration)*Normal.y;
					auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
					obj->CenterPos.x += px;
					obj->CenterPos.y += py;
					obj->CenterPos.z += pz;





				}




			}
			else//Dot�� ��� �߶վ��� �� �ϰ�� �� ��ְ� (�ι�°��-ù��°��) ���Ͱ� ���°�� 
			{

				XMFLOAT3 ProjAB = Normal;
				ProjAB.x *= Dot;
				ProjAB.y *= Dot;
				ProjAB.z *= Dot;

				//�����Ǿ����� V1
				auto V4 = Float4Add(V1, XMFloat3to4(ProjAB), false);
				V4 = Float4Normalize(V4);

				//������ ����.


				//�������� float���� �ߴµ�, 0�̾ƴϾ�� �ϴµ� 0�̳����°�찡 ����..


				auto tempdot = V1.x*V4.x + V1.y*V4.y + V1.z*V4.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//������ ����.  
				double theta = acos(tempdot);

				if (abs(theta) != 0 && obj->rb->AmendTime <= 0)
				{

					//ȸ������ ���ϰ�
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V4));
					mAxis = Float3Normalize(mAxis);


					//������ ��Ų��.
					AmendObject(mAxis, theta, obj);
					obj->rb->SetAngularVelocity(0, 0, 0);

				}






			}


		}
		else if (size == 4)
		{


			//�ӽ��������� �׳� �ϴ� penetration�� �ӵ��� ��������.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->CenterPos.x += px;
			obj->CenterPos.y += py;
			obj->CenterPos.z += pz;

			//���� ������� ���� �ӵ��� y�� �ݰ��Ǿ�� �Ѵ�. ������ ź������� ������.. �׳� ���� ���ҽ�Ų�� ��ȣ�� -�� ����.
			auto d = obj->rb->GetVelocity();
			d.y = -0.01 * d.y;
			obj->rb->SetVelocity(d);
			obj->rb->SetAccel(0, 0, 0);


		}
		else if (size != 0)//�� ��
		{
			//�ӽ��������� �׳� �ϴ� penetration�� �ӵ��� ��������.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->CenterPos.x += px;
			obj->CenterPos.y += py;
			obj->CenterPos.z += pz;

			//���� ������� ���� �ӵ��� y�� �ݰ��Ǿ�� �Ѵ�. ������ ź������� ������.. �׳� ���� ���ҽ�Ų�� ��ȣ�� -�� ����.
			auto d = obj->rb->GetVelocity();
			d.y = -0.01 * d.y;
			obj->rb->SetVelocity(d);
			obj->rb->SetAccel(0, 0, 0);

		}

		//==================================== �浹 �� ������ ��Ȳ�� �����ϱ� ���� ó���� ============================================//

		//�ʹ� ������ġ�� ������ �ӵ��� 0����
		if (obj->CenterPos.y < -200)
			obj->rb->SetVelocity(0, 0, 0);


		// ���� ������ �ϰų� �Ѵ��� ������ �κ��� ���ذ�.

		XMFLOAT4 tarr[8];
		obj->rb->GetEightPoint(tarr, obj->GetUpvector(), obj->Lookvector, obj->Rightvector);//���� 8 ���� ���� �����´�.
		std::vector<CollisionPoint> tallpoint;
		std::vector<CollisionPoint> ttempcollisionpoint;
		std::vector<CollisionPoint> tcontactpoint;



		for (int i = 0; i < 8; i++)
		{
			float temppenetration = tarr[i].x*Normal.x + tarr[i].y*Normal.y + tarr[i].z*Normal.z;
			//�浹 ���� �����Ѵ��� �����Ѵ�.
			CollisionPoint cp;
			cp.Pos = tarr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			tallpoint.push_back(cp);
		}

		//penetration�� ���� ������ ����
		sort(tallpoint.begin(), tallpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
		{
			return a.penetration < b.penetration;
		});

		//������ ���ĵ� �༮�� 1�������� �浹�� �� �༮���� tempcollisionpoint�� �����Ѵ�.
		for (auto i : tallpoint)
		{
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0���ϸ� ������ �浹��
				ttempcollisionpoint.push_back(i);

		}

		if (ttempcollisionpoint.size() > 0)
		{
			auto px = fabsf(ttempcollisionpoint[0].penetration)*Normal.x;
			auto py = fabsf(ttempcollisionpoint[0].penetration)*Normal.y;
			auto pz = fabsf(ttempcollisionpoint[0].penetration)*Normal.z;
			obj->CenterPos.x += px;
			obj->CenterPos.y += py;
			obj->CenterPos.z += pz;

		}


		allpoint.clear();
		tempcollisionpoint.clear();
		contactpoint.clear();
	}
}




//void MainFrameWork::RigidBodyCollisionPlane(XMFLOAT3 & Normal, float distance, CGameObject * obj)
//{
//
//
//	if (obj->rb != NULL)
//	{
//		if (obj->rb->AmendTime > 0)
//			obj->rb->AmendTime -= mTimer.DeltaTime();
//		else
//			obj->rb->AmendTime = 0;
//
//		XMFLOAT4 arr[8];
//		obj->rb->GetEightPoint(arr, obj->GetUpvector(), obj->Lookvector, obj->Rightvector);//���� 8 ���� ���� �����´�.
//		std::vector<CollisionPoint> allpoint;
//		std::vector<CollisionPoint> tempcollisionpoint;
//		std::vector<CollisionPoint> contactpoint;
//
//
//
//		for (int i = 0; i < 8; i++)
//		{
//			float temppenetration = arr[i].x*Normal.x + arr[i].y*Normal.y + arr[i].z*Normal.z;
//			//�浹 ���� �����Ѵ��� �����Ѵ�.
//			CollisionPoint cp;
//			cp.Pos = arr[i];
//			cp.penetration = temppenetration;
//			cp.pAxis = Normal;
//			allpoint.push_back(cp);
//		}
//		// 5) ���� ó����
//		//penetration�� ���� ������ ����
//		sort(allpoint.begin(), allpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
//		{
//			return a.penetration < b.penetration;
//		});
//
//		// 6) ���� ó����.
//		//������ ���ĵ� �༮�� 1�������� �浹�� �� �༮���� tempcollisionpoint�� �����Ѵ�.
//		for (auto i : allpoint)
//		{
//			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
//			if (temppenetration < 0)//0���ϸ� ������ �浹��
//				tempcollisionpoint.push_back(i);
//
//		}
//		//���� ���� ���� �༮�� �������� �浹����Ʈ�� ���鼭 penetration�� ������ ����� ����� ������
//		//�̳༮�� �о�ų� �������Ҷ� ���� �� �ִ� �浹���̴�. �� �͵��� �������� �浹���̴�.
//
//		// 7) ���� ó����
//		for (auto i : tempcollisionpoint)
//		{
//			//�����浹�� ù��° ���� - penetration���� ���ؼ� Epsilon�̸� ���� ���� �������� ����Ǵ� ���� �ִ´�.
//			if (fabsf(tempcollisionpoint.front().penetration - i.penetration) <= 0.12)//���� 0.1������ ������ �浹 ���ɼ��� �ִٰ� �Ǵ��Ѵ�.
//			{
//				contactpoint.push_back(i);
//			}
//
//		}
//
//		//------------- ���Ⱑ ���� �߿��� �б� -----------------//
//
//		//------------- size �� 1/2/4/�׿� �϶� �� ������ ó�� ------------------//
//		int size = contactpoint.size();
//		if (size == 1)
//		{
//			//size�� 1�̸� ���ؾ��ұ�? �ٷ� ���� 2���� �ºε������ֵ��� ȸ�����Ѿ��Ѵ�.
//			//�׷��� ���ؼ��� ������ ��ġ�� ��ݷ��� ���Ѵ�.
//			//������ �̰� ��ȸ���� �Ͼ �� �ִ�.
//			//���� ������ ȸ�������� �Ǹ� �� ������ �����Ѵ�. �ٸ� �ش� ��ݷ��� �ʹ� ũ�� �������� �����Ѵ�.
//
//			//2������ - 1�������� �ؼ� 2�������� ���ϴ� ���͸� �����.
//			//�̶� 2���� ���� ���ϴ� ������ Ư������ ���ϸ� �� ������ �����Ѵ�.
//
//			//������ 2�������� ��������� �Ұ��ΰ� �̴�. ����Ǽ��� 2������. �����̳� �������� / �Ǵ� �밢�� ��.
//
//			auto V1 = Float4Add(allpoint[1].Pos, contactpoint[0].Pos, false);
//			V1 = Float4Normalize(V1);
//
//			float NdotV1 = Normal.x*V1.x + Normal.y*V1.y + Normal.z*V1.z;
//
//			XMFLOAT3 ProjAB = Normal;
//			ProjAB.x *= NdotV1;
//			ProjAB.y *= NdotV1;
//			ProjAB.z *= NdotV1;
//
//			auto V2 = Float4Add(V1, XMFloat3to4(ProjAB), false);
//			V2 = Float4Normalize(V2);
//
//
//			//���� ���հ����� ���Ѵ�.
//			//�������� float���� �ߴµ�, 0�̾ƴϾ�� �ϴµ� 0�̳����°�찡 ����..
//
//
//			auto tempdot = V1.x*V2.x + V1.y*V2.y + V1.z*V2.z;
//			if (abs(tempdot) > 1)
//			{
//				if (tempdot > 0)
//					tempdot = 1;
//				else
//					tempdot = -1;
//			}
//			//������ ����.  
//			double theta = acos(tempdot);
//
//
//			//��ݷ��� ����. ��ݷ��� Ư�� �� �����϶��� ��������.
//
//			CollisionPoint fp;//��ݷ��� ���� ����
//			fp.Pos = XMFLOAT4(0, 0, 0, 0);
//			fp.pAxis = Normal;
//
//			fp.Pos = contactpoint[0].Pos;
//			fp.penetration = contactpoint[0].penetration;
//			float impurse = obj->rb->CalculateImpulse(fp, NULL, 1);
//
//			//�ִ����޽��� ���Ѵ�.
//			if (fabsf(impurse) > 500)
//				impurse = 500;
//
//
//			//�ּ� ���޽��� ���Ѵ�.
//			if (fabsf(impurse) < 65)
//				impurse = 65;
//
//
//
//
//			//���� ���հ��� Ư������ ���ϸ� ������Ų��. 
//			//�� �̰� double�� �ص� 0�̾ƴѵ� 0�̳����°�찡 �����.
//			//���� 0�ϰ�� �׳� ��ݷ��� ���ؼ� ������ �����Ų��.
//			if (abs(theta) <= MMPE_PI / 26 && abs(theta) != 0 && abs(impurse) <= 200&& obj->rb->AmendTime <= 0)//�뷫 5�� ���ϸ� ������Ŵ.
//			{
//				//ȸ������ ���ϰ�..
//				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V2));
//				mAxis = Float3Normalize(mAxis);
//				//������ ��Ų��.
//				AmendObject(mAxis, theta, obj);
//
//				//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
//				//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
//				obj->rb->SetAngularVelocity(0, 0, 0);
//
//
//			}
//			else//�ƴ϶�� ���� ��ݷ��� ������ ������ ���Ѵ�!
//			{
//				//���⿡ �Դٴ°��� ���̻� ������ ���� �ʰų� ������ ���� ���ʿ䰡 ���� ����.
//
//
//
//				//���� ���⼭ ���ӵ��� ���� ���� �κ��� ������
//				auto d = obj->rb->GetVelocity();
//				//d.x = -obj->rb->GetE() *d.x;
//				d.y = -obj->rb->GetE() * d.y;
//				//d.z = -obj->rb->GetE() *d.z;
//				obj->rb->SetVelocity(d);
//
//
//				//��ݷ��� ����. impurse = impurse��ŭ 0.01�ʵ��� ���Ѱ�. �ð��� �۰Ե� ������ ���� ���̱� ���ؼ�.
//				XMFLOAT3 impurseV = Normal;
//				impurseV.x *= impurse;
//				impurseV.y *= impurse;
//				impurseV.z *= impurse;
//				obj->rb->AddForcePoint(impurseV, fp.Pos);
//				obj->rb->integrate(0.01);
//
//
//				//���� �ӵ��� ���ӵ��� ���� ������, ������ �κ� �ؼ�
//				//���� �������� penetration(���� ����)��ŭ �ø��� �ȴ�.
//				auto px = fabsf(contactpoint[0].penetration)*Normal.x;
//				auto py = fabsf(contactpoint[0].penetration)*Normal.y;
//				auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
//				obj->CenterPos.x += px;
//				obj->CenterPos.y += py;
//				obj->CenterPos.z += pz;
//
//
//
//
//
//			}
//
//
//		}
//		else if (size == 2)
//		{
//			//�߶վ��� �� 2���� ������ �ؾ��Ѵ�.
//			//������ �ؾ��ϴ��� �˻縦 �ϱ� ���� ����� ��ְ� ������ �ΰ��� ���� ���Ͱ� ������ 0�� �������� �˻��Ѵ�.
//			auto V1 = Float4Add(contactpoint[1].Pos, contactpoint[0].Pos, false);
//			V1 = Float4Normalize(V1);
//
//			float Dot = V1.x*Normal.x + V1.y*Normal.y + V1.z*Normal.z;
//
//			if (fabsf(Dot) <= MMPE_EPSILON)
//				Dot = 0;
//			//����� 0�̸� ������ �ʿ䰡 ����.
//			if (Dot == 0)
//			{
//				//���� 2���� �浹�� 4���� ���浹�� �ٲ� �� �ִ� ������ �� �� �ִ��� �˻��Ѵ�.
//				//�׷��� ���ؼ� 3��° ��- ù��° �� �� �ؼ� 3��° ���� ���ϴ� ���͸� �����.
//				//�� �� �̰� V1�� ������ 0 �� ������ ù��° ���� ����° ���� �밢���� �ƴ϶����̰�,
//				//�ƴ϶�� �ι�° ���� ����°���� �̿��� ���͸� �������Ѵ�.
//				auto V2 = Float4Add(allpoint[2].Pos, contactpoint[0].Pos, false);
//				V2 = Float4Normalize(V2);
//				float Dot2 = V1.x*V2.x + V1.y*V2.y + V1.z*V2.z;
//
//				if (fabsf(Dot2) <= MMPE_EPSILON)
//					Dot2 = 0;
//
//				//ù��° ���� ����°���� ���� �ʴ°�� �ι�° ���� ����°������ �����.
//				if (Dot2 != 0)
//				{
//
//					V2 = Float4Add(allpoint[2].Pos, contactpoint[1].Pos, false);
//					V2 = Float4Normalize(V2);
//				}
//				//���� ���� �Դٴ°���, 4���� ���� ���� �� �ִ� �༮���� ã�� ���̴�.
//				//�ٸ� �ٷ� �����ϸ� �ȵǰ�, �̰� ������� ���� ���� �� ���� �ȴ�.
//				//�׷� �� ������ �����ΰ�??
//				/*
//				��  1)(V2)
//				�� /
//				��/
//				�ѤѤ�2)(V3)
//
//				���� V2�� �ٷ� 1) �̴�.
//				�̰� ����� ��ְ� ������ 2) �� ���ͷ� ����ǵ�
//				1)�� 2) ������ ��ٰ� ������ ���� �� 5~10 �� ���� �Ǹ� �׸�ŭ ȸ�� ��Ű�� ���̴�!
//				�׷��� ���ؼ� 2)�� ���ؾ��Ѵ�. 2)�� ����� ����� A ���� V2�� B�� �ϰ� ProjAB �Ѱ���� V2�� ������ V2�� ���ȭ �ϸ� �ȴ�.
//				*/
//
//				float NdotV2 = Normal.x*V2.x + Normal.y*V2.y + Normal.z*V2.z;
//				XMFLOAT3 ProjAB = Normal;
//				ProjAB.x *= NdotV2;
//				ProjAB.y *= NdotV2;
//				ProjAB.z *= NdotV2;
//
//				auto V3 = Float4Add(V2, XMFloat3to4(ProjAB), false);
//				V3 = Float4Normalize(V3);
//				//���� V2�� V3�� �ƴϱ� V2�� V3�� ���հ��� ���Ѵ��� V2���� V3�� �����ؼ� Axis�� ���Ѵ�.
//
//				//���� ���հ����� ���Ѵ�.
//
//				//�������� float���� �ߴµ�, 0�̾ƴϾ�� �ϴµ� 0�̳����°�찡 ����..
//
//
//				auto tempdot = V2.x*V3.x + V2.y*V3.y + V2.z*V3.z;
//				if (abs(tempdot) > 1)
//				{
//					if (tempdot > 0)
//						tempdot = 1;
//					else
//						tempdot = -1;
//				}
//				//������ ����.  
//				double theta = acos(tempdot);
//
//
//				CollisionPoint fp;//��ݷ��� ���� ����
//				fp.Pos = XMFLOAT4(0, 0, 0, 0);
//				fp.pAxis = Normal;
//
//				for (auto i : contactpoint)
//				{
//					fp.Pos = Float4Add(fp.Pos, i.Pos);
//					fp.penetration += i.penetration;
//				}
//
//				fp.Pos.x /= contactpoint.size();
//				fp.Pos.y /= contactpoint.size();
//				fp.Pos.z /= contactpoint.size();
//
//				fp.penetration /= contactpoint.size();
//
//				float impurse = obj->rb->CalculateImpulse(fp, NULL, 1);
//
//				//�ִ����޽��� ���Ѵ�.
//				if (fabsf(impurse) > 500)
//					impurse = 500;
//
//				//�ּ� ���޽��� ���Ѵ�.
//				if (fabsf(impurse) < 65)
//					impurse = 65;
//
//
//				//���� ���հ��� Ư������ ���ϸ� ������Ų��. 
//				if (abs(theta) <= MMPE_PI / 18 && abs(theta) != 0 && abs(impurse) <= 300 && obj->rb->AmendTime <= 0)//�뷫 5�� ���ϸ� ������Ŵ.
//				{
//					//ȸ������ ���ϰ�
//					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
//					mAxis = Float3Normalize(mAxis);
//					//������ ��Ų��.
//					AmendObject(mAxis, theta, obj);
//
//					//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
//					//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
//					obj->rb->SetAngularVelocity(0, 0, 0);
//					obj->rb->SetAccel(0, 0, 0);
//
//
//				}
//				else//�ƴ϶�� ���� ��ݷ��� ������ ������ ���Ѵ�!
//				{
//					//���⿡ �Դٴ°��� ���̻� ������ ���� �ʰų� ������ ���� ���ʿ䰡 ���� ����.
//
//					//���� ���� �浹���� 2���̴� 2���� �浹���� ���� �� 2�� �����ָ� �浹���� �ϼ��̴�.
//					//��ݷ��� ���Ŀ� �ٽ� ���ؾ߰�����, ����� 0,50,0 ������ ���ϰڴ�.
//
//					//�ٵ� ���⼭ ����� ���� �����. �˴ٽ��� �밢�����·� ��Ȯ�ϰ� 45�� �������� ���ִ� ����Ǽ��� �����.
//					//�̰��� ���ַ��� ���� �ش����� ���� ���ϴ����� �˻��ؾ��Ѵ�.
//
//					//���� �� ����Ǽ��� �ذ��ؾ߰ڴ�. �˻縦 �Ϸ��� �¿� ��Ī������ ����ɰͰ���.
//					//�׸��� ��Ī�̸� �����̵� �ƴϸ� ���������� ��ݷ��� �ణ ���ϴ� �ϸ� �ɰͰ���.
//
//					//���� 2/3/4/5 �ε����� ���� ���ϸ鼭 �� ���̰� 0.0001���� ���̸� ���.
//					float ComparePenetration = allpoint[2].penetration;
//					bool pass = true;
//					for (int i = 2; i < 6; i++)
//					{
//						if (fabsf(ComparePenetration - allpoint[i].penetration) > 0.25)
//						{
//							pass = false;
//							break;
//						}
//					}
//
//
//
//					//4���� ��� ���� ���̸� ������ �̷�°��̹Ƿ� ������ �μŹ�����.
//					if (pass)
//					{
//						theta = -MMPE_PI / 18;
//						XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
//						mAxis = Float3Normalize(mAxis);
//						//������ ��Ų��.
//						AmendObject(mAxis, theta, obj);
//
//						//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
//						//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
//						obj->rb->SetAngularVelocity(0, 0, 0);
//
//
//						return;
//					}
//
//
//
//					//���� ���⼭ ���ӵ��� ���� ���� �κ��� ������
//					auto d = obj->rb->GetVelocity();
//					//d.x = -obj->rb->GetE() *d.x;
//					d.y = -obj->rb->GetE() * d.y;
//					//d.z = -obj->rb->GetE() *d.z;
//					obj->rb->SetVelocity(d);
//
//
//					//��ݷ��� ����. impurse = impurse��ŭ 0.01�ʵ��� ���Ѱ�. �ð��� �۰Ե� ������ ���� ���̱� ���ؼ�.
//					XMFLOAT3 impurseV = Normal;
//					impurseV.x *= impurse;
//					impurseV.y *= impurse;
//					impurseV.z *= impurse;
//					obj->rb->AddForcePoint(impurseV, fp.Pos);
//					obj->rb->integrate(0.01);
//
//					//���� �ӵ��� ���ӵ��� ���� ������, ������ �κ� �ؼ�
//					//���� �������� penetration(���� ����)��ŭ �ø��� �ȴ�.
//					auto px = fabsf(contactpoint[0].penetration)*Normal.x;
//					auto py = fabsf(contactpoint[0].penetration)*Normal.y;
//					auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
//					obj->CenterPos.x += px;
//					obj->CenterPos.y += py;
//					obj->CenterPos.z += pz;
//
//
//
//
//
//				}
//
//
//
//
//			}
//			else//Dot�� ��� �߶վ��� �� �ϰ�� �� ��ְ� (�ι�°��-ù��°��) ���Ͱ� ���°�� 
//			{
//
//				XMFLOAT3 ProjAB = Normal;
//				ProjAB.x *= Dot;
//				ProjAB.y *= Dot;
//				ProjAB.z *= Dot;
//
//				//�����Ǿ����� V1
//				auto V4 = Float4Add(V1, XMFloat3to4(ProjAB), false);
//				V4 = Float4Normalize(V4);
//
//				//������ ����.
//
//
//				//�������� float���� �ߴµ�, 0�̾ƴϾ�� �ϴµ� 0�̳����°�찡 ����..
//
//
//				auto tempdot = V1.x*V4.x + V1.y*V4.y + V1.z*V4.z;
//				if (abs(tempdot) > 1)
//				{
//					if (tempdot > 0)
//						tempdot = 1;
//					else
//						tempdot = -1;
//				}
//				//������ ����.  
//				double theta = acos(tempdot);
//
//				if (abs(theta) != 0 && obj->rb->AmendTime <= 0)
//				{
//
//					//ȸ������ ���ϰ�
//					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V4));
//					mAxis = Float3Normalize(mAxis);
//
//
//					//������ ��Ų��.
//					AmendObject(mAxis, theta, obj);
//				}
//
//
//
//
//
//
//			}
//
//
//		}
//		else if (size == 4)
//		{
//
//
//			//�ӽ��������� �׳� �ϴ� penetration�� �ӵ��� ��������.
//			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
//			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
//			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
//			obj->CenterPos.x += px;
//			obj->CenterPos.y += py;
//			obj->CenterPos.z += pz;
//
//			//���� ������� ���� �ӵ��� y�� �ݰ��Ǿ�� �Ѵ�. ������ ź������� ������.. �׳� ���� ���ҽ�Ų�� ��ȣ�� -�� ����.
//			auto d = obj->rb->GetVelocity();
//			d.y = -0.7 * d.y;
//			obj->rb->SetVelocity(d);
//			obj->rb->SetAccel(0, 0, 0);
//
//
//		}
//		else if (size != 0)//�� ��
//		{
//			//�ӽ��������� �׳� �ϴ� penetration�� �ӵ��� ��������.
//			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
//			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
//			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
//			obj->CenterPos.x += px;
//			obj->CenterPos.y += py;
//			obj->CenterPos.z += pz;
//
//			//���� ������� ���� �ӵ��� y�� �ݰ��Ǿ�� �Ѵ�. ������ ź������� ������.. �׳� ���� ���ҽ�Ų�� ��ȣ�� -�� ����.
//			auto d = obj->rb->GetVelocity();
//			d.y = -0.2 * d.y;
//			obj->rb->SetVelocity(d);
//			obj->rb->SetAccel(0, 0, 0);
//
//		}
//
//		//==================================== �浹 �� ������ ��Ȳ�� �����ϱ� ���� ó���� ============================================//
//
//		//�ʹ� ������ġ�� ������ �ӵ��� 0����
//		if (obj->CenterPos.y < -200)
//			obj->rb->SetVelocity(0, 0, 0);
//
//
//		// ���� ������ �ϰų� �Ѵ��� ������ �κ��� ���ذ�.
//
//		XMFLOAT4 tarr[8];
//		obj->rb->GetEightPoint(tarr, obj->GetUpvector(), obj->Lookvector, obj->Rightvector);//���� 8 ���� ���� �����´�.
//		std::vector<CollisionPoint> tallpoint;
//		std::vector<CollisionPoint> ttempcollisionpoint;
//		std::vector<CollisionPoint> tcontactpoint;
//
//
//
//		for (int i = 0; i < 8; i++)
//		{
//			float temppenetration = tarr[i].x*Normal.x + tarr[i].y*Normal.y + tarr[i].z*Normal.z;
//			//�浹 ���� �����Ѵ��� �����Ѵ�.
//			CollisionPoint cp;
//			cp.Pos = tarr[i];
//			cp.penetration = temppenetration;
//			cp.pAxis = Normal;
//			tallpoint.push_back(cp);
//		}
//
//		//penetration�� ���� ������ ����
//		sort(tallpoint.begin(), tallpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
//		{
//			return a.penetration < b.penetration;
//		});
//
//		//������ ���ĵ� �༮�� 1�������� �浹�� �� �༮���� tempcollisionpoint�� �����Ѵ�.
//		for (auto i : tallpoint)
//		{
//			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
//			if (temppenetration < 0)//0���ϸ� ������ �浹��
//				ttempcollisionpoint.push_back(i);
//
//		}
//
//		if (ttempcollisionpoint.size() > 0)
//		{
//			auto px = fabsf(ttempcollisionpoint[0].penetration)*Normal.x;
//			auto py = fabsf(ttempcollisionpoint[0].penetration)*Normal.y;
//			auto pz = fabsf(ttempcollisionpoint[0].penetration)*Normal.z;
//			obj->CenterPos.x += px;
//			obj->CenterPos.y += py;
//			obj->CenterPos.z += pz;
//
//		}
//
//
//		allpoint.clear();
//		tempcollisionpoint.clear();
//		contactpoint.clear();
//	}
//}

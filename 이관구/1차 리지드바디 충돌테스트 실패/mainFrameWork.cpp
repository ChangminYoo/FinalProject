#include"mainFrameWork.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    try
    {
        MainFrameWork theApp(hInstance);
        if(!theApp.Initialize())
            return 0;

        return theApp.Run();
    }
    catch(DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}

MainFrameWork::MainFrameWork(HINSTANCE hInstance): FrameWork(hInstance) 
{

}

MainFrameWork::~MainFrameWork()
{
}

bool MainFrameWork::Initialize()
{
    if(!FrameWork::Initialize())
		return false;
	
	

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
	//�ݵ�� ����ü�� ������� ��븦 �˻��Ѵ�.
	for (auto i = scene->BulletObject.begin(); i != scene->BulletObject.end(); i++)
	{
		//FindCollisionObject �Լ��� ���߿� ��������. ����� �׳� DynamicObject�� �ش�.
		//�ش� ��ü�� �浹ó�� �˻縦 �� ����� ������.
		//�� ����� ������ �浹�˻縦 �ϵ�����.

		//����ü������ �˻� X ����ü�� �ݵ�� ���̳��Ϳ�����Ʈ ���� �˻��ؾ���.
		(*i)->Collision(&scene->DynamicObject, gt.DeltaTime());
		(*i)->Collision(&scene->StaticObject, gt.DeltaTime());
	}

	//������ ��ü�� ������ ��� ������Ʈ�� �浹�˻縦 �ϵ��� ��.
	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		//FindCollisionObject �Լ��� ���߿� ��������. ����� �׳� DynamicObject�� �ش�.
		//�ش� ��ü�� �浹ó�� �˻縦 �� ����� ������.
		//�� ����� ������ �浹�˻縦 �ϵ�����.
		
		(*i)->Collision(&scene->DynamicObject, gt.DeltaTime());
		(*i)->Collision(&scene->StaticObject, gt.DeltaTime());

	}

}

void MainFrameWork::System(const GameTimer & gt)
{
	GravitySystem(gt);
}

void MainFrameWork::GravitySystem(const GameTimer & gt)
{
	GeneratorGravity gg;
	gg.SetGravityAccel(XMFLOAT3(0, -100, 0));
	//������ ��ü�� ������ ��������Ʈ�� �߷��� ���Ѵ�. �� ����ü�� �����Ѵ�.
	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		
		gg.Update(gt.DeltaTime(), *(*i)->pp);
	}
	for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
	{

		gg.Update(gt.DeltaTime(), *(*i)->rb);
	}

}

void MainFrameWork::AfterGravitySystem(const GameTimer & gt)
{
	
	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		//�� ���� ������ �ƴ� pp�� �������� ó���ϳĸ� ���������� �������� pp�� ������ �����̳�
		//pp�������� �����̰� ���������� �����̳� ������, UpdatePPosCenterPos�� ��������
		//pp�� �����̰� cp�� pp�� �����.
		float ppy = (*i)->pp->GetPosition().y;
		float hby = (*i)->pp->GetHalfBox().y;
		if (ppy-hby < 0)//pp�� ����y-�����ڽ��� y���� �Ѱ���� 0���� ������ ���Ʒ��� ������
		{
			XMFLOAT3 gp = (*i)->pp->GetPosition();
			gp.y += hby - ppy;//�׷��� �ݴ�� �����ڽ�y��-����y��ŭ �ø��� �ȴ�.
			(*i)->pp->SetPosition(gp);
			(*i)->UpdatePPosCenterPos();
			auto v=(*i)->pp->GetVelocity();
			v.y = 0;//�߷¿� ���� �ӵ��� 0���� ����
			(*i)->pp->SetVelocity(v);
			(*i)->AirBone = false;
		}
	}

	//����ü�� y�� 0���� ������ ���ŵȴ�.
	for (auto i = scene->BulletObject.begin(); i != scene->BulletObject.end(); i++)
	{
		if ((*i)->CenterPos.y <= 0)
			(*i)->DelObj = true;
	}


	for (auto j = scene->StaticObject.begin();j != scene->StaticObject.end();j++)
	{
		
		XMFLOAT4 arr[8];
		(*j)->rb->GetEightPoint(arr, (*j)->GetUpvector(), (*j)->Lookvector, (*j)->Rightvector);//���� 8 ���� ���� �����´�.
		//���� �浹�ߴ��� �˻縦 �ؾ��Ѵ�.
		XMFLOAT3 PlaneNormal{ 0,1,0 };
		vector<CollisionPoint> tempcp;
		for (int i = 0; i < 8; i++)
		{
			float temppenetration = arr[i].x*PlaneNormal.x + arr[i].y*PlaneNormal.y + arr[i].z*PlaneNormal.z;
			if (temppenetration < 0)//����� �ڿ������� �浹�Ѱ�.
			{
				//�浹 ���� �����Ѵ��� �����Ѵ�.
				CollisionPoint cp;
				cp.Pos = arr[i];
				cp.penetration = fabsf(temppenetration);
				cp.pAxis = PlaneNormal;
				tempcp.push_back(cp);
			}

		}

		//����� 0���� ũ�� �浹�� ������ �ִ°�.
		if (tempcp.size() > 0)
		{
			//���� ���� ���� �浹 ���̸� �������� �����Ѵ�.��? ������� �浹�Ѹ�ŭ �о���ϴϱ�.
			sort(tempcp.begin(), tempcp.end(), [](CollisionPoint& a, CollisionPoint& b) {
				return a.penetration > b.penetration;
			});

			float firstpenetration = tempcp.front().penetration;//���� �������� ����.

			//���� ���� ���� �༮�� �������� �浹����Ʈ�� ���鼭 penetration�� ������ ����� ����� ������
			//�̳༮�� �о�ų� �������Ҷ� ���� �� �ִ� �浹���̴�. �� �͵��� �������� �浹���̴�.
			for (auto i : tempcp)
			{
				if (firstpenetration - i.penetration <= MMPE_EPSILON)
				{
					(*j)->rb->CollisionPointVector.push_back(i);
				}
			}
		

			CollisionPoint fp;
			fp.Pos = XMFLOAT4(0, 0, 0, 0);
			fp.pAxis = PlaneNormal;
			for (auto i : (*j)->rb->CollisionPointVector)
			{
				fp.Pos = Float4Add(fp.Pos, i.Pos);
				fp.penetration += i.penetration;
				
			}

			fp.Pos.x /= (*j)->rb->CollisionPointVector.size();
			fp.Pos.y /= (*j)->rb->CollisionPointVector.size();
			fp.Pos.z /= (*j)->rb->CollisionPointVector.size();
			fp.penetration/=  (*j)->rb->CollisionPointVector.size();

			// ���� ����������� �浹������ ó���ؾ��Ұ͵��� �� �����ϰ��ִٴ°��̴�.
			//���� �� �ؾ��ϳ�? 1. �ӵ��� �����Ѵ�. 2. ������ �κ��� �ؼ��Ѵ�.
			
			//�׽�Ʈ ��� ���޽��� ������ ��������, ��Ȯ���� ������..

			
				auto impurse = (*j)->rb->CalculateImpulse(fp);
			

					impurse = XMFLOAT3(0, 0.11, 0);
				
					if ((*j)->rb->CollisionPointVector.size() > 2)
					{
						impurse = XMFLOAT3(0, 0, 0);
						(*j)->rb->SetAngularVelocity(0, 0, 0);
					}
				XMFLOAT3 p = XMFloat4to3(fp.Pos);
				auto p2 = XMFloat4to3((*j)->rb->GetPosition());
				p = Float3Add(p, p2, false);//p-=p2
				XMFLOAT3 t = Float3Cross(p, impurse);
				XMVECTOR torq = XMLoadFloat3(&t);
				XMMATRIX iim = XMLoadFloat4x4(&(*j)->rb->GetIMoment());
				XMFLOAT3 av;
				XMStoreFloat3(&av, XMVector3Transform(torq, iim));//���ӵ�

				(*j)->rb->SetAngularVelocity(av);


				//������ �κ� �ؼ�
				auto px = (*j)->rb->CollisionPointVector.front().penetration*(*j)->rb->CollisionPointVector.front().pAxis.x;
				auto py = (*j)->rb->CollisionPointVector.front().penetration*(*j)->rb->CollisionPointVector.front().pAxis.y;
				auto pz = (*j)->rb->CollisionPointVector.front().penetration*(*j)->rb->CollisionPointVector.front().pAxis.z;
				(*j)->CenterPos.x += px;
				(*j)->CenterPos.y += py;
				(*j)->CenterPos.z += pz;
				auto d = (*j)->rb->GetVelocity();
				d.y = 0;
				(*j)->rb->SetVelocity(d);
				(*j)->rb->SetVelocity(impurse.x*(*j)->rb->GetMass(), impurse.y*(*j)->rb->GetMass(), impurse.z*(*j)->rb->GetMass());

	
				(*j)->rb->integrate(gt.DeltaTime());
				(*j)->rb->CollisionPointVector.clear();
			}
		}

		

		
	
}

void MainFrameWork::OnResize()
{
	FrameWork::OnResize();
}

void MainFrameWork::FrameAdvance(const GameTimer& gt)
{
	//�����Ӿ�庥���� ������ �޸𸮸��º��ʹ�.
	mDirectCmdListAlloc->Reset();
	mCommandList->Reset(mDirectCmdListAlloc.Get(),NULL);
	
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

	Update(gt);
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
	System(gt);
	//��Ŭ������ ������Ʈ�� ȣ���Ѵ�.
	scene->Tick(gt);

	//�߷� ��ó�� �ý����� ƽ�Լ� ���Ŀ� ó���ؾ���
	AfterGravitySystem(gt);
	//�浹 ó�� �ý����� ƽ�Լ� ���Ŀ� ó���ؾ��Ѵ�.
	CollisionSystem(gt);
}

void MainFrameWork::Draw(const GameTimer& gt)
{
	scene->Render(gt);
}

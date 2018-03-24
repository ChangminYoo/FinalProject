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

//충돌 시스템은 반드시 틱함수 이후에 처리되야한다. 왜냐하면 틱함수까지 처리되면 실제로 움직인게 되기 때문이다.
//충돌은 움직인 이후에 발생한다.
//먼저 고정된 객체를 제외한 모든 오브젝트를 검사해야한다.
//요소 하나씩 접근을 하는데, 접근한다음, 해당 오브젝트가 검사해야할 목록을 찾아서 인자로 넣어준다.
//오브젝트는 Collision함수를 호출하는데 아까 인자로 받은 리스트 목록을 주르륵 돌면서
//CollisionTest함수를 호출한다. 이녀석이 실제로 OBB충돌검사를 하는 녀석이다.
//충돌을 했으면 CollsionResolve 함수를 호출해서 충돌을 해소해준다.
//중요한점은 충돌해소후에 반드시! UpdatePPosCenterPos함수를 호출해서 PP의 중점으로 게임오브젝트의 중점을 갱신해야한다.
void MainFrameWork::CollisionSystem(const GameTimer& gt)
{
	//반드시 투사체가 가장먼저 상대를 검사한다.
	for (auto i = scene->BulletObject.begin(); i != scene->BulletObject.end(); i++)
	{
		//FindCollisionObject 함수를 나중에 만들어야함. 현재는 그냥 DynamicObject를 준다.
		//해당 객체가 충돌처리 검사를 할 목록을 저장함.
		//이 목록을 가지고 충돌검사를 하도록함.

		//투사체끼리는 검사 X 투사체는 반드시 다이나믹오브젝트 들을 검사해야함.
		(*i)->Collision(&scene->DynamicObject, gt.DeltaTime());
		(*i)->Collision(&scene->StaticObject, gt.DeltaTime());
	}

	//고정된 객체를 제외한 모든 오브젝트를 충돌검사를 하도록 함.
	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		//FindCollisionObject 함수를 나중에 만들어야함. 현재는 그냥 DynamicObject를 준다.
		//해당 객체가 충돌처리 검사를 할 목록을 저장함.
		//이 목록을 가지고 충돌검사를 하도록함.
		
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
	//고정된 물체를 제외한 모든오브젝트에 중력을 가한다. 단 투사체는 제외한다.
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
		//왜 실제 중점이 아닌 pp의 중점으로 처리하냐면 실제중점을 움직인후 pp의 중점을 움직이나
		//pp의중점을 움직이고 실제중점을 움직이나 같지만, UpdatePPosCenterPos를 쓰기위해
		//pp를 움직이고 cp를 pp로 맞춘다.
		float ppy = (*i)->pp->GetPosition().y;
		float hby = (*i)->pp->GetHalfBox().y;
		if (ppy-hby < 0)//pp의 중점y-하프박스의 y값을 한결과가 0보다 작으면 땅아래에 묻힌셈
		{
			XMFLOAT3 gp = (*i)->pp->GetPosition();
			gp.y += hby - ppy;//그러면 반대로 하프박스y값-중점y만큼 올리면 된다.
			(*i)->pp->SetPosition(gp);
			(*i)->UpdatePPosCenterPos();
			auto v=(*i)->pp->GetVelocity();
			v.y = 0;//중력에 의한 속도를 0으로 만듬
			(*i)->pp->SetVelocity(v);
			(*i)->AirBone = false;
		}
	}

	//투사체는 y가 0보다 작으면 제거된다.
	for (auto i = scene->BulletObject.begin(); i != scene->BulletObject.end(); i++)
	{
		if ((*i)->CenterPos.y <= 0)
			(*i)->DelObj = true;
	}


	for (auto j = scene->StaticObject.begin();j != scene->StaticObject.end();j++)
	{
		
		XMFLOAT4 arr[8];
		(*j)->rb->GetEightPoint(arr, (*j)->GetUpvector(), (*j)->Lookvector, (*j)->Rightvector);//먼저 8 개의 점을 가져온다.
		//점이 충돌했는지 검사를 해야한다.
		XMFLOAT3 PlaneNormal{ 0,1,0 };
		vector<CollisionPoint> tempcp;
		for (int i = 0; i < 8; i++)
		{
			float temppenetration = arr[i].x*PlaneNormal.x + arr[i].y*PlaneNormal.y + arr[i].z*PlaneNormal.z;
			if (temppenetration < 0)//평면의 뒤에있으면 충돌한것.
			{
				//충돌 점을 생성한다음 저장한다.
				CollisionPoint cp;
				cp.Pos = arr[i];
				cp.penetration = fabsf(temppenetration);
				cp.pAxis = PlaneNormal;
				tempcp.push_back(cp);
			}

		}

		//사이즈가 0보다 크면 충돌한 지점이 있는것.
		if (tempcp.size() > 0)
		{
			//이제 가장 깊은 충돌 깊이를 기준으로 정렬한다.왜? 가장깊이 충돌한만큼 밀어내야하니까.
			sort(tempcp.begin(), tempcp.end(), [](CollisionPoint& a, CollisionPoint& b) {
				return a.penetration > b.penetration;
			});

			float firstpenetration = tempcp.front().penetration;//가장 깊은값을 꺼냄.

			//이제 가장 깊은 녀석을 기준으로 충돌리스트를 돌면서 penetration을 뺐을때 결과가 충분히 작으면
			//이녀석도 밀어내거나 힘을가할때 사용될 수 있는 충돌점이다. 이 것들이 최종적인 충돌점이다.
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

			// 이제 여기까지오면 충돌지점을 처리해야할것들을 다 저장하고있다는것이다.
			//이제 뭘 해야하나? 1. 속도를 변경한다. 2. 겹쳐진 부분을 해소한다.
			
			//테스트 결과 임펄스가 작으면 작을수록, 정확도가 높아짐..

			
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
				XMStoreFloat3(&av, XMVector3Transform(torq, iim));//각속도

				(*j)->rb->SetAngularVelocity(av);


				//겹쳐진 부분 해소
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
	//프레임어드벤스의 시작은 메모리리셋부터다.
	mDirectCmdListAlloc->Reset();
	mCommandList->Reset(mDirectCmdListAlloc.Get(),NULL);
	
	//뷰포트 연결
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);



	//현재백버퍼를 렌더타겟으로 변경
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//후면버퍼와 깊이버퍼 초기화
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// 현재 백버퍼를 연결.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	//현재 여기까지 연결된 파이프라인의 구성요소
	// RS 뷰포트와 시저렉트 , OM의 렌더타겟과 뎁스스텐실 뷰
	//앞으로 해야할것. 루트시그니처와 VS,PS등 PSO, 정점버퍼, 인덱스버퍼 , 상수버퍼뷰

	Update(gt);
	Draw(gt);

	//여기까지 왔으면 PSO에 모든게 다 연결되어 있고, 다 그려져있는것이다.
	//리소스 베리어를 읽기용으로 변경한다.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));


	// 커맨드리스트를 닫는다.
	ThrowIfFailed(mCommandList->Close());

	// 커맨드리스트를 커맨드 큐에 집어넣는다.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// 읽기용으로 프리젠트한다
	ThrowIfFailed(SwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	//플러쉬
	FlushCommandQueue();

}

void MainFrameWork::Update(const GameTimer& gt)
{
	//먼저 시스템 클래스를 업데이트 한다. 시스템 클래스는 게임 전반적인것들을 관여함. 중력 등
	System(gt);
	//씬클래스의 업데이트를 호출한다.
	scene->Tick(gt);

	//중력 후처리 시스템은 틱함수 이후에 처리해야함
	AfterGravitySystem(gt);
	//충돌 처리 시스템은 틱함수 이후에 처리해야한다.
	CollisionSystem(gt);
}

void MainFrameWork::Draw(const GameTimer& gt)
{
	scene->Render(gt);
}

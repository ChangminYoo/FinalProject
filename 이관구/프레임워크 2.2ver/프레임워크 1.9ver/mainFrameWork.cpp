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

	}

	//������ ��ü�� ������ ��� ������Ʈ�� �浹�˻縦 �ϵ��� ��.
	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		//FindCollisionObject �Լ��� ���߿� ��������. ����� �׳� DynamicObject�� �ش�.
		//�ش� ��ü�� �浹ó�� �˻縦 �� ����� ������.
		//�� ����� ������ �浹�˻縦 �ϵ�����.
		
		(*i)->Collision(&scene->DynamicObject, gt.DeltaTime());

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
	

}

void MainFrameWork::AfterGravitySystem(const GameTimer & gt)
{
	//�����̾ƴ� ��ü�� ó���Ѵ�
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

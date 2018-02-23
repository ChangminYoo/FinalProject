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
	KeyboardInput(gt);
	//���� �ý��� Ŭ������ ������Ʈ �Ѵ�. �ý��� Ŭ������ ���� �������ΰ͵��� ������. �߷� ��

	//��Ŭ������ ������Ʈ�� ȣ���Ѵ�.
	scene->Tick(gt);
}

void MainFrameWork::Draw(const GameTimer& gt)
{

	scene->Render();
}

void MainFrameWork::KeyboardInput(const GameTimer& gt)
{
	const float dt = gt.DeltaTime();

	if (GetAsyncKeyState('W') & 0x8000)
		scene->camera->Walk(30.0f*dt);

	if (GetAsyncKeyState('S') & 0x8000)
		scene->camera->Walk(-30.0f*dt);

	if (GetAsyncKeyState('A') & 0x8000)
		scene->camera->Strafe(-30.0f*dt);

	if (GetAsyncKeyState('D') & 0x8000)
		scene->camera->Strafe(30.0f*dt);


	scene->camera->UpdateViewMatrix();
}
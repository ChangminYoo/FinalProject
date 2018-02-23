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
	KeyboardInput(gt);
	//먼저 시스템 클래스를 업데이트 한다. 시스템 클래스는 게임 전반적인것들을 관여함. 중력 등

	//씬클래스의 업데이트를 호출한다.
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
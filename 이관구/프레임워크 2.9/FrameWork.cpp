#include "FrameWork.h"
#include <WindowsX.h>
#include"MMPE\MyMiniPysicsEngine.h"
using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

UINT CbvSrvDescriptorSize = 0;
#define MAXRAYLEN 300

LRESULT CALLBACK
GetWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before hwnd is valid.
	return FrameWork::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
}

FrameWork* FrameWork::mApp = nullptr;
FrameWork* FrameWork::GetApp()
{
	return mApp;
}

void FrameWork::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		wstring fpsStr = to_wstring(fps);
		wstring mspfStr = to_wstring(mspf);

		wstring windowText = mGetWndCaption +
			L"    fps: " + fpsStr +
			L"   mspf: " + mspfStr;

		SetWindowText(hwnd, windowText.c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
FrameWork::FrameWork(HINSTANCE hInstance)
	: hinstance(hInstance)
{
	// Only one FrameWork can be constructed.
	assert(mApp == nullptr);
	mApp = this;




}

FrameWork::~FrameWork()
{
	if (scene != NULL)
		delete scene;

	if (Device != nullptr)
		FlushCommandQueue();
}

HINSTANCE FrameWork::GetInst()const
{
	return hinstance;
}

HWND FrameWork::GetWnd()const
{
	return hwnd;
}

float FrameWork::AspectRatio()const
{
	return static_cast<float>(mClientWidth) / mClientHeight;
}

bool FrameWork::Get4xMsaaState()const
{
	return m4xMsaaState;
}

void FrameWork::Set4xMsaaState(bool value)
{
	if (m4xMsaaState != value)
	{
		m4xMsaaState = value;

		// Recreate the swapchain and buffers with new multisample settings.
		CreateSwapChain();
		OnResize();
	}
}

int FrameWork::Run()
{
	MSG msg = { 0 };

	mTimer.Reset();

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{
			mTimer.Tick();

			if (!bAppPaused)
			{
				CalculateFrameStats();
				FrameAdvance(mTimer);
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}

bool FrameWork::Initialize()
{
	if (!InitMainWindow())//���������� �ʱ�ȭ
		return false;

	if (!InitDirect3D())//���̷�Ʈ3d �ʱ�ȭ
		return false;

	OnResize();//ȭ�� ������

	CbvSrvDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	//������ �Ҷ� �̰� �ʱ�ȭ ���������.. �̰Ŷ����� 5�ð������� 
	//�̰Ÿ� �ؾ� Ŀ�ǵ帮��Ʈ�� Ŀ�ǵ�˷������Ͱ� Open���°��Ǽ� ��Ŭ�������ִ� ������Ʈ�� �����¸����
	//ó���� �� �ְԵ�.. �ٽ��ѹ� ��ġ��
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));
	if (scene == NULL)
		scene = new Scene(hwnd,Device.Get(), mCommandList.Get(),mClientWidth,mClientHeight);
	
	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();
	

	return true;
}

void FrameWork::OnResize()
{
	assert(Device);
	assert(SwapChain);
	assert(mDirectCmdListAlloc);


	FlushCommandQueue();

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));


	for (int i = 0; i < SwapChainBufferCount; ++i)
		SwapChainBuffer[i].Reset();
	mDepthStencilBuffer.Reset();

	// Resize the swap chain.
	ThrowIfFailed(SwapChain->ResizeBuffers(
		SwapChainBufferCount,
		mClientWidth, mClientHeight,
		mBackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	mCurrBackBuffer = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SwapChainBufferCount; i++)
	{
		ThrowIfFailed(SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i])));
		Device->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}

	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;

	// Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from 
	// the depth buffer.  Therefore, because we need to create two views to the same resource:
	//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
	//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
	// we need to create the depth buffer resource with a typeless format.  
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	ThrowIfFailed(Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = mDepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	Device->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

	// Transition the resource from its initial state to be used as a depth buffer.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	// Execute the resize commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until resize is complete.
	FlushCommandQueue();

	// Update the viewport transform to cover the client area.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, mClientWidth, mClientHeight };
}

LRESULT FrameWork::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			bAppPaused = true;
			mTimer.Stop();
		}
		else
		{
			bAppPaused = false;
			mTimer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		mClientWidth = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);

	//������ ����� �ٲ� ���� UI�� ��ġ�� �޸��ؾ��Ѵ�. ���� ƽ�Լ��� ���� ������, ���� ��ƽ���� �Ҹ��ʿ�� �����ϱ� �̷��� ó������.
		if (scene != NULL && scene->GetGameState()==GS_PLAY)
		{
			scene->mHeight = mClientHeight;
			scene->mWidth = mClientWidth;

			scene->resize = true;

			if (scene->BackGround != NULL)
			{
				scene->BackGround->ObjData.Scale = mClientWidth;
				scene->BackGround->ObjData.CustomData1.y = mClientHeight;
				
			}
			for(int i=0;i<4;i++)
				if (scene->SkillCoolBar[i] != NULL)
				{
		
					scene->SkillCoolBar[i]->ObjData.Scale = mClientWidth / 10;
					scene->SkillCoolBar[i]->CenterPos.x = i * mClientWidth / 8 - (mClientWidth / 8)*1.5;
					scene->SkillCoolBar[i]->CenterPos.y = 0.98*-mClientHeight / 2;

					scene->SkillUI[i]->ObjData.Scale = mClientWidth / 10;
					scene->SkillUI[i]->CenterPos.x = i * mClientWidth / 8 - (mClientWidth / 8)*1.5;
					scene->SkillUI[i]->CenterPos.y = 0.9*-mClientHeight / 2;
			
				}
		}

		if (Device)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				bAppPaused = true;
				bMinimized = true;
				bMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				bAppPaused = false;
				bMinimized = false;
				bMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (bMinimized)
				{
					bAppPaused = false;
					bMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (bMaximized)
				{
					bAppPaused = false;
					bMaximized = false;
					OnResize();
				}
				else if (bResizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or SwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		bAppPaused = true;
		bResizing = true;
		mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		bAppPaused = false;
		bResizing = false;
		mTimer.Start();
		OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if(scene->GetGameState()==GS_PLAY)
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:

		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		if (scene->GetGameState() == GS_PLAY)
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		else if ((int)wParam == VK_F2)
			Set4xMsaaState(!m4xMsaaState);

		return 0;
	}
	//SetCursorPos(0, 0);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


void FrameWork::OnMouseDown(WPARAM btnState, int x, int y)
{
	
	//���̸� ���. �� �÷��̾� ������Ʈ�� ����ְ�, �ش� ��ų�� ����ִ� true���¶��!
	if (scene->Player->PlayerObject->gamedata.HP > 0 && scene->Player->skilldata.isSkillOn[scene->Player->skilldata.SellectBulletIndex])
	{//���� �ִϸ��̼����� ��ȯ
		scene->Player->PlayerObject->SetAnimation(2);
		auto RAY = MousePicking(x, y, scene->Player->Camera.CamData.EyePos, scene->Player->Camera.CamData.View, scene->Player->Camera.CamData.Proj);
		XMFLOAT3 savepoint;

		bool Shot = false;
		//���⿡ ��� ������Ʈ����Ʈ���� ���ư��鼭 �˻縦 �Ѵ�.
		for (auto b = scene->DynamicObject.begin(); b != scene->DynamicObject.end(); b++)
		{
			if ((*b) != scene->Player->PlayerObject)//�÷��̾ �ƴҰ��
			{
				//������ ���̺��� ����ﶧ
				if (FloatLength(Float4Add(scene->Player->PlayerObject->CenterPos, (*b)->CenterPos, false)) <= MAXRAYLEN)
				{


					if ((*b)->rco.RayCasting(RAY.RayOrgin, RAY.RayDir, XMFloat4to3((*b)->CenterPos), XMFloat4to3(scene->Player->PlayerObject->CenterPos),
						scene->Player->PlayerObject->Lookvector, &savepoint) == true && Shot == false)//������ �ٸ� ������Ʈ�� ������
					{
						//���⼭ ������ ó���Ѵ�.
						//�÷��̾ ���� ����� ����ü �ѹ��� ���� ��ü������ ����ü�� �����ؼ� �ҷ�������Ʈ ����Ʈ�� ������.
						scene->Player->CreateBullet(Device.Get(), mCommandList.Get(), savepoint, (*b), &scene->BulletObject);
						Shot = true;
					}
				}
			}
		}


		for (auto b = scene->StaticObject.begin(); b != scene->StaticObject.end(); b++)
		{
			if ((*b) != scene->Player->PlayerObject)//�÷��̾ �ƴҰ��
			{
				//������ ���̺��� ����ﶧ
				if (FloatLength(Float4Add(scene->Player->PlayerObject->CenterPos, (*b)->CenterPos, false)) <= MAXRAYLEN)
				{


					if ((*b)->rco.RayCasting(RAY.RayOrgin, RAY.RayDir, XMFloat4to3((*b)->CenterPos), XMFloat4to3(scene->Player->PlayerObject->CenterPos),
						scene->Player->PlayerObject->Lookvector, &savepoint) == true && Shot == false)//������ �ٸ� ������Ʈ�� ������
					{
						//���⼭ ������ ó���Ѵ�.
						//�÷��̾ ���� ����� ����ü �ѹ��� ���� ��ü������ ����ü�� �����ؼ� �ҷ�������Ʈ ����Ʈ�� ������.
						scene->Player->CreateBullet(Device.Get(), mCommandList.Get(), savepoint, (*b), &scene->BulletObject);
						Shot = true;
					}


				}
			}

		}
		//������ �ٵ� �˻��ؾ���.
		for (auto b = scene->RigidObject.begin(); b != scene->RigidObject.end(); b++)
		{
			if ((*b) != scene->Player->PlayerObject)//�÷��̾ �ƴҰ��
			{
				//������ ���̺��� ����ﶧ
				if (FloatLength(Float4Add(scene->Player->PlayerObject->CenterPos, (*b)->CenterPos,false)) <= MAXRAYLEN)
				{


					if ((*b)->rco.RayCasting(RAY.RayOrgin, RAY.RayDir, XMFloat4to3((*b)->CenterPos), XMFloat4to3(scene->Player->PlayerObject->CenterPos),
						scene->Player->PlayerObject->Lookvector, &savepoint) == true && Shot == false)//������ �ٸ� ������Ʈ�� ������
					{
						//���⼭ ������ ó���Ѵ�.
						//�÷��̾ ���� ����� ����ü �ѹ��� ���� ��ü������ ����ü�� �����ؼ� �ҷ�������Ʈ ����Ʈ�� ������.
						scene->Player->CreateBullet(Device.Get(), mCommandList.Get(), savepoint, (*b), &scene->BulletObject);
						Shot = true;
					}


				}
			}

		}

		if (Shot == false)//���߽�Ų ���� ������.
		{
			//���� �˻縦 �غ�.
			if (scene->Player->PlayerObject->rco.RayCastingField(RAY.RayOrgin, RAY.RayDir, XMFloat4to3(scene->Player->PlayerObject->CenterPos),
				scene->Player->PlayerObject->Lookvector, &savepoint) == true)
			{
				scene->Player->CreateBullet(Device.Get(), mCommandList.Get(), savepoint,NULL, &scene->BulletObject);
			

				Shot = true;
			}
			else//������Ʈ�� �� ��� �ȸ¾����� ��������.
			{
				savepoint = RayShot(RAY.RayOrgin, RAY.RayDir, MAXRAYLEN);
				scene->Player->CreateBullet(Device.Get(), mCommandList.Get(), savepoint, NULL, &scene->BulletObject);
			}
		}
	}
}

void FrameWork::OnMouseMove(WPARAM btnState, int x, int y)
{
	scene->Player->TPSCameraSystem(x, y,0.01);
	
	RECT rc,rc2;
	POINT p1, p2;

	GetClientRect(hwnd, &rc);

	p1.x = rc.left; p1.y = rc.top; p2.x = rc.right; p2.y = rc.bottom;

	ClientToScreen(hwnd, &p1);
	ClientToScreen(hwnd, &p2);

	rc2.left = p1.x; rc2.top = p1.y; rc2.right = p2.x; rc2.bottom = p2.y;

	//ClipCursor(&rc);


	SetCursorPos((rc2.left + rc2.right) / 2, (rc2.top + rc2.bottom) / 2);
	//�����̰��� �ٽ� ���ڸ��ε��ƿ��ϱ� �������� ���ݤ����ƾƾ�
	scene->Player->ox = (rc.left + rc.right) / 2;
	scene->Player->oy = (rc.top + rc.bottom) / 2;
	ShowCursor(false);



	// ���콺�� �����϶����� ���� �������� ã�ƾ� �Ұ�� ����. ��  �÷��̾�� ���ٴڸ� ���������� ǥ���Ѵ�.
	//�Ϲ������� � ������ ��Ÿ���ų� �Ҷ��� ���ٴ������� �����ϴϱ�. �ٵ� �� ����ƽ ������Ʈ�� �߰��߳ĸ�
	// ť�������Ʈ��  �ʿ��ϴ� ������ �Ǽ���. ���̾� ��� �ö󼭵� ���� ������ �׳� ���ܽ��Ѽ� ������ �ø��°� ������, ť��� ���߿�
	//2�������� ���鶧 �ٽ��̵Ǵ� ��ᰡ �ɰ� ���� �����̴�. ���� �߰�����.

	//�� ���� ���� ���� �������� ��/�÷��̾�/ť�������Ʈ �� 3������ �Ǹ� ������ ������Ʈ�� ��, ������ ������ �ȵ�.

	if(scene->Player->PlayerObject->gamedata.HP > 0 && scene->Player->MouseTrace)
	{
	
		auto RAY = MousePicking(x, y, scene->Player->Camera.CamData.EyePos, scene->Player->Camera.CamData.View, scene->Player->Camera.CamData.Proj);
		XMFLOAT3 savepoint;

		bool Shot = false;
		//���⿡ ��� ������Ʈ����Ʈ���� ���ư��鼭 �˻縦 �Ѵ�.
		for (auto b = scene->DynamicObject.begin(); b != scene->DynamicObject.end(); b++)
		{
			if (Shot == false)
			{

				if ((*b) != scene->Player->PlayerObject)//�÷��̾ �ƴҰ��
				{
					//������ ���̺��� ����ﶧ
					if (FloatLength(Float4Add(scene->Player->PlayerObject->CenterPos, (*b)->CenterPos, false)) <= MAXRAYLEN)
					{


						if ((*b)->rco.RayCasting(RAY.RayOrgin, RAY.RayDir, XMFloat4to3((*b)->CenterPos), XMFloat4to3(scene->Player->PlayerObject->CenterPos),
							scene->Player->PlayerObject->Lookvector, &savepoint) == true && Shot == false)//������ �ٸ� ������Ʈ�� ������
						{
							scene->Player->TraceObject->CenterPos = XMFloat3to4(savepoint);
							
							Shot = true;
						}
					}
				}
			}
		}


		for (auto b = scene->StaticObject.begin(); b != scene->StaticObject.end(); b++)
		{
			
			if (Shot == false)
			{
				if ((*b) != scene->Player->PlayerObject)//�÷��̾ �ƴҰ��
				{

					//������ ���̺��� ����ﶧ
					if (FloatLength(Float4Add(scene->Player->PlayerObject->CenterPos, (*b)->CenterPos, false)) <= MAXRAYLEN)
					{
						
						
						if ((*b)->rco.RayCasting(RAY.RayOrgin, RAY.RayDir, XMFloat4to3((*b)->CenterPos), XMFloat4to3(scene->Player->PlayerObject->CenterPos),
							scene->Player->PlayerObject->Lookvector, &savepoint) == true && Shot == false && (*b)->obs!=Wall&& (*b)->rco.isTopPlane==true)//������ �ٸ� ������Ʈ�� ������
						{
							Shot = true;
							scene->Player->TraceObject->CenterPos = XMFloat3to4(savepoint);
						
						}


					}
				}
			}

		}
		
		if (Shot == false)//���߽�Ų ���� ������.
		{
			//���� �˻縦 �غ�.
			if (scene->Player->PlayerObject->rco.RayCastingField(RAY.RayOrgin, RAY.RayDir, XMFloat4to3(scene->Player->PlayerObject->CenterPos),
				scene->Player->PlayerObject->Lookvector, &savepoint) == true)
			{
				
				if (scene->Player->MouseTrace)
				{
					scene->Player->TraceObject->CenterPos = XMFloat3to4(savepoint);
					
				}

				Shot = true;
			}
			else//������Ʈ�� �� ��� �ȸ¾����� ��������.
			{
				savepoint = XMFLOAT3(-10000,-10000,-10000);
				scene->Player->TraceObject->CenterPos = XMFloat3to4(savepoint);
				
			}
		}
	}

}


bool FrameWork::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = GetWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"GetWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, mClientWidth, mClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	hwnd = CreateWindow(L"GetWnd", mGetWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hinstance, 0);
	if (!hwnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return true;
}

void FrameWork::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (DxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		LogAdapterOutputs(adapterList[i]);
		ReleaseCom(adapterList[i]);
	}
}

void FrameWork::LogAdapterOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, mBackBufferFormat);

		ReleaseCom(output);

		++i;
	}
}

void FrameWork::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}




void FrameWork::CreateRtvAndDsvDescriptorHeaps()
{
	//����Ÿ�ٺ� ����
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	ThrowIfFailed(Device->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

	

	//�������ٽǺ� ����
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	ThrowIfFailed(Device->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));
}



bool FrameWork::InitDirect3D()
{
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
	{
		ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}
#endif

	//���� ���丮 ����
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&DxgiFactory)));


	IDXGIAdapter1* pd3dAdapter = NULL;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND !=DxgiFactory->EnumAdapters1(i, &pd3dAdapter); i++)
	{
		//�׷���ī���� ������ �����´�.
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc; pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;
		//��밡���ϸ� �ش� ��� ������ �������� ���̷�ƮX12������ ����̽��� �����Ѵ�.
		//��ó������ 11�� �ٲٸ� 60������ ����
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0,IID_PPV_ARGS(&Device))))
			break;
	}




	if (!pd3dAdapter) //��͵��� ���Ұ��� �Ұ��( 12���� ���� ���ϴ°��)
	{
		DxgiFactory->EnumWarpAdapter(__uuidof(IDXGIFactory4), (void **)&pd3dAdapter);
		D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&Device));
	} //Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� ������ �� ������



	//�潺�ϳ� ����. �̶� �潺���� 0
	ThrowIfFailed(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&mFence)));

	//����Ÿ���̳� ��ũ���͵��� �ڵ� ������� CPU���� �ٸ��Ƿ� �װ��� ���´�.
	mRtvDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//4x�� ��Ƽ���ø� ���� ����
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	ThrowIfFailed(Device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

#ifdef _DEBUG
	LogAdapters();
#endif

	CreateCommandObjects();//Ŀ�ǵ� ���� ��ü����
	CreateSwapChain();//����ü�� �����
	CreateRtvAndDsvDescriptorHeaps();//����Ÿ�ٰ� ��ũ���� �� �����

	return true;
}

void FrameWork::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

	ThrowIfFailed(Device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf())));

	ThrowIfFailed(Device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc.Get(), // Associated command allocator
		nullptr,                   // Initial PipelineStateObject
		IID_PPV_ARGS(mCommandList.GetAddressOf())));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	mCommandList->Close();
}

void FrameWork::CreateSwapChain()
{
	// Release the previous swapchain we will be recreating.
	SwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SwapChainBufferCount;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	ThrowIfFailed(DxgiFactory->CreateSwapChain(
		mCommandQueue.Get(),
		&sd,
		SwapChain.GetAddressOf()));
}

void FrameWork::FlushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	mCurrentFence++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), mCurrentFence));

	// Wait until the GPU has completed commands up to this fence point.
	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFence, eventHandle));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

ID3D12Resource* FrameWork::CurrentBackBuffer()const
{
	return SwapChainBuffer[mCurrBackBuffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE FrameWork::CurrentBackBufferView()const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrBackBuffer,
		mRtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE FrameWork::DepthStencilView()const
{
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}



Ray FrameWork::MousePicking(float mx, float my, XMFLOAT3& eye, XMFLOAT4X4& ViewM, XMFLOAT4X4& ProjM)
{


	XMVECTOR M;
	XMFLOAT4 Mouse = XMFLOAT4(mx, my, 1, 1);//�ȼ���ǥ��

											//���콺 ���ʹ� ���� ȭ�� ��ǥ���. �̰��� ������ǥ��� ��Ÿ������ ������ ����.

											//������ǥ�� * ���� * ī�޶� * ���� * ����Ʈ = ȭ�� ��ǥ��

											//������ǥ�� * ���� =ȭ�� ��ǥ�� * ����Ʈ ����ȯ * ��������ȯ * ī�޶� ����ȯ


											//�� ��Ʈ ����ȯ = Ŭ������
	Mouse.x = -1.0f - (mx / mClientWidth * (-2.0f));
	Mouse.y = 1.0f - (my / mClientHeight * (2.0f));
	Mouse.z = 1;


	//���� �� ��ȯ = ī�޶���������� ������ ����.
	Mouse.x /= ProjM._11;
	Mouse.y /= ProjM._22;
	Mouse.w = 1;

	M = XMLoadFloat4(&Mouse);

	//3. ī�޶��� ����ȯ
	XMMATRIX InverseCamera;

	auto Det = XMMatrixDeterminant(XMLoadFloat4x4(&ViewM));

	InverseCamera = XMMatrixInverse(&Det, XMLoadFloat4x4(&ViewM));
	M = XMVector4Transform(M, InverseCamera);

	XMStoreFloat4(&Mouse, M);

	XMFLOAT3 p;
	p.x = Mouse.x;
	p.y = Mouse.y;
	p.z = Mouse.z;










	//ī�޶�������� ������ ��ġ�� 0,0,0 �� ī�޶� ��ġ.
	Ray r;
	r.RayOrgin = XMFLOAT3(0, 0, 0);

	//ī�޶󿪺�ȯ = �������(����)

	XMFLOAT4 PickEnd = Mouse;

	//ī�޶� ����ȯ = �������(������)

	XMVECTOR ps = XMLoadFloat3(&r.RayOrgin);

	ps = XMVector3Transform(ps, InverseCamera);

	XMStoreFloat3(&r.RayOrgin, ps);


	//�������ʹ� ���� - ������.
	r.RayDir.x = PickEnd.x - r.RayOrgin.x;
	r.RayDir.y = PickEnd.y - r.RayOrgin.y;
	r.RayDir.z = PickEnd.z - r.RayOrgin.z;

	//������ ����ȭ
	ps = XMLoadFloat3(&r.RayDir);
	ps = XMVector3Normalize(ps);
	XMStoreFloat3(&r.RayDir, ps);

	return r;
}
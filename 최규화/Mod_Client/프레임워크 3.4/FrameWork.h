#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3dUtil.h"
#include "Timer.h"
#include"Scene.h"
// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


struct Ray
{
	XMFLOAT3 RayOrgin;//광원시작점
	XMFLOAT3 RayDir;//광원방향 노멀이여야한다.
};


class FrameWork
{
protected:

	FrameWork(HINSTANCE hInstance);
	FrameWork(const FrameWork& rhs) = delete;
	FrameWork& operator=(const FrameWork& rhs) = delete;
	virtual ~FrameWork();

protected:

	static FrameWork* mApp;



	GameTimer mTimer;//타이머

	Microsoft::WRL::ComPtr<IDXGIFactory4> DxgiFactory;//펙토리
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;//스왑체인
	Microsoft::WRL::ComPtr<ID3D12Device> Device;//디바이스

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;//펜스
	UINT64 mCurrentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;//커맨드리스트들을 모아두는곳
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;//커맨드리스트의 메모리관리소
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;//커맨드리스트 = DC

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> SwapChainBuffer[SwapChainBufferCount];//후면버퍼와 전면버퍼
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;//뎁스스텐실버퍼

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;//렌더타겟뷰 힙
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;//뎁스스텐실뷰 힙

	D3D12_VIEWPORT mScreenViewport;//뷰포트
	D3D12_RECT mScissorRect;//클리핑용

	UINT mRtvDescriptorSize = 0;//렌더타렛뷰 사이즈
	UINT mDsvDescriptorSize = 0;//뎁스스텐실뷰 사이즈
	UINT mCbvSrvUavDescriptorSize = 0;//상수버퍼뷰 사이즈

	int mClientWidth = 800;
	int mClientHeight = 600;

	
	
	Scene* scene=NULL;//씬클래스


	//----------------------------------------------------------------------------------------------
	
		HINSTANCE hinstance = nullptr; // 인스턴스핸들
		HWND      hwnd = nullptr; // 윈도우핸들
		bool      bAppPaused = false;  // 앱이멈춤?
		bool      bMinimized = false;  // 최소화?
		bool      bMaximized = false;  // 최대화?
		bool      bResizing = false;   // 화면리사이즈?
		bool      bFullscreenState = false;// 풀스크린?

										   // Set true to use 4X MSAA (?.1.8).  The default is false.
		bool      m4xMsaaState = false;    // 4X MSAA enabled
		UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

		// Derived class should set these in derived constructor to customize starting values.
		std::wstring mGetWndCaption = L"FrameWork";
		D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	


protected:
	virtual void CreateRtvAndDsvDescriptorHeaps();//렌더타겟과 스텐실의 힙들
	virtual void OnResize();//화면 크기 조정
	
	virtual void FrameAdvance(const GameTimer& gt) = 0;//매프레임마다 처리할것들. 아래의 업데이트와 드로우함수호출한다.
	virtual void Update(const GameTimer& gt) = 0;
	virtual void Draw(const GameTimer& gt) = 0;
	
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

protected:

	bool InitMainWindow();//윈도우초기화
	bool InitDirect3D();//다렉3d요소 초기화. dxgi나 id3d12Device 등
	void CreateCommandObjects();//커맨드 알로케이터 등을 생성한다.
	void CreateSwapChain();//스왑체인 생성
	void FlushCommandQueue();//gpu가 처리할때까지 cpu대기

	ID3D12Resource* CurrentBackBuffer()const;//후면버퍼얻기
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;//후면버퍼의 뷰 얻기
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;//뎁스스텐실뷰 얻기

	void CalculateFrameStats();//프레임계산
										
	//디버깅용
	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);


public:

	static FrameWork* GetApp();//해당 프레임워크 포인터를 얻음
	HINSTANCE GetInst()const;//HINSTANCE 획득
	HWND      GetWnd()const;//윈도우얻음
	float     AspectRatio()const;//종횡비 이건 나중에 카메라 클래스에 넘겨야함.

	bool Get4xMsaaState()const;//멀티샘플링 관련정보들
	void Set4xMsaaState(bool value);

	int Run();//픽메시지

	virtual bool Initialize();//초기화
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//메시지처리부분

	Ray MousePicking(int mx, int my,XMFLOAT3& at, XMFLOAT4X4& ViewM, XMFLOAT4X4& ProjM);

};

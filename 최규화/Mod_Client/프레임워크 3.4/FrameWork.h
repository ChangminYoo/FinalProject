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
	XMFLOAT3 RayOrgin;//����������
	XMFLOAT3 RayDir;//�������� ����̿����Ѵ�.
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



	GameTimer mTimer;//Ÿ�̸�

	Microsoft::WRL::ComPtr<IDXGIFactory4> DxgiFactory;//���丮
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;//����ü��
	Microsoft::WRL::ComPtr<ID3D12Device> Device;//����̽�

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;//�潺
	UINT64 mCurrentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;//Ŀ�ǵ帮��Ʈ���� ��Ƶδ°�
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;//Ŀ�ǵ帮��Ʈ�� �޸𸮰�����
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;//Ŀ�ǵ帮��Ʈ = DC

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> SwapChainBuffer[SwapChainBufferCount];//�ĸ���ۿ� �������
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;//�������ٽǹ���

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;//����Ÿ�ٺ� ��
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;//�������ٽǺ� ��

	D3D12_VIEWPORT mScreenViewport;//����Ʈ
	D3D12_RECT mScissorRect;//Ŭ���ο�

	UINT mRtvDescriptorSize = 0;//����Ÿ���� ������
	UINT mDsvDescriptorSize = 0;//�������ٽǺ� ������
	UINT mCbvSrvUavDescriptorSize = 0;//������ۺ� ������

	int mClientWidth = 800;
	int mClientHeight = 600;

	
	
	Scene* scene=NULL;//��Ŭ����


	//----------------------------------------------------------------------------------------------
	
		HINSTANCE hinstance = nullptr; // �ν��Ͻ��ڵ�
		HWND      hwnd = nullptr; // �������ڵ�
		bool      bAppPaused = false;  // ���̸���?
		bool      bMinimized = false;  // �ּ�ȭ?
		bool      bMaximized = false;  // �ִ�ȭ?
		bool      bResizing = false;   // ȭ�鸮������?
		bool      bFullscreenState = false;// Ǯ��ũ��?

										   // Set true to use 4X MSAA (?.1.8).  The default is false.
		bool      m4xMsaaState = false;    // 4X MSAA enabled
		UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

		// Derived class should set these in derived constructor to customize starting values.
		std::wstring mGetWndCaption = L"FrameWork";
		D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	


protected:
	virtual void CreateRtvAndDsvDescriptorHeaps();//����Ÿ�ٰ� ���ٽ��� ����
	virtual void OnResize();//ȭ�� ũ�� ����
	
	virtual void FrameAdvance(const GameTimer& gt) = 0;//�������Ӹ��� ó���Ұ͵�. �Ʒ��� ������Ʈ�� ��ο��Լ�ȣ���Ѵ�.
	virtual void Update(const GameTimer& gt) = 0;
	virtual void Draw(const GameTimer& gt) = 0;
	
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

protected:

	bool InitMainWindow();//�������ʱ�ȭ
	bool InitDirect3D();//�ٷ�3d��� �ʱ�ȭ. dxgi�� id3d12Device ��
	void CreateCommandObjects();//Ŀ�ǵ� �˷������� ���� �����Ѵ�.
	void CreateSwapChain();//����ü�� ����
	void FlushCommandQueue();//gpu�� ó���Ҷ����� cpu���

	ID3D12Resource* CurrentBackBuffer()const;//�ĸ���۾��
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;//�ĸ������ �� ���
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;//�������ٽǺ� ���

	void CalculateFrameStats();//�����Ӱ��
										
	//������
	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);


public:

	static FrameWork* GetApp();//�ش� �����ӿ�ũ �����͸� ����
	HINSTANCE GetInst()const;//HINSTANCE ȹ��
	HWND      GetWnd()const;//���������
	float     AspectRatio()const;//��Ⱦ�� �̰� ���߿� ī�޶� Ŭ������ �Ѱܾ���.

	bool Get4xMsaaState()const;//��Ƽ���ø� ����������
	void Set4xMsaaState(bool value);

	int Run();//�ȸ޽���

	virtual bool Initialize();//�ʱ�ȭ
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);//�޽���ó���κ�

	Ray MousePicking(int mx, int my,XMFLOAT3& at, XMFLOAT4X4& ViewM, XMFLOAT4X4& ProjM);

};


#include "D3D12Helper.h"
#include <Dxgi1_4.h>
#include <D3d12SDKLayers.h>
#include <DirectXMath.h>
#include "FBXLoader.h"

#define RESOURCE_SETUP	// 리소스설정

// 입력 레이아웃
const D3D12_INPUT_ELEMENT_DESC INPUT_LAYOUT[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};


// 상수버퍼
struct cbMatrix
{
	DirectX::XMMATRIX	_WVP;
};


static const int SCREEN_WIDTH = 1280;					
static const int SCREEN_HEIGHT = 720;					
static const LPTSTR	CLASS_NAME = TEXT("D3D12OnFBX");	// 창 이름
static const UINT BACKBUFFER_COUNT = 2;					// 백버퍼 수
static const UINT CONSTANT_BUFFER_COUNT = 2;			// 상수버퍼 수

// 디스크립터 힙 유형
enum DESCRIPTOR_HEAP_TYPE
{
	DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,	// UAV
	DESCRIPTOR_HEAP_TYPE_SAMPLER,		// 샘플러
	DESCRIPTOR_HEAP_TYPE_RTV,			// 렌더타겟
	DESCRIPTOR_HEAP_TYPE_DSV,			// 깊이 스텐실	
	DESCRIPTOR_HEAP_TYPE_MAX,			// 디스크립터 힙 수
	DESCRIPTOR_HEAP_TYPE_SET = DESCRIPTOR_HEAP_TYPE_SAMPLER + 1,
};


// 전역변수

#if _DEBUG
ID3D12Debug*				g_pDebug;											// 디버깅 객체
#endif

ID3D12Device*				g_pDevice;											// 장치
ID3D12CommandAllocator*		g_pCommandAllocator;								// 명령할당
ID3D12CommandQueue*			g_pCommandQueue;									// 명령 큐
IDXGIDevice2*				g_pGIDevice;										// GI장치
IDXGIAdapter*				g_pGIAdapter;										// GI어댑터
IDXGIFactory2*				g_pGIFactory;										// GI팩토리
IDXGISwapChain3*			g_pGISwapChain;										// GI스왑체인
ID3D12PipelineState*		g_pPipelineState;									// 파이프라인 스테이트
ID3D12DescriptorHeap*		g_pDescripterHeapArray[DESCRIPTOR_HEAP_TYPE_MAX];	// 디스크립터 힙 배열
ID3D12GraphicsCommandList*	g_pGraphicsCommandList;								// 그리기 명령 목록
ID3D12RootSignature*		g_pRootSignature;									// 루트 서명
D3D12_VIEWPORT				g_viewPort;											// 뷰포트
ID3D12Fence*				g_pFence;											// 팬스 개체
HANDLE						g_hFenceEvent;										// 팮스 이벤트 핸들 
UINT64						g_CurrentFenceIndex = 0;							// 현재의 팬스 인덱스

UINT						g_CurrentBuckBufferIndex = 0;						// 현재 백 버퍼
ID3D12Resource*				g_pBackBufferResource[BACKBUFFER_COUNT];			// 백 버퍼 리소스
D3D12_CPU_DESCRIPTOR_HANDLE	g_hBackBuffer[BACKBUFFER_COUNT];					// 백 버퍼 핸들

ID3D12Resource*				g_pDepthStencilResource;							// 깊이 스텐실 리소스
D3D12_CPU_DESCRIPTOR_HANDLE	g_hDepthStencil;									// 깊이 스텐실 핸들

LPD3DBLOB					g_pVSBlob;											// 버텍스 쉐이더 물방울
LPD3DBLOB					g_pPSBlob;											// 픽셀쉐이더 물방울

ID3D12Resource*				g_pVertexBufferResource;							// 버텍스 버퍼 리소스
D3D12_VERTEX_BUFFER_VIEW	g_VertexBufferView;									// 정점버퍼뷰

ID3D12Resource*				g_pConstantBufferResource;							// 상수버퍼 리소스
cbMatrix					g_ConstantBufferData;								// 상수 버퍼 실제 데이터
D3D12_CPU_DESCRIPTOR_HANDLE	g_hConstantBuffer[CONSTANT_BUFFER_COUNT];			// 상수 버퍼 핸들

VertexDataArray				g_vertexDataArray;									// 정점 데이터 배열(로드 저장용)

// DirectX초기화
bool initDirectX(HWND hWnd)
{
	HRESULT hr;
	UINT GIFlag = 0;
#if _DEBUG
	// 디버그 레이어 만들기
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&g_pDebug));
	if(showErrorMessage(hr, TEXT("디버그 레이어 생성 실패")))
	{
		return false;
	}
	if(g_pDebug)
	{
		g_pDebug->EnableDebugLayer();
	}
	GIFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif

	// GI팩토리 획득
	// 디버그 모드 팩토리 작성
	hr = CreateDXGIFactory2(GIFlag, IID_PPV_ARGS(&g_pGIFactory));
	if(showErrorMessage(hr, TEXT("GI 팩토리 획득 실패")))
	{
		return false;
	}

	IDXGIAdapter* pGIAdapter = nullptr;
	hr = g_pGIFactory->EnumAdapters(0, &pGIAdapter);
	if(showErrorMessage(hr, TEXT("GI어댑터 획득 실패 ")))
	{
		return false;
	}

	// 명령 할당 작성
	hr = D3D12CreateDevice(
		pGIAdapter,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&g_pDevice));

	safeRelease(pGIAdapter);

	if(showErrorMessage(hr, TEXT("장치생성 실패")))
	{
		return false;
	}

	// 명령 할당 작성
	hr = g_pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&g_pCommandAllocator));

	if(showErrorMessage(hr, TEXT("명령 할당 작성 실패")))
	{
		hr = g_pDevice->GetDeviceRemovedReason();
		return false;
	}

	// 명령 대기열 생성
	// 
	//g_pDevice->GetDefaultCommandQueue(&g_pCommandQueue);
	D3D12_COMMAND_QUEUE_DESC commandQueueDesk;
	commandQueueDesk.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// 명령 목록 유형
	commandQueueDesk.Priority = 0;							// 우선순위 
	commandQueueDesk.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// 플래그 
	commandQueueDesk.NodeMask = 0x00000000;					// 노드 마스크

	hr = g_pDevice->CreateCommandQueue(&commandQueueDesk, IID_PPV_ARGS(&g_pCommandQueue));

	if(showErrorMessage(hr, TEXT("명령 대기열 생성 실패 ")))
	{
		return false;
	}

	// 스왑체인 생성


	DXGI_SWAP_CHAIN_DESC descSwapChain;
	ZeroMemory(&descSwapChain, sizeof(descSwapChain));
	descSwapChain.BufferCount = BACKBUFFER_COUNT;					
	descSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descSwapChain.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	descSwapChain.OutputWindow = hWnd;
	descSwapChain.SampleDesc.Count = 1;
	descSwapChain.Windowed = TRUE;

	// 장치가 아니라 명령 대기열 전달
	// 그렇지 않으면 런타임 오류가 발생함
	hr = g_pGIFactory->CreateSwapChain(g_pCommandQueue, &descSwapChain, reinterpret_cast<IDXGISwapChain**>(&g_pGISwapChain));
	if(showErrorMessage(hr, TEXT("스왑체인 생성 실패")))
	{
		return false;
	}

	// ?그리기 명령목록(커맨드리스트) 작성
	hr = g_pDevice->CreateCommandList(
		0x00000000,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		g_pCommandAllocator,
		g_pPipelineState,
		IID_PPV_ARGS(&g_pGraphicsCommandList));

	if(showErrorMessage(hr, TEXT("커맨드 리스트 생성 실패")))
	{
		return false;
	}

	// 쉐이더 컴파일 
	if(compileShaderFlomFile(L"shader/VertexShader.hlsl", "main", "vs_5_1", &g_pVSBlob) == false)
	{
		showErrorMessage(E_FAIL, TEXT("정점 셰이더 컴파일 실패"));
	}
	if (compileShaderFlomFile(L"shader/PixelShader.hlsl", "main", "ps_5_1", &g_pPSBlob) == false)
	{
		showErrorMessage(E_FAIL, TEXT("픽셀 셰이더 컴파일 실패"));
	}


	// 정점 셰이더 상수 버퍼를 전달할 루트 시그니쳐 작성
	D3D12_DESCRIPTOR_RANGE range;
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	range.NumDescriptors = 1;
	range.BaseShaderRegister = 0;
	range.RegisterSpace = 0;
	range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_DESCRIPTOR_TABLE rootDescriptorTable;
	rootDescriptorTable.NumDescriptorRanges = 1;
	rootDescriptorTable.pDescriptorRanges = &range;

	D3D12_ROOT_PARAMETER rootParameter;
	rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameter.DescriptorTable = rootDescriptorTable;

	LPD3DBLOB pOutBlob = nullptr;
	D3D12_ROOT_SIGNATURE_DESC descRootSignature = D3D12_ROOT_SIGNATURE_DESC();
	descRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	descRootSignature.NumParameters = 1;
	descRootSignature.pParameters = &rootParameter;

	D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &pOutBlob, nullptr);
	hr = g_pDevice->CreateRootSignature(
		0x00000001,
		pOutBlob->GetBufferPointer(),
		pOutBlob->GetBufferSize(),
		IID_PPV_ARGS(&g_pRootSignature));

	safeRelease(pOutBlob);
	if(showErrorMessage(hr, TEXT("루트시그니쳐 생성 실패")))
	{
		return false;
	}

	// 레스터라이저 상태 설정
	D3D12_RASTERIZER_DESC descRasterizer;
	descRasterizer.FillMode = D3D12_FILL_MODE_SOLID;
	descRasterizer.CullMode = D3D12_CULL_MODE_NONE;
	descRasterizer.FrontCounterClockwise = FALSE;
	descRasterizer.DepthBias = 0;
	descRasterizer.SlopeScaledDepthBias = 0.0f;
	descRasterizer.DepthBiasClamp = 0.0f;
	descRasterizer.DepthClipEnable = TRUE;
	descRasterizer.MultisampleEnable = FALSE;
	descRasterizer.AntialiasedLineEnable = FALSE;
	descRasterizer.ForcedSampleCount = 0;
	descRasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	// 블랜딩 상태 설정
	D3D12_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	descBlend.RenderTarget[0].BlendEnable = FALSE;
	descBlend.RenderTarget[0].LogicOpEnable = FALSE;
	descBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	descBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	descBlend.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// 파이프라인 상태 객체 생성
	// 정점 셰이더와 픽셀 셰이더가 없으면 만들지 못함
	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPSO;
	ZeroMemory(&descPSO, sizeof(descPSO));
	descPSO.InputLayout = { INPUT_LAYOUT, ARRAYSIZE(INPUT_LAYOUT) };										// 
	descPSO.pRootSignature = g_pRootSignature;																// 
	descPSO.VS = { reinterpret_cast<BYTE*>(g_pVSBlob->GetBufferPointer()), g_pVSBlob->GetBufferSize() };	// 
	descPSO.PS = { reinterpret_cast<BYTE*>(g_pPSBlob->GetBufferPointer()), g_pPSBlob->GetBufferSize() };	// 
	descPSO.RasterizerState = descRasterizer;																// 
	descPSO.BlendState = descBlend;																			// 
	descPSO.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);									// 
	descPSO.SampleMask = UINT_MAX;																			// 
	descPSO.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;									// 
	descPSO.NumRenderTargets = 1;																			// 
	descPSO.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;														// 
	descPSO.SampleDesc.Count = 1;																			// 

	hr = g_pDevice->CreateGraphicsPipelineState(&descPSO, IID_PPV_ARGS(&g_pPipelineState));

	if(showErrorMessage(hr, TEXT("파이프라인 생성 실패")))
	{
		return false;
	}

	// 디스크립터 힙 생성
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	ZeroMemory(&heapDesc, sizeof(heapDesc));

	heapDesc.NumDescriptors = BACKBUFFER_COUNT;

	for(int i = 0; i < DESCRIPTOR_HEAP_TYPE_MAX; ++i)
	{
		heapDesc.Flags = (i == D3D12_DESCRIPTOR_HEAP_TYPE_RTV || i == D3D12_DESCRIPTOR_HEAP_TYPE_DSV) ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i);

		hr = g_pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&g_pDescripterHeapArray[i]));
		if(showErrorMessage(hr, TEXT("디스크립터 힙 생성 실패")))
		{
			return false;
		}
	}

	// 디스크립터힙과 커맨드 리스트 연결 
	g_pGraphicsCommandList->SetDescriptorHeaps(DESCRIPTOR_HEAP_TYPE_SET, g_pDescripterHeapArray);

	// 랜더타겟뷰(백버퍼) 작성
	UINT strideHandleBytes = g_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for(UINT i = 0; i < BACKBUFFER_COUNT; ++i)
	{
		hr = g_pGISwapChain->GetBuffer(i, IID_PPV_ARGS(&g_pBackBufferResource[i]));
		if(showErrorMessage(hr, TEXT("랜더 타겟뷰 생성 실패")))
		{
			return false;
		}
		g_hBackBuffer[i] = g_pDescripterHeapArray[DESCRIPTOR_HEAP_TYPE_RTV]->GetCPUDescriptorHandleForHeapStart();
		g_hBackBuffer[i].ptr += i * strideHandleBytes;	// 렌더 타겟의 오프셋만큼 포인터를 늦춘다?
		g_pDevice->CreateRenderTargetView(g_pBackBufferResource[i], nullptr, g_hBackBuffer[i]);
	}

	// 깊이 스텐실 생성
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
		depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

		D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
		depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
		depthOptimizedClearValue.DepthStencil.Stencil = 0;

		hr = g_pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depthOptimizedClearValue,
			IID_PPV_ARGS(&g_pDepthStencilResource));

		if(showErrorMessage(hr, TEXT("깊이 스텐실 생성 실패")))
		{
			return false;
		}

		g_hDepthStencil = g_pDescripterHeapArray[DESCRIPTOR_HEAP_TYPE_DSV]->GetCPUDescriptorHandleForHeapStart();
		g_pDevice->CreateDepthStencilView(
			g_pDepthStencilResource,
			&depthStencilDesc,
			g_hDepthStencil);
	}

	// 뷰포트 설정 
	g_viewPort.TopLeftX = 0;			// X
	g_viewPort.TopLeftY = 0;			// Y
	g_viewPort.Width = SCREEN_WIDTH;	// 폭 
	g_viewPort.Height = SCREEN_HEIGHT;	// 높이
	g_viewPort.MinDepth = 0.0f;			// 최소 깊이
	g_viewPort.MaxDepth = 1.0f;			// 최대 깊이

	// 팬스 객체 생성
	g_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_pFence));

	if(showErrorMessage(hr, TEXT("팬스객체 생성 실패")))
	{
		return false;
	}

	// 울타리 이벤트 핸들 생성
	g_hFenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	return true;
}

// DirectX12정리
void cleanupDirectX()
{
	CloseHandle(g_hFenceEvent);
	safeRelease(g_pFence);
	safeRelease(g_pDepthStencilResource);
	for(UINT i = 0; i < BACKBUFFER_COUNT; ++i)
	{
		safeRelease(g_pBackBufferResource[i]);
	}
	for(int i = 0; i < DESCRIPTOR_HEAP_TYPE_MAX; ++i)
	{
		safeRelease(g_pDescripterHeapArray[i]);
	}
	safeRelease(g_pPipelineState);
	safeRelease(g_pRootSignature);
	safeRelease(g_pPSBlob);
	safeRelease(g_pVSBlob);

	safeRelease(g_pGISwapChain);
	safeRelease(g_pGIFactory);
	safeRelease(g_pGIAdapter);
	safeRelease(g_pGIDevice);
	safeRelease(g_pCommandQueue);
	safeRelease(g_pGraphicsCommandList);
	safeRelease(g_pCommandAllocator);
	safeRelease(g_pDevice);
#if _DEBUG
	safeRelease(g_pDebug);
#endif
}

#if defined(RESOURCE_SETUP)
// 자원 설정
bool setupResource()
{
	HRESULT hr;

	// 버텍스 버퍼 생성
	{
		// FBX모델 로드
		if(LoadFBXConvertToVertexData("../models/dragon.fbx", g_vertexDataArray) == false)
		{
			if(showErrorMessage(E_FAIL, TEXT("FBX모델을 로드하지 못했습니다.")))
			{
				return false;
			}
		}

		D3D12_HEAP_PROPERTIES heapPropaty;
		heapPropaty.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapPropaty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapPropaty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapPropaty.CreationNodeMask = 1;
		heapPropaty.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC descResource;
		descResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		descResource.Alignment = 0;
		descResource.Width = g_vertexDataArray.size() * sizeof(VertexData);
		descResource.Height = 1;
		descResource.DepthOrArraySize = 1;
		descResource.MipLevels = 1;
		descResource.Format = DXGI_FORMAT_UNKNOWN;
		descResource.SampleDesc.Count = 1;
		descResource.SampleDesc.Quality = 0;
		descResource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		descResource.Flags = D3D12_RESOURCE_FLAG_NONE;

		hr = g_pDevice->CreateCommittedResource(
			&heapPropaty,
			D3D12_HEAP_FLAG_NONE,
			&descResource,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&g_pVertexBufferResource));

		if(showErrorMessage(hr, TEXT("버텍스 버퍼 생성 실패")))
		{
			return false;
		}

		// 정점버퍼에 삼각형 정보 복사
		UINT8* dataBegin;
		if(SUCCEEDED(g_pVertexBufferResource->Map(0, nullptr, reinterpret_cast<void**>(&dataBegin))))
		{
			VertexData* temp = reinterpret_cast<VertexData*>(dataBegin);
			for(size_t i = 0; i < g_vertexDataArray.size(); ++i)
			{
				temp[i] = g_vertexDataArray[i];
			}
			g_pVertexBufferResource->Unmap(0, nullptr);
		}
		else
		{
			showErrorMessage(E_FAIL, TEXT("정점버퍼 매핑 실패"));
			return false;
		}

		// 정점 버퍼 뷰 설정
		g_VertexBufferView.BufferLocation = g_pVertexBufferResource->GetGPUVirtualAddress();
		g_VertexBufferView.StrideInBytes = sizeof(VertexData);
		g_VertexBufferView.SizeInBytes = g_vertexDataArray.size() * sizeof(VertexData);
	}

	// 상수 버퍼 생성
	{
		D3D12_HEAP_PROPERTIES heapPropaty;
		heapPropaty.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapPropaty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapPropaty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapPropaty.CreationNodeMask = 1;
		heapPropaty.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC descResource;
		descResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		descResource.Alignment = 0;
		descResource.Width = sizeof(cbMatrix);
		descResource.Height = 1;
		descResource.DepthOrArraySize = 1;
		descResource.MipLevels = 1;
		descResource.Format = DXGI_FORMAT_UNKNOWN;
		descResource.SampleDesc.Count = 1;
		descResource.SampleDesc.Quality = 0;
		descResource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		descResource.Flags = D3D12_RESOURCE_FLAG_NONE;

		hr = g_pDevice->CreateCommittedResource(
			&heapPropaty,
			D3D12_HEAP_FLAG_NONE,
			&descResource,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&g_pConstantBufferResource));

		if(showErrorMessage(hr, TEXT("상수버퍼 생성 실패")))
		{
			return false;
		}

		// 상수 버퍼 뷰 생성
		D3D12_CONSTANT_BUFFER_VIEW_DESC descConstantBufferView = {};

		descConstantBufferView.BufferLocation = g_pConstantBufferResource->GetGPUVirtualAddress();
		descConstantBufferView.SizeInBytes = (sizeof(cbMatrix) + 255) & ~255;	//256배수로 설정해야

		g_pDevice->CreateConstantBufferView(
			&descConstantBufferView,
			g_pDescripterHeapArray[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetCPUDescriptorHandleForHeapStart());

		UINT strideHandleBytes = g_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		for(int i = 0; i < CONSTANT_BUFFER_COUNT; ++i)
		{
			g_hConstantBuffer[i] = g_pDescripterHeapArray[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetCPUDescriptorHandleForHeapStart();
			g_hConstantBuffer[i].ptr += i * strideHandleBytes;

			g_pDevice->CreateConstantBufferView(
				&descConstantBufferView,
				g_hConstantBuffer[i]);
		}
	}

	return true;
}
#endif

#if defined(RESOURCE_SETUP)
// 리소스 정리
void cleanupResource()
{
	safeRelease(g_pConstantBufferResource);
	safeRelease(g_pVertexBufferResource);
}
#endif

// 랜더링
void Render()
{
	// 현재의 커맨드 리스트 획득
	ID3D12GraphicsCommandList* pCommand = g_pGraphicsCommandList;

	// 파이프 라인 상태 객체 및 루트 시그니쳐를 세트함
	pCommand->SetPipelineState(g_pPipelineState);
	pCommand->SetGraphicsRootSignature(g_pRootSignature);

	// 사각형생성
	D3D12_RECT clearRect;
	clearRect.left = 0;
	clearRect.top = 0;
	clearRect.right = SCREEN_WIDTH;
	clearRect.bottom = SCREEN_HEIGHT;

	// 백버퍼의 인덱스 변경
	g_CurrentBuckBufferIndex = g_pGISwapChain->GetCurrentBackBufferIndex();

	// 랜더 타겟 뷰 설정
	pCommand->OMSetRenderTargets(
		1,
		&g_hBackBuffer[g_CurrentBuckBufferIndex],
		FALSE, 
		&g_hDepthStencil);

	// 랜더 타겟에 리소스 설정
	setResourceBarrier(
		pCommand,
		g_pBackBufferResource[g_CurrentBuckBufferIndex],
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	// 클리어
	static float count = 0;
	count = fmod(count + 0.01f, 1.0f);
	float clearColor[] = { 0.0f /*count*/, 0.2f, 0.4f, 1.0f };
	pCommand->RSSetViewports(1, &g_viewPort);
	pCommand->RSSetScissorRects(1, &clearRect);
	pCommand->ClearRenderTargetView(g_hBackBuffer[g_CurrentBuckBufferIndex], clearColor, 1, &clearRect);
	pCommand->ClearDepthStencilView(g_hDepthStencil, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#if defined(RESOURCE_SETUP)

	// 상수 버퍼 업데이트
	using namespace DirectX;

	XMMATRIX view = XMMatrixLookAtLH(
		XMLoadFloat3(&XMFLOAT3(0, 0, -100)),
		XMLoadFloat3(&XMFLOAT3(0, 0, 0)),
		XMLoadFloat3(&XMFLOAT3(0, 1, 0)));

	XMMATRIX proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60),
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
		1,
		1000);

	static XMFLOAT3 rotation(0, 0, 0);
	rotation.x = fmod(rotation.x + 1.f, 360.f);
	rotation.y = fmod(rotation.y + 2.f, 360.f);
	rotation.z = fmod(rotation.z + 3.f, 360.f);

	XMMATRIX world;

	{
		world = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(0.0f),
			XMConvertToRadians(rotation.y),
			XMConvertToRadians(0.0f));

		g_ConstantBufferData._WVP = XMMatrixTranspose(world * view * proj);

		UINT8* dataBegin;
		if(SUCCEEDED(g_pConstantBufferResource->Map(0, nullptr, reinterpret_cast<void**>(&dataBegin))))
		{
			memcpy(dataBegin, &g_ConstantBufferData, sizeof(g_ConstantBufferData));
			g_pConstantBufferResource->Unmap(0, nullptr);
		}
		else
		{
			showErrorMessage(S_FALSE, TEXT("상수버퍼의 매핑 실패"));
		}

		// 상수 버퍼 설정
		ID3D12DescriptorHeap* pHeaps[] = { g_pDescripterHeapArray[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV] };
		pCommand->SetDescriptorHeaps(ARRAYSIZE(pHeaps), pHeaps);
		pCommand->SetGraphicsRootDescriptorTable(0, g_pDescripterHeapArray[DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV]->GetGPUDescriptorHandleForHeapStart());

		// 삼각형 그리기 
		pCommand->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pCommand->IASetVertexBuffers(0, 1, &g_VertexBufferView);
		pCommand->DrawIndexedInstanced(36, 1, 0, 0, 0);
		pCommand->DrawInstanced(g_vertexDataArray.size(), g_vertexDataArray.size() / 3, 0, 0);
	}
#endif

	// present이전 처리 
	setResourceBarrier(
		pCommand,
		g_pBackBufferResource[g_CurrentBuckBufferIndex],
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	// 그리기 명령을 실행하여 반전?(플립)
	pCommand->Close();
	ID3D12CommandList* pTemp = pCommand;
	g_pCommandQueue->ExecuteCommandLists(1, &pTemp);
	g_pGISwapChain->Present(1, 0);

	// 명령 대기열 처리를 대기
	const UINT64 FENCE_INDEX = g_CurrentFenceIndex;
	g_pCommandQueue->Signal(g_pFence, FENCE_INDEX);
	g_CurrentFenceIndex++;

	if(g_pFence->GetCompletedValue() < FENCE_INDEX)
	{
		g_pFence->SetEventOnCompletion(FENCE_INDEX, g_hFenceEvent);
		WaitForSingleObject(g_hFenceEvent, INFINITE);
	}

	// 명령 할당과 명령 목록을 재설정 
	g_pCommandAllocator->Reset();
	pCommand->Reset(g_pCommandAllocator, g_pPipelineState);
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	
	switch(msg)
	{
	case WM_KEYDOWN:
		switch(wparam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_DESTROY:	
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	winc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winc.lpfnWndProc = WndProc;				
	winc.cbClsExtra = 0;
	winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = CLASS_NAME;

	
	if(RegisterClass(&winc) == false)
	{
		return 1;
	}

	// 창 생성
	RECT windowRect = { 0, 0, static_cast<LONG>(SCREEN_WIDTH), static_cast<LONG>(SCREEN_HEIGHT) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindow(
		CLASS_NAME,
		CLASS_NAME,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(hwnd == NULL)
	{
		return 1;
	}

	// DirectX초기화
	if(initDirectX(hwnd) == false)
	{
		return 1;
	}

#if defined(RESOURCE_SETUP)
	if(setupResource() == false)
	{
		return 1;
	}
#endif

	// 창 표시
	ShowWindow(hwnd, nCmdShow);

	// 메시지 루프 
	do {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	} while(msg.message != WM_QUIT);


#if defined(RESOURCE_SETUP)
	cleanupResource();
#endif
	cleanupDirectX();


	UnregisterClass(CLASS_NAME, hInstance);

	return 0;
}
#pragma once
#include "d3dUtil.h"
#include"CGameObject.h"
#include"CPlayer.h"
using Microsoft::WRL::ComPtr;


class Shader
{
public:
	CPlayer * player=NULL;
	Shader();
	~Shader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();//입력레이아웃생성	
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout_GS(); //GSㅇ용

	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();//레스터라이저생성
	virtual D3D12_BLEND_DESC CreateBlendState(bool isBlend=false);//블랜더상태생성
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState(bool isBlend=false);//깊이상태생성
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(WCHAR *shaderfile, LPCSTR SName,LPCSTR Profile);//버텍스쉐이더생성
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile);//픽셀쉐이더생성
	virtual D3D12_SHADER_BYTECODE CreateGeometryShader(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile);

	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile,ID3DBlob** blob);//실제 쉐이더생성함수

	virtual void CreateShader(ID3D12Device * Device, ID3D12RootSignature * GraphicsRootSignature);//PSO생성
	virtual void SetShader(ID3D12GraphicsCommandList* commandlist,bool isBlend=false);
	virtual void SetSkyShader(ID3D12GraphicsCommandList* commandlist);
	virtual void SetBillboardShader(ID3D12GraphicsCommandList* commandlist);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,const GameTimer& gt);
	virtual bool isRender(CGameObject* obj);
	//카메라와 플레이어 사이에있는 오브젝트는 반투명하게 그린다.
	virtual bool ishalfalphaRender(CGameObject* obj);

protected:
	ComPtr<ID3DBlob> VertexShader = nullptr;
	ComPtr<ID3DBlob> PixelShader = nullptr;
	ComPtr<ID3DBlob> GeometryShader = nullptr;

	ComPtr<ID3D12PipelineState> PSO = nullptr;
	ComPtr<ID3D12PipelineState> BlendPSO = nullptr;
	ComPtr<ID3D12PipelineState> SkyPSO = nullptr;
	ComPtr<ID3D12PipelineState> BillboardPSO = nullptr;

public:
	list<CGameObject*>* DynamicObject=NULL;//애니메이션이 되는 오브젝트들이 여기에 모임.Scene클래스가 가진거를 공유만.
	list<CGameObject*>* BulletObject = NULL;//투사체들이 여기에 모임.Scene클래스가 가진거를 공유만.
	list<CGameObject*>* StaticObject = NULL;//애니메이션이 *되지 않는* 오브젝트들이 여기에 모임.Scene클래스가 가진거를 공유만.
	list<CGameObject*>* RigidObject = NULL;//리지드바디 오브젝트
	list<CGameObject*>* BbObject = NULL;

	CGameObject** SkyObject = NULL;//다른오브젝트와 달리 하나만 있으면 됨
	
};


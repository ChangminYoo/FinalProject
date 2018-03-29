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

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();//�Է·��̾ƿ�����	
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout_GS(); //GS����

	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();//�����Ͷ���������
	virtual D3D12_BLEND_DESC CreateBlendState(bool isBlend=false);//�������»���
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState(bool isBlend=false);//���̻��»���
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(WCHAR *shaderfile, LPCSTR SName,LPCSTR Profile);//���ؽ����̴�����
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile);//�ȼ����̴�����
	virtual D3D12_SHADER_BYTECODE CreateGeometryShader(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile);

	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile,ID3DBlob** blob);//���� ���̴������Լ�

	virtual void CreateShader(ID3D12Device * Device, ID3D12RootSignature * GraphicsRootSignature);//PSO����
	virtual void SetShader(ID3D12GraphicsCommandList* commandlist,bool isBlend=false);
	virtual void SetSkyShader(ID3D12GraphicsCommandList* commandlist);
	virtual void SetBillboardShader(ID3D12GraphicsCommandList* commandlist);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList,const GameTimer& gt);
	virtual bool isRender(CGameObject* obj);
	//ī�޶�� �÷��̾� ���̿��ִ� ������Ʈ�� �������ϰ� �׸���.
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
	list<CGameObject*>* DynamicObject=NULL;//�ִϸ��̼��� �Ǵ� ������Ʈ���� ���⿡ ����.SceneŬ������ �����Ÿ� ������.
	list<CGameObject*>* BulletObject = NULL;//����ü���� ���⿡ ����.SceneŬ������ �����Ÿ� ������.
	list<CGameObject*>* StaticObject = NULL;//�ִϸ��̼��� *���� �ʴ�* ������Ʈ���� ���⿡ ����.SceneŬ������ �����Ÿ� ������.
	list<CGameObject*>* RigidObject = NULL;//������ٵ� ������Ʈ
	list<CGameObject*>* BbObject = NULL;

	CGameObject** SkyObject = NULL;//�ٸ�������Ʈ�� �޸� �ϳ��� ������ ��
	
};


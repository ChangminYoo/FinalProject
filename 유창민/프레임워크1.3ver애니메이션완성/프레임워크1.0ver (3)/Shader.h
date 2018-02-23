#pragma once
#include "d3dUtil.h"
#include"CGameObject.h"
using Microsoft::WRL::ComPtr;


class Shader
{
public:
	Shader();
	~Shader();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();//�Է·��̾ƿ�����
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();//�����Ͷ���������
	virtual D3D12_BLEND_DESC CreateBlendState();//�������»���
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();//���̻��»���
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(WCHAR *shaderfile, LPCSTR SName,LPCSTR Profile);//���ؽ����̴�����
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile);//�ȼ����̴�����

	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile,ID3DBlob** blob);//���� ���̴������Լ�

	virtual void CreateShader(ID3D12Device * Device, ID3D12RootSignature * GraphicsRootSignature, WCHAR * vsfile, LPCSTR vsname, LPCSTR vsprofile, WCHAR * psfile, LPCSTR psname, LPCSTR psprofile);//PSO����

	virtual void SetShader(ID3D12GraphicsCommandList* commandlist);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
protected:
	ComPtr<ID3DBlob> VertexShader = nullptr;
	ComPtr<ID3DBlob> PixelShader = nullptr;

	D3D12_INPUT_ELEMENT_DESC mInputLayout;

	ComPtr<ID3D12PipelineState> PSO = nullptr;

public:
	CGameObject * testObj=NULL;
};


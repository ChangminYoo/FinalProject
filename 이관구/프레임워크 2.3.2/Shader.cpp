#include "Shader.h"



Shader::Shader()
{
}


Shader::~Shader()
{
}

D3D12_RASTERIZER_DESC Shader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	return(d3dRasterizerDesc);
}

D3D12_DEPTH_STENCIL_DESC Shader::CreateDepthStencilState(bool isBlend)
{
	if (isBlend == false)
	{
		D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
		::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
		d3dDepthStencilDesc.DepthEnable = true;
		d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		d3dDepthStencilDesc.StencilEnable = FALSE;
		d3dDepthStencilDesc.StencilReadMask = 0x00;
		d3dDepthStencilDesc.StencilWriteMask = 0x00;
		d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		return(d3dDepthStencilDesc);
	}
	else
	{
		D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
		::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
		d3dDepthStencilDesc.DepthEnable = true;
		d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		d3dDepthStencilDesc.StencilEnable = FALSE;
		d3dDepthStencilDesc.StencilReadMask = 0x00;
		d3dDepthStencilDesc.StencilWriteMask = 0x00;
		d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		return(d3dDepthStencilDesc);

	}

}

D3D12_BLEND_DESC Shader::CreateBlendState(bool isBlend)
{
	if (isBlend == false)
	{
		D3D12_BLEND_DESC d3dBlendDesc;
		::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
		d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		return(d3dBlendDesc);

	}
	else//���� ���
	{
		D3D12_BLEND_DESC d3dBlendDesc;
		::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].BlendEnable = true;
		d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
		d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		return(d3dBlendDesc);

	}
}

D3D12_SHADER_BYTECODE Shader::CompileShaderFromFile(WCHAR *pszFileName, LPCSTR
	pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **blob)
{
	/*
	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	::D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderProfile,
		nCompileFlags, 0, blob, NULL);
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*blob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*blob)->GetBufferPointer();
	return(d3dShaderByteCode);
	*/
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;


	ComPtr<ID3DBlob> errors;
	hr = D3DCompileFromFile(pszFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		pszShaderName, pszShaderProfile, compileFlags, 0, blob, &errors);

	if (errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	ThrowIfFailed(hr);
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*blob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*blob)->GetBufferPointer();
	return d3dShaderByteCode;

}

void Shader::CreateShader(ID3D12Device * Device, ID3D12RootSignature * GraphicsRootSignature, WCHAR * vsfile, LPCSTR vsname, LPCSTR vsprofile, WCHAR * psfile, LPCSTR psname, LPCSTR psprofile)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = GraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(vsfile,vsname,vsprofile);
	d3dPipelineStateDesc.PS = CreatePixelShader(psfile,psname,psprofile);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&PSO));//PSO����


	//������ PSO�� ����
	d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = GraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(vsfile, vsname, vsprofile);
	d3dPipelineStateDesc.PS = CreatePixelShader(psfile, psname, psprofile);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState(true);//�� �ΰ����� true�� �ٲ�
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState(true);//�� �ΰ����� true�� �ٲ� ������ ����
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&BlendPSO));//����� PSO����

				
								 
	//SKY PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC skyPsoDesc = d3dPipelineStateDesc;
	skyPsoDesc.VS = CreateVertexShader(L"Sky.hlsl", "VS", "vs_5_1");
	skyPsoDesc.PS = CreatePixelShader(L"Sky.hlsl", "PS", "ps_5_1");
	skyPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	skyPsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	Device->CreateGraphicsPipelineState(&skyPsoDesc,IID_PPV_ARGS(&SkyPSO));


	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[]
		d3dPipelineStateDesc.InputLayout.pInputElementDescs;


}

void Shader::SetShader(ID3D12GraphicsCommandList* commandlist,bool isBlend)
{
	if (isBlend == false)
		commandlist->SetPipelineState(PSO.Get());
	else
		commandlist->SetPipelineState(BlendPSO.Get());

}

void Shader::SetSkyShader(ID3D12GraphicsCommandList * commandlist)
{
	commandlist->SetPipelineState(SkyPSO.Get());
}

void Shader::Render(ID3D12GraphicsCommandList * CommandList, const GameTimer& gt)
{
	//���� �̷��� �׸��� ���� �׸������� �ǹٸ� PSO�� �����ϰ�, ������Ʈ���� �׸���.
	//������ ������� ���� ������, ����� ���� �׸��°� ���ٰ� �ǴܵǼ� ��ī�� ������Ʈ�� �����׸�
	SetSkyShader(CommandList);
	(*SkyObject)->Render(CommandList, gt);

	//���� �̷��� �׸��� ���� �׸������� �ǹٸ� PSO�� �����ϰ�, ������Ʈ���� �׸���.
	SetShader(CommandList, false);
	for (auto b = DynamicObject->cbegin(); b != DynamicObject->cend(); b++)
	{
		if ((*b)->Blending)
		{
			//������ PSO ����
			SetShader(CommandList, true);
			//������ PSO�� �׸�
			(*b)->Render(CommandList, gt);
			//�ٽ� ������ PSO�� ����
			SetShader(CommandList, false);
		}
		else//���� �Ⱦ�
				(*b)->Render(CommandList, gt);
	}
	
	//���� �̷��� �׸��� ���� �׸������� �ǹٸ� PSO�� �����ϰ�, ������Ʈ���� �׸���.
	//����ü���� �׸���.
	for (auto b = BulletObject->cbegin(); b != BulletObject->cend(); b++)
	{
		if ((*b)->Blending)
		{
			//������ PSO ����
			SetShader(CommandList, true);
			//������ PSO�� �׸�
			(*b)->Render(CommandList, gt);
			//�ٽ� ������ PSO�� ����
			SetShader(CommandList, false);
		}
		else//���� �Ⱦ�
			(*b)->Render(CommandList, gt);
	}

	for (auto b = StaticObject->cbegin(); b != StaticObject->cend(); b++)
	{
		if ((*b)->Blending)
		{
			//������ PSO ����
			SetShader(CommandList, true);
			//������ PSO�� �׸�
			(*b)->Render(CommandList, gt);
			//�ٽ� ������ PSO�� ����
			SetShader(CommandList, false);
		}
		else//���� �Ⱦ�
			(*b)->Render(CommandList, gt);
	}


}

D3D12_INPUT_LAYOUT_DESC Shader::CreateInputLayout()
{
	UINT nInputElementDescs = 10;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new
		D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "STARTWEIGHT", 0, DXGI_FORMAT_R32_SINT, 0, 36,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[4] = { "WEIGHTCOUNT", 0, DXGI_FORMAT_R32_SINT, 0, 40,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[5] = { "JOINTID", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[6] = { "WEIGHTPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[7] = { "WEIGHTPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 76,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[8] = { "WEIGHTPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 92,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[9] = { "WEIGHTPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 108,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };



	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE Shader::CreateVertexShader(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile)
{
	return(Shader::CompileShaderFromFile(shaderfile, SName,Profile,
		VertexShader.GetAddressOf()));
}
D3D12_SHADER_BYTECODE Shader::CreatePixelShader(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile)
{
	return(Shader::CompileShaderFromFile(shaderfile, SName, Profile,
		PixelShader.GetAddressOf()));
}
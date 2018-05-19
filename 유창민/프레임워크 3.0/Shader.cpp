
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

void Shader::CreateShader(ID3D12Device * Device, ID3D12RootSignature * GraphicsRootSignature)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = GraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(L"shaders/ShaderFile.hlsl", "VS", "vs_5_0");
	d3dPipelineStateDesc.PS = CreatePixelShader(L"shaders/ShaderFile.hlsl", "PS", "ps_5_0");
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
	Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc, IID_PPV_ARGS(&PSO));//PSO����


	//������ PSO�� ����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC blendPsoDesc = d3dPipelineStateDesc;
	blendPsoDesc.VS = CreateVertexShader(L"shaders/ShaderFile.hlsl", "VS", "vs_5_0");
	blendPsoDesc.PS = CreatePixelShader(L"shaders/ShaderFile.hlsl", "PS", "ps_5_0");;
	blendPsoDesc.BlendState = CreateBlendState(true);//�� �ΰ����� true�� �ٲ�
	blendPsoDesc.DepthStencilState = CreateDepthStencilState(true);//�� �ΰ����� true�� �ٲ� ������ ����
	Device->CreateGraphicsPipelineState(&blendPsoDesc, IID_PPV_ARGS(&BlendPSO));//����� PSO����


	D3D12_GRAPHICS_PIPELINE_STATE_DESC bulletPsoDesc = d3dPipelineStateDesc;
	bulletPsoDesc.VS = CreateVertexShader(L"shaders/ShaderFile.hlsl", "VS", "vs_5_0");
	bulletPsoDesc.PS = CreatePixelShader(L"shaders/ShaderFile.hlsl", "PS2", "ps_5_0");;
	Device->CreateGraphicsPipelineState(&bulletPsoDesc, IID_PPV_ARGS(&BulletPSO));//����� PSO����


	//SKY PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC skyPsoDesc = d3dPipelineStateDesc;
	skyPsoDesc.VS = CreateVertexShader(L"shaders/Sky.hlsl", "VS", "vs_5_0");
	skyPsoDesc.PS = CreatePixelShader(L"shaders/Sky.hlsl", "PS", "ps_5_0");
	skyPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	skyPsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	Device->CreateGraphicsPipelineState(&skyPsoDesc, IID_PPV_ARGS(&SkyPSO));



	//Billboard PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC billPsoDesc = d3dPipelineStateDesc;
	billPsoDesc.VS = CreateVertexShader(L"shaders/GeometryShader.hlsl", "VS", "vs_5_0");
	billPsoDesc.GS = CreateGeometryShader(L"shaders/GeometryShader.hlsl", "GS", "gs_5_0");
	billPsoDesc.PS = CreatePixelShader(L"shaders/GeometryShader.hlsl", "PS", "ps_5_0");
	billPsoDesc.InputLayout = CreateInputLayout_GS();
	billPsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	billPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	Device->CreateGraphicsPipelineState(&billPsoDesc, IID_PPV_ARGS(&BillboardPSO));


	//Particle PSO ������̳� ��ƼŬȿ������ ����. ������� ����ѵ� PS�� �ణ�ٸ���.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC particlePsoDesc = d3dPipelineStateDesc;
	particlePsoDesc.VS = CreateVertexShader(L"shaders/GeometryShader.hlsl", "VS", "vs_5_0");
	particlePsoDesc.GS = CreateGeometryShader(L"shaders/GeometryShader.hlsl", "GS", "gs_5_0");
	particlePsoDesc.PS = CreatePixelShader(L"shaders/GeometryShader.hlsl", "PS2", "ps_5_0");
	particlePsoDesc.InputLayout = CreateInputLayout_GS();
	particlePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	particlePsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	Device->CreateGraphicsPipelineState(&particlePsoDesc, IID_PPV_ARGS(&ParticlePSO));


	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs)
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;


}

void Shader::SetShader(ID3D12GraphicsCommandList* commandlist, bool isBlend)
{
	if (isBlend == false)
		commandlist->SetPipelineState(PSO.Get());
	else
		commandlist->SetPipelineState(BlendPSO.Get());

}
void Shader::SetBulletShader(ID3D12GraphicsCommandList* commandlist)
{
	commandlist->SetPipelineState(BulletPSO.Get());
}

void Shader::SetSkyShader(ID3D12GraphicsCommandList * commandlist)
{
	commandlist->SetPipelineState(SkyPSO.Get());
}

void Shader::SetBillboardShader(ID3D12GraphicsCommandList * commandlist)
{
	commandlist->SetPipelineState(BillboardPSO.Get());
}

void Shader::SetParticleShader(ID3D12GraphicsCommandList * commandlist)
{
	commandlist->SetPipelineState(ParticlePSO.Get());
}



void Shader::Render(ID3D12GraphicsCommandList * CommandList, const GameTimer& gt)
{
	//���� �̷��� �׸��� ���� �׸������� �ǹٸ� PSO�� �����ϰ�, ������Ʈ���� �׸���.
	//������ ������� ���� ������, ����� ���� �׸��°� ���ٰ� �ǴܵǼ� ��ī�� ������Ʈ�� �����׸�
	SetSkyShader(CommandList);
	(*SkyObject)->Render(CommandList, gt);

	SetShader(CommandList, false);

	for (auto b = LandObject->cbegin(); b != LandObject->cend(); b++)
	{

		(*b)->Render(CommandList, gt);
	}

	//���� �̷��� �׸��� ���� �׸������� �ǹٸ� PSO�� �����ϰ�, ������Ʈ���� �׸���.
	SetShader(CommandList, false);
	for (auto b = DynamicObject->cbegin(); b != DynamicObject->cend(); b++)
	{
		if (isRender(*b) == true)
		{
			if ((*b)->Blending)
			{
				//������ PSO ����
				SetShader(CommandList, true);
				//������ PSO�� �׸�
				(*b)->Render(CommandList, gt);
			}
			else//���� �Ⱦ�
			{

				(*b)->Render(CommandList, gt);


			}
		}
	}



	//���� �̷��� �׸��� ���� �׸������� �ǹٸ� PSO�� �����ϰ�, ������Ʈ���� �׸���.
	//����ü���� �׸���.
	SetShader(CommandList, false);
	for (auto b = BulletObject->cbegin(); b != BulletObject->cend(); b++)
	{
		if (isRender(*b) == true)
		{
			if ((*b)->Blending)
			{
				//������ PSO ����
				SetBulletShader(CommandList);
				//������ PSO�� �׸�
				(*b)->Render(CommandList, gt);
				//�ٽ� ������ PSO�� ����
				SetShader(CommandList, false);
			}
			else//���� �Ⱦ�
			{
				if (ishalfalphaRender(*b) == false)
				{
					SetBulletShader(CommandList);
					(*b)->Render(CommandList, gt);
				}
				else
				{
					auto tempv = (*b)->ObjData.BlendValue;
					(*b)->ObjData.BlendValue = 0.5f;
					SetBulletShader(CommandList);
					//������ PSO�� �׸�1
					(*b)->Render(CommandList, gt);
					//�ٽ� ������ PSO�� ����
					SetShader(CommandList, false);
					(*b)->ObjData.BlendValue = tempv;
				}
			}
		}
	}


	//============ ����ƽ������Ʈ�� ������ٵ� ������Ʈ�� ������ �Ͼ�Ƿ�, ����� -> ���� ������Ʈ ������ �׷����Ѵ�. ==================//
	vector<CGameObject*> NoBlendingVector;//�����Ⱦ�
	vector<CGameObject*> BlendingVector;//���ʿ� �������� ��ü
	vector<CGameObject*> HalfBlendingVector;//���� �Ⱦ��µ� �������� �ӽ������� ������ ��ü

	//������ �ٵ�

	for (auto b = RigidObject->cbegin(); b != RigidObject->cend(); b++)
	{
		if (isRender(*b) == true)
		{
			if ((*b)->Blending)
			{
				BlendingVector.push_back(*b);
			}
			else//���� �Ⱦ�
			{
				if (ishalfalphaRender(*b) == false)
					NoBlendingVector.push_back(*b);
				else
					HalfBlendingVector.push_back(*b);


			}
		}
	}

	for (auto b = StaticObject->cbegin(); b != StaticObject->cend(); b++)
	{
		
			if ((*b)->Blending)
			{
				BlendingVector.push_back(*b);
			}
			else//���� �Ⱦ�
			{
				if (isRender(*b) == true)
				{

					if (ishalfalphaRender(*b) == false)
						NoBlendingVector.push_back(*b);
					else
						HalfBlendingVector.push_back(*b);

				}
			}
		
	}

	//������ �ȵǴ� ������Ʈ.
	SetShader(CommandList, false);
	for (auto b = NoBlendingVector.cbegin(); b != NoBlendingVector.cend(); b++)
	{
		(*b)->Render(CommandList, gt);
	}

	//���� ���� ������Ʈ
	SetShader(CommandList, true);
	for (auto b = BlendingVector.cbegin(); b != BlendingVector.cend(); b++)
	{
		(*b)->Render(CommandList, gt);
	}

	//�������� ���� ������Ʈ.
	SetShader(CommandList, true);
	for (auto b = HalfBlendingVector.cbegin(); b != HalfBlendingVector.cend(); b++)
	{
		//���� �������� ����.
		auto tempblend = (*b)->ObjData.BlendValue;

		//���������� �������� ����.
		(*b)->ObjData.BlendValue = 0.4f;
		//�׸���
		(*b)->Render(CommandList, gt);
		//���� ���� ���󺹱�
		(*b)->ObjData.BlendValue = tempblend;
	}

	// ========================= ��ƼŬ ======================================//
	SetParticleShader(CommandList);
	for (auto b = BbObject->cbegin(); b != BbObject->cend(); b++)
	{
		(*b)->Render(CommandList, gt);
	}



	//�÷��̾��� ����������Ʈ.
	SetShader(CommandList, true);
	player->TraceObject->Render(CommandList, gt);

}

bool Shader::isRender(CGameObject * obj)
{
	if (player != NULL)
	{
		auto v = Float3Add(XMFloat4to3(obj->CenterPos), player->Camera.CamData.EyePos, false);


		float l = FloatLength(v);
		if (l > player->Camera.mFarZ)//�ִ�� �����ִ°ͺ��� �ָ�������
			return false;

		v = Float3Normalize(v);
		auto ae = Float3Add(player->Camera.CamData.At, player->Camera.CamData.EyePos, false);
		auto d = ae.x*v.x + ae.y*v.y + ae.z*v.z;
		if (d < -0.1)//���� ����� �ڿ��������
			return false;
	}

	return true;

}

bool Shader::ishalfalphaRender(CGameObject * obj)
{

	if (player != NULL)
	{
		//�÷��̾ ī�޶� ���ϴ� ����
		auto v = Float3Add(player->Camera.CamData.EyePos, XMFloat4to3(obj->CenterPos), false);
		//�÷��̾ �ش� �繰�� ���ϴ� ����
		auto v2 = Float3Add(XMFloat4to3(obj->CenterPos), XMFloat4to3(player->PlayerObject->CenterPos), false);

		auto n = Float3Normalize(v);
		auto n2 = Float3Normalize(v2);

		//��������� 0�̻��̸� ī�޶�� �÷��̾�� �ڿ��ְ�, �繰�� �÷��̾�� �ڿ������Ƿ�, ������ȭ.
		auto d = n2.x*n.x + n2.y*n.y + n2.z*n.z;
		if (d > 0.0)
			return true;


		//�׷����� ���� ���̳� �ǹ�ó�� ���� ���� ��ġ�� �������ִ°�� �и��� �÷��̾�� �ڿ������� ������ ����ϸ� 90���� �Ѿ ����������찡 ����.
		//�̸����� ī�޶󿡼� �÷��̾�� ��ü�� ���� y���� 0���� �ΰ�, �Ÿ��� ����ؼ� �÷��̾ �ǹ��̳� �̷��ź��� �տ������� �ǹ��� �������ϰ��Ѵ�.

		auto n3 = player->Camera.CamData.Look.x*player->PlayerObject->CenterPos.x + player->Camera.CamData.Look.y*player->PlayerObject->CenterPos.y + player->Camera.CamData.Look.z*player->PlayerObject->CenterPos.z;
		auto n4 = player->Camera.CamData.Look.x*obj->CenterPos.x + player->Camera.CamData.Look.y*obj->CenterPos.y + player->Camera.CamData.Look.z*obj->CenterPos.z;

		if (n3 > n4)
			return true;
		//
		//auto v3 = Float3Add(player->Camera.CamData.EyePos, XMFloat4to3(player->PlayerObject->CenterPos), false);
		//v.y = 0;
		//v3.y = 0;

		//

		////�ǹ����� �÷��̾ �տ������� ������ȭ
		//if (FloatLength(v) < FloatLength(v3))
		//	return true;



	}
	return false;
}

D3D12_INPUT_LAYOUT_DESC Shader::CreateInputLayout()
{
	UINT nInputElementDescs = 10;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "STARTWEIGHT", 0, DXGI_FORMAT_R32_FLOAT, 0, 36,
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

D3D12_INPUT_LAYOUT_DESC Shader::CreateInputLayout_GS()
{
	UINT nInputElementDescs = 1;
	D3D12_INPUT_ELEMENT_DESC *pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}



D3D12_SHADER_BYTECODE Shader::CreateVertexShader(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile)
{
	return(Shader::CompileShaderFromFile(shaderfile, SName, Profile,
		VertexShader.GetAddressOf()));
}
D3D12_SHADER_BYTECODE Shader::CreatePixelShader(WCHAR *shaderfile, LPCSTR SName, LPCSTR Profile)
{
	return(Shader::CompileShaderFromFile(shaderfile, SName, Profile,
		PixelShader.GetAddressOf()));
}

D3D12_SHADER_BYTECODE Shader::CreateGeometryShader(WCHAR * shaderfile, LPCSTR SName, LPCSTR Profile)
{
	return(Shader::CompileShaderFromFile(shaderfile, SName, Profile,
		GeometryShader.GetAddressOf()));
}


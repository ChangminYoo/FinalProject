#include "CMesh.h"


CMesh::CMesh(void)
{
	

	nStride = 0;
	nOffset = 0;
	nVertex = 0;

	SubResource = NULL;
	Index = NULL;
	
}


CMesh::~CMesh(void)
{

	if (nVertex > 1)
	{
		if (SubResource)
			delete[] SubResource;

		if (Index)
			delete[] Index;

	}
	else
	{
		if (SubResource)
			delete SubResource;

		if (Index)
			delete Index;
	}

	if (jarr != NULL)
		delete jarr;
}

void CMesh::CreateVertexBuffer(ID3D12Device* m_Device,ID3D12GraphicsCommandList* commandlist)
{
	VertexBuffer= d3dUtil::CreateDefaultBuffer(m_Device,
		commandlist,(void*)SubResource,nStride*nVertex, VertexInitBuffer);
	
}
void CMesh::CreateIndexBuffer(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)
{
	IndexBuffer = d3dUtil::CreateDefaultBuffer(m_Device,
		commandlist, (void*)Index, nisize*nindex, IndexInitBuffer);
}
void CMesh::Render(ID3D12GraphicsCommandList* commandlist)
{
	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = nStride;
	vbv.SizeInBytes = nStride * nVertex;

	commandlist->IASetVertexBuffers(0, 1, &vbv);

	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R32_UINT;
	ibv.SizeInBytes = nisize * nindex;

	commandlist->IASetIndexBuffer(&ibv);
	commandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	
	commandlist->DrawIndexedInstanced(nindex,1, nioffset,nOffset, 0);
	
	
}


void CMesh::SetNormal(bool particle)//이녀석은 반드시 정점과 인덱스를 다 값을 채운뒤, 호출하며 버퍼생성전에 호출!
{
	for (int i = 0; i<(nindex / 3); i++)
	{
		int i0 = Index[i * 3 + 0];
		int i1 = Index[i * 3 + 1];
		int i2 = Index[i * 3 + 2];

		CVertex v0 = SubResource[i0];
		CVertex v1 = SubResource[i1];
		CVertex v2 = SubResource[i2];
		XMVECTOR v1v = XMVectorSet(v1.V.x, v1.V.y, v1.V.z,0);
		XMVECTOR v0v = XMVectorSet(v0.V.x, v0.V.y, v0.V.z, 0);
		XMVECTOR v2v = XMVectorSet(v2.V.x, v2.V.y, v2.V.z, 0);

		XMFLOAT3 vec0;
		XMFLOAT3 vec1;

		XMStoreFloat3(&vec0, v1v - v0v);
		XMStoreFloat3(&vec1, v2v - v0v);

		XMVECTOR Norm;
		
		Norm = XMVector3Cross(XMLoadFloat3(&vec0), XMLoadFloat3(&vec1));


		XMVECTOR n = Norm;
	
		XMFLOAT4 fn;
		XMStoreFloat4(&fn, n);

		fn.w = 0;
		//각정점별로 계산된 노멀을 더해줌

		SubResource[i0].N.x += fn.x; SubResource[i0].N.y += fn.y; SubResource[i0].N.z += fn.z;
		SubResource[i1].N.x += fn.x; SubResource[i1].N.y += fn.y; SubResource[i1].N.z += fn.z;
		SubResource[i2].N.x += fn.x, SubResource[i2].N.x += fn.y; SubResource[i2].N.z += fn.z;

	}

	//이후 각정점의 노멀을 단위화 하면됨.

	for (int i = 0; i<nVertex; i++)
	{
		auto tvn = XMVector4Normalize(XMLoadFloat4(&SubResource[i].N));
		
		XMStoreFloat4(&SubResource[i].N, tvn);

		//D3DXVec4Normalize(&SubResource[i].N, &SubResource[i].N);
		if (particle == true)
		{
			srand(time(0));
			SubResource[i].N.w = ((float)(rand() % 1000 + 1)) / 1000;
		}
	}
}

/*
void CMesh::SetFieldTangent()
{
	XMFLOAT3 d = SubResource[0].N;
	XMFLOAT3 tn(0, 1, 0);
	D3DXVec3Cross(&tn, &d, &tn);
	D3DXVec3Normalize(&tn, &tn);

	if (tn.x == 0 && tn.y == 0 && tn.z <= 0)
		tn = XMFLOAT3{ 1,0,0 };

	if (tn.x < 0 || tn.y < 0 || tn.z < 0)
		tn = -tn;

	D3DXVec3Normalize(&tn, &tn);


	for (int i = 0; i < 4; ++i)
		SubResource[i].Tangent = tn;

}
*/
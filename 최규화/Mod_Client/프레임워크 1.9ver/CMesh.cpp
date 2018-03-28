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


void CMesh::SetNormal(bool particle)//�̳༮�� �ݵ�� ������ �ε����� �� ���� ä���, ȣ���ϸ� ���ۻ������� ȣ��!
{
	for (int i = 0; i<(nindex / 3); i++)
	{
		int i0 = Index[i * 3 + 0];
		int i1 = Index[i * 3 + 1];
		int i2 = Index[i * 3 + 2];

		CVertex v0 = SubResource[i0];
		CVertex v1 = SubResource[i1];
		CVertex v2 = SubResource[i2];

		XMVECTOR V0 = XMLoadFloat3(&v0.V);
		XMVECTOR V1 = XMLoadFloat3(&v1.V);
		XMVECTOR V2 = XMLoadFloat3(&v2.V);

		XMVECTOR N0 = XMLoadFloat4(&v0.N);
		XMVECTOR N1 = XMLoadFloat4(&v1.N);
		XMVECTOR N2 = XMLoadFloat4(&v2.N);


		XMVECTOR vec0=V1-V0;
		XMVECTOR vec1=V2-V0;

		
		XMVECTOR Norm;
		Norm = XMVector3Cross(vec0, vec1);
		
		//���������� ���� ����� ������
		XMStoreFloat4(&SubResource[i0].N, N0 + Norm);
		XMStoreFloat4(&SubResource[i1].N, N1 + Norm);
		XMStoreFloat4(&SubResource[i2].N, N2 + Norm);

		SubResource[i0].N.w = 0;
		SubResource[i1].N.w = 0;
		SubResource[i2].N.w = 0;
		
	}

	//���� �������� ����� ����ȭ �ϸ��.

	for (int i = 0; i<nVertex; i++)
	{
		XMVECTOR N = XMLoadFloat4(&SubResource[i].N);
		N=XMVector4Normalize(N);
		XMStoreFloat4(&SubResource[i].N, N);

		if (particle == true)
		{

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
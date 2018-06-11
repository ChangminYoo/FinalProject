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


void CMesh::SetNormal(bool PlayerMesh,bool particle)//�̳༮�� �ݵ�� ������ �ε����� �� ���� ä���, ȣ���ϸ� ���ۻ������� ȣ��!
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
		N=XMVector3Normalize(N);
		
		XMStoreFloat4(&SubResource[i].N, N);
		
		if (PlayerMesh == true)//���� �������ִ� �÷��̾� �޽� ����� �� �̻��ϰ� �Ǿ�����. x��z�� �ٲ���ִ�.
		{
			auto x = SubResource[i].N.x;
			SubResource[i].N.x =- SubResource[i].N.z;
			SubResource[i].N.z = x;
		}
		


		if (particle == true)
		{

			SubResource[i].N.w = ((float)(rand() % 1000 + 1)) / 1000;
		}
	}

	if (!particle)
	{
		SetTangent();
	}

}


void CMesh::SetTangent()
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

		XMVECTOR T0 = XMLoadFloat3(&v0.Tangent);
		XMVECTOR T1 = XMLoadFloat3(&v1.Tangent);
		XMVECTOR T2 = XMLoadFloat3(&v2.Tangent);



		XMVECTOR vec0 = V1 - V0;
		XMVECTOR vec1 = V2 - V0;

		XMFLOAT3 vector0;
		XMFLOAT3 vector1;

		XMStoreFloat3(&vector0, vec0);
		XMStoreFloat3(&vector1, vec1);


		XMFLOAT2 tuVector;//X��
		XMFLOAT2 tvVector;//Y��

		//tuVector�� x�� v1-v0�� x�� y �� v2-v0�� x��.
		//tvVector�� x�� v1-v0�� y�� y �� v2-v0�� y��.

		tuVector.x = v1.Tex.x - v0.Tex.x; 
		tvVector.x = v1.Tex.y - v0.Tex.y;
		
		tuVector.y = v2.Tex.x - v0.Tex.x;
		tvVector.y = v2.Tex.y - v0.Tex.y;

		float den = 1.0f / (tuVector.x * tvVector.x - tuVector.y * tvVector.y);

		XMFLOAT3 tangent;
		tangent.x = (tvVector.y * vector0.x - tvVector.x * vector1.x) * den;
		tangent.y = (tvVector.y * vector0.y - tvVector.x * vector1.y) * den;
		tangent.z = (tvVector.y * vector0.z - tvVector.x * vector1.z) * den;


		

	
		XMStoreFloat3(&SubResource[i0].Tangent, XMLoadFloat3(&tangent));
		XMStoreFloat3(&SubResource[i1].Tangent, XMLoadFloat3(&tangent));
		XMStoreFloat3(&SubResource[i2].Tangent, XMLoadFloat3(&tangent));

		

	}

	//���� �������� ź��Ʈ�� ����ȭ �ϸ��.

	for (int i = 0; i<nVertex; i++)
	{

		XMVECTOR T = XMLoadFloat3(&SubResource[i].Tangent);
		T = XMVector3Normalize(T);

		XMStoreFloat3(&SubResource[i].Tangent, T);
		
		auto v=SubResource[i];
	}

}

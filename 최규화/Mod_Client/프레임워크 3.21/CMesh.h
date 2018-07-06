#pragma once
#include"CVertex.h"
#include"utility.h"
#include"UploadBuffer.h"
using namespace std;


class CMesh
{
public:
	CVertex * SubResource;	//������ �迭. ���긮�ҽ���. �������ۿ� ä������
	
	UINT* Index;	//�ε��� �迭

	ComPtr<ID3D12Resource> VertexBuffer;//��������
	ComPtr<ID3D12Resource>IndexBuffer;	//�ε��� ����

	ComPtr<ID3D12Resource> VertexInitBuffer;//�������� �ʱ�ȭ�� ����
	ComPtr<ID3D12Resource>IndexInitBuffer;	//�ε��� ���� �ʱ�ȭ�� ����

	
	UINT nStride;	//������ ������
	UINT nVertex;	//������ ����
	UINT nOffset;	//������ ������ġ

	UINT nioffset;	//�ε��� ������
	UINT nindex;	//�ε��� ����
	UINT nisize;	//�ε���������

	
	

	bool isMd5Mesh = false;//�ܺο��� �ҷ��� �޽��ΰ�?

	//���� �ܺο��� �ҷ��� �޽����
	vector<Weight> weights;
	vector<Joint> joints;
	int numJoints;
	int numWeights;
	int numTriangles;

	

public:
	CMesh();
	~CMesh(void);

	void Render(ID3D12GraphicsCommandList* commandlist);
	void CreateVertexBuffer(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	void CreateIndexBuffer(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	void SetNormal(bool PlayerMesh=false,bool particle = false);//����� ����Ѵ�.
	void SetTangent();//ź��Ʈ�� �����
};


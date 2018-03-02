#pragma once
#include"CVertex.h"
#include"utility.h"
#include"UploadBuffer.h"
using namespace std;
#define MAXJOINT 65

struct JointArr
{
	ShaderJoint j[MAXJOINT];

};
class CMesh
{
public:
	CVertex * SubResource;	//정점의 배열. 서브리소스다. 정점버퍼에 채워질값
	UINT* Index;	//인덱스 배열

	ComPtr<ID3D12Resource> VertexBuffer;//정점버퍼
	ComPtr<ID3D12Resource>IndexBuffer;	//인덱스 버퍼

	ComPtr<ID3D12Resource> VertexInitBuffer;//정점버퍼 초기화용 버퍼
	ComPtr<ID3D12Resource>IndexInitBuffer;	//인덱스 버퍼 초기화용 버퍼

	
	UINT nStride;	//정점의 사이즈
	UINT nVertex;	//정점의 갯수
	UINT nOffset;	//정점의 시작위치

	UINT nioffset;	//인덱스 오프셋
	UINT nindex;	//인덱스 갯수
	UINT nisize;	//인덱스사이즈

	
	

	bool isMd5Mesh = false;//외부에서 불러온 메쉬인가?

	//만약 외부에서 불러온 메쉬라면
	vector<Weight> weights;
	vector<Joint> joints;
	int numJoints;
	int numWeights;
	int numTriangles;

	UploadBuffer<JointArr>* jarr = NULL;


public:
	CMesh();
	~CMesh(void);

	void Render(ID3D12GraphicsCommandList* commandlist);
	void CreateVertexBuffer(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	void CreateIndexBuffer(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	void SetNormal(bool particle = false);//노멀을 계산한다.
	//void SetFieldTangent();//탄젠트를 계산함
};


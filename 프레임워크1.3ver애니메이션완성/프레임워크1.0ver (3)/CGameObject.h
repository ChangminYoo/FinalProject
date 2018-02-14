#pragma once
#include"CMesh.h"
#include"UploadBuffer.h"
#include"Timer.h"
#include"CTexture.h"

struct ObjectData
{
	XMFLOAT4X4 WorldMatrix;
	float Scale;
	//XMFLOAT4 CustomData1;
	//XMFLOAT4 CustomData2;
};

class CGameObject
{
public:
	CGameObject();
	~CGameObject();
	ID3D12GraphicsCommandList* commandlist;//그래픽커맨드리스트
protected:
	CMesh * Mesh=NULL;

	ObjectData ObjData;

	UploadBuffer<ObjectData>* ConstBuffer=NULL;	//월드행렬과 커스텀데이터를 저장하기위한 버퍼

public:

	
	XMFLOAT4 CenterPos;//중점

	XMFLOAT3 Lookvector;//룩벡터. 오브젝트가 바라보고있는 방향.

	float ThetaY;	//Y축회전용 각도

	float currAnimTime = 0;//현재애니메이션시간
	std::vector<ModelAnimation> animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
	int n_Animation = 0;//몇번째 애니메이션을 쓸건가?
	
	int TickValue =0;//애니메이션 호출 프레임
	
	UploadBuffer<CVertex>* DynamicVertexBuffer=NULL;// (애니메이션용)

	CGameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist,bool animation,XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	virtual void SetWorldMatrix();//월드매트릭스 설정.
	
	virtual void Render(ID3D12GraphicsCommandList* commandlist);
	virtual void Tick(const GameTimer& gt);
	virtual void CreateConstBuffer(ID3D12Device* m_Device);
	virtual void UpdateConstBuffer(ID3D12GraphicsCommandList* commandlist);
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성

	void SetTexture(ID3D12GraphicsCommandList* commandlist);
	void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist, string texturename, wstring FileName);
	
	unordered_map<string, unique_ptr<CTexture>> Textures; //나중에 스테틱으로

	ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap = nullptr;
	
};


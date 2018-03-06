#pragma once
#include"CMesh.h"
#include"UploadBuffer.h"
#include"Timer.h"
#include"CTexture.h"
#include"CMaterial.h"
#include"MMPE\MyMiniPysicsEngine.h"
struct ObjectData
{
	XMFLOAT4X4 WorldMatrix;
	float Scale;
	bool isAnimation = false;
	//XMFLOAT4 CustomData1;
	//XMFLOAT4 CustomData2;
};


class CGameObject//이 클래스를 기본으로 상속받아 다른 오브젝트를 만듬. ex) 검사오브젝트, 좀비오브젝트 등
{
public:
	CGameObject();
	~CGameObject();
	ID3D12GraphicsCommandList* commandlist;//그래픽커맨드리스트
	ObjectData ObjData;//월드행렬과 기타 데이터
	PhysicsPoint* pp = NULL;//질점 오브젝트. 오브젝트는 질점오브젝트와 강체오브젝트로 나뉘며 하나만 사용한다.
	bool staticobject = false;//고정된 건물같은 객체인가?
	list<CGameObject*>* CollisionList=NULL;

protected:

	
	UploadBuffer<ObjectData>* ConstBuffer=NULL;	//월드행렬과 커스텀데이터를 저장하기위한 버퍼

public:

	//위치와 회전정보
	XMFLOAT4 CenterPos;//중점
	float ThetaY;	//Y축회전용 각도
	bool AirBone = false;
	//룩벡터와 라이트벡터
	XMFLOAT3 Lookvector;//룩벡터. 오브젝트가 바라보고있는 방향.
	XMFLOAT3 Rightvector;//라이트벡터. 오브젝트가 바라보고있는 방향의 오른쪽방향.
	XMFLOAT3 OffLookvector;//초기룩벡터. 오브젝트가 바라보고있는 방향.
	XMFLOAT3 OffRightvector;//초기라이트벡터. 오브젝트가 바라보고있는 방향의 오른쪽방향.

	XMFLOAT3 GetUpvector();//룩벡터와 라이트벡터를 이용해 업벡터를 얻는함수

	//애니메이션 관련데이터
	int n_Animation = 0;//몇번째 애니메이션을 쓸건가?
	float currAnimTime = 0;//현재애니메이션시간
	int TickValue =0;//애니메이션 호출 프레임
	

	//기타 공용 데이터들
	bool DelObj = false;//이게 참이면 실제로 제거된다.
	float Speed = 0;
	RayCastObject rco;//레이캐스트 오브젝트
	bool Blending = false;
	
	CGameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist,XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	virtual void SetWorldMatrix();//월드매트릭스 설정.
	
	virtual void CreateConstBuffer(ID3D12Device* m_Device);
	virtual void UpdateConstBuffer(ID3D12GraphicsCommandList* commandlist);
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)=0;//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist) {}
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime)=0;
	bool Texturing = false;//로드 텍스처를 호출하면 자동으로 true가됨.
	void UpdatePPosCenterPos();//질점의 중점으로 센터포즈를 변경함. 반드시 충돌하거나 하면 처리해야함.
	void UpdateLookVector();//항상호출해줄것.
	
	//게임오브젝트를 상속받고 이것들을 모두 static으로 생성할것.
	//bool CrateMesh;
	//unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	//CMesh * Mesh = NULL;//나중에 메쉬는 static으로 둬야함
	//std::vector<ModelAnimation> animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
	//ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap = nullptr;//텍스처 용 힙

	
	
};
void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap);
void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist, CGameObject* obj, unordered_map<string, unique_ptr<CTexture>>& Textures, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, string texturename, wstring FileName);

//상속받은 게임오브젝트는 다음과 같은것을 처리할것.
//1. 생성자
//2. 틱함수(게임오브젝트마다 다른 행동을 할테니까)
//3. 메쉬와 텍스처, 애니메이션 벡터 등을 static으로 만듬
//4. 렌더함수를 그대로 가져옴. 왜 이러냐면 static Mesh를 써야하기때문이다..
//5. 콜리전함수 추가해야함. 질점오브젝트와 강체오브젝트는 서로 다르게 생성해야함.(우선 질점오브젝트밖에 없음.)

class CZombieObject : public CGameObject
{
public:
	CZombieObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static std::vector<ModelAnimation> animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap ;//텍스처 용 힙

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	
};


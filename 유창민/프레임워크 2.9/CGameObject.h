#pragma once
#include"CMesh.h"
#include"UploadBuffer.h"
#include"Timer.h"
#include"CTexture.h"
#include"CMaterial.h"
#include"MakeGeometry.h"
#include"MMPE\MyMiniPysicsEngine.h"

#define CUBEMAN_HALFWIDTH 3
#define CUBEMAN_HALFEIGHT 10
#define BULLET_SIZE 1

enum Obj_State
{
	Dynamic=0,
	Static,
	Wall,
	Particle,
	Bullet,
	Rigid,
	UI
};

enum Ani_State 
{
	Idle = 0,
	Run,
	Attack,
	Dead,
	Victory,
	Crying
	
};

struct ObjectData
{
	XMFLOAT4X4 WorldMatrix = MathHelper::Identity4x4();
	float Scale;
	float SpecularParamater=1;
	int isAnimation = 0;
	float BlendValue = 1.0f;//최종 블렌딩값
	XMFLOAT4 CustomData1 = XMFLOAT4{ 0,0,0,0 };//오브젝트별 특화 데이터.
	//XMFLOAT4 CustomData2;
	float PTime = 0.0f;
	XMFLOAT3 Velocity = XMFLOAT3{ 0,0,0 };
};


struct GameData //게임캐릭터의 정보 흔히 알고있는 체력, 데미지, 스피드가 있다.
{
	float MAXHP=0;
	float HP=0;
	float Damage=0;
	float Speed=0;
	bool GodMode = false;
};

class CGameObject//이 클래스를 기본으로 상속받아 다른 오브젝트를 만듬. ex) 검사오브젝트, 좀비오브젝트 등
{
public:
	//오브젝트는 반드시 디바이스,커맨드리스트,파티클리스트,포지션을 받는다.
	//왜파티클 리스트가 필요하냐면 충돌이나 마법효과에서 파티클을 넣기 위해서다.
	CGameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject();
	virtual ~CGameObject();
	
	ID3D12Device* device;//디바이스
	ID3D12GraphicsCommandList* commandlist;//그래픽커맨드리스트
	ObjectData ObjData;//월드행렬과 기타 데이터
	RigidBody* rb = NULL;//리지드 바디 오브젝트. 강체오브젝트는 아직 미완성. 테스트및 구현중이다.
	PhysicsPoint* pp = NULL;//질점 오브젝트. 오브젝트는 질점오브젝트와 강체오브젝트로 나뉘며 하나만 사용한다.
	bool staticobject = false;//고정된 건물같은 객체인가?
	
	list<CGameObject*>* CollisionList=NULL;
	list<CGameObject*>* ParticleList = NULL;
protected:
	
	UploadBuffer<ObjectData>* ConstBuffer=NULL;	//월드행렬과 커스텀데이터를 저장하기위한 버퍼

public:

	//위치와 회전정보
	XMFLOAT4 CenterPos;//중점
	XMFLOAT4 Orient;//방향을 나타낸다.
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
	UploadBuffer<JointArr>* jarr = NULL;//계산된 조인트들이 저장되는 상수버퍼. 왜이게 스테틱이 아니냐면
	//1번 오브젝트가 애니메이션을 업데이트를 한뒤 연결을 한다음, 2번 오브젝트가 애니메이션을 다시 업데이트하면
	//2번 오브젝트가 가지는 애니메이션이 최종적인 jarr가되므로 그걸 그리게 된다. 따라서 각자 갖게해야함.


	//기타 공용 데이터들
	bool DelObj = false;//이게 참이면 실제로 제거된다.
	bool Blending = false;
	//벽들에 굳이 마우스를 움직일때마다 체크할 필요는 없으므로 추가함. 또 벽은 또 벽대로 뭔가 처리할게 있을것같음.
	Obj_State obs = Dynamic;


	GameData gamedata;
	RayCastObject rco;//레이캐스트 오브젝트
	CGameObject* LockOn=NULL;

	
	//기타 공용 함수들
	virtual void SetWorldMatrix();//월드매트릭스 설정.
	
	virtual void CreateConstBuffer(ID3D12Device* m_Device);
	virtual void UpdateConstBuffer(ID3D12GraphicsCommandList* commandlist);
	
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)=0;//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime)=0;
	
	virtual void ToDamage(float Damage);
	virtual void ToDead();
	
	void UpdatePPosCenterPos();//질점의 중점으로 센터포즈를 변경함. 반드시 충돌하거나 하면 처리해야함.
	void UpdateLookVector();//항상호출해줄것.
	
	void SetAnimation(int n_Ani);//몇번째 애니메이션을 쓸건가?
	virtual void EndAnimation(int nAni) {}//애니메이션 종료되었음을 알린다.
	//게임오브젝트를 상속받고 이것들을 모두 static으로 생성할것.
	//bool CrateMesh;
	//unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	//CMesh * Mesh = NULL;//나중에 메쉬는 static으로 둬야함
	//std::vector<ModelAnimation> animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
	//ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap = nullptr;//텍스처 용 힙

	
	
};
void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, ID3D12Resource* texture, bool isCubeMap, int Offset = 0);
void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist, CGameObject* obj, unordered_map<string, unique_ptr<CTexture>>& Textures, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, string texturename, wstring FileName, bool isCubeMap, int NumDescriptors = 1, int Offset = 0);

//상속받은 게임오브젝트는 다음과 같은것을 처리할것.
//1. 생성자
//2. 틱함수(게임오브젝트마다 다른 행동을 할테니까)
//3. 메쉬와 텍스처, 애니메이션 벡터 등을 static으로 만듬
//4. 렌더함수를 그대로 가져옴. 왜 이러냐면 static Mesh를 써야하기때문이다..
//5. 콜리전함수 추가해야함. 질점오브젝트와 강체오브젝트는 서로 다르게 생성해야함.(우선 질점오브젝트밖에 없음.)

class BarObject : public CGameObject
{
public:
	BarObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, CGameObject* master, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//소유자
public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};

class BarFrameObject : public CGameObject
{
public:
	BarFrameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, CGameObject* master, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//소유자
public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};
//============ 캐릭터 ==========//
class CCubeManObject : public CGameObject
{
public:
	CCubeManObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~CCubeManObject();
	BarObject* Hpbar = NULL;
	BarFrameObject* HPFrame = NULL;
public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static std::vector<ModelAnimation> animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap ;//텍스처 용 힙
		
	virtual void ToDead();
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void EndAnimation(int nAni);

};



//---------------------- 투 사 체 -----------------------------//
class ParticleObject;

class BulletCube : public CGameObject
{
public:
	BulletCube(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist,CGameObject* master,XMFLOAT4& ori,CGameObject* lockon=NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~BulletCube();
	CGameObject* Master = NULL;//소유자
	CGameObject* LockOn = NULL;//유도시사용됨
	float LifeTime = 10;//생존시간. 10초 후 제거됨
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};


class HeavyBulletCube : public CGameObject
{
public:
	HeavyBulletCube(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~HeavyBulletCube();
	CGameObject* Master = NULL;//소유자
	CGameObject* LockOn = NULL;//유도시사용됨
	float LifeTime = 10;//생존시간. 10초 후 제거됨
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};

//사각형모양
class Tetris1 : public CGameObject
{
public:
	Tetris1(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~Tetris1();
	CGameObject* Master = NULL;//소유자

	float LifeTime = 10;//생존시간. 10초 후 제거됨
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};

// l 자 모양
class Tetris2 : public CGameObject
{
public:
	Tetris2(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~Tetris2();
	CGameObject* Master = NULL;//소유자

	float LifeTime = 10;//생존시간. 10초 후 제거됨
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};

// l 자 모양
class Tetris3 : public CGameObject
{
public:
	Tetris3(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~Tetris3();
	CGameObject* Master = NULL;//소유자

	float LifeTime = 10;//생존시간. 10초 후 제거됨
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};

// ㅗ 자 모양
class Tetris4 : public CGameObject
{
public:
	Tetris4(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~Tetris4();
	CGameObject* Master = NULL;//소유자

	float LifeTime = 10;//생존시간. 10초 후 제거됨
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};



class Tetrike : public CGameObject
{
public:
	Tetrike(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*Bulletlist, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~Tetrike();
	CGameObject* Master = NULL;//소유자
	CGameObject* LockOn = NULL;//유도시사용됨
	float LifeTime = 10;//생존시간. 10초 후 제거됨
	float ShotTime = 0;//0.25초마다 발동
	list<CGameObject*>* Blist = NULL;
public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};

//================= 스태틱 오브젝트 ===============//

class SphereObject : public CGameObject
{
public:
	SphereObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙

	
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime){}

};

class CubeObject : public CGameObject
{
public:
	CubeObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	int selectColor;
public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime){}

};

class MoveCubeObject : public CGameObject
{
public:
	MoveCubeObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, float rad, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	int selectColor;
	float Rad;
	float n = 0;
public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void Tick(const GameTimer& gt);
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};

class RigidCubeObject : public CGameObject
{
public:
	RigidCubeObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void Tick(const GameTimer& gt);
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};

class GridObject : public CGameObject
{
public:
	GridObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	
public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime){}

};


class SmallWallObject : public CGameObject
{
public:
	SmallWallObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, float dgree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};

class BigWallObject : public CGameObject
{
public:
	BigWallObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, float dgree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};


class BuildingObject : public CGameObject
{
public:
	BuildingObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, float dgree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};



class RangeObject : public CGameObject
{
public:
	RangeObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};

// ============================= Particle ==================================//

class ParticleObject : public CGameObject
{
public:
	ParticleObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, CGameObject* master, float lifeTime, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;
	float LifeTime = 0.2f;
	float ParticleTime = 0.0f;

public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime){}

};

class BloodParticleObject : public CGameObject
{
public:
	BloodParticleObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, CGameObject* master, float lifeTime, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;
	float LifeTime = 0.2f;
	float ParticleTime = 0.0f;

public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};


class DamageObject : public CGameObject
{
public:
	DamageObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, float damaged, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	float LifeTime = 1.5f;
	int damaged = 0;

public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime){}

};

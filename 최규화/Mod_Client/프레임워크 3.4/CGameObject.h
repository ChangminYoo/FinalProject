 #pragma once
#include"CMesh.h"
#include"UploadBuffer.h"
#include"Timer.h"
#include"CTexture.h"
#include"Light.h"
#include"CMaterial.h"
#include"MakeGeometry.h"
#include"MMPE\MyMiniPysicsEngine.h"
#include"server\main_client\stdafx.h"

#define CUBEMAN_HALFWIDTH 3
#define CUBEMAN_HALFEIGHT 10
#define BULLET_SIZE 1
class FSM;
enum Obj_State
{
	Dynamic=0,
	Static,
	Wall,
	Particle,
	Bullet,
	Rigid,
	UI,test
};

/*
enum Ani_State 
{
	Idle = 0,
	Run,
	Attack,
	Dead,
	Victory,
	Crying
	
};
*/

typedef struct PointRank
{
	int Point = 0; //현재 점수
	int Rank = 5;  //현재 랭킹
	int KillCount{ 0 };
	int DeathCount{ 0 };
	bool TopMode = false;//1위일때 탑모드. 현재 체력 + 800 / 크기 1.3배 / 이속 1.5배
	bool Init = false;//1위하면 1위모드로 초기화 해야함.
}PointRank;

struct ObjectData
{
	XMFLOAT4X4 WorldMatrix = MathHelper::Identity4x4();
	float Scale;
	float SpecularParamater=1;
	int isAnimation = 0;
	float BlendValue = 1.0f;//최종 블렌딩값
	//CustomData1의 w가 1234 이면 노멀매핑을 쓰는것.
	XMFLOAT4 CustomData1 = XMFLOAT4{ 0,0,0,0 };//오브젝트별 특화 데이터.
	//XMFLOAT4 CustomData2;
	float PTime = 0.0f;
	XMFLOAT3 Velocity = XMFLOAT3{ 0,0,0 };
	XMFLOAT4 TexClamp = XMFLOAT4{ 0, 1 ,0 ,0 };
};


struct GameData //게임캐릭터의 정보 흔히 알고있는 체력, 데미지, 스피드가 있다.
{
	float MAXHP=0;
	float HP=0;
	float Damage=0;
	float Speed=0;
	bool GodMode = false;
	
};

class CPlayer;

class CGameObject//이 클래스를 기본으로 상속받아 다른 오브젝트를 만듬. ex) 검사오브젝트, 좀비오브젝트 등
{
public:
	//오브젝트는 반드시 디바이스,커맨드리스트,파티클리스트,포지션을 받는다.
	//왜파티클 리스트가 필요하냐면 충돌이나 마법효과에서 파티클을 넣기 위해서다.
	CGameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject();
	virtual ~CGameObject();
	
	ID3D12Device* device;//디바이스
	ID3D12GraphicsCommandList* commandlist;//그래픽커맨드리스트
	ObjectData ObjData;//월드행렬과 기타 데이터
	RigidBody* rb = NULL;//리지드 바디 오브젝트. 강체오브젝트는 아직 미완성. 테스트및 구현중이다.
	PhysicsPoint* pp = NULL;//질점 오브젝트. 오브젝트는 질점오브젝트와 강체오브젝트로 나뉘며 하나만 사용한다.
	bool staticobject = false;//고정된 건물같은 객체인가?
	bool MinDepth = false;//UI가 아닌 오브젝트 에서 깊이를 가장 낮게(항상 최우선적으로) 그려야하는가?
	
	

	list<CGameObject*>* CollisionList=NULL;
	list<CGameObject*>* ParticleList = NULL;
	list<CGameObject*>* Shadow = NULL;

protected:
	
	UploadBuffer<ObjectData>* ConstBuffer=NULL;	//월드행렬과 커스텀데이터를 저장하기위한 버퍼

public:
	//서버에서 1등이라고 넘겨주면 능력치 강화.
	PointRank pointrank;

	//위치와 회전정보
	XMFLOAT4 CenterPos;//중점
	XMFLOAT4 Orient;//방향을 나타낸다.
	XMFLOAT4 OrgPos;//초창기 중점
	bool AirBone = false;

	char* TextureName = NULL;
	char* NTextureName = NULL;
	UINT TexOff = 0;
	UINT NTexOff = 0;

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
	bool DrawObj = true;
	bool Blending = false;
	bool PrevCool = false;
	bool isShieldOn = false;
	bool isHit1 = false;

	float TexStride = 0.f;

	//벽들에 굳이 마우스를 움직일때마다 체크할 필요는 없으므로 추가함. 또 벽은 또 벽대로 뭔가 처리할게 있을것같음.
	Obj_State obs = Dynamic;

	GameData gamedata;

	RayCastObject rco;//레이캐스트 오브젝트
	CGameObject* LockOn=NULL;

	//CMesh sMesh;
	//std::vector<ModelAnimation> sAnimations;
	//기타 공용 함수들
	virtual void SetWorldMatrix();//월드매트릭스 설정.
	virtual void SetShadowMatrix();

	virtual void CreateConstBuffer(ID3D12Device* m_Device);
	virtual void UpdateConstBuffer(ID3D12GraphicsCommandList* commandlist, bool isShadow);
	
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)=0;//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime)=0;
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) = 0;
	
	virtual void ToDamage(float Damage);
	virtual void ToDead();
	
	
	void UpdateLookVector();//항상호출해줄것.
	
	void SetAnimation(int n_Ani);//몇번째 애니메이션을 쓸건가?
	virtual void EndAnimation(int nAni) {}//애니메이션 종료되었음을 알린다.
	//게임오브젝트를 상속받고 이것들을 모두 static으로 생성할것.
	//bool CrateMesh;
	//unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	//CMesh * Mesh = NULL;//나중에 메쉬는 static으로 둬야함
	//std::vector<ModelAnimation> animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
	//ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap = nullptr;//텍스처 용 힙

public:
	//서버 추가
	static int g_numofdice;
	
	static int g_npcID;
	static int g_npcBulletID;
	bool	   isNPC{ false };

	//bullet ID
	static unsigned short myID;
	static bool			  m_IsBulletFirstCount;
	static list<unsigned short>    BulletIDList;
	int							   m_bullet_type;

	//HammerBullet ID - 따로 관리 - 기존 불렛과는 다르게 서버에서 해당 불렛 아이디를 변경시킬 수 있으므로
	static unsigned short m_hmBulletID;
	static bool			  m_hmBulletFirstCount;

	//서버->클라 관리용 캐릭터 데이터(GameObject에 넣어둬야함)
	Player_Data			 m_player_data;

	//서버->클라 관리용 NPC 데이터
	Npc_Data			 m_npc_data;

	//서버->클라 관리용 StaticObject 데이터
	StaticObject_Info	 m_sobj_data;

	//서버->클라 관리용 MoveStaticObject 데이터
	MoveObjectData       m_mvobj_data;

	//서버->클라 관리용 RigidbodyObject 데이터
	RigidbodyData		 m_rigidbody_data;

	//서버->클라 관리용 BulletObject 데이터
	STC_BulletObject_Info	 m_bullet_data;

	CPlayer* m_player{ nullptr };

	bool m_end_attack{ false }, m_end_die{ false };
	bool m_end_npc_attack{ false }, m_end_npc_die{ false };
	
	//불렛 파티클 타입
	int m_particle_type;

	float	m_degree;

	//파동파 스킬 마스터 아이디 - 서버연동 용
	int m_waveMasterID{ -1 };

};

void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, ID3D12Resource* texture, int texMap = 0, int Offset = 0);
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
	BarObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, float size, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//소유자
	float YPos;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class BarFrameObject : public CGameObject
{
public:
	BarFrameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, float size, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//소유자
	float YPos;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class Rank1Object : public CGameObject
{
public:
	Rank1Object(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class DiceObject : public CGameObject
{
public:
	DiceObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT3 goal, list<CGameObject*>* bulletlist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//소유자
	list<CGameObject*>* Bulletlist;
	ID3D12Device* Device;
	ID3D12GraphicsCommandList* Commandlist;
	list<CGameObject*>* plist;

	XMFLOAT3 V;

	int dTime = 0;
	float LifeTime = 3.0f;
	int TexStart = 0;
	float YPos;
	int Dicedata = 0;
public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {};
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class ShieldArmor;
class ShadowObject;
//class CPlayer;
//============ 캐릭터 ==========//
class CCubeManObject : public CGameObject
{
public:
	CCubeManObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~CCubeManObject();
	BarObject* Hpbar = NULL;
	BarFrameObject* HPFrame = NULL;
	ShadowObject* s = NULL;
	Rank1Object* Rank1 = NULL;

	int select = 0;
	float reviveTime = 5.0f;

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static std::vector<ModelAnimation> animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap ;//텍스처 용 힙
	CPlayer* player=NULL;

	virtual void ToDead();
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
	virtual void EndAnimation(int nAni);

};

//---------------------- 투 사 체 -----------------------------//
class ParticleObject;
class ParticleObject2;
class ParticleObject3;
class BulletCube : public CGameObject
{
public:
	BulletCube(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master,XMFLOAT4& ori,CGameObject* lockon=NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0), bool IsMine = true);
	~BulletCube();
	CGameObject* Master = NULL;//소유자
	CGameObject* LockOn = NULL;//유도시사용됨
	float LifeTime = 10;//생존시간. 10초 후 제거됨
	ParticleObject* BulletParticles = NULL;
	int TexStart = 0;
	int dTime = 0;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
	//서버용
	short		 GetBulletID() const { return myID; }
};


class HeavyBulletCube : public CGameObject
{
public:
	HeavyBulletCube(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0), bool IsMine = true);
	~HeavyBulletCube();
	CGameObject* Master = NULL;//소유자
	CGameObject* LockOn = NULL;//유도시사용됨
	float LifeTime = 10;//생존시간. 10초 후 제거됨
	ParticleObject* BulletParticles = NULL;

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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);

};
class StoneBullet : public CGameObject
{
public:
	StoneBullet(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0), XMFLOAT4 pp = XMFLOAT4(0, 0, 0, 0));
	~StoneBullet();
	CGameObject* Master = NULL;//소유자
	CGameObject* LockOn = NULL;//유도시사용됨
	float LifeTime = 10;//생존시간. 10초 후 제거됨
	ParticleObject3* BulletParticles = NULL;
	XMFLOAT4 orgpluspos;//몬스터 중심에서 어느방향으로 떨어져있는지나타냄.
	float tempangle = 0;//몬스터 주위로 회전할때 각도를 저장.
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);

public:
	//서버 추가

	int myNPC_StoneBulletID;		//stonebullet 아이디
};


class HammerBullet : public CGameObject
{
public:
	HammerBullet(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow,list<CGameObject*>* bulletlist,int count, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0), XMFLOAT4 opp = XMFLOAT4(0, 0, 0, 0));
	~HammerBullet();
	CGameObject* Master = NULL;//소유자
	CGameObject* LockOn = NULL;//유도시사용됨
	float LifeTime = 15;//생존시간. 15초 후 제거됨
	ParticleObject3* BulletParticles = NULL;
	XMFLOAT4 orgpluspos;//몬스터 중심에서 어느방향으로 떨어져있는지나타냄.
	XMFLOAT4 offsetPos;//해머 최초생성시 플레이어의 중점
	float tempangle = 0;//몬스터 주위로 회전할때 각도를 저장.
	int Count;//추가로 생성될수 있는 횟수
public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙
	static list<CGameObject*>* BulletLists;
	
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
};

//사각형모양
class Tetris1 : public CGameObject
{
public:
	Tetris1(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
};

// l 자 모양
class Tetris2 : public CGameObject
{
public:
	Tetris2(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
};

// l 자 모양
class Tetris3 : public CGameObject
{
public:
	Tetris3(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
};

// ㅗ 자 모양
class Tetris4 : public CGameObject
{
public:
	Tetris4(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
};



class Tetrike : public CGameObject
{
public:
	Tetrike(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, list<CGameObject*>*Bulletlist, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class DiceStrike : public CGameObject
{
public:
	DiceStrike(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4& ori, float degree, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~DiceStrike();
	CGameObject* Master = NULL;//소유자

	float LifeTime = 10;

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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);

};

class ShieldArmor : public CGameObject
{
public:
	ShieldArmor(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//소유자

	float LifeTime = 5;

public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};


//================= 스태틱 오브젝트 ===============//

class SphereObject : public CGameObject
{
public:
	SphereObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙

	
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime){}
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class CubeObject : public CGameObject
{
public:
	CubeObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~CubeObject();
	int selectColor;
	ShadowObject* s = NULL;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class MoveCubeObject : public CGameObject
{
public:
	MoveCubeObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float len, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~MoveCubeObject();
	int selectColor;
	float Len;
	float n = 0;
	ShadowObject* s = NULL;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class RigidCubeObject : public CGameObject
{
public:
	RigidCubeObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~RigidCubeObject();
	ShadowObject* s = NULL;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class GridObject : public CGameObject
{
public:
	GridObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, float size, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class Floor2Object : public CGameObject
{
public:
	Floor2Object(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float size, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class SmallWallObject : public CGameObject
{
public:
	SmallWallObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~SmallWallObject();
	ShadowObject* s = NULL;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class BigWallObject : public CGameObject
{
public:
	BigWallObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~BigWallObject();
	ShadowObject* s = NULL;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class BuildingObject : public CGameObject
{
public:
	BuildingObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~BuildingObject();
	ShadowObject* s = NULL;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class ColumnObject : public CGameObject
{
public:
	ColumnObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~ColumnObject();
	ShadowObject* s = NULL;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};
class BreakCartObject : public CGameObject
{
public:
	BreakCartObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~BreakCartObject();
	ShadowObject* s = NULL;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class Rock1Object : public CGameObject
{
public:
	Rock1Object(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, float degree, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	ShadowObject* s = NULL;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};


class RangeObject : public CGameObject
{
public:
	RangeObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class RingObject : public CGameObject
{
public:
	RingObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	int selectColor;
	XMFLOAT4 DummyPos;
	float times = 0;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {};

};

// ============================= Particle ==================================//

class ParticleObject : public CGameObject
{
public:
	ParticleObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, float lifeTime, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class ParticleObject2 : public CGameObject
{
public:
	ParticleObject2(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, float lifeTime,float num, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;
	float LifeTime = 0.2f;
	float ParticleTime = 0.0f;
	float Num;
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};
class ParticleObject3 : public CGameObject
{
public:
	ParticleObject3(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, float lifeTime, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class ShadowObject : public CGameObject
{
public:
	ShadowObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT3 size, int kinds, float degree,  XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;
	XMFLOAT3 Size = XMFLOAT3{ 10,10,10 };
	int Kinds;
	float Degree;

	enum shadowKinds
	{
		Cubeman, //0
		Cube, //1
		Boss, //2
		Meteor, //3
		SmallWall, //4
		tower, //5
		column, //6
		elevator //7
	};
public:
	static bool CreateMesh ;//최초로 false며 메쉬를 만든후 true가된다.
	static bool CreatecMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static bool CreateiMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static bool CreateoMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static bool CreatemMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static bool CreateSmallwallMesh;
	static bool CreateTowerMesh;
	static bool CreateColumnMesh;
	static bool CreateElevatorMesh;
	
	static CMesh cMesh;//cubeman
	static CMesh iMesh;//cube
	static CMesh oMesh;//boss
	static CMesh mMesh;//meteor
	static CMesh smallwallMesh;
	static CMesh towerMesh;
	static CMesh columnMesh;
	static CMesh elevatorMesh;

	static CMaterial Mat;
	static std::vector<ModelAnimation> cAnimations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
	static std::vector<ModelAnimation> iAnimations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);	
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};


class DamageObject : public CGameObject
{
public:
	DamageObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, float damaged, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	float LifeTime = 1.5f;
	int damaged = 0;

public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime){}
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class ImpObject : public CGameObject
{
public:
	ImpObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~ImpObject();
	BarObject* Hpbar = NULL;
	BarFrameObject* HPFrame = NULL;
	FSM* fsm = NULL;
	ShadowObject* s = NULL;
public:
	static CMaterial Mat;
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static std::vector<ModelAnimation> animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙

	virtual void ToDead();
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //머테리얼 생성
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
	virtual void EndAnimation(int nAni);

public:
	//서버 추가
	int					 myNPCID;
};


class MeteorObject : public CGameObject
{
public:
	MeteorObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4& ori, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~MeteorObject();
	ShadowObject* s = NULL;
	CGameObject* Master = NULL;//소유자
	list<CGameObject*>* Blist = NULL;
	float LifeTime = 7.0f;

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
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

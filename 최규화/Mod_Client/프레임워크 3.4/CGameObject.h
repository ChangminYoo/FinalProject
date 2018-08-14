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
	int Point = 0; //���� ����
	int Rank = 5;  //���� ��ŷ
	int KillCount{ 0 };
	int DeathCount{ 0 };
	bool TopMode = false;//1���϶� ž���. ���� ü�� + 800 / ũ�� 1.3�� / �̼� 1.5��
	bool Init = false;//1���ϸ� 1������ �ʱ�ȭ �ؾ���.
}PointRank;

struct ObjectData
{
	XMFLOAT4X4 WorldMatrix = MathHelper::Identity4x4();
	float Scale;
	float SpecularParamater=1;
	int isAnimation = 0;
	float BlendValue = 1.0f;//���� ������
	//CustomData1�� w�� 1234 �̸� ��ָ����� ���°�.
	XMFLOAT4 CustomData1 = XMFLOAT4{ 0,0,0,0 };//������Ʈ�� Ưȭ ������.
	//XMFLOAT4 CustomData2;
	float PTime = 0.0f;
	XMFLOAT3 Velocity = XMFLOAT3{ 0,0,0 };
	XMFLOAT4 TexClamp = XMFLOAT4{ 0, 1 ,0 ,0 };
};


struct GameData //����ĳ������ ���� ���� �˰��ִ� ü��, ������, ���ǵ尡 �ִ�.
{
	float MAXHP=0;
	float HP=0;
	float Damage=0;
	float Speed=0;
	bool GodMode = false;
	
};

class CPlayer;

class CGameObject//�� Ŭ������ �⺻���� ��ӹ޾� �ٸ� ������Ʈ�� ����. ex) �˻������Ʈ, ���������Ʈ ��
{
public:
	//������Ʈ�� �ݵ�� ����̽�,Ŀ�ǵ帮��Ʈ,��ƼŬ����Ʈ,�������� �޴´�.
	//����ƼŬ ����Ʈ�� �ʿ��ϳĸ� �浹�̳� ����ȿ������ ��ƼŬ�� �ֱ� ���ؼ���.
	CGameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject();
	virtual ~CGameObject();
	
	ID3D12Device* device;//����̽�
	ID3D12GraphicsCommandList* commandlist;//�׷���Ŀ�ǵ帮��Ʈ
	ObjectData ObjData;//������İ� ��Ÿ ������
	RigidBody* rb = NULL;//������ �ٵ� ������Ʈ. ��ü������Ʈ�� ���� �̿ϼ�. �׽�Ʈ�� �������̴�.
	PhysicsPoint* pp = NULL;//���� ������Ʈ. ������Ʈ�� ����������Ʈ�� ��ü������Ʈ�� ������ �ϳ��� ����Ѵ�.
	bool staticobject = false;//������ �ǹ����� ��ü�ΰ�?
	bool MinDepth = false;//UI�� �ƴ� ������Ʈ ���� ���̸� ���� ����(�׻� �ֿ켱������) �׷����ϴ°�?
	
	

	list<CGameObject*>* CollisionList=NULL;
	list<CGameObject*>* ParticleList = NULL;
	list<CGameObject*>* Shadow = NULL;

protected:
	
	UploadBuffer<ObjectData>* ConstBuffer=NULL;	//������İ� Ŀ���ҵ����͸� �����ϱ����� ����

public:
	//�������� 1���̶�� �Ѱ��ָ� �ɷ�ġ ��ȭ.
	PointRank pointrank;

	//��ġ�� ȸ������
	XMFLOAT4 CenterPos;//����
	XMFLOAT4 Orient;//������ ��Ÿ����.
	XMFLOAT4 OrgPos;//��â�� ����
	bool AirBone = false;

	char* TextureName = NULL;
	char* NTextureName = NULL;
	UINT TexOff = 0;
	UINT NTexOff = 0;

	//�躤�Ϳ� ����Ʈ����
	XMFLOAT3 Lookvector;//�躤��. ������Ʈ�� �ٶ󺸰��ִ� ����.
	XMFLOAT3 Rightvector;//����Ʈ����. ������Ʈ�� �ٶ󺸰��ִ� ������ �����ʹ���.
	XMFLOAT3 OffLookvector;//�ʱ�躤��. ������Ʈ�� �ٶ󺸰��ִ� ����.
	XMFLOAT3 OffRightvector;//�ʱ����Ʈ����. ������Ʈ�� �ٶ󺸰��ִ� ������ �����ʹ���.
	XMFLOAT3 GetUpvector();//�躤�Ϳ� ����Ʈ���͸� �̿��� �����͸� ����Լ�

	//�ִϸ��̼� ���õ�����
	int n_Animation = 0;//���° �ִϸ��̼��� ���ǰ�?
	float currAnimTime = 0;//����ִϸ��̼ǽð�
	int TickValue =0;//�ִϸ��̼� ȣ�� ������
	UploadBuffer<JointArr>* jarr = NULL;//���� ����Ʈ���� ����Ǵ� �������. ���̰� ����ƽ�� �ƴϳĸ�
	//1�� ������Ʈ�� �ִϸ��̼��� ������Ʈ�� �ѵ� ������ �Ѵ���, 2�� ������Ʈ�� �ִϸ��̼��� �ٽ� ������Ʈ�ϸ�
	//2�� ������Ʈ�� ������ �ִϸ��̼��� �������� jarr���ǹǷ� �װ� �׸��� �ȴ�. ���� ���� �����ؾ���.


	//��Ÿ ���� �����͵�
	bool DelObj = false;//�̰� ���̸� ������ ���ŵȴ�.
	bool DrawObj = true;
	bool Blending = false;
	bool PrevCool = false;
	bool isShieldOn = false;
	bool isHit1 = false;

	float TexStride = 0.f;

	//���鿡 ���� ���콺�� �����϶����� üũ�� �ʿ�� �����Ƿ� �߰���. �� ���� �� ����� ���� ó���Ұ� �����Ͱ���.
	Obj_State obs = Dynamic;

	GameData gamedata;

	RayCastObject rco;//����ĳ��Ʈ ������Ʈ
	CGameObject* LockOn=NULL;

	//CMesh sMesh;
	//std::vector<ModelAnimation> sAnimations;
	//��Ÿ ���� �Լ���
	virtual void SetWorldMatrix();//�����Ʈ���� ����.
	virtual void SetShadowMatrix();

	virtual void CreateConstBuffer(ID3D12Device* m_Device);
	virtual void UpdateConstBuffer(ID3D12GraphicsCommandList* commandlist, bool isShadow);
	
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)=0;//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime)=0;
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) = 0;
	
	virtual void ToDamage(float Damage);
	virtual void ToDead();
	
	
	void UpdateLookVector();//�׻�ȣ�����ٰ�.
	
	void SetAnimation(int n_Ani);//���° �ִϸ��̼��� ���ǰ�?
	virtual void EndAnimation(int nAni) {}//�ִϸ��̼� ����Ǿ����� �˸���.
	//���ӿ�����Ʈ�� ��ӹް� �̰͵��� ��� static���� �����Ұ�.
	//bool CrateMesh;
	//unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	//CMesh * Mesh = NULL;//���߿� �޽��� static���� �־���
	//std::vector<ModelAnimation> animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
	//ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap = nullptr;//�ؽ�ó �� ��

public:
	//���� �߰�
	static int g_numofdice;
	
	static int g_npcID;
	static int g_npcBulletID;
	bool	   isNPC{ false };

	//bullet ID
	static unsigned short myID;
	static bool			  m_IsBulletFirstCount;
	static list<unsigned short>    BulletIDList;
	int							   m_bullet_type;

	//HammerBullet ID - ���� ���� - ���� �ҷ����� �ٸ��� �������� �ش� �ҷ� ���̵� �����ų �� �����Ƿ�
	static unsigned short m_hmBulletID;
	static bool			  m_hmBulletFirstCount;

	//����->Ŭ�� ������ ĳ���� ������(GameObject�� �־�־���)
	Player_Data			 m_player_data;

	//����->Ŭ�� ������ NPC ������
	Npc_Data			 m_npc_data;

	//����->Ŭ�� ������ StaticObject ������
	StaticObject_Info	 m_sobj_data;

	//����->Ŭ�� ������ MoveStaticObject ������
	MoveObjectData       m_mvobj_data;

	//����->Ŭ�� ������ RigidbodyObject ������
	RigidbodyData		 m_rigidbody_data;

	//����->Ŭ�� ������ BulletObject ������
	STC_BulletObject_Info	 m_bullet_data;

	CPlayer* m_player{ nullptr };

	bool m_end_attack{ false }, m_end_die{ false };
	bool m_end_npc_attack{ false }, m_end_npc_die{ false };
	
	//�ҷ� ��ƼŬ Ÿ��
	int m_particle_type;

	float	m_degree;

	//�ĵ��� ��ų ������ ���̵� - �������� ��
	int m_waveMasterID{ -1 };

};

void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, ID3D12Resource* texture, int texMap = 0, int Offset = 0);
void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist, CGameObject* obj, unordered_map<string, unique_ptr<CTexture>>& Textures, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, string texturename, wstring FileName, bool isCubeMap, int NumDescriptors = 1, int Offset = 0);

//��ӹ��� ���ӿ�����Ʈ�� ������ �������� ó���Ұ�.
//1. ������
//2. ƽ�Լ�(���ӿ�����Ʈ���� �ٸ� �ൿ�� ���״ϱ�)
//3. �޽��� �ؽ�ó, �ִϸ��̼� ���� ���� static���� ����
//4. �����Լ��� �״�� ������. �� �̷��ĸ� static Mesh�� ����ϱ⶧���̴�..
//5. �ݸ����Լ� �߰��ؾ���. ����������Ʈ�� ��ü������Ʈ�� ���� �ٸ��� �����ؾ���.(�켱 ����������Ʈ�ۿ� ����.)

class BarObject : public CGameObject
{
public:
	BarObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, float size, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//������
	float YPos;
public:
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class BarFrameObject : public CGameObject
{
public:
	BarFrameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, float size, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//������
	float YPos;
public:
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};

class Rank1Object : public CGameObject
{
public:
	Rank1Object(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//������
public:
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class DiceObject : public CGameObject
{
public:
	DiceObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT3 goal, list<CGameObject*>* bulletlist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//������
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {};
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

class ShieldArmor;
class ShadowObject;
//class CPlayer;
//============ ĳ���� ==========//
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static std::vector<ModelAnimation> animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap ;//�ؽ�ó �� ��
	CPlayer* player=NULL;

	virtual void ToDead();
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
	virtual void EndAnimation(int nAni);

};

//---------------------- �� �� ü -----------------------------//
class ParticleObject;
class ParticleObject2;
class ParticleObject3;
class BulletCube : public CGameObject
{
public:
	BulletCube(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master,XMFLOAT4& ori,CGameObject* lockon=NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0), bool IsMine = true);
	~BulletCube();
	CGameObject* Master = NULL;//������
	CGameObject* LockOn = NULL;//�����û���
	float LifeTime = 10;//�����ð�. 10�� �� ���ŵ�
	ParticleObject* BulletParticles = NULL;
	int TexStart = 0;
	int dTime = 0;
public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
	//������
	short		 GetBulletID() const { return myID; }
};


class HeavyBulletCube : public CGameObject
{
public:
	HeavyBulletCube(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0), bool IsMine = true);
	~HeavyBulletCube();
	CGameObject* Master = NULL;//������
	CGameObject* LockOn = NULL;//�����û���
	float LifeTime = 10;//�����ð�. 10�� �� ���ŵ�
	ParticleObject* BulletParticles = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	CGameObject* Master = NULL;//������
	CGameObject* LockOn = NULL;//�����û���
	float LifeTime = 10;//�����ð�. 10�� �� ���ŵ�
	ParticleObject3* BulletParticles = NULL;
	XMFLOAT4 orgpluspos;//���� �߽ɿ��� ����������� �������ִ�����Ÿ��.
	float tempangle = 0;//���� ������ ȸ���Ҷ� ������ ����.
public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);

public:
	//���� �߰�

	int myNPC_StoneBulletID;		//stonebullet ���̵�
};


class HammerBullet : public CGameObject
{
public:
	HammerBullet(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow,list<CGameObject*>* bulletlist,int count, CGameObject* master, XMFLOAT4& ori, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0), XMFLOAT4 opp = XMFLOAT4(0, 0, 0, 0));
	~HammerBullet();
	CGameObject* Master = NULL;//������
	CGameObject* LockOn = NULL;//�����û���
	float LifeTime = 15;//�����ð�. 15�� �� ���ŵ�
	ParticleObject3* BulletParticles = NULL;
	XMFLOAT4 orgpluspos;//���� �߽ɿ��� ����������� �������ִ�����Ÿ��.
	XMFLOAT4 offsetPos;//�ظ� ���ʻ����� �÷��̾��� ����
	float tempangle = 0;//���� ������ ȸ���Ҷ� ������ ����.
	int Count;//�߰��� �����ɼ� �ִ� Ƚ��
public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��
	static list<CGameObject*>* BulletLists;
	
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
};

//�簢�����
class Tetris1 : public CGameObject
{
public:
	Tetris1(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~Tetris1();
	CGameObject* Master = NULL;//������

	float LifeTime = 10;//�����ð�. 10�� �� ���ŵ�
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
};

// l �� ���
class Tetris2 : public CGameObject
{
public:
	Tetris2(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~Tetris2();
	CGameObject* Master = NULL;//������

	float LifeTime = 10;//�����ð�. 10�� �� ���ŵ�
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
};

// l �� ���
class Tetris3 : public CGameObject
{
public:
	Tetris3(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~Tetris3();
	CGameObject* Master = NULL;//������

	float LifeTime = 10;//�����ð�. 10�� �� ���ŵ�
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);
};

// �� �� ���
class Tetris4 : public CGameObject
{
public:
	Tetris4(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, CGameObject* lockon = NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~Tetris4();
	CGameObject* Master = NULL;//������

	float LifeTime = 10;//�����ð�. 10�� �� ���ŵ�
	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;

public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	CGameObject* Master = NULL;//������
	CGameObject* LockOn = NULL;//�����û���
	float LifeTime = 10;//�����ð�. 10�� �� ���ŵ�
	float ShotTime = 0;//0.25�ʸ��� �ߵ�
	list<CGameObject*>* Blist = NULL;
public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	CGameObject* Master = NULL;//������

	float LifeTime = 10;

	ParticleObject* BulletParticles = NULL;
	ParticleObject* BulletParticles2 = NULL;
public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos);

};

class ShieldArmor : public CGameObject
{
public:
	ShieldArmor(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//������

	float LifeTime = 5;

public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
};


//================= ����ƽ ������Ʈ ===============//

class SphereObject : public CGameObject
{
public:
	SphereObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist , list<CGameObject*>*Plist, list<CGameObject*>*shadow, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��

	
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void Tick(const GameTimer& gt);
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void Tick(const GameTimer& gt);
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
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
	static bool CreateMesh ;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static bool CreatecMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static bool CreateiMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static bool CreateoMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static bool CreatemMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
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
	static std::vector<ModelAnimation> cAnimations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
	static std::vector<ModelAnimation> iAnimations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


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
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static std::vector<ModelAnimation> animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��

	virtual void ToDead();
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}
	virtual void EndAnimation(int nAni);

public:
	//���� �߰�
	int					 myNPCID;
};


class MeteorObject : public CGameObject
{
public:
	MeteorObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, list<CGameObject*>*shadow, CGameObject* master, XMFLOAT4& ori, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	~MeteorObject();
	ShadowObject* s = NULL;
	CGameObject* Master = NULL;//������
	list<CGameObject*>* Blist = NULL;
	float LifeTime = 7.0f;

public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void Collision(int coll_type, int damage, const XMFLOAT4& bullet_pos, const XMFLOAT4& target_pos) {}

};

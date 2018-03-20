#pragma once
#include"CMesh.h"
#include"UploadBuffer.h"
#include"Timer.h"
#include"CTexture.h"
#include"CMaterial.h"
#include"MMPE\MyMiniPysicsEngine.h"

enum Ani_State 
{
	Idle = 0,
	Run,
	Attack,
	Victory,
	Crying
};

struct ObjectData
{
	XMFLOAT4X4 WorldMatrix;
	float Scale;
	float SpecularParamater=1;
	int isAnimation = 0;
	float BlendValue = 1.0f;//���� ������
	XMFLOAT4 CustomData1 = XMFLOAT4{ 1,2,3,4 };//������Ʈ�� Ưȭ ������.
	//XMFLOAT4 CustomData2;
};


struct GameData //����ĳ������ ���� ���� �˰��ִ� ü��, ������, ���ǵ尡 �ִ�.
{
	float MAXHP=0;
	float HP=0;
	float Damage=0;
	float Speed=0;
	bool GodMode = false;
};

class CGameObject//�� Ŭ������ �⺻���� ��ӹ޾� �ٸ� ������Ʈ�� ����. ex) �˻������Ʈ, ���������Ʈ ��
{
public:
	CGameObject();
	~CGameObject();
	ID3D12GraphicsCommandList* commandlist;//�׷���Ŀ�ǵ帮��Ʈ
	ObjectData ObjData;//������İ� ��Ÿ ������
	PhysicsPoint* pp = NULL;//���� ������Ʈ. ������Ʈ�� ����������Ʈ�� ��ü������Ʈ�� ������ �ϳ��� ����Ѵ�.
	RigidBody* rb = NULL;//������ �ٵ� ������Ʈ. ��ü������Ʈ�� ���� �̿ϼ�. �׽�Ʈ�� �������̴�.
	bool staticobject = false;//������ �ǹ����� ��ü�ΰ�?
	
	list<CGameObject*>* CollisionList=NULL;

protected:
	
	UploadBuffer<ObjectData>* ConstBuffer=NULL;	//������İ� Ŀ���ҵ����͸� �����ϱ����� ����

public:

	//��ġ�� ȸ������
	XMFLOAT4 CenterPos;//����
	XMFLOAT4 Orient;//������ ��Ÿ����.
	bool AirBone = false;
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
	GameData gamedata;
	RayCastObject rco;//����ĳ��Ʈ ������Ʈ
	bool Blending = false;
	CGameObject* LockOn=NULL;

	CGameObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist,XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	virtual void SetWorldMatrix();//�����Ʈ���� ����.
	
	virtual void CreateConstBuffer(ID3D12Device* m_Device);
	virtual void UpdateConstBuffer(ID3D12GraphicsCommandList* commandlist);
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist)=0;//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist) {}
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime)=0;
	virtual void ToDamage(float Damage);
	virtual void ToDead();
	void UpdatePPosCenterPos();//������ �������� ������� ������. �ݵ�� �浹�ϰų� �ϸ� ó���ؾ���.
	void UpdateLookVector();//�׻�ȣ�����ٰ�.
	void SetAnimation(int n_Ani);//���° �ִϸ��̼��� ���ǰ�?
	virtual void EndAnimation(int nAni) {}//�ִϸ��̼� ����Ǿ����� �˸���.
	//���ӿ�����Ʈ�� ��ӹް� �̰͵��� ��� static���� �����Ұ�.
	//bool CrateMesh;
	//unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	//CMesh * Mesh = NULL;//���߿� �޽��� static���� �־���
	//std::vector<ModelAnimation> animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
	//ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap = nullptr;//�ؽ�ó �� ��

	
	
};
void SetTexture(ID3D12GraphicsCommandList * commandlist, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, ID3D12Resource* texture, bool isCubeMap);

void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandlist, CGameObject* obj, unordered_map<string, unique_ptr<CTexture>>& Textures, ComPtr<ID3D12DescriptorHeap>& SrvDescriptorHeap, string texturename, wstring FileName, bool isCubeMap);

//��ӹ��� ���ӿ�����Ʈ�� ������ �������� ó���Ұ�.
//1. ������
//2. ƽ�Լ�(���ӿ�����Ʈ���� �ٸ� �ൿ�� ���״ϱ�)
//3. �޽��� �ؽ�ó, �ִϸ��̼� ���� ���� static���� ����
//4. �����Լ��� �״�� ������. �� �̷��ĸ� static Mesh�� ����ϱ⶧���̴�..
//5. �ݸ����Լ� �߰��ؾ���. ����������Ʈ�� ��ü������Ʈ�� ���� �ٸ��� �����ؾ���.(�켱 ����������Ʈ�ۿ� ����.)

class CCubeManObject : public CGameObject
{
public:
	CCubeManObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	
public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static std::vector<ModelAnimation> animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap ;//�ؽ�ó �� ��
		

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void EndAnimation(int nAni);
};

class CZombieObject : public CGameObject
{
public:
	CZombieObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static CMaterial Mat;
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static std::vector<ModelAnimation> animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void SetMaterial(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist); //���׸��� ����
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);
	virtual void EndAnimation(int nAni);
};


//---------------------- �� �� ü -----------------------------//

class BulletCube : public CGameObject
{
public:
	BulletCube(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist,CGameObject* master,XMFLOAT4& ori,CGameObject* lockon=NULL, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//������
	CGameObject* LockOn = NULL;//�����û���
	float LifeTime = 10;//�����ð�. 10�� �� ���ŵ�
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

};



//---------------------- ����ƽ ������Ʈ -----------------------------//

class SphereObject : public CGameObject
{
public:
	SphereObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

public:
	static bool CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//�ؽ�ó���� ������
	static CMesh Mesh;//������ �ѹ��� �������
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//�ؽ�ó �� ��

	
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//�¸޽ô� �޽ø� ���������� �����Ѵ�. �� �޽ø������ϴ� ������ �ﰢ���������ϴ��ε������ۻ���
	virtual void Tick(const GameTimer& gt);
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};

class CubeObject : public CGameObject
{
public:
	CubeObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));

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
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime);

};


// ���������
void CreateCube(CMesh* Mesh, float size);
void CreateGrid(CMesh* Mesh, float width, float depth);
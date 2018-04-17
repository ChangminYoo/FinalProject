#include"CGameObject.h"


bool CCubeManObject::CreateMesh=false;
unordered_map<string, unique_ptr<CTexture>> CCubeManObject::Textures;//�ؽ�ó���� ������
CMesh  CCubeManObject::Mesh;//���߿� �޽��� static���� �־���
std::vector<ModelAnimation> CCubeManObject::animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
ComPtr<ID3D12DescriptorHeap> CCubeManObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial CCubeManObject::Mat;

//--------------------------------------------------------------------------------------

bool CZombieObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CZombieObject::Textures;//�ؽ�ó���� ������
CMesh  CZombieObject::Mesh;//���߿� �޽��� static���� �־���
std::vector<ModelAnimation> CZombieObject::animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
ComPtr<ID3D12DescriptorHeap> CZombieObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial CZombieObject::Mat;

//--------------------------------------------------------------------------------------
bool BulletCube::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BulletCube::Textures;//�ؽ�ó���� ������
CMesh  BulletCube::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> BulletCube::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial BulletCube::Mat;

//--------------------------------------------------------------------------------------
bool HeavyBulletCube::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> HeavyBulletCube::Textures;//�ؽ�ó���� ������
CMesh  HeavyBulletCube::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> HeavyBulletCube::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial HeavyBulletCube::Mat;

//--------------------------------------------------------------------------------------
bool SphereObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> SphereObject::Textures;//�ؽ�ó���� ������
CMesh  SphereObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> SphereObject::SrvDescriptorHeap;//�ؽ�ó �� ��

//--------------------------------------------------------------------------------------
bool CubeObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CubeObject::Textures;//�ؽ�ó���� ������
CMesh  CubeObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> CubeObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial CubeObject::Mat;

//--------------------------------------------------------------------------------------
bool GridObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> GridObject::Textures;//�ؽ�ó���� ������
CMesh  GridObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> GridObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial GridObject::Mat;

//--------------------------------------------------------------------------------------
bool ParticleObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> ParticleObject::Textures;//�ؽ�ó���� ������
CMesh  ParticleObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> ParticleObject::SrvDescriptorHeap;//�ؽ�ó �� ��

//--------------------------------------------------------------------------------------
bool DamageObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> DamageObject::Textures;//�ؽ�ó���� ������
CMesh  DamageObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> DamageObject::SrvDescriptorHeap;//�ؽ�ó �� ��

//--------------------------------------------------------------------------------------
bool RigidCubeObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> RigidCubeObject::Textures;//�ؽ�ó���� ������
CMesh  RigidCubeObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> RigidCubeObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial RigidCubeObject::Mat;

//--------------------------------------------------------------------------------------
bool AimObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> AimObject::Textures;//�ؽ�ó���� ������
CMesh  AimObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> AimObject::SrvDescriptorHeap;//�ؽ�ó �� ��

 //--------------------------------------------------------------------------------------

bool BackGroundSkillObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BackGroundSkillObject::Textures;//�ؽ�ó���� ������
CMesh  BackGroundSkillObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> BackGroundSkillObject::SrvDescriptorHeap;//�ؽ�ó �� ��

bool SkillUIObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> SkillUIObject::Textures;//�ؽ�ó���� ������
CMesh  SkillUIObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> SkillUIObject::SrvDescriptorHeap;//�ؽ�ó �� ��


//--------------------------------------------------------------------------------------
bool BarObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BarObject::Textures;//�ؽ�ó���� ������
CMesh  BarObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> BarObject::SrvDescriptorHeap;//�ؽ�ó �� ��

bool BarFrameObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BarFrameObject::Textures;//�ؽ�ó���� ������
CMesh  BarFrameObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> BarFrameObject::SrvDescriptorHeap;//�ؽ�ó �� ��
														  
//--------------------------------------------------------------------------------------
bool CoolBarObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CoolBarObject::Textures;//�ؽ�ó���� ������
CMesh  CoolBarObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> CoolBarObject::SrvDescriptorHeap;//�ؽ�ó �� ��


 // --------------------------------------------------------------------------------------

CMaterial SmallWallObject::Mat;
bool SmallWallObject::CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
unordered_map<string, unique_ptr<CTexture>> SmallWallObject::Textures;//�ؽ�ó���� ������
CMesh SmallWallObject::Mesh;//������ �ѹ��� �������
ComPtr<ID3D12DescriptorHeap> SmallWallObject::SrvDescriptorHeap;//�ؽ�ó �� ��

// --------------------------------------------------------------------------------------

CMaterial BigWallObject::Mat;
bool BigWallObject::CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
unordered_map<string, unique_ptr<CTexture>> BigWallObject::Textures;//�ؽ�ó���� ������
CMesh BigWallObject::Mesh;//������ �ѹ��� �������
ComPtr<ID3D12DescriptorHeap> BigWallObject::SrvDescriptorHeap;//�ؽ�ó �� ��

// --------------------------------------------------------------------------------------

CMaterial BuildingObject::Mat;
bool BuildingObject::CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
unordered_map<string, unique_ptr<CTexture>> BuildingObject::Textures;//�ؽ�ó���� ������
CMesh BuildingObject::Mesh;//������ �ѹ��� �������
ComPtr<ID3D12DescriptorHeap> BuildingObject::SrvDescriptorHeap;//�ؽ�ó �� ��

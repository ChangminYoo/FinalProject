#include"CGameObject.h"


bool CCubeManObject::CreateMesh=false;
unordered_map<string, unique_ptr<CTexture>> CCubeManObject::Textures;//텍스처들을 저장함
CMesh  CCubeManObject::Mesh;//나중에 메쉬는 static으로 둬야함
std::vector<ModelAnimation> CCubeManObject::animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
ComPtr<ID3D12DescriptorHeap> CCubeManObject::SrvDescriptorHeap;//텍스처 용 힙
CMaterial CCubeManObject::Mat;

//--------------------------------------------------------------------------------------

bool CZombieObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CZombieObject::Textures;//텍스처들을 저장함
CMesh  CZombieObject::Mesh;//나중에 메쉬는 static으로 둬야함
std::vector<ModelAnimation> CZombieObject::animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
ComPtr<ID3D12DescriptorHeap> CZombieObject::SrvDescriptorHeap;//텍스처 용 힙
CMaterial CZombieObject::Mat;

//--------------------------------------------------------------------------------------
bool BulletCube::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BulletCube::Textures;//텍스처들을 저장함
CMesh  BulletCube::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> BulletCube::SrvDescriptorHeap;//텍스처 용 힙
CMaterial BulletCube::Mat;

//--------------------------------------------------------------------------------------
bool HeavyBulletCube::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> HeavyBulletCube::Textures;//텍스처들을 저장함
CMesh  HeavyBulletCube::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> HeavyBulletCube::SrvDescriptorHeap;//텍스처 용 힙
CMaterial HeavyBulletCube::Mat;

//--------------------------------------------------------------------------------------
bool SphereObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> SphereObject::Textures;//텍스처들을 저장함
CMesh  SphereObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> SphereObject::SrvDescriptorHeap;//텍스처 용 힙

//--------------------------------------------------------------------------------------
bool CubeObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CubeObject::Textures;//텍스처들을 저장함
CMesh  CubeObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> CubeObject::SrvDescriptorHeap;//텍스처 용 힙
CMaterial CubeObject::Mat;

//--------------------------------------------------------------------------------------
bool GridObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> GridObject::Textures;//텍스처들을 저장함
CMesh  GridObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> GridObject::SrvDescriptorHeap;//텍스처 용 힙
CMaterial GridObject::Mat;

//--------------------------------------------------------------------------------------
bool ParticleObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> ParticleObject::Textures;//텍스처들을 저장함
CMesh  ParticleObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> ParticleObject::SrvDescriptorHeap;//텍스처 용 힙

//--------------------------------------------------------------------------------------
bool DamageObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> DamageObject::Textures;//텍스처들을 저장함
CMesh  DamageObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> DamageObject::SrvDescriptorHeap;//텍스처 용 힙

//--------------------------------------------------------------------------------------
bool RigidCubeObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> RigidCubeObject::Textures;//텍스처들을 저장함
CMesh  RigidCubeObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> RigidCubeObject::SrvDescriptorHeap;//텍스처 용 힙
CMaterial RigidCubeObject::Mat;

//--------------------------------------------------------------------------------------
bool AimObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> AimObject::Textures;//텍스처들을 저장함
CMesh  AimObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> AimObject::SrvDescriptorHeap;//텍스처 용 힙

 //--------------------------------------------------------------------------------------

bool BackGroundSkillObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BackGroundSkillObject::Textures;//텍스처들을 저장함
CMesh  BackGroundSkillObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> BackGroundSkillObject::SrvDescriptorHeap;//텍스처 용 힙

bool SkillUIObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> SkillUIObject::Textures;//텍스처들을 저장함
CMesh  SkillUIObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> SkillUIObject::SrvDescriptorHeap;//텍스처 용 힙


//--------------------------------------------------------------------------------------
bool BarObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BarObject::Textures;//텍스처들을 저장함
CMesh  BarObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> BarObject::SrvDescriptorHeap;//텍스처 용 힙

bool BarFrameObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BarFrameObject::Textures;//텍스처들을 저장함
CMesh  BarFrameObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> BarFrameObject::SrvDescriptorHeap;//텍스처 용 힙
														  
//--------------------------------------------------------------------------------------
bool CoolBarObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CoolBarObject::Textures;//텍스처들을 저장함
CMesh  CoolBarObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> CoolBarObject::SrvDescriptorHeap;//텍스처 용 힙


 // --------------------------------------------------------------------------------------

CMaterial SmallWallObject::Mat;
bool SmallWallObject::CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
unordered_map<string, unique_ptr<CTexture>> SmallWallObject::Textures;//텍스처들을 저장함
CMesh SmallWallObject::Mesh;//오로지 한번만 만들어짐
ComPtr<ID3D12DescriptorHeap> SmallWallObject::SrvDescriptorHeap;//텍스처 용 힙

// --------------------------------------------------------------------------------------

CMaterial BigWallObject::Mat;
bool BigWallObject::CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
unordered_map<string, unique_ptr<CTexture>> BigWallObject::Textures;//텍스처들을 저장함
CMesh BigWallObject::Mesh;//오로지 한번만 만들어짐
ComPtr<ID3D12DescriptorHeap> BigWallObject::SrvDescriptorHeap;//텍스처 용 힙

// --------------------------------------------------------------------------------------

CMaterial BuildingObject::Mat;
bool BuildingObject::CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
unordered_map<string, unique_ptr<CTexture>> BuildingObject::Textures;//텍스처들을 저장함
CMesh BuildingObject::Mesh;//오로지 한번만 만들어짐
ComPtr<ID3D12DescriptorHeap> BuildingObject::SrvDescriptorHeap;//텍스처 용 힙

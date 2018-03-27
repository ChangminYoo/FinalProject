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
bool TreeObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> TreeObject::Textures;//텍스처들을 저장함
CMesh  TreeObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> TreeObject::SrvDescriptorHeap;//텍스처 용 힙
CMaterial TreeObject::Mat;

//--------------------------------------------------------------------------------------
bool DamageObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> DamageObject::Textures;//텍스처들을 저장함
CMesh  DamageObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> DamageObject::SrvDescriptorHeap;//텍스처 용 힙
CMaterial DamageObject::Mat;
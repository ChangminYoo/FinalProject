#include"CGameObject.h"

//---------------------------------------------------------
// DinamicObject
//---------------------------------------------------------

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






//---------------------------------------------------------
// StaticObject
//---------------------------------------------------------
float CSphereObject::radius = 0.0f;
int CSphereObject::stack = 0;
int CSphereObject::slice = 0;
bool CSphereObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CSphereObject::Textures;//텍스처들을 저장함
CMesh  CSphereObject::Mesh;//나중에 메쉬는 static으로 둬야함
ComPtr<ID3D12DescriptorHeap> CSphereObject::SrvDescriptorHeap;//텍스처 용 힙
CMaterial CSphereObject::Mat;
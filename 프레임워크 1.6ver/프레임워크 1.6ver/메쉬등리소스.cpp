#include"CGameObject.h"


bool CZombieObject::CreateMesh=false;
unordered_map<string, unique_ptr<CTexture>> CZombieObject::Textures;//텍스처들을 저장함
CMesh  CZombieObject::Mesh;//나중에 메쉬는 static으로 둬야함
std::vector<ModelAnimation> CZombieObject::animations;//애니메이션 데이터 저장. 메쉬와 이거,텍스처는 한번만생성해서 공유하도록해야됨
ComPtr<ID3D12DescriptorHeap> CZombieObject::SrvDescriptorHeap;//텍스처 용 힙

//--------------------------------------------------------------------------------------

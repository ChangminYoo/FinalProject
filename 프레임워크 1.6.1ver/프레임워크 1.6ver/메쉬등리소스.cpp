#include"CGameObject.h"


bool CZombieObject::CreateMesh=false;
unordered_map<string, unique_ptr<CTexture>> CZombieObject::Textures;//�ؽ�ó���� ������
CMesh  CZombieObject::Mesh;//���߿� �޽��� static���� �־���
std::vector<ModelAnimation> CZombieObject::animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
ComPtr<ID3D12DescriptorHeap> CZombieObject::SrvDescriptorHeap;//�ؽ�ó �� ��

//--------------------------------------------------------------------------------------

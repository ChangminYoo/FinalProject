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
bool TreeObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> TreeObject::Textures;//�ؽ�ó���� ������
CMesh  TreeObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> TreeObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial TreeObject::Mat;

//--------------------------------------------------------------------------------------
bool DamageObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> DamageObject::Textures;//�ؽ�ó���� ������
CMesh  DamageObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> DamageObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial DamageObject::Mat;
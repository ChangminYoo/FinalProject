#include"CGameObject.h"
#include"CGameUIObject.h"


bool CCubeManObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CCubeManObject::Textures;//�ؽ�ó���� ������
CMesh  CCubeManObject::Mesh;//���߿� �޽��� static���� �־���
std::vector<ModelAnimation> CCubeManObject::animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
ComPtr<ID3D12DescriptorHeap> CCubeManObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial CCubeManObject::Mat;


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
bool StoneBullet::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> StoneBullet::Textures;//�ؽ�ó���� ������
CMesh  StoneBullet::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> StoneBullet::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial StoneBullet::Mat;
//--------------------------------------------------------------------------------------
bool HammerBullet::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> HammerBullet::Textures;//�ؽ�ó���� ������
CMesh  HammerBullet::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> HammerBullet::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial HammerBullet::Mat;
list<CGameObject*>* HammerBullet::BulletLists;
//--------------------------------------------------------------------------------------
bool Tetris1::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> Tetris1::Textures;//�ؽ�ó���� ������
CMesh  Tetris1::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> Tetris1::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial Tetris1::Mat;
//--------------------------------------------------------------------------------------
bool Tetris2::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> Tetris2::Textures;//�ؽ�ó���� ������
CMesh  Tetris2::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> Tetris2::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial Tetris2::Mat;
//--------------------------------------------------------------------------------------
bool Tetris3::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> Tetris3::Textures;//�ؽ�ó���� ������
CMesh  Tetris3::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> Tetris3::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial Tetris3::Mat;
//--------------------------------------------------------------------------------------
bool Tetris4::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> Tetris4::Textures;//�ؽ�ó���� ������
CMesh  Tetris4::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> Tetris4::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial Tetris4::Mat;
//--------------------------------------------------------------------------------------
bool Tetrike::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> Tetrike::Textures;//�ؽ�ó���� ������
CMesh  Tetrike::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> Tetrike::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial Tetrike::Mat;
//--------------------------------------------------------------------------------------
bool DiceStrike::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> DiceStrike::Textures;//�ؽ�ó���� ������
CMesh  DiceStrike::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> DiceStrike::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial DiceStrike::Mat;
//--------------------------------------------------------------------------------------
bool ShieldArmor::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> ShieldArmor::Textures;//�ؽ�ó���� ������
CMesh ShieldArmor::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> ShieldArmor::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial ShieldArmor::Mat;
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
bool MoveCubeObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> MoveCubeObject::Textures;//�ؽ�ó���� ������
CMesh  MoveCubeObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> MoveCubeObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial MoveCubeObject::Mat;
//--------------------------------------------------------------------------------------
bool GridObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> GridObject::Textures;//�ؽ�ó���� ������
CMesh  GridObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> GridObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial GridObject::Mat;
//--------------------------------------------------------------------------------------
bool Floor2Object::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> Floor2Object::Textures;//�ؽ�ó���� ������
CMesh Floor2Object::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> Floor2Object::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial Floor2Object::Mat;
//--------------------------------------------------------------------------------------
bool ParticleObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> ParticleObject::Textures;//�ؽ�ó���� ������
CMesh  ParticleObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> ParticleObject::SrvDescriptorHeap;//�ؽ�ó �� ��
															   //--------------------------------------------------------------------------------------
bool ParticleObject2::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> ParticleObject2::Textures;//�ؽ�ó���� ������
CMesh  ParticleObject2::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> ParticleObject2::SrvDescriptorHeap;//�ؽ�ó �� ��
																//--------------------------------------------------------------------------------------
bool ParticleObject3::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> ParticleObject3::Textures;//�ؽ�ó���� ������
CMesh  ParticleObject3::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> ParticleObject3::SrvDescriptorHeap;//�ؽ�ó �� ��
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
bool BackGroundObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BackGroundObject::Textures;//�ؽ�ó���� ������
CMesh  BackGroundObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> BackGroundObject::SrvDescriptorHeap;//�ؽ�ó �� ��
																 //--------------------------------------------------------------------------------------
bool SkillFrameUIObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> SkillFrameUIObject::Textures;//�ؽ�ó���� ������
CMesh  SkillFrameUIObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> SkillFrameUIObject::SrvDescriptorHeap;//�ؽ�ó �� ��

bool SkillUIObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> SkillUIObject::Textures;//�ؽ�ó���� ������
CMesh  SkillUIObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> SkillUIObject::SrvDescriptorHeap;//�ؽ�ó �� ��
															  //--------------------------------------------------------------------------------------
bool BarObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BarObject::Textures;//�ؽ�ó���� ������
CMesh  BarObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> BarObject::SrvDescriptorHeap;//�ؽ�ó �� ��
														  //--------------------------------------------------------------------------------------
bool BarFrameObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> BarFrameObject::Textures;//�ؽ�ó���� ������
CMesh  BarFrameObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> BarFrameObject::SrvDescriptorHeap;//�ؽ�ó �� ��
															   //--------------------------------------------------------------------------------------
bool Rank1Object::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> Rank1Object::Textures;//�ؽ�ó���� ������
CMesh  Rank1Object::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> Rank1Object::SrvDescriptorHeap;//�ؽ�ó �� ��
															//--------------------------------------------------------------------------------------
bool SelectBarObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> SelectBarObject::Textures;//�ؽ�ó���� ������
CMesh SelectBarObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> SelectBarObject::SrvDescriptorHeap;//�ؽ�ó �� ��			

																//--------------------------------------------------------------------------------------
bool CoolBarObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CoolBarObject::Textures;//�ؽ�ó���� ������
CMesh  CoolBarObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> CoolBarObject::SrvDescriptorHeap;//�ؽ�ó �� ��
															  //--------------------------------------------------------------------------------------
bool CharacterSelectObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> CharacterSelectObject::Textures;//�ؽ�ó���� ������
CMesh  CharacterSelectObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> CharacterSelectObject::SrvDescriptorHeap;//�ؽ�ó �� ��


																	  //--------------------------------------------------------------------------------------
bool DiceObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> DiceObject::Textures;//�ؽ�ó���� ������
CMesh  DiceObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> DiceObject::SrvDescriptorHeap;//�ؽ�ó �� ��

														   //--------------------------------------------------------------------------------------
bool TimerObject1::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> TimerObject1::Textures;//�ؽ�ó���� ������
CMesh TimerObject1::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> TimerObject1::SrvDescriptorHeap;//�ؽ�ó �� ��
															 //--------------------------------------------------------------------------------------
bool TimerObject2::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> TimerObject2::Textures;//�ؽ�ó���� ������
CMesh  TimerObject2::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> TimerObject2::SrvDescriptorHeap;//�ؽ�ó �� ��
															 //--------------------------------------------------------------------------------------
bool TimerObject3::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> TimerObject3::Textures;//�ؽ�ó���� ������
CMesh  TimerObject3::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> TimerObject3::SrvDescriptorHeap;//�ؽ�ó �� ��

															 //--------------------------------------------------------------------------------------

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
															   // --------------------------------------------------------------------------------------

CMaterial RingObject::Mat;
bool RingObject::CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
unordered_map<string, unique_ptr<CTexture>> RingObject::Textures;//�ؽ�ó���� ������
CMesh RingObject::Mesh;//������ �ѹ��� �������
ComPtr<ID3D12DescriptorHeap> RingObject::SrvDescriptorHeap;//�ؽ�ó �� ��
														   // --------------------------------------------------------------------------------------

CMaterial Rock1Object::Mat;
bool Rock1Object::CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
unordered_map<string, unique_ptr<CTexture>> Rock1Object::Textures;//�ؽ�ó���� ������
CMesh Rock1Object::Mesh;//������ �ѹ��� �������
ComPtr<ID3D12DescriptorHeap> Rock1Object::SrvDescriptorHeap;//�ؽ�ó �� ��

															//--------------------------------------------------------------------------------------
bool RangeObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> RangeObject::Textures;//�ؽ�ó���� ������
CMesh  RangeObject::Mesh;//���߿� �޽��� static���� �־���
ComPtr<ID3D12DescriptorHeap> RangeObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial RangeObject::Mat;
//--------------------------------------------------------------------------------------
bool ShadowObject::CreateMesh = false;
bool ShadowObject::CreatemMesh = false;
bool ShadowObject::CreatecMesh = false;
bool ShadowObject::CreateiMesh = false;
bool ShadowObject::CreateoMesh = false;
bool ShadowObject::CreateSmallwallMesh = false;
bool ShadowObject::CreateTowerMesh = false;
bool ShadowObject::CreateColumnMesh = false;
bool ShadowObject::CreateElevatorMesh = false;

CMesh ShadowObject::cMesh;//���߿� �޽��� static���� �־���
CMesh ShadowObject::iMesh;//���߿� �޽��� static���� �־���
CMesh ShadowObject::oMesh;//���߿� �޽��� static���� �־���
CMesh ShadowObject::mMesh;//���߿� �޽��� static���� �־���
CMesh ShadowObject::smallwallMesh;
CMesh ShadowObject::towerMesh;
CMesh ShadowObject::columnMesh;
CMesh ShadowObject::elevatorMesh;

CMaterial ShadowObject::Mat;
std::vector<ModelAnimation> ShadowObject::cAnimations;
std::vector<ModelAnimation> ShadowObject::iAnimations;
//-------------------------------------------------------------------------------------
bool ImpObject::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> ImpObject::Textures;//�ؽ�ó���� ������
CMesh  ImpObject::Mesh;//���߿� �޽��� static���� �־���
std::vector<ModelAnimation> ImpObject::animations;//�ִϸ��̼� ������ ����. �޽��� �̰�,�ؽ�ó�� �ѹ��������ؼ� �����ϵ����ؾߵ�
ComPtr<ID3D12DescriptorHeap> ImpObject::SrvDescriptorHeap;//�ؽ�ó �� ��
CMaterial ImpObject::Mat;
// --------------------------------------------------------------------------------------
CMaterial BreakCartObject::Mat;
bool BreakCartObject::CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
unordered_map<string, unique_ptr<CTexture>> BreakCartObject::Textures;//�ؽ�ó���� ������
CMesh BreakCartObject::Mesh;//������ �ѹ��� �������
ComPtr<ID3D12DescriptorHeap> BreakCartObject::SrvDescriptorHeap;//�ؽ�ó �� ��
																// --------------------------------------------------------------------------------------
bool ColumnObject::CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
CMaterial ColumnObject::Mat;
unordered_map<string, unique_ptr<CTexture>> ColumnObject::Textures;//�ؽ�ó���� ������
CMesh ColumnObject::Mesh;//������ �ѹ��� �������
ComPtr<ID3D12DescriptorHeap> ColumnObject::SrvDescriptorHeap;//�ؽ�ó �� ��
															 // --------------------------------------------------------------------------------------
bool MeteorObject::CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
CMaterial MeteorObject::Mat;
unordered_map<string, unique_ptr<CTexture>> MeteorObject::Textures;//�ؽ�ó���� ������
CMesh MeteorObject::Mesh;//������ �ѹ��� �������
ComPtr<ID3D12DescriptorHeap> MeteorObject::SrvDescriptorHeap;//�ؽ�ó �� ��
// --------------------------------------------------------------------------------------
bool TitleObject::CreateMesh;//���ʷ� false�� �޽��� ������ true���ȴ�.
unordered_map<string, unique_ptr<CTexture>> TitleObject::Textures;//�ؽ�ó���� ������
CMesh TitleObject::Mesh;//������ �ѹ��� �������
ComPtr<ID3D12DescriptorHeap> TitleObject::SrvDescriptorHeap;//�ؽ�ó �� ��
//--------------------------------------------------------------------------------------
bool PointObject1::CreateMesh = false;
unordered_map<string, unique_ptr<CTexture>> PointObject1::Textures;
CMesh PointObject1::Mesh;
ComPtr<ID3D12DescriptorHeap> PointObject1::SrvDescriptorHeap;

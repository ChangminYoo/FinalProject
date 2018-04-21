#pragma once
#include"CGameObject.h"
#include"Camera.h"

class Scene;
typedef struct SkillData
{

	//���� ������ ��ų�ε���. 0~3���� ���� ������. Skills�� �ε�����.
	int SellectBulletIndex = 0;

	//��ų�� �� 4������ ����. ������ ���� �ǹ��ϴ°��� �����Ҷ� ���� ��ų�ѹ���
	int Skills[4] = { 0,0,0,0 };

	//�� ��ų���� ��Ÿ��.
	float SkillsCoolTime[4] = { 0.0f };

	//��ų�� ��밡�� ����
	bool isSkillOn[4] = { true };

	
};
class CPlayer
{
public:
	
	CPlayer(HWND hwnd,ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist, float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u);
	~CPlayer();
	CGameObject* TraceObject=NULL;
	void TPSCameraSystem(int mx,int my,float DeltaTime);
	void PlayerCameraReLocate();
	void SetPlayer(CGameObject* obj);
	void PlayerInput(float DeltaTime,Scene* scene);
	CCamera Camera;
	CGameObject* PlayerObject;

	//ī�޶� ȸ������ �����͵�
	float ox = -1;//�������콺 x��ǥ
	float oy = -1;//�������콺 y��ǥ
	float xtheta = 0;
	float ytheta = 0;

	SkillData skilldata;
	

	void Tick(float DeltaTime);
	void CreateBullet(ID3D12Device* Device,ID3D12GraphicsCommandList* cl, XMFLOAT3& Goal, CGameObject* lock, list<CGameObject*>* bulletlist);
	void CheckTraceSkill();//���� ������ ��ų�ѹ����� ���������Ͱ� �����ؾ��ϴ��� �˻��ϰ�, �˻��� ����� MouseTrace�� ������. 
	bool MouseTrace = false;

};


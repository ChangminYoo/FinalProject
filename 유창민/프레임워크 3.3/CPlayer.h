#pragma once
#include"CGameObject.h"
#include"Camera.h"

class Scene;
//�߻� ��ų�� ���콺�ٿ�� ó���ǰ�(ũ������Ʈ �Ҹ�)
//������� �÷��̾���ǲ���� ó���ȴ�.
struct SkillData
{

	//���� ������ ��ų�ε���. 0~3���� ���� ������. Skills�� �ε�����.
	int SellectBulletIndex = 0;

	//��ų�� �� 4������ ����. ������ ���� �ǹ��ϴ°��� �����Ҷ� ���� ��ų�ѹ���
	int Skills[4] = { 0,0,0,0 };

	//�� ��ų���� ��Ÿ��. 0~3�� ���� ����.
	float SkillsCoolTime[4] = { 0.0f };

	//��� ��ų���� ��Ÿ�ӵ��� ������. ��ų������ �þ ����, �迭�� Ŀ�����Ѵ�.
	/*
	��ų�ѹ��� 0 : ����Ʈ ť��
	��ų�ѹ��� 1 : ��� ť��
	��ų�ѹ��� 2 : ��Ʈ����ũ
	��ų�ѹ��� 3 : ���̽�Ʈ����ũ
	��ų�ѹ��� 4 : �ĵ���
	��ų�ѹ��� 5 : �ǵ�
	��ų�ѹ��� 6 : �ظӺҷ�
	��ų�ѹ��� 7 : ���׿�
	*/
	float SkillsMaxCoolTime[8] = { 0.6f, 1.1f, 25.0f, 5.0f,20.0f, 20.0f,3.5f, 5.0f };


	//��ų�� ��밡�� ����
	bool isSkillOn[8] = { true };

	
};

typedef struct PointRank
{
	int Point=0;//���� ����
	int Rank=5;//���� ��ŷ
	bool TopMode = false;//1���϶� ž���. ���� ü�� + 800 / ũ�� 1.3�� / �̼� 1.5��
	bool Init = false;//1���ϸ� 1������ �ʱ�ȭ �ؾ���.
}PointRank;

class CPlayer
{
public:
	UINT MouseOn = 0;
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
	
	bool DiceReady = false;
	void Tick(float DeltaTime);
	void CreateBullet(ID3D12Device* Device,ID3D12GraphicsCommandList* cl, XMFLOAT3& Goal, CGameObject* lock, list<CGameObject*>* bulletlist);
	void CheckTraceSkill();//���� ������ ��ų�ѹ����� ���������Ͱ� �����ؾ��ϴ��� �˻��ϰ�, �˻��� ����� MouseTrace�� ������. 
	bool MouseTrace = false;


	//�������� 1���̶�� �Ѱ��ָ� �ɷ�ġ ��ȭ.
	PointRank pointrank;

};


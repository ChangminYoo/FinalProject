#pragma once
#include"CGameObject.h"
#include"Camera.h"
#include"server\main_client\AsyncClient.h"

class AsyncClient;
class Scene;

//�߻� ��ų�� ���콺�ٿ�� ó���ǰ�(ũ������Ʈ �Ҹ�)
//������� �÷��̾���ǲ���� ó���ȴ�.
typedef struct SkillData
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

public:
	//������

	void CreateOtherClientBullet(ID3D12Device* Device, ID3D12GraphicsCommandList* cl, Position3D& Goal, CGameObject* lock, list<CGameObject*>* bulletlist, STC_BulletObject_Info server_bulldata);
	void CreateOtherClientDicestrikeSkill(ID3D12Device* Device, ID3D12GraphicsCommandList* cl, Position3D& Goal, CGameObject* lock, list<CGameObject*>* bulletlist, STC_BulletObject_Info server_bulldata, const XMFLOAT3& OffLookvector);

	char		  curr_playerdir{ 0 };
	bool		  m_right_flag{ false }, m_left_flag{ false }, m_up_flag{ false }, m_down_flag{ false };
	bool		  m_jump_flag{ false };

	unsigned int m_lobbySceneMyID;
	AsyncClient * m_async_client;
};


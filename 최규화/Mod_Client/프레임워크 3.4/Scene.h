#pragma once
#include "d3dUtil.h"
#include"Shader.h"
#include "Timer.h"
#include"Light.h"
#include"Sound.h"
#include "CPlayer.h"

using Microsoft::WRL::ComPtr;

#define LIGHT_MAX 10
#define GS_START 1//���ۻ���
#define GS_LOAD 2//�ε�����
#define GS_PLAY 3//�÷��� ����
#define GS_END 4//���� ����


class Scene
{
private:
	//����� ��ü ��ġ�� ���� ����� �ִ´�.
	float BigWall_X1 = 300 * sinf(0.4f * MMPE_PI);
	float BigWall_Z1 = (600 * cosf(0.4f * MMPE_PI) + 600) / 2;

	float BigWall_X2 = ((600 * sinf(0.4f * MMPE_PI)) + (600 * sinf(0.8f * MMPE_PI))) / 2;
	float BigWall_Z2 = ((600 * cosf(0.4f * MMPE_PI)) + (-600 * cosf(0.2f * MMPE_PI))) / 2;

	float BigWall_Z3 = -600 * cosf(0.2f * MMPE_PI);

	float BigWall_Rad1 = MMPE_PI / 5; //36degree

public:
	Scene(HWND hwnd, ID3D12Device* m_Device, ID3D12GraphicsCommandList* m_DC, float cw, float ch);
	Scene();
	~Scene();
	float mWidth = 0;
	float mHeight = 0;
	bool resize = false;

	//���ӿ�����Ʈ, ī�޶�,���̴�������Ʈ,����Ʈ,���÷� �����ؾߴ�	
	int nShader;//���̴� ����
	int GAMESTATE = GS_START;//���ӻ���.
	bool FirstLoad = false;//ù �ε��ΰ�?

	HWND hWnd;
	ComPtr<ID3D12RootSignature> rootsg;//��Ʈ�ñ״�ó
	array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
	CSound *Sound= NULL;
public:

	//���� ����. �� �ΰ����� �����ӿ�ũ���� �����صа��� �����ٰ��� ���Ƿ� �ڴ�� ������ �ϸ� �ȵȴ�.
	ID3D12GraphicsCommandList* commandlist;//Ŀ�ǵ帮��Ʈ = dc
	ID3D12Device* device;//����̽�

	void SetGameState(int num) { GAMESTATE = num; }//���ӻ��� ����
	void SceneState();
	void CreateRootSignature();//��Ʈ�ñ״�ó�� �׻� ����� ���ҽ���� �����ؾ���.(���� ���ؽ��� �ε������۴� ����)
	void CreateShaderObject();
	void CreateGameObject();
	void CreateUI();
	//UI���� ƽ�Լ��� ����⺸�� �׳� UI���� ���ƽ�� ���⿡�� ó����. ���̷����ϳĸ� UI�� ���� � ������Ʈ����Ʈ�� ���� �ʱ⶧��.
	void UITick(const GameTimer& gt);
	//	void CreateAllObject();
	//	void DeleteStaticMesh();

	int GetGameState() { return GAMESTATE; }

	void Render(const GameTimer& gt);	//���� ī�޶� ���ڷ� �޾ƾߵ�
	void Tick(const GameTimer& gt);


	Shader* Shaders = NULL;
	CPlayer* Player = NULL;
	CLight* light = NULL;

	list<CGameObject*> DynamicObject;//�ִϸ��̼��� �Ǵ� ������Ʈ���� ���⿡ ����.
	list<CGameObject*> BulletObject;//����ü���� ���δ�.
	list<CGameObject*> NoCollObject;
	list<CGameObject*> StaticObject;//�ִϸ��̼��� ���� ��� ������Ʈ���� ���δ�.
	list<CGameObject*> BbObject;//�����尡 ���Ǵ� ������Ʈ��(������, hp��, ��Ÿ ��ƼŬ��)
	list<CGameObject*> RigidObject;//������ٵ� ����ϴ� ������Ʈ��.
	CGameObject* SkyObject = NULL;//��ī�̹ڽ��� ��ü�� �ٷ��. ��ī�̹ڽ��� �ϳ��� ��.

	vector<CGameObject*> LandObject;
	list<CGameObject*> Shadows;

	CGameObject* AimUI = NULL;
	CGameObject* BackGround = NULL;
	CGameObject* CharacterSelect = NULL;
	CGameObject* SkillUI[4] = {NULL};
	CGameObject* SkillFrameUI[4] = { NULL };
	CGameObject* SkillCoolBar[4] = { NULL };
	CGameObject* SelectBar = NULL; //��ų����

	CGameObject* EmptyObject = nullptr;


public:

	//������
	Player_Data * Get_PlayerServerData(const unsigned int& id);
	Player_Data* Get_MonsterServerData(const unsigned int& id);
	int my_ClientID{ 0 };

	void SET_PLAYER_BY_SEVER_DATA(const unsigned short& id, const Player_Data& playerdata, const unsigned char& packet_type);

	void SET_NPC_BY_SERVER_DATA(const unsigned short& id, const Npc_Data& data, const unsigned char& monster_type, const unsigned char& packet_type);
	void SET_NPC_ATTACK_BY_SERVER_DATA(const unsigned short& id, const NPC_BulletObject_Info& data, unsigned char type, bool first_bullet, const XMFLOAT4& xmf4_pos);

	void SET_SOBJECT_BY_SERVER_DATA(const unsigned short& id, StaticObject_Info& sobjdata, const unsigned char& type);
	void SET_MOVEOBJECT_BY_SERVER_DATA(const unsigned int& id, MoveObjectData& mvobjdata);
	void SET_MOVEOBJECT_NOCREATE_BY_SERVER_DATA(const unsigned int&id, MoveObjectData_NoCreate& nomvobjdata);
	void SET_RIGIDOBJECT_BY_SERVER_DATA(const unsigned int& id, RigidbodyData& rbobjdata, const unsigned char& type);

	void SET_BULLET_BY_SERVER_DATA(STC_BulletObject_Info& bulldata, const unsigned char& packet_type, bool first_bullet, const XMFLOAT3& OffLookvector = { 0,0,0 });

	void SET_PLAYER_ANIMATION_BY_SERVER_DATA(const unsigned short& id, const unsigned char& ani);

	void Set_RemovePlayerData(const unsigned int& id, Player_Data& playerdata);

	void SET_PLAYER_STATE_BY_SERVER_DATA(const Player_Data& playerdata);

	void SET_PLAYER_SKILL(const unsigned int& id, const STC_SkillData& playerdata);

	void SET_PLAYER_SKILL(const unsigned int& id, const STC_SkillData& playerdata, unsigned char texture_number);


	//�������� ���� PlayerData�� �̿��ϸ� �� �Լ��� �����
	//PlayerData�� ������� Player�� ���� �޶������̹Ƿ�
	CGameObject* Get_GameObject{ nullptr };


};


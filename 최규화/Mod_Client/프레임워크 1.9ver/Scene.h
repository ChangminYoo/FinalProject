#pragma once
#include"d3dUtil.h"
#include"Shader.h"
#include"GameTimer.h"
#include"Light.h"
#include"CPlayer.h"
using Microsoft::WRL::ComPtr;

#define LIGHT_MAX 10
#define GS_START 1//���ۻ���
#define GS_LOAD 2//�ε�����
#define GS_PLAY 3//�÷��� ����
#define GS_END 4//���� ����

class Scene
{
public:
	Scene();
	~Scene();
	float mWidth = 0;
	float mHeight = 0;
	//���ӿ�����Ʈ, ī�޶�,���̴�������Ʈ,����Ʈ,���÷� �����ؾߴ�
	int nShader;//���̴� ����
	
	ComPtr<ID3D12RootSignature> rootsg;//��Ʈ�ñ״�ó
	

	void CreateRootSignature();//��Ʈ�ñ״�ó�� �׻� ����� ���ҽ���� �����ؾ���.(���� ���ؽ��� �ε������۴� ����)

	array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

	
public:
	
	//���� ����. �� �ΰ����� �����ӿ�ũ���� �����صа��� �����ٰ��� ���Ƿ� �ڴ�� ������ �ϸ� �ȵȴ�.
	ID3D12GraphicsCommandList* commandlist;//Ŀ�ǵ帮��Ʈ = dc
	ID3D12Device* device;//����̽�
	
	Shader** Shaders=NULL;

	int GAMESTATE = GS_START;//���ӻ���.

	
//	void SetGameState(int num);//���ӻ��� ����

	bool FirstLoad = true;//ù �ε��ΰ�?

	
	Scene(ID3D12Device* m_Device, ID3D12GraphicsCommandList* m_DC, float cw, float ch);
	

	void CreateShaderObject();
	void CreateGameObject();
//	void CreateAllObject();

//	void DeleteStaticMesh();

	void Render();	//���� ī�޶� ���ڷ� �޾ƾߵ�
	void Tick(const GameTimer& gt);

	CPlayer* Player = NULL;
	CLight* light = NULL;
	list<CGameObject*> DynamicObject;//�ִϸ��̼��� �Ǵ� ������Ʈ���� ���⿡ ����.

//�����߰�
public :
	Player_Data* Get_PlayerServerData(const unsigned int& id);
	Player_Data* Get_MonsterServerData(const unsigned int& id);

	//�������� ���� PlayerData�� �̿��ϸ� �� �Լ��� �����
	//PlayerData�� ������� Player�� ���� �޶������̹Ƿ�
	CGameObject* Get_GameObject{ nullptr };
};


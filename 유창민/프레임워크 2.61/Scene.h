#pragma once
#include "d3dUtil.h"
#include"Shader.h"
#include "Timer.h"
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
	Scene(HWND hwnd,ID3D12Device* m_Device, ID3D12GraphicsCommandList* m_DC, float cw, float ch);
	Scene();
	~Scene();
	float mWidth = 0;
	float mHeight = 0;
	//���ӿ�����Ʈ, ī�޶�,���̴�������Ʈ,����Ʈ,���÷� �����ؾߴ�	
	int nShader;//���̴� ����
	int GAMESTATE = GS_START;//���ӻ���.
	bool FirstLoad = true;//ù �ε��ΰ�?

	HWND hWnd;
	ComPtr<ID3D12RootSignature> rootsg;//��Ʈ�ñ״�ó
	array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

public:
	
	//���� ����. �� �ΰ����� �����ӿ�ũ���� �����صа��� �����ٰ��� ���Ƿ� �ڴ�� ������ �ϸ� �ȵȴ�.
	ID3D12GraphicsCommandList* commandlist;//Ŀ�ǵ帮��Ʈ = dc
	ID3D12Device* device;//����̽�
	
	void SetGameState(int num) { GAMESTATE = num; }//���ӻ��� ����

	void CreateRootSignature();//��Ʈ�ñ״�ó�� �׻� ����� ���ҽ���� �����ؾ���.(���� ���ؽ��� �ε������۴� ����)
	void CreateShaderObject();
	void CreateGameObject();
	void CreateUI();
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
	list<CGameObject*> StaticObject;//�ִϸ��̼��� ���� ��� ������Ʈ���� ���δ�.
	list<CGameObject*> BbObject;//�����尡 ���Ǵ� ������Ʈ��(������, hp��, ��Ÿ ��ƼŬ��)
	list<CGameObject*> RigidObject;//������ٵ� ����ϴ� ������Ʈ��.
	CGameObject* SkyObject=NULL;//��ī�̹ڽ��� ��ü�� �ٷ��. ��ī�̹ڽ��� �ϳ��� ��.
	
	vector<CGameObject*> LandObject;


	CGameObject* AimUI = NULL;
};


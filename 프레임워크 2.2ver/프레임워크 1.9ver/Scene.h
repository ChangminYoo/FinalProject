#pragma once
#include "d3dUtil.h"
#include"Shader.h"
#include "GameTimer.h"
#include"Light.h"
#include"CPlayer.h"


#define LIGHT_MAX 10
#define GS_START 1//���ۻ���
#define GS_LOAD 2//�ε�����
#define GS_PLAY 3//�÷��� ����
#define GS_END 4//���� ����


class Scene
{
public:
	Scene(ID3D12Device* m_Device, ID3D12GraphicsCommandList* m_DC, float cw, float ch);
	Scene();
	~Scene();
	//���ӿ�����Ʈ, ī�޶�,���̴�������Ʈ,����Ʈ,���÷� �����ؾߴ�
	
	ComPtr<ID3D12RootSignature> rootsg;//��Ʈ�ñ״�ó
	array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
	
	//���� ����. �� �ΰ����� �����ӿ�ũ���� �����صа��� �����ٰ��� ���Ƿ� �ڴ�� ������ �ϸ� �ȵȴ�.
	ID3D12GraphicsCommandList* commandlist;//Ŀ�ǵ帮��Ʈ = dc
	ID3D12Device* device;//����̽�

	//
	Shader** Shaders = NULL;
	CPlayer* Player = NULL;
	CLight* light = NULL;

	//GameObject
	list<CGameObject*> DynamicObject;//�ִϸ��̼��� �Ǵ� ������Ʈ���� ���⿡ ����.
	list<CGameObject*> StaticObject;

public:
	
//	void SetGameState(int num);//���ӻ��� ����
	void CreateShaderObject();
	void CreateGameObject();
	void CreateRootSignature();//��Ʈ�ñ״�ó�� �׻� ����� ���ҽ���� �����ؾ���.(���� ���ؽ��� �ε������۴� ����)
//	void CreateAllObject();
//	void DeleteStaticMesh();
	int GetGameState() { return GameState; };


	void Render(const GameTimer& gt);	//���� ī�޶� ���ڷ� �޾ƾߵ�
	void Tick(const GameTimer& gt);


private:
	bool FirstLoad = true;//ù �ε��ΰ�?

	int GameState = GS_START;//���ӻ���.
	float mWidth = 0;
	float mHeight = 0;
	int nShader;//���̴� ����
};


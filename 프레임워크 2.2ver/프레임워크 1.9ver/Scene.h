#pragma once
#include "d3dUtil.h"
#include"Shader.h"
#include "GameTimer.h"
#include"Light.h"
#include"CPlayer.h"


#define LIGHT_MAX 10
#define GS_START 1//시작상태
#define GS_LOAD 2//로딩상태
#define GS_PLAY 3//플레이 상태
#define GS_END 4//엔딩 상태


class Scene
{
public:
	Scene(ID3D12Device* m_Device, ID3D12GraphicsCommandList* m_DC, float cw, float ch);
	Scene();
	~Scene();
	//게임오브젝트, 카메라,세이더오브젝트,라이트,샘플러 포함해야댐
	
	ComPtr<ID3D12RootSignature> rootsg;//루트시그니처
	array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
	
	//주의 할점. 이 두가지는 프레임워크에서 생성해둔것을 가져다가만 쓰므로 멋대로 릴리즈 하면 안된다.
	ID3D12GraphicsCommandList* commandlist;//커맨드리스트 = dc
	ID3D12Device* device;//디바이스

	//
	Shader** Shaders = NULL;
	CPlayer* Player = NULL;
	CLight* light = NULL;

	//GameObject
	list<CGameObject*> DynamicObject;//애니메이션이 되는 오브젝트들이 여기에 모임.
	list<CGameObject*> StaticObject;

public:
	
//	void SetGameState(int num);//게임상태 변경
	void CreateShaderObject();
	void CreateGameObject();
	void CreateRootSignature();//루트시그니처는 항상 연결될 리소스들과 동일해야함.(물론 버텍스나 인덱스버퍼는 제외)
//	void CreateAllObject();
//	void DeleteStaticMesh();
	int GetGameState() { return GameState; };


	void Render(const GameTimer& gt);	//이후 카메라를 인자로 받아야됨
	void Tick(const GameTimer& gt);


private:
	bool FirstLoad = true;//첫 로딩인가?

	int GameState = GS_START;//게임상태.
	float mWidth = 0;
	float mHeight = 0;
	int nShader;//세이더 갯수
};


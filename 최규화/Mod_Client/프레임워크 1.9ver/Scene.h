#pragma once
#include"d3dUtil.h"
#include"Shader.h"
#include"GameTimer.h"
#include"Light.h"
#include"CPlayer.h"
using Microsoft::WRL::ComPtr;

#define LIGHT_MAX 10
#define GS_START 1//시작상태
#define GS_LOAD 2//로딩상태
#define GS_PLAY 3//플레이 상태
#define GS_END 4//엔딩 상태

class Scene
{
public:
	Scene();
	~Scene();
	float mWidth = 0;
	float mHeight = 0;
	//게임오브젝트, 카메라,세이더오브젝트,라이트,샘플러 포함해야댐
	int nShader;//세이더 갯수
	
	ComPtr<ID3D12RootSignature> rootsg;//루트시그니처
	

	void CreateRootSignature();//루트시그니처는 항상 연결될 리소스들과 동일해야함.(물론 버텍스나 인덱스버퍼는 제외)

	array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

	
public:
	
	//주의 할점. 이 두가지는 프레임워크에서 생성해둔것을 가져다가만 쓰므로 멋대로 릴리즈 하면 안된다.
	ID3D12GraphicsCommandList* commandlist;//커맨드리스트 = dc
	ID3D12Device* device;//디바이스
	
	Shader** Shaders=NULL;

	int GAMESTATE = GS_START;//게임상태.

	
//	void SetGameState(int num);//게임상태 변경

	bool FirstLoad = true;//첫 로딩인가?

	
	Scene(ID3D12Device* m_Device, ID3D12GraphicsCommandList* m_DC, float cw, float ch);
	

	void CreateShaderObject();
	void CreateGameObject();
//	void CreateAllObject();

//	void DeleteStaticMesh();

	void Render();	//이후 카메라를 인자로 받아야됨
	void Tick(const GameTimer& gt);

	CPlayer* Player = NULL;
	CLight* light = NULL;
	list<CGameObject*> DynamicObject;//애니메이션이 되는 오브젝트들이 여기에 모임.

//서버추가
public :
	Player_Data* Get_PlayerServerData(const unsigned int& id);
	Player_Data* Get_MonsterServerData(const unsigned int& id);

	//서버에서 받은 PlayerData를 이용하면 이 함수는 없어도됨
	//PlayerData를 기반으로 Player의 값이 달라질것이므로
	CGameObject* Get_GameObject{ nullptr };
};


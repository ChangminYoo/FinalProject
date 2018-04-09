#pragma once
#include "d3dUtil.h"
#include"Shader.h"
#include "Timer.h"
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
	Scene(HWND hwnd,ID3D12Device* m_Device, ID3D12GraphicsCommandList* m_DC, float cw, float ch);
	Scene();
	~Scene();
	float mWidth = 0;
	float mHeight = 0;
	//게임오브젝트, 카메라,세이더오브젝트,라이트,샘플러 포함해야댐	
	int nShader;//세이더 갯수
	int GAMESTATE = GS_START;//게임상태.
	bool FirstLoad = true;//첫 로딩인가?

	HWND hWnd;
	ComPtr<ID3D12RootSignature> rootsg;//루트시그니처
	array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

public:
	
	//주의 할점. 이 두가지는 프레임워크에서 생성해둔것을 가져다가만 쓰므로 멋대로 릴리즈 하면 안된다.
	ID3D12GraphicsCommandList* commandlist;//커맨드리스트 = dc
	ID3D12Device* device;//디바이스
	
	void SetGameState(int num) { GAMESTATE = num; }//게임상태 변경

	void CreateRootSignature();//루트시그니처는 항상 연결될 리소스들과 동일해야함.(물론 버텍스나 인덱스버퍼는 제외)
	void CreateShaderObject();
	void CreateGameObject();
	void CreateUI();
//	void CreateAllObject();
//	void DeleteStaticMesh();

	int GetGameState() { return GAMESTATE; }

	void Render(const GameTimer& gt);	//이후 카메라를 인자로 받아야됨
	void Tick(const GameTimer& gt);


	Shader* Shaders = NULL;
	CPlayer* Player = NULL;
	CLight* light = NULL;

	list<CGameObject*> DynamicObject;//애니메이션이 되는 오브젝트들이 여기에 모임.
	list<CGameObject*> BulletObject;//투사체들이 모인다.
	list<CGameObject*> StaticObject;//애니메이션이 없는 모든 오브젝트들이 모인다.
	list<CGameObject*> BbObject;//빌보드가 사용되는 오브젝트들(데미지, hp바, 기타 파티클등)
	list<CGameObject*> RigidObject;//리지드바디를 사용하는 오브젝트들.
	CGameObject* SkyObject=NULL;//스카이박스용 객체를 다룬다. 스카이박스는 하나면 됨.
	
	vector<CGameObject*> LandObject;


	CGameObject* AimUI = NULL;
};


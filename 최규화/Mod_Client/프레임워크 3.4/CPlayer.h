#pragma once
#include"CGameObject.h"
#include"Camera.h"
#include"server\main_client\AsyncClient.h"

class AsyncClient;
class Scene;

//발사 스킬은 마우스다운에서 처리되고(크리에이트 불릿)
//생존기는 플레이어인풋에서 처리된다.
typedef struct SkillData
{

	//현재 선택한 스킬인덱스. 0~3까지 값만 가진다. Skills의 인덱스다.
	int SellectBulletIndex = 0;

	//스킬들 총 4개까지 있음. 내부의 값이 의미하는것은 생성할때 사용될 스킬넘버링
	int Skills[4] = { 0,0,0,0 };

	//각 스킬별로 쿨타임. 0~3의 값을 가짐.
	float SkillsCoolTime[4] = { 0.0f };

	//모든 스킬들의 쿨타임들을 정의함. 스킬갯수가 늘어날 수록, 배열도 커져야한다.
	/*
	스킬넘버링 0 : 라이트 큐브
	스킬넘버링 1 : 헤비 큐브
	스킬넘버링 2 : 테트라이크
	스킬넘버링 3 : 다이스트라이크
	스킬넘버링 4 : 파동파
	스킬넘버링 5 : 실드
	스킬넘버링 6 : 해머불렛
	스킬넘버링 7 : 메테오
	*/
	float SkillsMaxCoolTime[8] = { 0.6f, 1.1f, 25.0f, 5.0f,20.0f, 20.0f,3.5f, 5.0f };


	//스킬별 사용가능 여부
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

	//카메라 회전관련 데이터들
	float ox = -1;//이전마우스 x좌표
	float oy = -1;//이전마우스 y좌표
	float xtheta = 0;
	float ytheta = 0;

	SkillData skilldata;
	
	bool DiceReady = false;
	void Tick(float DeltaTime);
	void CreateBullet(ID3D12Device* Device,ID3D12GraphicsCommandList* cl, XMFLOAT3& Goal, CGameObject* lock, list<CGameObject*>* bulletlist);
	void CheckTraceSkill();//현재 선택한 스킬넘버링이 추적데이터가 존재해야하는지 검사하고, 검사한 결과를 MouseTrace에 입힌다. 
	bool MouseTrace = false;

public:
	//서버용

	void CreateOtherClientBullet(ID3D12Device* Device, ID3D12GraphicsCommandList* cl, Position3D& Goal, CGameObject* lock, list<CGameObject*>* bulletlist, STC_BulletObject_Info server_bulldata);
	void CreateOtherClientDicestrikeSkill(ID3D12Device* Device, ID3D12GraphicsCommandList* cl, Position3D& Goal, CGameObject* lock, list<CGameObject*>* bulletlist, STC_BulletObject_Info server_bulldata, const XMFLOAT3& OffLookvector);

	char		  curr_playerdir{ 0 };
	bool		  m_right_flag{ false }, m_left_flag{ false }, m_up_flag{ false }, m_down_flag{ false };
	bool		  m_jump_flag{ false };

	unsigned int m_lobbySceneMyID;
	AsyncClient * m_async_client;
};


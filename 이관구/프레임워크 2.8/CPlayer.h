#pragma once
#include"CGameObject.h"
#include"Camera.h"

class Scene;
typedef struct SkillData
{

	//현재 선택한 스킬인덱스. 0~3까지 값만 가진다. Skills의 인덱스다.
	int SellectBulletIndex = 0;

	//스킬들 총 4개까지 있음. 내부의 값이 의미하는것은 생성할때 사용될 스킬넘버링
	int Skills[4] = { 0,0,0,0 };

	//각 스킬별로 쿨타임.
	float SkillsCoolTime[4] = { 0.0f };

	//스킬별 사용가능 여부
	bool isSkillOn[4] = { true };

	
};
class CPlayer
{
public:
	
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
	

	void Tick(float DeltaTime);
	void CreateBullet(ID3D12Device* Device,ID3D12GraphicsCommandList* cl, XMFLOAT3& Goal, CGameObject* lock, list<CGameObject*>* bulletlist);
	void CheckTraceSkill();//현재 선택한 스킬넘버링이 추적데이터가 존재해야하는지 검사하고, 검사한 결과를 MouseTrace에 입힌다. 
	bool MouseTrace = false;

};


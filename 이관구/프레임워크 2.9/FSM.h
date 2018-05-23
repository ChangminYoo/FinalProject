#pragma once
/*
	일단 여러 상태를 둘 수도 있지만, 공격/대기/추적 정도만 구현할 예정.
	추후에 다양한 공격상태를 추가해도 좋을듯.

*/
#include"CGameObject.h"

struct AIdata
{
	//현재 선택된 타겟
	CGameObject* Target = NULL;
	XMFLOAT4 LastPosition;//타겟을 발견했던 마지막 위치.
	float VisionLength = 150;
	float FireLength = 30;
	bool FireOn = true;
	float cooltime = 0;
	float damagetime = 0;//0.2초후 데미지를 입히도록 하기 위한용
	float timeout = 0.0f;
};

class state
{

public:
	
	virtual state* Execute(float DeltaTime, CGameObject* master, AIdata& adata) { return NULL; }

};

class state_attack : public state
{
	static state* instance;
	state_attack() {}
public:
	static state* Instance();
	virtual state* Execute(float DeltaTime, CGameObject* master, AIdata& adata);

};

class state_idle : public state
{
	static state* instance;
	state_idle() {}
public:
	static state* Instance();
	virtual state* Execute(float DeltaTime, CGameObject* master, AIdata& adata);

};

class state_trace : public state
{
	static state* instance;
	state_trace() {}
public:
	static state* Instance();
	virtual state* Execute(float DeltaTime, CGameObject* master, AIdata& adata);

};


class state_global: public state
{
	static state* instance;
	state_global() {}
public:
	static state* Instance();
	virtual state* Execute(float DeltaTime, CGameObject* master, AIdata& adata);

};


/*
	FSM 흐름.
	몬스터 클래스의 매 틱마다 FSM의 업데이트를 호출.
	먼저 체크타겟이 호출되면서 현재 시야로 볼수 있는 플레이어나 육감(엄청나게 가까운 경우 뒤에 있거나 시야에 안보여도 찾아내도록할 예정)으로 타겟을
	얻어옴.이때 마지막 발견 지점도 생성 및 변경

	만약 타겟을 발견하지 못했으면, 글로벌 상태에서 별다른 처리가 없을것이고, 이는 현재 상태가 중요해짐. 보통 대기상태였다면 그냥 대기하고 있음.
	그러나 추적상태였다면, 해당 목표지점까지 가야함. 갔다가 



*/

class FSM
{
public:
	CGameObject * Master = NULL;
	//타겟이 될 수 있는 오브젝트들.
	list<CGameObject*>* DynamicObj = NULL;
	AIdata aidata;
private:
	state * GlobalState = NULL;//모든 상태에서 바뀔 수 있는 상태. 대표적으로 공격상태가 있음. 추적상태든 대기상태든 공격을 해야할땐 바로 바뀌어야하니까.
	state* CurrentState = NULL;//현재 상태.
public:
	void Update(float DeltaTime);//업데이트에서 현재 상태와 글로벌상태를 처리함. 또 체크타겟을 해서 데이터를 수집함.
	void ChangeState(state* st);//'현재 상태' 를 바꿈
	void CheckTarget(float DeltaTime);

	FSM(CGameObject* master,list<CGameObject*>* dobj);
	~FSM()
	{

	}
};

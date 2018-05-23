#pragma once
/*
	�ϴ� ���� ���¸� �� ���� ������, ����/���/���� ������ ������ ����.
	���Ŀ� �پ��� ���ݻ��¸� �߰��ص� ������.

*/
#include"CGameObject.h"

struct AIdata
{
	//���� ���õ� Ÿ��
	CGameObject* Target = NULL;
	XMFLOAT4 LastPosition;//Ÿ���� �߰��ߴ� ������ ��ġ.
	float VisionLength = 150;
	float FireLength = 30;
	bool FireOn = true;
	float cooltime = 0;
	float damagetime = 0;//0.2���� �������� �������� �ϱ� ���ѿ�
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
	FSM �帧.
	���� Ŭ������ �� ƽ���� FSM�� ������Ʈ�� ȣ��.
	���� üũŸ���� ȣ��Ǹ鼭 ���� �þ߷� ���� �ִ� �÷��̾ ����(��û���� ����� ��� �ڿ� �ְų� �þ߿� �Ⱥ����� ã�Ƴ������� ����)���� Ÿ����
	����.�̶� ������ �߰� ������ ���� �� ����

	���� Ÿ���� �߰����� ��������, �۷ι� ���¿��� ���ٸ� ó���� �������̰�, �̴� ���� ���°� �߿�����. ���� �����¿��ٸ� �׳� ����ϰ� ����.
	�׷��� �������¿��ٸ�, �ش� ��ǥ�������� ������. ���ٰ� 



*/

class FSM
{
public:
	CGameObject * Master = NULL;
	//Ÿ���� �� �� �ִ� ������Ʈ��.
	list<CGameObject*>* DynamicObj = NULL;
	AIdata aidata;
private:
	state * GlobalState = NULL;//��� ���¿��� �ٲ� �� �ִ� ����. ��ǥ������ ���ݻ��°� ����. �������µ� �����µ� ������ �ؾ��Ҷ� �ٷ� �ٲ����ϴϱ�.
	state* CurrentState = NULL;//���� ����.
public:
	void Update(float DeltaTime);//������Ʈ���� ���� ���¿� �۷ι����¸� ó����. �� üũŸ���� �ؼ� �����͸� ������.
	void ChangeState(state* st);//'���� ����' �� �ٲ�
	void CheckTarget(float DeltaTime);

	FSM(CGameObject* master,list<CGameObject*>* dobj);
	~FSM()
	{

	}
};

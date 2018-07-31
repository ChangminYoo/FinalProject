#pragma once
/*
	�ϴ� ���� ���¸� �� ���� ������, ����/���/���� ������ ������ ����.
	���Ŀ� �پ��� ���ݻ��¸� �߰��ص� ������.

*/ 

class CPlayerObject;

enum curState_State
{
	s_Idle, s_Attack, s_Trace
};

struct AIdata
{
	//���� ���õ� Ÿ��
	CPlayerObject* Target = NULL;
	XMFLOAT4 LastPosition;//Ÿ���� �߰��ߴ� ������ ��ġ.
	float VisionLength = 250;
	float FireLength = 35;
	bool FireOn = true;
	float cooltime = 0;
	float damagetime = 0;//0.2���� �������� �������� �ϱ� ���ѿ�
	float timeout = 0.0f;
	curState_State curstateEnum = s_Idle;
	XMFLOAT3 collisionmove=XMFLOAT3(0,0,0);
	int stack{ 0 };//�� ������ �ǹ��ϴ°��� 5���� ������ �ֺ��� ���������� ���� ȸ����Ŵ.
};

class state
{

public:	
	virtual state* Execute(double deltime, CNpcObject* master, AIdata& adata) { return NULL; }

};

class state_attack : public state
{
	static state* instance;
	state_attack() {}
public:
	static state* Instance();
	virtual state* Execute(double deltime, CNpcObject* master, AIdata& adata);

};

class state_idle : public state
{
	static state* instance;
	state_idle() {}
public:
	static state* Instance();
	virtual state* Execute(double deltime, CNpcObject* master, AIdata& adata);

};

class state_trace : public state
{
	static state* instance;
	state_trace() {}
public:
	static state* Instance();
	virtual state* Execute(double deltime, CNpcObject* master, AIdata& adata);

};


class state_global: public state
{
	static state* instance;
	state_global() {}
public:
	static state* Instance();
	virtual state* Execute(double deltime, CNpcObject* master, AIdata& adata);

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
	CNpcObject * Master = NULL;
	//Ÿ���� �� �� �ִ� ������Ʈ��.

	//list<CGameObject*>* DynamicObj = NULL;
	//list<CGameObject*>* StaticObj = NULL;
	//list<CGameObject*>* BulletObj = NULL;

	AIdata aidata;
private:
	state * GlobalState = NULL;//��� ���¿��� �ٲ� �� �ִ� ����. ��ǥ������ ���ݻ��°� ����. �������µ� �����µ� ������ �ؾ��Ҷ� �ٷ� �ٲ����ϴϱ�.
	state* CurrentState = NULL;//���� ����.
public:
	void Update(double deltime);//������Ʈ���� ���� ���¿� �۷ι����¸� ó����. �� üũŸ���� �ؼ� �����͸� ������.
	void ChangeState(state* st);//'���� ����' �� �ٲ�
	void CheckTarget(double deltime);

	//FSM(CNpcObject* master,vector<CPlayerObject*>* pobjs, unordered_set<CStaticObject*>* sobjs, list<CBulletObject*>* bobjs);
	FSM(CNpcObject* master);
	~FSM()
	{

	}
};

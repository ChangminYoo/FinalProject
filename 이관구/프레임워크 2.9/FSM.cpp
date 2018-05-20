#include "FSM.h"

// ����ƽ ������� ����
state* state_global::instance = NULL;
state* state_idle::instance=NULL;
state* state_attack::instance = NULL;

//-----------------------------------------------------------//


void FSM::Update(float DeltaTime)
{
	CheckTarget(DeltaTime);
	ChangeState(GlobalState->Execute(DeltaTime, Master, aidata));
	ChangeState(CurrentState->Execute(DeltaTime, Master, aidata));
}

void FSM::ChangeState(state * st)
{
	if (st != NULL)
	{
		CurrentState = st;
	}
}

void FSM::CheckTarget(float DeltaTime)
{
	for (auto i = DynamicObj->begin(); i!=DynamicObj->end(); i++)
	{
		auto l = FloatLength(Float4Add((*i)->CenterPos, Master->CenterPos, false));
		
		if (aidata.VisionLength >= l)
		{
			//����� ��ֹ��� �̿����� ���Ѵ�. ���Ŀ� �߰��Ұ�



			//���� ��ֹ��� �������� �ʾҴٸ�. �� ������� ���� ��ֹ��� �Ȱ�������.
			if (aidata.Target == NULL)
			{
				aidata.Target = *i;
			}
			else
			{
				//�̹�Ÿ���� �ְ�, �� Ÿ�ٺ��� ����� ����������
				if (l < FloatLength(Float4Add(aidata.Target->CenterPos, Master->CenterPos, false)))
					aidata.Target = *i;
			}
		}
	}

}

FSM::FSM(CGameObject* master, list<CGameObject*>* dobj)
{
	Master = master;
	DynamicObj = dobj;
	GlobalState = state_global::Instance();
	CurrentState = state_idle::Instance();
	

}

state * state_idle::Instance()
{
	if (instance == NULL)
	{
		instance = new state_idle;
	}

	return instance;
}

state * state_idle::Execute(float DeltaTime, CGameObject* master, AIdata& adata)
{
	//����� ���� ó���ҰԾ���. �׳� �ش� ��ġ�� ����ϸ鼭 �ִϸ��̼��� ���̵��̸� �ȴ�.
	if (master != NULL)
	{
		master->SetAnimation(Ani_State::Idle);
		
	}
	return NULL;
}

state * state_global::Instance()
{
	if (instance == NULL)
	{
		instance = new state_global;
	}

	return instance;
}

state * state_global::Execute(float DeltaTime, CGameObject * master, AIdata& adata)
{
	if (master != NULL)
	{
		//Ÿ���� �����ϰ�, �ش� Ÿ���� ��Ÿ� �ȿ� ������ ���� ���·� ��ȯ�Ѵ�.
		if (adata.Target != NULL && FloatLength(Float4Add(adata.Target->CenterPos, master->CenterPos, false)) <= adata.FireLength)
			return state_attack::Instance();

	}

	return NULL;
}

state * state_attack::Instance()
{
	if (instance == NULL)
	{
		instance = new state_attack;
	}

	return instance;
}

state * state_attack::Execute(float DeltaTime, CGameObject * master, AIdata & adata)
{
	// ���� ���� �����ؾ��Ѵ�.
	return nullptr;
}

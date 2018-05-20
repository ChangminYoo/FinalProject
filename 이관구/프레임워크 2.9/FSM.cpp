#include "FSM.h"

// 스테틱 멤버변수 생성
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
			//현재는 장애물을 이용하지 못한다. 이후에 추가할것



			//만약 장애물에 가려지지 않았다면. 즉 여기까지 오면 장애물에 안가려진것.
			if (aidata.Target == NULL)
			{
				aidata.Target = *i;
			}
			else
			{
				//이미타겟이 있고, 그 타겟보다 가까운 적이있으면
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
	//현재는 따로 처리할게없음. 그냥 해당 위치를 사수하면서 애니메이션이 아이들이면 된다.
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
		//타겟이 존재하고, 해당 타겟이 사거리 안에 있으면 공격 상태로 전환한다.
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
	// 공격 상태 구현해야한다.
	return nullptr;
}

#include "FSM.h"

// 스테틱 멤버변수 생성
state* state_global::instance = NULL;
state* state_idle::instance=NULL;
state* state_attack::instance = NULL;
state* state_trace::instance = NULL;

//-----------------------------------------------------------//


void FSM::Update(float DeltaTime)
{
	if (Master->gamedata.HP > 0)
	{
		//타겟 검색 
		CheckTarget(DeltaTime);
		ChangeState(GlobalState->Execute(DeltaTime, Master, aidata));//공격상태로 전환 가능한지 따진다.
		ChangeState(CurrentState->Execute(DeltaTime, Master, aidata));//현재 상태를 처리함.

		//공격을 할수없는 경우
		if (aidata.FireOn == false)
		{
			//쿨타임을 줄이자
			aidata.cooltime -= DeltaTime;
			if (aidata.cooltime <= 0)
			{
				//공격가능해진 타이밍이면 데미지를 입힐 수있는 지연타임을 0초로 초기화하고 쿨타임도 0으로초기화하고 공격가능으로 바꿈.
				aidata.damagetime = 0;
				aidata.cooltime = 0;
				aidata.FireOn = true;
			}

		}

		//매 프레임마다, 몬스터가 플레이어를 볼 수 있으면 보는 방향으로 회전해야함.
		//룩벡터와 플레이어-몬스터 의 노멀라이즈 한 벡터의 사잇각만큼 회전필요
		//사잇각은 코사인하면되는데 알다시피 각도가 음수든 양수든 같은 결과를 가지므로 즉, 절댓값 각을 가지므로
		//회전축을 알아야함. 회전축은 외적으로 구함.
		//이두가지를 이용해 회전용 쿼터니언을 구하고 이를 곱하면 끝
		
			auto l1 = Master->Lookvector;
			auto l2 = Float3Add(XMFloat4to3(aidata.LastPosition), XMFloat4to3(Master->CenterPos), false);
			l2.y = 0;
			l2 = Float3Normalize(l2);

			float t = l1.x*l2.x + l1.y*l2.y + l1.z*l2.z;

			if (t < -1)
				t = -1;
			if (t > 1)
				t = 1;
			float ac = acos(t);

			
			
			auto axis = Float3Cross(l1, l2);
			axis = Float3Normalize(axis);
			
			Master->Orient = QuaternionMultiply(Master->Orient, QuaternionRotation(axis, ac));
		}
	
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
	aidata.Target = NULL;
	for (auto i = DynamicObj->begin(); i!=DynamicObj->end(); i++)
	{
		if (*i == Master)
			continue;

		auto v1 = Float4Add((*i)->CenterPos, Master->CenterPos, false);
		auto nv1 = Float4Normalize(v1);
		float nv1dotLook = nv1.x*Master->Lookvector.x + nv1.y*Master->Lookvector.y + nv1.z*Master->Lookvector.z;
		auto l = FloatLength(v1);
		
		if (aidata.VisionLength >= l && nv1dotLook>0.5)
		{
			//현재는 장애물을 이용하지 못한다. 이후에 추가할것



			//만약 장애물에 가려지지 않았다면. 즉 여기까지 오면 장애물에 안가려진것.
			if (aidata.Target == NULL)
			{
				aidata.Target = *i;
				aidata.LastPosition = (*i)->CenterPos;
			}
			else
			{
				//이미타겟이 있고, 그 타겟보다 가까운 적이있으면
				if (l < FloatLength(Float4Add(aidata.Target->CenterPos, Master->CenterPos, false)))
				{
					aidata.Target = *i;
					aidata.LastPosition = (*i)->CenterPos;
				}
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
	aidata.LastPosition = master->CenterPos;

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
		if (adata.Target != NULL)
			return state_trace::Instance();

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
		if (adata.Target != NULL && FloatLength(Float4Add(adata.Target->CenterPos, master->CenterPos, false)) <= adata.FireLength && adata.FireOn)
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
	if (adata.FireOn)
		master->SetAnimation(Ani_State::Attack);

	if (adata.damagetime >= 0.2f && adata.FireOn==true)
	{
		
		if (adata.Target != NULL)
			adata.Target->ToDamage(0.20);

		adata.FireOn = false;
		
		adata.cooltime = 0.7f;
		
		
	}
	else
		adata.damagetime += DeltaTime;
	
	//공격 모션이 끝나면 아이들 애니메이션일테므로 아이들 상태로 변경가능

	if (master->n_Animation == Ani_State::Idle)
		return state_idle::Instance();
	
	return nullptr;
}

state * state_trace::Instance()
{
	if (instance == NULL)
	{
		instance = new state_trace;
	}

	return instance;
}

state * state_trace::Execute(float DeltaTime, CGameObject * master, AIdata & adata)
{
	master->SetAnimation(Ani_State::Run);
	auto v = XMFloat4to3(Float4Add(adata.LastPosition, master->CenterPos, false));
	v.y = 0;
	auto d = FloatLength(v);
	auto v2 = Float4Add(XMFLOAT4(100, 0, 110, 0), master->CenterPos,false);
	v2.y = 0;
	v = Float3Normalize(v);
	v = Float3Float(v, DeltaTime);
	if (fabs(d) <= 20 && adata.Target == NULL)
		adata.LastPosition = XMFLOAT4(100, 0, 110, 0);//현재는 테스트중이니까 이렇게했고, 골렘은 고유위치 즉 원래위치를 기억해둬야함.

	if(fabs(d) > adata.FireLength)
		master->CenterPos=Float4Add(master->CenterPos, XMFloat3to4(Float3Float(v, 20)));
	if (FloatLength(v2) <= 20 && adata.Target == NULL) 
		return state_idle::Instance();


	return nullptr;
}

#include "../stdafx.h"

// 스테틱 멤버변수 생성
state* state_global::instance = NULL;
state* state_idle::instance=NULL;
state* state_attack::instance = NULL;
state* state_trace::instance = NULL;

//-----------------------------------------------------------//


void FSM::Update(double deltime)
{
	if (Master->GetMyCurrHP() > 0)
	{
		//타겟 검색 - 바라볼 수 있는 플레이어가 있는지 확인. LastPosition에 플레이어를 서치하면 해당 플레이어의 위치를 저장
		CheckTarget(deltime);
		ChangeState(GlobalState->Execute(deltime, Master, aidata));//공격상태로 전환 가능한지 따진다.
		ChangeState(CurrentState->Execute(deltime, Master, aidata));//현재 상태를 처리함.

		//공격을 할수없는 경우
		
		if (aidata.FireOn == false)
		{
			//쿨타임을 줄이자
			aidata.cooltime -= deltime;
			if (aidata.cooltime <= 0 && static_cast<unsigned int>(Master->GetMyAnimation()) != Ani_State::Attack)
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
		
		
		auto l1 = Master->GetLookVector();
		auto l2 = Float3Add(XMFloat4to3(aidata.LastPosition), XMFloat4to3(Master->GetCenterPos4f()), false);
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
		
		Master->SetRotatePos4f(QuaternionMultiply(Master->GetRotatePos4f(), QuaternionRotation(axis, ac)));
		
	}
		
	
} 

void FSM::ChangeState(state * st)
{
	if (st != NULL)
	{
		CurrentState = st;
	}
}

void FSM::CheckTarget(double deltime)
{
	aidata.Target = NULL;
	for (auto iter = g_clients.begin(); iter!= g_clients.end(); ++iter)
	{
		//Master는 몬스터 npc - client들과 비교
		//if (*iter == Master)
		//	continue;

		auto v1 = Float4Add((*iter)->GetCenterPos4f(), Master->GetCenterPos4f(), false);
		v1.y = 0;
		auto nv1 = Float4Normalize(v1);
		float nv1dotLook = nv1.x*Master->GetLookVector().x + nv1.y*Master->GetLookVector().y + nv1.z*Master->GetLookVector().z;
		auto l = FloatLength(v1);
		
		bool blind = false;
		if (aidata.VisionLength >= l && nv1dotLook>1.53/2.0f)
		{
			if (aidata.Target == NULL)
			{
				aidata.Target = *iter;
				aidata.LastPosition = (*iter)->GetCenterPos4f();
			}
			else
			{
				//이미타겟이 있고, 그 타겟보다 가까운 적이있으면
				if (l < FloatLength(Float4Add(aidata.Target->GetCenterPos4f(), Master->GetCenterPos4f(), false)))
				{
					aidata.Target = *iter;
					aidata.LastPosition = (*iter)->GetCenterPos4f();
				}
			}
		}
	}

}


FSM::FSM(CNpcObject* master)
{
	Master = master;
	//DynamicObj = dobj;
	//StaticObj = sobj;
	//BulletObj = bobj;
	GlobalState = state_global::Instance();
	CurrentState = state_idle::Instance();
	aidata.LastPosition = master->GetCenterPos4f();

}


state * state_idle::Instance()
{	
	if (instance == NULL)
	{
		instance = new state_idle;
	}

	return instance;
}


state * state_idle::Execute(double deltime, CNpcObject* master, AIdata& adata)
{
	adata.curstateEnum = s_Idle;
	//현재는 따로 처리할게없음. 그냥 해당 위치를 사수하면서 애니메이션이 아이들이면 된다.
	if (master != NULL)
	{
		if (adata.Target != NULL)
			return state_trace::Instance();
		else
		{
			auto v2 = Float4Add(master->GetOriginCenterPos4f(), master->GetCenterPos4f(), false);
			v2.y = 0;

			if (FloatLength(v2) > 50)
				 return state_trace::Instance();
		}
		master->SetMyAnimation(Ani_State::Idle);

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


state * state_global::Execute(double deltime, CNpcObject* master, AIdata& adata)
{
	if (master != NULL)
	{
		//if (adata.stack >= 5)
		if (adata.stack >= 1)
		{
			g_bullets.emplace_back(new CStoneBulletObject(master, master->GetCenterPos4f(), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(30, 0, 0, 0)));
			g_bullets.emplace_back(new CStoneBulletObject(master, master->GetCenterPos4f(), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(-30, 0, 0, 0)));
			g_bullets.emplace_back(new CStoneBulletObject(master, master->GetCenterPos4f(), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 30, 0)));
			g_bullets.emplace_back(new CStoneBulletObject(master, master->GetCenterPos4f(), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, -30, 0)));
			
			/*
			((ImpObject*)master)->fsm->BulletObj->push_back(new StoneBullet(master->device, master->commandlist, master->ParticleList, NULL, master, XMFLOAT4(0, 0, 0, 1), NULL, master->CenterPos, XMFLOAT4(30, 0, 0, 0)));
			((ImpObject*)master)->fsm->BulletObj->push_back(new StoneBullet(master->device, master->commandlist, master->ParticleList, NULL, master, XMFLOAT4(0, 0, 0, 1), NULL, master->CenterPos, XMFLOAT4(-30, 0, 0, 0)));
			((ImpObject*)master)->fsm->BulletObj->push_back(new StoneBullet(master->device, master->commandlist, master->ParticleList, NULL, master, XMFLOAT4(0, 0, 0, 1), NULL, master->CenterPos, XMFLOAT4(0, 0, 30, 0)));
			((ImpObject*)master)->fsm->BulletObj->push_back(new StoneBullet(master->device, master->commandlist, master->ParticleList, NULL, master, XMFLOAT4(0, 0, 0, 1), NULL, master->CenterPos, XMFLOAT4(0, 0, -30, 0)));
			*/

			adata.stack = 0;
		}

		float l;
		XMFLOAT4 v1;

		if (adata.Target != NULL)
		{
			v1 = Float4Add(adata.Target->GetCenterPos4f(), master->GetCenterPos4f(), false);
			v1.y = 0;
			l = FloatLength(v1);
		}

		//타겟이 존재하고, 해당 타겟이 사거리 안에 있으면 공격 상태로 전환한다.
		if (adata.Target != NULL && l <= adata.FireLength && adata.FireOn)
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


state * state_attack::Execute(double deltime, CNpcObject* master, AIdata & adata)
{
	adata.curstateEnum = s_Attack;

	if (adata.FireOn)
		master->SetMyAnimation(Ani_State::Attack);
	
	if (adata.damagetime >= 0.2f && adata.FireOn==true)
	{
		if (adata.Target != NULL)
		{
			adata.Target->GetDamaged(master->GetMyBasicStatus().attack);
			adata.stack += 1;
		}

		adata.FireOn = false;
		
		adata.cooltime = 0.7f;
		
		
	}
	else
		adata.damagetime += deltime;
	
	//공격 모션이 끝나면 아이들 애니메이션일테므로 아이들 상태로 변경가능

	if (static_cast<unsigned int>(master->GetMyAnimation()) == Ani_State::Idle)
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


state * state_trace::Execute(double deltime, CNpcObject* master, AIdata& adata)
{
	adata.curstateEnum = s_Trace;
	master->SetMyAnimation(Ani_State::Run);

	auto v = XMFloat4to3(Float4Add(adata.LastPosition, master->GetCenterPos4f(), false));
	v.y = 0;
	auto d = FloatLength(v);
	auto v2 = Float4Add(master->GetOriginCenterPos4f(), master->GetCenterPos4f(), false);
	v2.y = 0;

	v = Float3Normalize(v);
	v = Float3Add(v, adata.collisionmove);
	v = Float3Normalize(v);
	v = Float3Float(v, deltime);

	if (fabs(d) <= adata.FireLength && adata.Target == NULL)
		adata.LastPosition = master->GetOriginCenterPos4f();//고유 초창기 위치로 ㄱㄱ

	if (fabs(FloatLength(v2)) >= 75)		//중앙에서 어느정도 멀어지면 
	{
		adata.LastPosition = master->GetOriginCenterPos4f();
	}

	if (adata.Target != NULL)
	{
		if(fabs(d) > adata.FireLength)//타겟이 존재하면 타겟을 공격할 수 있는 사정거리까지만 간다.
			master->SetCenterPos4f(Float4Add(master->GetCenterPos4f(), XMFloat3to4(Float3Float(v, master->GetMyBasicStatus().speed))));
	}
	else//아니라면 그냥 해당 위치로 계속감.
		master->SetCenterPos4f(Float4Add(master->GetCenterPos4f(), XMFloat3to4(Float3Float(v, master->GetMyBasicStatus().speed))));
	
	if (FloatLength(v2) <= 20 && adata.Target == NULL) 
		return state_idle::Instance();

	master->UpdatePhysicsCenterPos();		//pp의 centerpos 업데이트

	return nullptr;
}

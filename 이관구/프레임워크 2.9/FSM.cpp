#include "FSM.h"

// ����ƽ ������� ����
state* state_global::instance = NULL;
state* state_idle::instance=NULL;
state* state_attack::instance = NULL;
state* state_trace::instance = NULL;

//-----------------------------------------------------------//


void FSM::Update(float DeltaTime)
{
	if (Master->gamedata.HP > 0)
	{
		CheckTarget(DeltaTime);
		ChangeState(GlobalState->Execute(DeltaTime, Master, aidata));
		ChangeState(CurrentState->Execute(DeltaTime, Master, aidata));

		if (aidata.FireOn == false)
		{
			aidata.cooltime -= DeltaTime;
			if (aidata.cooltime <= 0)
			{
				aidata.cooltime = 0;
				aidata.FireOn = true;
			}

		}

		
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
			//����� ��ֹ��� �̿����� ���Ѵ�. ���Ŀ� �߰��Ұ�



			//���� ��ֹ��� �������� �ʾҴٸ�. �� ������� ���� ��ֹ��� �Ȱ�������.
			if (aidata.Target == NULL)
			{
				aidata.Target = *i;
				aidata.LastPosition = (*i)->CenterPos;
			}
			else
			{
				//�̹�Ÿ���� �ְ�, �� Ÿ�ٺ��� ����� ����������
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
	//����� ���� ó���ҰԾ���. �׳� �ش� ��ġ�� ����ϸ鼭 �ִϸ��̼��� ���̵��̸� �ȴ�.
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
		//Ÿ���� �����ϰ�, �ش� Ÿ���� ��Ÿ� �ȿ� ������ ���� ���·� ��ȯ�Ѵ�.
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
		adata.damagetime = 0;
		adata.cooltime = 0.7f;
		
		
	}
	else
		adata.damagetime += DeltaTime;
	
	//���� ����� ������ ���̵� �ִϸ��̼����׹Ƿ� ���̵� ���·� ���氡��

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
		adata.LastPosition = XMFLOAT4(100, 0, 110, 0);//����� �׽�Ʈ���̴ϱ� �̷����߰�, ���� ������ġ �� ������ġ�� ����ص־���.

	if(fabs(d) > adata.FireLength)
		master->CenterPos=Float4Add(master->CenterPos, XMFloat3to4(Float3Float(v, 20)));
	if (FloatLength(v2) <= 20 && adata.Target == NULL) 
		return state_idle::Instance();


	return nullptr;
}

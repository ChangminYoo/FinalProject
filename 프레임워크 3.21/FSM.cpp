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
		//Ÿ�� �˻� 
		CheckTarget(DeltaTime);
		ChangeState(GlobalState->Execute(DeltaTime, Master, aidata));//���ݻ��·� ��ȯ �������� ������.
		ChangeState(CurrentState->Execute(DeltaTime, Master, aidata));//���� ���¸� ó����.

		//������ �Ҽ����� ���
		if (aidata.FireOn == false)
		{
			//��Ÿ���� ������
			aidata.cooltime -= DeltaTime;
			if (aidata.cooltime <= 0 && Master->n_Animation!=Ani_State::Attack)
			{
				//���ݰ������� Ÿ�̹��̸� �������� ���� ���ִ� ����Ÿ���� 0�ʷ� �ʱ�ȭ�ϰ� ��Ÿ�ӵ� 0�����ʱ�ȭ�ϰ� ���ݰ������� �ٲ�.
				aidata.damagetime = 0;
				aidata.cooltime = 0;
				aidata.FireOn = true;
			}

		}

		//�� �����Ӹ���, ���Ͱ� �÷��̾ �� �� ������ ���� �������� ȸ���ؾ���.
		//�躤�Ϳ� �÷��̾�-���� �� ��ֶ����� �� ������ ���հ���ŭ ȸ���ʿ�
		//���հ��� �ڻ����ϸ�Ǵµ� �˴ٽ��� ������ ������ ����� ���� ����� �����Ƿ� ��, ���� ���� �����Ƿ�
		//ȸ������ �˾ƾ���. ȸ������ �������� ����.
		//�̵ΰ����� �̿��� ȸ���� ���ʹϾ��� ���ϰ� �̸� ���ϸ� ��
		
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
		//v1.y = 0;
		auto nv1 = Float4Normalize(v1);
		float nv1dotLook = nv1.x*Master->Lookvector.x + nv1.y*Master->Lookvector.y + nv1.z*Master->Lookvector.z;
		auto l = FloatLength(v1);
		
		bool blind = false;
		if (aidata.VisionLength >= l && nv1dotLook>1.73/2.0f)
		{
			////����� ��ֹ��� �̿����� ���Ѵ�. ���Ŀ� �߰��Ұ�
			//for (auto s : *StaticObj)
			//{
			//	auto v2 = XMFloat4to3(Float4Add(s->CenterPos, Master->CenterPos, false));
			//	float l2 = FloatLength(v2);
			//	v2 = Float3Normalize(v2);

			//	float b = v2.x*nv1.x + v2.y*nv1.y + v2.z*nv1.z;

			//	float a = nv1.x*nv1.x + nv1.y*nv1.y + nv1.z*nv1.z;

			//	float c = v2.x*v2.x + v2.y*v2.y + v2.z*v2.z - s->pp->GetRad()/2*s->pp->GetRad()/2;

			//	if (b*b - a * c >= 0 && l2<=l && b>0.8f)
			//		blind = true;
			//}

			//if (blind)
			//	continue;
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

FSM::FSM(CGameObject* master, list<CGameObject*>* dobj,list<CGameObject*>* sobj)
{
	Master = master;
	DynamicObj = dobj;
	StaticObj = sobj;
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
	adata.curstateEnum = s_Idle;
	//����� ���� ó���ҰԾ���. �׳� �ش� ��ġ�� ����ϸ鼭 �ִϸ��̼��� ���̵��̸� �ȴ�.
	if (master != NULL)
	{
		if (adata.Target != NULL)
			return state_trace::Instance();
		else
		{
			auto v2 = Float4Add(master->OrgPos, master->CenterPos, false);
			v2.y = 0;

			if (FloatLength(v2) > 50)
				 return state_trace::Instance();
		}
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
		float l;
		XMFLOAT4 v1;
		
		if (adata.Target != NULL)
		{
			v1 = Float4Add(adata.Target->CenterPos, master->CenterPos, false);
			v1.y = 0;
			l = FloatLength(v1);
		}
		//Ÿ���� �����ϰ�, �ش� Ÿ���� ��Ÿ� �ȿ� ������ ���� ���·� ��ȯ�Ѵ�.
		if (adata.Target != NULL &&l <= adata.FireLength && adata.FireOn)
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
	adata.curstateEnum = s_Attack;
	if (adata.FireOn)
		master->SetAnimation(Ani_State::Attack);

	if (adata.damagetime >= 0.2f && adata.FireOn==true)
	{
		
		if (adata.Target != NULL)
		{
			if (adata.Target->isShieldOn)
				adata.Target->ToDamage(100);
			else
				adata.Target->ToDamage(20);
		}

		adata.FireOn = false;
		
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
	adata.curstateEnum = s_Trace;
	master->SetAnimation(Ani_State::Run);
	auto v = XMFloat4to3(Float4Add(adata.LastPosition, master->CenterPos, false));
	v.y = 0;
	auto d = FloatLength(v);
	auto v2 = Float4Add(master->OrgPos, master->CenterPos,false);
	v2.y = 0;
	v = Float3Normalize(v);
	v = Float3Add(v, adata.collisionmove);
	v = Float3Normalize(v);
	v = Float3Float(v, DeltaTime);
	if (fabs(d) <=adata.FireLength && adata.Target == NULL)
		adata.LastPosition = master->OrgPos;//���� ��â�� ��ġ�� ����

	if (adata.Target != NULL)
	{
		if(fabs(d) > adata.FireLength)//Ÿ���� �����ϸ� Ÿ���� ������ �� �ִ� �����Ÿ������� ����.
			master->CenterPos = Float4Add(master->CenterPos, XMFloat3to4(Float3Float(v, master->gamedata.Speed)));
	}
	else//�ƴ϶�� �׳� �ش� ��ġ�� ��Ӱ�.
		master->CenterPos = Float4Add(master->CenterPos, XMFloat3to4(Float3Float(v, master->gamedata.Speed)));
	if (FloatLength(v2) <= 20 && adata.Target == NULL) 
		return state_idle::Instance();

	

	return nullptr;
}

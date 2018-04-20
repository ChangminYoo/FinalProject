#include "StaticObject.h"


void StaticObject::CycleStaticObjects()
{
	pe = new PhysicalEffect();

	//1. ����
	SetPosOfBox();

	InitBoxObjects();

	AfterGravitySystem();
}

void StaticObject::SetPosOfBox()
{
	m_boxPos = 
	{
		{0, {-20.f, 0.f, 0.f, 0.f}},
		{1, {-40.f, 0.f, 0.f, 0.f}},
		{2, {50.f, 0.f, -40.f, 0.f}},
		{3, {30.f, 0.f, 40.f, 0.f}}
	};
}

void StaticObject::InitBoxObjects()
{
	//1. StaticObjectList�� ������ �־��ֱ�.
	//   �ѹ� �����ϰ� �ٲ��� �����Ƿ� �ʱ� �ѹ��� ����

	for (int i = 0; i < MAX_BOX_NUM; ++i)
	{
		StaticObject *sobj = new StaticObject();
		sobj->pp = new PhysicsPoint();
		
		sobj->m_sobjdata.Ani = Ani_State::Idle;
		sobj->m_sobjdata.GodMode = true;
		sobj->m_sobjdata.ID = i;
		sobj->m_sobjdata.Pos = m_boxPos[i];
		sobj->m_sobjdata.Rotate_status = { 0.0f, 0.0f, 0.0f, 0.0f };
		sobj->m_sobjdata.cur_hp = 100;
		sobj->m_sobjdata.origin_hp = 100;
		sobj->m_sobjdata.player_status.attack = 0;
		sobj->m_sobjdata.player_status.speed = 0;
		sobj->m_sobjdata.Fixed = true;

		sobj->OffLookvector = XMFLOAT3(0, 0, 1);
		sobj->OffRightvector = XMFLOAT3(1, 0, 0);

		sobj->pe->UpdateLookVector(sobj->OffLookvector, sobj->OffRightvector, sobj->m_sobjdata.Rotate_status,
								   sobj->Lookvector, sobj->Rightvector);

		sobj->pe->GetUpVector(sobj->Lookvector, sobj->Rightvector, sobj->Upvector);

		XMFLOAT4 xmf4 = { sobj->m_sobjdata.Pos.x, sobj->m_sobjdata.Pos.y, sobj->m_sobjdata.Pos.z, sobj->m_sobjdata.Pos.w };
		sobj->pp->SetPosition(xmf4);
		sobj->pp->SetHalfBox(5, 5, 5);
		sobj->pp->SetDamping(0.5f);
		sobj->pp->SetBounce(false);
		sobj->pp->SetMass(INFINITY);

		m_sobjs.insert(sobj);
	}
}

void StaticObject::AfterGravitySystem()
{
	for(auto sobj : m_sobjs)
	{
		//�� ���� ������ �ƴ� pp�� �������� ó���ϳĸ� ���������� �������� pp�� ������ �����̳�
		//pp�������� �����̰� ���������� �����̳� ������, UpdatePPosCenterPos�� ��������
		//pp�� �����̰� cp�� pp�� �����.
		float ppy = sobj->pp->GetPosition().y;
		float hby = sobj->pp->GetHalfBox().y;
		if (ppy - hby < 0)											//pp�� ����y-�����ڽ��� y���� �Ѱ���� 0���� ������ ���Ʒ��� ������
		{
			XMFLOAT3 gp = sobj->pp->GetPosition();
			gp.y += hby - ppy;										//�׷��� �ݴ�� �����ڽ�y��-����y��ŭ �ø��� �ȴ�.
			sobj->pp->SetPosition(gp);
			sobj->UpdatePPosCenterPos(sobj->pp->GetPosition());
			auto v = sobj->pp->GetVelocity();
			v.y = 0;												//�߷¿� ���� �ӵ��� 0���� ����
			sobj->pp->SetVelocity(v);
			sobj->m_airbone = false;
		}
	}
}

void StaticObject::UpdatePPosCenterPos(XMFLOAT3& xmf3)
{
	XMFLOAT4 xmf4 = XMFloat3to4(xmf3);

	m_sobjdata.Pos.x = xmf4.x;
	m_sobjdata.Pos.y = xmf4.y; 
	m_sobjdata.Pos.z = xmf4.z;
	m_sobjdata.Pos.w = xmf4.w;
}


StaticObject::~StaticObject()
{
	for (auto sobj : m_sobjs)
		delete sobj;

	delete pe;

}


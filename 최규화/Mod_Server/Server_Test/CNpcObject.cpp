#include "stdafx.h"

CNpcObject::CNpcObject()
{
}


CNpcObject::CNpcObject(int id, int type)
{
	m_state = IDLE;

	//-------------------------- �⺻ �����ͼ���

	m_id = id;
	m_monster_type = type;
	m_ani = Ani_State::Idle;
	m_dir = 0;
	m_airbone = false;
	m_ai = true;
	m_pos4f = { 0.f,100.f,0.f,0.f };
	m_orgPos4f = m_pos4f;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };
	m_godmode = false;
	m_fixed = false;
	m_alive = true;
	m_type = OBJECT_TYPE::NPC_MONSTER_IMP;
	m_connect = true;

	m_ability.attack = 20;
	m_ability.orignHP = 5000;
	m_ability.curHP = 5000;
	m_ability.speed = 40;
	m_ability.exp = 0;
	m_ability.level = 1;

	//---------------------------- Orient�� �̿��� Lookvector // Rightvector // Upvector ����
	m_OffLookvector = XMFLOAT3(0, 0, -1);
	m_OffRightvector = XMFLOAT3(-1, 0, 0);

	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, MMPE_PI);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();
	UpdateUpvector();

	//------------------------------ ����ȿ�� �⺻ ������ ����
	pp = new PhysicsPoint();

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(10, 20, 10);
	pp->SetDamping(0.25);
	pp->SetBounce(false);
	pp->SetMass(500);

	//------------------------------ ����ȿ�� ����
	GravitySystem(0);

	pp->integrate(0);

	AfterGravitySystem(0);

	UpdatePPosCenterPos();

	//------------------------------
	UpdateDataForPacket();
}

void CNpcObject::UpdateDataForPacket()
{
	//npc_data.ai = m_ai;
	npc_data.airbone = m_airbone;
	npc_data.ani = m_ani;
	npc_data.connect = m_connect;
	//npc_data.dir = m_dir;
	npc_data.godmode = m_godmode;
	npc_data.id = m_id;
	npc_data.pos = m_pos4f;
	npc_data.rot = m_rot4f;
	npc_data.monster_type = m_monster_type;
	npc_data.alive = m_alive;

	npc_data.status.attack = m_ability.attack;
	npc_data.status.cur_hp = m_ability.curHP;
	npc_data.status.exp = m_ability.exp;
	npc_data.status.level = m_ability.level;
	npc_data.status.origin_hp = m_ability.orignHP;
	npc_data.status.speed = m_ability.speed;
}

//NPC�� ���� ������ �ֱ�
void CNpcObject::GetDamaged(int damage)
{
	if (m_godmode) return;

	if (m_ability.curHP > 0)
	{
		//NPC ���� hp�� ����
		m_ability.curHP -= damage;
	}
	
	if (m_ability.curHP <= 0)
	{
		m_alive = false;
		m_ani = Ani_State::Dead;
		npc_data.alive = false;
		npc_data.ani = Ani_State::Dead;
		
	}
}

Npc_Data CNpcObject::GetMyBasicPacketData() const
{
	return npc_data;
}

void CNpcObject::SetMyAnimation(unsigned char curr_anim)
{
	 m_ani = curr_anim;
	 npc_data.ani = curr_anim; 
}

unsigned char CNpcObject::GetMyAnimation() const
{
	return m_ani; 
}


void CNpcObject::Tick(double deltime)
{
	auto t = pp->GetTotalForce();
	t.y = 0;
	pp->ForceClear();
	pp->AddForce(t);

	*pp->CenterPos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->integrate(deltime);

	UpdatePPosCenterPos();

	if (fsm != nullptr)
		fsm->Update(deltime);

	//cout << "����:" << static_cast<int>(m_ani) << "\n";
}

void CNpcObject::Tick(double deltime, Position & pos4f)
{
	auto t = pp->GetTotalForce();
	t.y = 0;
	pp->ForceClear();
	pp->AddForce(t);

	*pp->CenterPos = { pos4f.x, pos4f.y, pos4f.z, pos4f.w };
	pp->integrate(deltime);

	UpdatePPosCenterPos();

	if (fsm != nullptr)
		fsm->Update(deltime);
	
}

void CNpcObject::Collision(vector<CPlayerObject*>* clients, double deltime)
{
	fsm->aidata.collisionmove = XMFLOAT3(0, 0, 0);

	for (auto iter = clients->begin(); iter != clients->end(); ++iter)
	{
		if (*iter != this)
		{
			bool test = pp->CollisionTest(*(*iter)->pp, m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->m_Lookvector, (*iter)->m_Rightvector, (*iter)->m_Upvector);

			//�浹������ �浹�ؼҸ� �ؾ��Ѵ�.
			if (test)
			{
				//�浹 ������ ���� (0,1,0) �̸� Airbone�� false�� �д�. �̴� ���� ���������� ��Ÿ����.
				if (pp->pAxis.y > 0)
				{
					pp->SetVelocity(pp->GetVelocity().x, 0, pp->GetVelocity().z);
					m_airbone = false;
				}

				//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
				//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
				if (pp->pAxis.y < 0)
				{
					(*iter)->pp->SetVelocity((*iter)->pp->GetVelocity().x, 0, (*iter)->pp->GetVelocity().z);
					(*iter)->SetAirbone(false);
				}

				//������ ��ü�� �ƴϸ�
				XMFLOAT3 cn;
				cn = XMFloat4to3(Float4Add(pp->GetPosition(), ((*iter)->pp)->GetPosition(), false));
				cn = Float3Normalize(cn);

				if (((*iter)->GetObjectType() == Box || (*iter)->GetObjectType() == SmallWall || (*iter)->GetObjectType() == BigWall ||
					(*iter)->GetObjectType() == NormalBuilding || (*iter)->GetObjectType() == Rigidbodybox) && fsm->aidata.curstateEnum == s_Trace)
				{
					xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
					auto v = XMFloat4to3(Float4Add(fsm->aidata.LastPosition, xmf4_pos, false));
					v.y = 0;
					auto d = FloatLength(v); // �ӵ��� ũ��
					v = Float3Normalize(v); // ���� �ӵ� ����

					auto p = XMFloat4to3(Float4Add(xmf4_pos, (*iter)->GetCenterPos4f(), false));
					p.y = 0;
					p = Float3Normalize(p);

					if (Float3Cross(v, p).y > 0) // - 80�� ������ p�� ȸ�����Ѿ���
					{
						auto r = QuaternionRotation(XMFLOAT3(0, 1, 0), -MMPE_PI / 2.5);
						auto m = XMMatrixRotationQuaternion(XMLoadFloat4(&r));

						XMVECTOR pv = XMLoadFloat3(&p);
						pv = XMVector3Transform(pv, m);
						XMStoreFloat3(&fsm->aidata.collisionmove, pv);
					}
					else
					{
						auto r = QuaternionRotation(XMFLOAT3(0, -1, 0), -MMPE_PI / 2.5);
						auto m = XMMatrixRotationQuaternion(XMLoadFloat4(&r));

						XMVECTOR pv = XMLoadFloat3(&p);
						pv = XMVector3Transform(pv, m);
						XMStoreFloat3(&fsm->aidata.collisionmove, pv);
					}
					
				}

				//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
				pp->CollisionResolve(*(*iter)->pp, cn, deltime);//����� ƨ���� �ʴ´�.
				UpdatePPosCenterPos();
				(*iter)->UpdatePPosCenterPos();

				//cout << "���Ϳ� ����� �浹" << "\n";

			} //__ Collision Test ��
		
		} 


	} //__ for�� ��
}

void CNpcObject::Collision(unordered_set<CStaticObject*>* sobjs, double deltime)
{
	fsm->aidata.collisionmove = XMFLOAT3(0, 0, 0);

	for (auto iter = sobjs->begin(); iter != sobjs->end(); ++iter)
	{
		bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
			(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

		//�浹������ �浹�ؼҸ� �ؾ��Ѵ�.
		if (test)
		{
			//�浹 ������ ���� (0,1,0) �̸� Airbone�� false�� �д�. �̴� ���� ���������� ��Ÿ����.
			if (pp->pAxis.y > 0)
			{
				pp->SetVelocity(pp->GetVelocity().x, 0, pp->GetVelocity().z);
				m_airbone = false;
			}

			//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
			//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
			if (pp->pAxis.y < 0)
			{
				(*iter)->GetPhysicsPoint()->SetVelocity((*iter)->GetPhysicsPoint()->GetVelocity().x, 0, (*iter)->GetPhysicsPoint()->GetVelocity().z);
				(*iter)->SetAirbone(false);
			}

			//������ ��ü���
			XMFLOAT3 cn;
			cn = pp->pAxis;

			// static object �� rigidbody �浹üũ
			if (((*iter)->GetObjectType() == Box || (*iter)->GetObjectType() == SmallWall || (*iter)->GetObjectType() == BigWall ||
				(*iter)->GetObjectType() == NormalBuilding || (*iter)->GetObjectType() == Rigidbodybox) && fsm->aidata.curstateEnum == s_Trace)
			{
				xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
				auto v = XMFloat4to3(Float4Add(fsm->aidata.LastPosition, xmf4_pos, false));
				v.y = 0;
				auto d = FloatLength(v); // �ӵ��� ũ��
				v = Float3Normalize(v); // ���� �ӵ� ����

				auto p = XMFloat4to3(Float4Add(xmf4_pos, (*iter)->GetCenterPos4f(), false));
				p.y = 0;
				p = Float3Normalize(p);

				if (Float3Cross(v, p).y > 0) // - 80�� ������ p�� ȸ�����Ѿ���
				{
					auto r = QuaternionRotation(XMFLOAT3(0, 1, 0), -MMPE_PI / 2.5);
					auto m = XMMatrixRotationQuaternion(XMLoadFloat4(&r));

					XMVECTOR pv = XMLoadFloat3(&p);
					pv = XMVector3Transform(pv, m);
					XMStoreFloat3(&fsm->aidata.collisionmove, pv);
				}
				else
				{
					auto r = QuaternionRotation(XMFLOAT3(0, -1, 0), -MMPE_PI / 2.5);
					auto m = XMMatrixRotationQuaternion(XMLoadFloat4(&r));

					XMVECTOR pv = XMLoadFloat3(&p);
					pv = XMVector3Transform(pv, m);
					XMStoreFloat3(&fsm->aidata.collisionmove, pv);
					
				}

			}

			//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
			pp->CollisionResolve(*(*iter)->GetPhysicsPoint(), cn, deltime);//����� ƨ���� �ʴ´�.
			UpdatePPosCenterPos();
			(*iter)->UpdatePPosCenterPos();

			//cout << "���Ϳ� ����ƽ ������Ʈ�� �浹" << "\n";

		} //__ Collision Test ��
			

	} //__ for�� ��

}




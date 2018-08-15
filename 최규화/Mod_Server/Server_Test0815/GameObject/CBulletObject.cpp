#include "../stdafx.h"

int CStoneBulletObject::g_npc_bulletID = -1;

CBulletObject::CBulletObject(const unsigned short & master_id, const unsigned short & my_id, const Position & pos, const Rotation & rot, float bulltime, Vel3f & vel, const unsigned char & type, const Position3D & endpt, const float& degree)
{

	pp = new PhysicsPoint();

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	m_pos4f = pos;
	m_orgPos4f = pos;
	m_rot4f = rot;

	m_orgpluspos = { 0.f, 0.f, 0.f, 0.f };
	m_tempangle = 0.0;

	m_masterID = master_id;
	m_id = my_id;

	m_lifetime = bulltime;
	m_vel3f = vel;

	m_type = type;
	m_alive = true;

	m_savept = endpt;

	m_degree = degree;

	m_createfirst = true;

	m_after_collision = AFTER_COLLISION_EFFECT::EMPTY;		//�⺻�� �浹 �� ��ƼŬ ����x


	/*
	if (m_type == protocol_DiceBullet)
	{
		XMFLOAT3 l{ 0,0,1 };
		XMVECTOR ol = XMLoadFloat3(&l);
		XMVECTOR nl = XMLoadFloat3(&g_clients[m_masterID]->GetLookVector());

		auto axis = XMVector3Cross(ol, nl);
		//�������� �ϼ�.
		axis = XMVector3Normalize(axis);
		XMFLOAT3 Axis;
		XMStoreFloat3(&Axis, axis);
		//���� ȸ�������� ���ؾ��Ѵ�. ������ ���� ȸ�������� ���Ѵ�.

		auto temp = XMVector3Dot(ol, nl);

		float d;//���� �躤�Ϳ� ���ο� �躤�͸� ������ ���.
		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������

		auto ori = QuaternionRotation(Axis, d);
		auto wmatrix = XMMatrixIdentity();
		auto quater = XMLoadFloat4(&ori);
		wmatrix *= XMMatrixRotationQuaternion(quater);

		auto orr = XMVectorSet(1, 0, 0, 0);
		orr = XMVector4Transform(orr, wmatrix);//��¥ ����Ʈ ����
		orr = XMVector3Normalize(orr);
		auto RealRight = XMVector3Cross(axis, nl);//��¥ ����Ʈ����
		RealRight = XMVector3Normalize(RealRight);

		//��¥ ����Ʈ ���Ϳ� ��¥ ����Ʈ ���͸� ������.
		temp = XMVector3Dot(RealRight, orr);

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������
		auto ori2 = XMQuaternionRotationAxis(nl, d);//��¥ �躤�͸� ȸ�������� ��� ��¥����Ʈ�� ��¥����Ʈ�� ���հ���ŭȸ��

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//���� ȸ�� ����
		
		XMFLOAT4 Orient{ 0,0,0,1 };
		Orient = QuaternionMultiply(Orient, ori);
		XMFLOAT3 axis2{ 0,1,0 };
		auto q2 = QuaternionRotation(axis2, m_degree);
		Orient = QuaternionMultiply(Orient, q2);
		
		m_rot4f = { Orient.x, Orient.y, Orient.z, Orient.w };

	}
	*/

	UpdateLookvector();			//degree�� �̿��� ����� ȸ������ Ŭ���̾�Ʈ�κ��� �ǳ׹޾���. �̸� �̿��� lookvector, rightvector, upvector ������Ʈ
	UpdateUpvector();

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetVelocity(vel.x, vel.y, vel.z);

	pp->SetDamping(1);
	pp->SetBounce(false);

	m_ability.curHP = 1;
	m_ability.orignHP = 1;
	m_ability.exp = 0;
	m_ability.level = 0;

	if (BULLET_TYPE::protocol_LightBullet == static_cast<int>(type))
	{
		m_ability.speed = 150;
		pp->SetHalfBox(1, 1, 1);
		pp->SetMass(0.35f);
		m_ability.attack = 1000;
	}
	else if (BULLET_TYPE::protocol_HeavyBullet == static_cast<int>(type)) 
	{
		m_ability.speed = 180;
		pp->SetHalfBox(2, 2, 2);
		pp->SetMass(1.0f);
		m_ability.attack = 30;
	}
	else if (BULLET_TYPE::protocol_DiceBullet == static_cast<int>(type)) 
	{
		m_ability.speed = 50;
		pp->SetHalfBox(2, 2, 2);
		pp->SetMass(0.8f);
		m_ability.attack = 20;
	}
	
	m_bulldata.damage = m_ability.attack;
	UpdateDataForPacket();

	//--------------------------------------------------------------------------------------------

	/*
	m_bulldata.alive = m_alive;				
	m_bulldata.endpoint = m_savept;				//����
	m_bulldata.master_id = m_masterID;			//����
	m_bulldata.my_id = m_id;						//����
	m_bulldata.pos4f = m_pos4f;
	m_bulldata.rot4f = m_rot4f;			
	m_bulldata.type = m_type;					//����
	m_bulldata.vel3f = m_vel3f;					//����
	m_bulldata.degree = m_degree;				//�ѹ� �ʱ�ȭ�Ǹ� �� ���� // ����
	*/

	//--------------------------------------------------------------------------------------------
}

STC_BulletObject_Info CBulletObject::GetChangedBulletState() const
{
	STC_BulletObject_Info stc_bullet;
	stc_bullet.alive = m_alive;
	stc_bullet.degree = m_degree;
	stc_bullet.endpoint = m_savept;
	stc_bullet.master_id = m_masterID;
	stc_bullet.my_id = m_id;
	stc_bullet.pos4f = m_pos4f;
	stc_bullet.rot4f = m_rot4f;
	stc_bullet.type = m_type;
	stc_bullet.after_coll = m_after_collision;
	stc_bullet.damage = m_bulldata.damage;

	return STC_BulletObject_Info(stc_bullet);
}

void CBulletObject::UpdateDataForPacket()
{
	m_bulldata.alive = m_alive;
	m_bulldata.endpoint = m_savept;
	m_bulldata.master_id = m_masterID;
	m_bulldata.my_id = m_id;
	m_bulldata.pos4f = m_pos4f;
	m_bulldata.rot4f = m_rot4f;
	m_bulldata.type = m_type;
	m_bulldata.vel3f = m_vel3f;
	m_bulldata.degree = m_degree;
	m_bulldata.after_coll = m_after_collision;
}

void CBulletObject::AfterGravitySystem()
{
	if (m_pos4f.y <= 0 && m_alive == true)
	{
		m_alive = false;
		m_bulldata.alive = false;
	}
		
}

void CBulletObject::Tick(double deltime)
{
	m_lifetime += deltime;
	
	if (m_lifetime >= MAX_LIGHT_BULLET_TIME && BULLET_TYPE::protocol_LightBullet == static_cast<int>(m_type)) {
		m_alive = false; 
		m_bulldata.alive = false;
		return;
	}
	else if (m_lifetime >= MAX_HEAVY_BULLET_TIME && BULLET_TYPE::protocol_HeavyBullet == static_cast<int>(m_type)) {
		m_alive = false;
		m_bulldata.alive = false;
		return;
	}
	else if (m_lifetime >= MAX_DICE_BULLET_TIME && BULLET_TYPE::protocol_DiceBullet == static_cast<int>(m_type)) {
		m_alive = false;
		m_bulldata.alive = false;
		return;
	}


	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };

	pp->SetPosition(&xmf4_pos);
	
	pp->integrate(deltime);

	xmf4_rot = QuaternionMultiply(xmf4_rot, QuaternionRotation(m_Lookvector, MMPE_PI * deltime));
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };
	
	UpdatePPosCenterPos();	//integrate�� ���� ����� pp->Centerpos�� �̿��� �ҷ� Ŭ������ �����ϴ� �ҷ� ��ġ�� m_pos4f ������Ʈ

	UpdateLookvector();		//Lookvector�� ������ ȸ������ xmf4_rot�� �̿��� QuterninMultiply�� ���� ����� xmf4_rot, �� �ҷ�ȸ������ �̿��� lookvector, rightvector
	UpdateUpvector();		//upvector ������Ʈ

	m_bulldata.pos4f = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	m_bulldata.rot4f = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	m_bulldata.alive = m_alive;

	//cout << "RotX: " << m_rot4f.x << "," << "RotY: " << m_rot4f.y << "," << "RotZ: " << m_rot4f.z << "," << "RotW: " << m_rot4f.w << "\n";

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//�÷��̾� ���� - ����Ʈ �ҷ�, ��� �ҷ�, ���̽���Ʈ����ũ
//npc���� ���� - ���� �ҷ�
//�̵��� CBulletObject���� ����

//�÷��̾ �߻��ϴ� �ҷ�Ÿ�� ��ų��� �÷��̾ �浹�� ���
void CBulletObject::Collision(vector<CPlayerObject*>* clients, double deltime)
{
	for (auto iter = clients->begin(); iter != clients->end(); ++iter)
	{
		//----------------------------------------------------�浹 ��ŵ------------//

		//1. �ش� �ҷ��� �� ������ �� �ڽ��� ���
		if ((*iter)->GetID() == m_masterID) continue;

		//2. �ش� �ҷ��� �� ������ �׾��� ��� 
		if (!g_clients[this->m_masterID]->GetAlive()) continue;

		//3. ��밡 �׾����� ���
		if (!(*iter)->GetAlive()) continue;

		//----------------------------------------------------�浹 ��ŵ------------//

		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				//player �� �ҷ��� �ε����� ��� ������ ��ƼŬ�� �������
				m_after_collision = AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM;
				m_bulldata.after_coll = m_after_collision;

				(*iter)->GetDamaged(m_ability.attack);

				//�ش� �ҷ��� ���� �÷��̾ ���� �����ϰ�� // ������ 1 ��ƼŬ�� �ߵ��� Ŭ�󿡺����ߵ�
				if ((*iter)->GetShieldState())
					m_bulldata.damage = 1;
				else
					m_bulldata.damage = m_ability.attack;

				//������ ��ü�� �ƴϸ�
				// �ҷ��� ���� ��밡 �׾��� ���, �� ��밡 �÷��̾��� ��� ������ �ø���
				if ((*iter)->GetMyCurrHP() <= 0)
				{
					(*iter)->SetMyCharacterDeathCount();
					(*iter)->CalculatePlayerScoreForRanking();

					if (g_clients[this->GetBulletMasterID()]->GetAlive())
					{
						if ((*iter)->GetIsTopRanker())
							g_clients[this->GetBulletMasterID()]->SetPlayerScore(TOP_RANKER_SCORE);
						else
							g_clients[this->GetBulletMasterID()]->SetPlayerScore(NORMAL_PLAYER_SCORE);

						g_clients[this->GetBulletMasterID()]->SetMyCharacterKillCount();
						g_clients[this->GetBulletMasterID()]->CalculatePlayerScoreForRanking();
					}
				}

				XMFLOAT3 cn;
				cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*iter)->GetPhysicsPoint()).GetPosition(), false));
				cn = Float3Normalize(cn);

				(*iter)->GetPhysicsPoint()->SetBounce(true);

				pp->ResolveVelocity(*(*iter)->GetPhysicsPoint(), cn, deltime);
				(*iter)->GetPhysicsPoint()->SetBounce(false);

				m_alive = false;
				m_bulldata.alive = false;
			}
		}
	}
	
}

//�÷��̾ �߻��ϴ� �ҷ�Ÿ�� ��ų��� npc �浹�� ���
void CBulletObject::Collision(vector<CNpcObject*>* npcs, double deltime)
{
	for (auto iter = npcs->begin(); iter != npcs->end(); ++iter)
	{
		//�ҷ�(�÷��̾� �ҷ�, npc �ҷ�)�� �׵��� ����(npc)�� �浹x
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			//----------------------------------------------------�浹 ��ŵ------------//

			//1. �ҷ��� �浹�ߴµ� ������ ������� ��ŵ
			if ((*iter)->GetAlive() == false) continue;

			//2. �ҷ��� �浹�ߴµ�, �ҷ��� �� ��밡 �������
			if (!g_clients[this->m_masterID]->GetAlive()) continue;

			//3. �ش� �ҷ��� ������ �� �ҷ��� ��� ��ŵ
			if (this->GetObjectType() == protocol_NpcStoneBullet) continue;

			//----------------------------------------------------�浹 ��ŵ------------//

			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				//NPC �� �ҷ��� �ε����� ��� ������ ��ƼŬ�� �������
				m_after_collision = AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM;
				m_bulldata.after_coll = m_after_collision;

				(*iter)->GetDamaged(m_ability.attack);

				//npc ������ ��� -> �Ѿ� ������ �������Ѵ�
				if ((*iter)->GetMyBasicPacketData().monster_type == IMP)
					(*iter)->fsm->aidata.LastPosition = this->GetCenterPos4f();
				

				//�÷��̾ �� �ҷ��� �°� ����(IMP)������ ü���� 0���ϰ� �Ǿ� �׾���
				//�ش� �ҷ��� ����ID�� ���� �÷��̾ ����ִٸ� ������ �ø���
				//����(IMP) ���ھ�� 100
				if ((*iter)->GetMyCurrHP() <= 0)
				{
					if (g_clients[this->GetBulletMasterID()]->GetAlive())
					{
						g_clients[this->GetBulletMasterID()]->SetPlayerScore(MONSTER_IMP_SCORE);
						g_clients[this->GetBulletMasterID()]->SetMyCharacterKillCount(3);
						g_clients[this->GetBulletMasterID()]->CalculatePlayerScoreForRanking();
					}
				}

				//������ ��ü�� �ƴϸ�
				XMFLOAT3 cn;
				cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*iter)->GetPhysicsPoint()).GetPosition(), false));
				cn = Float3Normalize(cn);
				(*iter)->GetPhysicsPoint()->SetBounce(true);

				pp->ResolveVelocity(*(*iter)->GetPhysicsPoint(), cn, deltime);
				(*iter)->GetPhysicsPoint()->SetBounce(false);

				m_alive = false;
				m_bulldata.alive = false;
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CBulletObject::Collision(unordered_set<CStaticObject*>* sobjs, double deltime)
{

	for (auto iter = sobjs->begin(); iter != sobjs->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				//StaticObject �� �ҷ��� �ε����� ��� ������ ��ƼŬ�� ����� ���ƾ���
				m_after_collision = AFTER_COLLISION_EFFECT::BOOM;
				m_bulldata.after_coll = m_after_collision;

				//������ ��ü��
				XMFLOAT3 cn;
				cn = pp->pAxis;

				pp->ResolveVelocity(*(*iter)->GetPhysicsPoint(), cn, deltime);
				(*iter)->GetPhysicsPoint()->SetBounce(false);
				m_alive = false;
				m_bulldata.alive = false;
			}
		}
	}
	
}

void CBulletObject::SetBulletRotatevalue(const XMFLOAT4 & xmf4)
{
	m_rot4f.x = xmf4.x;
	m_rot4f.y = xmf4.y;
	m_rot4f.z = xmf4.z;
	m_rot4f.w = xmf4.w;

	//m_bulldata.Rotate_status = m_rot4f;
}

CBulletObject::~CBulletObject()
{
	//if (pp != nullptr)
	//	delete pp;
}


STC_HammerSkillInfo CHammerBulletObject::GetChangedHammerBulletState() const
{
	return STC_HammerSkillInfo(m_stc_hammerInfo);
}

CHammerBulletObject::CHammerBulletObject(const unsigned short & master_id, const unsigned short & myid, const Position & pos, const Rotation & rot, int numOfHammer, const XMFLOAT4 & opp)
{
	m_masterID = master_id;
	m_id = myid;

	m_headBullet = true;

	m_type = BULLET_TYPE::protocol_HammerBullet;
	m_alive = true;
	m_createfirst = true;

	m_after_collision = AFTER_COLLISION_EFFECT::EMPTY;
	
	m_orgPos4f = pos;
	m_orgpluspos = opp;
	m_tempangle = 0.0;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	//ó�� �ظӺҷ� ��Ŷ�� ��� rot���� �ش� �ظӺҷ��� ori���̴� (0 , 0 , 0 , 1)
	XMFLOAT4 ori = { rot.x, rot.y, rot.z, rot.w };
	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	xmf4_rot = QuaternionMultiply(xmf4_rot, ori);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();
	UpdateUpvector();

	m_numOfHammer = numOfHammer;
	m_ability.curHP = 1;
	m_ability.orignHP = 1;
	m_ability.attack = 50;
	m_godmode = false;
	m_ability.speed = 0;
	m_lifetime = 12.0;
	
	m_pos4f = pos;
	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };

	pp = new PhysicsPoint();
	pp->SetPosition(&xmf4_pos); //�� ���� �׻� ���ŵǾ��Ѵ�.
	pp->SetHalfBox(2, 2, 2);	//�浹 �ڽ��� x,y,z ũ��
	pp->SetDamping(1);			//������ ��� ���Ǵ� ���ΰ��. �� ƽ���� 0.5�辿 �ӵ�����
	pp->SetBounce(false);		//ƨ���� �ʴ´�.
	pp->SetVelocity(0, 0, 0);	//Ʈ���������� �����ϰ���
	pp->SetMass(5.35f);

	UpdateDataForPacket();
}

void CHammerBulletObject::UpdateDataForPacket()
{
	m_stc_hammerInfo.my_id = m_id;
	m_stc_hammerInfo.master_id = m_masterID;

	m_stc_hammerInfo.pos4f = m_pos4f;
	m_stc_hammerInfo.rot4f = m_rot4f;
	m_stc_hammerInfo.opp_pos4f = { m_orgpluspos.x, m_orgpluspos.y, m_orgpluspos.z, m_orgpluspos.w };


	m_stc_hammerInfo.alive = m_alive;
	m_stc_hammerInfo.damage = m_ability.attack;
	m_stc_hammerInfo.create_first = m_createfirst;
	m_stc_hammerInfo.after_coll = m_after_collision;
	m_stc_hammerInfo.headBullet = m_headBullet;
}

void CHammerBulletObject::Tick(double deltime)
{
	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	xmf4_rot = QuaternionMultiply(xmf4_rot, QuaternionRotation(m_Upvector, MMPE_PI * deltime));
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	XMFLOAT4 CenterPos = m_orgpluspos;

	m_tempangle += MMPE_PI / 3 * deltime;
	XMFLOAT4 tempori = QuaternionRotation(XMFLOAT3(0, 1, 0), m_tempangle);

	XMVECTOR to = XMLoadFloat4(&tempori);
	XMVECTOR cp = XMLoadFloat4(&CenterPos);

	cp = XMVector4Transform(cp, XMMatrixRotationQuaternion(to));
	XMStoreFloat4(&CenterPos, cp);
	CenterPos = Float4Add(CenterPos, XMFLOAT4(m_orgPos4f.x, m_orgPos4f.y, m_orgPos4f.z, m_orgPos4f.w));
	
	m_pos4f = { CenterPos.x, CenterPos.y, CenterPos.z, CenterPos.w };
	UpdatePhysicsCenterPos();

	m_lifetime -= deltime;
	m_orgpluspos.z += deltime * 7.0;

	if (m_lifetime <= 0)
		m_alive = false;
	
	m_stc_hammerInfo.pos4f = m_pos4f;
	m_stc_hammerInfo.rot4f = m_rot4f;
	m_stc_hammerInfo.opp_pos4f = { m_orgpluspos.x, m_orgpluspos.y, m_orgpluspos.z, m_orgpluspos.w };

	m_stc_hammerInfo.alive = m_alive;
	m_stc_hammerInfo.create_first = m_createfirst;
}

void CHammerBulletObject::AfterGravitySystem()
{
	if (m_pos4f.y <= 0 && m_alive == true)
	{
		m_alive = false;
		m_stc_hammerInfo.alive = false;
	}

}

void CHammerBulletObject::Collision(vector<CPlayerObject*>* clients, double deltime)
{
	for (auto iter = clients->begin(); iter != clients->end(); ++iter)
	{
		//Ŭ���̾�Ʈ ���̵� == �ҷ��� ���ξ��̵� -> �浹x
		if ((*iter)->GetID() == m_masterID) continue;

		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{

				//player �� �ҷ��� �ε����� ��� ������ ��ƼŬ�� �������
				m_after_collision = AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM;
				m_stc_hammerInfo.after_coll = m_after_collision;

				(*iter)->GetDamaged(m_ability.attack);

				//�ش� �ҷ��� ���� �÷��̾ ���� �����ϰ�� // ������ 1 ��ƼŬ�� �ߵ��� Ŭ�󿡺����ߵ�
				if ((*iter)->GetShieldState())
					m_stc_hammerInfo.damage = 1;
				else
					m_stc_hammerInfo.damage = m_ability.attack;

				//������ ��ü�� �ƴϸ�
				// �ҷ��� ���� ��밡 �׾��� ���, �� ��밡 �÷��̾��� ��� ������ �ø���
				if ((*iter)->GetMyCurrHP() <= 0)
				{
					if (g_clients[this->GetBulletMasterID()]->GetAlive())
					{
						if ((*iter)->GetIsTopRanker()) 
							g_clients[this->GetBulletMasterID()]->SetPlayerScore(TOP_RANKER_SCORE);
						else
							g_clients[this->GetBulletMasterID()]->SetPlayerScore(NORMAL_PLAYER_SCORE);
					}
				}

				m_alive = false;
				m_bulldata.alive = false;

				//�ظӴ� ���̳��Ϳ�����Ʈ�� �浹�� ������ȴ�.
				if (m_numOfHammer > 0)
				{
					auto nPos4f = Float4Add(XMFLOAT4(m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w), XMFLOAT4(15, 0, 15, 0));

					g_bullets.push_back(move(new CHammerBulletObject(m_masterID, ++m_id, { nPos4f.x, nPos4f.y, nPos4f.z, nPos4f.w }, { 0,0,0,1 }, m_numOfHammer - 1, XMFLOAT4(0, 0, 35, 0))));

					m_numOfHammer = 0;
				}

				
			}
		}
	}
}

void CHammerBulletObject::Collision(vector<CNpcObject*>* npcs, double deltime)
{
	for (auto iter = npcs->begin(); iter != npcs->end(); ++iter)
	{
		//�ҷ�(�÷��̾� �ҷ�, npc �ҷ�)�� �׵��� ����(npc)�� �浹x
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				//NPC �� �ҷ��� �ε����� ��� ������ ��ƼŬ�� �������
				m_after_collision = AFTER_COLLISION_EFFECT::DAMAGE_AND_BOOM;
				m_stc_hammerInfo.after_coll = m_after_collision;

				(*iter)->GetDamaged(m_ability.attack);

				//npc ������ ��� -> �Ѿ� ������ �������Ѵ�
				if ((*iter)->GetMyBasicPacketData().monster_type == IMP)
					(*iter)->fsm->aidata.LastPosition = this->GetCenterPos4f();


				//�÷��̾ �� �ҷ��� �°� ����(IMP)������ ü���� 0���ϰ� �Ǿ� �׾���
				//�ش� �ҷ��� ����ID�� ���� �÷��̾ ����ִٸ� ������ �ø���
				//����(IMP) ���ھ�� 100
				if ((*iter)->GetMyCurrHP() <= 0)
				{
					if (g_clients[this->GetBulletMasterID()]->GetAlive())
					{
						g_clients[this->GetBulletMasterID()]->SetPlayerScore(MONSTER_IMP_SCORE);
					}
				}

				//�ظӴ� ���̳��Ϳ�����Ʈ�� �浹�� ������ȴ�.
				if (m_numOfHammer > 0)
				{
					auto nPos4f = Float4Add(XMFLOAT4(m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w), XMFLOAT4(15, 0, 15, 0));

					CBulletObject * bullet = new CHammerBulletObject(m_masterID, ++m_id, { nPos4f.x, nPos4f.y, nPos4f.z, nPos4f.w }, { 0,0,0,1 }, m_numOfHammer - 1, XMFLOAT4(0, 0, 35, 0));
					
					if (dynamic_cast<CHammerBulletObject*>(bullet))
					{
						dynamic_cast<CHammerBulletObject*>(bullet)->m_headBullet = false;
						dynamic_cast<CHammerBulletObject*>(bullet)->m_stc_hammerInfo.headBullet = false;
					}
					else { cout << "In CHammerBulletObject::Collision Error: Hammer Bullet dynamic_cast Error\n"; }

					cout << "Hammer ID: " << bullet->GetID() << " , " << "is HeadBullet ?? " << dynamic_cast<CHammerBulletObject*>(bullet)->m_headBullet << "\n";

					g_bullets.emplace_back(bullet);
					m_numOfHammer = 0;
				}

				m_alive = false;
				m_bulldata.alive = false;
			}
		}
	}
}

void CHammerBulletObject::Collision(unordered_set<CStaticObject*>* sobjs, double deltime)
{
	for (auto iter = sobjs->begin(); iter != sobjs->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				//StaticObject �� �ҷ��� �ε����� ��� ������ ��ƼŬ�� ����� ���ƾ���
				m_after_collision = AFTER_COLLISION_EFFECT::BOOM;
				m_stc_hammerInfo.after_coll = m_after_collision;

				m_alive = false;
				m_bulldata.alive = false;
			}
		}
	}
}




CStoneBulletObject::CStoneBulletObject(CNpcObject *master, const XMFLOAT4 & in_pos4f, const XMFLOAT4 & in_rot4f,  XMFLOAT4& ori, const XMFLOAT4& opp)
{
	m_createfirst = true;

	m_npc_master = master;
	m_masterID = master->GetID();

	m_type = protocol_NpcStoneBullet;
	m_after_collision = AFTER_COLLISION_EFFECT::EMPTY;

	//�������� npc�� ������ �ҷ��� ���� -> �������� ���ΰ��� -> ��Ŷ�ִ� id������ 0xffff(65535)�� ������ 
	//id ���� ��� ����Ѱ� -> id�� 0���� ����� ó������ id�� ����
	if (g_npc_bulletID >= 0xffff )
		g_npc_bulletID = -1;

	m_npc_bulletID = ++g_npc_bulletID;

	m_orgpluspos = opp;
	m_tempangle = 0.0;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	xmf4_rot = { in_rot4f.x, in_rot4f.y, in_rot4f.z, in_rot4f.w };
	xmf4_rot = QuaternionMultiply(xmf4_rot, ori);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();

	m_ability.orignHP = 1;
	m_ability.curHP = 1;
	m_ability.attack = 40;
	m_ability.speed = 0;
	m_godmode = false;
	m_lifetime = 10;

	pp = new PhysicsPoint();

	xmf4_pos = { in_pos4f.x, in_pos4f.y, in_pos4f.z, in_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(1, 1, 1);
	pp->SetDamping(1);
	pp->SetBounce(false);
	pp->SetVelocity(0, 0, 0);
	pp->SetMass(0.35f);

	UpdatePPosCenterPos();

	//��Ŷ���� ���� CStornBullet ������ �ʱ�ȭ
	UpdateDataForPacket();
}

/*
NPC_BulletObject_Info CBulletObject::GetChangedNPCBulletState() const
{
	//������� ����. �⺻���� �ʱ�ȭ. ��� ����� STONEBULLET���� ����ϱ� ���� ��Ӻθ��� BULLET�� �׳� ������
	NPC_BulletObject_Info stc_imp_bullet;
	stc_imp_bullet.alive = false;
	stc_imp_bullet.master_id = 0;
	stc_imp_bullet.my_id = 0;
	stc_imp_bullet.pos4f = { 0.f,0.f,0.f,0.f };
	stc_imp_bullet.rot4f = { 0.f,0.f,0.f,1.0f };
	stc_imp_bullet.create_first = true;
	stc_imp_bullet.after_coll = AFTER_COLLISION_EFFECT::EMPTY;
	stc_imp_bullet.damage = m_ability.attack;

	return NPC_BulletObject_Info(stc_imp_bullet);
}
*/

NPC_BulletObject_Info CStoneBulletObject::GetChangedNPCBulletState() const
{
	NPC_BulletObject_Info stc_imp_bullet;
	stc_imp_bullet.alive = m_alive;
	stc_imp_bullet.master_id = m_masterID;
	stc_imp_bullet.my_id = m_npc_bulletID;
	stc_imp_bullet.pos4f = m_pos4f;
	stc_imp_bullet.rot4f = m_rot4f;
	stc_imp_bullet.create_first = m_createfirst;
	stc_imp_bullet.after_coll = m_after_collision;
	stc_imp_bullet.damage = m_npc_bulldata.damage;

	return NPC_BulletObject_Info(stc_imp_bullet);
}

void CStoneBulletObject::AfterGravitySystem()
{
	if (m_pos4f.y <= 0 && m_alive == true)
	{
		m_alive = false;
		m_npc_bulldata.alive = false;
	}
}

void CStoneBulletObject::UpdateDataForPacket()
{
	m_npc_bulldata.alive = m_alive;
	m_npc_bulldata.master_id = m_masterID;
	m_npc_bulldata.my_id = m_npc_bulletID;
	m_npc_bulldata.pos4f = m_pos4f;
	m_npc_bulldata.rot4f = m_rot4f;
	m_npc_bulldata.after_coll = m_after_collision;
}

void CStoneBulletObject::Tick(double deltime)
{
	//UpdateLookvector();
	//UpdateUpvector();

	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	xmf4_rot = QuaternionMultiply(xmf4_rot, QuaternionRotation(m_Upvector, MMPE_PI * deltime));

	xmf4_pos = m_orgpluspos;

	m_tempangle += MMPE_PI / 4 * deltime;
	XMFLOAT4 tempori = QuaternionRotation(XMFLOAT3(0, 1, 0), m_tempangle);

	XMVECTOR to = XMLoadFloat4(&tempori);
	XMVECTOR cp = XMLoadFloat4(&xmf4_pos);

	cp = XMVector4Transform(cp, XMMatrixRotationQuaternion(to));
	XMStoreFloat4(&xmf4_pos, cp);
	xmf4_pos = Float4Add(xmf4_pos, m_npc_master->GetCenterPos4f());

	m_pos4f = { xmf4_pos.x, xmf4_pos.y, xmf4_pos.z, xmf4_pos.w };
	UpdateLookvector();

	m_lifetime -= deltime;

	if (m_lifetime <= 0)
		m_alive = false;
	
}

void CStoneBulletObject::Collision(vector<CPlayerObject*>* clients, double deltime)
{
	for (auto iter = clients->begin(); iter != clients->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				//Player �� ������ ����ҷ��� �ε����� ��� ������ ��ƼŬ�� �������
				m_after_collision = AFTER_COLLISION_EFFECT::DAMAGE;
				m_npc_bulldata.after_coll = m_after_collision;

				(*iter)->GetDamaged(m_ability.attack);

				if ((*iter)->GetShieldState())
					m_npc_bulldata.damage = 1;
				else
					m_npc_bulldata.damage = m_ability.attack;

				if ((*iter)->GetMyCurrHP() <= 0)
				{
					(*iter)->SetMyCharacterDeathCount();
					(*iter)->CalculatePlayerScoreForRanking();
				}


				m_alive = false;
			}
		}
	}
}

void CStoneBulletObject::Collision(vector<CNpcObject*>* npcs, double deltime)
{
	//�ϴ� �� ����
	return;
}

void CStoneBulletObject::Collision(unordered_set<CStaticObject*>* sobjs, double deltime)
{
	for (auto iter = sobjs->begin(); iter != sobjs->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{

			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				//StaticObject �� ������ ����ҷ��� �ε����� ��� ������ ��ƼŬ�� ����� ���ƾ���
				m_after_collision = AFTER_COLLISION_EFFECT::EMPTY;
				m_npc_bulldata.after_coll = m_after_collision;

				m_alive = false;
			
			}
			
		}
	}
}


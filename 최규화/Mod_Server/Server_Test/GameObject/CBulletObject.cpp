#include "../stdafx.h"

int CStoneBulletObject::g_npc_bulletID = -1;

CBulletObject::CBulletObject(const unsigned short & master_id, const unsigned short & my_id, const Position & pos, const Rotation & rot, float bulltime, Vel3f & vel, const unsigned char & type, const Position3D & endpt, const float& degree)
{

	pp = new PhysicsPoint();

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	m_pos4f = pos;
	m_rot4f = rot;

	m_masterID = master_id;
	m_id = my_id;

	m_lifetime = bulltime;
	m_vel3f = vel;

	m_type = type;
	m_alive = true;

	m_savept = endpt;

	m_degree = degree;

	m_createfirst = true;

	/*
	if (m_type == protocol_DiceBullet)
	{
		XMFLOAT3 l{ 0,0,1 };
		XMVECTOR ol = XMLoadFloat3(&l);
		XMVECTOR nl = XMLoadFloat3(&g_clients[m_masterID]->GetLookVector());

		auto axis = XMVector3Cross(ol, nl);
		//방향축을 완성.
		axis = XMVector3Normalize(axis);
		XMFLOAT3 Axis;
		XMStoreFloat3(&Axis, axis);
		//이제 회전각도를 구해야한다. 내적을 통해 회전각도를 구한다.

		auto temp = XMVector3Dot(ol, nl);

		float d;//기존 룩벡터와 새로운 룩벡터를 내적한 결과.
		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
			d = acos(d);//각도 완성. 라디안임

		auto ori = QuaternionRotation(Axis, d);
		auto wmatrix = XMMatrixIdentity();
		auto quater = XMLoadFloat4(&ori);
		wmatrix *= XMMatrixRotationQuaternion(quater);

		auto orr = XMVectorSet(1, 0, 0, 0);
		orr = XMVector4Transform(orr, wmatrix);//가짜 라이트 벡터
		orr = XMVector3Normalize(orr);
		auto RealRight = XMVector3Cross(axis, nl);//진짜 라이트벡터
		RealRight = XMVector3Normalize(RealRight);

		//진짜 라이트 벡터와 가짜 라이트 벡터를 내적함.
		temp = XMVector3Dot(RealRight, orr);

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
			d = acos(d);//각도 완성. 라디안임
		auto ori2 = XMQuaternionRotationAxis(nl, d);//진짜 룩벡터를 회전축으로 삼고 진짜라이트와 가짜라이트의 사잇각만큼회전

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//최종 회전 방향
		
		XMFLOAT4 Orient{ 0,0,0,1 };
		Orient = QuaternionMultiply(Orient, ori);
		XMFLOAT3 axis2{ 0,1,0 };
		auto q2 = QuaternionRotation(axis2, m_degree);
		Orient = QuaternionMultiply(Orient, q2);
		
		m_rot4f = { Orient.x, Orient.y, Orient.z, Orient.w };

	}
	*/

	UpdateLookvector();			//degree를 이용해 변경된 회전값을 클라이언트로부터 건네받았음. 이를 이용해 lookvector, rightvector, upvector 업데이트
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
		m_ability.attack = 10;
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
	
	UpdateDataForPacket();

	//--------------------------------------------------------------------------------------------

	/*
	m_bulldata.alive = m_alive;				
	m_bulldata.endpoint = m_savept;				//고정
	m_bulldata.master_id = m_masterID;			//고정
	m_bulldata.my_id = m_id;						//고정
	m_bulldata.pos4f = m_pos4f;
	m_bulldata.rot4f = m_rot4f;			
	m_bulldata.type = m_type;					//고정
	m_bulldata.vel3f = m_vel3f;					//고정
	m_bulldata.degree = m_degree;				//한번 초기화되면 안 변함 // 고정
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
	
	UpdatePPosCenterPos();	//integrate를 거쳐 변경된 pp->Centerpos를 이용한 불렛 클래스가 관리하는 불렛 위치인 m_pos4f 업데이트

	UpdateLookvector();		//Lookvector와 기존의 회전값인 xmf4_rot를 이용한 QuterninMultiply에 의해 변경된 xmf4_rot, 즉 불렛회전값을 이용해 lookvector, rightvector
	UpdateUpvector();		//upvector 업데이트

	m_bulldata.pos4f = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	m_bulldata.rot4f = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	m_bulldata.alive = m_alive;

	//cout << "RotX: " << m_rot4f.x << "," << "RotY: " << m_rot4f.y << "," << "RotZ: " << m_rot4f.z << "," << "RotW: " << m_rot4f.w << "\n";

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//플레이어 공격 - 라이트 불렛, 헤비 불렛, 다이스스트라이크
//npc몬스터 공격 - 스톤 불렛
//이들을 CBulletObject에서 관리

//플레이어가 발사하는 불렛타입 스킬들과 플레이어가 충돌할 경우
void CBulletObject::Collision(vector<CPlayerObject*>* clients, double deltime)
{
	for (auto iter = clients->begin(); iter != clients->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr && (*iter)->GetID() != m_masterID)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				(*iter)->GetDamaged(m_ability.attack);

				//고정된 물체가 아니면
				// 불렛을 맞은 상대가 죽었을 경우, 그 상대가 플레이어일 경우 점수를 올린다
				if ((*iter)->GetMyCurrHP() <= 0)
				{
					if (g_clients[this->GetBulletMasterID()]->GetAlive())
					{
						g_clients[this->GetBulletMasterID()]->SetPlayerScore(NORMAL_PLAYER_SCORE);
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

//플레이어가 발사하는 불렛타입 스킬들과 npc 충돌할 경우
void CBulletObject::Collision(vector<CNpcObject*>* npcs, double deltime)
{
	for (auto iter = npcs->begin(); iter != npcs->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				(*iter)->GetDamaged(m_ability.attack);

				//npc 몬스터인 경우 -> 총알 방향을 보도록한다
				if ((*iter)->GetMyBasicPacketData().monster_type == IMP)
				{
					(*iter)->fsm->aidata.LastPosition = this->GetCenterPos4f();
				}

				//플레이어가 쏜 불렛에 맞고 임프(IMP)몬스터의 체력이 0이하가 되어 죽었음
				//해당 불렛의 주인ID를 가진 플레이어가 살아있다면 점수를 올린다
				//임프(IMP) 스코어는 100
				if ((*iter)->GetMyCurrHP() <= 0)
				{
					if (g_clients[this->GetBulletMasterID()]->GetAlive())
					{
						g_clients[this->GetBulletMasterID()]->SetPlayerScore(MONSTER_IMP_SCORE);
					}
				}

				//고정된 물체가 아니면
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
				//고정된 물체면
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


CStoneBulletObject::CStoneBulletObject(CNpcObject *master, const XMFLOAT4 & in_pos4f, const XMFLOAT4 & in_rot4f,  XMFLOAT4& ori, const XMFLOAT4& opp)
{
	m_createfirst = true;

	m_npc_master = master;
	m_masterID = master->GetID();

	m_type = protocol_NpcStoneBullet;

	//서버에서 npc가 생성한 불렛을 생성 -> 서버에서 따로관리 -> 패킷최대 id범위인 0xffff(65535)를 넘으면 
	//id 값을 모두 사용한것 -> id를 0으로 만들어 처음부터 id값 재사용
	if (g_npc_bulletID >= 0xffff )
		g_npc_bulletID = -1;

	m_npc_bulletID = ++g_npc_bulletID;

	m_orgpluspos = opp;

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
	m_deltime = 10;
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

	//패킷으로 보낼 CStornBullet 데이터 초기화
	UpdateDataForPacket();
}

NPC_BulletObject_Info CBulletObject::GetChangedNPCBulletState() const
{
	NPC_BulletObject_Info stc_imp_bullet;
	stc_imp_bullet.alive = false;
	stc_imp_bullet.master_id = 0;
	stc_imp_bullet.my_id = 0;
	stc_imp_bullet.pos4f = { 0.f,0.f,0.f,0.f };
	stc_imp_bullet.rot4f = { 0.f,0.f,0.f,1.0f };
	stc_imp_bullet.create_first = true;

	return NPC_BulletObject_Info(stc_imp_bullet);
}

NPC_BulletObject_Info CStoneBulletObject::GetChangedNPCBulletState() const
{
	NPC_BulletObject_Info stc_imp_bullet;
	stc_imp_bullet.alive = m_alive;
	stc_imp_bullet.master_id = m_masterID;
	stc_imp_bullet.my_id = m_npc_bulletID;
	stc_imp_bullet.pos4f = m_pos4f;
	stc_imp_bullet.rot4f = m_rot4f;
	stc_imp_bullet.create_first = m_createfirst;

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
}

void CStoneBulletObject::Tick(double deltime)
{
	UpdateLookvector();
	UpdateUpvector();

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

	m_lifetime -= deltime;

	if (m_lifetime <= 0)
		m_alive = true;
	


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
				/*
				if ((*iter)->GetShieldState())
					m_ability.attack = 1;
				else
					m_ability.attack = 40;

				(*iter)->ToDamage(m_ability.attack);
				*/
				(*iter)->GetDamaged(m_ability.attack);

				m_alive = true;
			}
		}
	}
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
				m_alive = true;
			
		}
	}
}

#include "BulletObject.h"

BulletObject::BulletObject(const unsigned short& master_id, const unsigned short& my_id,
						   const Position& pos, const Rotation& rot, double bulltime,
						   Vel3f& vel, const unsigned char& type, const Position3D& endpt)
{
	pe = new PhysicalEffect();

	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	m_bulldata.pos = move(pos);
	m_bulldata.Rotate_status = move(rot);

	pe->UpdateLookVector(OffLookvector, OffRightvector, m_bulldata.Rotate_status, Lookvector, Rightvector);
	pe->GetUpVector(Lookvector, Rightvector, Upvector);

	m_bulldata.Master_ID = master_id;
	m_bulldata.myID = my_id;

	lifetime = bulltime;	
	m_bulldata.vel3f = move(vel);

	m_type = type;
	m_bulldata.type = type;

	m_bulldata.endpoint = endpt;

	m_bulldata.alive = true;

	//myID = my_id;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	pp = new PhysicsPoint();

	pp->SetPosition(m_bulldata.pos.x, m_bulldata.pos.y, m_bulldata.pos.z);

	XMFLOAT3 xmf3;
	xmf3.x = vel.x; xmf3.y = vel.y; xmf3.z = vel.z;
	pp->SetVelocity(xmf3);
	pp->SetHalfBox(1, 1, 1);
	pp->SetDamping(1);
	pp->SetBounce(false);
	pp->SetVelocity(Lookvector.x * Speed, Lookvector.y * Speed, Lookvector.z * Speed);

}

void BulletObject::AfterGravitySystem()
{
	if (m_bulldata.pos.y <= 0)
		m_bulldata.alive = false;
}

void BulletObject::Update(float deltatime)
{
	
}

void BulletObject::Collision_StaticObjects(unordered_set<StaticObject*>& sobjs, float DeltaTime)
{
	for (auto sobj : sobjs)
	{
		if (sobj->GetPhysicsPoint() != nullptr)
		{
			bool test = pp->CollisionTest(*sobj->GetPhysicsPoint(),
										  Lookvector, Rightvector, Upvector,
										  sobj->GetLookVector(), sobj->GetRightVector(), sobj->GetUpVector());
			if (test)
			{
				XMFLOAT3 cn;
				cn = pp->pAxis;
				pp->ResolveVelocity(*sobj->GetPhysicsPoint(), cn, DeltaTime);
				
				m_bulldata.alive = false;
			}
		}
	}
}

void BulletObject::Collision_Players(vector<Player_Session*>& clients, float DeltaTime)
{
	for (auto client : clients)
	{
		if (client->GetPhysicsPoint() != nullptr && client->Get_ID() != m_bulldata.Master_ID )
		{
			bool test = pp->CollisionTest(*client->GetPhysicsPoint(),
										  Lookvector, Rightvector, Upvector,
										  client->GetLookVector(), client->GetRightVecotr(), client->GetUpVector());
			if (test)
			{
				client->Damaged(Damage);

				XMFLOAT3 cn;

				cn = Float3Add(pp->GetPosition(), client->GetPhysicsPoint()->GetPosition(), false);
				cn = Float3Normalize(cn);

				//파티클 리스트에 데미지 오브젝트를 생성해서 넣음. 파티클을 띄운다.
				//if (ParticleList != nullptr)


				pp->ResolveVelocity(*client->GetPhysicsPoint(), cn, DeltaTime);
				m_bulldata.alive = false;
			}
			
		}
	}
}

BulletObject::~BulletObject()
{
}

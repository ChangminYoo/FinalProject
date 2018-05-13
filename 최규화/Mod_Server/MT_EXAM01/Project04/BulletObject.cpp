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
	if (m_bulldata.alive)
	{
		for (auto sobj : sobjs)
		{
			if (sobj->GetPhysicsPoint() != nullptr)
			{
				m_lock.lock();

				bool test = pp->CollisionTest(*sobj->GetPhysicsPoint(),
					Lookvector, Rightvector, Upvector,
					sobj->GetLookVector(), sobj->GetRightVector(), sobj->GetUpVector());
				
				m_lock.unlock();

				if (test)
				{
					XMFLOAT3 cn;

					m_lock.lock();
					cn = pp->pAxis;
					pp->ResolveVelocity(*sobj->GetPhysicsPoint(), cn, DeltaTime);

					m_bulldata.alive = false;

					m_lock.unlock();
				}
			}
		}
	}
}

void BulletObject::Collision_Players(vector<Player_Session*>& clients, float DeltaTime)
{
	// 지금의 Bullet이 살아있는 상태일때만 충돌검사
	//m_lock.lock();
	
	m_lock.lock();
	if (m_bulldata.alive)
	{
		m_lock.unlock();
		for (auto client : clients)
		{
			client->PlayerLock();
			m_lock.lock();
			if (client->GetPhysicsPoint() != nullptr && client->Get_ID() != m_bulldata.Master_ID)
			{
				//충돌 유무를 파악 = 지역변수에 저장 
				bool test = pp->CollisionTest(*client->GetPhysicsPoint(),
					Lookvector, Rightvector, Upvector,
					client->GetLookVector(), client->GetRightVecotr(), client->GetUpVector());

				m_lock.unlock();
				client->PlayerUnLock();

				//충돌이 일어났을 때만 일어남
				if (test)
				{
					XMFLOAT3 cn;

					m_lock.lock();
					client->PlayerLock();

					cn = Float3Add(pp->GetPosition(), client->GetPhysicsPoint()->GetPosition(), false);
					cn = Float3Normalize(cn);

					//client->PlayerUnLock();
					//m_lock.unlock();
					//파티클 리스트에 데미지 오브젝트를 생성해서 넣음. 파티클을 띄운다.
					//if (ParticleList != nullptr)
					//client->PlayerLock();
					//m_lock.lock();

					pp->ResolveVelocity(*client->GetPhysicsPoint(), cn, DeltaTime);

					client->PlayerUnLock();
					
					m_bulldata.alive = false;

					m_lock.unlock();

					//m_lock.lock();
					//m_bulldata.alive = false;
					//m_lock.unlock();

					client->PlayerLock();
					client->Damaged(Damage);
					cout << "Client ID " << client->GetPlayerData().ID << "was Damaged";
					cout << "HP: " << client->GetPlayerData().UserInfo.cur_hp << endl;
					client->PlayerUnLock();
				}
			}
			else
			{
				m_lock.unlock();
				client->PlayerUnLock();
			}
		}
	}
	else
	{
		m_lock.unlock();
		cout << "불렛 시간이 지나 죽음 " << endl;

		//m_lock.lock();
		//auto remove_data = remove(Player_Session::m_bullobjs.begin(), Player_Session::m_bullobjs.end(), this);
		///Player_Session::m_bullobjs.erase(remove_data);
		//m_lock.unlock();

	}
}

void BulletObject::SetBulletRotatevalue(const XMFLOAT4 & xmf4)
{
	m_bulldata.Rotate_status.x = xmf4.x;
	m_bulldata.Rotate_status.y = xmf4.y;
	m_bulldata.Rotate_status.z = xmf4.z;
	m_bulldata.Rotate_status.w = xmf4.w;
}

BulletObject::~BulletObject()
{
	delete rb;
	delete pp;
	delete pe;
}

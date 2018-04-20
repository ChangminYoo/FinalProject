#include "BulletObject.h"

BulletObject::BulletObject(const unsigned short& master_id, const unsigned short& target_id,
						   const Position& pos, const Rotation& rot, float bulltime, const unsigned short& my_id)
{
	pe = new PhysicalEffect();

	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	//XMFLOAT4 xmf4 = { m_bulldata.Rotate_status.x, m_bulldata.Rotate_status.y, m_bulldata.Rotate_status.z, m_bulldata.Rotate_status.w };
	//m_bulldata.Rotate_status = QuaternionMultiply(xmf4, )

	m_bulldata.Rotate_status = move(rot);

	pe->UpdateLookVector(OffLookvector, OffRightvector, m_bulldata.Rotate_status, Lookvector, Rightvector);
	pe->GetUpVector(Lookvector, Rightvector, Upvector);

	lifetime = bulltime;

	m_bulldata.pos = move(pos);
	
	m_bulldata.Master_ID = master_id;
	m_bulldata.LookOn_ID = target_id;
	
	myID = my_id;

	//광선충돌 검사용 육면체
	XMFLOAT3 rx(1, 0, 0);
	XMFLOAT3 ry(0, 1, 0);
	XMFLOAT3 rz(0, 0, 1);
	rco.SetPlane(rx, ry, rz);

	pp = new PhysicsPoint();

	pp->SetPosition(m_bulldata.pos.x, m_bulldata.pos.y, m_bulldata.pos.z);
	pp->SetHalfBox(1, 1, 1);
	pp->SetDamping(1);
	pp->SetBounce(false);
	pp->SetVelocity(Lookvector.x * Speed, Lookvector.y * Speed, Lookvector.z * Speed);

}

void BulletObject::UpdateLookVector()
{
	auto wmatrix = XMMatrixIdentity();

	//클라이언트에서 MouseMove를 통해 카메라를 회전할 때 마다 Rotate_status가 달라짐
	XMFLOAT4 orient_xmfloat4 =
	{	m_bulldata.Rotate_status.x ,
		m_bulldata.Rotate_status.y ,
		m_bulldata.Rotate_status.z ,
		m_bulldata.Rotate_status.w };

	auto quater = XMLoadFloat4(&orient_xmfloat4);
	wmatrix *= XMMatrixRotationQuaternion(quater);

	//OffLookvector 와 OffRightvector는 플레이어타입(캐릭터, 불렛, 스테틱오브젝트 등에 따라 다름)
	auto ol = XMLoadFloat3(&OffLookvector);
	auto or = XMLoadFloat3(&OffRightvector);

	ol = XMVector4Transform(ol, wmatrix);
	or = XMVector4Transform(or , wmatrix);

	XMStoreFloat3(&Lookvector, ol);
	XMStoreFloat3(&Rightvector, or );

	if (fabsf(Lookvector.x) < MMPE_EPSILON / 10)
		Lookvector.x = 0;
	if (fabsf(Lookvector.y) < MMPE_EPSILON / 10)
		Lookvector.y = 0;
	if (fabsf(Lookvector.z) < MMPE_EPSILON / 10)
		Lookvector.z = 0;


	if (fabsf(Rightvector.x) < MMPE_EPSILON / 10)
		Rightvector.x = 0;
	if (fabsf(Rightvector.y) < MMPE_EPSILON / 10)
		Rightvector.y = 0;
	if (fabsf(Rightvector.z) < MMPE_EPSILON / 10)
		Rightvector.z = 0;

	Lookvector = Float3Normalize(Lookvector);
	Rightvector = Float3Normalize(Rightvector);

}

void BulletObject::GetUpVector()
{
	XMVECTOR l = XMLoadFloat3(&Lookvector);
	XMVECTOR r = XMLoadFloat3(&Rightvector);
	auto u = XMVector3Cross(l, r);

	XMFLOAT3 up;
	XMStoreFloat3(&up, u);
	up = Float3Normalize(up);

	Upvector = move(up);
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
				
				m_delobj = true;
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
				m_delobj = true;
			}
			
		}
	}
}

BulletObject::~BulletObject()
{
}

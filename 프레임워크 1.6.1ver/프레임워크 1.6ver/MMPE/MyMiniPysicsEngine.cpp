
#include "MyMiniPysicsEngine.h"
#include<math.h>



PhysicsPoint::PhysicsPoint()
{
	 InverseMass = 1;
	Velocity = XMFLOAT3(0, 0, 0);
	CenterPos = XMFLOAT3(0, 0, 0);
	damping = 0.99;
	TotalForce = XMFLOAT3(0, 0, 0);
	Accel = XMFLOAT3(0, 0, 0);
}


PhysicsPoint::~PhysicsPoint()
{
}

void PhysicsPoint::integrate(float DeltaTime, XMFLOAT4* ObjPos, XMFLOAT3* ObjVel)
{

	if (InverseMass <= 0.0f)
		return;

	assert(DeltaTime > 0.0);

	XMVECTOR centerpos = XMLoadFloat3(&CenterPos);
	XMVECTOR velocity = XMLoadFloat3(&Velocity);
	XMVECTOR accel = XMLoadFloat3(&Accel);
	XMVECTOR totalforce = XMLoadFloat3(&TotalForce);
	//먼저 힘과 질량으로 가속도를 계산한다.

	//가속도 = 최종힘 * 질량의 역수. 가속도는 힘을가했을때만 생기는 것. F가 0이면 a는 0
	//따라서 힘을 한번만 가했으면 가속도는 한번만 0이아니고 이후는 계속 0이다.
	//다만 속도는 가속도로 인해 계산된 결과를 계속 유지하며 댐핑지수로 인해 0 에 가까워짐.
	accel =  totalforce * InverseMass;


	//속도와 위치는 갱신되야한다.

	//속도와 가속도를 적분해 중점을 구한다.
	centerpos = centerpos + velocity * DeltaTime + 0.5*accel*DeltaTime*DeltaTime;
	
	
	//가속도를 통해 속도를 추가한다.
	velocity = velocity + accel * DeltaTime;

	// 댐핑지수를 통해 감속한다.
	velocity *= powf(damping, DeltaTime);
	float e = 0;
	XMStoreFloat(&e, XMVector3Length(velocity));
	if (e <= MMPE_EPSILON)
		velocity = XMVectorZero();
	//결과 물을 저장한다.
	XMStoreFloat3(&CenterPos, centerpos);
	XMStoreFloat3(&Velocity, velocity);
	XMStoreFloat3(&Accel, accel);
	
	//모든 계산이 끝났으면 힘을 초기화 한다.
	ForceClear();


	//계산된 결과의 중점과 속도 정보를 PhysicsPoint를 가지고있는 오브젝트가 직접가지고 있어야 한다면

	if (ObjPos != NULL)//만약 게임오브젝트가 따로 가지고 있는 중점 변수가 있으면
		*ObjPos = XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z,ObjPos->w);
	if (ObjVel != NULL)//만약 게임오브젝트가 따로 가지고 있는 속도 변수가 있으면
		*ObjVel = Velocity;
}

void PhysicsPoint::SetMass(float M)
{
	assert(M != 0);
	InverseMass = (1.0) / M;
}

float PhysicsPoint::GetMass(bool Inverse)
{
	if (Inverse)
		return InverseMass;
	else
		return (1.0) / InverseMass;
}

void PhysicsPoint::SetDamping(float D)
{
	damping = D;
}

float PhysicsPoint::GetDamping()
{
	return damping;
}

void PhysicsPoint::SetPosition(XMFLOAT3 & pos)
{
	CenterPos = pos;
}
void PhysicsPoint::SetPosition(XMFLOAT4 & pos)
{
	CenterPos = XMFLOAT3(pos.x,pos.y,pos.z);
}
void PhysicsPoint::SetPosition(float x, float y, float z)
{
	CenterPos = XMFLOAT3(x, y, z);
}

XMFLOAT3 PhysicsPoint::GetPosition()
{
	return CenterPos;
}

void PhysicsPoint::SetVelocity(XMFLOAT3 & V)
{
	Velocity = V;
}

void PhysicsPoint::SetVelocity(float x, float y, float z)
{
	Velocity = XMFLOAT3(x, y, z);
}

XMFLOAT3 PhysicsPoint::GetVelocity()
{
	return Velocity;
}

void PhysicsPoint::SetAccel(XMFLOAT3 & A)
{
	Accel = A;
}

void PhysicsPoint::SetAccel(float x, float y, float z)
{
	Accel = XMFLOAT3(x, y, z);
}

XMFLOAT3 PhysicsPoint::GetAccel()
{
	return Accel;
}

void PhysicsPoint::ForceClear()
{
	TotalForce = XMFLOAT3(0, 0, 0);
}

void PhysicsPoint::AddForce(XMFLOAT3 & F)
{
	XMVECTOR f=XMLoadFloat3(&TotalForce);
	XMVECTOR f2 = XMLoadFloat3(&F);
	f = f + f2;
	XMStoreFloat3(&TotalForce, f);
}

void PhysicsPoint::AddForce(float x, float y, float z)
{

	XMVECTOR f = XMLoadFloat3(&TotalForce);
	XMVECTOR f2 = XMLoadFloat3(&XMFLOAT3(x,y,z));
	f = f + f2;
	XMStoreFloat3(&TotalForce, f);
}

XMFLOAT3 PhysicsPoint::GetTotalForce()
{
	return TotalForce;
}

//레이캐스팅 오브젝트는 광원이 시작되는 카메라 위치와 광선의 방향, 레이캐스트오브젝트를 가진 오브젝트의 중점,카메라를
//보유한 플레이어 오브젝트의 중점, 그리고 교점이 저장될 XMFLOAT3가 입력받아야 한다.
//결과가 반환될때 true면 해당 오브젝트는 광선에 충돌이 되었으며 광선과 충돌할때 가장 가까운 평면의 위치는
//SavePos에 저장된다.

//예를들어 3개의 오브젝트가 RayCasting을 호출해서 3개 모두 충돌했으면, 그중 가장가까운 SavePos를 갖는놈이
//최종적으로 충돌된 녀석일것이다. 이것은 레이캐스트 시스템에서 처리해야한다.

bool MiniPhysicsEngineG9::RayCastObject::RayCasting(XMFLOAT3 & RayOrgin, XMFLOAT3 & RayDir, XMFLOAT3& ObjCenterPos, XMFLOAT3& PlayerPos,XMFLOAT3& Look, XMFLOAT3 * SavePos)
//ObjCenterPos는 해당 검사 대상오브젝트의 중점이고, PlayerPos는 카메라를 보유한 플레이어오브젝트의 위치다.
{
	//우리는 t가 가장 가까운 min값을 가지는 면을 향해 쏠것이므로 필요하다.
	float mint = INFINITY;
	
	

	XMVECTOR objpos = XMLoadFloat3(&ObjCenterPos);
	XMVECTOR playerpos = XMLoadFloat3(&PlayerPos);
	XMVECTOR Rd = XMLoadFloat3(&RayDir);
	XMVECTOR Ro = XMLoadFloat3(&RayOrgin);
	XMVECTOR look = XMLoadFloat3(&Look);
	//먼저 오브젝트가 플레이어 뒤에있으면 검사 대상에서 벗어난다.


	auto frontV=XMVector3Dot(look, XMVector3Normalize(objpos - playerpos));

	float isFront = 0;
	XMStoreFloat(&isFront, frontV);
	if (isFront < 0)
		return false;

	//플레이어가 검사대상이면 6면을 검사한다.

	for (int i = 0; i < 6; i++)//6면을 검사한다.
	{
		XMVECTOR N = XMLoadFloat3(&Plane[i].Normal);
		
		XMFLOAT3 NdotRayDir;
		XMStoreFloat3(&NdotRayDir, XMVector3Dot(N, Rd));
		if (NdotRayDir.x == 0)//광선과 평면의 노멀이 수직이면 교점이 없음(무수히 많거나 없거나)
		{
			continue;
		}

		//여기에 오면 우선 교점은 있다는 소리

		//이를 위해 t를 구해야함. 직선위의 임의의 점은 X = RayOrgin + RayDir*t
		//평면위 의 임의의점은 Plane[i].Normal*(X-(Plane[i].DefaultCenterPos+ObjCenterPos))=0
		//Plane[i].Normal*(RayOrgin+RayDir*t - (Plane[i].DefaultCenterPos+ObjCenterPos))=0
		//Plane[i].Normal*(RayOrgin+RayDir*t)-Plane[i].Normal*(Plane[i].DefaultCenterPos+ObjCenterPos)=0
		//t={((Plane[i].DefaultCenterPos+ObjCenterPos) - RayOrgin)/Plane[i].Normal 도트 RayDir}  도트 Plane[i].Normal
		XMVECTOR defaultpos = XMLoadFloat3(&Plane[i].DefaultCenterPos);
		
		auto sik1 = ((defaultpos + objpos) - Ro) / NdotRayDir.x;
		XMFLOAT3 temp;
		XMStoreFloat3(&temp, XMVector3Dot(sik1, N));

		float t = temp.x;
		
		//이제 t를 이용해 교점X가 유한평면 안에 있는지 검사한다.
		//이후 유한평면안에있으면서 dir와 testP-playerpos를 내적시 0보다 크거나 같으면 
		//mint를 갱신한다.SavePos도 갱신한다. 아니라면 continue

		auto X = Ro + Rd * t;
		//X가 6개의 평면에서 하나라도 0보다 큰게 있으면 바깥에있는것이며 충돌안한것이다.
		bool In = true;
	
		for (int j = 0; j < 6; j++)
		{
			XMVECTOR testN = XMLoadFloat3(&Plane[j].Normal);
			XMVECTOR testP = XMLoadFloat3(&Plane[j].DefaultCenterPos);
			auto testv = XMVector3Dot(testN, (X - (testP + objpos)));
			XMFLOAT3 result;
			XMStoreFloat3(&result, testv);
	
			if (result.x > 0)//0보다 크면 바운딩박스 안에 없다.
			{
				In = false;
				break;
			}			

		}
		//여기까지 오면 t와 SavePos는 유효한 판정을 가지는 교점이다.
		//다만 이 교점이 최종적으로 갱신되기 위해선
		//1. 기존 교점보다 카메라에 가까워야 하고
		//2. 플레이어의 오브젝트 앞에 존재해야한다.
		//즉 플레이어와 카메라가 떨어져있는 3인칭 게임에서 플레이어 몬스터 카메라 순으로 있는 경우
		//플레이어는 앞을 보고 있을테므로, 뒤로 탄이 날아가면 안되기 때문에
		//몬스터가 플레이어 앞에 있을때만 레이캐스팅이 성공해야한다.
		//이를 위해 교점을 구했던 평면(defaultpos+objpos)의 위치에서 플레이어 위치를 뺀 벡터와
		//카메라가 바라보고있는 방향(만약 오차가 심하면 플레이어의 Look으로 해야한다.)과 내적시
		//0이상이고, t가 카메라에 더 가까우면 그 값으로 갱신한다.
		if (In == true)
		{
			float isfront = 0;
			auto ntp=XMVector3Normalize(defaultpos+objpos - playerpos);
			XMStoreFloat(&isfront, XMVector3Dot(look,ntp ));

			if (fabsf(t) < fabsf(mint)&&isfront>=0)
			{
				mint = t;
				XMStoreFloat3(SavePos, X);
			}

		}
		
	}


	if (mint == INFINITY)
		return false;

	return true;
}

void MiniPhysicsEngineG9::RayCastObject::SetPlane(XMFLOAT3 & x, XMFLOAT3 & y, XMFLOAT3 & z)
{
	//0 왼쪽 1 오른쪽 2 앞쪽 3 뒷쪽 4 아래쪽 5 위쪽
	Plane[0].Normal = XMFLOAT3(-1, 0, 0);
	Plane[0].DefaultCenterPos = XMFLOAT3(-x.x,-x.y,-x.z);

	Plane[1].Normal = XMFLOAT3(1, 0, 0);
	Plane[1].DefaultCenterPos = XMFLOAT3(x.x, x.y, x.z);

	Plane[2].Normal = XMFLOAT3(0, 0, -1);
	Plane[2].DefaultCenterPos = XMFLOAT3(-z.x, -z.y, -z.z);

	Plane[3].Normal = XMFLOAT3(0, 0, 1);
	Plane[3].DefaultCenterPos = XMFLOAT3(z.x, z.y, z.z);

	Plane[4].Normal = XMFLOAT3(0, -1, 0);
	Plane[4].DefaultCenterPos = XMFLOAT3(-y.x, -y.y, -y.z);

	Plane[5].Normal = XMFLOAT3(0, 1, 0);
	Plane[5].DefaultCenterPos = XMFLOAT3(y.x, y.y, y.z);


}

void MiniPhysicsEngineG9::SystemGravity::SetGravityAccel(XMFLOAT3 & ga)
{
	GravityAccel = ga;
}

XMFLOAT3 MiniPhysicsEngineG9::SystemGravity::GetGravityAccel()
{
	return GravityAccel;
}

void MiniPhysicsEngineG9::SystemGravity::Update(float DeltaTime, PhysicsPoint & pp)
{
	XMVECTOR ga = XMLoadFloat3(&GravityAccel);
	ga *= pp.GetMass(false);//중력 = 중력가속도 * 질량
	XMFLOAT3 gravityForce;
	XMStoreFloat3(&gravityForce, ga);

	pp.AddForce(gravityForce);
}

float MiniPhysicsEngineG9::FloatLength(XMFLOAT3 & v)
{
	XMVECTOR t;
	t=XMLoadFloat3(&v);
	t=XMVector3Length(t);
	float l = 0;
	XMStoreFloat(&l, t);
	return l;
}

float MiniPhysicsEngineG9::FloatLength(XMFLOAT4 & v)
{
	XMVECTOR t;
	t = XMLoadFloat4(&v);
	t = XMVector4Length(t);
	float l = 0;
	XMStoreFloat(&l, t);
	return l;
}

float MiniPhysicsEngineG9::FloatLength(XMVECTOR & v)
{
	XMVECTOR t;
	
	t = XMVector3Length(v);
	float l = 0;
	XMStoreFloat(&l, t);
	return l;
}

XMFLOAT3 MiniPhysicsEngineG9::Float3Normalize(XMFLOAT3 & v)
{
	XMFLOAT3 t;
	XMStoreFloat3(&t, XMVector3Normalize(XMLoadFloat3(&v)));

	return t;
}

XMFLOAT4 MiniPhysicsEngineG9::Float4Normalize(XMFLOAT4 & v)
{

	XMFLOAT4 t;
	XMStoreFloat4(&t, XMVector4Normalize(XMLoadFloat4(&v)));

	return t;
}

XMFLOAT3 MiniPhysicsEngineG9::XMFloat4to3(XMFLOAT4 & v)
{
	return XMFLOAT3(v.x,v.y,v.z);
}

XMFLOAT4 MiniPhysicsEngineG9::XMFloat3to4(XMFLOAT3 & v)
{
	return XMFLOAT4(v.x,v.y,v.z,0);
}

#include "MyMiniPysicsEngine.h"
#include<math.h>
#include<algorithm>
using namespace std;

//해당 위치에서 해당방향으로 MaxLen에 도달할때의 점을 반환함.
XMFLOAT3 MiniPhysicsEngineG9::RayShot(XMFLOAT3 & RayOrgin, XMFLOAT3 & RayDir, float MaxLen)
{
	XMVECTOR ro = XMLoadFloat3(&RayOrgin);
	XMVECTOR rd = XMLoadFloat3(&RayDir);
	ro += rd * MaxLen;
	XMFLOAT3 result;
	XMStoreFloat3(&result, ro);
	return result;
}

float MiniPhysicsEngineG9::FloatLength(XMFLOAT3 & v)
{
	XMVECTOR t;
	t = XMLoadFloat3(&v);
	t = XMVector3Length(t);
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

XMFLOAT3 MiniPhysicsEngineG9::Float3Add(XMFLOAT3 & v1, XMFLOAT3& v2, bool add)
{
	XMVECTOR a = XMLoadFloat3(&v1);
	XMVECTOR b = XMLoadFloat3(&v2);
	XMFLOAT3 r;
	if (add)
		XMStoreFloat3(&r, a + b);
	else
		XMStoreFloat3(&r, a - b);
	return r;
}

XMFLOAT4 MiniPhysicsEngineG9::Float4Add(XMFLOAT4 & v1, XMFLOAT4& v2, bool add)
{
	XMVECTOR a = XMLoadFloat4(&v1);
	XMVECTOR b = XMLoadFloat4(&v2);
	XMFLOAT4 r;
	if (add)
		XMStoreFloat4(&r, a + b);
	else
		XMStoreFloat4(&r, a - b);
	return r;
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

XMFLOAT3 MiniPhysicsEngineG9::Float3Cross(XMFLOAT3 & v1, XMFLOAT3 & v2)
{
	XMVECTOR c1 = XMLoadFloat3(&v1);
	XMVECTOR c2 = XMLoadFloat3(&v2);

	auto result = XMVector3Cross(c1, c2);
	XMFLOAT3 r;
	XMStoreFloat3(&r, result);
	return r;
}

XMFLOAT4 MiniPhysicsEngineG9::Float4Cross(XMFLOAT4 & v1, XMFLOAT4 & v2)
{
	XMVECTOR c1 = XMLoadFloat4(&v1);
	XMVECTOR c2 = XMLoadFloat4(&v2);

	auto result = XMVector3Cross(c1, c2);
	XMFLOAT4 r;
	XMStoreFloat4(&r, result);
	return r;
}

XMFLOAT3 MiniPhysicsEngineG9::XMFloat4to3(XMFLOAT4 & v)
{
	return XMFLOAT3(v.x, v.y, v.z);
}

XMFLOAT4 MiniPhysicsEngineG9::XMFloat3to4(XMFLOAT3 & v)
{
	return XMFLOAT4(v.x, v.y, v.z, 0);
}

XMFLOAT3 MiniPhysicsEngineG9::Float3Float(XMFLOAT3 & v, float v2)
{
	return XMFLOAT3(v.x*v2, v.y*v2, v.z*v2);
}

XMFLOAT4 MiniPhysicsEngineG9::Float4Float(XMFLOAT4 & v, float v2)
{
	return XMFLOAT4(v.x*v2, v.y*v2, v.z*v2, v.w*v2);
}

//회전축과 각도를 주면 그 축으로 회전하는 쿼터니언을 반환
XMFLOAT4 MiniPhysicsEngineG9::QuaternionRotation(XMFLOAT3 & Axis, float radian)
{


	Axis = Float3Normalize(Axis);
	XMFLOAT4 Result;
	//회전축이없으면 단위쿼터니언을 반환하도록함.
	if (FloatLength(Axis) == 0)
		Result = XMFLOAT4(0, 0, 0, 1);
	else
	{
		auto q = XMQuaternionRotationAxis(XMLoadFloat3(&Axis), radian);
		XMStoreFloat4(&Result, q);
	}
	return Result;
}

XMFLOAT4 MiniPhysicsEngineG9::QuaternionMultiply(XMFLOAT4 & q1, XMFLOAT4 & q2)
{
	auto q = XMQuaternionMultiply(XMLoadFloat4(&q1), XMLoadFloat4(&q2));
	XMFLOAT4 Result;
	q = XMQuaternionNormalize(q);
	XMStoreFloat4(&Result, q);
	return Result;
}





PhysicsPoint::PhysicsPoint()
{
	InverseMass = 1;
	Velocity = XMFLOAT3(0, 0, 0);
	//CenterPos = XMFLOAT3(0, 0, 0);
	damping = 0.99;
	TotalForce = XMFLOAT3(0, 0, 0);
	Accel = XMFLOAT3(0, 0, 0);
	Bounce = false;
}


PhysicsPoint::~PhysicsPoint()
{
}

void PhysicsPoint::integrate(double DeltaTime)
{

	if (InverseMass <= 0.0f)
		return;

	assert(DeltaTime >= 0);

	//XMFLOAT3 temp_objPos;
	//temp_objPos.x = ObjPos->x; temp_objPos.y = ObjPos->y; temp_objPos.z = ObjPos->z;
	//XMVECTOR centerpos = XMLoadFloat3(&temp_objPos);
	XMVECTOR centerpos = XMLoadFloat4(CenterPos);

	//중력에 의해 속도가 너무 빨라질경우를 대비한 if문이다.
	//-40보다 더 늦게 떨어지면 그대로 가되, -40이상의 속도면 -40으로 고정한다.
	//tempV를 이용한 이유는 실제 속도를 -40으로 변경해버리면 댐핑이후에 속도가 이전보다 느려지기 때문이다.
	XMVECTOR velocity = XMLoadFloat3(&Velocity);
	/*if(Velocity.y>-40)
	velocity = XMLoadFloat3(&Velocity);
	else
	{
	XMFLOAT3 tempV = Velocity;
	tempV.y = -40;
	velocity = XMLoadFloat3(&tempV);
	}*/
	XMVECTOR accel = XMLoadFloat3(&Accel);
	XMVECTOR totalforce = XMLoadFloat3(&TotalForce);
	//먼저 힘과 질량으로 가속도를 계산한다.

	//가속도 = 최종힘 * 질량의 역수. 가속도는 힘을가했을때만 생기는 것. F가 0이면 a는 0
	//따라서 힘을 한번만 가했으면 가속도는 한번만 0이아니고 이후는 계속 0이다.
	//다만 속도는 가속도로 인해 계산된 결과를 계속 유지하며 댐핑지수로 인해 0 에 가까워짐.
	accel = totalforce * InverseMass;


	//속도와 위치는 갱신되야한다.

	//속도와 가속도를 적분해 중점을 구한다.

	centerpos = centerpos + velocity * DeltaTime + 0.5*accel*DeltaTime*DeltaTime;

	//속도를 다시 기존속도로 맞춰준다. 즉 tempV.y가 변경되었던거를 기존에 -80보다 빨랐던거로 바꿈
	velocity = XMLoadFloat3(&Velocity);
	//가속도를 통해 속도를 추가한다.
	velocity = velocity + accel * DeltaTime;

	// 댐핑지수를 통해 감속한다.
	velocity *= powf(damping, DeltaTime);
	float e = 0;
	XMStoreFloat(&e, XMVector3Length(velocity));

	if (e <= MMPE_EPSILON)
		velocity = XMVectorZero();
	//결과 물을 저장한다.
	XMStoreFloat4(CenterPos, centerpos);
	XMStoreFloat3(&Velocity, velocity);
	XMStoreFloat3(&Accel, accel);

	//모든 계산이 끝났으면 힘을 초기화 한다.
	ForceClear();


	//계산된 결과의 중점과 속도 정보를 PhysicsPoint를 가지고있는 오브젝트가 직접가지고 있어야 한다면

	//if (ObjPos != NULL)//만약 게임오브젝트가 따로 가지고 있는 중점 변수가 있으면
	//	*ObjPos = XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, ObjPos->w);
	//if (ObjVel != NULL)//만약 게임오브젝트가 따로 가지고 있는 속도 변수가 있으면
	//	*ObjVel = Velocity;
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

float MiniPhysicsEngineG9::PhysicsPoint::GetRad()
{
	return rad;
}

void MiniPhysicsEngineG9::PhysicsPoint::SetBounce(bool bounce)
{
	Bounce = bounce;
}

void PhysicsPoint::SetPosition(XMFLOAT4* pos)
{
	CenterPos = pos;
}

XMFLOAT4 PhysicsPoint::GetPosition()
{
	return *CenterPos;
}

/*
void PhysicsPoint::SetPosition(XMFLOAT4* pos)
{
	CenterPos = XMFLOAT4(pos->x, pos->y, pos->z, pos->w);
}
void PhysicsPoint::SetPosition(float x, float y, float z)
{
	CenterPos = XMFLOAT4(x, y, z, 0);
}

XMFLOAT3 PhysicsPoint::GetPosition()
{
	return CenterPos;
}
*/

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
	XMVECTOR f = XMLoadFloat3(&TotalForce);
	XMVECTOR f2 = XMLoadFloat3(&F);
	f = f + f2;
	XMStoreFloat3(&TotalForce, f);
}

void PhysicsPoint::AddForce(float x, float y, float z)
{

	XMVECTOR f = XMLoadFloat3(&TotalForce);
	XMVECTOR f2 = XMLoadFloat3(&XMFLOAT3(x, y, z));
	f = f + f2;
	XMStoreFloat3(&TotalForce, f);
}

XMFLOAT3 PhysicsPoint::GetTotalForce()
{
	return TotalForce;
}

void MiniPhysicsEngineG9::PhysicsPoint::SetHalfBox(float x, float y, float z)
{
	halfbox.x = x;
	halfbox.y = y;
	halfbox.z = z;

	rad = sqrt(x*x + y * y + z * z);
}

XMFLOAT3 MiniPhysicsEngineG9::PhysicsPoint::GetHalfBox()
{
	return halfbox;
}

bool MiniPhysicsEngineG9::PhysicsPoint::CollisionTest(PhysicsPoint & p2, XMFLOAT3 & l1, XMFLOAT3 & r1, XMFLOAT3 & u1, XMFLOAT3 & l2, XMFLOAT3 & r2, XMFLOAT3 & u2)
{
	//먼저 구 오브젝트로 검사한다.
	float L = FloatLength(Float4Add(*CenterPos, *p2.CenterPos, false));
	if (GetRad() + p2.GetRad() > L)
	{
		//15번 검사를 해야하며, 각 축의 정보는 다음과같다.
		XMFLOAT3 axis[15];
		//1번 상자의 x,y,z축
		axis[0] = r1;
		axis[1] = u1;
		axis[2] = l1;

		//2번 상자의 x,y,z축
		axis[3] = r2;
		axis[4] = u2;
		axis[5] = l2;

		//1번과 2번 각 축과 축의 외적
		axis[6] = Float3Cross(r1, r2);
		axis[7] = Float3Cross(r1, u2);
		axis[8] = Float3Cross(r1, l2);

		axis[9] = Float3Cross(u1, r2);
		axis[10] = Float3Cross(u1, u2);
		axis[11] = Float3Cross(u1, l2);

		axis[12] = Float3Cross(l1, r2);
		axis[13] = Float3Cross(l1, u2);
		axis[14] = Float3Cross(l1, l2);
		XMFLOAT3 CenterL;//중점끼리 연결한 벡터
						 //최소로 겹치는 부분을 찾아야함. 그곳이 겹치는 최소영역의 길이

		float minoverlap = INFINITY;
		//최소겹치는 영역에서 밀어내야할 방향의 인덱스
		int minindex;
		//15번 반복함
		for (int i = 0; i < 15; i++)
		{
			XMFLOAT3 a = axis[i];
			if (FloatLength(a) < 0.001)//외적의 결과가 0이면(부동소수점오차때문에 0에가까우면) 사실상 0이므로 검사X
				continue;

			a = Float3Normalize(a);
			CenterL = XMFLOAT3(p2.GetPosition().x - GetPosition().x, p2.GetPosition().y - GetPosition().y, p2.GetPosition().z - GetPosition().z);

			float overlap = penetrationAxis(p2, l1, r1, u1, l2, r2, u2, CenterL, a);
			if (overlap < 0)//하나라도 겹치는 부분이 없으면 분리되어있음
				return false;
			if (overlap < minoverlap)
			{
				minoverlap = overlap;
				minindex = i;
			}
		}

		//여기 까지 왔으면 충돌한거임
		penetration = minoverlap;
		pAxis = axis[minindex];
		//중점을 이은벡터와 선택된 벡터가 내적시 0보다 크면 반대방향으로 바꿔줘야함. 왜냐하면 밀리는 방향은 나를향해야하므로

		float l = Float3Normalize(CenterL).x*axis[minindex].x + Float3Normalize(CenterL).y*axis[minindex].y + Float3Normalize(CenterL).z*axis[minindex].z;
		if (l > 0)
			pAxis = XMFLOAT3(-pAxis.x, -pAxis.y, -pAxis.z);

		//return true;
		if (penetration > MMPE_EPSILON)//엡실론보다 크면 충돌 아니면 정말 미세한 충돌이므로 처리를 할 필요가 없음
			return true;
		else
			return false;
	}
	return false;
}

void MiniPhysicsEngineG9::PhysicsPoint::CollisionResolve(PhysicsPoint & p2, XMFLOAT3 & CollisionN, double DeltaTime)
{
	//충돌후 속도를 계산함.
	ResolveVelocity(p2, CollisionN, DeltaTime);

	//속도계산후 겹치는 부분을 밀어냄
	ResolvePenetration(p2, DeltaTime);
}

float MiniPhysicsEngineG9::PhysicsPoint::GetSeparateVelocity(PhysicsPoint & p2, XMFLOAT3 & CollisionN)
{

	XMFLOAT3 v1 = GetVelocity();
	XMFLOAT3 v2 = p2.GetVelocity();
	v1 = Float3Add(v1, v2, false);
	return v1.x*CollisionN.x + v1.y*CollisionN.y + v1.z*CollisionN.z;

}

bool MiniPhysicsEngineG9::PhysicsPoint::GetBounce()
{
	return Bounce;
}

void MiniPhysicsEngineG9::PhysicsPoint::ResolveVelocity(PhysicsPoint & p2, XMFLOAT3 & CollisionN, double DeltaTime)
{
	//충돌후 속도 변화는 다음과 같다.
	//1. 충돌전 분리속도를 구하고
	//2. 충돌 후 분리속도를 구한다 Vf=-eVi 다.
	//3. 충격량 크기를 계산한다. W= M(Vf-Vi)
	//4. 충격량을 계산한다. 충격량 크기 * CollisionN이다. 기본적으로 CollisionN은 나의위치-상대방위치를 노멀화한것.
	//5. 최종 실속도 = 기존 실속도 + 충격량(MV)/M1 또는 M2 왜 이렇게 되냐면 최종적으로 움직이는 크기는 나의 질량에 반비례
	//해야하기 때문이다. 예를들어 M=Inverse(Inverse(M1)+Inverse(M2)) 이고, M1=1 M2=9였으면 M=9/10 가 나온다.
	//이걸 계산하면 M1인 녀석은 0.9만큼 밀리고 M2는 0.1 만큼 밀린다.
	//다만 키보드 입력시 밀려나야하는방향은 내가 키보드 누른 키의 반대방향의 속도다.


	float sv = GetSeparateVelocity(p2, CollisionN);
	if (sv > 0)//이미 멀어지고있음
		return;
	float nsv = -sv * 1;//원래 여기에 1대신 탄성계수 e가 들어가는데, 우리가 만드는 게임의 질점오브젝트의 e는 1이다.
	float deltav = nsv - sv;

	float totalImass = GetMass();
	totalImass += p2.GetMass();
	if (totalImass <= 0)
		return;
	float impurse = deltav / totalImass;

	XMFLOAT3 impursevector(CollisionN.x*impurse, CollisionN.y*impurse, CollisionN.z*impurse);

	//현재 물체의 변화 된 최종 속도
	auto resultv = GetVelocity();
	XMFLOAT3 iv1(impursevector.x*GetMass(), impursevector.y*GetMass(), impursevector.z*GetMass());
	resultv = Float3Add(resultv, iv1);
	SetVelocity(resultv);


	//나와 부딪힌 물체의 최종속도

	auto resultv2 = p2.GetVelocity();
	XMFLOAT3 iv2(-impursevector.x*p2.GetMass(), -impursevector.y*p2.GetMass(), -impursevector.z*p2.GetMass());
	resultv2 = Float3Add(resultv2, iv2);
	p2.SetVelocity(resultv2);

	if (Bounce == false)
	{
		//튕겨나가기를 하지 않는다면 다음과 같은 절차가 필요하다.
		//1. 내가 bounce false면 나의 속도는 0이다.
		//2. 상대방 속도는 그냥 계산한 대로 둔다.

		auto gv = GetVelocity();


		//바운스가 false면 튕겨나가지 않도록 0으로 만든다.
		gv = XMFLOAT3(0, 0, 0);
		SetVelocity(gv);

	}
	//상대방도 Bounce가 false면 튕겨나가지 않도록함.
	if (p2.GetBounce() == false)
	{
		auto gv2 = p2.GetVelocity();

		gv2 = XMFLOAT3(0, 0, 0);
		p2.SetVelocity(gv2);
	}




}
//겹쳐질경우 서로를 밀어냄. 키보드 입력일땐 자신만 밀려남
//공중일때가 좀 문제임 공중이면 상대방이 밀릴수있음 왜냐하면 이게 정상인게
//공중상태로 내려갈때는 중력만 영향을 받게되므s로 키보드 입력이 아니게됨 이때 충돌이 일어나면
//대각선으로 보통 밀어내게 되고 이렇게 되면 y값을 제외하면 x나 z값이 0이아니게 되면서 밀리는것.
//만약 이게 마음에 안들면 x와 z의 속도가 0이면 밀려나지 않도록 하는 방법이 있음.
//물론 이게 또 문제가 뭐냐면 아예 완전히 겹쳐있는 순간에 안움직이고 있으면 안떼어내진다.
//나는 그냥 공중에서는 밀어내도 상관 없도록 하는게 나을것 같음.
void MiniPhysicsEngineG9::PhysicsPoint::ResolvePenetration(PhysicsPoint & p2, double DeltaTime)
{
	if (penetration < MMPE_EPSILON)//밀어낼 필요가 없는경우
		return;
	float totalInverseMass = GetMass();
	totalInverseMass += p2.GetMass();

	if (totalInverseMass <= 0)//둘다 무한대일경우
		return;
	//물체를 pAxis방향으로 옮긴다. 다만 질량에 반비례하게 움직임. 질량이클수록 덜움직인다.
	XMFLOAT3 movevector(pAxis.x*(penetration / totalInverseMass), pAxis.y*(penetration / totalInverseMass), pAxis.z*(penetration / totalInverseMass));

	//충분히 작으면 움직이지 않는다.왜 엡실론을 안썻냐면 엡실론을쓸경우 penetration이 엡실론정도일때
	//밀려나야할경우 안밀려나는 상황이 일어나서, 엡실론보다 10배작은 값으로 검사한다.
	if (FloatLength(movevector) <= 0.0001)
		movevector = XMFLOAT3(0, 0, 0);

	XMFLOAT3 m1(movevector.x*GetMass(), movevector.y*GetMass(), movevector.z*GetMass());
	auto p1 = GetPosition();
	auto result1 = Float4Add(p1, XMFloat3to4(m1));

	*CenterPos = result1;

	//SetPosition(result1);//자신을 옮긴다.


	XMFLOAT3 m2(-movevector.x*p2.GetMass(), -movevector.y*p2.GetMass(), -movevector.z*p2.GetMass());
	auto pp2 = p2.GetPosition();
	auto result2 = Float4Add(pp2, XMFloat3to4(m2));
	*p2.CenterPos = result2;//겹쳐진 대상을 옮긴다.

	//auto result2 = Float3Add(pp2, m2);
	//p2.SetPosition(result2);//겹쳐진 대상을 옮긴다.



}

//하나의 축을 기준으로 두 박스를 내적한 결과를 합산한후 중점의 거리벡터를 기준 축으로 투영한 결과를 뺐을때 0보다 크면 겹치고있으며 0보다 작으면 분리되어있다.
float MiniPhysicsEngineG9::PhysicsPoint::penetrationAxis(PhysicsPoint & p2, XMFLOAT3 & l1, XMFLOAT3 & r1, XMFLOAT3 & u1, XMFLOAT3 & l2, XMFLOAT3 & r2, XMFLOAT3 & u2, XMFLOAT3 & CenterLength, XMFLOAT3& Axis)
{
	float prj1;
	float prj2;
	float dst;

	//기준축과 x축y축z축을 내적한 결과를 다 더한다.

	prj1 = GetHalfBox().x*fabsf(Axis.x*r1.x + Axis.y*r1.y + Axis.z*r1.z) + GetHalfBox().y*fabsf(Axis.x*u1.x + Axis.y*u1.y + Axis.z*u1.z) + GetHalfBox().z*fabsf(Axis.x*l1.x + Axis.y*l1.y + Axis.z*l1.z);
	prj2 = p2.GetHalfBox().x*fabsf(Axis.x*r2.x + Axis.y*r2.y + Axis.z*r2.z) + p2.GetHalfBox().y*fabsf(Axis.x*u2.x + Axis.y*u2.y + Axis.z*u2.z) + p2.GetHalfBox().z*fabsf(Axis.x*l2.x + Axis.y*l2.y + Axis.z*l2.z);
	dst = fabsf(CenterLength.x*Axis.x + CenterLength.y*Axis.y + CenterLength.z*Axis.z);

	//이결과가 양수면 겹치고 음수면 분리되어있다.
	return prj1 + prj2 - dst;

}

//레이캐스팅 오브젝트는 광원이 시작되는 카메라 위치와 광선의 방향, 레이캐스트오브젝트를 가진 오브젝트의 중점,카메라를
//보유한 플레이어 오브젝트의 중점, 그리고 교점이 저장될 XMFLOAT3가 입력받아야 한다.
//결과가 반환될때 true면 해당 오브젝트는 광선에 충돌이 되었으며 광선과 충돌할때 가장 가까운 평면의 위치는
//SavePos에 저장된다.

//예를들어 3개의 오브젝트가 RayCasting을 호출해서 3개 모두 충돌했으면, 그중 가장가까운 SavePos를 갖는놈이
//최종적으로 충돌된 녀석일것이다. 이것은 레이캐스트 시스템에서 처리해야한다.
//
//bool MiniPhysicsEngineG9::RayCastObject::RayCasting(XMFLOAT3 & RayOrgin, XMFLOAT3 & RayDir, XMFLOAT3& ObjCenterPos, XMFLOAT3& PlayerPos,XMFLOAT3& Look, XMFLOAT3 * SavePos)
////ObjCenterPos는 해당 검사 대상오브젝트의 중점이고, PlayerPos는 카메라를 보유한 플레이어오브젝트의 위치다.
//{
//	//우리는 t가 가장 가까운 min값을 가지는 면을 향해 쏠것이므로 필요하다.
//	float mint = INFINITY;
//	
//	
//
//	XMVECTOR objpos = XMLoadFloat3(&ObjCenterPos);
//	XMVECTOR playerpos = XMLoadFloat3(&PlayerPos);
//	XMVECTOR Rd = XMLoadFloat3(&RayDir);
//	XMVECTOR Ro = XMLoadFloat3(&RayOrgin);
//	XMVECTOR look = XMLoadFloat3(&Look);
//	//먼저 오브젝트가 플레이어 뒤에있으면 검사 대상에서 벗어난다.
//
//
//	auto frontV=XMVector3Dot(look, XMVector3Normalize(objpos - playerpos));
//
//	float isFront = 0;
//	XMStoreFloat(&isFront, frontV);
//	if (isFront < 0)
//		return false;
//
//	//플레이어가 검사대상이면 6면을 검사한다.
//
//	for (int i = 0; i < 6; i++)//6면을 검사한다.
//	{
//		XMVECTOR N = XMLoadFloat3(&Plane[i].Normal);
//		
//		XMFLOAT3 NdotRayDir;
//		XMStoreFloat3(&NdotRayDir, XMVector3Dot(N, Rd));
//		if (NdotRayDir.x == 0)//광선과 평면의 노멀이 수직이면 교점이 없음(무수히 많거나 없거나)
//		{
//			continue;
//		}
//
//		//여기에 오면 우선 교점은 있다는 소리
//
//		//이를 위해 t를 구해야함. 직선위의 임의의 점은 X = RayOrgin + RayDir*t
//		//평면위 의 임의의점은 Plane[i].Normal*(X-(Plane[i].DefaultCenterPos+ObjCenterPos))=0
//		//Plane[i].Normal*(RayOrgin+RayDir*t - (Plane[i].DefaultCenterPos+ObjCenterPos))=0
//		//Plane[i].Normal*(RayOrgin+RayDir*t)-Plane[i].Normal*(Plane[i].DefaultCenterPos+ObjCenterPos)=0
//		//t={((Plane[i].DefaultCenterPos+ObjCenterPos) - RayOrgin)/Plane[i].Normal 도트 RayDir}  도트 Plane[i].Normal
//		XMVECTOR defaultpos = XMLoadFloat3(&Plane[i].DefaultCenterPos);
//		
//		auto sik1 = ((defaultpos + objpos) - Ro) / NdotRayDir.x;
//		XMFLOAT3 temp;
//		XMStoreFloat3(&temp, XMVector3Dot(sik1, N));
//
//		float t = temp.x;
//		
//		//이제 t를 이용해 교점X가 유한평면 안에 있는지 검사한다.
//		//이후 유한평면안에있으면서 dir와 testP-playerpos를 내적시 0보다 크거나 같으면 
//		//mint를 갱신한다.SavePos도 갱신한다. 아니라면 continue
//
//		auto X = Ro + Rd * t;
//		//X가 6개의 평면에서 하나라도 0보다 큰게 있으면 바깥에있는것이며 충돌안한것이다.
//		bool In = true;
//		
//		for (int j = 0; j < 6; j++)
//		{
//			XMVECTOR testN = XMLoadFloat3(&Plane[j].Normal);
//			XMVECTOR testP = XMLoadFloat3(&Plane[j].DefaultCenterPos);
//			auto testv = XMVector3Dot(testN, (X - (testP + objpos)));
//			XMFLOAT3 result;
//			XMStoreFloat3(&result, testv);
//	
//			if (result.x > 0)//0보다 크면 바운딩박스 안에 없다.
//			{
//				In = false;
//				break;
//			}			
//
//		}
//		//여기까지 오면 t와 SavePos는 유효한 판정을 가지는 교점이다.
//		//다만 이 교점이 최종적으로 갱신되기 위해선
//		//1. 기존 교점보다 카메라에 가까워야 하고
//		//2. 플레이어의 오브젝트 앞에 존재해야한다.
//		//즉 플레이어와 카메라가 떨어져있는 3인칭 게임에서 플레이어 몬스터 카메라 순으로 있는 경우
//		//플레이어는 앞을 보고 있을테므로, 뒤로 탄이 날아가면 안되기 때문에
//		//몬스터가 플레이어 앞에 있을때만 레이캐스팅이 성공해야한다.
//		//이를 위해 교점을 구했던 평면(defaultpos+objpos)의 위치에서 플레이어 위치를 뺀 벡터와
//		//카메라가 바라보고있는 방향(만약 오차가 심하면 플레이어의 Look으로 해야한다.)과 내적시
//		//0이상이고, t가 카메라에 더 가까우면 그 값으로 갱신한다.
//		if (In == true)
//		{
//			float isfront = 0;
//			auto ntp=XMVector3Normalize(defaultpos+objpos - playerpos);
//			XMStoreFloat(&isfront, XMVector3Dot(look,ntp ));
//
//			if (fabsf(t) < fabsf(mint)&&isfront>=0)
//			{
//				mint = t;
//				XMStoreFloat3(SavePos, X);
//			}
//
//		}
//		
//	}
//
//
//	if (mint == INFINITY)
//		return false;
//
//	return true;
//}

bool MiniPhysicsEngineG9::RayCastObject::RayCasting(XMFLOAT3 & RayOrgin, XMFLOAT3 & RayDir, XMFLOAT3& ObjCenterPos, XMFLOAT3& PlayerPos, XMFLOAT3& Look, XMFLOAT3 * SavePos)
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


	auto frontV = XMVector3Dot(look, XMVector3Normalize(objpos - playerpos));

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
		//2. 플레이어의 오브젝트 앞에 X가 존재해야한다.
		//즉 플레이어와 카메라가 떨어져있는 3인칭 게임에서 플레이어 몬스터 카메라 순으로 있는 경우
		//플레이어는 앞을 보고 있을테므로, 뒤로 탄이 날아가면 안되기 때문에
		//몬스터가 플레이어 앞에 있을때만 레이캐스팅이 성공해야한다.
		//이를 위해 교점의 위치에서 플레이어 위치를 뺀 벡터와
		//카메라가 바라보고있는 방향(만약 오차가 심하면 플레이어의 Look으로 해야한다.)과 내적시
		//0이상이고, t가 카메라에 더 가까우면 그 값으로 갱신한다.

		if (In == true)
		{
			float isfront = 0;
			auto ntp = XMVector3Normalize(X - playerpos);
			XMStoreFloat(&isfront, XMVector3Dot(look, ntp));

			if (fabsf(t) < fabsf(mint) && isfront >= 0)
			{
				mint = t;
				XMStoreFloat3(SavePos, X);

				if (i == 5)
					isTopPlane = true;
				else
					isTopPlane = false;
			}

		}

	}


	if (mint == INFINITY)
		return false;

	return true;
}

bool MiniPhysicsEngineG9::RayCastObject::RayCastingField(XMFLOAT3 & RayOrgin, XMFLOAT3 & RayDir, XMFLOAT3 & PlayerPos, XMFLOAT3 & Look, XMFLOAT3 * SavePos)
{



	XMVECTOR playerpos = XMLoadFloat3(&PlayerPos);
	XMVECTOR Rd = XMLoadFloat3(&RayDir);
	XMVECTOR Ro = XMLoadFloat3(&RayOrgin);
	XMVECTOR look = XMLoadFloat3(&Look);
	//먼저 오브젝트가 플레이어 뒤에있으면 검사 대상에서 벗어난다.


	//땅은 원점에 존재하는 평면이다.


	XMVECTOR N = XMVectorSet(0, 1, 0, 0);

	XMFLOAT3 NdotRayDir;
	XMStoreFloat3(&NdotRayDir, XMVector3Dot(N, Rd));
	if (NdotRayDir.x == 0)//광선과 평면의 노멀이 수직이면 교점이 없음(무수히 많거나 없거나)
	{
		return false;
	}

	//여기에 오면 우선 교점은 있다는 소리

	//이를 위해 t를 구해야함. 직선위의 임의의 점은 X = RayOrgin + RayDir*t
	//평면위 의 임의의점은 Plane[i].Normal*(X-(Plane[i].DefaultCenterPos+ObjCenterPos))=0
	//Plane[i].Normal*(RayOrgin+RayDir*t - (Plane[i].DefaultCenterPos+ObjCenterPos))=0
	//Plane[i].Normal*(RayOrgin+RayDir*t)-Plane[i].Normal*(Plane[i].DefaultCenterPos+ObjCenterPos)=0
	//t={((Plane[i].DefaultCenterPos+ObjCenterPos) - RayOrgin)/Plane[i].Normal 도트 RayDir}  도트 Plane[i].Normal


	auto sik1 = (-Ro) / NdotRayDir.x;
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, XMVector3Dot(sik1, N));

	float t = temp.x;

	//이제 t를 이용해 교점X가 유한평면 안에 있는지 검사한다.
	//이후 유한평면안에있으면서 dir와 testP-playerpos를 내적시 0보다 크거나 같으면 
	//mint를 갱신한다.SavePos도 갱신한다. 아니라면 continue

	auto X = Ro + Rd * t;
	//X가 6개의 평면에서 하나라도 0보다 큰게 있으면 바깥에있는것이며 충돌안한것이다.

	float isfront = 0;
	auto ntp = XMVector3Normalize(X - playerpos);
	XMStoreFloat(&isfront, XMVector3Dot(look, ntp));

	if (isfront >= 0)
	{

		XMStoreFloat3(SavePos, X);
		return true;

	}
	else
		return false;







}

void MiniPhysicsEngineG9::RayCastObject::SetPlane(XMFLOAT3 & x, XMFLOAT3 & y, XMFLOAT3 & z)
{
	//0 왼쪽 1 오른쪽 2 앞쪽 3 뒷쪽 4 아래쪽 5 위쪽
	Plane[0].Normal = XMFLOAT3(-1, 0, 0);
	Plane[0].DefaultCenterPos = XMFLOAT3(-x.x, -x.y, -x.z);

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

void MiniPhysicsEngineG9::GeneratorGravity::SetGravityAccel(XMFLOAT3 & ga)
{
	GravityAccel = ga;
}

XMFLOAT3 MiniPhysicsEngineG9::GeneratorGravity::GetGravityAccel()
{
	return GravityAccel;
}

void MiniPhysicsEngineG9::GeneratorGravity::Update(float DeltaTime, PhysicsPoint & pp)
{
	XMVECTOR ga = XMLoadFloat3(&GravityAccel);
	ga *= pp.GetMass(false);//중력 = 중력가속도 * 질량
	XMFLOAT3 gravityForce;
	XMStoreFloat3(&gravityForce, ga);

	pp.AddForce(gravityForce);
}
void MiniPhysicsEngineG9::GeneratorGravity::Update(float DeltaTime, RigidBody & rb)
{
	XMVECTOR ga = XMLoadFloat3(&GravityAccel);
	ga *= rb.GetMass(false);//중력 = 중력가속도 * 질량
	XMFLOAT3 gravityForce;
	XMStoreFloat3(&gravityForce, ga);

	rb.AddForce(gravityForce);
}

//힘계산할때 -k할거니까 그냥 k를 넣을것!!!
void MiniPhysicsEngineG9::GeneratorAnchor::SetAnchorSpring(XMFLOAT3 & a, float k, float l)
{
	AnchorPos = a;
	constantspring = k;
	DefaultLength = l;
}

void MiniPhysicsEngineG9::GeneratorAnchor::Update(float DeltaTime, PhysicsPoint & pp)
{
	XMVECTOR objpos = XMLoadFloat4(&pp.GetPosition());
	XMVECTOR ancpos = XMLoadFloat3(&AnchorPos);

	XMVECTOR obj_anc = objpos - ancpos;

	float curlen = FloatLength(obj_anc);

	//F=K(defaultlength-curlen); 왜 -를안붙였냐면 원래 델타X구할때 현재길이 - 기본길이임. 근데 반대로 해서 -1곱한거랑같음
	float F = constantspring * (DefaultLength - curlen);

	//이제 힘을 벡터화 시킨다. 즉 obj_anc를 노멀라이즈해서 방향으로하고 F를 크기로 한다.

	obj_anc = XMVector3Normalize(obj_anc);
	obj_anc *= F;
	XMFLOAT3 FinalForce;
	XMStoreFloat3(&FinalForce, obj_anc);
	pp.AddForce(FinalForce);

}

void MiniPhysicsEngineG9::GeneratorJump::Update(float DeltaTime, PhysicsPoint & pp)
{
	//점프는 힘을 가하기보단 속도를 직접 가한다. 왜냐하면 힘을 가할경우 힘이 정말 무지막지하게 필요하다.
	//왜냐하면 힘은 지속적으로 가해져야 쓸만하지, 스페이스바 눌렀을때 딱한번 발동되게 하려면 10000이상 줘야한다.
	//따라서 그냥 깔끔하게 속도를 지정해서 준다. 이러면 속도는 고정값이므로 무난하게 점프가 된다.
	auto v1 = pp.GetVelocity();
	auto p = Float3Add(v1, JumpVel);
	pp.SetVelocity(p);

}

void MiniPhysicsEngineG9::GeneratorJump::SetJumpVel(XMFLOAT3& Jump)
{
	JumpVel = Jump;
}

MiniPhysicsEngineG9::RigidBody::RigidBody()
{
	InverseMass = 1;
	Velocity = XMFLOAT3(0, 0, 0);
	AngularVelocity = XMFLOAT3(0, 0, 0);
	damping = 0.99;
	Angulardamping = 0.99;
	TotalForce = XMFLOAT3(0, 0, 0);
	TotalTorque = XMFLOAT3(0, 0, 0);
	Accel = XMFLOAT3(0, 0, 0);
	Bounce = false;
}

MiniPhysicsEngineG9::RigidBody::~RigidBody()
{
}

void MiniPhysicsEngineG9::RigidBody::integrate(double DeltaTime)
{
	//질량이 0 이하면 벽같은 것이므로 움직일 필요가 없다.
	if (InverseMass <= 0.0f)
		return;

	assert(DeltaTime >= 0.0);

	XMVECTOR centerpos = XMLoadFloat4(CenterPos);

	XMVECTOR velocity = XMLoadFloat3(&Velocity);

	XMVECTOR accel = XMLoadFloat3(&Accel);

	XMVECTOR totalforce = XMLoadFloat3(&TotalForce);

	//먼저 힘과 질량으로 가속도를 계산한다.

	//가속도 = 최종힘 * 질량의 역수. 가속도는 힘을가했을때만 생기는 것. F가 0이면 a는 0
	//따라서 힘을 한번만 가했으면 가속도는 한번만 0이아니고 이후는 계속 0이다.

	//다만 속도는 가속도로 인해 계산된 결과를 계속 유지하며 댐핑지수로 인해 0 에 가까워짐.


	//가속도 구하고 속도 구하고 마찰로 감속시킨후 위치를 구한다.

	accel = totalforce * InverseMass;

	//가속도를 통해 속도를 추가한다.
	velocity = velocity + accel * DeltaTime;



	// 댐핑지수를 통해 감속한다.
	velocity *= powf(damping, DeltaTime);

	//감속시킨게 엡실론 정도면 속도를 0으로 만듬.
	float e = 0;
	XMStoreFloat(&e, XMVector3Length(velocity));
	if (e <= MMPE_EPSILON)
		velocity = XMVectorZero();

	//속도와 가속도를 적분해 중점을 구한다.

	centerpos = centerpos + velocity * DeltaTime + 0.5*accel*DeltaTime*DeltaTime;



	//결과 물을 저장한다.
	XMStoreFloat4(CenterPos, centerpos);
	XMStoreFloat3(&Velocity, velocity);
	XMStoreFloat3(&Accel, accel);

	//이제 방향을 변경한다.
	XMVECTOR Avelocity = XMLoadFloat3(&AngularVelocity);
	XMVECTOR totaltorque = XMLoadFloat3(&TotalTorque);
	XMVECTOR orient = XMLoadFloat4(Orient);
	XMMATRIX IT = XMLoadFloat4x4(&GetIMoment());

	//각가속도 구하고 각속도 구하고 마찰로 감속시킨후 방햐을 구한다.

	// A = T/I == T * Inverse I

	XMVECTOR Aaccel = XMVector4Transform(totaltorque, IT);

	//각속도를 구함.
	Avelocity += Aaccel * DeltaTime;

	// 댐핑지수를 통해 감속한다.
	Avelocity *= powf(Angulardamping, DeltaTime);

	//감속시킨게 엡실론 정도면 속도를 0으로 만듬.
	float av = 0;
	XMStoreFloat(&av, XMVector3Length(Avelocity));
	if (av <= MMPE_EPSILON)
		Avelocity = XMVectorZero();



	//방향을 최종적 설정. orient = orient + 0.5*W*orient. 이때 W*orient는 쿼터니언 곱.

	orient = orient + 0.5f*XMQuaternionMultiply(Avelocity, orient)*DeltaTime;

	orient = XMQuaternionNormalize(orient);

	//결과 물을 저장한다.
	XMStoreFloat4(Orient, orient);
	XMStoreFloat3(&AngularVelocity, Avelocity);

	//모든 계산이 끝났으면 힘과토크를 초기화 한다.
	ForceClear();
	TorqueClear();
}

void MiniPhysicsEngineG9::RigidBody::SetMass(float M)
{
	assert(M != 0);
	InverseMass = (1.0) / M;
}

float MiniPhysicsEngineG9::RigidBody::GetMass(bool Inverse)
{
	if (Inverse)
		return InverseMass;
	else
		return (1.0) / InverseMass;
}

void MiniPhysicsEngineG9::RigidBody::SetIMoment(XMFLOAT4X4 & i)
{
	XMMATRIX ii = XMLoadFloat4x4(&i);
	XMVECTOR dt = XMMatrixDeterminant(ii);//행렬식

	ii = XMMatrixInverse(&dt, ii);

	XMStoreFloat4x4(&Inverse_I_Moment, ii);
}

//육면체용 관성모멘트 설정.
void MiniPhysicsEngineG9::RigidBody::SetIMoment(float x, float y, float z)
{
	XMFLOAT4X4 i{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	i._11 = (GetMass(false)*(z*z + y * y)) / 12;
	i._22 = (GetMass(false)*(z*z + x * x)) / 12;
	i._33 = (GetMass(false)*(x*x + y * y)) / 12;

	XMMATRIX ii = XMLoadFloat4x4(&i);
	XMVECTOR dt = XMMatrixDeterminant(ii);//행렬식

	ii = XMMatrixInverse(&dt, ii);

	XMStoreFloat4x4(&Inverse_I_Moment, ii);
}

XMFLOAT4X4 MiniPhysicsEngineG9::RigidBody::GetIMoment(bool Inverse)
{
	if (Inverse)
	{
		XMMATRIX ii = XMLoadFloat4x4(&Inverse_I_Moment);
		XMVECTOR dt = XMMatrixDeterminant(ii);//행렬식
		float dtf = 0;
		XMStoreFloat(&dtf, dt);
		if (dtf <= MMPE_EPSILON)
			int a = 3;
		ii = XMMatrixInverse(&dt, ii);//역이아닌 정상적인 관성모멘트

		XMVECTOR orient = XMLoadFloat4(Orient); // 물체가 회전한만큼 회전시킨다.
		XMMATRIX orientmatrix = XMMatrixRotationQuaternion(orient);
		ii *= orientmatrix;//ii는 디폴트 관성모멘트 * 방향쿼터니언행렬

						   //다시 역행렬로 변환
		dt = XMMatrixDeterminant(ii);//행렬식
		ii = XMMatrixInverse(&dt, ii);
		XMFLOAT4X4 temp;
		XMStoreFloat4x4(&temp, ii);
		return temp;
	}
	else
	{
		XMMATRIX ii = XMLoadFloat4x4(&Inverse_I_Moment);
		XMVECTOR dt = XMMatrixDeterminant(ii);//행렬식

		ii = XMMatrixInverse(&dt, ii);
		XMFLOAT4X4 temp;
		XMStoreFloat4x4(&temp, ii);

		return temp;
	}
}

void MiniPhysicsEngineG9::RigidBody::SetDamping(float D, float Ad)
{
	damping = D;
	Angulardamping = Ad;
}

float MiniPhysicsEngineG9::RigidBody::GetDamping()
{
	return damping;
}

float MiniPhysicsEngineG9::RigidBody::GetAngularDamping()
{
	return Angulardamping;
}

void MiniPhysicsEngineG9::RigidBody::SetBounce(bool bounce)
{
	Bounce = bounce;
}

void MiniPhysicsEngineG9::RigidBody::SetPosition(XMFLOAT4 * pos)
{
	CenterPos = pos;
}

void MiniPhysicsEngineG9::RigidBody::SetOrient(XMFLOAT4 * ori)
{
	Orient = ori;
}

void MiniPhysicsEngineG9::RigidBody::SetMinMaxImpurse(float min, float max)
{
	MaxImpurse = max;
	MinImpurse = min;
}

float MiniPhysicsEngineG9::RigidBody::GetMinImpurse()
{
	return MinImpurse;
}

float MiniPhysicsEngineG9::RigidBody::GetMaxImpurse()
{
	return MaxImpurse;
}


XMFLOAT4 MiniPhysicsEngineG9::RigidBody::GetPosition()
{
	return *CenterPos;
}

XMFLOAT4 MiniPhysicsEngineG9::RigidBody::GetOrient()
{
	return *Orient;
}

void MiniPhysicsEngineG9::RigidBody::SetVelocity(XMFLOAT3 & V)
{
	Velocity = V;

}

void MiniPhysicsEngineG9::RigidBody::SetVelocity(float x, float y, float z)
{
	Velocity = XMFLOAT3(x, y, z);
}

XMFLOAT3 MiniPhysicsEngineG9::RigidBody::GetVelocity()
{
	return Velocity;
}

void MiniPhysicsEngineG9::RigidBody::SetAngularVelocity(XMFLOAT3 & V)
{
	AngularVelocity = V;
}

void MiniPhysicsEngineG9::RigidBody::SetAngularVelocity(float x, float y, float z)
{
	AngularVelocity = XMFLOAT3(x, y, z);
}

XMFLOAT3 MiniPhysicsEngineG9::RigidBody::GetAngularVelocity()
{
	return AngularVelocity;
}

void MiniPhysicsEngineG9::RigidBody::SetAccel(XMFLOAT3 & A)
{
	Accel = A;
}

void MiniPhysicsEngineG9::RigidBody::SetAccel(float x, float y, float z)
{
	Accel = XMFLOAT3(x, y, z);
}

XMFLOAT3 MiniPhysicsEngineG9::RigidBody::GetAccel()
{
	return Accel;
}

void MiniPhysicsEngineG9::RigidBody::ForceClear()
{
	TotalForce = XMFLOAT3(0, 0, 0);
}

void MiniPhysicsEngineG9::RigidBody::AddForce(XMFLOAT3 & F)
{
	XMVECTOR f = XMLoadFloat3(&TotalForce);
	XMVECTOR f2 = XMLoadFloat3(&F);
	f = f + f2;
	XMStoreFloat3(&TotalForce, f);
}

void MiniPhysicsEngineG9::RigidBody::AddForce(float x, float y, float z)
{
	XMVECTOR f = XMLoadFloat3(&TotalForce);
	XMVECTOR f2 = XMLoadFloat3(&XMFLOAT3(x, y, z));
	f = f + f2;
	XMStoreFloat3(&TotalForce, f);
}

XMFLOAT3 MiniPhysicsEngineG9::RigidBody::GetTotalForce()
{
	return TotalForce;
}
//F2는 물체가 이동한 방향으로 힘을가하는것
void MiniPhysicsEngineG9::RigidBody::AddForcePoint(XMFLOAT3 & F, XMFLOAT3 & pointposition, XMFLOAT3& F2)
{
	XMFLOAT3 p = pointposition;
	auto p2 = XMFloat4to3(*CenterPos);
	p = Float3Add(p, p2, false);//p-=p2

	if (fabsf(p.x) <= MMPE_EPSILON / 10)
		p.x = 0;
	if (fabsf(p.y) <= MMPE_EPSILON / 10)
		p.y = 0;
	if (fabsf(p.z) <= MMPE_EPSILON / 10)
		p.z = 0;

	XMFLOAT3 t;
	if (F2.x == 0 && F2.y == 0 && F2.z == 0)
		t = Float3Cross(p, F);//토크 = 중점으로부터 힘을 가해진 벡터 X 힘의 방향
	else
		t = Float3Cross(p, F2);//토크 = 중점으로부터 힘을 가해진 벡터 X 힘의 방향

	if (fabsf(t.x) <= MMPE_EPSILON / 10)
		t.x = 0;
	if (fabsf(t.y) <= MMPE_EPSILON / 10)
		t.y = 0;
	if (fabsf(t.z) <= MMPE_EPSILON / 10)
		t.z = 0;

	AddForce(F);
	AddTorque(t);
}

//F2는 물체가 이동한 방향으로 힘을가하는것
void MiniPhysicsEngineG9::RigidBody::AddForcePoint(XMFLOAT3 & F, XMFLOAT4 & pointposition, XMFLOAT3& F2)
{
	XMFLOAT4 p = pointposition;
	auto p2 = *CenterPos;
	p = Float4Add(p, p2, false);//p-=p2

	if (fabsf(p.x) <= MMPE_EPSILON)
		p.x = 0;
	if (fabsf(p.y) <= MMPE_EPSILON)
		p.y = 0;
	if (fabsf(p.z) <= MMPE_EPSILON)
		p.z = 0;


	XMFLOAT3 p1 = XMFloat4to3(p);
	XMFLOAT3 t;
	if (F2.x == 0 && F2.y == 0 && F2.z == 0)
		t = Float3Cross(p1, F);//토크 = 중점으로부터 힘을 가해진 벡터 X 힘의 방향
	else
		t = Float3Cross(p1, F2);//토크 = 중점으로부터 힘을 가해진 벡터 X 힘의 방향

	if (fabsf(t.x) <= MMPE_EPSILON / 10)
		t.x = 0;
	if (fabsf(t.y) <= MMPE_EPSILON / 10)
		t.y = 0;
	if (fabsf(t.z) <= MMPE_EPSILON / 10)
		t.z = 0;



	AddForce(F);
	AddTorque(t);
}

void MiniPhysicsEngineG9::RigidBody::TorqueClear()
{
	TotalTorque = XMFLOAT3(0, 0, 0);
}

void MiniPhysicsEngineG9::RigidBody::AddTorque(XMFLOAT3 & T)
{
	XMVECTOR t = XMLoadFloat3(&TotalTorque);
	XMVECTOR t2 = XMLoadFloat3(&T);
	t = t + t2;
	XMStoreFloat3(&TotalTorque, t);
}

void MiniPhysicsEngineG9::RigidBody::AddTorque(float x, float y, float z)
{
	XMVECTOR t = XMLoadFloat3(&TotalTorque);
	XMFLOAT3 T{ x,y,z };
	XMVECTOR t2 = XMLoadFloat3(&T);
	t = t + t2;
	XMStoreFloat3(&TotalTorque, t);
}

XMFLOAT3 MiniPhysicsEngineG9::RigidBody::GetTotalTorque()
{
	return TotalTorque;
}

void MiniPhysicsEngineG9::RigidBody::SetHalfBox(float x, float y, float z)
{
	halfbox.x = x;
	halfbox.y = y;
	halfbox.z = z;
	rad = sqrt(x*x + y * y + z * z);
}

float MiniPhysicsEngineG9::RigidBody::GetRad()
{
	return rad;
}

XMFLOAT3 MiniPhysicsEngineG9::RigidBody::GetHalfBox()
{
	return halfbox;
}

void MiniPhysicsEngineG9::RigidBody::GetEightPoint(XMFLOAT4 * arr, XMFLOAT3& Up, XMFLOAT3& Look, XMFLOAT3& Right)
{
	XMVECTOR cp = XMLoadFloat4(CenterPos);
	XMVECTOR up = XMLoadFloat3(&Up);
	XMVECTOR look = XMLoadFloat3(&Look);
	XMVECTOR right = XMLoadFloat3(&Right);
	up = XMVector3Normalize(up);
	look = XMVector3Normalize(look);
	right = XMVector3Normalize(right);
	up *= halfbox.y;
	right *= halfbox.x;
	look *= halfbox.z;

	XMVECTOR ar[8];
	ar[0] = cp + up - right + look;
	ar[1] = cp + up + right + look;
	ar[2] = cp + up + right - look;
	ar[3] = cp + up - right - look;

	ar[4] = cp - up - right + look;
	ar[5] = cp - up + right + look;
	ar[6] = cp - up + right - look;
	ar[7] = cp - up - right - look;

	for (int i = 0; i < 8; i++)
		XMStoreFloat4(&arr[i], ar[i]);

}

bool MiniPhysicsEngineG9::RigidBody::GetBounce()
{
	return Bounce;
}

float MiniPhysicsEngineG9::RigidBody::penetrationAxis(RigidBody & rb2, XMFLOAT3 & l1, XMFLOAT3 & r1, XMFLOAT3 & u1, XMFLOAT3 & l2, XMFLOAT3 & r2, XMFLOAT3 & u2, XMFLOAT3 & CenterLength, XMFLOAT3 & Axis)
{
	float prj1;
	float prj2;
	float dst;

	//기준축과 x축y축z축을 내적한 결과를 다 더한다.

	prj1 = GetHalfBox().x*fabsf(Axis.x*r1.x + Axis.y*r1.y + Axis.z*r1.z) + GetHalfBox().y*fabsf(Axis.x*u1.x + Axis.y*u1.y + Axis.z*u1.z) + GetHalfBox().z*fabsf(Axis.x*l1.x + Axis.y*l1.y + Axis.z*l1.z);
	prj2 = rb2.GetHalfBox().x*fabsf(Axis.x*r2.x + Axis.y*r2.y + Axis.z*r2.z) + rb2.GetHalfBox().y*fabsf(Axis.x*u2.x + Axis.y*u2.y + Axis.z*u2.z) + rb2.GetHalfBox().z*fabsf(Axis.x*l2.x + Axis.y*l2.y + Axis.z*l2.z);
	dst = fabsf(CenterLength.x*Axis.x + CenterLength.y*Axis.y + CenterLength.z*Axis.z);

	//이결과가 양수면 겹치고 음수면 분리되어있다.
	return prj1 + prj2 - dst;

}
bool MiniPhysicsEngineG9::RigidBody::CollisionTest(RigidBody & rb2, XMFLOAT3 & l1, XMFLOAT3 & r1, XMFLOAT3 & u1, XMFLOAT3 & l2, XMFLOAT3 & r2, XMFLOAT3 & u2)
{

	//먼저 구 오브젝트로 검사한다.
	float L = FloatLength(XMFloat4to3(Float4Add(*CenterPos, *rb2.CenterPos, false)));
	if (GetRad() + rb2.GetRad() > L)
	{
		CollisionPointVector.clear();

		RigidBody* Testing = NULL;//정점검사 대상
		RigidBody* Other = NULL;//아닌녀석

								//15번 검사를 해야하며, 각 축의 정보는 다음과같다.
		XMFLOAT3 axis[15];
		//1번 상자의 x,y,z축
		axis[0] = r1;
		axis[1] = u1;
		axis[2] = l1;

		//2번 상자의 x,y,z축
		axis[3] = r2;
		axis[4] = u2;
		axis[5] = l2;

		//1번과 2번 각 축과 축의 외적
		axis[6] = Float3Cross(r1, r2);
		axis[6] = Float3Normalize(axis[6]);
		axis[7] = Float3Cross(r1, u2);
		axis[7] = Float3Normalize(axis[7]);
		axis[8] = Float3Cross(r1, l2);
		axis[8] = Float3Normalize(axis[8]);


		axis[9] = Float3Cross(u1, r2);
		axis[9] = Float3Normalize(axis[9]);
		axis[10] = Float3Cross(u1, u2);
		axis[10] = Float3Normalize(axis[10]);
		axis[11] = Float3Cross(u1, l2);
		axis[11] = Float3Normalize(axis[11]);

		axis[12] = Float3Cross(l1, r2);
		axis[12] = Float3Normalize(axis[12]);
		axis[13] = Float3Cross(l1, u2);
		axis[13] = Float3Normalize(axis[13]);
		axis[14] = Float3Cross(l1, l2);
		axis[14] = Float3Normalize(axis[14]);

		XMFLOAT3 CenterL;//중점끼리 연결한 벡터
						 //최소로 겹치는 부분을 찾아야함. 그곳이 겹치는 최소영역의 길이

		float minoverlap = INFINITY;
		//최소겹치는 영역에서 밀어내야할 방향의 인덱스
		int minindex;
		CenterL = XMFLOAT3(rb2.GetPosition().x - GetPosition().x, rb2.GetPosition().y - GetPosition().y, rb2.GetPosition().z - GetPosition().z);

		//0~5중 현재 minindex;
		int minSingleAxis = 0;
		//15번 반복함
		for (int i = 0; i < 15; i++)
		{

			XMFLOAT3 a = axis[i];
			if (FloatLength(a) < 0.001)//외적의 결과가 0이면(부동소수점오차때문에 0에가까우면) 사실상 0이므로 검사X
				continue;

			a = Float3Normalize(a);

			float overlap = penetrationAxis(rb2, l1, r1, u1, l2, r2, u2, CenterL, a);
			if (overlap < 0)//하나라도 겹치는 부분이 없으면 분리되어있음
				return false;
			if (overlap < minoverlap)
			{
				minoverlap = overlap;
				minindex = i;
			}
		}

		//여기 까지 왔으면 충돌한거임

		auto penetration = minoverlap;

		//0~5까지 일경우 이값이  paxis가 된다.
		auto pAxis = axis[minindex];

		//방향은 나중에 바꿔주자. 우선 해야할건 정점을 검사할 오브젝트를 찾기 위한 CenterLNormal을 구하는것.

		auto CenterLNormal = Float3Normalize(CenterL);

		float DotCp = CenterLNormal.x*pAxis.x + CenterLNormal.y*pAxis.y + CenterLNormal.z*pAxis.z;

		XMFLOAT4 arr[8]; // 저장될 정점 배열
		XMFLOAT4 arr2[8]; // 저장될 정점 배열 other

						  //6개의 Other 평면
		XMFLOAT3 Plane[6];
		//6개의 Testing평면
		XMFLOAT3 Plane2[6];
		//6개의 평면의 원점에서의 위치.
		XMFLOAT3 PlaneOrg[6];
		//6개의 평면의 원점에서의 위치.
		XMFLOAT3 PlaneOrg2[6];


		//결과가 양수면 정점 검사할 대상은 B 아니면 A.
		if (DotCp >= 0)
		{
			Testing = &rb2;
			Other = this;

			Testing->GetEightPoint(arr, u2, l2, r2);
			Other->GetEightPoint(arr2, u1, l1, r1);
			Plane[0] = r1;
			Plane[1] = u1;
			Plane[2] = l1;
			Plane[3] = Float3Float(r1, -1);
			Plane[4] = Float3Float(u1, -1);
			Plane[5] = Float3Float(l1, -1);


			//6개의 평면에서의 위치
			PlaneOrg[0] = Float3Float(r1, Other->halfbox.x);
			PlaneOrg[1] = Float3Float(u1, Other->halfbox.y);
			PlaneOrg[2] = Float3Float(l1, Other->halfbox.z);
			PlaneOrg[3] = Float3Float(r1, -1 * Other->halfbox.x);
			PlaneOrg[4] = Float3Float(u1, -1 * Other->halfbox.y);
			PlaneOrg[5] = Float3Float(l1, -1 * Other->halfbox.z);


			Plane2[0] = r2;
			Plane2[1] = u2;
			Plane2[2] = l2;
			Plane2[3] = Float3Float(r2, -1);
			Plane2[4] = Float3Float(u2, -1);
			Plane2[5] = Float3Float(l2, -1);


			//6개의 평면에서의 위치
			PlaneOrg2[0] = Float3Float(r2, Testing->halfbox.x);
			PlaneOrg2[1] = Float3Float(u2, Testing->halfbox.y);
			PlaneOrg2[2] = Float3Float(l2, Testing->halfbox.z);
			PlaneOrg2[3] = Float3Float(r2, -1 * Testing->halfbox.x);
			PlaneOrg2[4] = Float3Float(u2, -1 * Testing->halfbox.y);
			PlaneOrg2[5] = Float3Float(l2, -1 * Testing->halfbox.z);
		}
		else
		{
			Testing = this;
			Other = &rb2;

			Testing->GetEightPoint(arr, u1, l1, r1);
			Other->GetEightPoint(arr2, u2, l2, r2);

			Plane[0] = r2;
			Plane[1] = u2;
			Plane[2] = l2;
			Plane[3] = Float3Float(r2, -1);
			Plane[4] = Float3Float(u2, -1);
			Plane[5] = Float3Float(l2, -1);


			//6개의 평면에서의 위치
			PlaneOrg[0] = Float3Float(r2, Other->halfbox.x);
			PlaneOrg[1] = Float3Float(u2, Other->halfbox.y);
			PlaneOrg[2] = Float3Float(l2, Other->halfbox.z);
			PlaneOrg[3] = Float3Float(r2, -1 * Other->halfbox.x);
			PlaneOrg[4] = Float3Float(u2, -1 * Other->halfbox.y);
			PlaneOrg[5] = Float3Float(l2, -1 * Other->halfbox.z);


			Plane2[0] = r1;
			Plane2[1] = u1;
			Plane2[2] = l1;
			Plane2[3] = Float3Float(r1, -1);
			Plane2[4] = Float3Float(u1, -1);
			Plane2[5] = Float3Float(l1, -1);


			//6개의 평면에서의 위치
			PlaneOrg2[0] = Float3Float(r1, Testing->halfbox.x);
			PlaneOrg2[1] = Float3Float(u1, Testing->halfbox.y);
			PlaneOrg2[2] = Float3Float(l1, Testing->halfbox.z);
			PlaneOrg2[3] = Float3Float(r1, -1 * Testing->halfbox.x);
			PlaneOrg2[4] = Float3Float(u1, -1 * Testing->halfbox.y);
			PlaneOrg2[5] = Float3Float(l1, -1 * Testing->halfbox.z);


		}

		std::vector<CollisionPoint> AllPoint;
		std::vector<CollisionPoint> AllPoint2;

		for (int i = 0; i < 8; i++)
		{
			CollisionPoint cp;
			cp.Pos = arr[i];
			cp.penetration = pAxis.x*arr[i].x + pAxis.y*arr[i].y + pAxis.z*arr[i].z;
			AllPoint.push_back(cp);

			//other
			CollisionPoint cp2;
			cp2.Pos = arr2[i];
			cp2.penetration = pAxis.x*arr2[i].x + pAxis.y*arr2[i].y + pAxis.z*arr2[i].z;
			AllPoint2.push_back(cp2);

		}
		//모든 정점을 상대적인 penetration으로 정렬한다. 굳이 정확하지 않아도 된다. 그저 가장 작은 penetration을 찾는게 목표니까.
		sort(AllPoint.begin(), AllPoint.end(), [](CollisionPoint& p1, CollisionPoint& p2) {return p1.penetration < p2.penetration; });
		//other은 크기가 큰순으로
		sort(AllPoint2.begin(), AllPoint2.end(), [](CollisionPoint& p1, CollisionPoint& p2) {return p1.penetration > p2.penetration; });


		//이제 정렬된 8개의 점중 평면의 방정식에 의해 일단 충돌되었다고 생각되는 점들을 찾는다.
		//이 점의 갯수가 1개 2개 4개 일것이며, 그에따라 다르게 처리해야한다.
		vector<CollisionPoint> ColPoint;//일단 충돌은 일어난 정점들.
		vector<CollisionPoint> ColPoint2;//일단 충돌은 일어난 정점들. other

										 //어처피 충돌이 일어났다면 가장 작은 penetration과 일치하는 점은 최대 4개밖에 없으므로 4번만 검사한다.
		for (int i = 0; i < 4; i++)
		{
			if (fabsf(AllPoint[0].penetration - AllPoint[i].penetration) <= MMPE_EPSILON)
				ColPoint.push_back(AllPoint[i]);

			if (fabsf(AllPoint2[0].penetration - AllPoint2[i].penetration) <= MMPE_EPSILON)
				ColPoint2.push_back(AllPoint2[i]);
		}



		//이제 ColPoint를 통해 실제로 충돌한 점이 무엇인지 검사한다. ColPoint에 저장된 녀석들은 일단 pAxis의 평면의 뒤쪽에 있기에 당첨된 점들이므로
		//만약 점이 1개만 ColPoint라면 무조건 그점은 실제 충돌점이다. 왜? 일단 애초에 육면체 충돌이 일어났기 때문에 반드시 점 1개는 충돌했을텐데, 점이 딱 1개니까.
		//2개라면 여기서 부터 좀 생각해야한다. 왜냐하면 점 2개일때는 선과 면충돌이 되서 충돌을 안하는경우가 생김. 이를 보정하기위해 중점3을 추가해서 육면체안에 있는지
		//검사해야함.

		int sz = ColPoint.size();
		int sz2 = ColPoint2.size();


		vector<CollisionPoint> lastPoints;

		if (sz == 1)
		{
			CollisionPointVector.push_back(ColPoint[0]);

		}
		else if (sz == 2)
		{
			//최종적인 충돌점들.


			XMFLOAT4 pos3 = Float4Add(ColPoint[0].Pos, ColPoint[1].Pos);
			pos3 = Float4Float(pos3, 0.5);
			CollisionPoint newPoint = ColPoint[0];
			newPoint.Pos = pos3;
			//중점3을 만들어서 넣음.
			ColPoint.push_back(newPoint);

			//상대방 육면체 안에 존재하는 점이 있는지 검사한다.
			for (int i = 0; i < 3; i++)
			{
				bool In = true;
				XMVECTOR X = XMLoadFloat4(&ColPoint[i].Pos);
				for (int j = 0; j < 6; j++)
				{


					XMVECTOR testN = XMLoadFloat3(&Plane[j]);
					XMVECTOR testP = XMLoadFloat4(Other->CenterPos);
					XMVECTOR objpos = XMLoadFloat3(&PlaneOrg[j]);
					auto testv = XMVector3Dot(testN, (X - (testP + objpos)));
					XMFLOAT3 result;
					XMStoreFloat3(&result, testv);

					if (result.x > 0)//0보다 크면 바운딩박스 안에 없다.
					{
						In = false;
						break;
					}

				}

				//안에 존재하는 점이면 그점은 최종적인 충돌점에 추가한다.
				if (In == true)
				{
					lastPoints.push_back(ColPoint[i]);
				}

			}


			//여기선 lastPoints에 저장된 점을  CollisionPointVector 에 추가한다.

			//만약에 충돌점이 0인경우 other로 다시 검사한다.
			if (lastPoints.size() == 0)
			{
				if (sz2 == 1)
				{
					CollisionPointVector.push_back(ColPoint2[0]);
				}
				else if (sz2 == 2)
				{

					XMFLOAT4 pos3 = Float4Add(ColPoint2[0].Pos, ColPoint2[1].Pos);
					pos3 = Float4Float(pos3, 0.5);
					CollisionPoint newPoint = ColPoint2[0];
					newPoint.Pos = pos3;
					//중점3을 만들어서 넣음.
					ColPoint2.push_back(newPoint);

					//상대방 육면체 안에 존재하는 점이 있는지 검사한다.
					for (int i = 0; i < 3; i++)
					{
						bool In = true;
						XMVECTOR X = XMLoadFloat4(&ColPoint2[i].Pos);
						for (int j = 0; j < 6; j++)
						{

							XMVECTOR testN = XMLoadFloat3(&Plane2[j]);
							XMVECTOR testP = XMLoadFloat4(Testing->CenterPos);
							XMVECTOR objpos = XMLoadFloat3(&PlaneOrg2[j]);
							auto testv = XMVector3Dot(testN, (X - (testP + objpos)));
							XMFLOAT3 result;
							XMStoreFloat3(&result, testv);

							if (result.x > 0)//0보다 크면 바운딩박스 안에 없다.
							{
								In = false;
								break;
							}

						}

						//안에 존재하는 점이면 그점은 최종적인 충돌점에 추가한다.
						if (In == true)
						{
							lastPoints.push_back(ColPoint2[i]);
						}


					}
					//상대방 점으로 계산해도 실패하면...
					if (lastPoints.size() == 0)
					{
						//ColPoint2 의 중점을 이용해 가장가까운 점을 찾는다. 이때 좀더 정확하게 하기위해 ColPoint의 중점3과 점1의 중간점, 중점3과 점2의중간점을 추가
						//해서 5개의 점으로 검사하자.
						XMFLOAT4 pos4 = Float4Add(ColPoint[0].Pos, ColPoint[2].Pos);
						pos4 = Float4Float(pos4, 0.5);
						CollisionPoint newPoint = ColPoint[0];
						newPoint.Pos = pos4;
						//중점4를 만들어서 넣음.
						ColPoint.push_back(newPoint);

						pos4 = Float4Add(ColPoint[1].Pos, ColPoint[2].Pos);
						pos4 = Float4Float(pos4, 0.5);
						newPoint = ColPoint[0];
						newPoint.Pos = pos4;
						//중점5를 만들어서 넣음.
						ColPoint.push_back(newPoint);

						//Other가 가진 중점과 5개의 ColPoint점 중 가장 가까운 점한개를 찾음.
						float MinLn = 10000;
						int index = 0;
						for (int g = 0; g < 5; g++)
						{
							auto vl = Float4Add(ColPoint2[2].Pos, ColPoint[g].Pos, false);
							auto ml = FloatLength(vl);
							if (MinLn > ml)
							{
								MinLn = ml;
								index = g;
							}
						}

						//가장짧았던 ColPoint의 인덱스의 점을 최종
						lastPoints.push_back(ColPoint[index]);
					}

					if (lastPoints.size() == 1)//점1개  경우 그점을 최종점으로 한다.
					{
						CollisionPointVector.push_back(lastPoints[0]);
					}
					else//그외는 중점으로
					{
						CollisionPointVector.push_back(ColPoint2[2]);
					}


				}
				else if (sz2 == 4)
				{
					XMFLOAT4 pos1 = Float4Add(ColPoint2[0].Pos, ColPoint2[1].Pos);
					XMFLOAT4 pos2 = Float4Add(ColPoint2[2].Pos, ColPoint2[3].Pos);
					XMFLOAT4 pos5 = Float4Add(pos1, pos2);
					pos5 = Float4Float(pos5, 0.25);
					CollisionPoint newPoint = ColPoint2[0];
					newPoint.Pos = pos5;
					//중점3을 만들어서 넣음.
					ColPoint2.push_back(newPoint);

					//상대방 육면체 안에 존재하는 점이 있는지 검사한다.
					for (int i = 0; i < 5; i++)
					{
						bool In = true;
						XMVECTOR X = XMLoadFloat4(&ColPoint2[i].Pos);
						for (int j = 0; j < 6; j++)
						{


							XMVECTOR testN = XMLoadFloat3(&Plane2[j]);
							XMVECTOR testP = XMLoadFloat4(Testing->CenterPos);
							XMVECTOR objpos = XMLoadFloat3(&PlaneOrg2[j]);
							auto testv = XMVector3Dot(testN, (X - (testP + objpos)));
							XMFLOAT3 result;
							XMStoreFloat3(&result, testv);

							if (result.x > 0)//0보다 크면 바운딩박스 안에 없다.
							{
								In = false;
								break;
							}

						}

						//안에 존재하는 점이면 그점은 최종적인 충돌점에 추가한다.
						if (In == true)
						{
							lastPoints.push_back(ColPoint2[i]);
						}


					}

					if (lastPoints.size() == 1)//점1개면 그 점을 충돌점으로 한다.
					{
						CollisionPointVector.push_back(lastPoints[0]);
					}
					else if (lastPoints.size() == 2)//점2개면 두개의 중점.
					{

						XMFLOAT4 pos = Float4Add(lastPoints[0].Pos, lastPoints[1].Pos);
						pos = Float4Float(pos, 0.5);
						CollisionPoint newPoint = lastPoints[0];
						newPoint.Pos = pos;

						CollisionPointVector.push_back(newPoint);

					}
					else//점3개이상이면 중점5로
					{
						CollisionPointVector.push_back(ColPoint2[4]);
					}


				}
			}
			else if (lastPoints.size() == 1)//점1개  경우 그점을 최종점으로 한다.
			{
				CollisionPointVector.push_back(lastPoints[0]);
			}
			else//그외는 중점으로
			{
				CollisionPointVector.push_back(ColPoint[2]);
			}
		}
		else if (sz == 4)
		{
			XMFLOAT4 pos1 = Float4Add(ColPoint[0].Pos, ColPoint[1].Pos);
			XMFLOAT4 pos2 = Float4Add(ColPoint[2].Pos, ColPoint[3].Pos);
			XMFLOAT4 pos5 = Float4Add(pos1, pos2);
			pos5 = Float4Float(pos5, 0.25);
			CollisionPoint newPoint = ColPoint[0];
			newPoint.Pos = pos5;
			//중점3을 만들어서 넣음.
			ColPoint.push_back(newPoint);

			//상대방 육면체 안에 존재하는 점이 있는지 검사한다.
			for (int i = 0; i < 5; i++)
			{
				bool In = true;
				XMVECTOR X = XMLoadFloat4(&ColPoint[i].Pos);
				for (int j = 0; j < 6; j++)
				{


					XMVECTOR testN = XMLoadFloat3(&Plane[j]);
					XMVECTOR testP = XMLoadFloat4(Other->CenterPos);
					XMVECTOR objpos = XMLoadFloat3(&PlaneOrg[j]);
					auto testv = XMVector3Dot(testN, (X - (testP + objpos)));
					XMFLOAT3 result;
					XMStoreFloat3(&result, testv);

					if (result.x > 0)//0보다 크면 바운딩박스 안에 없다.
					{
						In = false;
						break;
					}

				}

				//안에 존재하는 점이면 그점은 최종적인 충돌점에 추가한다.
				if (In == true)
				{
					lastPoints.push_back(ColPoint[i]);
				}


			}

			//만약에 충돌점이 0인경우 other로 다시 검사한다.
			if (lastPoints.size() == 0)
			{
				if (sz2 == 1)
				{
					CollisionPointVector.push_back(ColPoint2[0]);
				}
				else if (sz2 == 2)
				{

					XMFLOAT4 pos3 = Float4Add(ColPoint2[0].Pos, ColPoint2[1].Pos);
					pos3 = Float4Float(pos3, 0.5);
					CollisionPoint newPoint = ColPoint2[0];
					newPoint.Pos = pos3;
					//중점3을 만들어서 넣음.
					ColPoint2.push_back(newPoint);

					//상대방 육면체 안에 존재하는 점이 있는지 검사한다.
					for (int i = 0; i < 3; i++)
					{
						bool In = true;
						XMVECTOR X = XMLoadFloat4(&ColPoint2[i].Pos);
						for (int j = 0; j < 6; j++)
						{

							XMVECTOR testN = XMLoadFloat3(&Plane2[j]);
							XMVECTOR testP = XMLoadFloat4(Testing->CenterPos);
							XMVECTOR objpos = XMLoadFloat3(&PlaneOrg2[j]);
							auto testv = XMVector3Dot(testN, (X - (testP + objpos)));
							XMFLOAT3 result;
							XMStoreFloat3(&result, testv);

							if (result.x > 0)//0보다 크면 바운딩박스 안에 없다.
							{
								In = false;
								break;
							}

						}

						//안에 존재하는 점이면 그점은 최종적인 충돌점에 추가한다.
						if (In == true)
						{
							lastPoints.push_back(ColPoint2[i]);
						}



					}
					//상대방 점으로 계산해도 실패하면...
					if (lastPoints.size() == 0)
					{
						//ColPoint2 의 중점을 이용해 가장가까운 점을 찾는다. 이때 좀더 정확하게 하기위해 ColPoint의 중점3과 점1의 중간점, 중점3과 점2의중간점을 추가
						//해서 5개의 점으로 검사하자.
						XMFLOAT4 pos4 = Float4Add(ColPoint[0].Pos, ColPoint[2].Pos);
						pos4 = Float4Float(pos4, 0.5);
						CollisionPoint newPoint = ColPoint[0];
						newPoint.Pos = pos4;
						//중점4를 만들어서 넣음.
						ColPoint.push_back(newPoint);

						pos4 = Float4Add(ColPoint[1].Pos, ColPoint[2].Pos);
						pos4 = Float4Float(pos4, 0.5);
						newPoint = ColPoint[0];
						newPoint.Pos = pos4;
						//중점5를 만들어서 넣음.
						ColPoint.push_back(newPoint);

						//Other가 가진 중점과 5개의 ColPoint점 중 가장 가까운 점한개를 찾음.
						float MinLn = 10000;
						int index = 0;
						for (int g = 0; g < 5; g++)
						{
							auto vl = Float4Add(ColPoint2[2].Pos, ColPoint[g].Pos, false);
							auto ml = FloatLength(vl);
							if (MinLn > ml)
							{
								MinLn = ml;
								index = g;
							}
						}

						//가장짧았던 ColPoint의 인덱스의 점을 최종
						lastPoints.push_back(ColPoint[index]);
					}

					if (lastPoints.size() == 1)//점1개  경우 그점을 최종점으로 한다.
					{
						CollisionPointVector.push_back(lastPoints[0]);
					}
					else//그외는 중점으로
					{
						CollisionPointVector.push_back(ColPoint2[2]);
					}


				}
				else if (sz2 == 4)
				{

					XMFLOAT4 pos1 = Float4Add(ColPoint2[0].Pos, ColPoint2[1].Pos);
					XMFLOAT4 pos2 = Float4Add(ColPoint2[2].Pos, ColPoint2[3].Pos);
					XMFLOAT4 pos5 = Float4Add(pos1, pos2);
					pos5 = Float4Float(pos5, 0.25);
					CollisionPoint newPoint = ColPoint2[0];
					newPoint.Pos = pos5;
					//중점3을 만들어서 넣음.
					ColPoint2.push_back(newPoint);

					//상대방 육면체 안에 존재하는 점이 있는지 검사한다.
					for (int i = 0; i < 5; i++)
					{
						bool In = true;
						XMVECTOR X = XMLoadFloat4(&ColPoint2[i].Pos);
						for (int j = 0; j < 6; j++)
						{


							XMVECTOR testN = XMLoadFloat3(&Plane2[j]);
							XMVECTOR testP = XMLoadFloat4(Testing->CenterPos);
							XMVECTOR objpos = XMLoadFloat3(&PlaneOrg2[j]);
							auto testv = XMVector3Dot(testN, (X - (testP + objpos)));
							XMFLOAT3 result;
							XMStoreFloat3(&result, testv);

							if (result.x > 0)//0보다 크면 바운딩박스 안에 없다.
							{
								In = false;
								break;
							}

						}

						//안에 존재하는 점이면 그점은 최종적인 충돌점에 추가한다.
						if (In == true)
						{
							lastPoints.push_back(ColPoint2[i]);
						}


					}

					if (lastPoints.size() == 1)//점1개면 그 점을 충돌점으로 한다.
					{
						CollisionPointVector.push_back(lastPoints[0]);
					}
					else if (lastPoints.size() == 2)//점2개면 두개의 중점.
					{

						XMFLOAT4 pos = Float4Add(lastPoints[0].Pos, lastPoints[1].Pos);
						pos = Float4Float(pos, 0.5);
						CollisionPoint newPoint = lastPoints[0];
						newPoint.Pos = pos;

						CollisionPointVector.push_back(newPoint);

					}
					else if (lastPoints.size() >= 3)//점3개이상이면 중점5로
					{
						CollisionPointVector.push_back(ColPoint2[4]);
					}
					else//최종적으로 못찾은 경우
					{
						//면과 면 충돌인데 뒤집어서 검사해도 없다면, 두번째 오브젝트의 중점에 가장 가까운 첫번째 오브젝트의 충돌점을 선택한다.
						float MinLn = 10000;
						int index = 0;
						for (int g = 0; g < 5; g++)
						{
							auto vl = Float4Add(ColPoint2[4].Pos, ColPoint[g].Pos, false);
							auto ml = FloatLength(vl);
							if (MinLn > ml)
							{
								MinLn = ml;
								index = g;
							}
						}
						CollisionPointVector.push_back(ColPoint[index]);
					}
				}
			}
			else if (lastPoints.size() == 1)//점1개면 그 점을 충돌점으로 한다.
			{
				CollisionPointVector.push_back(lastPoints[0]);
			}
			else if (lastPoints.size() == 2)//점2개면 두개의 중점.
			{

				XMFLOAT4 pos = Float4Add(lastPoints[0].Pos, lastPoints[1].Pos);
				pos = Float4Float(pos, 0.5);
				CollisionPoint newPoint = lastPoints[0];
				newPoint.Pos = pos;

				CollisionPointVector.push_back(newPoint);

			}
			else//점3개이상이면 중점5로
			{
				CollisionPointVector.push_back(ColPoint[4]);
			}


		}

		//모든 처리를 했는데 못찾은경우
		if (CollisionPointVector.size() == 0)
		{
			CollisionPoint cp;
			cp.pAxis = pAxis;
			cp.penetration = penetration;
			auto p = XMFloat3to4(pAxis);
			cp.Pos = Float4Add(p, *CenterPos);
			CollisionPointVector.push_back(cp);
		}


		//최종적인 pAxis
		float l = Float3Normalize(CenterL).x*axis[minindex].x + Float3Normalize(CenterL).y*axis[minindex].y + Float3Normalize(CenterL).z*axis[minindex].z;
		if (l > 0)
			pAxis = XMFLOAT3(-pAxis.x, -pAxis.y, -pAxis.z);

		for (int i = 0; i < CollisionPointVector.size(); i++)
		{
			CollisionPointVector[i].pAxis = pAxis;
			CollisionPointVector[i].penetration = penetration;
		}
		if (penetration > MMPE_EPSILON)//엡실론보다 크면 충돌 아니면 정말 미세한 충돌이므로 처리를 할 필요가 없음
			return true;
		else
			return false;

	}
	return false;

}


void MiniPhysicsEngineG9::RigidBody::CollisionResolve(RigidBody & rb2, XMFLOAT3 & CollisionN, double DeltaTime, float i1, float i2, float amendtime)
{
	ResolveVelocity(rb2, CollisionN, DeltaTime, i1, i2, amendtime);
	ResolvePenetration(rb2, DeltaTime);

}

float MiniPhysicsEngineG9::RigidBody::GetSeparateVelocity(RigidBody & rb2, XMFLOAT3 & CollisionN)
{
	return 0.0f;
}

void MiniPhysicsEngineG9::RigidBody::ResolveVelocity(RigidBody & rb2, XMFLOAT3 & CollisionN, double DeltaTime, float i1, float i2, float amendtime)
{
	//고정된 물체가아니면.
	if (rb2.GetMass() > MMPE_EPSILON)
	{
		AmendTime = amendtime;
		auto cn = Float4Add(GetPosition(), rb2.GetPosition(), false);
		cn = Float4Normalize(cn);
		cn = Float4Float(cn, i1);
		auto vn = rb2.GetVelocity();


		if (CollisionPointVector[0].Pos.y >= GetPosition().y - GetHalfBox().y / 2 && CollisionPointVector[0].Pos.y <= GetPosition().y + GetHalfBox().y / 2)
		{
			CollisionPointVector[0].Pos.y = GetPosition().y;
			vn.y = 0;

		}
		vn = Float3Normalize(vn);
		vn = Float3Float(vn, i2);
		AddForcePoint(XMFloat4to3(cn), CollisionPointVector[0].Pos, vn);
		integrate(0.01);
	}
}

void MiniPhysicsEngineG9::RigidBody::ResolvePenetration(RigidBody & rb2, double DeltaTime)
{
	if (CollisionPointVector[0].penetration < MMPE_EPSILON)//밀어낼 필요가 없는경우
		return;
	float totalInverseMass = GetMass();
	totalInverseMass += rb2.GetMass();

	if (totalInverseMass <= 0)//둘다 무한대일경우
		return;
	//물체를 pAxis방향으로 옮긴다. 다만 질량에 반비례하게 움직임. 질량이클수록 덜움직인다.
	XMFLOAT3 movevector(CollisionPointVector[0].pAxis.x*(CollisionPointVector[0].penetration / totalInverseMass), CollisionPointVector[0].pAxis.y*(CollisionPointVector[0].penetration / totalInverseMass), CollisionPointVector[0].pAxis.z*(CollisionPointVector[0].penetration / totalInverseMass));

	//충분히 작으면 움직이지 않는다.왜 엡실론을 안썻냐면 엡실론을쓸경우 penetration이 엡실론정도일때
	//밀려나야할경우 안밀려나는 상황이 일어나서, 엡실론보다 10배작은 값으로 검사한다.
	if (FloatLength(movevector) <= 0.0001)
		movevector = XMFLOAT3(0, 0, 0);

	XMFLOAT4 m1(movevector.x*GetMass(), movevector.y*GetMass(), movevector.z*GetMass(), 0);
	auto p1 = GetPosition();
	auto result1 = Float4Add(p1, m1);

	*CenterPos = result1;//자신을 옮긴다.


	XMFLOAT4 m2(-movevector.x*rb2.GetMass(), -movevector.y*rb2.GetMass(), -movevector.z*rb2.GetMass(), 0);
	auto pp2 = rb2.GetPosition();
	auto result2 = Float4Add(pp2, m2);
	*rb2.CenterPos = result2;//겹쳐진 대상을 옮긴다.


}

void MiniPhysicsEngineG9::RigidBody::SetE(float tempE)
{
	e = tempE;
}

float MiniPhysicsEngineG9::RigidBody::GetE()
{
	return e;
}


float MiniPhysicsEngineG9::RigidBody::CalculateImpulse(CollisionPoint& cp, RigidBody* rb2, double deltatime, float E)
{

	float finalE = -(1 + E);
	XMFLOAT3 SeparateVel = GetVelocity();
	XMFLOAT3 v2;
	if (rb2 != NULL)
		v2 = rb2->GetVelocity();
	else
		v2 = XMFLOAT3(0, 0, 0);
	SeparateVel = Float3Add(SeparateVel, v2, false);

	//Wr1 = 충돌전 각속도 X (Q-P)
	auto av = GetAngularVelocity();
	auto r1 = Float4Add(cp.Pos, *CenterPos, false);
	auto r2 = XMFloat4to3(r1);
	XMFLOAT3 Wr1 = Float3Cross(av, r2);
	//Wr2도 마찬가지.
	XMFLOAT3 Wr2;

	if (rb2 != NULL)
	{
		auto r3 = Float4Add(cp.Pos, *rb2->CenterPos, false);
		auto r4 = rb2->GetAngularVelocity();
		auto r5 = XMFloat4to3(r3);
		Wr2 = Float3Cross(r4, r5);
	}

	else
		Wr2 = XMFLOAT3(0, 0, 0);

	//1차식. -(1+e)(충돌전 상대속도 + (충돌전 각속도 X (Q-P) - 충돌전 각속도2 X (Q-P2) ) * 접촉방향 
	auto s1 = Float3Add(Wr1, Wr2, false);
	auto t1 = (Float3Add(SeparateVel, s1, true));
	t1.x *= cp.pAxis.x;
	t1.y *= cp.pAxis.y;
	t1.z *= cp.pAxis.z;
	float First = finalE * (t1.x + t1.y + t1.z);


	//temp1 = ( (Q-P1)X 접촉방향 );
	//u1 = ( ( temp1 * 역관성 1 ) X (Q-P1) ) * 접촉방향
	auto s2 = Float4Add(cp.Pos, *CenterPos, false);
	auto s3 = XMFloat4to3(s2);
	auto s4 = Float4Add(cp.Pos, *CenterPos, false);
	auto s5 = XMFloat4to3(s4);
	auto temp1 = Float3Cross(s3, cp.pAxis);
	XMVECTOR temp1v = XMLoadFloat3(&temp1);
	XMMATRIX ii1 = XMLoadFloat4x4(&GetIMoment());
	temp1v = XMVector3Transform(temp1v, ii1);
	XMStoreFloat3(&temp1, temp1v);
	temp1 = Float3Cross(temp1, s5);

	float u1 = temp1.x* cp.pAxis.x + temp1.y * cp.pAxis.y + temp1.z* cp.pAxis.z;

	//temp2 = ( (Q-P2)X 접촉방향 );
	//u2 = ( ( temp2 * 역관성 2 ) X (Q-P2) ) * 접촉방향

	float u2 = 0;
	if (rb2 != NULL)
	{
		auto q1 = Float4Add(cp.Pos, *rb2->CenterPos, false);
		auto q2 = XMFloat4to3(q1);
		auto temp2 = Float3Cross(q2, cp.pAxis);
		XMVECTOR temp2v = XMLoadFloat3(&temp2);
		XMMATRIX ii2 = XMLoadFloat4x4(&rb2->GetIMoment());
		temp2v = XMVector3Transform(temp2v, ii2);
		XMStoreFloat3(&temp2, temp2v);

		auto q3 = Float4Add(cp.Pos, *rb2->CenterPos, false);
		auto q4 = XMFloat4to3(q3);
		temp2 = Float3Cross(temp2, q4);

		u2 = temp2.x* cp.pAxis.x + temp2.y * cp.pAxis.y + temp2.z* cp.pAxis.z;
	}




	float inverseMs = GetMass();
	if (rb2 != NULL)
		inverseMs += rb2->GetMass();

	//식2. u1+u2 + inverseMs

	//float Second = u1 + u2 + inverseMs;

	//수정본. 이게더 자연스러움.
	float Second = inverseMs;

	//식3 = (식1 / 식2)*충격량을 가한 시간  = 최종 임펄스

	if (Second != 0)
		return abs((First / Second)*deltatime);
	else
		return 0;


}
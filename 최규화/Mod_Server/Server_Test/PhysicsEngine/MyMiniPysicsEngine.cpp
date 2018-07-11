#include "MyMiniPysicsEngine.h"
#include<math.h>
#include<algorithm>
using namespace std;

//�ش� ��ġ���� �ش�������� MaxLen�� �����Ҷ��� ���� ��ȯ��.
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

//ȸ����� ������ �ָ� �� ������ ȸ���ϴ� ���ʹϾ��� ��ȯ
XMFLOAT4 MiniPhysicsEngineG9::QuaternionRotation(XMFLOAT3 & Axis, float radian)
{


	Axis = Float3Normalize(Axis);
	XMFLOAT4 Result;
	//ȸ�����̾����� �������ʹϾ��� ��ȯ�ϵ�����.
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

	//�߷¿� ���� �ӵ��� �ʹ� ��������츦 ����� if���̴�.
	//-40���� �� �ʰ� �������� �״�� ����, -40�̻��� �ӵ��� -40���� �����Ѵ�.
	//tempV�� �̿��� ������ ���� �ӵ��� -40���� �����ع����� �������Ŀ� �ӵ��� �������� �������� �����̴�.
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
	//���� ���� �������� ���ӵ��� ����Ѵ�.

	//���ӵ� = ������ * ������ ����. ���ӵ��� �������������� ����� ��. F�� 0�̸� a�� 0
	//���� ���� �ѹ��� �������� ���ӵ��� �ѹ��� 0�̾ƴϰ� ���Ĵ� ��� 0�̴�.
	//�ٸ� �ӵ��� ���ӵ��� ���� ���� ����� ��� �����ϸ� ���������� ���� 0 �� �������.
	accel = totalforce * InverseMass;


	//�ӵ��� ��ġ�� ���ŵǾ��Ѵ�.

	//�ӵ��� ���ӵ��� ������ ������ ���Ѵ�.

	centerpos = centerpos + velocity * DeltaTime + 0.5*accel*DeltaTime*DeltaTime;

	//�ӵ��� �ٽ� �����ӵ��� �����ش�. �� tempV.y�� ����Ǿ����Ÿ� ������ -80���� �������ŷ� �ٲ�
	velocity = XMLoadFloat3(&Velocity);
	//���ӵ��� ���� �ӵ��� �߰��Ѵ�.
	velocity = velocity + accel * DeltaTime;

	// ���������� ���� �����Ѵ�.
	velocity *= powf(damping, DeltaTime);
	float e = 0;
	XMStoreFloat(&e, XMVector3Length(velocity));

	if (e <= MMPE_EPSILON)
		velocity = XMVectorZero();
	//��� ���� �����Ѵ�.
	XMStoreFloat4(CenterPos, centerpos);
	XMStoreFloat3(&Velocity, velocity);
	XMStoreFloat3(&Accel, accel);

	//��� ����� �������� ���� �ʱ�ȭ �Ѵ�.
	ForceClear();


	//���� ����� ������ �ӵ� ������ PhysicsPoint�� �������ִ� ������Ʈ�� ���������� �־�� �Ѵٸ�

	//if (ObjPos != NULL)//���� ���ӿ�����Ʈ�� ���� ������ �ִ� ���� ������ ������
	//	*ObjPos = XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z, ObjPos->w);
	//if (ObjVel != NULL)//���� ���ӿ�����Ʈ�� ���� ������ �ִ� �ӵ� ������ ������
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
	//���� �� ������Ʈ�� �˻��Ѵ�.
	float L = FloatLength(Float4Add(*CenterPos, *p2.CenterPos, false));
	if (GetRad() + p2.GetRad() > L)
	{
		//15�� �˻縦 �ؾ��ϸ�, �� ���� ������ ����������.
		XMFLOAT3 axis[15];
		//1�� ������ x,y,z��
		axis[0] = r1;
		axis[1] = u1;
		axis[2] = l1;

		//2�� ������ x,y,z��
		axis[3] = r2;
		axis[4] = u2;
		axis[5] = l2;

		//1���� 2�� �� ��� ���� ����
		axis[6] = Float3Cross(r1, r2);
		axis[7] = Float3Cross(r1, u2);
		axis[8] = Float3Cross(r1, l2);

		axis[9] = Float3Cross(u1, r2);
		axis[10] = Float3Cross(u1, u2);
		axis[11] = Float3Cross(u1, l2);

		axis[12] = Float3Cross(l1, r2);
		axis[13] = Float3Cross(l1, u2);
		axis[14] = Float3Cross(l1, l2);
		XMFLOAT3 CenterL;//�������� ������ ����
						 //�ּҷ� ��ġ�� �κ��� ã�ƾ���. �װ��� ��ġ�� �ּҿ����� ����

		float minoverlap = INFINITY;
		//�ּҰ�ġ�� �������� �о���� ������ �ε���
		int minindex;
		//15�� �ݺ���
		for (int i = 0; i < 15; i++)
		{
			XMFLOAT3 a = axis[i];
			if (FloatLength(a) < 0.001)//������ ����� 0�̸�(�ε��Ҽ������������� 0��������) ��ǻ� 0�̹Ƿ� �˻�X
				continue;

			a = Float3Normalize(a);
			CenterL = XMFLOAT3(p2.GetPosition().x - GetPosition().x, p2.GetPosition().y - GetPosition().y, p2.GetPosition().z - GetPosition().z);

			float overlap = penetrationAxis(p2, l1, r1, u1, l2, r2, u2, CenterL, a);
			if (overlap < 0)//�ϳ��� ��ġ�� �κ��� ������ �и��Ǿ�����
				return false;
			if (overlap < minoverlap)
			{
				minoverlap = overlap;
				minindex = i;
			}
		}

		//���� ���� ������ �浹�Ѱ���
		penetration = minoverlap;
		pAxis = axis[minindex];
		//������ �������Ϳ� ���õ� ���Ͱ� ������ 0���� ũ�� �ݴ�������� �ٲ������. �ֳ��ϸ� �и��� ������ �������ؾ��ϹǷ�

		float l = Float3Normalize(CenterL).x*axis[minindex].x + Float3Normalize(CenterL).y*axis[minindex].y + Float3Normalize(CenterL).z*axis[minindex].z;
		if (l > 0)
			pAxis = XMFLOAT3(-pAxis.x, -pAxis.y, -pAxis.z);

		//return true;
		if (penetration > MMPE_EPSILON)//���Ƿк��� ũ�� �浹 �ƴϸ� ���� �̼��� �浹�̹Ƿ� ó���� �� �ʿ䰡 ����
			return true;
		else
			return false;
	}
	return false;
}

void MiniPhysicsEngineG9::PhysicsPoint::CollisionResolve(PhysicsPoint & p2, XMFLOAT3 & CollisionN, double DeltaTime)
{
	//�浹�� �ӵ��� �����.
	ResolveVelocity(p2, CollisionN, DeltaTime);

	//�ӵ������ ��ġ�� �κ��� �о
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
	//�浹�� �ӵ� ��ȭ�� ������ ����.
	//1. �浹�� �и��ӵ��� ���ϰ�
	//2. �浹 �� �и��ӵ��� ���Ѵ� Vf=-eVi ��.
	//3. ��ݷ� ũ�⸦ ����Ѵ�. W= M(Vf-Vi)
	//4. ��ݷ��� ����Ѵ�. ��ݷ� ũ�� * CollisionN�̴�. �⺻������ CollisionN�� ������ġ-������ġ�� ���ȭ�Ѱ�.
	//5. ���� �Ǽӵ� = ���� �Ǽӵ� + ��ݷ�(MV)/M1 �Ǵ� M2 �� �̷��� �ǳĸ� ���������� �����̴� ũ��� ���� ������ �ݺ��
	//�ؾ��ϱ� �����̴�. ������� M=Inverse(Inverse(M1)+Inverse(M2)) �̰�, M1=1 M2=9������ M=9/10 �� ���´�.
	//�̰� ����ϸ� M1�� �༮�� 0.9��ŭ �и��� M2�� 0.1 ��ŭ �и���.
	//�ٸ� Ű���� �Է½� �з������ϴ¹����� ���� Ű���� ���� Ű�� �ݴ������ �ӵ���.


	float sv = GetSeparateVelocity(p2, CollisionN);
	if (sv > 0)//�̹� �־���������
		return;
	float nsv = -sv * 1;//���� ���⿡ 1��� ź����� e�� ���µ�, �츮�� ����� ������ ����������Ʈ�� e�� 1�̴�.
	float deltav = nsv - sv;

	float totalImass = GetMass();
	totalImass += p2.GetMass();
	if (totalImass <= 0)
		return;
	float impurse = deltav / totalImass;

	XMFLOAT3 impursevector(CollisionN.x*impurse, CollisionN.y*impurse, CollisionN.z*impurse);

	//���� ��ü�� ��ȭ �� ���� �ӵ�
	auto resultv = GetVelocity();
	XMFLOAT3 iv1(impursevector.x*GetMass(), impursevector.y*GetMass(), impursevector.z*GetMass());
	resultv = Float3Add(resultv, iv1);
	SetVelocity(resultv);


	//���� �ε��� ��ü�� �����ӵ�

	auto resultv2 = p2.GetVelocity();
	XMFLOAT3 iv2(-impursevector.x*p2.GetMass(), -impursevector.y*p2.GetMass(), -impursevector.z*p2.GetMass());
	resultv2 = Float3Add(resultv2, iv2);
	p2.SetVelocity(resultv2);

	if (Bounce == false)
	{
		//ƨ�ܳ����⸦ ���� �ʴ´ٸ� ������ ���� ������ �ʿ��ϴ�.
		//1. ���� bounce false�� ���� �ӵ��� 0�̴�.
		//2. ���� �ӵ��� �׳� ����� ��� �д�.

		auto gv = GetVelocity();


		//�ٿ�� false�� ƨ�ܳ����� �ʵ��� 0���� �����.
		gv = XMFLOAT3(0, 0, 0);
		SetVelocity(gv);

	}
	//���浵 Bounce�� false�� ƨ�ܳ����� �ʵ�����.
	if (p2.GetBounce() == false)
	{
		auto gv2 = p2.GetVelocity();

		gv2 = XMFLOAT3(0, 0, 0);
		p2.SetVelocity(gv2);
	}




}
//��������� ���θ� �о. Ű���� �Է��϶� �ڽŸ� �з���
//�����϶��� �� ������ �����̸� ������ �и������� �ֳ��ϸ� �̰� �����ΰ�
//���߻��·� ���������� �߷¸� ������ �ްԵǹ�s�� Ű���� �Է��� �ƴϰԵ� �̶� �浹�� �Ͼ��
//�밢������ ���� �о�� �ǰ� �̷��� �Ǹ� y���� �����ϸ� x�� z���� 0�̾ƴϰ� �Ǹ鼭 �и��°�.
//���� �̰� ������ �ȵ�� x�� z�� �ӵ��� 0�̸� �з����� �ʵ��� �ϴ� ����� ����.
//���� �̰� �� ������ ���ĸ� �ƿ� ������ �����ִ� ������ �ȿ����̰� ������ �ȶ������.
//���� �׳� ���߿����� �о�� ��� ������ �ϴ°� ������ ����.
void MiniPhysicsEngineG9::PhysicsPoint::ResolvePenetration(PhysicsPoint & p2, double DeltaTime)
{
	if (penetration < MMPE_EPSILON)//�о �ʿ䰡 ���°��
		return;
	float totalInverseMass = GetMass();
	totalInverseMass += p2.GetMass();

	if (totalInverseMass <= 0)//�Ѵ� ���Ѵ��ϰ��
		return;
	//��ü�� pAxis�������� �ű��. �ٸ� ������ �ݺ���ϰ� ������. ������Ŭ���� �������δ�.
	XMFLOAT3 movevector(pAxis.x*(penetration / totalInverseMass), pAxis.y*(penetration / totalInverseMass), pAxis.z*(penetration / totalInverseMass));

	//����� ������ �������� �ʴ´�.�� ���Ƿ��� �ț��ĸ� ���Ƿ�������� penetration�� ���Ƿ������϶�
	//�з������Ұ�� �ȹз����� ��Ȳ�� �Ͼ��, ���Ƿк��� 10������ ������ �˻��Ѵ�.
	if (FloatLength(movevector) <= 0.0001)
		movevector = XMFLOAT3(0, 0, 0);

	XMFLOAT3 m1(movevector.x*GetMass(), movevector.y*GetMass(), movevector.z*GetMass());
	auto p1 = GetPosition();
	auto result1 = Float4Add(p1, XMFloat3to4(m1));

	*CenterPos = result1;

	//SetPosition(result1);//�ڽ��� �ű��.


	XMFLOAT3 m2(-movevector.x*p2.GetMass(), -movevector.y*p2.GetMass(), -movevector.z*p2.GetMass());
	auto pp2 = p2.GetPosition();
	auto result2 = Float4Add(pp2, XMFloat3to4(m2));
	*p2.CenterPos = result2;//������ ����� �ű��.

	//auto result2 = Float3Add(pp2, m2);
	//p2.SetPosition(result2);//������ ����� �ű��.



}

//�ϳ��� ���� �������� �� �ڽ��� ������ ����� �ջ����� ������ �Ÿ����͸� ���� ������ ������ ����� ������ 0���� ũ�� ��ġ�������� 0���� ������ �и��Ǿ��ִ�.
float MiniPhysicsEngineG9::PhysicsPoint::penetrationAxis(PhysicsPoint & p2, XMFLOAT3 & l1, XMFLOAT3 & r1, XMFLOAT3 & u1, XMFLOAT3 & l2, XMFLOAT3 & r2, XMFLOAT3 & u2, XMFLOAT3 & CenterLength, XMFLOAT3& Axis)
{
	float prj1;
	float prj2;
	float dst;

	//������� x��y��z���� ������ ����� �� ���Ѵ�.

	prj1 = GetHalfBox().x*fabsf(Axis.x*r1.x + Axis.y*r1.y + Axis.z*r1.z) + GetHalfBox().y*fabsf(Axis.x*u1.x + Axis.y*u1.y + Axis.z*u1.z) + GetHalfBox().z*fabsf(Axis.x*l1.x + Axis.y*l1.y + Axis.z*l1.z);
	prj2 = p2.GetHalfBox().x*fabsf(Axis.x*r2.x + Axis.y*r2.y + Axis.z*r2.z) + p2.GetHalfBox().y*fabsf(Axis.x*u2.x + Axis.y*u2.y + Axis.z*u2.z) + p2.GetHalfBox().z*fabsf(Axis.x*l2.x + Axis.y*l2.y + Axis.z*l2.z);
	dst = fabsf(CenterLength.x*Axis.x + CenterLength.y*Axis.y + CenterLength.z*Axis.z);

	//�̰���� ����� ��ġ�� ������ �и��Ǿ��ִ�.
	return prj1 + prj2 - dst;

}

//����ĳ���� ������Ʈ�� ������ ���۵Ǵ� ī�޶� ��ġ�� ������ ����, ����ĳ��Ʈ������Ʈ�� ���� ������Ʈ�� ����,ī�޶�
//������ �÷��̾� ������Ʈ�� ����, �׸��� ������ ����� XMFLOAT3�� �Է¹޾ƾ� �Ѵ�.
//����� ��ȯ�ɶ� true�� �ش� ������Ʈ�� ������ �浹�� �Ǿ����� ������ �浹�Ҷ� ���� ����� ����� ��ġ��
//SavePos�� ����ȴ�.

//������� 3���� ������Ʈ�� RayCasting�� ȣ���ؼ� 3�� ��� �浹������, ���� ���尡��� SavePos�� ���³���
//���������� �浹�� �༮�ϰ��̴�. �̰��� ����ĳ��Ʈ �ý��ۿ��� ó���ؾ��Ѵ�.
//
//bool MiniPhysicsEngineG9::RayCastObject::RayCasting(XMFLOAT3 & RayOrgin, XMFLOAT3 & RayDir, XMFLOAT3& ObjCenterPos, XMFLOAT3& PlayerPos,XMFLOAT3& Look, XMFLOAT3 * SavePos)
////ObjCenterPos�� �ش� �˻� ��������Ʈ�� �����̰�, PlayerPos�� ī�޶� ������ �÷��̾������Ʈ�� ��ġ��.
//{
//	//�츮�� t�� ���� ����� min���� ������ ���� ���� ����̹Ƿ� �ʿ��ϴ�.
//	float mint = INFINITY;
//	
//	
//
//	XMVECTOR objpos = XMLoadFloat3(&ObjCenterPos);
//	XMVECTOR playerpos = XMLoadFloat3(&PlayerPos);
//	XMVECTOR Rd = XMLoadFloat3(&RayDir);
//	XMVECTOR Ro = XMLoadFloat3(&RayOrgin);
//	XMVECTOR look = XMLoadFloat3(&Look);
//	//���� ������Ʈ�� �÷��̾� �ڿ������� �˻� ��󿡼� �����.
//
//
//	auto frontV=XMVector3Dot(look, XMVector3Normalize(objpos - playerpos));
//
//	float isFront = 0;
//	XMStoreFloat(&isFront, frontV);
//	if (isFront < 0)
//		return false;
//
//	//�÷��̾ �˻����̸� 6���� �˻��Ѵ�.
//
//	for (int i = 0; i < 6; i++)//6���� �˻��Ѵ�.
//	{
//		XMVECTOR N = XMLoadFloat3(&Plane[i].Normal);
//		
//		XMFLOAT3 NdotRayDir;
//		XMStoreFloat3(&NdotRayDir, XMVector3Dot(N, Rd));
//		if (NdotRayDir.x == 0)//������ ����� ����� �����̸� ������ ����(������ ���ų� ���ų�)
//		{
//			continue;
//		}
//
//		//���⿡ ���� �켱 ������ �ִٴ� �Ҹ�
//
//		//�̸� ���� t�� ���ؾ���. �������� ������ ���� X = RayOrgin + RayDir*t
//		//����� �� ���������� Plane[i].Normal*(X-(Plane[i].DefaultCenterPos+ObjCenterPos))=0
//		//Plane[i].Normal*(RayOrgin+RayDir*t - (Plane[i].DefaultCenterPos+ObjCenterPos))=0
//		//Plane[i].Normal*(RayOrgin+RayDir*t)-Plane[i].Normal*(Plane[i].DefaultCenterPos+ObjCenterPos)=0
//		//t={((Plane[i].DefaultCenterPos+ObjCenterPos) - RayOrgin)/Plane[i].Normal ��Ʈ RayDir}  ��Ʈ Plane[i].Normal
//		XMVECTOR defaultpos = XMLoadFloat3(&Plane[i].DefaultCenterPos);
//		
//		auto sik1 = ((defaultpos + objpos) - Ro) / NdotRayDir.x;
//		XMFLOAT3 temp;
//		XMStoreFloat3(&temp, XMVector3Dot(sik1, N));
//
//		float t = temp.x;
//		
//		//���� t�� �̿��� ����X�� ������� �ȿ� �ִ��� �˻��Ѵ�.
//		//���� �������ȿ������鼭 dir�� testP-playerpos�� ������ 0���� ũ�ų� ������ 
//		//mint�� �����Ѵ�.SavePos�� �����Ѵ�. �ƴ϶�� continue
//
//		auto X = Ro + Rd * t;
//		//X�� 6���� ��鿡�� �ϳ��� 0���� ū�� ������ �ٱ����ִ°��̸� �浹���Ѱ��̴�.
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
//			if (result.x > 0)//0���� ũ�� �ٿ���ڽ� �ȿ� ����.
//			{
//				In = false;
//				break;
//			}			
//
//		}
//		//������� ���� t�� SavePos�� ��ȿ�� ������ ������ �����̴�.
//		//�ٸ� �� ������ ���������� ���ŵǱ� ���ؼ�
//		//1. ���� �������� ī�޶� ������� �ϰ�
//		//2. �÷��̾��� ������Ʈ �տ� �����ؾ��Ѵ�.
//		//�� �÷��̾�� ī�޶� �������ִ� 3��Ī ���ӿ��� �÷��̾� ���� ī�޶� ������ �ִ� ���
//		//�÷��̾�� ���� ���� �����׹Ƿ�, �ڷ� ź�� ���ư��� �ȵǱ� ������
//		//���Ͱ� �÷��̾� �տ� �������� ����ĳ������ �����ؾ��Ѵ�.
//		//�̸� ���� ������ ���ߴ� ���(defaultpos+objpos)�� ��ġ���� �÷��̾� ��ġ�� �� ���Ϳ�
//		//ī�޶� �ٶ󺸰��ִ� ����(���� ������ ���ϸ� �÷��̾��� Look���� �ؾ��Ѵ�.)�� ������
//		//0�̻��̰�, t�� ī�޶� �� ������ �� ������ �����Ѵ�.
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
//ObjCenterPos�� �ش� �˻� ��������Ʈ�� �����̰�, PlayerPos�� ī�޶� ������ �÷��̾������Ʈ�� ��ġ��.
{
	//�츮�� t�� ���� ����� min���� ������ ���� ���� ����̹Ƿ� �ʿ��ϴ�.
	float mint = INFINITY;



	XMVECTOR objpos = XMLoadFloat3(&ObjCenterPos);
	XMVECTOR playerpos = XMLoadFloat3(&PlayerPos);
	XMVECTOR Rd = XMLoadFloat3(&RayDir);
	XMVECTOR Ro = XMLoadFloat3(&RayOrgin);
	XMVECTOR look = XMLoadFloat3(&Look);
	//���� ������Ʈ�� �÷��̾� �ڿ������� �˻� ��󿡼� �����.


	auto frontV = XMVector3Dot(look, XMVector3Normalize(objpos - playerpos));

	float isFront = 0;
	XMStoreFloat(&isFront, frontV);
	if (isFront < 0)
		return false;

	//�÷��̾ �˻����̸� 6���� �˻��Ѵ�.

	for (int i = 0; i < 6; i++)//6���� �˻��Ѵ�.
	{
		XMVECTOR N = XMLoadFloat3(&Plane[i].Normal);

		XMFLOAT3 NdotRayDir;
		XMStoreFloat3(&NdotRayDir, XMVector3Dot(N, Rd));
		if (NdotRayDir.x == 0)//������ ����� ����� �����̸� ������ ����(������ ���ų� ���ų�)
		{
			continue;
		}

		//���⿡ ���� �켱 ������ �ִٴ� �Ҹ�

		//�̸� ���� t�� ���ؾ���. �������� ������ ���� X = RayOrgin + RayDir*t
		//����� �� ���������� Plane[i].Normal*(X-(Plane[i].DefaultCenterPos+ObjCenterPos))=0
		//Plane[i].Normal*(RayOrgin+RayDir*t - (Plane[i].DefaultCenterPos+ObjCenterPos))=0
		//Plane[i].Normal*(RayOrgin+RayDir*t)-Plane[i].Normal*(Plane[i].DefaultCenterPos+ObjCenterPos)=0
		//t={((Plane[i].DefaultCenterPos+ObjCenterPos) - RayOrgin)/Plane[i].Normal ��Ʈ RayDir}  ��Ʈ Plane[i].Normal
		XMVECTOR defaultpos = XMLoadFloat3(&Plane[i].DefaultCenterPos);

		auto sik1 = ((defaultpos + objpos) - Ro) / NdotRayDir.x;
		XMFLOAT3 temp;
		XMStoreFloat3(&temp, XMVector3Dot(sik1, N));

		float t = temp.x;

		//���� t�� �̿��� ����X�� ������� �ȿ� �ִ��� �˻��Ѵ�.
		//���� �������ȿ������鼭 dir�� testP-playerpos�� ������ 0���� ũ�ų� ������ 
		//mint�� �����Ѵ�.SavePos�� �����Ѵ�. �ƴ϶�� continue

		auto X = Ro + Rd * t;
		//X�� 6���� ��鿡�� �ϳ��� 0���� ū�� ������ �ٱ����ִ°��̸� �浹���Ѱ��̴�.
		bool In = true;

		for (int j = 0; j < 6; j++)
		{
			XMVECTOR testN = XMLoadFloat3(&Plane[j].Normal);
			XMVECTOR testP = XMLoadFloat3(&Plane[j].DefaultCenterPos);
			auto testv = XMVector3Dot(testN, (X - (testP + objpos)));
			XMFLOAT3 result;
			XMStoreFloat3(&result, testv);

			if (result.x > 0)//0���� ũ�� �ٿ���ڽ� �ȿ� ����.
			{
				In = false;
				break;
			}

		}
		//������� ���� t�� SavePos�� ��ȿ�� ������ ������ �����̴�.
		//�ٸ� �� ������ ���������� ���ŵǱ� ���ؼ�
		//1. ���� �������� ī�޶� ������� �ϰ�
		//2. �÷��̾��� ������Ʈ �տ� X�� �����ؾ��Ѵ�.
		//�� �÷��̾�� ī�޶� �������ִ� 3��Ī ���ӿ��� �÷��̾� ���� ī�޶� ������ �ִ� ���
		//�÷��̾�� ���� ���� �����׹Ƿ�, �ڷ� ź�� ���ư��� �ȵǱ� ������
		//���Ͱ� �÷��̾� �տ� �������� ����ĳ������ �����ؾ��Ѵ�.
		//�̸� ���� ������ ��ġ���� �÷��̾� ��ġ�� �� ���Ϳ�
		//ī�޶� �ٶ󺸰��ִ� ����(���� ������ ���ϸ� �÷��̾��� Look���� �ؾ��Ѵ�.)�� ������
		//0�̻��̰�, t�� ī�޶� �� ������ �� ������ �����Ѵ�.

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
	//���� ������Ʈ�� �÷��̾� �ڿ������� �˻� ��󿡼� �����.


	//���� ������ �����ϴ� ����̴�.


	XMVECTOR N = XMVectorSet(0, 1, 0, 0);

	XMFLOAT3 NdotRayDir;
	XMStoreFloat3(&NdotRayDir, XMVector3Dot(N, Rd));
	if (NdotRayDir.x == 0)//������ ����� ����� �����̸� ������ ����(������ ���ų� ���ų�)
	{
		return false;
	}

	//���⿡ ���� �켱 ������ �ִٴ� �Ҹ�

	//�̸� ���� t�� ���ؾ���. �������� ������ ���� X = RayOrgin + RayDir*t
	//����� �� ���������� Plane[i].Normal*(X-(Plane[i].DefaultCenterPos+ObjCenterPos))=0
	//Plane[i].Normal*(RayOrgin+RayDir*t - (Plane[i].DefaultCenterPos+ObjCenterPos))=0
	//Plane[i].Normal*(RayOrgin+RayDir*t)-Plane[i].Normal*(Plane[i].DefaultCenterPos+ObjCenterPos)=0
	//t={((Plane[i].DefaultCenterPos+ObjCenterPos) - RayOrgin)/Plane[i].Normal ��Ʈ RayDir}  ��Ʈ Plane[i].Normal


	auto sik1 = (-Ro) / NdotRayDir.x;
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, XMVector3Dot(sik1, N));

	float t = temp.x;

	//���� t�� �̿��� ����X�� ������� �ȿ� �ִ��� �˻��Ѵ�.
	//���� �������ȿ������鼭 dir�� testP-playerpos�� ������ 0���� ũ�ų� ������ 
	//mint�� �����Ѵ�.SavePos�� �����Ѵ�. �ƴ϶�� continue

	auto X = Ro + Rd * t;
	//X�� 6���� ��鿡�� �ϳ��� 0���� ū�� ������ �ٱ����ִ°��̸� �浹���Ѱ��̴�.

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
	//0 ���� 1 ������ 2 ���� 3 ���� 4 �Ʒ��� 5 ����
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
	ga *= pp.GetMass(false);//�߷� = �߷°��ӵ� * ����
	XMFLOAT3 gravityForce;
	XMStoreFloat3(&gravityForce, ga);

	pp.AddForce(gravityForce);
}
void MiniPhysicsEngineG9::GeneratorGravity::Update(float DeltaTime, RigidBody & rb)
{
	XMVECTOR ga = XMLoadFloat3(&GravityAccel);
	ga *= rb.GetMass(false);//�߷� = �߷°��ӵ� * ����
	XMFLOAT3 gravityForce;
	XMStoreFloat3(&gravityForce, ga);

	rb.AddForce(gravityForce);
}

//������Ҷ� -k�ҰŴϱ� �׳� k�� ������!!!
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

	//F=K(defaultlength-curlen); �� -���Ⱥٿ��ĸ� ���� ��ŸX���Ҷ� ������� - �⺻������. �ٵ� �ݴ�� �ؼ� -1���ѰŶ�����
	float F = constantspring * (DefaultLength - curlen);

	//���� ���� ����ȭ ��Ų��. �� obj_anc�� ��ֶ������ؼ� ���������ϰ� F�� ũ��� �Ѵ�.

	obj_anc = XMVector3Normalize(obj_anc);
	obj_anc *= F;
	XMFLOAT3 FinalForce;
	XMStoreFloat3(&FinalForce, obj_anc);
	pp.AddForce(FinalForce);

}

void MiniPhysicsEngineG9::GeneratorJump::Update(float DeltaTime, PhysicsPoint & pp)
{
	//������ ���� ���ϱ⺸�� �ӵ��� ���� ���Ѵ�. �ֳ��ϸ� ���� ���Ұ�� ���� ���� ���������ϰ� �ʿ��ϴ�.
	//�ֳ��ϸ� ���� ���������� �������� ��������, �����̽��� �������� ���ѹ� �ߵ��ǰ� �Ϸ��� 10000�̻� ����Ѵ�.
	//���� �׳� ����ϰ� �ӵ��� �����ؼ� �ش�. �̷��� �ӵ��� �������̹Ƿ� �����ϰ� ������ �ȴ�.
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
	//������ 0 ���ϸ� ������ ���̹Ƿ� ������ �ʿ䰡 ����.
	if (InverseMass <= 0.0f)
		return;

	assert(DeltaTime >= 0.0);

	XMVECTOR centerpos = XMLoadFloat4(CenterPos);

	XMVECTOR velocity = XMLoadFloat3(&Velocity);

	XMVECTOR accel = XMLoadFloat3(&Accel);

	XMVECTOR totalforce = XMLoadFloat3(&TotalForce);

	//���� ���� �������� ���ӵ��� ����Ѵ�.

	//���ӵ� = ������ * ������ ����. ���ӵ��� �������������� ����� ��. F�� 0�̸� a�� 0
	//���� ���� �ѹ��� �������� ���ӵ��� �ѹ��� 0�̾ƴϰ� ���Ĵ� ��� 0�̴�.

	//�ٸ� �ӵ��� ���ӵ��� ���� ���� ����� ��� �����ϸ� ���������� ���� 0 �� �������.


	//���ӵ� ���ϰ� �ӵ� ���ϰ� ������ ���ӽ�Ų�� ��ġ�� ���Ѵ�.

	accel = totalforce * InverseMass;

	//���ӵ��� ���� �ӵ��� �߰��Ѵ�.
	velocity = velocity + accel * DeltaTime;



	// ���������� ���� �����Ѵ�.
	velocity *= powf(damping, DeltaTime);

	//���ӽ�Ų�� ���Ƿ� ������ �ӵ��� 0���� ����.
	float e = 0;
	XMStoreFloat(&e, XMVector3Length(velocity));
	if (e <= MMPE_EPSILON)
		velocity = XMVectorZero();

	//�ӵ��� ���ӵ��� ������ ������ ���Ѵ�.

	centerpos = centerpos + velocity * DeltaTime + 0.5*accel*DeltaTime*DeltaTime;



	//��� ���� �����Ѵ�.
	XMStoreFloat4(CenterPos, centerpos);
	XMStoreFloat3(&Velocity, velocity);
	XMStoreFloat3(&Accel, accel);

	//���� ������ �����Ѵ�.
	XMVECTOR Avelocity = XMLoadFloat3(&AngularVelocity);
	XMVECTOR totaltorque = XMLoadFloat3(&TotalTorque);
	XMVECTOR orient = XMLoadFloat4(Orient);
	XMMATRIX IT = XMLoadFloat4x4(&GetIMoment());

	//�����ӵ� ���ϰ� ���ӵ� ���ϰ� ������ ���ӽ�Ų�� ������ ���Ѵ�.

	// A = T/I == T * Inverse I

	XMVECTOR Aaccel = XMVector4Transform(totaltorque, IT);

	//���ӵ��� ����.
	Avelocity += Aaccel * DeltaTime;

	// ���������� ���� �����Ѵ�.
	Avelocity *= powf(Angulardamping, DeltaTime);

	//���ӽ�Ų�� ���Ƿ� ������ �ӵ��� 0���� ����.
	float av = 0;
	XMStoreFloat(&av, XMVector3Length(Avelocity));
	if (av <= MMPE_EPSILON)
		Avelocity = XMVectorZero();



	//������ ������ ����. orient = orient + 0.5*W*orient. �̶� W*orient�� ���ʹϾ� ��.

	orient = orient + 0.5f*XMQuaternionMultiply(Avelocity, orient)*DeltaTime;

	orient = XMQuaternionNormalize(orient);

	//��� ���� �����Ѵ�.
	XMStoreFloat4(Orient, orient);
	XMStoreFloat3(&AngularVelocity, Avelocity);

	//��� ����� �������� ������ũ�� �ʱ�ȭ �Ѵ�.
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
	XMVECTOR dt = XMMatrixDeterminant(ii);//��Ľ�

	ii = XMMatrixInverse(&dt, ii);

	XMStoreFloat4x4(&Inverse_I_Moment, ii);
}

//����ü�� �������Ʈ ����.
void MiniPhysicsEngineG9::RigidBody::SetIMoment(float x, float y, float z)
{
	XMFLOAT4X4 i{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	i._11 = (GetMass(false)*(z*z + y * y)) / 12;
	i._22 = (GetMass(false)*(z*z + x * x)) / 12;
	i._33 = (GetMass(false)*(x*x + y * y)) / 12;

	XMMATRIX ii = XMLoadFloat4x4(&i);
	XMVECTOR dt = XMMatrixDeterminant(ii);//��Ľ�

	ii = XMMatrixInverse(&dt, ii);

	XMStoreFloat4x4(&Inverse_I_Moment, ii);
}

XMFLOAT4X4 MiniPhysicsEngineG9::RigidBody::GetIMoment(bool Inverse)
{
	if (Inverse)
	{
		XMMATRIX ii = XMLoadFloat4x4(&Inverse_I_Moment);
		XMVECTOR dt = XMMatrixDeterminant(ii);//��Ľ�
		float dtf = 0;
		XMStoreFloat(&dtf, dt);
		if (dtf <= MMPE_EPSILON)
			int a = 3;
		ii = XMMatrixInverse(&dt, ii);//���̾ƴ� �������� �������Ʈ

		XMVECTOR orient = XMLoadFloat4(Orient); // ��ü�� ȸ���Ѹ�ŭ ȸ����Ų��.
		XMMATRIX orientmatrix = XMMatrixRotationQuaternion(orient);
		ii *= orientmatrix;//ii�� ����Ʈ �������Ʈ * �������ʹϾ����

						   //�ٽ� ����ķ� ��ȯ
		dt = XMMatrixDeterminant(ii);//��Ľ�
		ii = XMMatrixInverse(&dt, ii);
		XMFLOAT4X4 temp;
		XMStoreFloat4x4(&temp, ii);
		return temp;
	}
	else
	{
		XMMATRIX ii = XMLoadFloat4x4(&Inverse_I_Moment);
		XMVECTOR dt = XMMatrixDeterminant(ii);//��Ľ�

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
//F2�� ��ü�� �̵��� �������� �������ϴ°�
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
		t = Float3Cross(p, F);//��ũ = �������κ��� ���� ������ ���� X ���� ����
	else
		t = Float3Cross(p, F2);//��ũ = �������κ��� ���� ������ ���� X ���� ����

	if (fabsf(t.x) <= MMPE_EPSILON / 10)
		t.x = 0;
	if (fabsf(t.y) <= MMPE_EPSILON / 10)
		t.y = 0;
	if (fabsf(t.z) <= MMPE_EPSILON / 10)
		t.z = 0;

	AddForce(F);
	AddTorque(t);
}

//F2�� ��ü�� �̵��� �������� �������ϴ°�
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
		t = Float3Cross(p1, F);//��ũ = �������κ��� ���� ������ ���� X ���� ����
	else
		t = Float3Cross(p1, F2);//��ũ = �������κ��� ���� ������ ���� X ���� ����

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

	//������� x��y��z���� ������ ����� �� ���Ѵ�.

	prj1 = GetHalfBox().x*fabsf(Axis.x*r1.x + Axis.y*r1.y + Axis.z*r1.z) + GetHalfBox().y*fabsf(Axis.x*u1.x + Axis.y*u1.y + Axis.z*u1.z) + GetHalfBox().z*fabsf(Axis.x*l1.x + Axis.y*l1.y + Axis.z*l1.z);
	prj2 = rb2.GetHalfBox().x*fabsf(Axis.x*r2.x + Axis.y*r2.y + Axis.z*r2.z) + rb2.GetHalfBox().y*fabsf(Axis.x*u2.x + Axis.y*u2.y + Axis.z*u2.z) + rb2.GetHalfBox().z*fabsf(Axis.x*l2.x + Axis.y*l2.y + Axis.z*l2.z);
	dst = fabsf(CenterLength.x*Axis.x + CenterLength.y*Axis.y + CenterLength.z*Axis.z);

	//�̰���� ����� ��ġ�� ������ �и��Ǿ��ִ�.
	return prj1 + prj2 - dst;

}
bool MiniPhysicsEngineG9::RigidBody::CollisionTest(RigidBody & rb2, XMFLOAT3 & l1, XMFLOAT3 & r1, XMFLOAT3 & u1, XMFLOAT3 & l2, XMFLOAT3 & r2, XMFLOAT3 & u2)
{

	//���� �� ������Ʈ�� �˻��Ѵ�.
	float L = FloatLength(XMFloat4to3(Float4Add(*CenterPos, *rb2.CenterPos, false)));
	if (GetRad() + rb2.GetRad() > L)
	{
		CollisionPointVector.clear();

		RigidBody* Testing = NULL;//�����˻� ���
		RigidBody* Other = NULL;//�ƴѳ༮

								//15�� �˻縦 �ؾ��ϸ�, �� ���� ������ ����������.
		XMFLOAT3 axis[15];
		//1�� ������ x,y,z��
		axis[0] = r1;
		axis[1] = u1;
		axis[2] = l1;

		//2�� ������ x,y,z��
		axis[3] = r2;
		axis[4] = u2;
		axis[5] = l2;

		//1���� 2�� �� ��� ���� ����
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

		XMFLOAT3 CenterL;//�������� ������ ����
						 //�ּҷ� ��ġ�� �κ��� ã�ƾ���. �װ��� ��ġ�� �ּҿ����� ����

		float minoverlap = INFINITY;
		//�ּҰ�ġ�� �������� �о���� ������ �ε���
		int minindex;
		CenterL = XMFLOAT3(rb2.GetPosition().x - GetPosition().x, rb2.GetPosition().y - GetPosition().y, rb2.GetPosition().z - GetPosition().z);

		//0~5�� ���� minindex;
		int minSingleAxis = 0;
		//15�� �ݺ���
		for (int i = 0; i < 15; i++)
		{

			XMFLOAT3 a = axis[i];
			if (FloatLength(a) < 0.001)//������ ����� 0�̸�(�ε��Ҽ������������� 0��������) ��ǻ� 0�̹Ƿ� �˻�X
				continue;

			a = Float3Normalize(a);

			float overlap = penetrationAxis(rb2, l1, r1, u1, l2, r2, u2, CenterL, a);
			if (overlap < 0)//�ϳ��� ��ġ�� �κ��� ������ �и��Ǿ�����
				return false;
			if (overlap < minoverlap)
			{
				minoverlap = overlap;
				minindex = i;
			}
		}

		//���� ���� ������ �浹�Ѱ���

		auto penetration = minoverlap;

		//0~5���� �ϰ�� �̰���  paxis�� �ȴ�.
		auto pAxis = axis[minindex];

		//������ ���߿� �ٲ�����. �켱 �ؾ��Ұ� ������ �˻��� ������Ʈ�� ã�� ���� CenterLNormal�� ���ϴ°�.

		auto CenterLNormal = Float3Normalize(CenterL);

		float DotCp = CenterLNormal.x*pAxis.x + CenterLNormal.y*pAxis.y + CenterLNormal.z*pAxis.z;

		XMFLOAT4 arr[8]; // ����� ���� �迭
		XMFLOAT4 arr2[8]; // ����� ���� �迭 other

						  //6���� Other ���
		XMFLOAT3 Plane[6];
		//6���� Testing���
		XMFLOAT3 Plane2[6];
		//6���� ����� ���������� ��ġ.
		XMFLOAT3 PlaneOrg[6];
		//6���� ����� ���������� ��ġ.
		XMFLOAT3 PlaneOrg2[6];


		//����� ����� ���� �˻��� ����� B �ƴϸ� A.
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


			//6���� ��鿡���� ��ġ
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


			//6���� ��鿡���� ��ġ
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


			//6���� ��鿡���� ��ġ
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


			//6���� ��鿡���� ��ġ
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
		//��� ������ ������� penetration���� �����Ѵ�. ���� ��Ȯ���� �ʾƵ� �ȴ�. ���� ���� ���� penetration�� ã�°� ��ǥ�ϱ�.
		sort(AllPoint.begin(), AllPoint.end(), [](CollisionPoint& p1, CollisionPoint& p2) {return p1.penetration < p2.penetration; });
		//other�� ũ�Ⱑ ū������
		sort(AllPoint2.begin(), AllPoint2.end(), [](CollisionPoint& p1, CollisionPoint& p2) {return p1.penetration > p2.penetration; });


		//���� ���ĵ� 8���� ���� ����� �����Ŀ� ���� �ϴ� �浹�Ǿ��ٰ� �����Ǵ� ������ ã�´�.
		//�� ���� ������ 1�� 2�� 4�� �ϰ��̸�, �׿����� �ٸ��� ó���ؾ��Ѵ�.
		vector<CollisionPoint> ColPoint;//�ϴ� �浹�� �Ͼ ������.
		vector<CollisionPoint> ColPoint2;//�ϴ� �浹�� �Ͼ ������. other

										 //��ó�� �浹�� �Ͼ�ٸ� ���� ���� penetration�� ��ġ�ϴ� ���� �ִ� 4���ۿ� �����Ƿ� 4���� �˻��Ѵ�.
		for (int i = 0; i < 4; i++)
		{
			if (fabsf(AllPoint[0].penetration - AllPoint[i].penetration) <= MMPE_EPSILON)
				ColPoint.push_back(AllPoint[i]);

			if (fabsf(AllPoint2[0].penetration - AllPoint2[i].penetration) <= MMPE_EPSILON)
				ColPoint2.push_back(AllPoint2[i]);
		}



		//���� ColPoint�� ���� ������ �浹�� ���� �������� �˻��Ѵ�. ColPoint�� ����� �༮���� �ϴ� pAxis�� ����� ���ʿ� �ֱ⿡ ��÷�� �����̹Ƿ�
		//���� ���� 1���� ColPoint��� ������ ������ ���� �浹���̴�. ��? �ϴ� ���ʿ� ����ü �浹�� �Ͼ�� ������ �ݵ�� �� 1���� �浹�����ٵ�, ���� �� 1���ϱ�.
		//2����� ���⼭ ���� �� �����ؾ��Ѵ�. �ֳ��ϸ� �� 2���϶��� ���� ���浹�� �Ǽ� �浹�� ���ϴ°�찡 ����. �̸� �����ϱ����� ����3�� �߰��ؼ� ����ü�ȿ� �ִ���
		//�˻��ؾ���.

		int sz = ColPoint.size();
		int sz2 = ColPoint2.size();


		vector<CollisionPoint> lastPoints;

		if (sz == 1)
		{
			CollisionPointVector.push_back(ColPoint[0]);

		}
		else if (sz == 2)
		{
			//�������� �浹����.


			XMFLOAT4 pos3 = Float4Add(ColPoint[0].Pos, ColPoint[1].Pos);
			pos3 = Float4Float(pos3, 0.5);
			CollisionPoint newPoint = ColPoint[0];
			newPoint.Pos = pos3;
			//����3�� ���� ����.
			ColPoint.push_back(newPoint);

			//���� ����ü �ȿ� �����ϴ� ���� �ִ��� �˻��Ѵ�.
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

					if (result.x > 0)//0���� ũ�� �ٿ���ڽ� �ȿ� ����.
					{
						In = false;
						break;
					}

				}

				//�ȿ� �����ϴ� ���̸� ������ �������� �浹���� �߰��Ѵ�.
				if (In == true)
				{
					lastPoints.push_back(ColPoint[i]);
				}

			}


			//���⼱ lastPoints�� ����� ����  CollisionPointVector �� �߰��Ѵ�.

			//���࿡ �浹���� 0�ΰ�� other�� �ٽ� �˻��Ѵ�.
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
					//����3�� ���� ����.
					ColPoint2.push_back(newPoint);

					//���� ����ü �ȿ� �����ϴ� ���� �ִ��� �˻��Ѵ�.
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

							if (result.x > 0)//0���� ũ�� �ٿ���ڽ� �ȿ� ����.
							{
								In = false;
								break;
							}

						}

						//�ȿ� �����ϴ� ���̸� ������ �������� �浹���� �߰��Ѵ�.
						if (In == true)
						{
							lastPoints.push_back(ColPoint2[i]);
						}


					}
					//���� ������ ����ص� �����ϸ�...
					if (lastPoints.size() == 0)
					{
						//ColPoint2 �� ������ �̿��� ���尡��� ���� ã�´�. �̶� ���� ��Ȯ�ϰ� �ϱ����� ColPoint�� ����3�� ��1�� �߰���, ����3�� ��2���߰����� �߰�
						//�ؼ� 5���� ������ �˻�����.
						XMFLOAT4 pos4 = Float4Add(ColPoint[0].Pos, ColPoint[2].Pos);
						pos4 = Float4Float(pos4, 0.5);
						CollisionPoint newPoint = ColPoint[0];
						newPoint.Pos = pos4;
						//����4�� ���� ����.
						ColPoint.push_back(newPoint);

						pos4 = Float4Add(ColPoint[1].Pos, ColPoint[2].Pos);
						pos4 = Float4Float(pos4, 0.5);
						newPoint = ColPoint[0];
						newPoint.Pos = pos4;
						//����5�� ���� ����.
						ColPoint.push_back(newPoint);

						//Other�� ���� ������ 5���� ColPoint�� �� ���� ����� ���Ѱ��� ã��.
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

						//����ª�Ҵ� ColPoint�� �ε����� ���� ����
						lastPoints.push_back(ColPoint[index]);
					}

					if (lastPoints.size() == 1)//��1��  ��� ������ ���������� �Ѵ�.
					{
						CollisionPointVector.push_back(lastPoints[0]);
					}
					else//�׿ܴ� ��������
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
					//����3�� ���� ����.
					ColPoint2.push_back(newPoint);

					//���� ����ü �ȿ� �����ϴ� ���� �ִ��� �˻��Ѵ�.
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

							if (result.x > 0)//0���� ũ�� �ٿ���ڽ� �ȿ� ����.
							{
								In = false;
								break;
							}

						}

						//�ȿ� �����ϴ� ���̸� ������ �������� �浹���� �߰��Ѵ�.
						if (In == true)
						{
							lastPoints.push_back(ColPoint2[i]);
						}


					}

					if (lastPoints.size() == 1)//��1���� �� ���� �浹������ �Ѵ�.
					{
						CollisionPointVector.push_back(lastPoints[0]);
					}
					else if (lastPoints.size() == 2)//��2���� �ΰ��� ����.
					{

						XMFLOAT4 pos = Float4Add(lastPoints[0].Pos, lastPoints[1].Pos);
						pos = Float4Float(pos, 0.5);
						CollisionPoint newPoint = lastPoints[0];
						newPoint.Pos = pos;

						CollisionPointVector.push_back(newPoint);

					}
					else//��3���̻��̸� ����5��
					{
						CollisionPointVector.push_back(ColPoint2[4]);
					}


				}
			}
			else if (lastPoints.size() == 1)//��1��  ��� ������ ���������� �Ѵ�.
			{
				CollisionPointVector.push_back(lastPoints[0]);
			}
			else//�׿ܴ� ��������
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
			//����3�� ���� ����.
			ColPoint.push_back(newPoint);

			//���� ����ü �ȿ� �����ϴ� ���� �ִ��� �˻��Ѵ�.
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

					if (result.x > 0)//0���� ũ�� �ٿ���ڽ� �ȿ� ����.
					{
						In = false;
						break;
					}

				}

				//�ȿ� �����ϴ� ���̸� ������ �������� �浹���� �߰��Ѵ�.
				if (In == true)
				{
					lastPoints.push_back(ColPoint[i]);
				}


			}

			//���࿡ �浹���� 0�ΰ�� other�� �ٽ� �˻��Ѵ�.
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
					//����3�� ���� ����.
					ColPoint2.push_back(newPoint);

					//���� ����ü �ȿ� �����ϴ� ���� �ִ��� �˻��Ѵ�.
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

							if (result.x > 0)//0���� ũ�� �ٿ���ڽ� �ȿ� ����.
							{
								In = false;
								break;
							}

						}

						//�ȿ� �����ϴ� ���̸� ������ �������� �浹���� �߰��Ѵ�.
						if (In == true)
						{
							lastPoints.push_back(ColPoint2[i]);
						}



					}
					//���� ������ ����ص� �����ϸ�...
					if (lastPoints.size() == 0)
					{
						//ColPoint2 �� ������ �̿��� ���尡��� ���� ã�´�. �̶� ���� ��Ȯ�ϰ� �ϱ����� ColPoint�� ����3�� ��1�� �߰���, ����3�� ��2���߰����� �߰�
						//�ؼ� 5���� ������ �˻�����.
						XMFLOAT4 pos4 = Float4Add(ColPoint[0].Pos, ColPoint[2].Pos);
						pos4 = Float4Float(pos4, 0.5);
						CollisionPoint newPoint = ColPoint[0];
						newPoint.Pos = pos4;
						//����4�� ���� ����.
						ColPoint.push_back(newPoint);

						pos4 = Float4Add(ColPoint[1].Pos, ColPoint[2].Pos);
						pos4 = Float4Float(pos4, 0.5);
						newPoint = ColPoint[0];
						newPoint.Pos = pos4;
						//����5�� ���� ����.
						ColPoint.push_back(newPoint);

						//Other�� ���� ������ 5���� ColPoint�� �� ���� ����� ���Ѱ��� ã��.
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

						//����ª�Ҵ� ColPoint�� �ε����� ���� ����
						lastPoints.push_back(ColPoint[index]);
					}

					if (lastPoints.size() == 1)//��1��  ��� ������ ���������� �Ѵ�.
					{
						CollisionPointVector.push_back(lastPoints[0]);
					}
					else//�׿ܴ� ��������
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
					//����3�� ���� ����.
					ColPoint2.push_back(newPoint);

					//���� ����ü �ȿ� �����ϴ� ���� �ִ��� �˻��Ѵ�.
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

							if (result.x > 0)//0���� ũ�� �ٿ���ڽ� �ȿ� ����.
							{
								In = false;
								break;
							}

						}

						//�ȿ� �����ϴ� ���̸� ������ �������� �浹���� �߰��Ѵ�.
						if (In == true)
						{
							lastPoints.push_back(ColPoint2[i]);
						}


					}

					if (lastPoints.size() == 1)//��1���� �� ���� �浹������ �Ѵ�.
					{
						CollisionPointVector.push_back(lastPoints[0]);
					}
					else if (lastPoints.size() == 2)//��2���� �ΰ��� ����.
					{

						XMFLOAT4 pos = Float4Add(lastPoints[0].Pos, lastPoints[1].Pos);
						pos = Float4Float(pos, 0.5);
						CollisionPoint newPoint = lastPoints[0];
						newPoint.Pos = pos;

						CollisionPointVector.push_back(newPoint);

					}
					else if (lastPoints.size() >= 3)//��3���̻��̸� ����5��
					{
						CollisionPointVector.push_back(ColPoint2[4]);
					}
					else//���������� ��ã�� ���
					{
						//��� �� �浹�ε� ����� �˻��ص� ���ٸ�, �ι�° ������Ʈ�� ������ ���� ����� ù��° ������Ʈ�� �浹���� �����Ѵ�.
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
			else if (lastPoints.size() == 1)//��1���� �� ���� �浹������ �Ѵ�.
			{
				CollisionPointVector.push_back(lastPoints[0]);
			}
			else if (lastPoints.size() == 2)//��2���� �ΰ��� ����.
			{

				XMFLOAT4 pos = Float4Add(lastPoints[0].Pos, lastPoints[1].Pos);
				pos = Float4Float(pos, 0.5);
				CollisionPoint newPoint = lastPoints[0];
				newPoint.Pos = pos;

				CollisionPointVector.push_back(newPoint);

			}
			else//��3���̻��̸� ����5��
			{
				CollisionPointVector.push_back(ColPoint[4]);
			}


		}

		//��� ó���� �ߴµ� ��ã�����
		if (CollisionPointVector.size() == 0)
		{
			CollisionPoint cp;
			cp.pAxis = pAxis;
			cp.penetration = penetration;
			auto p = XMFloat3to4(pAxis);
			cp.Pos = Float4Add(p, *CenterPos);
			CollisionPointVector.push_back(cp);
		}


		//�������� pAxis
		float l = Float3Normalize(CenterL).x*axis[minindex].x + Float3Normalize(CenterL).y*axis[minindex].y + Float3Normalize(CenterL).z*axis[minindex].z;
		if (l > 0)
			pAxis = XMFLOAT3(-pAxis.x, -pAxis.y, -pAxis.z);

		for (int i = 0; i < CollisionPointVector.size(); i++)
		{
			CollisionPointVector[i].pAxis = pAxis;
			CollisionPointVector[i].penetration = penetration;
		}
		if (penetration > MMPE_EPSILON)//���Ƿк��� ũ�� �浹 �ƴϸ� ���� �̼��� �浹�̹Ƿ� ó���� �� �ʿ䰡 ����
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
	//������ ��ü���ƴϸ�.
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
	if (CollisionPointVector[0].penetration < MMPE_EPSILON)//�о �ʿ䰡 ���°��
		return;
	float totalInverseMass = GetMass();
	totalInverseMass += rb2.GetMass();

	if (totalInverseMass <= 0)//�Ѵ� ���Ѵ��ϰ��
		return;
	//��ü�� pAxis�������� �ű��. �ٸ� ������ �ݺ���ϰ� ������. ������Ŭ���� �������δ�.
	XMFLOAT3 movevector(CollisionPointVector[0].pAxis.x*(CollisionPointVector[0].penetration / totalInverseMass), CollisionPointVector[0].pAxis.y*(CollisionPointVector[0].penetration / totalInverseMass), CollisionPointVector[0].pAxis.z*(CollisionPointVector[0].penetration / totalInverseMass));

	//����� ������ �������� �ʴ´�.�� ���Ƿ��� �ț��ĸ� ���Ƿ�������� penetration�� ���Ƿ������϶�
	//�з������Ұ�� �ȹз����� ��Ȳ�� �Ͼ��, ���Ƿк��� 10������ ������ �˻��Ѵ�.
	if (FloatLength(movevector) <= 0.0001)
		movevector = XMFLOAT3(0, 0, 0);

	XMFLOAT4 m1(movevector.x*GetMass(), movevector.y*GetMass(), movevector.z*GetMass(), 0);
	auto p1 = GetPosition();
	auto result1 = Float4Add(p1, m1);

	*CenterPos = result1;//�ڽ��� �ű��.


	XMFLOAT4 m2(-movevector.x*rb2.GetMass(), -movevector.y*rb2.GetMass(), -movevector.z*rb2.GetMass(), 0);
	auto pp2 = rb2.GetPosition();
	auto result2 = Float4Add(pp2, m2);
	*rb2.CenterPos = result2;//������ ����� �ű��.


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

	//Wr1 = �浹�� ���ӵ� X (Q-P)
	auto av = GetAngularVelocity();
	auto r1 = Float4Add(cp.Pos, *CenterPos, false);
	auto r2 = XMFloat4to3(r1);
	XMFLOAT3 Wr1 = Float3Cross(av, r2);
	//Wr2�� ��������.
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

	//1����. -(1+e)(�浹�� ���ӵ� + (�浹�� ���ӵ� X (Q-P) - �浹�� ���ӵ�2 X (Q-P2) ) * ���˹��� 
	auto s1 = Float3Add(Wr1, Wr2, false);
	auto t1 = (Float3Add(SeparateVel, s1, true));
	t1.x *= cp.pAxis.x;
	t1.y *= cp.pAxis.y;
	t1.z *= cp.pAxis.z;
	float First = finalE * (t1.x + t1.y + t1.z);


	//temp1 = ( (Q-P1)X ���˹��� );
	//u1 = ( ( temp1 * ������ 1 ) X (Q-P1) ) * ���˹���
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

	//temp2 = ( (Q-P2)X ���˹��� );
	//u2 = ( ( temp2 * ������ 2 ) X (Q-P2) ) * ���˹���

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

	//��2. u1+u2 + inverseMs

	//float Second = u1 + u2 + inverseMs;

	//������. �̰Դ� �ڿ�������.
	float Second = inverseMs;

	//��3 = (��1 / ��2)*��ݷ��� ���� �ð�  = ���� ���޽�

	if (Second != 0)
		return abs((First / Second)*deltatime);
	else
		return 0;


}
#include "MyMiniPysicsEngine.h"
#include<math.h>

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
	if(add)
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
	if(add)
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



PhysicsPoint::PhysicsPoint()
{
	 InverseMass = 1;
	Velocity = XMFLOAT3(0, 0, 0);
	CenterPos = XMFLOAT3(0, 0, 0);
	damping = 0.99;
	TotalForce = XMFLOAT3(0, 0, 0);
	Accel = XMFLOAT3(0, 0, 0);
	Bounce = false;
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
	//�߷¿� ���� �ӵ��� �ʹ� ��������츦 ����� if���̴�.
	//-40���� �� �ʰ� �������� �״�� ����, -40�̻��� �ӵ��� -40���� �����Ѵ�.
	//tempV�� �̿��� ������ ���� �ӵ��� -40���� �����ع����� �������Ŀ� �ӵ��� �������� �������� �����̴�.
	XMVECTOR velocity;
	if(Velocity.y>-40)
		velocity = XMLoadFloat3(&Velocity);
	else
	{
		XMFLOAT3 tempV = Velocity;
		tempV.y = -40;
		velocity = XMLoadFloat3(&tempV);
	}
	XMVECTOR accel = XMLoadFloat3(&Accel);
	XMVECTOR totalforce = XMLoadFloat3(&TotalForce);
	//���� ���� �������� ���ӵ��� ����Ѵ�.

	//���ӵ� = ������ * ������ ����. ���ӵ��� �������������� ����� ��. F�� 0�̸� a�� 0
	//���� ���� �ѹ��� �������� ���ӵ��� �ѹ��� 0�̾ƴϰ� ���Ĵ� ��� 0�̴�.
	//�ٸ� �ӵ��� ���ӵ��� ���� ���� ����� ��� �����ϸ� ���������� ���� 0 �� �������.
	accel =  totalforce * InverseMass;


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
	XMStoreFloat3(&CenterPos, centerpos);
	XMStoreFloat3(&Velocity, velocity);
	XMStoreFloat3(&Accel, accel);
	
	//��� ����� �������� ���� �ʱ�ȭ �Ѵ�.
	ForceClear();


	//���� ����� ������ �ӵ� ������ PhysicsPoint�� �������ִ� ������Ʈ�� ���������� �־�� �Ѵٸ�

	if (ObjPos != NULL)//���� ���ӿ�����Ʈ�� ���� ������ �ִ� ���� ������ ������
		*ObjPos = XMFLOAT4(CenterPos.x, CenterPos.y, CenterPos.z,ObjPos->w);
	if (ObjVel != NULL)//���� ���ӿ�����Ʈ�� ���� ������ �ִ� �ӵ� ������ ������
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

void MiniPhysicsEngineG9::PhysicsPoint::SetBounce(bool bounce)
{
	Bounce = bounce;
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

void MiniPhysicsEngineG9::PhysicsPoint::SetHalfBox(float x, float y, float z)
{
	halfbox.x = x;
	halfbox.y = y;
	halfbox.z = z;

}

XMFLOAT3 MiniPhysicsEngineG9::PhysicsPoint::GetHalfBox()
{
	return halfbox;
}

bool MiniPhysicsEngineG9::PhysicsPoint::CollisionTest(PhysicsPoint & p2, XMFLOAT3 & l1, XMFLOAT3 & r1, XMFLOAT3 & u1, XMFLOAT3 & l2, XMFLOAT3 & r2, XMFLOAT3 & u2)
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
		CenterL = XMFLOAT3(p2.GetPosition().x-GetPosition().x , p2.GetPosition().y-GetPosition().y , p2.GetPosition().z-GetPosition().z );

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
		pAxis = XMFLOAT3(-pAxis.x,-pAxis.y,-pAxis.z);
	
	//return true;
	if (penetration > MMPE_EPSILON)//���Ƿк��� ũ�� �浹 �ƴϸ� ���� �̼��� �浹�̹Ƿ� ó���� �� �ʿ䰡 ����
		return true;
	else
		return false;
}

void MiniPhysicsEngineG9::PhysicsPoint::CollisionResolve(PhysicsPoint & p2, XMFLOAT3 & CollisionN, float DeltaTime, bool KeyBoard)
{
	//�浹�� �ӵ��� �����.
	ResolveVelocity(p2, CollisionN, DeltaTime,KeyBoard);

	//�ӵ������ ��ġ�� �κ��� �о
	ResolvePenetration(p2, DeltaTime,KeyBoard);
}

float MiniPhysicsEngineG9::PhysicsPoint::GetSeparateVelocity(PhysicsPoint & p2, XMFLOAT3 & CollisionN, bool KeyBoard)
{
	if (KeyBoard)//Ű���� ó����
	{
		return 0;
	}
	else//Ű���� ó���� �ƴѰ��
	{
		XMFLOAT3 v1 = GetVelocity();
		XMFLOAT3 v2 = p2.GetVelocity();
		v1 = Float3Add(v1, v2, false);
		return v1.x*CollisionN.x + v1.y*CollisionN.y + v1.z*CollisionN.z;
	}
}

bool MiniPhysicsEngineG9::PhysicsPoint::GetBounce()
{
	return Bounce;
}

void MiniPhysicsEngineG9::PhysicsPoint::ResolveVelocity(PhysicsPoint & p2, XMFLOAT3 & CollisionN, float DeltaTime, bool KeyBoard)
{
	//�浹�� �ӵ� ��ȭ�� ������ ����.
	//1. �浹�� �и��ӵ��� ���ϰ�
	//2. �浹 �� �и��ӵ��� ���Ѵ� Vf=-eVi ��.
	//3. ��ݷ� ũ�⸦ ����Ѵ�.
	//4. ��ݷ��� ����Ѵ�. ��ݷ� ũ�� * CollisionN�̴�. �⺻������ CollisionN�� ������ġ-������ġ�� ���ȭ�Ѱ�.
	//�ٸ� Ű���� �Է½� �з������ϴ¹����� ���� Ű���� ���� Ű�� �ݴ������ �ӵ���.

	if (KeyBoard)//Ű���� �Է�ó���� ���� �ǵ��ƿ;��ϸ�, �ӵ��� �����ϴ°� �ƴ϶� ��ġ�� �����Ѵ�!!
	{
		//���ŵ� ��ġ = ������ġ - Ű���� ���� ����
		auto op = GetPosition();
		op = Float3Add(op, CollisionN, false);
		SetPosition(op);//��ġ�� �����Ѵ�. �߿������� �̰� ������ ��ġ�̹Ƿ� �ݵ�� �浹�ؼ��� ������Ʈ ������ �����϶�

	}
	else//�Ϲ����� ��쿡�� �ӵ��� �����Ų��. 
	{
		float sv = GetSeparateVelocity(p2, CollisionN);
		if (sv > 0)//�̹� �־���������
			return;
		float nsv = -sv*1;//���� ���⿡ 1��� ź����� e�� ���µ�, �츮�� ����� ������ ����������Ʈ�� e�� 1�̴�.
		float deltav = nsv - sv;

		float totalImass = GetMass();
		totalImass += p2.GetMass();
		if (totalImass <= 0)
			return;
		float impurse = deltav / totalImass;

		XMFLOAT3 impursevector(CollisionN.x*impurse, CollisionN.y*impurse, CollisionN.z*impurse);

		//���� ��ü�� ��ȭ �� ���� �ӵ�
		auto resultv = GetVelocity();
		XMFLOAT3 iv1 (impursevector.x*GetMass(), impursevector.y*GetMass(), impursevector.z*GetMass());
		resultv = Float3Add(resultv, iv1);
		SetVelocity(resultv);

		
		//���� �ε��� ��ü�� �����ӵ�
		
		auto resultv2= p2.GetVelocity();
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
			gv = XMFLOAT3(0,0,0);
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
}
//��������� ���θ� �о. Ű���� �Է��϶� �ڽŸ� �з���
//�����϶��� �� ������ �����̸� ������ �и������� �ֳ��ϸ� �̰� �����ΰ�
//���߻��·� ���������� �߷¸� ������ �ްԵǹǷ� Ű���� �Է��� �ƴϰԵ� �̶� �浹�� �Ͼ��
//�밢������ ���� �о�� �ǰ� �̷��� �Ǹ� y���� �����ϸ� x�� z���� 0�̾ƴϰ� �Ǹ鼭 �и��°�.
//���� �̰� ������ �ȵ�� x�� z�� �ӵ��� 0�̸� �з����� �ʵ��� �ϴ� ����� ����.
//���� �̰� �� ������ ���ĸ� �ƿ� ������ �����ִ� ������ �ȿ����̰� ������ �ȶ������.
//���� �׳� ���߿����� �о�� ��� ������ �ϴ°� ������ ����.
void MiniPhysicsEngineG9::PhysicsPoint::ResolvePenetration(PhysicsPoint & p2, float DeltaTime,bool Keyboard)
{
	if (penetration <= 0)//�о �ʿ䰡 ���°��
		return;
	float totalInverseMass = GetMass();
	totalInverseMass += p2.GetMass();

	if (totalInverseMass <= 0)//�Ѵ� ���Ѵ��ϰ��
		return;
	//��ü�� pAxis�������� �ű��. �ٸ� ������ �ݺ���ϰ� ������. ������Ŭ���� �������δ�.
	XMFLOAT3 movevector(pAxis.x*(penetration / totalInverseMass), pAxis.y*(penetration / totalInverseMass), pAxis.z*(penetration / totalInverseMass));
	
	//����� ������ �������� �ʴ´�.�� ���Ƿ��� �ț��ĸ� ���Ƿ�������� penetration�� ���Ƿ������϶�
	//�з������Ұ�� �ȹз����� ��Ȳ�� �Ͼ��, ���Ƿк��� 10������ ������ �˻��Ѵ�.
	if (FloatLength(movevector) <=0.0001)
		movevector = XMFLOAT3(0, 0, 0);

	XMFLOAT3 m1(movevector.x*GetMass(), movevector.y*GetMass(), movevector.z*GetMass());
	auto p1 = GetPosition();
	auto result1 = Float3Add(p1, m1);
	
	SetPosition(result1);//�ڽ��� �ű��.

	if (Keyboard == false)
	{
		XMFLOAT3 m2(-movevector.x*p2.GetMass(), -movevector.y*p2.GetMass(), -movevector.z*p2.GetMass());
		auto pp2 = p2.GetPosition();
		auto result2 = Float3Add(pp2, m2);
		p2.SetPosition(result2);//������ ����� �ű��.
	}

	
}

//�ϳ��� ���� �������� �� �ڽ��� ������ ����� �ջ����� ������ �Ÿ����͸� ���� ������ ������ ����� ������ 0���� ũ�� ��ġ�������� 0���� ������ �и��Ǿ��ִ�.
float MiniPhysicsEngineG9::PhysicsPoint::penetrationAxis(PhysicsPoint & p2, XMFLOAT3 & l1, XMFLOAT3 & r1, XMFLOAT3 & u1, XMFLOAT3 & l2, XMFLOAT3 & r2, XMFLOAT3 & u2, XMFLOAT3 & CenterLength,XMFLOAT3& Axis)
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

bool MiniPhysicsEngineG9::RayCastObject::RayCasting(XMFLOAT3 & RayOrgin, XMFLOAT3 & RayDir, XMFLOAT3& ObjCenterPos, XMFLOAT3& PlayerPos,XMFLOAT3& Look, XMFLOAT3 * SavePos)
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


	auto frontV=XMVector3Dot(look, XMVector3Normalize(objpos - playerpos));

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
		//2. �÷��̾��� ������Ʈ �տ� �����ؾ��Ѵ�.
		//�� �÷��̾�� ī�޶� �������ִ� 3��Ī ���ӿ��� �÷��̾� ���� ī�޶� ������ �ִ� ���
		//�÷��̾�� ���� ���� �����׹Ƿ�, �ڷ� ź�� ���ư��� �ȵǱ� ������
		//���Ͱ� �÷��̾� �տ� �������� ����ĳ������ �����ؾ��Ѵ�.
		//�̸� ���� ������ ���ߴ� ���(defaultpos+objpos)�� ��ġ���� �÷��̾� ��ġ�� �� ���Ϳ�
		//ī�޶� �ٶ󺸰��ִ� ����(���� ������ ���ϸ� �÷��̾��� Look���� �ؾ��Ѵ�.)�� ������
		//0�̻��̰�, t�� ī�޶� �� ������ �� ������ �����Ѵ�.
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
	//0 ���� 1 ������ 2 ���� 3 ���� 4 �Ʒ��� 5 ����
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


//������Ҷ� -k�ҰŴϱ� �׳� k�� ������!!!
void MiniPhysicsEngineG9::GeneratorAnchor::SetAnchorSpring(XMFLOAT3 & a, float k, float l)
{
	AnchorPos = a;
	constantspring = k;
	DefaultLength = l;
}

void MiniPhysicsEngineG9::GeneratorAnchor::Update(float DeltaTime, PhysicsPoint & pp)
{
	XMVECTOR objpos = XMLoadFloat3(&pp.GetPosition());
	XMVECTOR ancpos = XMLoadFloat3(&AnchorPos);

	XMVECTOR obj_anc = objpos-ancpos;
	
	float curlen = FloatLength(obj_anc);

	//F=K(defaultlength-curlen); �� -���Ⱥٿ��ĸ� ���� ��ŸX���Ҷ� ������� - �⺻������. �ٵ� �ݴ�� �ؼ� -1���ѰŶ�����
	float F = constantspring * (DefaultLength - curlen);

	//���� ���� ����ȭ ��Ų��. �� obj_anc�� ��ֶ������ؼ� ���������ϰ� F�� ũ��� �Ѵ�.

	obj_anc=XMVector3Normalize(obj_anc);
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
	pp.SetVelocity(Float3Add(pp.GetVelocity(),JumpVel));
}

void MiniPhysicsEngineG9::GeneratorJump::SetJumpVel(XMFLOAT3& Jump)
{
	JumpVel = Jump;
}


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
	//���� ���� �������� ���ӵ��� ����Ѵ�.

	//���ӵ� = ������ * ������ ����. ���ӵ��� �������������� ����� ��. F�� 0�̸� a�� 0
	//���� ���� �ѹ��� �������� ���ӵ��� �ѹ��� 0�̾ƴϰ� ���Ĵ� ��� 0�̴�.
	//�ٸ� �ӵ��� ���ӵ��� ���� ���� ����� ��� �����ϸ� ���������� ���� 0 �� �������.
	accel =  totalforce * InverseMass;


	//�ӵ��� ��ġ�� ���ŵǾ��Ѵ�.

	//�ӵ��� ���ӵ��� ������ ������ ���Ѵ�.
	centerpos = centerpos + velocity * DeltaTime + 0.5*accel*DeltaTime*DeltaTime;
	
	
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
	ga *= pp.GetMass(false);//�߷� = �߷°��ӵ� * ����
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

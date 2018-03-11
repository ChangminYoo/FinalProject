#pragma once
#include<DirectXMath.h>
using namespace DirectX;
#define MMPE_EPSILON 0.001
#define MMPE_PI 3.14159287
namespace MiniPhysicsEngineG9
{
	class PhysicsPoint
	{

	public:
		PhysicsPoint();
		~PhysicsPoint();

	protected:
		float InverseMass;//������ ����. a=1/m * F �̹Ƿ� �̷�����. ���Ժ��� ���ӵ��� �� �߿��ϴϱ�. �̶� m�� �����̸� 0�̴�.
		XMFLOAT3 Velocity;//�ӵ�
		XMFLOAT3 CenterPos;//����
		float damping;//������ ��. ������ ������ε� ���
		XMFLOAT3 TotalForce;//���������� ������ ��� ��
		XMFLOAT3 Accel;//���ӵ�
		// ���� ���� ���ӵ��� ���ϰ� ���ӵ��� ��ġ�� �ӵ��� ����Ѵ�. ���Ϸ������ �̿���

		XMFLOAT3 halfbox;//x����y����z����
		bool Bounce = false;


	public:
		void integrate(float DeltaTime, XMFLOAT4* ObjPos = NULL, XMFLOAT3* ObjVel = NULL);//���б�. �ӵ��� ���ӵ��� ��ġ�� ���ϰ� ���ӵ��� �̿��� �ӵ��� ������.
		void SetMass(float M);//������ ���� ���� �����ϱ� ���� �Լ�. M�� ������ ������ ��������
		float GetMass(bool Inverse = true);//�⺻������ ������ ���� ����. �ٸ� false�� �θ� �״�� ���´�.
		void SetDamping(float D);//���������� ������.
		float GetDamping();//�������� ��ȯ

		void SetBounce(bool bounce);
		void SetPosition(XMFLOAT3& pos);//���� ��ġ ����
		void SetPosition(XMFLOAT4& pos);//���� ��ġ ����
		void SetPosition(float x, float y, float z);//���� ��ġ ����
		XMFLOAT3 GetPosition();//���� ���

		void SetVelocity(XMFLOAT3& V);//�ӵ�����. �̴� �ʱ�ӵ��̸�, ��κ��� �ӵ��� ���б⿡�� ���Ǿ��Ѵ�.
		void SetVelocity(float x, float y, float z);
		XMFLOAT3 GetVelocity();//�ӵ����

		void SetAccel(XMFLOAT3& A);//���ӵ� ����. �ʱ� ���ӵ��̸� ��κ��� ���ӵ��� �������� �����Ǿ���.
		void SetAccel(float x, float y, float z);
		XMFLOAT3 GetAccel();//���ӵ� ���

		void ForceClear();//��� �� �ʱ�ȭ
		void AddForce(XMFLOAT3& F);//Ư���������� ���� ���Ѵ�.
		void AddForce(float x, float y, float z);
		XMFLOAT3 GetTotalForce();//������� �� ���� ��ȯ.

		//�浹����
		float penetration = 0;//�о���ϴ� ����
		XMFLOAT3 pAxis;//�о ����
		void SetHalfBox(float x, float y, float z);
		XMFLOAT3 GetHalfBox();
		//���ڷ� ���� ���� �������� �ΰ��� ����ü�� �����Ѵ��� �� ���̸� �ջ��Ѱ��� �������Ấ�͸� �����Ѱ��� ���̿� ���� ����� ����� ��ġ�� �ƴϸ� �и��Ȱ�.
		float penetrationAxis(PhysicsPoint& p2, XMFLOAT3& l1, XMFLOAT3& r1, XMFLOAT3& u1, XMFLOAT3& l2, XMFLOAT3& r2, XMFLOAT3& u2, XMFLOAT3& CenterLength, XMFLOAT3& Axis);
		//�浹�ߴ��� ���θ� �˻���. 15���� ���� �˻���.
		bool CollisionTest(PhysicsPoint& p2, XMFLOAT3& l1, XMFLOAT3& r1, XMFLOAT3& u1, XMFLOAT3& l2, XMFLOAT3& r2, XMFLOAT3& u2);

		//�浹ü �ؼ� ����. �̶� Ű���� ó���� ���� �������. �ֳ��ϸ� �밢������ �������� �浹�ȵǴ� �κ��� �������ϰ�
		//�����̰� �ϱ����ؼ���. CollisionN�� �浹�� ���� �з������ϴ� ������ ��Ÿ����.
		void CollisionResolve(PhysicsPoint& p2,XMFLOAT3& CollisionN, float DeltaTime, bool KeyBoard=false);
		//���ӵ��� ���Ѵ�.
		float GetSeparateVelocity(PhysicsPoint& p2, XMFLOAT3& CollisionN, bool KeyBoard = false);
		//�浹�� �ӵ��� ���Ѵ�.
		bool GetBounce();
		void ResolveVelocity(PhysicsPoint& p2, XMFLOAT3& CollisionN, float DeltaTime, bool KeyBoard = false);
		//������ �κ��� �о��.
		void ResolvePenetration(PhysicsPoint& p2,float DeltaTime,bool Keyboard=false);
	};

	struct RayCastPlane
	{
		XMFLOAT3 Normal;//����� ���
		XMFLOAT3 DefaultCenterPos;//���������� ����� ������ ��ġ
	};

	class RayCastObject
	{
		RayCastPlane Plane[6];

	public:
		bool RayCasting(XMFLOAT3& RayOrgin, XMFLOAT3& RayDir,XMFLOAT3& ObjCenterPos,XMFLOAT3& PlayerPos,XMFLOAT3& Look, XMFLOAT3* SavePos = NULL);
		void SetPlane(XMFLOAT3& x, XMFLOAT3& y, XMFLOAT3& z);
	};
	XMFLOAT3 RayShot(XMFLOAT3& RayOrgin, XMFLOAT3& RayDir, float MaxLen);


	//********************* �ý��� Ŭ������ ���Ǵ� �� ������ ********************************//

	//@ �ý��� �Լ��� ���������ӿ�ũ�� �����Ѵ�. ��������� ��������� ������ �ִ´�. ���� Update�Լ��� ȣ�����ָ� �ȴ�. 

	//1. �߷� �ý��� �� ���� ��������
	class GeneratorGravity
	{
		XMFLOAT3 GravityAccel;//�߷°��ӵ�
	public:
		void SetGravityAccel(XMFLOAT3& ga);
		XMFLOAT3 GetGravityAccel();
		void Update(float DeltaTime, PhysicsPoint& pp);

	};

	//2. ������ ������ �� ������
	class GeneratorAnchor
	{
		XMFLOAT3 AnchorPos;//������ ��ġ��ǥ
		float constantspring;//f=k��x���� k�� ��Ÿ�� �� -k���ƴϳĸ� ��Ÿx���Ҷ� ������ �������-�⺻���� ��
		float DefaultLength;//�⺻����.��x=( DefaultLength - ������� );
	public:
		void SetAnchorSpring(XMFLOAT3& a, float k, float l);//������Ҷ� -k�ҰŴϱ� �׳� k�� ������!!!
		void Update(float DeltaTime, PhysicsPoint& pp);
	};

	//3. ���� �� ������
	class GeneratorJump
	{
		XMFLOAT3 JumpVel;
	public:
		void Update(float DeltaTime, PhysicsPoint& pp);
		void SetJumpVel(XMFLOAT3& Jump);
	};






	//*********************** ���� �Լ� �� *******************************************//
		float FloatLength(XMFLOAT3& v);
		float FloatLength(XMFLOAT4& v);
		float FloatLength(XMVECTOR& v);

		XMFLOAT3 Float3Add(XMFLOAT3& v1, XMFLOAT3& v2,bool add=true);
		XMFLOAT4 Float4Add(XMFLOAT4& v1,XMFLOAT4& v2, bool add = true);

		XMFLOAT3 Float3Normalize(XMFLOAT3& v);
		XMFLOAT4 Float4Normalize(XMFLOAT4& v);
		XMFLOAT3 Float3Cross(XMFLOAT3& v1, XMFLOAT3& v2);
		XMFLOAT4 Float4Cross(XMFLOAT4& v1, XMFLOAT4& v2);

		XMFLOAT3 XMFloat4to3(XMFLOAT4& v);
		XMFLOAT4 XMFloat3to4(XMFLOAT3& v);
		

		XMFLOAT4 QuaternionRotation(XMFLOAT3& Axis, float radian);
		XMFLOAT4 QuaternionMultiply(XMFLOAT4& q1, XMFLOAT4& q2);
}

using namespace MiniPhysicsEngineG9;
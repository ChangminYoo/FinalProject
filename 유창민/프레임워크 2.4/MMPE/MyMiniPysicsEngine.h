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
		void CollisionResolve(PhysicsPoint& p2,XMFLOAT3& CollisionN, float DeltaTime);
		//���ӵ��� ���Ѵ�.
		float GetSeparateVelocity(PhysicsPoint& p2, XMFLOAT3& CollisionN);
		//�浹�� �ӵ��� ���Ѵ�.
		bool GetBounce();
		void ResolveVelocity(PhysicsPoint& p2, XMFLOAT3& CollisionN, float DeltaTime);
		//������ �κ��� �о��.
		void ResolvePenetration(PhysicsPoint& p2,float DeltaTime);
	};

	class RigidBody
	{

	public:
		RigidBody();
		~RigidBody();

	protected:
		float InverseMass;//������ ����. a=1/m * F �̹Ƿ� �̷�����. ���Ժ��� ���ӵ��� �� �߿��ϴϱ�. �̶� m�� �����̸� 0�̴�.
		XMFLOAT4X4 Inverse_I_Moment;//t=Ia���� I�� �ι����Ѱ�. 

		XMFLOAT3 Velocity;//�ӵ�
		XMFLOAT3 AngularVelocity;//���ӵ�. 

								 //������ ������ �����ͷ� �����Ѵ�. �� ���� ������Ʈ�� ��ġ�� ��������!
		XMFLOAT4* CenterPos;//����
		XMFLOAT4* Orient;//����.

		float damping;//������ ��. ������ ������ε� ���
		float Angulardamping;//ȸ���� ������ ���


		XMFLOAT3 TotalForce;//���������� ������ ��� ��

							//���� ��ũ��. �̰� �߿��Ѱ� ��Ż������ ��Ż ��ũ�� ���ϸ�ȵȴ�. �������� ������� �߷°���
							//Ư�� ������ �ƴ϶� ��ü�� �������ų� �ϴ� ���� �ֱ� �����̴�. ���� Ư�� ������ ������ �������� ���ؾ���.
		XMFLOAT3 TotalTorque;

		XMFLOAT3 Accel;//���ӵ�  

		XMFLOAT3 halfbox;//x����y����z����
		bool Bounce = false;


	public:
		void integrate(float DeltaTime);//���б�. �ӵ��� ���ӵ��� ��ġ�� ���ϰ� ���ӵ��� �̿��� �ӵ��� ������.
		void SetMass(float M);//������ ���� ���� �����ϱ� ���� �Լ�. M�� ������ ������ ��������
		float GetMass(bool Inverse = true);//�⺻������ ������ ���� ����. �ٸ� false�� �θ� �״�� ���´�.

		void SetIMoment(XMFLOAT4X4& i);//�������Ʈ ����. ���ڷ� ������ �׳� �ְ� ������ �ι����������� �صд�.
		void SetIMoment(float x, float y, float z);//����ü�� �������Ʈ
		XMFLOAT4X4 GetIMoment(bool Inverse = true);//�⺻������ ���ټ��� ���Ѵ�.

		void SetDamping(float D, float Ad);//���������� ������.
		float GetDamping();//�������� ��ȯ
		float GetAngularDamping();//ȸ���������� ��ȯ

		void SetBounce(bool bounce);

		void SetPosition(XMFLOAT4* pos);//���� ��ġ ����
		void SetOrient(XMFLOAT4* ori);//���� ����

		XMFLOAT4 GetPosition();//���� ���
		XMFLOAT4 GetOrient();

		void SetVelocity(XMFLOAT3& V);//�ӵ�����. �̴� �ʱ�ӵ��̸�, ��κ��� �ӵ��� ���б⿡�� ���Ǿ��Ѵ�.
		void SetVelocity(float x, float y, float z);
		XMFLOAT3 GetVelocity();//�ӵ����

		void SetAngularVelocity(XMFLOAT3& V);//���ӵ�����. �̴� �ʱ�ӵ��̸�, ��κ��� �ӵ��� ���б⿡�� ���Ǿ��Ѵ�.
		void SetAngularVelocity(float x, float y, float z);
		XMFLOAT3 GetAngularVelocity();//�ӵ����

		void SetAccel(XMFLOAT3& A);//���ӵ� ����. �ʱ� ���ӵ��̸� ��κ��� ���ӵ��� �������� �����Ǿ���.
		void SetAccel(float x, float y, float z);
		XMFLOAT3 GetAccel();//���ӵ� ���

		void ForceClear();//��� �� �ʱ�ȭ
		void AddForce(XMFLOAT3& F);//Ư���������� ���� ���Ѵ�.
		void AddForce(float x, float y, float z);
		XMFLOAT3 GetTotalForce();//������� �� ���� ��ȯ.

								 //Ư�������� ��������. ��ü�� ���̾ƴϱ⿡ Ư�������� ���� ���Ҽ� �մ�.���� �̶� ��ũ�� ���� ���ȴ�.
		void AddForcePoint(XMFLOAT3& F, XMFLOAT3& pointposition);
		void AddForcePoint(XMFLOAT3& F, XMFLOAT4& pointposition);


		void TorqueClear();//��� �� �ʱ�ȭ
		void AddTorque(XMFLOAT3& T);//Ư���������� ��ũ�� ���Ѵ�.
		void AddTorque(float x, float y, float z);
		XMFLOAT3 GetTotalTorque();//������� �� ��ũ ��ȯ.


								  //�浹����
		float penetration = 0;//�о���ϴ� ����
		XMFLOAT3 pAxis;//�о ����

		void SetHalfBox(float x, float y, float z);
		XMFLOAT3 GetHalfBox();

		bool GetBounce();
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
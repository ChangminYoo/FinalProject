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


	public:
		
		void integrate(float DeltaTime, XMFLOAT4* ObjPos = NULL, XMFLOAT3* ObjVel = NULL);//���б�. �ӵ��� ���ӵ��� ��ġ�� ���ϰ� ���ӵ��� �̿��� �ӵ��� ������.
		void SetMass(float M);//������ ���� ���� �����ϱ� ���� �Լ�. M�� ������ ������ ��������
		float GetMass(bool Inverse = true);//�⺻������ ������ ���� ����. �ٸ� false�� �θ� �״�� ���´�.
		void SetDamping(float D);//���������� ������.
		float GetDamping();//�������� ��ȯ

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


	//********************* �ý��� Ŭ������ ********************************//

	//@ �ý��� �Լ��� ���������ӿ�ũ�� �����ؼ� ��������� ������ �ִ´�. ���� Update�Լ��� ȣ�����ָ� �ȴ�. 

	//1. �߷� �ý��� 
	class SystemGravity
	{
		XMFLOAT3 GravityAccel;//�߷°��ӵ�
	public:
		void SetGravityAccel(XMFLOAT3& ga);
		XMFLOAT3 GetGravityAccel();
		void Update(float DeltaTime, PhysicsPoint& pp);

	};




	float FloatLength(XMFLOAT3& v);
	float FloatLength(XMFLOAT4& v);
	float FloatLength(XMVECTOR& v);

	XMFLOAT3 Float3Normalize(XMFLOAT3& v);
	XMFLOAT4 Float4Normalize(XMFLOAT4& v);

	XMFLOAT3 XMFloat4to3(XMFLOAT4& v);
	XMFLOAT4 XMFloat3to4(XMFLOAT3& v);
}

using namespace MiniPhysicsEngineG9;
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
		float InverseMass;//무게의 역수. a=1/m * F 이므로 이렇게함. 무게보다 가속도가 더 중요하니까. 이때 m이 무한이면 0이다.
		XMFLOAT3 Velocity;//속도
		XMFLOAT3 CenterPos;//중점
		float damping;//제동용 값. 마찰력 대용으로도 사용
		XMFLOAT3 TotalForce;//최종적으로 가해진 모든 힘
		XMFLOAT3 Accel;//가속도
		// 힘을 통해 가속도를 구하고 가속도로 위치와 속도를 계산한다. 오일러방식을 이용함


	public:
		
		void integrate(float DeltaTime, XMFLOAT4* ObjPos = NULL, XMFLOAT3* ObjVel = NULL);//적분기. 속도와 가속도로 위치를 구하고 가속도를 이용해 속도를 갱신함.
		void SetMass(float M);//무게의 역을 쉽게 저장하기 위한 함수. M을 넣으면 역수로 저장해줌
		float GetMass(bool Inverse = true);//기본적으로 무게의 역을 구함. 다만 false로 두면 그대로 나온다.
		void SetDamping(float D);//댐핑지수를 설정함.
		float GetDamping();//댐핑지수 반환

		void SetPosition(XMFLOAT3& pos);//중점 위치 설정
		void SetPosition(XMFLOAT4& pos);//중점 위치 설정
		void SetPosition(float x, float y, float z);//중점 위치 설정
		XMFLOAT3 GetPosition();//중점 얻기

		void SetVelocity(XMFLOAT3& V);//속도설정. 이는 초기속도이며, 대부분의 속도는 적분기에서 계산되야한다.
		void SetVelocity(float x, float y, float z);
		XMFLOAT3 GetVelocity();//속도얻기

		void SetAccel(XMFLOAT3& A);//가속도 설정. 초기 가속도이며 대부분의 가속도는 힘에의해 결정되야함.
		void SetAccel(float x, float y, float z);
		XMFLOAT3 GetAccel();//가속도 얻기

		void ForceClear();//모든 힘 초기화
		void AddForce(XMFLOAT3& F);//특정방향으로 힘을 가한다.
		void AddForce(float x, float y, float z);
		XMFLOAT3 GetTotalForce();//현재까지 총 힘을 반환.


	};

	struct RayCastPlane
	{
		XMFLOAT3 Normal;//평면의 노멀
		XMFLOAT3 DefaultCenterPos;//원점에서의 평면의 중점의 위치
	};

	class RayCastObject
	{
		RayCastPlane Plane[6];

	public:
		bool RayCasting(XMFLOAT3& RayOrgin, XMFLOAT3& RayDir,XMFLOAT3& ObjCenterPos,XMFLOAT3& PlayerPos,XMFLOAT3& Look, XMFLOAT3* SavePos = NULL);
		void SetPlane(XMFLOAT3& x, XMFLOAT3& y, XMFLOAT3& z);
	};


	//********************* 시스템 클래스들 ********************************//

	//@ 시스템 함수는 메인프레임워크가 생성해서 멤버변수로 가지고 있는다. 각각 Update함수를 호출해주면 된다. 

	//1. 중력 시스템 
	class SystemGravity
	{
		XMFLOAT3 GravityAccel;//중력가속도
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
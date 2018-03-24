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

		XMFLOAT3 halfbox;//x길이y길이z길이
		bool Bounce = false;


	public:
		void integrate(float DeltaTime, XMFLOAT4* ObjPos = NULL, XMFLOAT3* ObjVel = NULL);//적분기. 속도와 가속도로 위치를 구하고 가속도를 이용해 속도를 갱신함.
		void SetMass(float M);//무게의 역을 쉽게 저장하기 위한 함수. M을 넣으면 역수로 저장해줌
		float GetMass(bool Inverse = true);//기본적으로 무게의 역을 구함. 다만 false로 두면 그대로 나온다.
		void SetDamping(float D);//댐핑지수를 설정함.
		float GetDamping();//댐핑지수 반환

		void SetBounce(bool bounce);
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

		//충돌관련
		float penetration = 0;//밀어내야하는 길이
		XMFLOAT3 pAxis;//밀어낼 방향
		void SetHalfBox(float x, float y, float z);
		XMFLOAT3 GetHalfBox();
		//인자로 받은 축을 기준으로 두개의 육면체를 투영한다음 그 길이를 합산한것을 중점연결벡터를 투영한것의 길이와 빼서 결과가 양수면 겹치고 아니면 분리된것.
		float penetrationAxis(PhysicsPoint& p2, XMFLOAT3& l1, XMFLOAT3& r1, XMFLOAT3& u1, XMFLOAT3& l2, XMFLOAT3& r2, XMFLOAT3& u2, XMFLOAT3& CenterLength, XMFLOAT3& Axis);
		//충돌했는지 여부를 검사함. 15개의 축을 검사함.
		bool CollisionTest(PhysicsPoint& p2, XMFLOAT3& l1, XMFLOAT3& r1, XMFLOAT3& u1, XMFLOAT3& l2, XMFLOAT3& r2, XMFLOAT3& u2);

		//충돌체 해소 관련. 이때 키보드 처리는 따로 해줘야함. 왜냐하면 대각선으로 눌렀을때 충돌안되는 부분은 스무스하게
		//움직이게 하기위해서다. CollisionN은 충돌시 내가 밀려나야하는 방향을 나타낸다.
		void CollisionResolve(PhysicsPoint& p2,XMFLOAT3& CollisionN, float DeltaTime);
		//상대속도를 구한다.
		float GetSeparateVelocity(PhysicsPoint& p2, XMFLOAT3& CollisionN);
		//충돌후 속도를 구한다.
		bool GetBounce();
		void ResolveVelocity(PhysicsPoint& p2, XMFLOAT3& CollisionN, float DeltaTime);
		//겹쳐진 부분을 밀어낸다.
		void ResolvePenetration(PhysicsPoint& p2,float DeltaTime);
	};

	class RigidBody
	{

	public:
		RigidBody();
		~RigidBody();

	protected:
		float InverseMass;//무게의 역수. a=1/m * F 이므로 이렇게함. 무게보다 가속도가 더 중요하니까. 이때 m이 무한이면 0이다.
		XMFLOAT4X4 Inverse_I_Moment;//t=Ia에서 I를 인버스한것. 

		XMFLOAT3 Velocity;//속도
		XMFLOAT3 AngularVelocity;//각속도. 

								 //중점과 방향은 포인터로 공유한다. 즉 실제 오브젝트의 위치와 방향으로!
		XMFLOAT4* CenterPos;//중점
		XMFLOAT4* Orient;//방향.

		float damping;//제동용 값. 마찰력 대용으로도 사용
		float Angulardamping;//회전의 마찰력 대용


		XMFLOAT3 TotalForce;//최종적으로 가해진 모든 힘

							//최종 토크력. 이거 중요한게 토탈포스로 토탈 토크를 구하면안된다. 그이유는 예를들어 중력같이
							//특정 지점이 아니라 전체에 가해지거나 하는 힘도 있기 때문이다. 따라서 특정 지점에 가해진 힘만으로 구해야함.
		XMFLOAT3 TotalTorque;

		XMFLOAT3 Accel;//가속도  

		XMFLOAT3 halfbox;//x길이y길이z길이
		bool Bounce = false;


	public:
		void integrate(float DeltaTime);//적분기. 속도와 가속도로 위치를 구하고 가속도를 이용해 속도를 갱신함.
		void SetMass(float M);//무게의 역을 쉽게 저장하기 위한 함수. M을 넣으면 역수로 저장해줌
		float GetMass(bool Inverse = true);//기본적으로 무게의 역을 구함. 다만 false로 두면 그대로 나온다.

		void SetIMoment(XMFLOAT4X4& i);//관성모멘트 설정. 인자로 넣을땐 그냥 넣고 저장은 인버스형식으로 해둔다.
		void SetIMoment(float x, float y, float z);//육면체용 관성모멘트
		XMFLOAT4X4 GetIMoment(bool Inverse = true);//기본적으로 역텐션을 구한다.

		void SetDamping(float D, float Ad);//댐핑지수를 설정함.
		float GetDamping();//댐핑지수 반환
		float GetAngularDamping();//회전댐핑지수 반환

		void SetBounce(bool bounce);

		void SetPosition(XMFLOAT4* pos);//중점 위치 설정
		void SetOrient(XMFLOAT4* ori);//방향 설정

		XMFLOAT4 GetPosition();//중점 얻기
		XMFLOAT4 GetOrient();

		void SetVelocity(XMFLOAT3& V);//속도설정. 이는 초기속도이며, 대부분의 속도는 적분기에서 계산되야한다.
		void SetVelocity(float x, float y, float z);
		XMFLOAT3 GetVelocity();//속도얻기

		void SetAngularVelocity(XMFLOAT3& V);//각속도설정. 이는 초기속도이며, 대부분의 속도는 적분기에서 계산되야한다.
		void SetAngularVelocity(float x, float y, float z);
		XMFLOAT3 GetAngularVelocity();//속도얻기

		void SetAccel(XMFLOAT3& A);//가속도 설정. 초기 가속도이며 대부분의 가속도는 힘에의해 결정되야함.
		void SetAccel(float x, float y, float z);
		XMFLOAT3 GetAccel();//가속도 얻기

		void ForceClear();//모든 힘 초기화
		void AddForce(XMFLOAT3& F);//특정방향으로 힘을 가한다.
		void AddForce(float x, float y, float z);
		XMFLOAT3 GetTotalForce();//현재까지 총 힘을 반환.

								 //특정지점에 힘을가함. 강체는 점이아니기에 특정지점에 힘을 가할수 잇다.보통 이때 토크가 같이 계산된다.
		void AddForcePoint(XMFLOAT3& F, XMFLOAT3& pointposition);
		void AddForcePoint(XMFLOAT3& F, XMFLOAT4& pointposition);


		void TorqueClear();//모든 힘 초기화
		void AddTorque(XMFLOAT3& T);//특정방향으로 토크를 가한다.
		void AddTorque(float x, float y, float z);
		XMFLOAT3 GetTotalTorque();//현재까지 총 토크 반환.


								  //충돌관련
		float penetration = 0;//밀어내야하는 길이
		XMFLOAT3 pAxis;//밀어낼 방향

		void SetHalfBox(float x, float y, float z);
		XMFLOAT3 GetHalfBox();

		bool GetBounce();
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
	XMFLOAT3 RayShot(XMFLOAT3& RayOrgin, XMFLOAT3& RayDir, float MaxLen);


	//********************* 시스템 클래스에 사용되는 힘 생성기 ********************************//

	//@ 시스템 함수는 메인프레임워크가 생성한다. 힘생성기는 멤버변수로 가지고 있는다. 각각 Update함수를 호출해주면 된다. 

	//1. 중력 시스템 에 사용될 힘생성기
	class GeneratorGravity
	{
		XMFLOAT3 GravityAccel;//중력가속도
	public:
		void SetGravityAccel(XMFLOAT3& ga);
		XMFLOAT3 GetGravityAccel();
		void Update(float DeltaTime, PhysicsPoint& pp);

	};

	//2. 고정된 스프링 힘 생성기
	class GeneratorAnchor
	{
		XMFLOAT3 AnchorPos;//고정된 위치좌표
		float constantspring;//f=k△x에서 k를 나타냄 왜 -k가아니냐면 델타x구할때 원래는 현재길이-기본길이 임
		float DefaultLength;//기본길이.△x=( DefaultLength - 현재길이 );
	public:
		void SetAnchorSpring(XMFLOAT3& a, float k, float l);//힘계산할때 -k할거니까 그냥 k를 넣을것!!!
		void Update(float DeltaTime, PhysicsPoint& pp);
	};

	//3. 점프 힘 생성기
	class GeneratorJump
	{
		XMFLOAT3 JumpVel;
	public:
		void Update(float DeltaTime, PhysicsPoint& pp);
		void SetJumpVel(XMFLOAT3& Jump);
	};






	//*********************** 편의 함수 들 *******************************************//
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
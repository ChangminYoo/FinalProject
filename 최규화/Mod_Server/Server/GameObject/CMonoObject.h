#pragma once
//#include "../PhysicsEngine/MyMiniPysicsEngine.h"
//#include "../PhysicsEngine/PhysicalEffect.h"

struct ObjectStatus
{
	int orignHP{ 100 };
	int curHP{ 100 };
	int attack{ 10 };
	int exp{ 0 };
	int level{ 1 };
	double speed;
};

class CMonoObject
{
protected:
	unsigned short		m_id{ 0 };
	char				m_fixed{ true };
	char				m_alive{ true };
	Position			m_pos4f;
	Rotation			m_rot4f;
	char				m_dir;
	char				m_ai;
	char				m_godmode{ false };
	char				m_airbone{ false };
	unsigned char	    m_type;			//스테틱 오브젝트 종류(box, wall, building...)

	//1. 오브젝트 스테이터스(공격력, 속력, 체력, 경험치, 레벨)
	ObjectStatus	    m_ability;

	//2. 룩벡터와 라이트벡터
	XMFLOAT3			m_Lookvector;//룩벡터. 오브젝트가 바라보고있는 방향.
	XMFLOAT3			m_Rightvector;//라이트벡터. 오브젝트가 바라보고있는 방향의 오른쪽방향.
	XMFLOAT3			m_Upvector; //업벡터
	XMFLOAT3			m_OffLookvector;//초기룩벡터. 오브젝트가 바라보고있는 방향.
	XMFLOAT3			m_OffRightvector;//초기라이트벡터. 오브젝트가 바라보고있는 방향의 오른쪽방향.

	//3. 물리효과
	RigidBody			*rb{ nullptr };
	PhysicsPoint		*pp{ nullptr };
	//PhysicalEffect		*pe{ nullptr };

	__int64				m_prevtime{ 0 };
	__int64				m_currtime{ 0 };
	float				m_deltime{ 0 };


public:
	CMonoObject();
	virtual void Update(float deltime);
	virtual void AfterGravitySystem(float deltime);
	virtual void GravitySystem(float deltime);
	virtual void Tick(float deltime);
	XMFLOAT3 GetRightVector() const { return m_Rightvector; }
	XMFLOAT3 GetLookVector() const { return m_Lookvector; }
	XMFLOAT3 GetUpVector() const { return m_Upvector; }
	bool	GetAirbone() const { return m_airbone; }
	void	SetAirbone(bool flag) { m_airbone = flag; }
	PhysicsPoint* GetPhysicsPoint() { return pp; }

	void UpdateUpvector();
	void UpdateLookvector();
	void UpdatePPosCenterPos();
	virtual ~CMonoObject();
};


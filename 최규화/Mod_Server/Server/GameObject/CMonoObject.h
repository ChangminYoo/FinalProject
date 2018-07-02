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
	unsigned char	    m_type;			//����ƽ ������Ʈ ����(box, wall, building...)

	//1. ������Ʈ �������ͽ�(���ݷ�, �ӷ�, ü��, ����ġ, ����)
	ObjectStatus	    m_ability;

	//2. �躤�Ϳ� ����Ʈ����
	XMFLOAT3			m_Lookvector;//�躤��. ������Ʈ�� �ٶ󺸰��ִ� ����.
	XMFLOAT3			m_Rightvector;//����Ʈ����. ������Ʈ�� �ٶ󺸰��ִ� ������ �����ʹ���.
	XMFLOAT3			m_Upvector; //������
	XMFLOAT3			m_OffLookvector;//�ʱ�躤��. ������Ʈ�� �ٶ󺸰��ִ� ����.
	XMFLOAT3			m_OffRightvector;//�ʱ����Ʈ����. ������Ʈ�� �ٶ󺸰��ִ� ������ �����ʹ���.

	//3. ����ȿ��
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


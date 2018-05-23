#pragma once
//#include "../PhysicsEngine/MyMiniPysicsEngine.h"
//#include "../PhysicsEngine/PhysicalEffect.h"

struct ObjectStatus
{
	unsigned short attack{ 10 };
	unsigned char speed{ 50 };
	unsigned short orignHP{ 100 };
	unsigned short curHP{ 100 };
	unsigned short exp{ 0 };
	unsigned char level{ 1 };
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
	virtual void Initialize();
	virtual void Update(float deltime);
	virtual void Collision();
	virtual void AfterGravitySystem(float deltime);
	virtual void GravitySystem(float deltime);
	virtual void Tick(float deltime);
	void UpdateUpvector();
	void UpdateLookvector();
	void UpdatePPosCenterPos();
	virtual ~CMonoObject();
};


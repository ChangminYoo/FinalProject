#pragma once
//#include "../PhysicsEngine/MyMiniPysicsEngine.h"
//#include "../PhysicsEngine/PhysicalEffect.h"

struct BasicInfo
{
	Position  pos;
	double    degree;
	INSTALLED_OBJECT_TYPE type;
};

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
	char				m_fixed;
	char				m_alive;
	Position			m_pos4f;
	Position			m_orgPos4f;
	Rotation			m_rot4f;
	char				m_dir;
	char				m_ai;
	char				m_godmode;
	char				m_airbone;
	unsigned char	    m_type;			//����ƽ ������Ʈ ����(box, wall, building...)
	float				m_degree;

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
	double				m_deltime{ 0 };

public:
	CMonoObject();
	virtual ~CMonoObject();

	static map<int, BasicInfo> g_objectData;

	XMFLOAT4 xmf4_pos;
	XMFLOAT4 xmf4_rot;

	PhysicsPoint* GetPhysicsPoint() { return pp; }
	RigidBody*    GetRigidBody() { return rb; }

	XMFLOAT3 GetRightVector() const { return m_Rightvector; }
	XMFLOAT3 GetLookVector() const { return m_Lookvector; }
	XMFLOAT3 GetUpVector() const { return m_Upvector; }
	bool	GetAirbone() const { return m_airbone; }
	void	SetAirbone(bool flag) { m_airbone = flag; }
	void    SetAlive(bool flag) { m_alive = flag; }
	bool    GetAlive() const { return m_alive; }
	int		GetMyCurrHP() const { return m_ability.curHP; }
	void	SetMyCurrHP(int currhp) { m_ability.curHP = currhp; }
	bool	GetIsFixed() const { return m_fixed; }
	void    SetIsFixed(bool flag) { m_fixed = flag; }
	int     GetObjectType() const { return static_cast<int>(m_type); }
	void    SetObjectType(int type) { m_type = type; }
	void	SetCenterPos4f(float x, float y, float z, float w) { m_pos4f.x = x; m_pos4f.y = y; m_pos4f.z = z; m_pos4f.w = w; }
	void	SetCenterPos3f(float x, float y, float z) { m_pos4f.x = x; m_pos4f.y = y; m_pos4f.z = z; }
	void	SetCenterPos4f(const XMFLOAT4& pos) { m_pos4f.x = pos.x; m_pos4f.y = pos.y; m_pos4f.z = pos.z; m_pos4f.w = pos.w; }
	void	SetCenterPos3f(const XMFLOAT3& pos) { m_pos4f.x = pos.x; m_pos4f.y = pos.y; m_pos4f.z = pos.z; }
	void	SetRotatePos4f(const XMFLOAT4& rot) { m_rot4f.x = rot.x; m_rot4f.y = rot.y; m_rot4f.z = rot.z; m_rot4f.w = rot.w; }
	int		GetID() const { return m_id; }

	virtual ObjectStatus GetMyBasicStatus() const;
	//virtual void ToDead();
	//virtual void ToDamage(float damage);
	virtual XMFLOAT4 GetOriginCenterPos4f() const { return XMFLOAT4(m_orgPos4f.x, m_orgPos4f.y, m_orgPos4f.z, m_orgPos4f.w); }
	virtual XMFLOAT4 GetRotatePos4f() const { return XMFLOAT4(m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w); }
	virtual XMFLOAT4 GetCenterPos4f() const { return XMFLOAT4(m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w); }
	virtual XMFLOAT3 GetCenterPos3f() const { return XMFLOAT3(m_pos4f.x, m_pos4f.y, m_pos4f.z); }

public:
	virtual void AfterGravitySystem(double deltime);
	virtual void GravitySystem(double deltime);
	virtual void Tick(double deltime);
	virtual void Tick(double deltime, Position& pos4f);

	virtual void UpdateUpvector();
	virtual void UpdateLookvector();
	virtual void UpdatePPosCenterPos();
	virtual void UpdatePRotatePos();

	virtual void UpdatePhysicsCenterPos();
	virtual void UpdateRPosCenterPos();
	virtual void UpdateRRotatePos();
	virtual void UpdateRigidCenterPos();
	virtual void UpdateRigidRotatePos();

	//��Ŷ���� ������ ���� �ش� ������Ʈ�� ��Ŷ������ ������Ʈ
	//���� �����Լ�
	virtual void UpdateDataForPacket() = 0;
};


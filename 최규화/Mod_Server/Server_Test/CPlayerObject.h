#pragma once
#include <vector>
//#include "GameObject\CMonoObject.h"

//#include "GameObject\CBulletObject.h"
//#include "CScene.h"

//class CScene;

enum PLAYER_STATE
{
	DISCONNECT,
	IDLE,
	MOVE,
	ROTATE,
	ATTACK,
	DEAD,
};

struct SKILL_SHIELD_DATA
{
	bool   once_flag;
	bool   on_using;
	double op_time;
};

struct SKILL_WAVESHOCK_DATA
{
	bool once_flag;
	bool on_using;
	bool operated;
	double op_time;

};

class CPlayerObject : public CMonoObject
{
private:
	boost::asio::ip::tcp::socket m_socket;

	unsigned short		m_id;
	char				m_connect{ false };
	char				m_state{ IDLE };
	unsigned char		m_ani;

	wchar_t				m_loginID[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t				m_loginPW[MAX_BUFFER_SIZE / 4]{ L"Guest" };

	Packet				m_recvbuf[MAX_BUFFER_SIZE]{ 0 };
	Packet				m_databuf[MAX_BUFFER_SIZE]{ 0 };

	//CBulletObject		*m_bullObj;
	float				m_prevtime{ 0 };
	float				m_elaspedtime{ 0 };
	SKILL_SHIELD_DATA	m_skill_shield;
	SKILL_WAVESHOCK_DATA m_skill_waveshock;

public:
	unsigned int		m_curr_packet_size{ 0 };
	unsigned int		m_prev_packet_size{ 0 };

	Player_Data			m_pdata;
	mutex				m_mtx;

	
	//void				Lock() { m_mtx.lock(); }
	//void				UnLock() { m_mtx.unlock(); }

public:
	CPlayerObject(const unsigned short& count, boost::asio::ip::tcp::socket socket) : m_id(count), m_socket(move(socket)) {};
	~CPlayerObject();

	// ---------------------------------------------------------------------------------------
	// [1]. ���� ��� ���� �Լ�

	// 1. �ش� ��ü�� ����� Ŭ���̾�Ʈ�� ����ϴ� ���� ���������� �Ѱ���
	boost::asio::ip::tcp::socket& Socket() { return m_socket; }

	// 2. Ŭ���̾�Ʈ�� ��Ŷ�� ���� �� ����ϴ� ��������� Send�Լ�  - async_write ���
	void SendPacket(Packet* packet);
	void SendStaticObjectPacket(const unordered_set<CStaticObject*>& sobjs);

	// 3. Ŭ���̾�Ʈ�κ��� ��Ŷ�� �޾ƿ��� ��Ŷ�� �������� �� ����ϴ� ��������� Recv�Լ� - async_read_some ���
	void RecvPacket();

	// 4. Ŭ���̾�Ʈ�� ���� ����, �׸��� ������ �� ��Ŷ������ ����� ���۸� �޾� �˸°� ó��
	void ProcessPacket(Packet* packet);

	// [2]. DB���� �Լ�

	// 1. DB �α������� üũ
	bool CheckPlayerInfo();

	// ---------------------------------------------------------------------------------------

	// [3]. �������� Ŭ���̾�Ʈ�� ������ ó���� �� ����ϴ� �Լ���
	// 1. �ʱ�ȭ (1. �������� �ʱ�ȭ 2. �÷��̾����� �ʱ�ȭ 3. �ʱ�ȭ������ ������ Ŭ���̾�Ʈ�� ����)
	void Init_MonsterInfo();
	void Init_PlayerInfo();
	void InitData_To_Client();
	void PlayerInput(double deltime);
	
	// ---------------------------------------------------------------------------------------
	// [4].��Ÿ GET - SET �Լ�
	
	Player_Data GetPlayerData() { return m_pdata; } const
	void SetPlayerData(Player_Data& pdata) { m_pdata = move(pdata); }

	int		GetID() const { return m_id; }

	void    SetState(int state) { m_state = state; }
	int		GetState() const { return m_state; }

	void	SetConnectState(char flag) { m_connect = flag; }
	bool	GetConnectState() const { return static_cast<bool>(m_connect); }

	Packet* Get_DataBuf() { return m_databuf; }
	Packet* Get_RecvBuf() { return m_recvbuf; }

	void	GetDamaged(int damage);
	bool    GetPlayerIsAlive() { return static_cast<bool>(m_alive); }
	void    SetPlayerIsAlive(char flag) { m_alive = flag; }

	void    SetIsAI(char ai) { m_ai = ai; }
	bool    GetIsAI() const { return static_cast<bool>(m_ai); }

	void    SetPlayerData_Airbone(char airbone) { m_airbone = airbone; }
	void	SetPlayerData_Pos(const XMFLOAT4& xmf4) { m_pos4f.x = xmf4.x; m_pos4f.y = xmf4.y; m_pos4f.z = xmf4.z; m_pos4f.w = xmf4.w; }
	void    SetPlayerData_Pos(const float& x, const float& y, const float& z, const float& w) { m_pos4f.x = x; m_pos4f.y = y; m_pos4f.z = z; m_pos4f.w = w; }

	void    SetChangedPlayerState();
	void    SetPlayerDirection(char dir) { m_dir = dir; }
	char	GetPlayerDirection() const   { return m_dir; }
	void	SetPlayerAnimation(unsigned char ani) { m_ani = ani; }
	unsigned char GetPlayerAnimation() const { return m_ani; }

	bool	GetShieldState() const { return m_skill_shield.on_using; }
	void	SetShieldState(bool flag) { m_skill_shield.on_using = flag; }

	bool    GetShieldOnceFlag() const { return m_skill_shield.once_flag; }
	void	SetShieldOnceFlag(bool flag) { m_skill_shield.once_flag = flag; }

	double  GetShieldCurrtime() const { return m_skill_shield.op_time; }
	void	SetShieldCurrtime(double time) { m_skill_shield.op_time = time; }
	
	bool    GetWaveshockState() const { return m_skill_waveshock.on_using; }
	void	SetWaveshockState(bool flag) { m_skill_waveshock.on_using = flag; }

	bool    GetWaveshockOnceFlag() const { return m_skill_waveshock.once_flag; }
	void	SetWaveshockOnceFlag(bool flag) { m_skill_waveshock.once_flag = flag; }

	double  GetWaveshockCurrtime() const { return m_skill_waveshock.op_time; }
	void    SetWaveshockCurrtime(double time) { m_skill_waveshock.op_time = time; }
	
	
	// ---------------------------------------------------------------------------------------
	// [5]. ����ȿ�� �Լ�

	//XMFLOAT3				GetLookVector()  const { return m_Lookvector; }
	//XMFLOAT3				GetRightVector() const { return m_Rightvector; }
	//XMFLOAT3				GetUpVector()	 const { return m_Upvector; }

	//PhysicsPoint*			GetPhysicsPoint() { return pp; }
	//PhysicalEffect*       GetPhysicsEffect() { return pe; }

	// ---------------------------------------------------------------------------------------
	//CScene				    *scene;

public:
	virtual void GravitySystem(double deltime) override;
	virtual void Tick(double deltime) override;
	virtual void Tick(double deltime, Position& pos4f) override;
	virtual void AfterGravitySystem(double deltime) override;

	void Collision(vector<CPlayerObject*>* clients, double deltime);
	void Collision(unordered_set<CStaticObject*>* sobjs, double deltime);
	void Collision_With_Waveshock();

};


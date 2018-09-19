#pragma once
#include <vector>
#include "GameObject\CMonoObject.h"
#include "GameObject\CBulletObject.h"
#include "ShareHeader.h"
#include "CScene.h"

class CScene;

enum PLAYER_STATE
{
	DISCONNECT,
	IDLE,
	MOVE,
	ROTATE,
	ATTACK,
	DEAD,
};

class CPlayerObject : public CMonoObject
{
private:
	boost::asio::ip::tcp::socket m_socket;

	unsigned short		m_id;
	char				m_connect{ false };
	char				m_state{ IDLE };
	char				m_ani;

	wchar_t				m_loginID[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t				m_loginPW[MAX_BUFFER_SIZE / 4]{ L"Guest" };

	Packet				m_recvbuf[MAX_BUFFER_SIZE]{ 0 };
	Packet				m_databuf[MAX_BUFFER_SIZE]{ 0 };

	CBulletObject		*m_bullObj;

public:
	unsigned int		m_curr_packet_size{ 0 };
	unsigned int		m_prev_packet_size{ 0 };

	Player_Data			m_pdata;

public:
	CPlayerObject(const unsigned short& count, boost::asio::ip::tcp::socket socket) : m_id(count), m_socket(move(socket)) {};
	~CPlayerObject();

	// ---------------------------------------------------------------------------------------
	// [1]. ���� ��� ���� �Լ�

	// 1. �ش� ��ü�� ����� Ŭ���̾�Ʈ�� ����ϴ� ���� ���������� �Ѱ���
	boost::asio::ip::tcp::socket& Socket() { return m_socket; }

	// 2. Ŭ���̾�Ʈ�� ��Ŷ�� ���� �� ����ϴ� ��������� Send�Լ�  - async_write ���
	void SendPacket(Packet* packet);

	// 3. Ŭ���̾�Ʈ�κ��� ��Ŷ�� �޾ƿ��� ��Ŷ�� �������� �� ����ϴ� ��������� Recv�Լ� - async_read_some ���
	void RecvPacket();

	// 4. Ŭ���̾�Ʈ�� ���� ����, �׸��� ������ �� ��Ŷ������ ����� ���۸� �޾� �˸°� ó��
	void ProcessPacket(Packet* packet);

	// ---------------------------------------------------------------------------------------
	virtual void GravitySystem(double deltime);
	virtual void Tick(double deltime);
	virtual void AfterGravitySystem(double deltime);

	// ---------------------------------------------------------------------------------------
	// [2]. DB���� �Լ�

	// 1. DB �α������� üũ
	bool CheckPlayerInfo();

	// ---------------------------------------------------------------------------------------
	
	// [3]. �������� Ŭ���̾�Ʈ�� ������ ó���� �� ����ϴ� �Լ���
	// 1. �ʱ�ȭ (1. �������� �ʱ�ȭ 2. �÷��̾����� �ʱ�ȭ 3. �ʱ�ȭ������ ������ Ŭ���̾�Ʈ�� ����)
	void Init_MonsterInfo();
	void Init_PlayerInfo();
	void InitData_To_Client();
	void RegularUpdate();

	//void SendStaticObjects(const unordered_set<StaticObject*>& sobjudset);

	// ---------------------------------------------------------------------------------------
	// [4].��Ÿ GET - SET �Լ�
	Player_Data GetPlayerData() { return m_pdata; } const
	void SetPlayerData(Player_Data& pdata) { m_pdata = move(pdata); }

	int		GetID() const { return m_id; }

	void    SetState(int state) { m_state = state; }
	int		GetState() const { return m_state; }

	void	SetConnectState(bool flag) { m_connect = flag; }
	bool	GetConnectState() const { return m_connect; }

	Packet* Get_DataBuf() { return m_databuf; }
	Packet* Get_RecvBuf() { return m_recvbuf; }

	void	Damaged(float damage);

	bool    GetIsAI() const { return m_ai; }

	void    SetPlayerData_Airbone(char airbone) { m_airbone = airbone; }
	void	SetPlayerData_Pos(const XMFLOAT4& xmf4) { m_pos4f.x = xmf4.x; m_pos4f.y = xmf4.y; m_pos4f.z = xmf4.z; m_pos4f.w = xmf4.w; }
	void    SetPlayerData_Pos(const float& x, const float& y, const float& z, const float& w) { m_pos4f.x = x; m_pos4f.y = y; m_pos4f.z = z; m_pos4f.w = w; }

	// ---------------------------------------------------------------------------------------
	// [5]. ����ȿ�� �Լ�

	XMFLOAT3				GetLookVector()  const { return m_Lookvector; }
	XMFLOAT3				GetRightVecotr() const { return m_Rightvector; }
	XMFLOAT3				GetUpVector()	 const { return m_Upvector; }

	PhysicsPoint*			GetPhysicsPoint() { return pp; }
	PhysicalEffect*         GetPhysicsEffect() { return pe; }

	// ---------------------------------------------------------------------------------------
	CScene				    *scene;

};


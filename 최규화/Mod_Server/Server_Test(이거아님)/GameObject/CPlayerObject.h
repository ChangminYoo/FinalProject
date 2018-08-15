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
	// [1]. 서버 통신 관련 함수

	// 1. 해당 객체로 연결된 클라이언트와 통신하는 서버 소켓정보를 넘겨줌
	boost::asio::ip::tcp::socket& Socket() { return m_socket; }

	// 2. 클라이언트로 패킷을 보낼 때 사용하는 사용자정의 Send함수  - async_write 사용
	void SendPacket(Packet* packet);

	// 3. 클라이언트로부터 패킷을 받아오고 패킷을 재조립할 때 사용하는 사용자정의 Recv함수 - async_read_some 사용
	void RecvPacket();

	// 4. 클라이언트로 부터 받은, 그리고 재조립 된 패킷정보가 저장된 버퍼를 받아 알맞게 처리
	void ProcessPacket(Packet* packet);

	// ---------------------------------------------------------------------------------------
	virtual void GravitySystem(double deltime);
	virtual void Tick(double deltime);
	virtual void AfterGravitySystem(double deltime);

	// ---------------------------------------------------------------------------------------
	// [2]. DB관련 함수

	// 1. DB 로그인정보 체크
	bool CheckPlayerInfo();

	// ---------------------------------------------------------------------------------------
	
	// [3]. 서버에서 클라이언트의 정보를 처리할 때 사용하는 함수들
	// 1. 초기화 (1. 몬스터정보 초기화 2. 플레이어정보 초기화 3. 초기화정보를 접속한 클라이언트로 보냄)
	void Init_MonsterInfo();
	void Init_PlayerInfo();
	void InitData_To_Client();
	void RegularUpdate();

	//void SendStaticObjects(const unordered_set<StaticObject*>& sobjudset);

	// ---------------------------------------------------------------------------------------
	// [4].기타 GET - SET 함수
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
	// [5]. 물리효과 함수

	XMFLOAT3				GetLookVector()  const { return m_Lookvector; }
	XMFLOAT3				GetRightVecotr() const { return m_Rightvector; }
	XMFLOAT3				GetUpVector()	 const { return m_Upvector; }

	PhysicsPoint*			GetPhysicsPoint() { return pp; }
	PhysicalEffect*         GetPhysicsEffect() { return pe; }

	// ---------------------------------------------------------------------------------------
	CScene				    *scene;

};


#pragma once
#include "Database\CTextTest.h"
#include "PhysicalEffect.h"
#include "StaticObject.h"
#include "ShareHeader.h"

enum PLAYER_STATE 
{
	IDLE,
	MOVE,
	ROTATE,
	ATTACK,
	DEAD,
};

class Player;
class StaticObject;
class BulletObject;

class Player_Session
{
private:
	//0. 통신을 위한 소켓 정보
	boost::asio::ip::tcp::socket m_socket;

	//1. 플레이어 정보
	unsigned short				 m_id{ 0 };
	char						 m_connect_state{ false };
	char					     m_state{ -1 };
	char						 m_isAI{ false };

	wchar_t						 m_loginID[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t						 m_loginPW[MAX_BUFFER_SIZE / 4]{ L"Guest" };


	Player_Data                  m_playerData;	 // 지금 이 클라이언트 객체가 관리하는 플레이어 정보(패킷으로 주고받는)
	PLAYERS                      m_playerType;
	//PLAYER_OBJECT_TYPE			 m_myObjType;


	//2. 해당 클라이언트의 패킷 데이터(recv할 때 사용 )
	Packet						 m_recvBuf[MAX_BUFFER_SIZE]{ 0 };		// recv 할 때 넘어오는 패킷정보를 저장
	Packet						 m_dataBuf[MAX_BUFFER_SIZE]{ 0 };       // recv 한 뒤 넘어온 패킷을 재조립할 때 사용


	//3. 몬스터 정보(1. 몬스터타입)																 
	MONSTERS				     m_monsterType;

	//4. 물리효과 처리
	RigidBody					 *rb{ nullptr };
	PhysicsPoint				 *pp{ nullptr };
	PhysicalEffect				 *pe{ nullptr };

	//5. 고정된 물체인가
	bool						  staticobject{ false };

	//6. 점프상태인가
	bool						  AirBone{ false };

	//7. 룩벡터와 라이트벡터
	XMFLOAT3					  Lookvector;//룩벡터. 오브젝트가 바라보고있는 방향.
	XMFLOAT3					  Rightvector;//라이트벡터. 오브젝트가 바라보고있는 방향의 오른쪽방향.
	XMFLOAT3					  Upvector; //업벡터
	XMFLOAT3					  OffLookvector;//초기룩벡터. 오브젝트가 바라보고있는 방향.
	XMFLOAT3					  OffRightvector;//초기라이트벡터. 오브젝트가 바라보고있는 방향의 오른쪽방향.

	//8. 해당 오브젝트가 죽었나
	bool						  delobj{ false };

	//9. 불렛 생성(클라이언트가 공격키를 눌러 서버에서 불렛 한개 생성)
	BulletObject				 *m_bullObj;

	//10. 레이캐스팅 처리
	RayCastObject				  rco;

	//11. RegularUpdate함수를 위한 타이머변수 등록
	float m_prevTime{ 0.f };
	float m_ElaspedTime{ 0.f };


public:
	unsigned int m_cur_packet_size{ 0 };
	unsigned int m_prev_packet_size{ 0 };
	
	
public:
	Player_Session(const short& count, boost::asio::ip::tcp::socket socket) : m_id(count), m_socket(move(socket))
	{};

	~Player_Session();


	// ---------------------------------------------------------------------------------------
	// [1]. 서버 통신 관련 함수

	// 1. 해당 객체로 연결된 클라이언트와 통신하는 서버 소켓정보를 넘겨줌
	boost::asio::ip::tcp::socket& Socket() { return m_socket;}

	// 2. 클라이언트로 패킷을 보낼 때 사용하는 사용자정의 Send함수  - async_write 사용
	void SendPacket(Packet* packet);

	// 3. 클라이언트로부터 패킷을 받아오고 패킷을 재조립할 때 사용하는 사용자정의 Recv함수 - async_read_some 사용
	void RecvPacket();

	// 4. 클라이언트로 부터 받은, 그리고 재조립 된 패킷정보가 저장된 버퍼를 받아 알맞게 처리
	void ProcessPacket(Packet* packet);

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

	void SendStaticObjects(const unordered_set<StaticObject*>& sobjudset);

	// ---------------------------------------------------------------------------------------
	// [4].기타 GET - SET 함수
	Player_Data GetPlayerData() { return m_playerData; } const

	int		Get_ID() const { return m_id; }

	void    Set_State(int state);
	int		Get_State() const { return m_state; }

	void	Set_Connect_State(bool flag) { m_connect_state = flag; }
	bool	Get_Connect_State() const { return m_connect_state; }

	void    Set_DataBuf(Packet* buf) { memcpy(m_dataBuf, buf, sizeof(buf)); }

	Packet* Get_DataBuf() { return m_dataBuf; }
	Packet* Get_RecvBuf() { return m_recvBuf; }

	bool	Get_IsAI() const { return m_isAI; } 

	vector<Player_Session*>* GetPlayerSession() { return &m_clients; }

	void	Damaged(float damage);
	// ---------------------------------------------------------------------------------------
	// [5]. 물리효과 함수

	XMFLOAT3				GetLookVector()  const { return Lookvector;  }
	XMFLOAT3				GetRightVecotr() const { return Rightvector; }
	XMFLOAT3				GetUpVector()	 const { return Upvector;    }

	PhysicsPoint*			GetPhysicsPoint() { return pp; }
	

	// ---------------------------------------------------------------------------------------
	// 서버에서 관리하는 클라이언트 객체들의 집합(vector 사용 - 나중에 멀쓰때 맞는 자료구조로 바꿀것)
	static vector<Player_Session*> m_clients;
	static list<BulletObject*>     m_bullobjs;
	static unordered_set<StaticObject*> m_sobjs;

	static int m_tempcount;
	int t_cnt{ 0 };
	float prevTime = 0.f, curTime = 0.f, elapsedTime = 0.f;

	//1. 플레이어와 스테틱 오브젝트들의 충돌
	void Collision_StaticObjects(unordered_set<StaticObject*>& sobjs, float DeltaTime);
	void Collision_Players(vector<Player_Session*>& clients, float DeltaTime);

	list<BulletObject*>& GetBulletObjectsList() { return m_bullobjs; }

	// ----------------------------------------------------------------
	// [6]. 주기적인 업데이트를 위한 함수 RegularUpdate()
	// 1초에 20번의 패킷을 보냄 - 모든 이동관련 작업들(캐릭이동, 불렛이동)

	void RegularUpdate();
};



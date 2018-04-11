#pragma once
#include "stdafx.h"
#include "Player.h"
#include "StaticObject.h"
#include "Database\CTextTest.h"
#include "PhysicsEngine\MyMiniPysicsEngine.h"

enum PLAYER_STATE 
{
	IDLE,
	MOVE,
	ROTATE,
	ATTACK,
	DEAD,
};

//enum PLAYER_OBJECT_TYPE
//{
//	PLAYER_OBJECT,
//	STATIC_OBJECT,
//	BULLET_OBJECT,
//};

class Player;
class StaticObject;

class Player_Session
{
protected:
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
	RigidBody					 *rb = nullptr;
	PhysicsPoint				 *pp = nullptr;

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
	void						  GetUpvector();//룩벡터와 라이트벡터를 이용해 업벡터를 얻는함수

	//8. 해당 오브젝트가 죽었나
	bool						  Delobj{ false };

public:
	unsigned int m_cur_packet_size{ 0 };
	unsigned int m_prev_packet_size{ 0 };
	
	StaticObject				  *m_staticobject{ nullptr };
	static bool					   m_InitFirst_SObjs;

public:
	Player_Session(const int& count, boost::asio::ip::tcp::socket socket) : m_id(count), m_socket(move(socket))
	{};
	~Player_Session() {};


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

	void UpdateLookVector();

	// 3. 움직이지 않는 오브젝트 생성 (예: 상자, 맵)
	//void Init_StaticObject();

	// ---------------------------------------------------------------------------------------
	// [4].기타 GET - SET 함수
	Player_Data GetPlayerData() { return m_playerData; } const

	int Get_ID() const { return m_id; }

	void Set_State(int state);
	int Get_State() const { return m_state; }

	void Set_Connect_State(bool flag) { m_connect_state = flag; }
	bool Get_Connect_State() const { return m_connect_state; }

	void Set_DataBuf(Packet* buf) { memcpy(m_dataBuf, buf, sizeof(buf)); }

	Packet* Get_DataBuf() { return m_dataBuf; }
	Packet* Get_RecvBuf() { return m_recvBuf; }

	bool Get_IsAI() const { return m_isAI; }

	// ---------------------------------------------------------------------------------------
	// 서버에서 관리하는 클라이언트 객체들의 집합(vector 사용 - 나중에 멀쓰때 맞는 자료구조로 바꿀것)
	static vector<Player_Session*> m_clients;
	static list<Player_Session*> m_staticobjs;

};



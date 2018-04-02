#pragma once
#include "stdafx.h"
#include "Database\CTextTest.h"
#include "Player.h"

enum PLAYER_STATE 
{
	IDLE,
	MOVE,
	ATTACK,
	DEAD,
};

class Player;

class Player_Session
{
private:
	//0. 통신을 위한 소켓 정보
	boost::asio::ip::tcp::socket m_socket;

	//삭제
	string m_WriteMessage;
	array<char, 128> m_ReceiveBuffer;

	//1. 플레이어 정보
	unsigned short				 m_id{ 0 };
	char						 m_connect_state{ false };
	char					     m_state{ -1 };
	char						 m_isAI{ false };

	wchar_t						 m_loginID[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t						 m_loginPW[MAX_BUFFER_SIZE / 4]{ L"Guest" };


	Player_Data                  m_playerData;	 // 지금 이 클라이언트 객체가 관리하는 플레이어 정보(패킷으로 주고받는)
	PLAYERS                      m_playerType;


	//2. 해당 클라이언트의 패킷 데이터(recv할 때 사용 )
	Packet						 m_recvBuf[MAX_BUFFER_SIZE]{ 0 };		// recv 할 때 넘어오는 패킷정보를 저장
	Packet						 m_dataBuf[MAX_BUFFER_SIZE]{ 0 };       // recv 한 뒤 넘어온 패킷을 재조립할 때 사용


	//3. 몬스터 정보(1. 몬스터타입)																 
	MONSTERS m_monsterType;

	//삭제
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);

	// 로그인 데이터베이스
	//CTextTest* m_ploginData;
	//Packet *m_new_sendBuf;

	//Player* m_player;

public:
	unsigned int m_cur_packet_size{ 0 };
	unsigned int m_prev_packet_size{ 0 };

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

	//삭제
	void PostReceive();
	//void Set_MyPlayerData(Player_Data& playerdata) { m_playerData = move(playerdata); }
};



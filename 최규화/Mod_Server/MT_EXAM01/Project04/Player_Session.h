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
	boost::asio::ip::tcp::socket m_socket;
	string m_WriteMessage;
	array<char, 128> m_ReceiveBuffer;

	int m_id{ 0 };
	bool m_connect_state{ false };
	int m_state{ -1 };

	wchar_t m_loginID[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t m_loginPW[MAX_BUFFER_SIZE / 4]{ L"Guest" };

	Player_Data m_playerData;
	PLAYERS m_playerType;

	Packet m_recvBuf[MAX_BUFFER_SIZE]{ 0 };
	Packet m_dataBuf[MAX_BUFFER_SIZE]{ 0 };

	unsigned int m_cur_packet_size{ 0 };
	unsigned int m_prev_packet_size{ 0 };

	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);

	// 로그인 데이터베이스
	//CTextTest* m_ploginData;

	// 몬스터 
	MONSTERS m_monsterType;

	//Player* m_player;

public:
	Player_Session(const int& count, boost::asio::ip::tcp::socket socket) : m_id(count), m_socket(move(socket))
	{};
	~Player_Session() {};

	static vector<Player_Session*> m_clients;

	Player_Data* GetPlayerData() { return &m_playerData; } const

	boost::asio::ip::tcp::socket& Socket() { return m_socket;}

	void Init_MonsterInfo();
	void Init_PlayerInfo();
	void InitData_To_Client();
	bool CheckPlayerInfo();

	int Get_ID() const { return m_id; }

	void SendPacket(Packet* packet);
	void RecvPacket();

	void ProcessPacket(Packet* packet);

	void Set_State(int state);
	int Get_State() const { return m_state; }

	void Set_Connect_State(bool flag) { m_connect_state = flag; }
	bool Get_Connect_State() const { return m_connect_state; }
	void PostReceive();

	Packet* Get_RecvBuf() { return m_recvBuf; }
};



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
	//0. ����� ���� ���� ����
	boost::asio::ip::tcp::socket m_socket;

	//����
	string m_WriteMessage;
	array<char, 128> m_ReceiveBuffer;

	//1. �÷��̾� ����
	unsigned short				 m_id{ 0 };
	char						 m_connect_state{ false };
	char					     m_state{ -1 };
	char						 m_isAI{ false };

	wchar_t						 m_loginID[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t						 m_loginPW[MAX_BUFFER_SIZE / 4]{ L"Guest" };


	Player_Data                  m_playerData;	 // ���� �� Ŭ���̾�Ʈ ��ü�� �����ϴ� �÷��̾� ����(��Ŷ���� �ְ�޴�)
	PLAYERS                      m_playerType;


	//2. �ش� Ŭ���̾�Ʈ�� ��Ŷ ������(recv�� �� ��� )
	Packet						 m_recvBuf[MAX_BUFFER_SIZE]{ 0 };		// recv �� �� �Ѿ���� ��Ŷ������ ����
	Packet						 m_dataBuf[MAX_BUFFER_SIZE]{ 0 };       // recv �� �� �Ѿ�� ��Ŷ�� �������� �� ���


	//3. ���� ����(1. ����Ÿ��)																 
	MONSTERS m_monsterType;

	//����
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);

	// �α��� �����ͺ��̽�
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
	// [1]. ���� ��� ���� �Լ�

	// 1. �ش� ��ü�� ����� Ŭ���̾�Ʈ�� ����ϴ� ���� ���������� �Ѱ���
	boost::asio::ip::tcp::socket& Socket() { return m_socket;}

	// 2. Ŭ���̾�Ʈ�� ��Ŷ�� ���� �� ����ϴ� ��������� Send�Լ�  - async_write ���
	void SendPacket(Packet* packet);

	// 3. Ŭ���̾�Ʈ�κ��� ��Ŷ�� �޾ƿ��� ��Ŷ�� �������� �� ����ϴ� ��������� Recv�Լ� - async_read_some ���
	void RecvPacket();

	// 4. Ŭ���̾�Ʈ�� ���� ����, �׸��� ������ �� ��Ŷ������ ����� ���۸� �޾� �˸°� ó��
	void ProcessPacket(Packet* packet);

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


	// ---------------------------------------------------------------------------------------
	// [4].��Ÿ GET - SET �Լ�
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
	// �������� �����ϴ� Ŭ���̾�Ʈ ��ü���� ����(vector ��� - ���߿� �־��� �´� �ڷᱸ���� �ٲܰ�)
	static vector<Player_Session*> m_clients;

	//����
	void PostReceive();
	//void Set_MyPlayerData(Player_Data& playerdata) { m_playerData = move(playerdata); }
};



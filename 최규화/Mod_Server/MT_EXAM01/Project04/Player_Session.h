#pragma once
#include "stdafx.h"
#include "Player.h"
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
	//0. ����� ���� ���� ����
	boost::asio::ip::tcp::socket m_socket;

	//1. �÷��̾� ����
	unsigned short				 m_id{ 0 };
	char						 m_connect_state{ false };
	char					     m_state{ -1 };
	char						 m_isAI{ false };

	wchar_t						 m_loginID[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t						 m_loginPW[MAX_BUFFER_SIZE / 4]{ L"Guest" };


	Player_Data                  m_playerData;	 // ���� �� Ŭ���̾�Ʈ ��ü�� �����ϴ� �÷��̾� ����(��Ŷ���� �ְ�޴�)
	PLAYERS                      m_playerType;
	//PLAYER_OBJECT_TYPE			 m_myObjType;


	//2. �ش� Ŭ���̾�Ʈ�� ��Ŷ ������(recv�� �� ��� )
	Packet						 m_recvBuf[MAX_BUFFER_SIZE]{ 0 };		// recv �� �� �Ѿ���� ��Ŷ������ ����
	Packet						 m_dataBuf[MAX_BUFFER_SIZE]{ 0 };       // recv �� �� �Ѿ�� ��Ŷ�� �������� �� ���


	//3. ���� ����(1. ����Ÿ��)																 
	MONSTERS				     m_monsterType;

	//4. ����ȿ�� ó��
	RigidBody					 *rb = nullptr;
	PhysicsPoint				 *pp = nullptr;

	//5. ������ ��ü�ΰ�
	bool						  staticobject{ false };

	//6. ���������ΰ�
	bool						  AirBone{ false };

	//7. �躤�Ϳ� ����Ʈ����
	XMFLOAT3					  Lookvector;//�躤��. ������Ʈ�� �ٶ󺸰��ִ� ����.
	XMFLOAT3					  Rightvector;//����Ʈ����. ������Ʈ�� �ٶ󺸰��ִ� ������ �����ʹ���.
	XMFLOAT3					  Upvector; //������
	XMFLOAT3					  OffLookvector;//�ʱ�躤��. ������Ʈ�� �ٶ󺸰��ִ� ����.
	XMFLOAT3					  OffRightvector;//�ʱ����Ʈ����. ������Ʈ�� �ٶ󺸰��ִ� ������ �����ʹ���.
	void						  GetUpvector();//�躤�Ϳ� ����Ʈ���͸� �̿��� �����͸� ����Լ�

	//8. �ش� ������Ʈ�� �׾���
	bool						  Delobj{ false };

	unordered_map<int, Position>  m_boxPos;

public:
	unsigned int m_cur_packet_size{ 0 };
	unsigned int m_prev_packet_size{ 0 };
	
	//StaticObject				  *m_staticobject{ nullptr };
	
public:
	Player_Session(const short& count, boost::asio::ip::tcp::socket socket) : m_id(count), m_socket(move(socket))
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

	//void InitStaticObjects(boost::asio::ip::tcp::socket my_sock);
	//void InitStaticObjects();
	//void SET_PosOfBox();
	void SendStaticObjects(const list<StaticObject*>& SObjList);

	void UpdateLookVector();

	// 3. �������� �ʴ� ������Ʈ ���� (��: ����, ��)
	//void Init_StaticObject();

	// ---------------------------------------------------------------------------------------
	// [4].��Ÿ GET - SET �Լ�
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

	// ---------------------------------------------------------------------------------------
	// �������� �����ϴ� Ŭ���̾�Ʈ ��ü���� ����(vector ��� - ���߿� �־��� �´� �ڷᱸ���� �ٲܰ�)
	static vector<Player_Session*> m_clients;
	static list<Player_Session*> m_staticobjs;

	int t_cnt{ 0 };
	void Update_Temp();
	float prevTime = 0.f, curTime = 0.f, elapsedTime = 0.f;
};



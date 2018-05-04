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
	RigidBody					 *rb{ nullptr };
	PhysicsPoint				 *pp{ nullptr };
	PhysicalEffect				 *pe{ nullptr };

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

	//8. �ش� ������Ʈ�� �׾���
	bool						  delobj{ false };

	//9. �ҷ� ����(Ŭ���̾�Ʈ�� ����Ű�� ���� �������� �ҷ� �Ѱ� ����)
	BulletObject				 *m_bullObj;

	//10. ����ĳ���� ó��
	RayCastObject				  rco;

	//11. RegularUpdate�Լ��� ���� Ÿ�̸Ӻ��� ���
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

	void SendStaticObjects(const unordered_set<StaticObject*>& sobjudset);

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

	vector<Player_Session*>* GetPlayerSession() { return &m_clients; }

	void	Damaged(float damage);
	// ---------------------------------------------------------------------------------------
	// [5]. ����ȿ�� �Լ�

	XMFLOAT3				GetLookVector()  const { return Lookvector;  }
	XMFLOAT3				GetRightVecotr() const { return Rightvector; }
	XMFLOAT3				GetUpVector()	 const { return Upvector;    }

	PhysicsPoint*			GetPhysicsPoint() { return pp; }
	

	// ---------------------------------------------------------------------------------------
	// �������� �����ϴ� Ŭ���̾�Ʈ ��ü���� ����(vector ��� - ���߿� �־��� �´� �ڷᱸ���� �ٲܰ�)
	static vector<Player_Session*> m_clients;
	static list<BulletObject*>     m_bullobjs;
	static unordered_set<StaticObject*> m_sobjs;

	static int m_tempcount;
	int t_cnt{ 0 };
	float prevTime = 0.f, curTime = 0.f, elapsedTime = 0.f;

	//1. �÷��̾�� ����ƽ ������Ʈ���� �浹
	void Collision_StaticObjects(unordered_set<StaticObject*>& sobjs, float DeltaTime);
	void Collision_Players(vector<Player_Session*>& clients, float DeltaTime);

	list<BulletObject*>& GetBulletObjectsList() { return m_bullobjs; }

	// ----------------------------------------------------------------
	// [6]. �ֱ����� ������Ʈ�� ���� �Լ� RegularUpdate()
	// 1�ʿ� 20���� ��Ŷ�� ���� - ��� �̵����� �۾���(ĳ���̵�, �ҷ��̵�)

	void RegularUpdate();
};



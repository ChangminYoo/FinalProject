#pragma once
#include "stdafx.h"
#include "Database\CTextTest.h"
#include "..\..\CGameObject.h"

class Scene;

struct AnimationCheck
{
	Time t;
	Ani_State AniState{ Ani_State::Idle };
};

struct RotateCheck
{
	Time t;
	Rotation RotationInfo;
};

struct PositionCheck
{
	Time t;
	Position PositionInfo;
	Ani_State AniState{ Ani_State::Idle };
	float Deltime;
};

struct RegularCheckInfo
{
	RotateCheck RtCheck;
	PositionCheck PtCheck;
	AnimationCheck AniCheck;
};

struct RecvLoginData
{
	int my_id;
	int textureID;
};

class AsyncClient
{
private:
	boost::asio::ip::tcp::socket m_socket;
	
	Packet m_recvBuf[MAX_BUFFER_SIZE]{ 0 };
	Packet m_dataBuf[MAX_BUFFER_SIZE]{ 0 };

	unsigned int m_cur_packet_size{ 0 };
	unsigned int m_prev_packet_size{ 0 };

	CTextTest* m_cur_logindata;

	CGameObject* m_myObj;
	Scene* m_myScene;

	float m_totalTime{ 0.f };
	float m_loadSceneTotalTime{ 0.f };
	float m_loginTotalTime{ 0.f };
	char m_move_prevdir{ 0 };

public:
	AsyncClient() : m_socket(g_io_service) {};
	~AsyncClient();

	bool m_myClientReady{ false };
	bool m_mySkipLogin{ false };
	bool m_selCharacterComplete{ false };
	int	 m_myTextureID;

	double m_myClient_StageTime{ 0.0 };

	vector<RecvLoginData> m_lobbyData;
	vector<RecvLoginData> m_clientsID;
	
	void Init(CGameObject* obj, Scene* scene);

	void Connect(boost::asio::ip::tcp::endpoint& endpoint);

	void RecvPacket(CGameObject& obj, Scene& scene);
	void SendPacket(Packet* packet);

	void ProcessPacket(Packet* packet, CGameObject& obj, Scene& scene);

	void SendPacketRegular(CGameObject& gobj , const GameTimer& gt);
	void SendPacketRegular(const GameTimer& gt);
	void SendPacketLoadScenePacketRegular(const GameTimer& gt);

	//�긦 databuf -> recvbuf�� �ٲٴ� ����� ���� �������� ���޵�
	//recv �Լ����� �ӽù��ۿ� �������� ���� �����͸� ���ʷ� ��°������� 
	//�ٷ� ���μ�����Ŷ�� �������� ���� �����͸� ������ ������ ������...
	Packet* Get_RecvBuf() { return m_recvBuf; }

	RegularCheckInfo RgCkInfo;
	bool			 m_start_attack{ false };
	int				 m_curr_skillnum{ -1};
	
	//���� - frameadvance���� �������� ���� �����Ͱ� processpacket �Լ��� ������
	//�ش� ����� gameobject�� �����͸� dynamicobject�� �־��ٰ�����? -> �������� �����͸� ���� ������
	//����� �����͵��� ���÷� ã�ư��� �ٲ����
	//processpacket ���ο��� ����� �����͸� �ٷ� ���ڷ� ���� dynamicobject�� �־��ٰ�����


	void SetMovePrevDir(char dir) { m_move_prevdir = dir; }
	char GetMovePrevDir() const { return m_move_prevdir; }

};


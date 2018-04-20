#pragma once
#include "stdafx.h"
#include "Database\CTextTest.h"
#include "..\..\CGameObject.h"

class Scene;

#define MAX_ROTATE_PACKET_TIME 0.05
#define MAX_POSITION_MOVE_PACKET_TIME 0.05

struct RotateCheck
{
	Time t;
	Rotation RotationInfo;
};

struct PositionCheck
{
	Time t;
	Position PositionInfo;
	Ani_State AniState;
	float Deltime;
};

struct RegularCheckInfo
{
	RotateCheck RtCheck;
	PositionCheck PtCheck;
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

	float m_totalTime{ 0.0f };

public:
	AsyncClient() : m_socket(g_io_service) {};
	~AsyncClient();

	void Init(CGameObject* obj, Scene* scene);

	void Connect(boost::asio::ip::tcp::endpoint& endpoint);

	void RecvPacket(CGameObject& obj, Scene& scene);
	void SendPacket(Packet* packet);

	void ProcessPacket(Packet* packet, CGameObject& obj, Scene& scene);

	void SendPacketRegular(CGameObject& gobj , const GameTimer& gt);

	//�긦 databuf -> recvbuf�� �ٲٴ� ����� ���� �������� ���޵�
	//recv �Լ����� �ӽù��ۿ� �������� ���� �����͸� ���ʷ� ��°������� 
	//�ٷ� ���μ�����Ŷ�� �������� ���� �����͸� ������ ������ ������...
	Packet* Get_RecvBuf() { return m_recvBuf; }

	RegularCheckInfo RgCkInfo;
	
	//���� - frameadvance���� �������� ���� �����Ͱ� processpacket �Լ��� ������
	//�ش� ����� gameobject�� �����͸� dynamicobject�� �־��ٰ�����? -> �������� �����͸� ���� ������
	//����� �����͵��� ���÷� ã�ư��� �ٲ����
	//processpacket ���ο��� ����� �����͸� �ٷ� ���ڷ� ���� dynamicobject�� �־��ٰ�����

};


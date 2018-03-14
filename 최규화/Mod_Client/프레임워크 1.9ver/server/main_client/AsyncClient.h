#include "stdafx.h"
#include "Database\CTextTest.h"
#include "../../CGameObject.h"
#include "../../Scene.h"

class AsyncClient
{
private:
	boost::asio::ip::tcp::socket m_socket;
	
	Packet m_recvBuf[MAX_BUFFER_SIZE]{ 0 };
	Packet m_dataBuf[MAX_BUFFER_SIZE]{ 0 };

	unsigned int m_cur_packet_size{ 0 };
	unsigned int m_prev_packet_size{ 0 };

	CTextTest* m_cur_logindata;
public:
	AsyncClient() : m_socket(g_io_service) {};
	~AsyncClient();

	void Init();

	void Connect(boost::asio::ip::tcp::endpoint& endpoint);

	void RecvPacket();
	void SendPacket(Packet* packet);

	void ProcessPacket(Packet* packet, CGameObject* obj, Scene& scene);

	//�긦 databuf -> recvbuf�� �ٲٴ� ����� ���� �������� ���޵�
	//recv �Լ����� �ӽù��ۿ� �������� ���� �����͸� ���ʷ� ��°������� 
	//�ٷ� ���μ�����Ŷ�� �������� ���� �����͸� ������ ������ ������...
	Packet* Get_RecvBuf() { return m_recvBuf; }
	
	//���� - frameadvance���� �������� ���� �����Ͱ� processpacket �Լ��� ������
	//�ش� ����� gameobject�� �����͸� dynamicobject�� �־��ٰ�����? -> �������� �����͸� ���� ������
	//����� �����͵��� ���÷� ã�ư��� �ٲ����
	//processpacket ���ο��� ����� �����͸� �ٷ� ���ڷ� ���� dynamicobject�� �־��ٰ�����

	void ReUpdateData();


};


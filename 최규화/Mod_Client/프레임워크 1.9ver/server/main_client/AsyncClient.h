#pragma once
#include "stdafx.h"
#include "Database\CTextTest.h"
#include "../../CGameObject.h"

class Scene;

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

	//얘를 databuf -> recvbuf로 바꾸니 제대로 값이 서버에서 전달됨
	//recv 함수에서 임시버퍼에 서버에서 받은 데이터를 차례로 담는과정없이 
	//바로 프로세스패킷에 서버에서 받은 데이터를 전달한 상태임 지금은...
	Packet* Get_RecvBuf() { return m_recvBuf; }
	
	//유보 - frameadvance에서 서버에서 받은 데이터가 processpacket 함수를 지나서
	//해당 변경된 gameobject의 데이터를 dynamicobject에 넣어줄것인지? -> 서버에서 데이터를 받을 때마다
	//변경된 데이터들을 수시로 찾아가며 바꿔야함
	//processpacket 내부에서 변경된 데이터를 바로 인자로 받은 dynamicobject에 넣어줄것인지

};


#include <random>
#include "Player_Session.h"

#define RAND_CREATE_X_POS 300
#define RAND_CREATE_Z_POS 100

bool Player_Session::CheckPlayerInfo()
{
	/*
	CTextTest* m_playerData = new CTextTest();

	if (m_ploginData->FileOpen("Test.txt"))
		return true;
	else
		return false;
	*/

	//Packet recv_client_num_buf[MAX_BUFFER_SIZE];
	//m_socket.receive(boost::asio::buffer(recv_client_num_buf, sizeof(Packet)));

	//DB.loginDataList.emplace_back()

	/*
	for (auto i = 0; i < static_cast<int>(recv_client_num_buf); ++i)
	{
		m_socket.receive(boost::asio::buffer(temp_buf, MAX_BUFFER_SIZE));

		if (m_id == i)
		{
			wcscpy(m_loginID, reinterpret_cast<wchar_t*>(&temp_buf[1]));
			wcscpy(m_loginPW, reinterpret_cast<wchar_t*>(&temp_buf[temp_buf[0] + 4]));
		}
	}
	*/

	//���ӿ���� �÷��̾��� ���̵� m_playerIndex (ai + player)

	CTextTest DB;
	Packet cur_logindata_packet[MAX_BUFFER_SIZE]{ 0 };
	Player_LoginDB logindata;

	m_connect_state = true;
	
	m_socket.receive(boost::asio::buffer(cur_logindata_packet, MAX_BUFFER_SIZE));

	wcscpy(m_loginID, reinterpret_cast<wchar_t*>(&cur_logindata_packet[1]));
	wcscpy(m_loginPW, reinterpret_cast<wchar_t*>(&cur_logindata_packet[cur_logindata_packet[0] + 4]));

	wcscpy(logindata.name, m_loginID); wcscpy(logindata.password, m_loginPW);
	DB.loginDataList.emplace_back(logindata);

	if ((wcscmp(L"guest", m_loginID) == 0) && (wcscmp(L"guest", m_loginPW) == 0))
	{
		cur_logindata_packet[0] = 1;
		m_socket.send(boost::asio::buffer(cur_logindata_packet, MAX_BUFFER_SIZE));
		return true;
	}

}


void Player_Session::Init_MonsterInfo()
{
	//�÷��̾� ���� �ʱ�ȭ
	default_random_engine generator(time(0));
	uniform_int_distribution<int> export_x(-RAND_CREATE_X_POS, RAND_CREATE_X_POS);
	uniform_int_distribution<int> export_z(-RAND_CREATE_Z_POS, RAND_CREATE_Z_POS);
	auto rand_x = export_x(generator);
	auto rand_z = export_z(generator);

	//������������ �߸�����1������ �����Ѵ�.
	srand((unsigned)time(NULL));
	int curr_mob = rand() % MAX_MONSTER_TYPE + 1;

	switch (curr_mob)
	{
	case MONSTERS::MONSTER01:
		m_monsterType = MONSTERS::MONSTER01;
		break;
	case MONSTERS::MONSTER02:
		m_monsterType = MONSTERS::MONSTER02;
		break;
	case MONSTERS::MONSTER03:
		m_monsterType = MONSTERS::MONSTER03;
		break;
	}

	m_playerData.Is_AI = true;
	m_playerData.ID = m_id;
	m_playerData.Dir = 0;

	//������ ���� ��ġ���� ���Ƿ� ���������Ѵ�.
	//���� ���� ���� ���� - lua����ؾߵ�
	m_playerData.Pos = { static_cast<float>(rand_x), 0.0f, static_cast<float>(rand_z) };

	m_playerData.UserInfo.origin_hp = 100;
	m_playerData.UserInfo.cur_hp = m_playerData.UserInfo.origin_hp;
	m_playerData.UserInfo.origin_mp = 50;
	m_playerData.UserInfo.cur_mp = m_playerData.UserInfo.origin_mp;

	m_playerData.UserInfo.exp = 200;
	m_playerData.UserInfo.level = 1;
	m_playerData.UserInfo.player_status.attack = 20;
	m_playerData.UserInfo.player_status.defend = 10;
	m_playerData.UserInfo.player_status.speed = 20;
	//

	m_playerType = PLAYERS::NO_PLAYER;
}

void Player_Session::Init_PlayerInfo()
{
	//�÷��̾� ���� �ʱ�ȭ

	default_random_engine generator(time(0));
	uniform_int_distribution<int> export_x(-RAND_CREATE_X_POS, RAND_CREATE_X_POS);
	uniform_int_distribution<int> export_z(-RAND_CREATE_Z_POS, RAND_CREATE_Z_POS);
	auto rand_x = export_x(generator);
	auto rand_z = export_z(generator);

	m_playerData.Is_AI = false;
	m_playerData.ID = m_id;
	m_playerData.Dir = 0;
	m_playerData.Pos = { static_cast<float>(rand_x), 0.0f, static_cast<float>(rand_z) };
	
	wcscpy(m_playerData.LoginData.name, m_loginID);
	wcscpy(m_playerData.LoginData.password, m_loginPW);
	
	STC_ClientInit init_player;
	init_player.pack_size = sizeof(STC_ClientInit);
	init_player.pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	init_player.player_data = m_playerData;
	
	// m_clients -> ���� id�� �´� �ش� ���Ϳ�Ұ� sendpacket�� ���� �����͸� Ŭ��� ��������
	auto clients = move(m_player->Get_Clients());
	clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&init_player));



}


void Player_Session::SendPacket(Packet * packet)
{
	int packet_size = packet[0];
	Packet *new_sendBuf = new Packet[packet_size];
	memcpy(new_sendBuf, packet, packet_size);


	//1. async_write_some - �񵿱� IO / ���������͸� ��� ����(���� ���ۿ� ����X)
	//2. async_write - �񵿱� IO / �������� �ϴ� �����Ͱ� ��� ���ۿ� ���� �����͸� ����
	m_socket.async_write_some(boost::asio::buffer(new_sendBuf, packet_size),
		[=](const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (!error)
		{
			if (bytes_transferred != packet_size)
			{
				cout << "Client No. [ " << m_id << " ] send no same size packet data by async_write_some" << endl;
			}

			delete[] new_sendBuf;
		}
	});
}

void Player_Session::RecvPacket()
{
	m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER_SIZE),
		[=](const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			//����: �۾��� ��ҵ� ��� 
			{
				if (error.value() == boost::asio::error::operation_aborted)
					cout << "Client No. [ " << m_id << " ] Disconnected [Operation_Aborted] -----  ";
				
				if (error.value() == boost::asio::error::message_size)
					cout << "Client No. [ " << m_id << " ] Disconnected [Message_Too_Long] -----  ";
				
				cout << "IP: " << m_socket.remote_endpoint().address().to_string() << " // ";
				cout << "PORT: " << m_socket.remote_endpoint().port() << endl;
				return;
			}
	

			// shutdown_both - �ְ� �޴� �� ��θ� �ߴ�
			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();

			// �ڽ��� ������� - ����
			Set_State(-1);
			m_connect_state = CONNECT_STATE::DISCONNECT;

			STC_Disconnected dis_msg;
			dis_msg.id = m_id;

			// �ڽ��ֺ��� �ִ� ���鿡�� �ڽ��� ������¿� ���� ������ ��������
			//auto clients = move(m_player->Get_Clients());
			//clients[m_id]->Set_Connect_State(CONNECT_STATE::DISCONNECT);
			//clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&dis_msg));

			return;
 		}

		int cur_data_proc = static_cast<int>(bytes_transferred);
		Packet* temp_buf = m_recvBuf;
		while (cur_data_proc > 0)
		{
			if (m_cur_packet_size == 0)
				m_cur_packet_size = temp_buf[0];
			
			int need_to_read = m_cur_packet_size - m_prev_packet_size;
			if (need_to_read <= cur_data_proc)
			{
				memcpy(m_dataBuf + m_prev_packet_size, temp_buf, need_to_read);

				ProcessPacket(m_dataBuf);

				m_prev_packet_size = 0;
				m_cur_packet_size = 0;
				cur_data_proc -= need_to_read;
				temp_buf += need_to_read;
			}
			else
			{
				memcpy(m_dataBuf + m_prev_packet_size, temp_buf, cur_data_proc);
				m_prev_packet_size += cur_data_proc;
				cur_data_proc = 0;
				temp_buf += cur_data_proc;
			}
		}
		RecvPacket();
	});
}

void Player_Session::ProcessPacket(Packet * packet)
{
	//packet[0] - ��Ŷ�� ������
	//packet[1] - �ش� ��Ŷ�� �ϴ���
	//packet[2] - �ش� ��Ŷ�� ����� �����͵�

	switch (packet[1])
	{
	case PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION:

		break;
	}
}

void Player_Session::Set_State(int state)
{
	if (state != -1)
		m_state = state;
}

void Player_Session::PostReceive()
{
	memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));
	m_socket.async_read_some
	(
		boost::asio::buffer(m_ReceiveBuffer),
		boost::bind(&Player_Session::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);
}

void Player_Session::handle_write(const boost::system::error_code &, size_t)
{
}

void Player_Session::handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof)
		{
			cout << "Ŭ���̾�Ʈ�� ������ ���������ϴ�." << endl;
		}
		else
		{
			cout << "error No: " << error.value() << " error Message: " << error.message() << endl;
		}
	}
	else
	{
		string strRecvMessage = m_ReceiveBuffer.data();

		cout << "Ŭ���̾�Ʈ���� ���� �޽���: " << strRecvMessage <<
			", ���� ũ��: " << bytes_transferred << endl;
		string first_msg = "Re: ";
		auto show_msg = first_msg + strRecvMessage;

		m_WriteMessage = show_msg;

		boost::asio::async_write(m_socket, boost::asio::buffer(m_WriteMessage),
			boost::bind(&Player_Session::handle_write, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

		PostReceive();
	}
}
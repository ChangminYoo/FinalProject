#include <random>
#include <memory>
#include "Player_Session.h"

#define RAND_CREATE_X_POS 300
#define RAND_CREATE_Z_POS 100

vector<Player_Session*> Player_Session::m_clients = vector<Player_Session*>();

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

	m_socket.receive(boost::asio::buffer(cur_logindata_packet, MAX_BUFFER_SIZE));

	wcscpy(m_loginID, reinterpret_cast<wchar_t*>(&cur_logindata_packet[1]));
	wcscpy(m_loginPW, reinterpret_cast<wchar_t*>(&cur_logindata_packet[cur_logindata_packet[0] + 4]));

	wcscpy(logindata.name, m_loginID); wcscpy(logindata.password, m_loginPW);
	DB.loginDataList.emplace_back(logindata);

	if ((wcscmp(L"guest", m_loginID) == 0) && (wcscmp(L"guest", m_loginPW) == 0))
	{
		cur_logindata_packet[0] = 1;
		m_socket.send(boost::asio::buffer(cur_logindata_packet, MAX_BUFFER_SIZE));

		m_connect_state = true;

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

	m_isAI = true;

	m_playerData.Is_AI = true;
	m_playerData.ID = m_id;
	m_playerData.Dir = 0;


	//������ ���� ��ġ���� ���Ƿ� ���������Ѵ�.
	//���� ���� ���� ���� - lua����ؾߵ�
	m_playerData.Pos = { static_cast<float>(rand_x), 0.0f, static_cast<float>(rand_z) , 0.0f};

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

	m_state = PLAYER_STATE::IDLE;
	m_playerType = PLAYERS::NO_PLAYER;
}

void Player_Session::Init_PlayerInfo()
{
	//�÷��̾� ���� �ʱ�ȭ -> send ������ recv�� ������Ѵ�

	default_random_engine generator(time(0));
	uniform_int_distribution<int> export_x(-RAND_CREATE_X_POS, RAND_CREATE_X_POS);
	uniform_int_distribution<int> export_z(-RAND_CREATE_Z_POS, RAND_CREATE_Z_POS);
	auto rand_x = export_x(generator);
	auto rand_z = export_z(generator);

	m_state = PLAYER_STATE::IDLE;
	m_playerType = PLAYERS::LUNA;
	m_isAI = false;

	m_playerData.Connect_Status = true;
	m_playerData.Is_AI = false;
	m_playerData.ID = m_id;
	m_playerData.Dir = 0;
	m_playerData.Ani = Ani_State::Idle;
	m_playerData.Rotate_status = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (m_playerData.ID == 0)
		m_playerData.Pos = { 0.0f , -1000.0f, 0.0f };
	else if (m_playerData.ID == 1)
		m_playerData.Pos = { 100.0f, -1000.0f, 0.0f };

	//m_playerData.Pos = { static_cast<float>(rand_x), 0.0f, static_cast<float>(rand_z) , 0.0f };
	
	//wcscpy(m_playerData.LoginData.name, m_loginID);
	//wcscpy(m_playerData.LoginData.password, m_loginPW);

	//2. �ʱ�ȭ�� ������ ����� Ŭ���̾�Ʈ�� ������.

}

void Player_Session::InitData_To_Client()
{
	//1. �� �ʱ�ȭ����
	STC_SetMyClient init_player;
	init_player.pack_size = sizeof(STC_SetMyClient);
	init_player.pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	init_player.player_data = m_playerData;

	cout << "Current ID: " << m_clients[m_id]->Get_ID() << endl;

	// �� �ʱ�ȭ ������ �ϴ� ���� ����� Ŭ���̾�Ʈ�� ������.
	m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&init_player));

	//------------------------------------------------------------------------------------
	//2. �ٸ� Ŭ���̾�Ʈ �ʱ�ȭ����
	STC_SetOtherClient init_otherplayer;
	init_otherplayer.pack_size = sizeof(STC_SetOtherClient);
	init_otherplayer.pack_type = PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT;

	//2. �� ������ �ٸ� Ŭ���̾�Ʈ���� �Ѱ��ش�
	for (auto i = 0; i < m_clients.size(); ++i)
	{
		if (m_clients[i]->Get_ID() == m_id) continue;
		if (m_clients[i]->Get_Connect_State() != false || m_clients[i]->Get_IsAI() != true)
		{
			// 1. �ٸ� Ŭ���̾�Ʈ ������ �� Ŭ���̾�Ʈ���� ������
			init_otherplayer.player_data = move(m_clients[i]->GetPlayerData());
			m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&init_otherplayer));

			// 2. �� ������ �ٸ� Ŭ���̾�Ʈ���� ������
			init_otherplayer.player_data = move(m_playerData);
			m_clients[i]->SendPacket(reinterpret_cast<Packet*>(&init_otherplayer));

		}
	}
	//------------------------------------------------------------------------------------

}


void Player_Session::SendPacket(Packet* packet)
{
	int packet_size = packet[0];
	Packet *new_sendBuf = new Packet[packet_size];
	memcpy(new_sendBuf, packet, packet_size);

	//shared_ptr<Packet*> new_sendBuf(new Packet[packet_size]);
	//m_new_sendBuf = new Packet[packet_size];

	auto a = *(reinterpret_cast<Player_Data*>(&new_sendBuf[2]));

	//1. async_write_some - �񵿱� IO / ���������͸� ��� ����(���� ���ۿ� ����X)
	//2. async_write - �񵿱� IO / �������� �ϴ� �����Ͱ� ��� ���ۿ� ���� �����͸� ����
	boost::asio::async_write(m_socket, boost::asio::buffer(new_sendBuf, packet_size),
		[&](const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error != 0)
		{
			if (bytes_transferred != packet_size)
			{
				cout << "Client No. [ " << m_id << " ] error code : " << error.value() << endl;
			}

			delete[] new_sendBuf;
			return;
		}
		
		RecvPacket();
	});

	/*
	m_socket.async_write_some(boost::asio::buffer(new_sendBuf, packet_size),
		[=](const boost::system::error_code& error, size_t bytes_transferred)
		-> void
	{
		if (error != 0)
		{
			if (bytes_transferred != packet_size)
			{
				cout << "Client No. [ " << m_id << " ] send no same size packet data by async_write_some" << endl;
			}

			delete[] new_sendBuf;
		}

		cout << "Client No. [ " << a.ID << " ] send " << "X: " << a.Pos.x << "Y: " << a.Pos.y << "Z: " << a.Pos.z << endl;

	});
	*/
	

}

void Player_Session::RecvPacket()
{
	//m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER_SIZE),
	//[=](const boost::system::error_code& error, size_t bytes_transferred)

	//��Ŷ���� �� ���ۻ������ ���޵� ������ ũ�⸸ŭ�� �Ҵ��ؾ��Ѵ�
	//1����Ʈ�� �޾� ��Ŷ����� �˰� �ش� ��Ŷ ����� �̿��� �ٽ� ��Ŷ�� �޴´�
	//boost::asio::async_read(m_socket, boost::asio::buffer(m_recvBuf , 1),
	//	[&](const boost::system::error_code& error, size_t bytes_transferred)
	m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER_SIZE),
		[&](const boost::system::error_code& error,const size_t& bytes_transferred)
	{	
		// error = 0 ���� , error != 0 ����
		if (error != 0)
		{
			STC_Disconnected disconnect_data;
			disconnect_data.connect = false;
			disconnect_data.id = m_id;

			// AI�� ������ �̹� ���� Ŭ���̾�Ʈ���Դ� ������ ������� ��Ŷ�� ������ �ʴ´�
			for (auto client : m_clients)
			{
				if (client->Get_ID() == disconnect_data.id) continue;
				if (client->Get_IsAI() != true && client->Get_Connect_State() != false)
				{
					client->SendPacket(reinterpret_cast<Packet*>(&disconnect_data));
				}
			}


			//����: �۾��� ��ҵ� ��� 
			cout << "Client No. [ " << m_id << " ] error code : " << error.value() << endl;

			cout << "IP: " << m_socket.remote_endpoint().address().to_string() << " // ";
			cout << "PORT: " << m_socket.remote_endpoint().port() << endl;

			// �ڽ��� ������� - ����
			Set_State(-1);
			m_connect_state = false;

			// shutdown_both - �ְ� �޴� �� ��θ� �ߴ�
			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();

			return;
		}

		//Packet *buf = Get_RecvBuf();
		//ProcessPacket(buf);

		/*
		Packet *buf = Get_RecvBuf();
		int cur_data_processing = static_cast<int>(bytes_transferred);

		while (cur_data_processing > 0)
		{
			if (m_cur_packet_size == 0)
			{
				m_cur_packet_size = buf[0];
			}

			//1����Ʈ �����͸� ���� ��ŭ ���� read���� ����� ������Ѵ�
			boost::asio::async_read(m_socket, boost::asio::buffer(Get_DataBuf(), m_cur_packet_size - 1),
				[&](const boost::system::error_code& error, size_t bytes_transferred)
			{
				//���⿡�� ó���� ���� ����[0]�κп� �ִ� ����� �����ϰ� �����͸� �о�����Ѵ�
				ProcessPacket(buf);

				m_cur_packet_size = 0;
			});
		}
		
	
		RecvPacket();
		*/

		int cur_data_proc = static_cast<int>(bytes_transferred);
		Packet* temp_buf = m_recvBuf;

		while (cur_data_proc > 0)
		{
			if (m_cur_packet_size == 0)			//�̷��� �ؾ� ��Ŷ�� ù ������ ������ ��Ŷ����� ��
			{
				m_cur_packet_size = temp_buf[0];
				if (temp_buf[0] > MAX_BUFFER_SIZE)
				{
					cout << "RecvPacket() Error, Client No. [ " << m_id << " ] recvBuf[0] is out of MAX_BUF_SIZE\n";
					exit(-1);
				}
			}
				
			int need_to_read = m_cur_packet_size - m_prev_packet_size;
			if (need_to_read <= cur_data_proc)
			{
				memcpy(m_dataBuf + m_prev_packet_size, temp_buf, need_to_read);

				ProcessPacket(m_dataBuf);

				cur_data_proc -= need_to_read;
				temp_buf += need_to_read;
				m_prev_packet_size = 0;
				m_cur_packet_size = 0;
			}
			else
			{
				memcpy(m_dataBuf + m_prev_packet_size, temp_buf, cur_data_proc);
				m_prev_packet_size += cur_data_proc;
				cur_data_proc = 0;

				//temp_buf += cur_data_proc;
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
		{
			if (m_state == PLAYER_STATE::DEAD)
				break;

			//1. �޾Ƶ��� ������(Ű�� ���� �÷��̾ ��������)���� ��ȭ�� ������ ����
			auto PosMove_Data = (reinterpret_cast<STC_ChangedPos*>(packet));

			if (PosMove_Data->ani_state == Ani_State::Idle)
				m_state = PLAYER_STATE::IDLE;
			else if (PosMove_Data->ani_state == Ani_State::Run)
				m_state = PLAYER_STATE::MOVE;

			//2. �̵� - (�ִϸ��̼�, ��ġ ����) ����� �����͸� �������������ϴ� �� Ŭ���̾�Ʈ�� ����
			m_clients[PosMove_Data->id]->m_playerData.Pos = move(PosMove_Data->pos);
			m_clients[PosMove_Data->id]->m_playerData.Ani = PosMove_Data->ani_state;

			//cout << "ID: " << PosMove_Data->id << " ��ȭ�� ��ġ��: " << "[x:" << PosMove_Data->pos.x << "\t" << "y:" << PosMove_Data->pos.y
			//	<< "\t" << "z:" << PosMove_Data->pos.z << "]" << "\t" << "w:" << PosMove_Data->pos.w << endl;

			//3. ��ȭ�� �� (������, �ִϸ��̼�) ������ �ٸ� Ŭ�� ���� - �ݵ�� �̷��� �ٽ� ����������
			//PosMove_Data�� �ٷ� sendpacket�� packet���� ����ȭ�Ͽ� ������ size error�� ����
			STC_ChangedPos c_to_other;
	
			c_to_other.id = PosMove_Data->id;
			c_to_other.ani_state = m_playerData.Ani;
			c_to_other.pos = move(m_playerData.Pos);
		
			for (auto client : m_clients)
			{
				//��밡 ai / ������� / ���� ��� ���� �ʿ� ����
				if (client->m_playerData.Is_AI == true) continue;
				if (client->m_playerData.Connect_Status == false) continue;

				//if (client->m_playerData.ID == PosMove_Data->id) continue;
				//���⼭ ����

				//���ŵ� ���� �����͸� ���濡�� ����
				client->SendPacket(reinterpret_cast<Packet*>(&c_to_other));	
			}

		}
		break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ROTATE:
		{
			if (m_state == PLAYER_STATE::DEAD)
				break;

			m_state = PLAYER_STATE::ROTATE;

			auto Rotation_Data = reinterpret_cast<STC_Rotation*>(packet);	

			// 1. ���� ������ �� Ŭ���̾�Ʈ�� �־��ְ�
			m_clients[Rotation_Data->id]->m_playerData.Rotate_status = move(Rotation_Data->rotate_status);

			//cout << "ID: " << Rotation_Data->id << " ��ȭ�� ȸ����: " << "[ x, y, z, w ]: "
			//	<< Rotation_Data->rotate_status.x << ", " << Rotation_Data->rotate_status.y << ", " << Rotation_Data->rotate_status.z << ", " << Rotation_Data->rotate_status.w << endl;

			// 2. �ٸ� Ŭ�󿡰� ������.
			STC_Rotation r_to_other;

			r_to_other.id = Rotation_Data->id;
			r_to_other.rotate_status = move(Rotation_Data->rotate_status);

			for (auto client : m_clients)
			{
				if (client->m_playerData.Is_AI == true) continue;
				if (client->m_playerData.Connect_Status == false) continue;
			
				client->SendPacket(reinterpret_cast<Packet*>(&r_to_other));
			}

		}
		break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ATTACK:
		{

		}
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

XMFLOAT3 Player_Session::GetUpvector()
{
	XMVECTOR l = XMLoadFloat3(&Lookvector);
	XMVECTOR r = XMLoadFloat3(&Rightvector);
	auto u = XMVector3Cross(l, r);

	XMFLOAT3 up;
	XMStoreFloat3(&up, u);
	up = Float3Normalize(up);
	return up;
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
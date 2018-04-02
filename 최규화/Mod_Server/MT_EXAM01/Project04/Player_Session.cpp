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

	//접속연결시 플레이어의 아이디 m_playerIndex (ai + player)

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
	//플레이어 정보 초기화
	default_random_engine generator(time(0));
	uniform_int_distribution<int> export_x(-RAND_CREATE_X_POS, RAND_CREATE_X_POS);
	uniform_int_distribution<int> export_z(-RAND_CREATE_Z_POS, RAND_CREATE_Z_POS);
	auto rand_x = export_x(generator);
	auto rand_z = export_z(generator);

	//스테이지에서 중립몬스터1마리가 등장한다.
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


	//몬스터의 원래 위치에서 임의로 움직여야한다.
	//몬스터 정보 추후 수정 - lua사용해야됨
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
	//플레이어 정보 초기화 -> send 다음에 recv를 해줘야한다

	default_random_engine generator(time(0));
	uniform_int_distribution<int> export_x(-RAND_CREATE_X_POS, RAND_CREATE_X_POS);
	uniform_int_distribution<int> export_z(-RAND_CREATE_Z_POS, RAND_CREATE_Z_POS);
	auto rand_x = export_x(generator);
	auto rand_z = export_z(generator);

	m_state = PLAYER_STATE::IDLE;
	m_playerType = PLAYERS::LUNA;
	m_isAI = false;

	m_playerData.Is_AI = false;
	m_playerData.ID = m_id;
	m_playerData.Dir = 0;
	m_playerData.Pos = { static_cast<float>(rand_x), 0.0f, static_cast<float>(rand_z) , 0.0f };
	
	wcscpy(m_playerData.LoginData.name, m_loginID);
	wcscpy(m_playerData.LoginData.password, m_loginPW);

	//2. 초기화된 정보를 연결된 클라이언트로 보낸다.
	InitData_To_Client(); 

}

void Player_Session::InitData_To_Client()
{
	//1. 내 초기화정보
	STC_SetMyClient init_player;
	init_player.pack_size = sizeof(STC_SetMyClient);
	init_player.pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	init_player.player_data = m_playerData;
	init_player.player_data.Pos = { 0.0f, -1000.0f, 0.0f };

	// 내 초기화 정보를 일단 나와 연결된 클라이언트에 보낸다.
	m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&init_player));

	//2. 다른 클라이언트 초기화정보
	STC_SetOtherClient init_otherplayer;
	init_otherplayer.pack_size = sizeof(STC_SetOtherClient);
	init_otherplayer.pack_type = PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT;

	//2. 내 정보를 다른 클라이언트에게 넘겨준다
	for (auto i = 0; i < m_clients.size(); ++i)
	{
		if (m_clients[i]->Get_ID() != m_id || m_clients[i]->Get_Connect_State() != false || m_clients[i]->Get_IsAI() != true)
		{
			// 1. 다른 클라이언트 정보를 내 클라이언트에게 보낸다
			init_otherplayer.player_data = move(m_clients[i]->GetPlayerData());
			m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&init_otherplayer));

			// 2. 내 정보를 다른 클라이언트에게 보낸다
			m_clients[i]->SendPacket(reinterpret_cast<Packet*>(&init_player));

		}
	}

}


void Player_Session::SendPacket(Packet* packet)
{
	int packet_size = packet[0];
	Packet *new_sendBuf = new Packet[packet_size];
	memcpy(new_sendBuf, packet, packet_size);

	//shared_ptr<Packet*> new_sendBuf(new Packet[packet_size]);
	//m_new_sendBuf = new Packet[packet_size];

	auto a = *(reinterpret_cast<Player_Data*>(&new_sendBuf[2]));

	//1. async_write_some - 비동기 IO / 받은데이터를 즉시 보냄(따로 버퍼에 저장X)
	//2. async_write - 비동기 IO / 보내고자 하는 데이터가 모두 버퍼에 담기면 데이터를 보냄
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

	//패킷받을 때 버퍼사이즈는 전달된 버퍼의 크기만큼만 할당해야한다
	//1바이트를 받아 패킷사이즈를 알고 해당 패킷 사이즈를 이용해 다시 패킷을 받는다
	//boost::asio::async_read(m_socket, boost::asio::buffer(m_recvBuf , 1),
	//	[&](const boost::system::error_code& error, size_t bytes_transferred)
	m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER_SIZE),
		[&](const boost::system::error_code& error,const size_t& bytes_transferred)
	{	
		// error = 0 성공 , error != 0 실패
		if (error != 0)
		{
			//에러: 작업이 취소된 경우 
			cout << "Client No. [ " << m_id << " ] error code : " << error.value() << endl;

			cout << "IP: " << m_socket.remote_endpoint().address().to_string() << " // ";
			cout << "PORT: " << m_socket.remote_endpoint().port() << endl;
			// shutdown_both - 주고 받는 쪽 모두를 중단
			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();

			// 자신의 연결상태 - 끊음
			Set_State(-1);
			m_connect_state = false;

			STC_Disconnected disconnect_data;
			disconnect_data.connect = false;
			disconnect_data.id = m_id;

			// AI와 연결이 이미 끊긴 클라이언트에게는 연결을 끊으라는 패킷을 보내지 않는다
			for (auto client : m_clients)
			{			
				if (client->Get_IsAI() != true && client->Get_Connect_State() != false)
				{
					client->SendPacket(reinterpret_cast<Packet*>(&disconnect_data));
				}
			}

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

			//1바이트 데이터를 받은 만큼 다음 read에서 사이즈를 빼줘야한다
			boost::asio::async_read(m_socket, boost::asio::buffer(Get_DataBuf(), m_cur_packet_size - 1),
				[&](const boost::system::error_code& error, size_t bytes_transferred)
			{
				//여기에서 처리할 때도 버퍼[0]부분에 있는 사이즈를 제외하고 데이터를 읽어줘야한다
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
			if (m_cur_packet_size == 0)			//이렇게 해야 패킷의 첫 정보는 무조건 패킷사이즈가 됨
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
	//packet[0] - 패킷의 사이즈
	//packet[1] - 해당 패킷이 하는일
	//packet[2] - 해당 패킷에 담겨진 데이터들

	switch (packet[1])
	{
	case PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION:
		{
			if (m_state == PLAYER_STATE::DEAD) break;
			m_state = PLAYER_STATE::MOVE;

			//1. 받아들인 데이터(키를 눌러 플레이어를 움직였음)에서 변화된 정보를 추출
			auto PosMove_Data = (reinterpret_cast<STC_ChangedPos*>(packet));

			//2. 변화된 정보를 서버에서 관리하고 있는 해당 클라이언트 객체에 저장 및 내 클라로 다시보냄
			m_clients[PosMove_Data->id]->m_playerData.Pos = move(PosMove_Data->pos);
			m_clients[PosMove_Data->id]->SendPacket(reinterpret_cast<Packet*>(&PosMove_Data));

			cout << "변화된 위치값: " << "[x:" << PosMove_Data->pos.x << "\t" << "y:" << PosMove_Data->pos.y
				<< "\t" << "z:" << PosMove_Data->pos.z << "]" << "\t" << "w:" << PosMove_Data->pos.w << endl;

			//변화된 포지션을 다른 클라에 전달
			STC_SetOtherClient changedpos_to_otherplayer;
			changedpos_to_otherplayer.player_data.ID = PosMove_Data->id;
			changedpos_to_otherplayer.player_data.Pos = move(PosMove_Data->pos);

			for (auto client : m_clients)
			{
				//상대가 ai / 연결끊김 / 나일 경우 보낼 필요 없음
				if (client->m_playerData.Is_AI == true) continue;
				if (client->m_playerData.Connect_Status == false) continue;
				if (client->m_playerData.ID == m_id) continue;

				//갱신된 나의 데이터를 상대방에게 전달
				client->SendPacket(reinterpret_cast<Packet*>(&changedpos_to_otherplayer));
			}

		}
		break;


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
			cout << "클라이언트와 연결이 끊어졌습니다." << endl;
		}
		else
		{
			cout << "error No: " << error.value() << " error Message: " << error.message() << endl;
		}
	}
	else
	{
		string strRecvMessage = m_ReceiveBuffer.data();

		cout << "클라이언트에서 받은 메시지: " << strRecvMessage <<
			", 받은 크기: " << bytes_transferred << endl;
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
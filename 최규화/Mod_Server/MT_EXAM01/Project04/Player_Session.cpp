#include <random>
#include <memory>
#include "Player_Session.h"
#include "Player.h"
#include "BulletObject.h"

#define RAND_CREATE_X_POS 300
#define RAND_CREATE_Z_POS 100

vector<Player_Session*> Player_Session::m_clients = vector<Player_Session*>();
list<BulletObject*> Player_Session::m_bullobjs = list <BulletObject*>();
unordered_set<StaticObject*> Player_Session::m_sobjs = unordered_set<StaticObject*>();
int Player_Session::m_tempcount = 0;

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

	//CTextTest DB;
	//Packet cur_logindata_packet[MAX_BUFFER_SIZE]{ 0 };
	//Player_LoginDB logindata;
	//
	//m_socket.receive(boost::asio::buffer(cur_logindata_packet, MAX_BUFFER_SIZE));
	//
	//wcscpy(m_loginID, reinterpret_cast<wchar_t*>(&cur_logindata_packet[1]));
	//wcscpy(m_loginPW, reinterpret_cast<wchar_t*>(&cur_logindata_packet[cur_logindata_packet[0] + 4]));
	//
	//wcscpy(logindata.name, m_loginID); wcscpy(logindata.password, m_loginPW);
	//DB.loginDataList.emplace_back(logindata);
	//
	//if ((wcscmp(L"guest", m_loginID) == 0) && (wcscmp(L"guest", m_loginPW) == 0))
	//{
	//	cur_logindata_packet[0] = 1;
	//	m_socket.send(boost::asio::buffer(cur_logindata_packet, MAX_BUFFER_SIZE));
	//
	//	m_connect_state = true;
	//
	//	//m_staticobject = new StaticObject();
	//
	//	return true;
	//}
	return true;
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
	//플레이어 정보 초기화 
	default_random_engine generator(time(0));
	uniform_int_distribution<int> export_x(-RAND_CREATE_X_POS, RAND_CREATE_X_POS);
	uniform_int_distribution<int> export_z(-RAND_CREATE_Z_POS, RAND_CREATE_Z_POS);
	auto rand_x = export_x(generator);
	auto rand_z = export_z(generator);

	m_state = PLAYER_STATE::IDLE;
	m_playerType = PLAYERS::LUNA;
	//m_myObjType = PLAYER_OBJECT_TYPE::PLAYER_OBJECT;
	m_isAI = false;

	m_playerData.Ani = Ani_State::Idle;
	m_playerData.Connect_Status = true;
	m_playerData.Dir = 0;
	m_playerData.GodMode = false;
	m_playerData.AirBone = false;
	m_playerData.ID = m_id;
	m_playerData.Is_AI = false;
	m_playerData.Rotate_status = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_playerData.UserInfo.player_status.attack = 50;
	m_playerData.UserInfo.player_status.defend = 50;
	m_playerData.UserInfo.player_status.speed = 100;

	m_playerData.UserInfo.cur_hp = 300;
	m_playerData.UserInfo.origin_hp = 300;
	m_playerData.UserInfo.exp = 0;
	m_playerData.UserInfo.level = 1;

	if (m_playerData.ID == 0)
		m_playerData.Pos = { -100.0f , -1000.0f, 0.0f };
	else if (m_playerData.ID == 1)
		m_playerData.Pos = { 300.0f, -1000.0f, 0.0f };
	//


	//Lookvector, Rightvector, Upvector 설정 - PhysicalEffect 클래스 함수이용
	pe = new PhysicalEffect();

	OffLookvector = XMFLOAT3(0, 0, -1);
	OffRightvector = XMFLOAT3(-1, 0, 0);	
	
	pe->UpdateLookVector(OffLookvector, OffRightvector, m_playerData.Rotate_status, Lookvector, Rightvector);
	pe->GetUpVector(Lookvector, Rightvector, Upvector);
	//

	//평면과의 레이캐스팅
	XMFLOAT3 rx(3, 0, 0);
	XMFLOAT3 ry(0, 10, 0);
	XMFLOAT3 rz(0, 0, 3);
	rco.SetPlane(rx, ry, rz);
	//

	//물리효과 및 충돌처리를 위한 PhysicsPoint 클래스
	pp = new PhysicsPoint();
	pp->SetPosition(m_playerData.Pos.x, m_playerData.Pos.y, m_playerData.Pos.z);
	pp->SetHalfBox(3, 10, 3);
	pp->SetDamping(0.7);
	pp->SetBounce(false);
	//

	//2. 물리효과 적용
	pp->SetPosition(m_playerData.Pos.x, m_playerData.Pos.y, m_playerData.Pos.z);

	pe->GravitySystem(0.0f, pp);

	XMFLOAT4 xmf4 = { m_playerData.Pos.x, m_playerData.Pos.y, m_playerData.Pos.z, m_playerData.Pos.w };
	pp->integrate(0.0f, &xmf4);

	m_playerData.Pos.x = xmf4.x;  m_playerData.Pos.y = xmf4.y;  m_playerData.Pos.z = xmf4.z;   m_playerData.Pos.w = xmf4.w;
	pe->AfterGravitySystem(0.0f, pp, OBJECT_TYPE::PLAYER, m_playerData.Pos, AirBone);

	//wcscpy(m_playerData.LoginData.name, m_loginID);
	//wcscpy(m_playerData.LoginData.password, m_loginPW);
}

void Player_Session::InitData_To_Client()
{
	//1. 내 초기화정보
	STC_SetMyClient init_player;
	init_player.pack_size = sizeof(STC_SetMyClient);
	init_player.pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	init_player.player_data = m_playerData;

	//cout << "socket : " << reinterpret_cast<int*>(&m_clients[m_id]->m_socket) << endl;

	// 내 초기화 정보를 일단 나와 연결된 클라이언트에 보낸다.
	m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&init_player));

	cout << "Current ID: " << m_playerData.ID << "Pos(x,y,z): " << m_playerData.Pos.x << " " << 
		m_playerData.Pos.y << " " << m_playerData.Pos.z << " " <<  m_playerData.Pos.w << endl;
	//------------------------------------------------------------------------------------
	//2. 다른 클라이언트 초기화정보
	STC_SetOtherClient init_otherplayer;
	init_otherplayer.pack_size = sizeof(STC_SetOtherClient);
	init_otherplayer.pack_type = PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT;

	//2. 내 정보를 다른 클라이언트에게 넘겨준다
	for (auto i = 0; i < m_clients.size(); ++i)
	{
		if (m_clients[i]->Get_ID() == m_id) continue;
		if (m_clients[i]->Get_Connect_State() != false || m_clients[i]->Get_IsAI() != true)
		{
			// 1. 다른 클라이언트 정보를 내 클라이언트에게 보낸다
			init_otherplayer.player_data = move(m_clients[i]->GetPlayerData());
			m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&init_otherplayer));

			// 2. 내 정보를 다른 클라이언트에게 보낸다
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

	//1. async_write_some - 비동기 IO / 받은데이터를 즉시 보냄(따로 버퍼에 저장X)
	//2. async_write - 비동기 IO / 보내고자 하는 데이터가 모두 버퍼에 담기면 데이터를 보냄
	boost::asio::async_write(m_socket, boost::asio::buffer(new_sendBuf, packet_size),
		[=](const boost::system::error_code& error, const size_t& bytes_transferred)
	{
		//cout << "Packet_Size : " << packet_size << "bytes_transferred : " << bytes_transferred << endl;
		if (!error)
		{
			if (bytes_transferred != packet_size)
			{
				cout << "Client No. [ " << m_id << " ] async_write error code : " << error.value() << endl;
			}

			delete[] new_sendBuf;
			return;
		}
	});
	
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
		//cout << "Bytes_Transferred: " << bytes_transferred << endl;
		// error = 0 성공 , error != 0 실패
		if (error)  // !error = 에러안났을때
		{
			STC_Disconnected disconnect_data;
			disconnect_data.connect = false;
			disconnect_data.id = m_id;

			// AI와 연결이 이미 끊긴 클라이언트에게는 연결을 끊으라는 패킷을 보내지 않는다
			for (auto client : m_clients)
			{
				if (client->Get_ID() == disconnect_data.id) continue;
				if (client->Get_IsAI() != true && client->Get_Connect_State() != false)
				{
					client->SendPacket(reinterpret_cast<Packet*>(&disconnect_data));
				}
			}


			//에러: 작업이 취소된 경우 
			cout << "Client No. [ " << m_id << " ] error code : " << error.value() << endl;

			cout << "IP: " << m_socket.remote_endpoint().address().to_string() << " // ";
			cout << "PORT: " << m_socket.remote_endpoint().port() << endl;

			// 자신의 연결상태 - 끊음
			Set_State(-1);
			m_connect_state = false;

			// shutdown_both - 주고 받는 쪽 모두를 중단
			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();

			return;
		}

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

				//cout << "cur_data_proc: " << cur_data_proc << " --- " << "need_to_read: " << need_to_read << endl;
			}
			else
			{
				memcpy(m_dataBuf + m_prev_packet_size, temp_buf, cur_data_proc);
				m_prev_packet_size += cur_data_proc;
				cur_data_proc -= cur_data_proc;
				temp_buf += cur_data_proc;
			}
		}

		//cur_data_proc 문제
		//bytes_transferred 은 20인데, need_to_read 와 m_prev_packet_size 와 m_cur_packet_size = 0인 현상
		//20bytes 가 넘어왔는데 들어있는 데이터가 모두 0 인상태

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
			if (m_state == PLAYER_STATE::DEAD)
				break;

			//1. 받아들인 데이터(키를 눌러 플레이어를 움직였음)에서 변화된 정보를 추출(물리효과 적용x)
			auto PosMove_Data = (reinterpret_cast<STC_ChangedPos*>(packet));
			if (PosMove_Data->deltime < 0)
				PosMove_Data->deltime = 0.f;

			if (PosMove_Data->ani_state == Ani_State::Idle)
				m_state = PLAYER_STATE::IDLE;
			else if (PosMove_Data->ani_state == Ani_State::Run)
				m_state = PLAYER_STATE::MOVE;

			//2. 물리효과 적용
			pp->SetPosition(PosMove_Data->pos.x, PosMove_Data->pos.y, PosMove_Data->pos.z);

			pe->GravitySystem(PosMove_Data->deltime, pp);

			XMFLOAT4 xmf4 = { PosMove_Data->pos.x, PosMove_Data->pos.y, PosMove_Data->pos.z, PosMove_Data->pos.w };
			pp->integrate(PosMove_Data->deltime, &xmf4);
			PosMove_Data->pos.x = xmf4.x; PosMove_Data->pos.y = xmf4.y;  PosMove_Data->pos.z = xmf4.z;  PosMove_Data->pos.w = xmf4.w;

			pe->AfterGravitySystem(PosMove_Data->deltime, pp, OBJECT_TYPE::PLAYER, PosMove_Data->pos, AirBone);

			//3. 이동 - (애니메이션, 위치 변경) 변경된 데이터를 서버에서관리하는 내 클라이언트에 저장
			m_clients[PosMove_Data->id]->m_playerData.Pos = move(PosMove_Data->pos);
			m_clients[PosMove_Data->id]->m_playerData.Ani = PosMove_Data->ani_state;

			cout << "ID: " << PosMove_Data->id << " 변화된 위치값: " << "[x:" << PosMove_Data->pos.x << "\t" << "y:" << PosMove_Data->pos.y
				<< "\t" << "z:" << PosMove_Data->pos.z << "]" << "\t" << "w:" << PosMove_Data->pos.w << endl;

			//4. 변화된 내 (포지션, 애니메이션) 정보를 다른 클라에 전달 - 반드시 이렇게 다시 만들어줘야함
			//PosMove_Data를 바로 sendpacket에 packet으로 형변화하여 보내면 size error가 난다
			STC_ChangedPos c_to_other;
			c_to_other.packet_size = sizeof(STC_ChangedPos);
			c_to_other.pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;
	
			c_to_other.id = PosMove_Data->id;
			c_to_other.ani_state = m_playerData.Ani;
			c_to_other.pos = move(m_playerData.Pos);
		
			for (auto client : m_clients)
			{
				//상대가 ai / 연결끊김 / 나일 경우 보낼 필요 없음
				if (static_cast<bool>(client->m_playerData.Is_AI) == true) continue;
				if (static_cast<bool>(client->m_playerData.Connect_Status) == false) continue;

				//if (client->m_playerData.ID == PosMove_Data->id) continue;
				//여기서 문제

				//갱신된 나의 데이터를 상대방에게 전달
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

			// 1. 받은 정보를 내 클라이언트에 넣어주고
			m_clients[Rotation_Data->id]->m_playerData.Rotate_status = move(Rotation_Data->rotate_status);

			// 2. 받은 정보를 토대로 lookvector와 rightvector를 업데이트
			//m_clients[Rotation_Data->id]->UpdateLookVector(); //이거넣으면 20byte -> 21byte씩 다시보내게됨
			pe->UpdateLookVector(OffLookvector, OffRightvector, Rotation_Data->rotate_status, Lookvector, Rightvector);
			pe->GetUpVector(Lookvector, Rightvector, Upvector);

			cout << "ID: " << Rotation_Data->id << " 변화된 회전값: " << "[ x, y, z, w ]: "
				<< Rotation_Data->rotate_status.x << ", " << Rotation_Data->rotate_status.y << ", " << Rotation_Data->rotate_status.z << ", " << Rotation_Data->rotate_status.w << endl;

			// 3. 다른 클라에게 보낸다.
			STC_Rotation r_to_other;
			r_to_other.packet_size = sizeof(STC_Rotation);
			r_to_other.pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ROTATE;

			r_to_other.id = Rotation_Data->id;
			r_to_other.rotate_status = move(Rotation_Data->rotate_status);

			for (auto client : m_clients)
			{
				if (static_cast<bool>(client->m_playerData.Is_AI) == true) continue;
				if (static_cast<bool>(client->m_playerData.Connect_Status) == false) continue;
				//if (client->m_playerData.ID == Rotation_Data->id) continue;

				client->SendPacket(reinterpret_cast<Packet*>(&r_to_other));
			}

		}
		break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ATTACK:
		{
			if (m_state == PLAYER_STATE::DEAD)		break;

			auto n_bldata = reinterpret_cast<STC_Attack*>(packet);

			//공격키를 눌렀을 시, 불렛 생성.
			//불렛을 생성한 캐릭터 ID, 유도를 대비한 타겟 ID, 불렛 초기생성위치, 불렛 초기회전값, 불렛 생성시간, 불렛아이디
			m_bullObj = new BulletObject(n_bldata->bull_data.Master_ID, n_bldata->bull_data.myID,
				n_bldata->bull_data.pos, n_bldata->bull_data.Rotate_status, n_bldata->lifetime,
				n_bldata->bull_data.vel3f, n_bldata->bull_data.type, n_bldata->bull_data.endpoint);

			//불렛 데이터 하나의 물리효과 적용해주기 (integrate -> aftergravity) -> set bullet start time
			m_bullObj->GetPhysicsPoint()->SetVelocity(n_bldata->bull_data.vel3f.x, n_bldata->bull_data.vel3f.y, n_bldata->bull_data.vel3f.z);
			m_bullObj->GetPhysicsPoint()->integrate(n_bldata->lifetime, reinterpret_cast<XMFLOAT4*>(&n_bldata->bull_data.pos));
			m_bullObj->AfterGravitySystem();
			m_bullObj->SetBulletLifeTime(n_bldata->lifetime); // 시간 0.1 ~ 0.2 추가
			//m_bullObj->SetIsFirstCreate(true);

			//불렛이 처음생성됐을 때의 시간을 저장. 이후 해당 이벤트마다 이 불렛의 생성주기시간을 더해줌
			__int64 currTime; QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
			m_bullObj->m_prevTime = currTime;
			
			m_bullobjs.emplace_back(m_bullObj);

			STC_Attack stc_attack;
			stc_attack.bull_data = move(m_bullObj->GetBulletInfo());
			stc_attack.lifetime = m_bullObj->GetBulletLifeTime();

			//불렛을 쏜 캐릭터의 공격애니메이션을 다른 클라이언트에게도 보내준다
			STC_CharAnimation stc_charani;
			stc_charani.id = m_bullObj->GetBulletMasterID();
			stc_charani.char_animation = Ani_State::Attack;

			//불렛이 생성된 위치를 나 말고도 상대방도 알고 있어야함. //상대방 클라이언트에게 내 불렛 생성위치를 보냄
			for (auto client : m_clients)
			{
				//불렛을 쏜 클라이언트는 자신이 불렛을 생성했으므로 따로 생성정보를 보내주지 않아도됨 
				//if (client->m_id == n_bldata->bull_data.Master_ID) continue;
				if (static_cast<bool>(client->m_isAI) == true || static_cast<bool>(client->m_connect_state) == false) continue;

				client->SendPacket(reinterpret_cast<Packet*>(&stc_charani));
				client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
			}



			if (n_bldata->bull_data.type == BULLET_TYPE::protocol_LightBullet)
				g_timer_queue.AddEvent(n_bldata->bull_data.myID, 0, LIGHT_BULLET, true, n_bldata->bull_data.Master_ID);

		}
		break;

	case PACKET_PROTOCOL_TYPE::TEST:
		{
			if (m_state == PLAYER_STATE::DEAD)
				break;

			auto test_data = reinterpret_cast<STC_Test*>(packet);
		
			cout << "ID: " << test_data->player_data.ID << "ElaspedTime: " << test_data->time.t_time << "------"
				"PrevTime: " << test_data->time.p_time << endl;

		}
		break;
	
	}
}

void Player_Session::Set_State(int state)
{
	if (state != -1)
		m_state = state;
}

void Player_Session::Damaged(float damage)
{
	if (m_playerData.GodMode == false && m_playerData.UserInfo.cur_hp > 0)
	{
		m_playerData.UserInfo.cur_hp -= damage;
	}

	if (m_playerData.UserInfo.cur_hp <= 0)
	{
		delobj = true;

		//리스폰 후처리 - timer_queue
	}

		
}

//1. 플레이어와 스테틱 오브젝트들의 충돌
void Player_Session::Collision_StaticObjects(unordered_set<StaticObject*>& sobjs, float DeltaTime)
{
	//주인은 사람 - 충돌 대상은 스테틱오브젝트
	for (auto iter = sobjs.begin(); iter != sobjs.end(); ++iter)
	{
		bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(),
					Lookvector, Rightvector, Upvector,
					(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

		if (test)
		{
			if (pp->pAxis.y > 0)
			{
				AirBone = false;
				m_playerData.AirBone = false;
			}
				
			if (pp->pAxis.y < 0)
			{
				(*iter)->SetAirBone(false);
			}
		
			XMFLOAT3 cn;
			if ((*iter)->GetIsStatic() == false)
			{
				cn = Float3Add(pp->GetPosition(), (*iter)->GetPhysicsPoint()->GetPosition(), false);
				cn = Float3Normalize(cn);
			}
			else
			{
				cn = pp->pAxis;
			}

			pp->CollisionResolve(*(*iter)->GetPhysicsPoint(), cn, DeltaTime);
			pe->UpdatePPosCenterPos(pp->GetPosition(), m_playerData.Pos);
			(*iter)->UpdatePPosCenterPos((*iter)->GetPhysicsPoint()->GetPosition());
			
		}
	}
}

void Player_Session::Collision_Players(vector<Player_Session*>& clients, float DeltaTime)
{
	for (auto iter = clients.begin(); iter != clients.end(); ++iter)
	{
		if (*iter != this)
		{
			bool test = pp->CollisionTest(*(*iter)->pp,
						Lookvector, Rightvector, Upvector,
						(*iter)->Lookvector, (*iter)->Rightvector, (*iter)->Upvector);

			if (test)//충돌했으면 충돌해소를 해야한다.
			{
				//충돌 했을때 축이 (0,1,0) 이면 Airbone을 false로 둔다. 이는 내가 위에있음을 나타낸다.
				if (pp->pAxis.y > 0)
				{
					AirBone = false;
					m_playerData.AirBone = false;
				}

				//충돌했을때  축이 (0,-1,0)이면 상대방 Airbone을 false로 둔다.  이는 상대가 내 위에있음을 나타낸다.
				//설사 상대 위에 다른 상대가 있어도 걱정말자. 자연스러운것임.
				if (pp->pAxis.y < 0)
				{
					(*iter)->m_playerData.AirBone = false;
					(*iter)->AirBone = false;
				}

				XMFLOAT3 cn;
				if ((*iter)->staticobject == false)
				{
					cn = Float3Add(pp->GetPosition(), (*iter)->pp->GetPosition(), false);
					cn = Float3Normalize(cn);
				}
				else//고정된 물체면 충돌한 평면의 노멀방향으로 cn을 설정할것.
				{
					cn = pp->pAxis;
				}

				//충돌해소 호출. 충돌해소 이후에 반드시 변경된 질점의 위치로 오브젝트위치를 일치시켜야한다.
				pp->CollisionResolve(*(*iter)->pp, cn, DeltaTime);
				pe->UpdatePPosCenterPos(pp->GetPosition(), m_playerData.Pos);
				(*iter)->pe->UpdatePPosCenterPos((*iter)->pp->GetPosition(), (*iter)->m_playerData.Pos);
					
			}
		}
	}
}

void Player_Session::RegularUpdate()
{
	//RegularUpdate 함수는 한번 들어오면 Loop를 돌면서 1초에 20번씩 
	//SendPacket을 해줘야함 - callback함수라 cost가 큼. 
	//스레드를 많이 할당받은 이 WorkerThread에서 작업해야함 

	auto local_prevTime = high_resolution_clock::now();
	while (true)
	{
		//1. 0.05초 뒤 업데이트된 불렛 정보 보내기
		auto local_durTime = high_resolution_clock::now() - local_prevTime;
		auto local_durTime_to_ms = duration_cast<milliseconds>(local_durTime).count();

		m_ElaspedTime = (local_durTime_to_ms / 1000.f) + m_prevTime;

		if (m_ElaspedTime > RegularPacketExchangeTime)
		{
			for (auto bullet : m_bullobjs)
			{
				STC_Attack stc_attack;
				stc_attack.pack_size = sizeof(STC_Attack);
				stc_attack.pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ATTACK;
				stc_attack.bull_data = move(bullet->GetBulletInfo());

				SendPacket(reinterpret_cast<Packet*>(&stc_attack));
			}

			m_prevTime = m_ElaspedTime - RegularPacketExchangeTime;
		}
	}
}



Player_Session::~Player_Session()
{
	delete pe;
	delete rb;
	delete pp;
}



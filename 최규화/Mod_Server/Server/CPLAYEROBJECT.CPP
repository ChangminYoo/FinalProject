#include "stdafx.h"

CPlayerObject::~CPlayerObject()
{
	//if (scene != nullptr)
	//	delete scene;

	if (g_bullobj != nullptr)
		delete g_bullobj;
}

bool CPlayerObject::CheckPlayerInfo()
{
	//scene = new CScene();
	//접속연결시 플레이어의 아이디 m_playerIndex (ai + player)
	/*
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

	m_connect = true;

	//m_staticobject = new StaticObject();

	return true;
	}
	*/
	return true;
}

void CPlayerObject::Init_MonsterInfo()
{

}

void CPlayerObject::Init_PlayerInfo()
{
	m_state = IDLE;

	m_ani = Ani_State::Idle;
	m_connect = true;
	m_dir = 0;
	m_airbone = false;
	m_ai = false;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };
	m_godmode = false;
	m_fixed = false;
	m_alive = true;
	m_type = OBJECT_TYPE::PLAYER;

	m_ability.attack = 10;
	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.exp = 0;
	m_ability.level = 1;
	m_ability.speed = 30000;

	if (m_id == 0)
		m_pos4f = { -100.f, -1000.f, 0.f, 0.f };
	else if (m_id == 1)
		m_pos4f = { 300.f, -1000.f, 0.f, 0.f };

	m_OffLookvector = XMFLOAT3(0, 0, -1);
	m_OffRightvector = XMFLOAT3(-1, 0, 0);

	XMFLOAT4 xmf4_rot{ m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, MMPE_PI);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_rot4f.x = xmf4_rot.x; m_rot4f.y = xmf4_rot.y; m_rot4f.z = xmf4_rot.z; m_rot4f.w = xmf4_rot.w;

	UpdateLookvector();
	UpdateUpvector();

	//물리효과 및 충돌처리를 위한 PhysicsPoint 클래스
	pp = new PhysicsPoint();

	pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
	pp->SetHalfBox(3, 10, 3);
	pp->SetDamping(0.7);
	pp->SetBounce(false);
	//

	//2. 물리효과 적용
	GravitySystem(0);

	XMFLOAT4 xmf4_pos{ m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->integrate(0, &xmf4_pos);
	m_pos4f.x = xmf4_pos.x;  m_pos4f.y = xmf4_pos.y; m_pos4f.z = xmf4_pos.z; m_pos4f.w = xmf4_pos.w;

	AfterGravitySystem(0);

	//wcscpy(m_pdata.LoginData.name, m_loginID);
	//wcscpy(m_pdata.LoginData.password, m_loginPW);
}

void CPlayerObject::InitData_To_Client()
{
	STC_SetMyClient stc_init;

	m_pdata.pos = m_pos4f;
	m_pdata.rot = m_rot4f;
	m_pdata.ai = m_ai;
	m_pdata.airbone = m_airbone;
	m_pdata.ani = m_ani;
	m_pdata.connect = m_connect;
	m_pdata.dir = m_dir;
	m_pdata.godmode = m_godmode;
	m_pdata.id = m_id;

	m_pdata.status.attack = m_ability.attack;
	m_pdata.status.speed = m_ability.speed;
	m_pdata.status.origin_hp = m_ability.orignHP;
	m_pdata.status.cur_hp = m_ability.curHP;
	m_pdata.status.exp = m_ability.exp;
	m_pdata.status.level = m_ability.level;

	stc_init.player_data = move(m_pdata);

	g_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&stc_init));

	cout << "Current ID: " << m_pdata.id << "Pos(x,y,z,w): " << m_pdata.pos.x << " " <<
		m_pdata.pos.y << " " << m_pdata.pos.z << " " << m_pdata.pos.w << endl;
	//------------------------------------------------------------------------------------

	STC_SetOtherClient stc_init_other;

	for (auto i = 0; i < g_clients.size(); ++i)
	{
		if (g_clients[i]->m_id == m_id) continue;
		if (g_clients[i]->m_connect != false || g_clients[i]->m_ai != true)
		{
			// 1. 다른 클라이언트 정보를 내 클라이언트에게 보낸다
			stc_init_other.player_data = move(g_clients[i]->GetPlayerData());
			g_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&stc_init_other));

			ZeroMemory(&stc_init_other, sizeof(stc_init_other));

			stc_init_other.player_data = move(stc_init.player_data);
			g_clients[i]->SendPacket(reinterpret_cast<Packet*>(&stc_init_other));
		}
	}


	//------------------------------------------------------------------------------------


}

/*
void CPlayerObject::RegularUpdate()
{
	
	auto local_prevTime = high_resolution_clock::now();
	while (true)
	{
		//1. 0.05초 뒤 업데이트된 불렛 정보 보내기
		auto local_durTime = high_resolution_clock::now() - local_prevTime;
		auto local_durTime_to_ms = duration_cast<milliseconds>(local_durTime).count();

		m_elaspedtime = (local_durTime_to_ms / 1000.f) + m_prevtime;

		if (m_elaspedtime > RegularPacketExchangeTime)
		{
			for (auto bullet : g_bullets)
			{
				STC_Attack stc_attack;
				stc_attack.pack_size = sizeof(STC_Attack);
				stc_attack.pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ATTACK;
				stc_attack.bull_data = move(bullet->GetBulletInfo());

				SendPacket(reinterpret_cast<Packet*>(&stc_attack));
			}

			m_prevtime = m_elaspedtime - RegularPacketExchangeTime;
		}
	}
	
}
*/

void CPlayerObject::SendPacket(Packet * packet)
{
	int packet_size = packet[0];
	Packet *new_sendBuf = new Packet[packet_size];
	memcpy(new_sendBuf, packet, packet_size);

	//1. async_write_some - 비동기 IO / 받은데이터를 즉시 보냄(따로 버퍼에 저장X)
	//2. async_write - 비동기 IO / 보내고자 하는 데이터가 모두 버퍼에 담기면 데이터를 보냄
	boost::asio::async_write(m_socket, boost::asio::buffer(new_sendBuf, packet_size),
		[=](const boost::system::error_code& error, const size_t& bytes_transferred)
	{
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

void CPlayerObject::SendStaticObjectPacket(const unordered_set<CStaticObject*>& sobjs)
{
	STC_StaticObject stc_set_sobjs;

	for (auto sobj : sobjs)
	{
		stc_set_sobjs.sobj_data = move(sobj->m_stc_sobjdata);
		g_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&stc_set_sobjs));
	}
}

void CPlayerObject::RecvPacket()
{
	//m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER_SIZE),
	//[=](const boost::system::error_code& error, size_t bytes_transferred)

	//패킷받을 때 버퍼사이즈는 전달된 버퍼의 크기만큼만 할당해야한다
	//1바이트를 받아 패킷사이즈를 알고 해당 패킷 사이즈를 이용해 다시 패킷을 받는다
	//boost::asio::async_read(m_socket, boost::asio::buffer(m_recvBuf , 1),
	//	[&](const boost::system::error_code& error, size_t bytes_transferred)
	m_socket.async_read_some(boost::asio::buffer(m_recvbuf, MAX_BUFFER_SIZE),
		[&](const boost::system::error_code& error, const size_t& bytes_transferred)
	{
		//cout << "Bytes_Transferred: " << bytes_transferred << endl;
		// error = 0 성공 , error != 0 실패
		if (error)  // !error = 에러안났을때
		{
			STC_Disconnected disconnect_data;
			disconnect_data.connect = false;
			disconnect_data.id = m_id;

			// AI와 연결이 이미 끊긴 클라이언트에게는 연결을 끊으라는 패킷을 보내지 않는다
			for (auto client : g_clients)
			{
				if (client->m_id == disconnect_data.id) continue;
				if (static_cast<bool>(client->m_ai)!= true && static_cast<bool>(client->m_connect) != false)
				{
					client->SendPacket(reinterpret_cast<Packet*>(&disconnect_data));
				}
			}


			//에러: 작업이 취소된 경우 
			cout << "Client No. [ " << m_id << " ] error code : " << error.value() << endl;

			cout << "IP: " << m_socket.remote_endpoint().address().to_string() << " // ";
			cout << "PORT: " << m_socket.remote_endpoint().port() << endl;

			// 자신의 연결상태 - 끊음
			SetState(PLAYER_STATE::DISCONNECT);

			m_connect = false;
			// shutdown_both - 주고 받는 쪽 모두를 중단
			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();

			return;
		}

		int cur_data_proc = static_cast<int>(bytes_transferred);
		Packet* temp_buf = m_recvbuf;

		while (cur_data_proc > 0)
		{
			if (m_curr_packet_size == 0)			//이렇게 해야 패킷의 첫 정보는 무조건 패킷사이즈가 됨
			{
				m_curr_packet_size = temp_buf[0];
				if (temp_buf[0] > MAX_BUFFER_SIZE)
				{
					cout << "RecvPacket() Error, Client No. [ " << m_id << " ] recvBuf[0] is out of MAX_BUF_SIZE\n";
					exit(-1);
				}
			}

			int need_to_read = m_curr_packet_size - m_prev_packet_size;

			if (need_to_read <= cur_data_proc)
			{
				memcpy(m_databuf + m_prev_packet_size, temp_buf, need_to_read);

				ProcessPacket(m_databuf);

				cur_data_proc -= need_to_read;
				temp_buf += need_to_read;
				m_prev_packet_size = 0;
				m_curr_packet_size = 0;

				//cout << "cur_data_proc: " << cur_data_proc << " --- " << "need_to_read: " << need_to_read << endl;
			}
			else
			{
				memcpy(m_databuf + m_prev_packet_size, temp_buf, cur_data_proc);
				m_prev_packet_size += cur_data_proc;
				cur_data_proc -= cur_data_proc;
				temp_buf += cur_data_proc;
			}
		}

		RecvPacket();
	});
}

void CPlayerObject::ProcessPacket(Packet * packet)
{
	//packet[0] - 패킷의 사이즈
	//packet[1] - 해당 패킷이 하는일
	//packet[2] - 해당 패킷에 담겨진 데이터들

	switch (packet[1])
	{
	case PACKET_PROTOCOL_TYPE::PLAYER_MOVE:
	{
		if (m_state == PLAYER_STATE::DEAD)
			break;

		auto mdata = reinterpret_cast<STC_CharMove*>(packet);
		m_dir = mdata->dir;

	}
	break;

	/*
	case PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION:
	{
		
		if (m_state == PLAYER_STATE::DEAD)
			break;

		1. 받아들인 데이터(키를 눌러 플레이어를 움직였음)에서 변화된 정보를 추출(물리효과 적용x)
		auto PosMove_Data = reinterpret_cast<STC_ChangedPos*>(packet);

		if (PosMove_Data->deltime <= 0)
			PosMove_Data->deltime = 0.f;

		if (PosMove_Data->ani_state == Ani_State::Idle)
			m_state = PLAYER_STATE::IDLE;
		else if (PosMove_Data->ani_state == Ani_State::Run)
			m_state = PLAYER_STATE::MOVE;

		2. 물리효과 적용
		GravitySystem(PosMove_Data->deltime);
		g_clients[PosMove_Data->id]->GravitySystem(PosMove_Data->deltime);

		Tick(PosMove_Data->deltime,PosMove_Data->pos);
		g_clients[PosMove_Data->id]->Tick(PosMove_Data->deltime, PosMove_Data->pos);

		pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
		g_clients[PosMove_Data->id]->pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);

		AfterGravitySystem(PosMove_Data->deltime);
		g_clients[PosMove_Data->id]->AfterGravitySystem(PosMove_Data->deltime);

		3. 이동 - (애니메이션, 위치 변경) 변경된 데이터를 서버에서관리하는 내 클라이언트에 저장
		g_clients[PosMove_Data->id]->m_ani = PosMove_Data->ani_state;
		SetChangedPlayerState();

		g_clients[PosMove_Data->id]->m_pos4f;
		

		cout << "ID: " << PosMove_Data->id << " 변화된 위치값: " << "[x:" << PosMove_Data->pos.x << "\t" << "y:" << PosMove_Data->pos.y
			<< "\t" << "z:" << PosMove_Data->pos.z << "]" << "\t" << "w:" << PosMove_Data->pos.w << endl;

		4. 변화된 내 (포지션, 애니메이션) 정보를 다른 클라에 전달 - 반드시 이렇게 다시 만들어줘야함
		PosMove_Data를 바로 sendpacket에 packet으로 형변화하여 보내면 size error가 난다
		
		STC_ChangedPos stc_other_pos;

		stc_other_pos.id = PosMove_Data->id;
		stc_other_pos.ani_state = PosMove_Data->ani_state;
		stc_other_pos.pos = move(g_clients[PosMove_Data->id]->m_pos4f);

		for (auto client : g_clients)
		{
			//상대가 ai / 연결끊김 / 나일 경우 보낼 필요 없음
			if (static_cast<bool>(client->m_pdata.ai) == true) continue;
			if (static_cast<bool>(client->m_pdata.connect) == false) continue;

			//if (client->m_pdata.ID == PosMove_Data->id) continue;
			//여기서 문제

			//갱신된 나의 데이터를 상대방에게 전달
			client->SendPacket(reinterpret_cast<Packet*>(&stc_other_pos));
		}
		
	}
	break;
	*/

	case PACKET_PROTOCOL_TYPE::PLAYER_ROTATE:
	{
		if (m_state == PLAYER_STATE::DEAD)
			break;

		m_state = PLAYER_STATE::ROTATE;

		auto rot_data = reinterpret_cast<STC_Rotation*>(packet);

		// 1. 받은 정보를 내 클라이언트에 넣어주고
		m_rot4f = move(rot_data->rotate_status);
		m_pdata.rot = move(rot_data->rotate_status);
		
		// 2. 받은 정보를 토대로 lookvector와 rightvector를 업데이트
		UpdateLookvector();
		UpdateUpvector();

		//cout << "ID: " << rot_data->id << " 변화된 회전값: " << "[ x, y, z, w ]: "
		//	<< rot_data->rotate_status.x << ", " << rot_data->rotate_status.y << ", " << rot_data->rotate_status.z << ", " << rot_data->rotate_status.w << endl;

		// 3. 다른 클라에게 보낸다.
		STC_Rotation stc_to_rot;
		stc_to_rot.id = rot_data->id;
		stc_to_rot.rotate_status = move(m_rot4f);

		for (auto client : g_clients)
		{
			if (client->GetIsAI() == true || client->GetConnectState() == false) continue;
			if (client->m_id == rot_data->id) continue;

			client->SendPacket(reinterpret_cast<Packet*>(&stc_to_rot));
		}

	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ATTACK:
	{
		if (m_state == PLAYER_STATE::DEAD)		
			break;

		m_state = PLAYER_STATE::ATTACK;

		auto n_bldata = reinterpret_cast<CTS_Attack*>(packet);

		//공격키를 눌렀을 시, 불렛 생성.
		//불렛을 생성한 캐릭터 ID, 유도를 대비한 타겟 ID, 불렛 초기생성위치, 불렛 초기회전값, 불렛 생성시간, 불렛아이디
		g_bullobj = new CBulletObject(n_bldata->bull_data.master_id, n_bldata->bull_data.my_id,
			n_bldata->bull_data.pos4f, n_bldata->bull_data.rot4f, n_bldata->lifetime,
			n_bldata->bull_data.vel3f, n_bldata->bull_data.type, n_bldata->bull_data.endpoint);

		STC_Attack stc_attack;
		stc_attack.bull_data.pos4f = move(n_bldata->bull_data.pos4f);
		stc_attack.bull_data.rot4f = move(n_bldata->bull_data.rot4f);
		stc_attack.bull_data.master_id = n_bldata->bull_data.master_id;
		stc_attack.bull_data.my_id = n_bldata->bull_data.my_id;
		stc_attack.bull_data.type = n_bldata->bull_data.type;
		stc_attack.bull_data.alive = n_bldata->bull_data.alive;
		stc_attack.bull_data.endpoint = move(n_bldata->bull_data.endpoint);

		for (auto client : g_clients)
		{
			if (client->m_id == n_bldata->bull_data.master_id) continue;
			if (client->GetIsAI() == true || client->GetConnectState() == false) continue;

			client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
		}

		g_bullets.emplace_back(g_bullobj);

		//불렛을 쏜 캐릭터의 공격애니메이션을 다른 클라이언트에게도 보내준다
		STC_CharAnimation stc_charani;
		stc_charani.id = g_bullobj->GetBulletMasterID();
		stc_charani.ani_state = Ani_State::Attack;

		//불렛이 생성된 위치를 나 말고도 상대방도 알고 있어야함. //상대방 클라이언트에게 내 불렛 생성위치를 보냄
		for (auto client : g_clients)
		{
			//불렛을 쏜 클라이언트는 자신이 불렛을 생성했으므로 따로 생성정보를 보내주지 않아도됨 
			if (client->m_id == n_bldata->bull_data.master_id) continue;
			if (client->GetIsAI() == true || client->GetConnectState() == false) continue;

			client->SendPacket(reinterpret_cast<Packet*>(&stc_charani));

		}
		//if (n_bldata->bull_data.type == BULLET_TYPE::protocol_LightBullet)
		//	g_timer_queue.AddEvent(n_bldata->bull_data.myID, 0, LIGHT_BULLET, true, n_bldata->bull_data.Master_ID);
		//if (n_bldata->bull_data.type == BULLET_TYPE::protocol_HeavyBullet)
		//	g_timer_queue.AddEvent(n_bldata->bull_data.myID, 0, HEAVY_BULLET, true, n_bldata->bull_data.Master_ID);
	}
	break;

	case PACKET_PROTOCOL_TYPE::TEST:
	{
		if (m_state == DEAD)
			break;

		auto test_data = reinterpret_cast<STC_Test*>(packet);

		cout << "ID: " << test_data->player_data.id << "ElaspedTime: " << test_data->time.t_time << "------"
			"PrevTime: " << test_data->time.p_time << endl;

	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ANIMATION:
	{
		if (m_state == DEAD)
			break;

		auto ani = reinterpret_cast<STC_CharAnimation*>(packet);

		if (ani->ani_state == Ani_State::Idle)
			m_state = Ani_State::Idle;
		else if (ani->ani_state == Ani_State::Run)
			m_state = Ani_State::Run;
		else if (ani->ani_state == Ani_State::Attack)
			m_state = Ani_State::Attack;
		else if (ani->ani_state == Ani_State::Dead)
			m_state = Ani_State::Dead;

		g_clients[ani->id]->m_ani = ani->ani_state;
		g_clients[ani->id]->m_pdata.ani = ani->ani_state;

		STC_CharAnimation stc_c_ani;
		stc_c_ani.ani_state = ani->ani_state;
		stc_c_ani.id = ani->id;

		for (auto client : g_clients)
		{
			if (client->m_id == ani->id) continue;
			if (client->GetIsAI() == true || client->GetConnectState() == false) continue;

			client->SendPacket(reinterpret_cast<Packet*>(&stc_c_ani));
		}
	}
	break;

	}
}

void CPlayerObject::GravitySystem(double deltime)
{
	GeneratorGravity gg;
	gg.SetGravityAccel(XMFLOAT3(0, -100, 0));

	if (m_fixed == false)
	{
		gg.Update(deltime, *pp);
	}
}

void CPlayerObject::Tick(double deltime)
{
	XMFLOAT4 xmf4_pos{ m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->integrate(deltime, &xmf4_pos);

	m_pos4f.x = xmf4_pos.x;
	m_pos4f.y = xmf4_pos.y;
	m_pos4f.z = xmf4_pos.z;
	m_pos4f.w = xmf4_pos.w;
}

void CPlayerObject::Tick(double deltime, Position& pos4f)
{
	XMFLOAT4 xmf4{ pos4f.x, pos4f.y, pos4f.z, pos4f.w };
	pp->integrate(deltime, &xmf4);

	m_pos4f.x = xmf4.x;
	m_pos4f.y = xmf4.y;
	m_pos4f.z = xmf4.z;
	m_pos4f.w = xmf4.w;
}

void CPlayerObject::AfterGravitySystem(double deltime)
{
	//cout << "AfterGravitySystem PosY: " << m_pos4f.y << "\n";

	float ppy = pp->GetPosition().y;
	float hby = pp->GetHalfBox().y;
	if (ppy - hby < 0)
	{
		XMFLOAT3 gp = pp->GetPosition();
		gp.y += hby - ppy;
		pp->SetPosition(gp);
		UpdatePPosCenterPos();
		auto v = pp->GetVelocity();
		v.y = 0;
		pp->SetVelocity(v);
		m_airbone = false;
	}
}

void CPlayerObject::Collision(vector<CPlayerObject*>* clients, double deltime)
{
	
	for (auto iter = clients->begin(); iter != clients->end(); ++iter)
	{
		if (*iter != this)
		{
			bool test = pp->CollisionTest(*(*iter)->pp,
				m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->m_Lookvector, (*iter)->m_Rightvector, (*iter)->m_Upvector);

			if (test)//충돌했으면 충돌해소를 해야한다.
			{
				//충돌 했을때 축이 (0,1,0) 이면 Airbone을 false로 둔다. 이는 내가 위에있음을 나타낸다.
				if (pp->pAxis.y > 0)
				{
					m_airbone = false;
				}

				//충돌했을때  축이 (0,-1,0)이면 상대방 Airbone을 false로 둔다.  이는 상대가 내 위에있음을 나타낸다.
				//설사 상대 위에 다른 상대가 있어도 걱정말자. 자연스러운것임.
				if (pp->pAxis.y < 0)
				{
					(*iter)->pp->SetVelocity((*iter)->pp->GetVelocity().x, 0, (*iter)->pp->GetVelocity().z);
					(*iter)->m_airbone = false;
				}

				XMFLOAT3 cn;
				cn = Float3Add(pp->GetPosition(), (*(*iter)->pp).GetPosition(), false);
				cn = Float3Normalize(cn);

				//충돌해소 호출. 충돌해소 이후에 반드시 변경된 질점의 위치로 오브젝트위치를 일치시켜야한다.
				pp->CollisionResolve(*(*iter)->pp, cn, deltime);
				UpdatePPosCenterPos();
				(*iter)->UpdatePPosCenterPos();

				cout << "사람과 충돌" << endl;

			}
		}
	}
	
}

void CPlayerObject::Collision(unordered_set<CStaticObject*>* sobjs, double deltime)
{
	//주인은 사람 - 충돌 대상은 스테틱오브젝트
	
	for (auto iter = sobjs->begin(); iter != sobjs->end(); ++iter)
	{
		bool test = pp->CollisionTest(*(*iter)->GetPhysicsPoint(), m_Lookvector, m_Rightvector, m_Upvector,
			(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

		if (test)
		{
			if (pp->pAxis.y > 0)
			{
				pp->SetVelocity(pp->GetVelocity().x, 0, pp->GetVelocity().z);
				m_airbone = false;
			}

			if (pp->pAxis.y < 0)
			{
				(*iter)->GetPhysicsPoint()->SetVelocity((*iter)->GetPhysicsPoint()->GetVelocity().x, 0, (*iter)->GetPhysicsPoint()->GetVelocity().z);
				(*iter)->SetAirbone(false);
			}

			XMFLOAT3 cn;
			cn = pp->pAxis;
			
			pp->CollisionResolve(*(*iter)->GetPhysicsPoint(), cn, deltime);
			UpdatePPosCenterPos();
			(*iter)->UpdatePPosCenterPos();

			cout << "스테틱 오브젝트와 충돌 " << endl;

		}
	}
	
}

void CPlayerObject::GetDamaged(int damage)
{
	if (m_godmode == false && m_ability.curHP > 0)
	{
		//캐릭터 hp를 감소
		m_ability.curHP -= damage;
	}

	if (m_pdata.status.cur_hp <= 0)
	{
		//캐릭터 m_alive를 false 로 바꿔준다
		m_alive = false;
	}

}

void CPlayerObject::SetChangedPlayerState()
{
	m_pdata.ai = m_ai;
	m_pdata.airbone = m_airbone;
	//m_pdata.ani = m_ani;		//애니메이션은 패킷받자마자 처리
	m_pdata.connect = m_connect;
	//m_pdata.dir = m_dir;
	m_pdata.godmode = m_godmode;
	m_pdata.id = m_id;
	m_pdata.pos = m_pos4f;
	//m_pdata.rot = m_rot4f;	//회전은 패킷받자마자 처리

	m_pdata.status.attack = m_ability.attack;
	m_pdata.status.cur_hp = m_ability.curHP;
	m_pdata.status.exp = m_ability.exp;
	m_pdata.status.level = m_ability.level;
	m_pdata.status.origin_hp = m_ability.orignHP;
	m_pdata.status.speed = m_ability.speed;

	pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
}

void CPlayerObject::PlayerInput(double deltime)
{
	switch (m_dir)
	{
		case CHAR_MOVE::FRONT_MOVE:
		{
			auto l = XMLoadFloat3(&m_Lookvector);
			l *= m_ability.speed * deltime;

			XMFLOAT4 xmf4{ m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
			auto p = XMLoadFloat4(&xmf4);
			p += l;
			XMStoreFloat4(&xmf4, p);
			pp->SetPosition(xmf4);

			m_pos4f.x = xmf4.x; m_pos4f.y = xmf4.y; m_pos4f.z = xmf4.z; m_pos4f.w = xmf4.w;
			
			//m_dir = 0;

			//cout << "Front Move Pos[x, y, z, w]: " << m_pos4f.x << " , " << m_pos4f.y << " , " << m_pos4f.z << " , " << m_pos4f.w << endl;
			//cout << "Front Move!!" << endl;
		}
		break;

		case CHAR_MOVE::BACK_MOVE:
		{
			auto l = XMLoadFloat3(&m_Lookvector);
			l *= m_ability.speed * deltime;

			XMFLOAT4 xmf4{ m_pos4f.x, m_pos4f.y,m_pos4f.z,m_pos4f.w };
			auto p = XMLoadFloat4(&xmf4);
			p -= l;
			XMStoreFloat4(&xmf4, p);
			pp->SetPosition(xmf4);

			m_pos4f.x = xmf4.x; m_pos4f.y = xmf4.y; m_pos4f.z = xmf4.z; m_pos4f.w = xmf4.w;
			
			//m_dir = 0;

			//cout << "Back Move Pos[x, y, z, w]: " << m_pos4f.x << " , " << m_pos4f.y << " , " << m_pos4f.z << " , " << m_pos4f.w << endl;
			//cout << "Back Move!!" << endl;
		}
		break;

		case CHAR_MOVE::LEFT_MOVE:
		{
			

			auto r = XMLoadFloat3(&m_Rightvector);
			r *= m_ability.speed * deltime;
			
			XMFLOAT4 xmf4{ m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };

			//cout << "Before Left Move Pos[x, y, z, w]: " << xmf4.x << ", " << xmf4.y << ", " << xmf4.z << ", " << xmf4.w << endl;

			auto p = XMLoadFloat4(&xmf4);
			p -= r;
			XMStoreFloat4(&xmf4, p);
			pp->SetPosition(xmf4);

			m_pos4f.x = xmf4.x; m_pos4f.y = xmf4.y; m_pos4f.z = xmf4.z; m_pos4f.w = xmf4.w;
			
			//m_dir = 0;

			//cout << "Left Move Pos[x, y, z, w]: " << m_pos4f.x << " , " << m_pos4f.y << " , " << m_pos4f.z << " , " << m_pos4f.w << endl;
			//cout << "Left Move!!" << endl;
		}
		break;

		case CHAR_MOVE::RIGHT_MOVE:
		{
			auto r = XMLoadFloat3(&m_Rightvector);
			r *= m_ability.speed * deltime;

			XMFLOAT4 xmf4{ m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
			auto p = XMLoadFloat4(&xmf4);
			p += r;
			XMStoreFloat4(&xmf4, p);
			pp->SetPosition(xmf4);

			m_pos4f.x = xmf4.x; m_pos4f.y = xmf4.y; m_pos4f.z = xmf4.z; m_pos4f.w = xmf4.w;
			
			//m_dir = 0;

			//cout << "Right Move Pos[x, y, z, w]: " << m_pos4f.x << " , " << m_pos4f.y << " , " << m_pos4f.z << " , " << m_pos4f.w << endl;
			//cout << "Right Move!!" << endl;
		}
		break;

		case CHAR_MOVE::JUMP:
		{
			/*
			GeneratorJump j;
			j.SetJumpVel(XMFLOAT3(0, 100, 0));
			j.Update(deltime, *pp);
			m_airbone = true;	
			*/
			//cout << "Jump!!" << endl;

			//m_dir = 0;
		}
		break;

	default:
		break;
	}

}

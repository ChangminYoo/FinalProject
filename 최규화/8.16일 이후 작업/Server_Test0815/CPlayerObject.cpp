#include "stdafx.h"
int CPlayerObject::m_topRankerID = -1;

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
	//���ӿ���� �÷��̾��� ���̵� m_playerIndex (ai + player)
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

void CPlayerObject::CheckMyClient()
{

}

void CPlayerObject::Init_PlayerInfo()
{
	m_state = IDLE;

	m_set_first_moveobjs = false;

	//-------------------------- �⺻ �����ͼ���
	m_killCount = 0;
	m_deathCount = 0;
	m_player_score = 0;
	m_myCurrRank = MAX_PLAYER;
	m_myOldRank = MAX_PLAYER;
	m_isTopRanker = false;

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

	m_ability.attack = 1000;
	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.exp = 0;
	m_ability.level = 1;
	m_ability.speed = 100;

	if (m_id % MAX_PLAYER_PER_GAME == 0)
		m_pos4f = { -290.f, -1000.f, 310.f, 0.f };
	else if ((m_id % MAX_PLAYER_PER_GAME) == 1)
		m_pos4f = { 290.f, -1000.f, 310.f, 0.f };
	else if ((m_id % MAX_PLAYER_PER_GAME) == 2)
		m_pos4f = { 0.f, -1000.f, -420.f, 0.f };
	else if ((m_id % MAX_PLAYER_PER_GAME) == 3)
		m_pos4f = { 400.f, -1000.f, -100.f, 0.f };
	else if ((m_id % MAX_PLAYER_PER_GAME) == 4)
		m_pos4f = { -400.f, -1000.f, -120.f, 0.f };

	//if (m_id == 0)
	//	m_player_score = 100;

	m_orgPos4f = m_pos4f;
	//---------------------------- Orient�� �̿��� Lookvector // Rightvector // Upvector ����
	//---------------------------- xmf4_rot -> m_rot4 ��

	m_OffLookvector = XMFLOAT3(0, 0, -1);
	m_OffRightvector = XMFLOAT3(-1, 0, 0);

	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, MMPE_PI);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();
	UpdateUpvector();

	//------------------------------


	//����ȿ�� �� �浹ó���� ���� PhysicsPoint Ŭ����
	//------------------------------ ����ȿ�� �⺻ ������ ����

	pp = new PhysicsPoint();
	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(3, 10, 3);
	pp->SetDamping(0.7);
	pp->SetBounce(false);
	
	//------------------------------ ����ȿ�� ����
	GravitySystem(0);

	pp->integrate(0);
	m_pos4f = { xmf4_pos.x, xmf4_pos.y, xmf4_pos.z, xmf4_pos.w };

	AfterGravitySystem(0);

	//------------------------------

	CalculatePlayerScoreForRanking();
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
	m_pdata.killcount = m_killCount;
	m_pdata.deathcount = m_deathCount;
	m_pdata.score = m_player_score;
	m_pdata.rank = m_myCurrRank;
	m_pdata.topRank = false;
	m_pdata.alive = m_alive;
	m_pdata.respawn_cnt = 0;				//0 : �� 

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
			// 1. �ٸ� Ŭ���̾�Ʈ ������ �� Ŭ���̾�Ʈ���� ������
			stc_init_other.player_data = move(g_clients[i]->GetPlayerData());
			g_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&stc_init_other));

			ZeroMemory(&stc_init_other, sizeof(stc_init_other));

			stc_init_other.player_data = move(stc_init.player_data);
			g_clients[i]->SendPacket(reinterpret_cast<Packet*>(&stc_init_other));
		}
	}


	//------------------------------------------------------------------------------------
}

void CPlayerObject::InitNPCData_To_Client()
{
	STC_SetMyNPC stc_npc_init;

	for (auto npc : g_npcs)
	{
		stc_npc_init.npc_data = move(npc->GetMyBasicPacketData());
		for (auto client : g_clients)
		{
			if (!client->GetConnectState()) continue;
			if ( client->GetIsAI()) continue;

			client->SendPacket(reinterpret_cast<Packet*>(&stc_npc_init));
		}

	}
}

void CPlayerObject::SendPacket(Packet * packet)
{
	int packet_size = packet[0];
	Packet *new_sendBuf = new Packet[packet_size];
	memcpy(new_sendBuf, packet, packet_size);

	//1. async_write_some - �񵿱� IO / ���������͸� ��� ����(���� ���ۿ� ����X)
	//2. async_write - �񵿱� IO / �������� �ϴ� �����Ͱ� ��� ���ۿ� ���� �����͸� ����
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
		stc_set_sobjs.sobj_data = move(sobj->GetStaticObjectInfo());
		g_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&stc_set_sobjs));
	}
}

void CPlayerObject::RecvPacket()
{
	//m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER_SIZE),
	//[=](const boost::system::error_code& error, size_t bytes_transferred)

	//��Ŷ���� �� ���ۻ������ ���޵� ������ ũ�⸸ŭ�� �Ҵ��ؾ��Ѵ�
	//1����Ʈ�� �޾� ��Ŷ����� �˰� �ش� ��Ŷ ����� �̿��� �ٽ� ��Ŷ�� �޴´�
	//boost::asio::async_read(m_socket, boost::asio::buffer(m_recvBuf , 1),
	//	[&](const boost::system::error_code& error, size_t bytes_transferred)

	//** [1]. RecvPacket �Լ� ������ �ð��� �����ɸ� �� �ִ� ProcessPacket �Լ��� ȣ���Ͽ� �����Ѵ�
	//   [2]. ���� io_service::run �� �̱۽����尡 �ƴ� ��Ƽ������ ȯ�濡�� �۵��Ǵ� ���̴�
	//   [1]�� [2]�� ��츦 �����ϴ� ��� ���� sendpacket �� ���� ������ recvpacket���� ó������ ���� ä
	//   �ٽ� recvpacket�� ȣ��Ǿ� ���ۿ� ����� ���� �ڼ��� �� �ִ�
	//   �̷� ��츦 �����ϱ� ���� strand�� ����Ͽ� �ش� �ڵ鷯�� �۾��� �Ϸ��ϱ� ������ �ش� �ڵ鷯�� 
	//   �ٽ� ȣ������ ���ϵ��� ���´�
	//m_socket.async_read_some(boost::asio::buffer(m_recvbuf, MAX_BUFFER_SIZE),
	//	[&](const boost::system::error_code& error, const size_t& bytes_transferred)
	m_socket.async_read_some(boost::asio::buffer(m_recvbuf, MAX_BUFFER_SIZE),
		m_strand.wrap([&](const boost::system::error_code& error, const size_t& bytes_transferred)
	{
		//cout << "Bytes_Transferred: " << bytes_transferred << endl;
		// error = 0 ���� , error != 0 ����
		if (error)  // !error = �����ȳ�����
		{
			STC_Disconnected disconnect_data;
			disconnect_data.connect = false;
			disconnect_data.id = m_id;

			// AI�� ������ �̹� ���� Ŭ���̾�Ʈ���Դ� ������ ������� ��Ŷ�� ������ �ʴ´�
			for (auto client : g_clients)
			{
				if (client->m_id == disconnect_data.id) continue;
				if (static_cast<bool>(client->m_ai)!= true && static_cast<bool>(client->m_connect) != false)
				{
					client->SendPacket(reinterpret_cast<Packet*>(&disconnect_data));
				}
			}


			//����: �۾��� ��ҵ� ��� 
			cout << "Client No. [ " << m_id << " ] error code : " << error.value() << endl;

			cout << "IP: " << m_socket.remote_endpoint().address().to_string() << " // ";
			cout << "PORT: " << m_socket.remote_endpoint().port() << endl;

			// �ڽ��� ������� - ����
			SetState(PLAYER_STATE::DISCONNECT);

			m_connect = false;
			// shutdown_both - �ְ� �޴� �� ��θ� �ߴ�
			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();

			return;
		}

		int cur_data_proc = static_cast<int>(bytes_transferred);
		Packet* temp_buf = m_recvbuf;

		while (cur_data_proc > 0)
		{
			if (m_curr_packet_size == 0)			//�̷��� �ؾ� ��Ŷ�� ù ������ ������ ��Ŷ����� ��
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
	}));
}

void CPlayerObject::ProcessPacket(Packet * packet)
{
	//packet[0] - ��Ŷ�� ������
	//packet[1] - �ش� ��Ŷ�� �ϴ���
	//packet[2] - �ش� ��Ŷ�� ����� �����͵�

	switch (packet[1])
	{
	case PACKET_PROTOCOL_TYPE::LOAD_SCENE_SELECT_CHARACTER:
	{
		auto data = reinterpret_cast<STC_CHAR_NUMBER_LOAD_SCENE*>(packet);

		STC_CHAR_NUMBER_LOAD_SCENE stc_number;
		stc_number.show_char_number.sel_id = data->show_char_number.sel_id;

		for (const auto& client : g_clients)
		{
			stc_number.show_char_number.my_id = client->GetID();
			client->SendPacket(reinterpret_cast<Packet*>(&stc_number));
		}

	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_LOGIN:
	{
		auto data = reinterpret_cast<CTS_PLAYER_LOGIN*>(packet);

		bool ready_flag = false;
		int curr_access_cnt = 0;

		if (data->logindata.isReady)
		{
			m_isReady = data->logindata.isReady;
			m_myTextureID = data->logindata.texture_id;
		}

		for (const auto& client : g_clients)
		{
			if (client->GetIsReady())
				++curr_access_cnt;
			else
				--curr_access_cnt;
		}

		if (curr_access_cnt == g_clients.size())
			ready_flag = true;
		
		//cout << "PlayerLogin ------ \t isReady: " << static_cast<int>(m_isReady) << ", \t TextureID: " << static_cast<int>(m_myTextureID) << "\n";

		if (g_clients.size() >= MIN_PLAYER_NUM && ready_flag) // 2�� �̻� ���� ������ �� ���� ��ŸƮ
		{
			STC_PLAYER_LOGIN stc_player_login;
	
			for (auto& client : g_clients)
			{
				for (auto& other_client : g_clients)
				{
					stc_player_login.logindata.my_id = other_client->m_id;
					stc_player_login.logindata.texture_id = other_client->m_myTextureID;

					client->SendPacket(reinterpret_cast<Packet*>(&stc_player_login));
				}

			}
		}

	}
	break;

	case PACKET_PROTOCOL_TYPE::SCENE_STATE_CHANGE:
	{
		auto data = reinterpret_cast<STC_CHANGE_SCENE*>(packet);

		//���� �ε����̰� �ε��� ���� �Ǿ��� ���
		//�ش� ����� Ŭ���̾�Ʈ �ʱ�ȭ
		m_isLoadSceneReady = data->state.my_currSceneReady;

		auto ready_flag = false;
		int curr_access_cnt = 0;
		for (auto& client : g_clients)
		{
			if (client->GetIsLoadSceneReady())
				++curr_access_cnt;
			else
				--curr_access_cnt;
		}

		if (curr_access_cnt == g_clients.size())
			ready_flag = true;

		if (ready_flag)
		{
			for (auto& client : g_clients)
			{
				client->Init_PlayerInfo();

				client->SendStaticObjectPacket(g_staticobjs);

				client->InitData_To_Client();

				client->InitNPCData_To_Client();

				client->SetIsReadyToPlay(true);
			}
		}

		//��� Ŭ���̾�Ʈ�� �ε������� �Ѿ �����̿Ϸ���� ���, timer_thread // physics_thread ����
		if (ready_flag)
		{
			//1. �������� ������ ����
			g_physics_worker.CheckPrevTime();
			g_start_physics = true;

			//2. Ÿ�̸� ������ ����
			g_timer_queue.CheckPrevTime();
			g_timer_queue.AddEvent(0, 0, REGULAR_PACKET_EXCHANGE, true, 0);

			g_start_oneGame = true;
		}

		cout << "���ÿϷ�\n";
	}
	break;

	case PACKET_PROTOCOL_TYPE::NPC_MONSTER_ANIM:
	{
		auto data = reinterpret_cast<STC_MyNPCAnim*>(packet);

		for (auto& npc : g_npcs)
		{
			if (npc->GetID() == data->id)
			{
				npc->SetMyAnimation(data->npc_anim);
				//cout << "NPC ID: " << static_cast<int>(data->id) << "�� �ִϸ��̼��� " << static_cast<int>(npc->GetMyAnimation()) << "�Դϴ�.\n";
				break;
			}
		}
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_MOVE:
	{
		if (m_state == PLAYER_STATE::DEAD) break;
		if (!m_isReadyToPlay) break;

		m_state = PLAYER_STATE::MOVE;

		auto mdata = reinterpret_cast<STC_CharMove*>(packet);
		m_dir = mdata->dir;

	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ROTATE:
	{
		if (m_state == PLAYER_STATE::DEAD)	break;
		if (!m_isReadyToPlay) break;

		m_state = PLAYER_STATE::ROTATE;

		auto rot_data = reinterpret_cast<STC_Rotation*>(packet);

		// 1. ���� ������ �� Ŭ���̾�Ʈ�� �־��ְ�
		m_rot4f = move(rot_data->rotate_status);
		m_pdata.rot = move(rot_data->rotate_status);

		// 2. ���� ������ ���� lookvector�� rightvector�� ������Ʈ
		UpdateLookvector();
		UpdateUpvector();

		//cout << "ID: " << rot_data->id << " ��ȭ�� ȸ����: " << "[ x, y, z, w ]: "
		//	<< rot_data->rotate_status.x << ", " << rot_data->rotate_status.y << ", " << rot_data->rotate_status.z << ", " << rot_data->rotate_status.w << endl;

		// 3. �ٸ� Ŭ�󿡰� ������.
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

		if (!m_isReadyToPlay) break;
		if (!m_alive) break;

		m_state = PLAYER_STATE::ATTACK;

		auto n_bldata = reinterpret_cast<CTS_Attack*>(packet);

		//����Ű�� ������ ��, �ҷ� ����.
		//�ҷ��� ������ ĳ���� ID, ������ ����� Ÿ�� ID, �ҷ� �ʱ������ġ, �ҷ� �ʱ�ȸ����, �ҷ� �����ð�, �ҷ����̵�
		g_bullobj = new CBulletObject(n_bldata->bull_data.master_id, n_bldata->bull_data.my_id,
			n_bldata->bull_data.pos4f, n_bldata->bull_data.rot4f, n_bldata->lifetime,
			n_bldata->bull_data.vel3f, n_bldata->bull_data.type, n_bldata->bull_data.endpoint, n_bldata->bull_data.degree);

		/*
		STC_EndBullet stc_ebullet;
		float		  life_time;
		
		if (n_bldata->bull_data.type == BULLET_TYPE::protocol_LightBullet)
			life_time = MAX_LIGHT_BULLET_TIME;
		else if (n_bldata->bull_data.type == BULLET_TYPE::protocol_HeavyBullet)
			life_time = MAX_HEAVY_BULLET_TIME;
		
		Position end_pos4f;
		end_pos4f.x =  n_bldata->bull_data.pos4f.x + n_bldata->bull_data.vel3f.x * life_time;
		end_pos4f.y = n_bldata->bull_data.pos4f.y + n_bldata->bull_data.vel3f.y * life_time;
		end_pos4f.z = n_bldata->bull_data.pos4f.z + n_bldata->bull_data.vel3f.z * life_time;
		end_pos4f.w = n_bldata->bull_data.pos4f.w;

		stc_ebullet.end_bull_data.ori_pos4f = n_bldata->bull_data.pos4f;
		stc_ebullet.end_bull_data.end_pos4f = end_pos4f;

		stc_ebullet.end_bull_data.master_id = n_bldata->bull_data.master_id;
		stc_ebullet.end_bull_data.my_id = n_bldata->bull_data.my_id;
		stc_ebullet.end_bull_data.type = n_bldata->bull_data.type;
		stc_ebullet.end_bull_data.endpoint = n_bldata->bull_data.endpoint;
		stc_ebullet.end_bull_data.degree = n_bldata->bull_data.degree;
		stc_ebullet.end_bull_data.lookvector = { 0.f, 0.f, 0.f };

		for (auto& client : g_clients)
		{
			if (!client->GetConnectState()) continue;

			client->SendPacket(reinterpret_cast<Packet*>(&stc_ebullet));
		}
		*/

		g_bullets.emplace_back(g_bullobj);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ANIMATION:
	{
		if (m_state == DEAD) break;

		if (!m_isReadyToPlay) break;

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

		STC_CharAnimation stc_cani;
		stc_cani.ani_state = ani->ani_state;
		stc_cani.id = ani->id;

		for (auto client : g_clients)
		{
			if (client->m_id == ani->id) continue;
			if (client->GetIsAI() == true || client->GetConnectState() == false) continue;

			client->SendPacket(reinterpret_cast<Packet*>(&stc_cani));
			//cout << "Client Number " << client->m_id << "Animation : " << static_cast<int>(ani->ani_state) << "\n";
		} 
	}
	break;


	//Timerthread�� ���� ���������� �������� ������Ʈ�ؼ� Ŭ���̾�Ʈ�� ������ߵ� �����͸� ó���ϴ� �κ�
	//1. Move Object(����������) �� Rigidbody Object (���������� ÷���� ��ü��)
	//2. Player (�÷��̾�) �� NPC (����)
	//3. Bullet (�÷��̾��� ���Ÿ� ���� �� ��ų)
	case PACKET_PROTOCOL_TYPE::PLAYER_REGULAR_DATA_BY_TIMER:
	{
		if (m_state == DEAD)  break;
		if (!m_isReadyToPlay) break;

		//timer thread���� �ش� �۾����� ������ ���� post�� �ٽ� �̸� ȣ��
		//[1]  MoveObject & RigidbodyObject	
		//------------------------------------��ġ�� ������Ʈ ����------------------------------------//

		auto prevtime = high_resolution_clock::now();
		
		//[1] - 1. Move Object (����������)  
		STC_MoveObject_NoCreate stc_move_object_no_create;
		//[1] - 2. Rigidbody Object (���������� ÷���� ��ü��)
		STC_RigidbodyObject stc_rigidbody_object;

		for (const auto& client : g_clients)
		{
			if (!client->GetConnectState()) continue;

			for (const auto& mvobj : g_moveobjs)
			{
				stc_move_object_no_create.mvobj_data.id = mvobj->GetMoveObjectData().id;
				stc_move_object_no_create.mvobj_data.pos4f = mvobj->GetMoveObjectData().pos4f;
				stc_move_object_no_create.mvobj_data.rot4f = mvobj->GetMoveObjectData().rot4f;

				client->SendPacket(reinterpret_cast<Packet*>(&stc_move_object_no_create));
			}

			for (const auto& rigid : g_rigidobjs)
			{
				stc_rigidbody_object.rbobj_data = move(rigid->GetRigidbodyData());
				client->SendPacket(reinterpret_cast<Packet*>(&stc_rigidbody_object));
			}
		}
		//auto currtime = high_resolution_clock::now();
		//auto deltime = duration_cast<microseconds>(currtime - prevtime).count();
		//cout << "Time: " << deltime / 1000000.0 << "\n\n";
		
		//[1] - 1. Move Object (����������)  
		
		/*
		auto prevtime = high_resolution_clock::now();
		for (const auto& mvobj : g_moveobjs)
		{
			STC_MoveObject_NoCreate stc_move_object_no_create;
			stc_move_object_no_create.mvobj_data.id = mvobj->GetMoveObjectData().id;
			stc_move_object_no_create.mvobj_data.pos4f = mvobj->GetMoveObjectData().pos4f;
			stc_move_object_no_create.mvobj_data.rot4f = mvobj->GetMoveObjectData().rot4f;

			for (const auto& client : g_clients)
			{
				//Ŭ���̾�Ʈ�� ������ �������� ������ ������ �ʴ´� (���� ������Ʈ)
				if (!client->GetConnectState()) continue;

				client->SendPacket(reinterpret_cast<Packet*>(&stc_move_object_no_create));
			}

		}
		

		//[1] - 2. Rigidbody Object (���������� ÷���� ��ü��)
		STC_RigidbodyObject stc_rigidbody_object;
		for (const auto& client : g_clients)
		{
			//Ŭ���̾�Ʈ�� ������ �������� ������ ������ �ʴ´� (����� �ٵ� ������Ʈ)
			if (!client->GetConnectState()) continue;

			for (const auto& rigid : g_rigidobjs)
			{
				stc_rigidbody_object.rbobj_data = move(rigid->GetRigidbodyData());
				client->SendPacket(reinterpret_cast<Packet*>(&stc_rigidbody_object));

				//cout << "RigidybodyObject ID: " << rigid->m_stc_robjdata.id << "PosX: " << rigid->m_stc_robjdata.pos4f.x << "PosY: "
				//	<< rigid->m_stc_robjdata.pos4f.y << "PosZ: " << rigid->m_stc_robjdata.pos4f.z << "PosW: " << rigid->m_stc_robjdata.pos4f.w << "\n";
			}
		}
		auto currtime = high_resolution_clock::now();
		auto deltime = duration_cast<microseconds>(currtime - prevtime).count();
		//cout << "Time: " << deltime / 1000000.0 << "\n\n";
		*/

		/*
		stack_double_time += deltime;
		check_cnt++;

		if (check_cnt % 500 == 0 && check_cnt != 0)
		{
			cout << "StackTime: " << static_cast<double>(stack_double_time / check_cnt) << "Count: " << check_cnt << "\n";
			check_cnt = 0;
			stack_double_time = 0.0;
		}
		*/	

		//[2]  All Clients �� ���� Rank �ø��������� ��Ʈ
		//--------------------------------�÷��̾� ��ŷ ����------------------------------------------//

		//* All Clients �� ���� Rank �ø��������� ��Ʈ
		
		int local_rank = 0;
		vector<CPlayerObject*> temp_clients;
		temp_clients.reserve(g_clients.size());
		copy(g_clients.begin(), g_clients.end(), back_inserter(temp_clients));

		//������ ����� temp_clients�� ������������ ��Ʈ�ؼ� ù �������� 1�� ~ �� �ű��
		sort(temp_clients.begin(), temp_clients.end(), [&](CPlayerObject* c1, CPlayerObject* c2) {return c1->GetMyScoreForRanking() > c2->GetMyScoreForRanking(); });
		for (auto& temp_client : temp_clients)
			temp_client->SetMyCharacterCurrRank(++local_rank);


		int  topRankerID = -1;
		bool once_flag = false;
		for (auto& client : g_clients)
		{
			client->SetIsTopRanker(false);
			client->m_pdata.topRank = false;

			for (auto& temp_client : temp_clients)
			{
				if (client->GetID() == temp_client->GetID())
				{
					client->SetMyCharacterCurrRank(temp_client->GetMyCharacterCurrRank());

					//�� Ŭ���̾�Ʈ�� ����� 1���̸� toprank�� true��, �� ���� ���� ����� �ְ� 1��, ������ 30���̻��� ��
					if (client->GetMyCharacterCurrRank() == 1 && client->GetPlayerScore() >= 30 && once_flag == false)
					{
						once_flag = true;
						//���⿡�� 1� ����, �װ͵� ù��° 1��
						CPlayerObject::m_topRankerID = client->GetID();

						client->SetIsTopRanker(true);
						client->m_pdata.topRank = true;

						if (!client->GetIsBuffState())
						{
							client->SetIsBuffState(true);
							client->GetPhysicsPoint()->SetHalfBox(4, 13, 4);			//�ʱ��ѹ�����, �� �ڷ� �ȹٲ�
							client->SetMyOriginalHP(700);								//�ʱ��ѹ�����, �� �ڷ� �ȹٲ�

							client->SetMyCurrSpeed(client->GetMyCurrSpeed() + 50);		//�ʱ��ѹ�����, �� �ڷ� �ȹٲ�
							client->SetMyCurrHP(client->GetMyCurrHP() + 600);			//�ʱ��ѹ�����, �� �ڷ� �ȹٲ�

						}

					}

					client->m_pdata.rank = client->GetMyCharacterCurrRank();
					break;
				}
			}
		}

		//[3] All Clients (Player)  // ĳ���Ϳ��� �������� �ð����� �� ������ ������ ����
		//------------------------------------�÷��̾� ����-------------------------------------------//
		//STC_STAGE_TIMER   stc_stage_timer;

		STC_CharCurrState stc_char_state;
		for (const auto& my_client : g_clients)
		{
			//Ŭ���̾�Ʈ�� ������ �������� ������ ������ �ʴ´�(MOVE_OBJECT, CLIENT)
			if (!my_client->GetConnectState()) continue;

			//������ ž��Ŀ���� ������ �ƴҰ�� ����ȿ���� false�� �ٲ��ش�
			if (CPlayerObject::m_topRankerID != my_client->GetID() && my_client->GetIsBuffState() == true)
				my_client->SetIsBuffState(false);

			//----------------------------------Character Respawn Data---------------------------------------//

			if (my_client->GetAlive() == false && my_client->m_respawn_state == false)
			{
				my_client->m_respawn_state = true;
				my_client->SetMyAnimation(Ani_State::Dead);

				//server to client packet data
				my_client->m_pdata.ani = Ani_State::Dead;
				my_client->m_pdata.respawn_cnt = 1;				//respawn_cnt �� 1�� ���� ����

				g_timer_queue.AddEvent(my_client->GetID(), Player_RespawnTime, PLAYER_RESPAWN, false, my_client->GetID());
			}

			//--------------------My Client Data to Other Client (Including My Client data)------------------//

			for (const auto& other_client : g_clients)
			{
				stc_char_state.player_data = move(my_client->m_pdata);
				other_client->SendPacket(reinterpret_cast<Packet*>(&stc_char_state));
			}

			//---------------------------------------------------------------Move Object--------------------//

			if (!my_client->GetFirstMoveObjects())
			{
				for (const auto& mvobj : g_moveobjs)
				{
					STC_MoveObject stc_move_object;
					stc_move_object.mvobj_data = move(mvobj->GetMoveObjectData());
					my_client->SendPacket(reinterpret_cast<Packet*>(&stc_move_object));
				}
			}
			my_client->SetFirstMoveObjects(true);

			//-------------------------------------------------------------------Stage Timer----------------//

			//stc_stage_timer.data.stage_time = m_deltime;
			//my_client->SendPacket(reinterpret_cast<Packet*>(&stc_stage_timer));

			if (my_client->m_pdata.respawn_cnt == 1)
				my_client->m_pdata.respawn_cnt = 0;

		}


		//[4] All NPCs (Non-Player)
		//------------------------------------NPC     ����-------------------------------------------//
		//4. All NPCs (Non-Player)
		STC_NpcMonsterCurrState stc_mnpcs_state;
		for (const auto& npc_monster : g_npcs)
		{
			for (const auto& client : g_clients)
			{
				//Ŭ���̾�Ʈ�� ������ �������� ������ ������ �ʴ´�(NPC)
				if (!client->GetConnectState()) continue;

				stc_mnpcs_state.npc_data = npc_monster->GetMyBasicPacketData();
				client->SendPacket(reinterpret_cast<Packet*>(&stc_mnpcs_state));
			}
		}


		//�����ϸ� �ʿ������ 
		//[5] All Bullets (LightBullet // HeavyBullet // DiceBullet)
		//--------------------------------�ҷ� ������Ʈ(����) ����-------------------------------------//
		for (auto bullet : g_bullets)
		{
			//cout << "Bullet ID: " << bullet->GetBulletID() << "Bullet MID: " << bullet->GetBulletMasterID() <<
			//	"Position: " << bullet->m_bulldata.pos4f.x << ", " << bullet->m_bulldata.pos4f.y << ", " << bullet->m_bulldata.pos4f.z <<
			//	"LifeTime: " << bullet->GetBulletLifeTime() <<" " << "IsAlive:" << static_cast<int>(bullet->GetBulletIsAlive()) << endl;

			if (bullet->GetAlive())
			{

				//1. NPC Bullet
				if (bullet->GetObjectType() == protocol_NpcStoneBullet)
				{
					STC_NpcMonsterAttackStoneBullet stc_imp_bullet;
					stc_imp_bullet.npc_bulldata = bullet->GetChangedNPCBulletState();

					auto stone_rnpos = dynamic_cast<CStoneBulletObject*>(bullet)->GetOrgPlusPos();
					stc_imp_bullet.stone_rnpos = { stone_rnpos.x, stone_rnpos.y, stone_rnpos.z, stone_rnpos.w };

					/*
					cout << "NPC Bullet ID: " << stc_imp_bullet.npc_bulldata.my_id << ", " << "PosXYZW: " << stc_imp_bullet.npc_bulldata.pos4f.x << ", "
					<< stc_imp_bullet.npc_bulldata.pos4f.y << ", " << stc_imp_bullet.npc_bulldata.pos4f.z << ", " << stc_imp_bullet.npc_bulldata.pos4f.w << "\n";
					*/

					for (const auto& client : g_clients)
					{
						if (!client->GetConnectState()) continue;

						client->SendPacket(reinterpret_cast<Packet*>(&stc_imp_bullet));
					}
				}
				else
				{
					//2. Player Bullet
					if (bullet->GetObjectType() == protocol_HammerBullet)
					{
						STC_SKILL_HAMMERBULLET stc_skill_hammer_bullet;
						stc_skill_hammer_bullet.skill_data = bullet->GetChangedHammerBulletState();

						for (const auto& client : g_clients)
						{
							if (!client->GetConnectState()) continue;

							client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_hammer_bullet));
						}
					}
					else if (bullet->m_bulldata.type == protocol_DiceBullet && bullet->GetIsCreateFirst())
					{
						STC_SKILL_DICESTRIKE stc_skill_dicestrike;
						stc_skill_dicestrike.bull_data = bullet->GetChangedBulletState();
						stc_skill_dicestrike.is_first = bullet->GetIsCreateFirst();
						stc_skill_dicestrike.lookvector = bullet->GetDicestrikeOffLookvector();

						for (const auto& client : g_clients)
						{
							if (!client->GetConnectState()) continue;

							client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_dicestrike));
						}
					}
					else
					{
						STC_Attack stc_attack;
						stc_attack.bull_data = bullet->GetChangedBulletState();
						stc_attack.is_first = bullet->GetIsCreateFirst();

						for (const auto& client : g_clients)
						{
							if (!client->GetConnectState()) continue;

							client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
						}

					}
				}

				if (bullet->GetIsCreateFirst())
					bullet->SetIsCreateFirst(false);

			}
		}

		auto currtime = high_resolution_clock::now();
		__int64 deltime = duration_cast<microseconds>(currtime - prevtime).count();
		auto ret = deltime / 1000000.0;
		//cout << "Time: " << ret << "\n\n";
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_SKILL_SHIELD:
	{
		if (m_state == DEAD)  break;
		if (!m_isReadyToPlay) break;

		auto data = reinterpret_cast<CTS_SKILL_SHIELD*>(packet);

		STC_SKILL_SHIELD stc_skill_shield;
		stc_skill_shield.skill_data.master_id = data->skill_data.master_id;
		stc_skill_shield.skill_data.my_id = data->skill_data.my_id;
		stc_skill_shield.skill_data.alive = true;

		m_skill_shield.data.curr_cooltime = high_resolution_clock::now();
		if (m_skill_shield.data.first_op)
			m_skill_shield.data.prev_cooltime = m_skill_shield.data.curr_cooltime;
		
		__int64 dur = duration_cast<microseconds>(m_skill_shield.data.curr_cooltime - m_skill_shield.data.prev_cooltime).count();
		m_skill_shield.data.prev_cooltime = m_skill_shield.data.curr_cooltime;
		double cool_time = dur / 1000000.0;

		stc_skill_shield.cooltime = cool_time;

		if (m_skill_shield.data.first_op == false && cool_time < SKILL_SHIELD_COOLTIME == true)
		{
			//Disconnect Client
		}

		cout << "Shield cool time: " << cool_time << "\n";

		if (cool_time >= SKILL_SHIELD_COOLTIME | m_skill_shield.data.first_op)
		{
			m_skill_shield.data.first_op = false;
		
			if (m_skill_shield.data.once_flag)
			{
				m_skill_shield.data.once_flag = false;
				m_skill_shield.data.op_time = 0.0;
				g_timer_queue.AddEvent(data->skill_data.my_id, 0.0, SKILL_SHIELD, true, data->skill_data.master_id);

				for (auto client : g_clients)
				{
					if (client->m_id == data->skill_data.master_id) continue;
					if (client->GetIsAI() == true || client->GetConnectState() == false) continue;

					client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_shield));
				}
			}
		}
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_SKILL_WAVESHOCK:
	{
		if (m_state == DEAD) break;

		if (!m_isReadyToPlay) break;

		auto data = reinterpret_cast<CTS_SKILL_WAVESHOCK*>(packet);

		STC_SKILL_WAVESHOCK stc_skill_waveshock;
		stc_skill_waveshock.skill_data = data->skill_data;

		m_skill_waveshock.data.curr_cooltime = high_resolution_clock::now();
		if (m_skill_waveshock.data.first_op)
			m_skill_waveshock.data.prev_cooltime = m_skill_waveshock.data.curr_cooltime;

		__int64 dur = duration_cast<microseconds>(m_skill_waveshock.data.curr_cooltime - m_skill_waveshock.data.prev_cooltime).count();
		m_skill_waveshock.data.prev_cooltime = m_skill_waveshock.data.curr_cooltime;
		double cool_time = dur / 1000000.0;
		
		if (cool_time >= SKILL_WAVESHOCK_COOLTIME | m_skill_waveshock.data.first_op)
		{
			m_skill_waveshock.data.first_op = false;
			m_skill_waveshock.operated = true;
			//stc_skill_waveshock.cooltime = 0.0;
			if (m_skill_waveshock.data.once_flag)
			{
				m_skill_waveshock.data.once_flag = false;
				m_skill_waveshock.data.op_time = 0.0;
				g_timer_queue.AddEvent(data->skill_data.my_id, 0.0, SKILL_WAVESHOCK, true, data->skill_data.master_id);

				for (auto client : g_clients)
				{
					if (client->m_id == data->skill_data.master_id) continue;
					if (client->GetIsAI() == true || client->GetConnectState() == false) continue;
				
					client->SendPacket(reinterpret_cast<Packet*>(&stc_skill_waveshock));
				}
			}
		}

	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_SKILL_DICESTRIKE:
	{
		if (m_state == PLAYER_STATE::DEAD)
			break;

		if (!m_isReadyToPlay) break;

		m_state = PLAYER_STATE::ATTACK;

		auto data = reinterpret_cast<CTS_SKILL_DICESTRIKE*>(packet);

		//----------------------------------------------------------------��ų ��Ÿ��-------------------------------------------------------//
		if (data->is_firstdice)
		{
			m_skill_dicestrike.data.curr_cooltime = high_resolution_clock::now();
			if (m_skill_dicestrike.data.first_op)
			{
				m_skill_dicestrike.data.prev_cooltime = m_skill_dicestrike.data.curr_cooltime;
			}

			__int64 dur = duration_cast<microseconds>(m_skill_dicestrike.data.curr_cooltime - m_skill_dicestrike.data.prev_cooltime).count();
			m_skill_dicestrike.data.prev_cooltime = m_skill_dicestrike.data.curr_cooltime;
			double cool_time = dur / 1000000.0;


			if (cool_time < SKILL_DICESTRIKE_COOLTIME & !m_skill_dicestrike.data.first_op)
			{
				cout << "Dicestrike skill cooltime Error \n";
				m_skill_dicestrike.cooltime_error = true;
				break;
			}

			if (cool_time >= SKILL_DICESTRIKE_COOLTIME & m_skill_dicestrike.cooltime_error)
			{
				cout << "Dicestrike skill cooltime Recovery \n";
				m_skill_dicestrike.cooltime_error = false;
			}

			if (m_skill_dicestrike.data.first_op)
				m_skill_dicestrike.data.first_op = false;

		}

		//----------------------------------------------------------------��ų ��Ÿ��-------------------------------------------------------//

		/*
		STC_EndBullet stc_ebullet;
		float		  life_time = MAX_DICE_BULLET_TIME;

		Position end_pos4f;
		end_pos4f.x = data->bull_data.pos4f.x + data->bull_data.vel3f.x * life_time;
		end_pos4f.y = data->bull_data.pos4f.y + data->bull_data.vel3f.y * life_time;
		end_pos4f.z = data->bull_data.pos4f.z + data->bull_data.vel3f.z * life_time;
		end_pos4f.w = data->bull_data.pos4f.w;

		stc_ebullet.end_bull_data.ori_pos4f = data->bull_data.pos4f;
		stc_ebullet.end_bull_data.end_pos4f = end_pos4f;

		stc_ebullet.end_bull_data.master_id = data->bull_data.master_id;
		stc_ebullet.end_bull_data.my_id = data->bull_data.my_id;
		stc_ebullet.end_bull_data.type = data->bull_data.type;
		stc_ebullet.end_bull_data.endpoint = data->bull_data.endpoint;
		stc_ebullet.end_bull_data.degree = data->bull_data.degree;
		stc_ebullet.end_bull_data.lookvector = {data->lookvector.x, data->lookvector.y, data->lookvector.z};


		for (auto& client : g_clients)
		{
			if (!client->GetConnectState()) continue;

			client->SendPacket(reinterpret_cast<Packet*>(&stc_ebullet));
		}
		*/

		//����Ű�� ������ ��, �ҷ� ����.
		//�ҷ��� ������ ĳ���� ID, ������ ����� Ÿ�� ID, �ҷ� �ʱ������ġ, �ҷ� �ʱ�ȸ����, �ҷ� �����ð�, �ҷ����̵�
		if (!m_skill_dicestrike.cooltime_error)
		{
			g_bullobj = new CBulletObject(data->bull_data.master_id, data->bull_data.my_id,
				data->bull_data.pos4f, data->bull_data.rot4f, 0.f,
				data->bull_data.vel3f, data->bull_data.type, data->bull_data.endpoint, data->bull_data.degree);

			g_bullobj->SetDicestrikeOffLookvector(data->lookvector);

			g_bullets.emplace_back(g_bullobj);
		}

	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_SKILL_HAMMER:
	{
		if (m_state == PLAYER_STATE::DEAD)
			break;

		if (!m_isReadyToPlay) break;

		m_state = PLAYER_STATE::ATTACK;

		auto data = reinterpret_cast<CTS_SKILL_HAMMERBULLET*>(packet);

		//----------------------------------------------------------------��ų ��Ÿ��-------------------------------------------------------//



	
		//----------------------------------------------------------------��ų ��Ÿ��-------------------------------------------------------//

		XMFLOAT4 inOpp = { data->skill_data.opp_pos4f.x, data->skill_data.opp_pos4f.y, data->skill_data.opp_pos4f.z, data->skill_data.opp_pos4f.w };
		g_bullobj = new CHammerBulletObject(data->skill_data.master_id, data->skill_data.my_id, data->skill_data.pos4f,
									        data->skill_data.rot4f, data->skill_data.weapon_num, inOpp, true);

		g_bullets.emplace_back(g_bullobj);
	}
	break;


	}
}

void CPlayerObject::CalculatePlayerScoreForRanking()
{
	//�÷��̾��� ��ŷ�� �ű������ ���ھ ����Ѵ�
	//1. ���ھ� 2. ų �� 3. ���� �� �� �ջ��� ����� �������� ��ŷüũ
	m_score_for_rankcheck = static_cast<float>(m_player_score) + static_cast<float>(m_killCount) * 3.8f - static_cast<float>(m_deathCount) * 1.8f;

}

void CPlayerObject::UpdateDataForPacket()
{
	m_pdata.ai = m_ai;
	m_pdata.airbone = m_airbone;
	//m_pdata.ani = m_ani;		//�ִϸ��̼��� ��Ŷ���ڸ��� ó��
	m_pdata.connect = m_connect;
	//m_pdata.dir = m_dir;
	m_pdata.godmode = m_godmode;
	m_pdata.id = m_id;
	m_pdata.pos = m_pos4f;
	//m_pdata.rot = m_rot4f;	//ȸ���� ��Ŷ���ڸ��� ó��

	m_pdata.status.attack = m_ability.attack;
	m_pdata.status.cur_hp = m_ability.curHP;
	m_pdata.status.exp = m_ability.exp;
	m_pdata.status.level = m_ability.level;
	m_pdata.status.origin_hp = m_ability.orignHP;
	m_pdata.status.speed = m_ability.speed;

	*pp->CenterPos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	//pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);

	m_pdata.killcount = m_killCount;
	m_pdata.deathcount = m_deathCount;
	m_pdata.score = m_player_score;
	m_pdata.rank = m_myCurrRank;
	m_pdata.alive = m_alive;
}

void CPlayerObject::Tick(double deltime)
{
	if (!m_alive) return;

	*pp->CenterPos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->integrate(deltime);
	
	UpdatePPosCenterPos();
	
}

void CPlayerObject::Tick(double deltime, Position& pos4f)
{
	if (!m_alive) return;

	*pp->CenterPos = { pos4f.x, pos4f.y, pos4f.z, pos4f.w };
	pp->integrate(deltime);

	UpdatePPosCenterPos();
	
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

			if (test)//�浹������ �浹�ؼҸ� �ؾ��Ѵ�.
			{
				//�浹 ������ ���� (0,1,0) �̸� Airbone�� false�� �д�. �̴� ���� ���������� ��Ÿ����.
				if (pp->pAxis.y > 0)
				{
					m_airbone = false;
				}

				//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
				//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
				if (pp->pAxis.y < 0)
				{
					(*iter)->pp->SetVelocity((*iter)->pp->GetVelocity().x, 0, (*iter)->pp->GetVelocity().z);
					(*iter)->m_airbone = false;
				}

				XMFLOAT3 cn;
				cn = XMFloat4to3(Float4Add(pp->GetPosition(), (*(*iter)->pp).GetPosition(), false));
				cn = Float3Normalize(cn);

				//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
				pp->CollisionResolve(*(*iter)->pp, cn, deltime);
				UpdatePPosCenterPos();
				(*iter)->UpdatePPosCenterPos();

				//cout << "����� �浹" << endl;

			}
		}
	}
	
}

void CPlayerObject::Collision(unordered_set<CStaticObject*>* sobjs, double deltime)
{
	//������ ��� - �浹 ����� ����ƽ������Ʈ	
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

			//cout << "����ƽ ������Ʈ�� �浹 " << endl;

		}
	}
	
}


void CPlayerObject::Collision_With_Waveshock()
{
	if (m_skill_waveshock.operated)
	{
		XMFLOAT4 wavepos = this->GetCenterPos4f();
		wavepos.y += -this->pp->GetHalfBox().y;
		float impurse = 20000;
		float rad = 150.f; //����

						   //���̳��� ������Ʈ�� �����������Ʈ�� ����������.
		for (auto client : g_clients)
		{
			if (client != this)
			{
				auto enemy = Float3Add(XMFloat4to3(client->GetCenterPos4f()), XMFloat4to3(wavepos), false);

				if (FloatLength(enemy) <= rad) //���� �ȿ�����������
				{
					enemy = Float3Normalize(enemy);
					enemy = Float3Float(enemy, impurse * (1 - FloatLength(enemy) / rad));
					client->pp->AddForce(enemy);
					client->pp->integrate(0.01);
				}
			}
		}

		for (auto rigid : g_rigidobjs)
		{
			auto enemy = Float3Add(XMFloat4to3(rigid->GetCenterPos4f()), XMFloat4to3(wavepos), false);

			if (FloatLength(enemy) <= rad)
			{
				enemy = Float3Normalize(enemy);
				enemy = Float3Float(enemy, impurse *(1 - FloatLength(enemy) / rad));
				rigid->GetRigidBody()->AddForce(enemy);
				rigid->GetRigidBody()->integrate(0.01);
			}
		}

		m_skill_waveshock.operated = false;
	}
}

void CPlayerObject::GetDamaged(int damage)
{
	if (m_godmode) return;

	if (m_skill_shield.data.on_using)			//�ǵ� ��ų ��� ���� �� ������ 1�� ��
	{
		m_ability.curHP -= 1;
	}
		
	if (m_ability.curHP > 0 && m_skill_shield.data.on_using == false)
	{
		//ĳ���� hp�� ����
		m_ability.curHP -= damage;
	}

	if (m_ability.curHP <= 0)
	{
		//ĳ���� m_alive�� false �� �ٲ��ش�
		m_ability.curHP = 0;
		m_alive = false;
	}

}

void CPlayerObject::PlayerInput(double deltime)
{
	if (!m_alive) return;

	switch (m_dir)
	{
		case CHAR_MOVE::FRONT_MOVE:
		{
			auto l = XMLoadFloat3(&m_Lookvector);
			l *= m_ability.speed * deltime;

			xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
			auto p = XMLoadFloat4(&xmf4_pos);
			p += l;
			XMStoreFloat4(&xmf4_pos, p);
			*pp->CenterPos = xmf4_pos;

			UpdatePPosCenterPos();

			//cout << "Front Move Pos[x, y, z, w]: " << m_pos4f.x << " , " << m_pos4f.y << " , " << m_pos4f.z << " , " << m_pos4f.w << endl;
			//cout << "Front Move!!" << endl;
		}
		break;

		case CHAR_MOVE::BACK_MOVE:
		{
			auto l = XMLoadFloat3(&m_Lookvector);
			l *= m_ability.speed * deltime;

			xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
			auto p = XMLoadFloat4(&xmf4_pos);
			p -= l;
			XMStoreFloat4(&xmf4_pos, p);
			*pp->CenterPos = xmf4_pos;
			
			UpdatePPosCenterPos();

			//cout << "Back Move Pos[x, y, z, w]: " << m_pos4f.x << " , " << m_pos4f.y << " , " << m_pos4f.z << " , " << m_pos4f.w << endl;
			//cout << "Back Move!!" << endl;
		}
		break;

		case CHAR_MOVE::LEFT_MOVE:
		{			
			auto r = XMLoadFloat3(&m_Rightvector);
			r *= m_ability.speed * deltime;

			xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
			auto p = XMLoadFloat4(&xmf4_pos);
			p -= r;
			XMStoreFloat4(&xmf4_pos, p);
			*pp->CenterPos = xmf4_pos;
			
			UpdatePPosCenterPos();
			
			//cout << "Left Move Pos[x, y, z, w]: " << m_pos4f.x << " , " << m_pos4f.y << " , " << m_pos4f.z << " , " << m_pos4f.w << endl;
			//cout << "Left Move!!" << endl;
		}
		break;

		case CHAR_MOVE::RIGHT_MOVE:
		{
			auto r = XMLoadFloat3(&m_Rightvector);
			r *= m_ability.speed * deltime;

			xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
			auto p = XMLoadFloat4(&xmf4_pos);
			p += r;
			XMStoreFloat4(&xmf4_pos, p);
			
			*pp->CenterPos = xmf4_pos;
			UpdatePPosCenterPos();
	
			//cout << "Right Move Pos[x, y, z, w]: " << m_pos4f.x << " , " << m_pos4f.y << " , " << m_pos4f.z << " , " << m_pos4f.w << endl;
			//cout << "Right Move!!" << endl;
		}
		break;

		case CHAR_MOVE::JUMP:
		{		
			if (!m_airbone)
			{
				GeneratorJump j;
				j.SetJumpVel(XMFLOAT3(0, 80, 0));
				j.Update(deltime, *pp);
				m_airbone = true;

				//cout << "Jump!!" << endl;
			}
		}
		break;

	default:
		break;
	}

}

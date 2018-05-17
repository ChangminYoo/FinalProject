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
	CTextTest* m_pdata = new CTextTest();

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

	m_pdata.ai = true;
	m_pdata.id = m_id;
	m_pdata.dir = 0;


	//������ ���� ��ġ���� ���Ƿ� ���������Ѵ�.
	//���� ���� ���� ���� - lua����ؾߵ�
	m_pdata.pos = { static_cast<float>(rand_x), 0.0f, static_cast<float>(rand_z) , 0.0f};

	m_pdata.status.origin_hp = 100;
	m_pdata.status.cur_hp = m_pdata.status.origin_hp;
	
	m_pdata.status.exp = 200;
	m_pdata.status.level = 1;
	m_pdata.status.attack = 20;
	m_pdata.status.defend = 10;
	m_pdata.status.speed = 20;
	//

	m_state = PLAYER_STATE::IDLE;
}

void Player_Session::Init_PlayerInfo()
{
	//�÷��̾� ���� �ʱ�ȭ 
	//default_random_engine generator(time(0));
	//uniform_int_distribution<int> export_x(-RAND_CREATE_X_POS, RAND_CREATE_X_POS);
	//uniform_int_distribution<int> export_z(-RAND_CREATE_Z_POS, RAND_CREATE_Z_POS);
	//auto rand_x = export_x(generator);
	//auto rand_z = export_z(generator);
	m_state = IDLE;

	m_pdata.ani = Ani_State::Idle;
	m_pdata.connect = true;
	m_pdata.dir = 0;
	m_pdata.airbone = false;
	m_pdata.id = m_id;
	m_pdata.ai = false;
	m_pdata.rot = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pdata.godmode = false;

	m_pdata.status.attack = 50;
	m_pdata.status.defend = 50;
	m_pdata.status.speed = 50;
	m_pdata.status.cur_hp = 100;
	m_pdata.status.origin_hp = 100;
	m_pdata.status.exp = 0;
	m_pdata.status.level = 1;
	
	if (m_pdata.id == 0)
		m_pdata.pos = { -100.0f , -1000.0f, 0.0f };
	else if (m_pdata.id == 1)
		m_pdata.pos = { 300.0f, -1000.0f, 0.0f };
	//

	//Lookvector, Rightvector, Upvector ���� - PhysicalEffect Ŭ���� �Լ��̿�
	pe = new PhysicalEffect();

	OffLookvector = XMFLOAT3(0, 0, -1);
	OffRightvector = XMFLOAT3(-1, 0, 0);	

	XMFLOAT4 xmf4_rot{ m_pdata.rot.x, m_pdata.rot.y, m_pdata.rot.z, m_pdata.rot.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, MMPE_PI);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_pdata.rot.x = xmf4_rot.x; m_pdata.rot.y = xmf4_rot.y; m_pdata.rot.z = xmf4_rot.z; m_pdata.rot.w = xmf4_rot.w;
	
	pe->UpdateLookVector(OffLookvector, OffRightvector, m_pdata.rot, Lookvector, Rightvector);
	pe->GetUpVector(Lookvector, Rightvector, Upvector);
	//

	//����ȿ�� �� �浹ó���� ���� PhysicsPoint Ŭ����
	pp = new PhysicsPoint();
	pp->SetPosition(m_pdata.pos.x, m_pdata.pos.y, m_pdata.pos.z);
	pp->SetHalfBox(3, 10, 3);
	pp->SetDamping(0.7);
	pp->SetBounce(false);
	//

	//2. ����ȿ�� ����
	pe->GravitySystem(0.0f, pp);

	XMFLOAT4 xmf4_pos{ m_pdata.pos.x, m_pdata.pos.y, m_pdata.pos.z, m_pdata.pos.w };
	pp->integrate(0.0f, &xmf4_pos);
	m_pdata.pos.x = xmf4_pos.x;  m_pdata.pos.y = xmf4_pos.y;  m_pdata.pos.z = xmf4_pos.z;   m_pdata.pos.w = xmf4_pos.w;
	
	pe->AfterGravitySystem(0.0f, pp, OBJECT_TYPE::PLAYER, m_pdata.pos, m_pdata.airbone);

	//wcscpy(m_pdata.LoginData.name, m_loginID);
	//wcscpy(m_pdata.LoginData.password, m_loginPW);
}

void Player_Session::InitData_To_Client()
{
	//1. �� �ʱ�ȭ����
	STC_SetMyClient init_player;
	init_player.player_data = move(m_pdata);

	// �� �ʱ�ȭ ������ �ϴ� ���� ����� Ŭ���̾�Ʈ�� ������.
	m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&init_player));

	cout << "Current ID: " << m_pdata.id << "Pos(x,y,z): " << m_pdata.pos.x << " " <<
		m_pdata.pos.y << " " << m_pdata.pos.z << " " <<  m_pdata.pos.w << endl;
	//------------------------------------------------------------------------------------
	
	//2. �ٸ� Ŭ���̾�Ʈ �ʱ�ȭ����
	STC_SetOtherClient init_otherplayer;

	//2. �� ������ �ٸ� Ŭ���̾�Ʈ���� �Ѱ��ش�
	for (auto i = 0; i < m_clients.size(); ++i)
	{
		if (m_clients[i]->m_pdata.id == m_id) continue;
		if (m_clients[i]->m_pdata.connect != false || m_clients[i]->m_pdata.ai != true)
		{
			// 1. �ٸ� Ŭ���̾�Ʈ ������ �� Ŭ���̾�Ʈ���� ������
			init_otherplayer.player_data = move(m_clients[i]->GetPlayerData());
			m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&init_otherplayer));

			ZeroMemory(&init_otherplayer, sizeof(init_otherplayer));

			// 2. �� ������ �ٸ� Ŭ���̾�Ʈ���� ������
			init_otherplayer.player_data = move(init_player.player_data);
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

	//1. async_write_some - �񵿱� IO / ���������͸� ��� ����(���� ���ۿ� ����X)
	//2. async_write - �񵿱� IO / �������� �ϴ� �����Ͱ� ��� ���ۿ� ���� �����͸� ����
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

	//��Ŷ���� �� ���ۻ������ ���޵� ������ ũ�⸸ŭ�� �Ҵ��ؾ��Ѵ�
	//1����Ʈ�� �޾� ��Ŷ����� �˰� �ش� ��Ŷ ����� �̿��� �ٽ� ��Ŷ�� �޴´�
	//boost::asio::async_read(m_socket, boost::asio::buffer(m_recvBuf , 1),
	//	[&](const boost::system::error_code& error, size_t bytes_transferred)
	m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER_SIZE),
		[&](const boost::system::error_code& error,const size_t& bytes_transferred)
	{	
		//cout << "Bytes_Transferred: " << bytes_transferred << endl;
		// error = 0 ���� , error != 0 ����
		if (error)  // !error = �����ȳ�����
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

		//cur_data_proc ����
		//bytes_transferred �� 20�ε�, need_to_read �� m_prev_packet_size �� m_cur_packet_size = 0�� ����
		//20bytes �� �Ѿ�Դµ� ����ִ� �����Ͱ� ��� 0 �λ���

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
			if (m_state == DEAD)
				break;

			//1. �޾Ƶ��� ������(Ű�� ���� �÷��̾ ��������)���� ��ȭ�� ������ ����(����ȿ�� ����x)
			auto PosMove_Data = reinterpret_cast<STC_ChangedPos*>(packet);

			if (PosMove_Data->deltime < 0)
				PosMove_Data->deltime = 0.f;

			if (PosMove_Data->ani_state == Ani_State::Idle)
				m_state = IDLE;
			else if (PosMove_Data->ani_state == Ani_State::Run)
				m_state = MOVE;

			//2. ����ȿ�� ����
			pp->SetPosition(PosMove_Data->pos.x, PosMove_Data->pos.y, PosMove_Data->pos.z);
			pe->GravitySystem(PosMove_Data->deltime, pp);

			XMFLOAT4 xmf4{ PosMove_Data->pos.x, PosMove_Data->pos.y, PosMove_Data->pos.z, PosMove_Data->pos.w };
			pp->integrate(PosMove_Data->deltime, &xmf4);
			PosMove_Data->pos.x = xmf4.x; PosMove_Data->pos.y = xmf4.y;  PosMove_Data->pos.z = xmf4.z;  PosMove_Data->pos.w = xmf4.w;

			pe->AfterGravitySystem(PosMove_Data->deltime, pp, OBJECT_TYPE::PLAYER, PosMove_Data->pos, m_pdata.airbone);

			//3. �̵� - (�ִϸ��̼�, ��ġ ����) ����� �����͸� �������������ϴ� �� Ŭ���̾�Ʈ�� ����
			m_clients[PosMove_Data->id]->m_pdata.pos = move(PosMove_Data->pos);
			m_clients[PosMove_Data->id]->m_pdata.ani = PosMove_Data->ani_state;

			//cout << "ID: " << PosMove_Data->id << " ��ȭ�� ��ġ��: " << "[x:" << PosMove_Data->pos.x << "\t" << "y:" << PosMove_Data->pos.y
			//	<< "\t" << "z:" << PosMove_Data->pos.z << "]" << "\t" << "w:" << PosMove_Data->pos.w << endl;

			//4. ��ȭ�� �� (������, �ִϸ��̼�) ������ �ٸ� Ŭ�� ���� - �ݵ�� �̷��� �ٽ� ����������
			//PosMove_Data�� �ٷ� sendpacket�� packet���� ����ȭ�Ͽ� ������ size error�� ����
			STC_ChangedPos c_to_other;
			
			c_to_other.id = PosMove_Data->id;
			c_to_other.ani_state = m_pdata.ani;
			c_to_other.pos = move(m_pdata.pos);
		
			for (auto client : m_clients)
			{
				//��밡 ai / ������� / ���� ��� ���� �ʿ� ����
				if (client->m_pdata.ai == true) continue;
				if (client->m_pdata.connect == false) continue;

				//if (client->m_pdata.ID == PosMove_Data->id) continue;
				//���⼭ ����

				//���ŵ� ���� �����͸� ���濡�� ����
				client->SendPacket(reinterpret_cast<Packet*>(&c_to_other));	
			}

		}
		break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ROTATE:
		{
			if (m_state == DEAD)
				break;

			m_state = ROTATE;

			auto Rotation_Data = reinterpret_cast<STC_Rotation*>(packet);	

			// 1. ���� ������ �� Ŭ���̾�Ʈ�� �־��ְ�
			m_clients[Rotation_Data->id]->m_pdata.rot = move(Rotation_Data->rotate_status);

			// 2. ���� ������ ���� lookvector�� rightvector�� ������Ʈ
			//m_clients[Rotation_Data->id]->UpdateLookVector(); //�̰ų����� 20byte -> 21byte�� �ٽú����Ե�
			pe->UpdateLookVector(OffLookvector, OffRightvector, Rotation_Data->rotate_status, Lookvector, Rightvector);
			pe->GetUpVector(Lookvector, Rightvector, Upvector);

			//cout << "ID: " << Rotation_Data->id << " ��ȭ�� ȸ����: " << "[ x, y, z, w ]: "
			//	<< Rotation_Data->rotate_status.x << ", " << Rotation_Data->rotate_status.y << ", " << Rotation_Data->rotate_status.z << ", " << Rotation_Data->rotate_status.w << endl;

			// 3. �ٸ� Ŭ�󿡰� ������.
			STC_Rotation r_to_other;
			r_to_other.id = Rotation_Data->id;
			r_to_other.rotate_status = move(Rotation_Data->rotate_status);

			for (auto client : m_clients)
			{
				if (static_cast<bool>(client->m_pdata.ai) == true) continue;
				if (static_cast<bool>(client->m_pdata.connect) == false) continue;
				//if (client->m_pdata.ID == Rotation_Data->id) continue;

				client->SendPacket(reinterpret_cast<Packet*>(&r_to_other));
			}

		}
		break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ATTACK:
		{
			if (m_state == PLAYER_STATE::DEAD)		break;

			auto n_bldata = reinterpret_cast<STC_Attack*>(packet);

			//����Ű�� ������ ��, �ҷ� ����.
			//�ҷ��� ������ ĳ���� ID, ������ ����� Ÿ�� ID, �ҷ� �ʱ������ġ, �ҷ� �ʱ�ȸ����, �ҷ� �����ð�, �ҷ����̵�
			m_bullObj = new BulletObject(n_bldata->bull_data.Master_ID, n_bldata->bull_data.myID,
				n_bldata->bull_data.pos, n_bldata->bull_data.Rotate_status, n_bldata->lifetime,
				n_bldata->bull_data.vel3f, n_bldata->bull_data.type, n_bldata->bull_data.endpoint);

			//�ҷ� ������ �ϳ��� ����ȿ�� �������ֱ� (integrate -> aftergravity) -> set bullet start time
			m_bullObj->GetPhysicsPoint()->SetVelocity(n_bldata->bull_data.vel3f.x, n_bldata->bull_data.vel3f.y, n_bldata->bull_data.vel3f.z);
			m_bullObj->GetPhysicsPoint()->integrate(n_bldata->lifetime, reinterpret_cast<XMFLOAT4*>(&n_bldata->bull_data.pos));
			m_bullObj->AfterGravitySystem();
			m_bullObj->SetBulletLifeTime(n_bldata->lifetime); // �ð� 0.1 ~ 0.2 �߰�
			//m_bullObj->SetIsFirstCreate(true);

			//�ҷ��� ó���������� ���� �ð��� ����. ���� �ش� �̺�Ʈ���� �� �ҷ��� �����ֱ�ð��� ������
			__int64 currTime; QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
			m_bullObj->m_prevTime = currTime;
			
			m_bullobjs.emplace_back(m_bullObj);

			STC_Attack stc_attack;
			stc_attack.bull_data = move(m_bullObj->GetBulletInfo());
			stc_attack.lifetime = m_bullObj->GetBulletLifeTime();

			//�ҷ��� �� ĳ������ ���ݾִϸ��̼��� �ٸ� Ŭ���̾�Ʈ���Ե� �����ش�
			STC_CharAnimation stc_charani;
			stc_charani.id = m_bullObj->GetBulletMasterID();
			stc_charani.char_animation = Ani_State::Attack;

			//�ҷ��� ������ ��ġ�� �� ���� ���浵 �˰� �־����. //���� Ŭ���̾�Ʈ���� �� �ҷ� ������ġ�� ����
			for (auto client : m_clients)
			{
				//�ҷ��� �� Ŭ���̾�Ʈ�� �ڽ��� �ҷ��� ���������Ƿ� ���� ���������� �������� �ʾƵ��� 
				if (client->m_id == n_bldata->bull_data.Master_ID) continue;
				if (static_cast<bool>(client->m_pdata.ai) == true || static_cast<bool>(client->m_connect_state) == false) continue;

				client->SendPacket(reinterpret_cast<Packet*>(&stc_charani));
				client->SendPacket(reinterpret_cast<Packet*>(&stc_attack));
			}

			if (n_bldata->bull_data.type == BULLET_TYPE::protocol_LightBullet)
				g_timer_queue.AddEvent(n_bldata->bull_data.myID, 0, LIGHT_BULLET, true, n_bldata->bull_data.Master_ID);
			 
			if (n_bldata->bull_data.type == BULLET_TYPE::protocol_HeavyBullet)
				g_timer_queue.AddEvent(n_bldata->bull_data.myID, 0, HEAVY_BULLET, true, n_bldata->bull_data.Master_ID);
		}
		break;

	case PACKET_PROTOCOL_TYPE::PLAYER_JUMP:
		{
			if (m_state == DEAD) break;

			auto jump_data = reinterpret_cast<STC_CharJump*>(packet);

			GeneratorJump j;
			j.SetJumpVel(XMFLOAT3(0, 100, 0));
			//j.Update(0.f, *pp);
			j.Update(0.f, *m_clients[jump_data->id]->GetPhysicsPoint());
			m_clients[jump_data->id]->m_pdata.airbone = true;
			//m_clients[jump_data->id]->AirBone = true;
			//m_pdata.airbone = true;
			//m_pdata.ani = jump_data.ani_state;
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
	
	}
}

void Player_Session::Set_State(int state)
{
	if (state != -1)
		m_state = state;
}

void Player_Session::Damaged(float damage)
{
	if (m_pdata.godmode == false && m_pdata.status.cur_hp > 0)
	{
		m_pdata.status.cur_hp -= damage;
	}

	if (m_pdata.status.cur_hp <= 0)
	{
		delobj = true;

		//������ ��ó�� - timer_queue
	}
	
}

//1. �÷��̾�� ����ƽ ������Ʈ���� �浹
void Player_Session::Collision_StaticObjects(unordered_set<StaticObject*>& sobjs, float DeltaTime)
{
	//������ ��� - �浹 ����� ����ƽ������Ʈ
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
				m_pdata.airbone = false;
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
			pe->UpdatePPosCenterPos(pp->GetPosition(), m_pdata.pos);
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

			if (test)//�浹������ �浹�ؼҸ� �ؾ��Ѵ�.
			{
				//�浹 ������ ���� (0,1,0) �̸� Airbone�� false�� �д�. �̴� ���� ���������� ��Ÿ����.
				if (pp->pAxis.y > 0)
				{
					AirBone = false;
					m_pdata.airbone = false;
				}

				//�浹������  ���� (0,-1,0)�̸� ���� Airbone�� false�� �д�.  �̴� ��밡 �� ���������� ��Ÿ����.
				//���� ��� ���� �ٸ� ��밡 �־ ��������. �ڿ����������.
				if (pp->pAxis.y < 0)
				{
					(*iter)->m_pdata.airbone = false;
					(*iter)->AirBone = false;
				}

				XMFLOAT3 cn;
				if ((*iter)->staticobject == false)
				{
					cn = Float3Add(pp->GetPosition(), (*iter)->pp->GetPosition(), false);
					cn = Float3Normalize(cn);
				}
				else//������ ��ü�� �浹�� ����� ��ֹ������� cn�� �����Ұ�.
				{
					cn = pp->pAxis;
				}

				//�浹�ؼ� ȣ��. �浹�ؼ� ���Ŀ� �ݵ�� ����� ������ ��ġ�� ������Ʈ��ġ�� ��ġ���Ѿ��Ѵ�.
				pp->CollisionResolve(*(*iter)->pp, cn, DeltaTime);
				pe->UpdatePPosCenterPos(pp->GetPosition(), m_pdata.pos);
				(*iter)->pe->UpdatePPosCenterPos((*iter)->pp->GetPosition(), (*iter)->m_pdata.pos);
					
			}
		}
	}
}

void Player_Session::RegularUpdate()
{
	//RegularUpdate �Լ��� �ѹ� ������ Loop�� ���鼭 1�ʿ� 20���� 
	//SendPacket�� ������� - callback�Լ��� cost�� ŭ. 
	//�����带 ���� �Ҵ���� �� WorkerThread���� �۾��ؾ��� 

	auto local_prevTime = high_resolution_clock::now();
	while (true)
	{
		//1. 0.05�� �� ������Ʈ�� �ҷ� ���� ������
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



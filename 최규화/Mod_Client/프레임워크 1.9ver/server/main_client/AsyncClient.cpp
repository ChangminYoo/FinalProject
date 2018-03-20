#include "AsyncClient.h"
#include "Database\CTextTest.h"

AsyncClient::~AsyncClient()
{
}

void AsyncClient::Init()
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVERIP), SERVERPORT);

	Connect(endpoint);
}

void AsyncClient::Connect(boost::asio::ip::tcp::endpoint& endpoint)
{
	m_cur_logindata = new CTextTest();
	m_cur_logindata->InputLoginData("Test.txt");

	m_socket.async_connect(
		endpoint, [&](const boost::system::error_code& error)
	{
		if (!error)
		{
			cout << "Connected" << endl;

			m_socket.send(boost::asio::buffer(m_cur_logindata->loginPacketBuf, MAX_BUFFER_SIZE));
			m_socket.receive(boost::asio::buffer(m_cur_logindata->loginPacketBuf, MAX_BUFFER_SIZE));

			if (m_cur_logindata->loginPacketBuf[0] == 1)
			{
				cout << "Client ID and PW send Success" << endl;

				//RecvPacket();
				//return;
			}
			//Packet send_client_num_buf = static_cast<unsigned char>(temp_DB.Get_LoginPacketInfo()->size());
			//m_socket.send(send_client_num_buf, sizeof(Packet));

			//2. ������ ���Ἲ�� �� ������ ���θ޴����� �Է��� �α��� ������ ����
			//�����ͺ��̽��� ���� �� �� �����͸� ������ ����		
			
			//for (auto i = 0; i < temp_DB->Get_LoginPacketInfo()->size(); ++i)
			//{
			//	//���� ���� - ���� ������
			//	auto temp_buf = temp_DB->Get_LoginPacketInfo()->data();
			//	m_socket.send(temp_buf[i], MAX_BUFFER_SIZE);
			//	
			//	m_socket.receive(temp_buf[i], MAX_BUFFER_SIZE);
			//}
			

		}
		else
		{
			cout << "Connected failed: " << error.message() << endl;
		}
	});


}

void AsyncClient::RecvPacket()
{
	//m_socket.async_read_some(boost::asio::buffer(m_recvBuf, MAX_BUFFER_SIZE),
	//	[&](const boost::system::error_code& error, size_t bytes_transferred) 
	//	-> void 
	boost::asio::async_read(m_socket, boost::asio::buffer(m_recvBuf, MAX_BUFFER_SIZE),
		[&](const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			//����: �۾��� ��ҵ� ��� 
			if (error.value() == boost::asio::error::operation_aborted)
				return;

			//	cout << "Client No. [ " << m_id << " ] Disconnected [Operation_Aborted] -----  ";

			//if (error.value() == boost::asio::error::message_size)
			//	cout << "Client No. [ " << m_id << " ] Disconnected [Message_Too_Long] -----  ";

			cout << "IP: " << m_socket.remote_endpoint().address().to_string() << " // ";
			cout << "PORT: " << m_socket.remote_endpoint().port() << endl;

			// shutdown_both - �ְ� �޴� �� ��θ� �ߴ�
			m_socket.shutdown(m_socket.shutdown_both);
			m_socket.close();

			return;

			// �ڽ��� ������� - ����
			//Set_State(-1);
			//m_connect_state = CONNECT_STATE::DISCONNECT;

			//STC_Disconnected dis_msg;
			//dis_msg.id = m_id;

			// �ڽ��ֺ��� �ִ� ���鿡�� �ڽ��� ������¿� ���� ������ ��������
			//auto clients = move(m_player->Get_Clients());
			//clients[m_id]->Set_Connect_State(CONNECT_STATE::DISCONNECT);
			//clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&dis_msg));	
		}

		cout << endl;
		//ProcessPacket(Get_RecvBuf);
		//RecvPacket();
		/*
		int cur_data_proc = static_cast<int>(bytes_transferred);
		Packet* temp_buf = m_recvBuf;
		while (cur_data_proc > 0)
		{
			if (m_cur_packet_size == 0)
			{
				m_cur_packet_size = temp_buf[0];
				if (temp_buf[0] > MAX_BUFFER_SIZE)
				{
					exit(-1);
				}
			}

			int need_to_read = m_cur_packet_size - m_prev_packet_size;
			if (need_to_read <= cur_data_proc)
			{
				memcpy(m_dataBuf + m_prev_packet_size, temp_buf, need_to_read);

				//ProcessPacket(m_dataBuf);

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
		*/

	});
}

void AsyncClient::SendPacket(Packet * packet)
{
	int packet_size = packet[0];
	Packet *new_sendBuf = new Packet[packet_size];
	memcpy(new_sendBuf, packet, packet_size);

	boost::asio::async_write(m_socket, boost::asio::buffer(new_sendBuf, packet_size),
		[&](const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error != 0)
		{
			//if (bytes_transferred != packet_size)
			//{
			//	cout << "Client No. [ " << m_id << " ] error code : " << error.value() << endl;
			//}

			delete[] new_sendBuf;
			return;
		}

		RecvPacket();
	});
}


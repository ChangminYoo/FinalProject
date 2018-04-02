//#pragma once
//#include "stdafx.h"

#include "Player.h"

Player::Player() : m_acceptor(g_io_service,
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), SERVERPORT)) , m_socket(g_io_service)
{
	//GetMyServerIP();

	CheckMyCPUCore();

	//���� �ʱ�ȭ�� �����Ǹ��� ����Ǿ����

	//1. �ʱ�ȭ
	//Monster_Init();

	//2. Ŭ���̾�Ʈ ���� �� ���������� �ʱ�ȭ
	Accept_Event();

	//3.cpu ������ŭ ������ ���� -> ��Ƽ�����带 �̿��� ��Ŷ��ȯ
	MainLogic();
}

Player::~Player()
{
	//for (auto del : m_clients)
	//	delete del;


}

void Player::GetMyServerIP(boost::asio::io_service& io_service)
{
	using Resolver = boost::asio::ip::tcp::resolver;
	Resolver resolver(io_service);
	Resolver::query query(boost::asio::ip::host_name(), " ");
	Resolver::iterator resolver_iter = resolver.resolve(query);

	while (resolver_iter != Resolver::iterator())
	{
		boost::asio::ip::address addr = (resolver_iter++)->endpoint().address();
		if (!addr.is_v6())
		{
			cout << "This Server's IPv4 Address: " << addr.to_string() << endl;
		}
	}
}

void Player::CheckMyCPUCore()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int numCPU = sysinfo.dwNumberOfProcessors;
	m_myCPUCoreCnt = numCPU;

	cout << "My CPU Core = " << m_myCPUCoreCnt << endl;
}

void Player::Monster_Init()
{
	//������ Ŭ���̾�Ʈ�� ������ ������� ���͵����Ϳ� ���� �ʱ�ȭ�� ����Ǿ����

	//m_clients.reserve(m_monsterCnt);

	for (auto i = 0; i < m_monsterCnt; ++i)
	{
		auto pNewSession = new Player_Session(m_playerIndex, boost::asio::ip::tcp::socket(g_io_service));	
		pNewSession->Init_MonsterInfo();
		pNewSession->m_clients.emplace_back(pNewSession);	
		++m_playerIndex;

		delete pNewSession;
	}
}

void Player::Accept_Event()
{
	//1. Ŭ���̾�Ʈ�� ����õ� 2. ����ƴٸ� �����Լ������� �ش� Ŭ���̾�Ʈ ���� �ʱ�ȭ���� �� ������ ����
	m_acceptor.async_accept(m_socket,
		[&](/*Player_Session* session, */ const boost::system::error_code& error)
	{
		if (!error)
		{
			cout << "Ŭ���̾�Ʈ [ " << m_playerIndex << " ] ���ӿϷ�. ";
			cout << "IP = " << m_socket.remote_endpoint().address().to_string() << " /// ";
			cout << "PORT = " << m_socket.remote_endpoint().port() << endl;

			Player_Session* pNewSession = new Player_Session(m_playerIndex, move(m_socket));

			if (pNewSession->CheckPlayerInfo())
			{
				pNewSession->Init_PlayerInfo();

				pNewSession->m_clients.emplace_back(pNewSession);

				//pNewSession->InitData_To_Client();

				cout << "Ŭ���̾�Ʈ [ " << m_playerIndex << " ] �������Ҵ� �Ϸ�. " << endl;

				++m_playerIndex;

				//delete�� ���ָ� �޸𸮿����� ��. pNewSession���� �۾��� ���� �� ���´µ� ���̷��� �ϱ⶧����
				//delete pNewSession;
			}
			else
			{
				cout << "Ŭ���̾�Ʈ [ " << m_playerIndex << " ] �������Ҵ� ����. " << endl;
			}
			

			//��� ���� ó��
			//session->PostReceive();
		}

		if (m_serverShutDown == false)
		{
			Accept_Event();
		}
	});

}

void Player::MainLogic()
{ 
	//g_io_service.run();
	thread f_thread([]() 
	{ 
		auto work = make_shared<boost::asio::io_service::work>(g_io_service);
		g_io_service.run(); 
	});

	f_thread.join();
	//for (auto i = 0; i < m_myCPUCoreCnt; ++i)
	//{
	//	m_pworkerThread.emplace_back(new thread{ [&]() {g_io_service.run(); } });
	//}
	//
	//for (auto thread : m_pworkerThread)
	//{
	//	thread->join();
	//	delete thread;
	//}
}

/*#include "ChattingServer.h"

void ChatServer::Init(const int nMaxSessionCount)
{
	for (auto i = 0; i < nMaxSessionCount; ++i)
	{
		Session *pNewSession = new Session(i, m_acceptor.get_io_service(), this);
		m_SessionList.push_back(pNewSession);
		m_SessionQueue.push_back(i);

	}
}*/

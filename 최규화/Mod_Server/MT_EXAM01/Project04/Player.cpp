//#pragma once
//#include "stdafx.h"

#include "Player.h"

Player::Player() : m_acceptor(g_io_service,
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), SERVERPORT)) , m_socket(g_io_service)
{
	//GetMyServerIP();

	CheckMyCPUCore();

	//몬스터 초기화는 게임판마다 실행되어야함

	//1. 초기화
	//Monster_Init();

	//2. 클라이언트 연결 및 유저데이터 초기화
	Accept_Event();

	//3.cpu 갯수만큼 스레드 생성 -> 멀티스레드를 이용한 패킷교환
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
	//서버와 클라이언트의 연동과 관계없는 몬스터데이터에 대한 초기화가 진행되어야함

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
	//1. 클라이언트와 연결시도 2. 연결됐다면 람다함수내에서 해당 클라이언트 정보 초기화진행 및 데이터 저장
	m_acceptor.async_accept(m_socket,
		[&](/*Player_Session* session, */ const boost::system::error_code& error)
	{
		if (!error)
		{
			cout << "클라이언트 [ " << m_playerIndex << " ] 접속완료. ";
			cout << "IP = " << m_socket.remote_endpoint().address().to_string() << " /// ";
			cout << "PORT = " << m_socket.remote_endpoint().port() << endl;

			Player_Session* pNewSession = new Player_Session(m_playerIndex, move(m_socket));

			if (pNewSession->CheckPlayerInfo())
			{
				pNewSession->Init_PlayerInfo();

				pNewSession->m_clients.emplace_back(pNewSession);

				//pNewSession->InitData_To_Client();

				cout << "클라이언트 [ " << m_playerIndex << " ] 데이터할당 완료. " << endl;

				++m_playerIndex;

				//delete를 해주면 메모리에러가 남. pNewSession에서 작업을 아직 안 끝냈는데 죽이려고 하기때문에
				//delete pNewSession;
			}
			else
			{
				cout << "클라이언트 [ " << m_playerIndex << " ] 데이터할당 오류. " << endl;
			}
			

			//요거 이제 처리
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

//#include "CAccpetPlayer.h"
#include <thread>
#include "stdafx.h"


CAccpetPlayer::CAccpetPlayer() : m_acceptor(g_io_service,
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), SERVERPORT)), m_socket(g_io_service)
{
	//1. 연결된 네트워크 IP획득
	//GetMyServerIP();

	//2. 서버가 작동하는 컴퓨터 cpu갯수 획득
	CheckMyCPUCore();

	//3. 스테틱오브젝트 초기화
	g_staticobj = new CStaticObject();
	g_staticobj->StaticObjectLoop();

	//4. 초기화 and 몬스터 초기화는 게임판마다 실행되어야함
	//Monster_Init();		

	//5. 클라이언트 연결 및 유저데이터 초기화
	AcceptEvent();

	//6.cpu 갯수만큼 스레드 생성 -> 멀티스레드를 이용한 패킷교환
	MainLogic();
}

void CAccpetPlayer::GetMyServerIP(boost::asio::io_service & io_service)
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

void CAccpetPlayer::CheckMyCPUCore()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	unsigned int numCPU = sysinfo.dwNumberOfProcessors;
	m_myCPUCoreCnt = numCPU;

	cout << "My CPU Core = " << m_myCPUCoreCnt << endl;
}

void CAccpetPlayer::InitMonster()
{
}

void CAccpetPlayer::AcceptEvent()
{
	//1. 클라이언트와 연결시도 2. 연결됐다면 람다함수내에서 해당 클라이언트 정보 초기화진행 및 데이터 저장
	//Accept -> Recv -> Send 순 
	
	m_acceptor.async_accept(m_socket, [&](const boost::system::error_code& error)
	{
		if (!error)
		{
			cout << "클라이언트 [ " << m_playerIndex << " ] 접속완료. ";
			cout << "IP = " << m_socket.remote_endpoint().address().to_string() << " /// ";
			cout << "PORT = " << m_socket.remote_endpoint().port() << endl;

			CPlayerObject* pNewSession = new CPlayerObject(m_playerIndex, move(m_socket));
		
			if (pNewSession->CheckPlayerInfo())
			{
				//1. Client 정보 초기화 및 Client 정보를 담은 벡터에 데이터 추가
				pNewSession->Init_PlayerInfo();

				g_clients.emplace_back(pNewSession);

				//2. Static Object 초기화 
				g_clients[m_playerIndex]->SendStaticObjectPacket(g_staticobjs);

				//3. 초기화된 정보를 연결된 클라이언트로 보낸다.
				g_clients[m_playerIndex]->InitData_To_Client();

				//4.accept -> recv 순으로 해서 클라이언트로 부터 패킷을 받을 준비를 한다
				g_clients[m_playerIndex]->RecvPacket();

				cout << "클라이언트 [ " << m_playerIndex << " ] 데이터할당 완료. " << endl;
				++m_playerIndex;

				if (m_start_timer)
				{
					g_timer_queue.SetRegularPrevTime();
					g_timer_queue.AddEvent(0, 0, REGULAR_PACKET_EXCHANGE, true, 0);
					m_start_timer = false;

				}
			}
			else
			{
				cout << "클라이언트 [ " << m_playerIndex << " ] 데이터할당 오류. " << endl;
			}

		}

		if (m_server_shutdown == false)
		{
			AcceptEvent();
		}
	});

}

void CAccpetPlayer::MainLogic()
{
	//g_io_service.run();

	//1. 패킷 송수신을 담당하는 Worker_Thread
	thread f_thread([]()
	{
		auto work = make_shared<boost::asio::io_service::work>(g_io_service);
		g_io_service.run();
	});

	//2. 물리효과와 충돌처리를 하는 물리엔진관련 작업을 하는 Physics_Thread
	m_pWorkerThread.emplace_back(new thread{ [&]()
	{ 
		g_physics_worker.CheckPrevTime();
		g_physics_worker.Update();   
	} });

	//3. 주기적으로 작업이 필요한 것들을 처리하는 Timer_Thread 
	m_pWorkerThread.emplace_back(new thread{ [&]()
	{ 
		g_timer_queue.CheckPrevTime();
		g_timer_queue.AddEvent(0, 0, REGULAR_PACKET_EXCHANGE, true, 0);
		g_timer_queue.TimerThread(); 

	} });

	f_thread.join();

	for (auto thread : m_pWorkerThread)
	{
		thread->join();
		delete thread;
	}

}

CAccpetPlayer::~CAccpetPlayer()
{
	if (g_staticobj != nullptr)
		delete g_staticobj;
}

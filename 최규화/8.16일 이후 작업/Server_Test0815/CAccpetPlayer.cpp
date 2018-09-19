#include <thread>
#include "stdafx.h"
bool CAccpetPlayer::m_start_mainLogic = false;

CAccpetPlayer::CAccpetPlayer() : m_acceptor(g_io_service,
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), SERVERPORT)), m_socket(g_io_service)
{
	g_clients.reserve(MAX_PLAYER + MAX_NPC_MONSTER_NUM);
	//1. 연결된 네트워크 IP획득
	//GetMyServerIP(g_io_service);

	//2. 서버가 작동하는 컴퓨터 cpu갯수 획득
	CheckMyCPUCore();

	//3. 스테틱오브젝트 초기화
	g_staticobj = new CStaticObject();
	g_staticobj->StaticObjectLoop();

	//4. 초기화 and 몬스터 초기화는 게임판마다 실행되어야함
	InitMonster();

	//5. 클라이언트 연결 및 유저데이터 초기화
	AcceptEvent(g_io_service);

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
	unsigned int numCPU = static_cast<unsigned int>(sysinfo.dwNumberOfProcessors);
	m_myCPUCoreCnt = numCPU;

	cout << "CPU Core = " << m_myCPUCoreCnt / 2 << "\n";
}

void CAccpetPlayer::InitMonster()
{
	for (int i = 0; i < MAX_IMP_NUM; ++i)
	{
		CNpcObject *imp = new CNpcObject(i, NPC_MONSTER_TYPE::IMP);
		imp->fsm = new FSM(imp);
		g_npcs.emplace_back(imp);
	}
}

void CAccpetPlayer::AcceptEvent(boost::asio::io_service& io_service)
{
	//1. 클라이언트와 연결시도 2. 연결됐다면 람다함수내에서 해당 클라이언트 정보 초기화진행 및 데이터 저장
	//Accept -> Recv -> Send 순 

	m_acceptor.async_accept(m_socket, [&](const boost::system::error_code& error)
	{
		if (!error)
		{
			if (g_start_oneGame)
			{
				cout << "게임이 시작되어 해당 방에 다시 접속할 수 없습니다\n";

				// shutdown_both - 주고 받는 쪽 모두를 중단
				m_socket.shutdown(m_socket.shutdown_both);

				m_socket.close();

				return;
			}

			if (m_playerIndex + 1 > MAX_PLAYER)
			{
				cout << "서버의 최대 동시 접속가능 인원을 초과하여 접속할 수 없습니다. \n";

				m_socket.shutdown(m_socket.shutdown_both);

				m_socket.close();

				return;
			}

			cout << "클라이언트 [ " << m_playerIndex << " ] 접속완료. ";
			cout << "IP = " << m_socket.remote_endpoint().address().to_string() << " /// ";
			cout << "PORT = " << m_socket.remote_endpoint().port() << endl;

			CPlayerObject* pNewSession = new CPlayerObject(m_playerIndex, move(m_socket), io_service);

			g_clients.emplace_back(pNewSession);

			g_clients[m_playerIndex]->RecvPacket();

			++m_playerIndex;

			//재귀를 사용하지 않고 루프로 돌리는 게 더 낫다
			if (m_server_shutdown == false)
			{
				AcceptEvent(io_service);
			}

		}
	});
	
	
	
}

void CAccpetPlayer::MainLogic()
{
	//1. 패킷 송수신을 담당하는 Worker_Thread
	for (auto i = 0; i < m_myCPUCoreCnt - 2; ++i)
		m_pWorkerThread.emplace_back(new thread{ [&]() { g_io_service.run(); } });
	
	//2. 물리효과와 충돌처리를 하는 물리엔진관련 작업을 하는 Physics_Thread
	m_pWorkerThread.emplace_back(new thread{ [&]() {  g_physics_worker.Update();   } });

	//3. 주기적으로 작업이 필요한 것들을 처리하는 Timer_Thread 
	m_pWorkerThread.emplace_back(new thread{ [&]()	{ g_timer_queue.TimerThread(); } });

	
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

	if (!g_clients.empty())
	{
		delete g_clients.back();
		g_clients.pop_back();
	}

	if (!g_npcs.empty())
	{
		delete g_npcs.back();
		g_npcs.pop_back();
	}
}

#include <thread>
#include "stdafx.h"
bool CAccpetPlayer::m_start_mainLogic = false;

CAccpetPlayer::CAccpetPlayer() : m_acceptor(g_io_service,
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), SERVERPORT)), m_socket(g_io_service)
{
	g_clients.reserve(MAX_PLAYER + MAX_NPC_MONSTER_NUM);
	//1. ����� ��Ʈ��ũ IPȹ��
	//GetMyServerIP(g_io_service);

	//2. ������ �۵��ϴ� ��ǻ�� cpu���� ȹ��
	CheckMyCPUCore();

	//3. ����ƽ������Ʈ �ʱ�ȭ
	g_staticobj = new CStaticObject();
	g_staticobj->StaticObjectLoop();

	//4. �ʱ�ȭ and ���� �ʱ�ȭ�� �����Ǹ��� ����Ǿ����
	InitMonster();

	//5. Ŭ���̾�Ʈ ���� �� ���������� �ʱ�ȭ
	AcceptEvent(g_io_service);

	//6.cpu ������ŭ ������ ���� -> ��Ƽ�����带 �̿��� ��Ŷ��ȯ
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
	//1. Ŭ���̾�Ʈ�� ����õ� 2. ����ƴٸ� �����Լ������� �ش� Ŭ���̾�Ʈ ���� �ʱ�ȭ���� �� ������ ����
	//Accept -> Recv -> Send �� 

	m_acceptor.async_accept(m_socket, [&](const boost::system::error_code& error)
	{
		if (!error)
		{
			if (g_start_oneGame)
			{
				cout << "������ ���۵Ǿ� �ش� �濡 �ٽ� ������ �� �����ϴ�\n";

				// shutdown_both - �ְ� �޴� �� ��θ� �ߴ�
				m_socket.shutdown(m_socket.shutdown_both);

				m_socket.close();

				return;
			}

			if (m_playerIndex + 1 > MAX_PLAYER)
			{
				cout << "������ �ִ� ���� ���Ӱ��� �ο��� �ʰ��Ͽ� ������ �� �����ϴ�. \n";

				m_socket.shutdown(m_socket.shutdown_both);

				m_socket.close();

				return;
			}

			cout << "Ŭ���̾�Ʈ [ " << m_playerIndex << " ] ���ӿϷ�. ";
			cout << "IP = " << m_socket.remote_endpoint().address().to_string() << " /// ";
			cout << "PORT = " << m_socket.remote_endpoint().port() << endl;

			CPlayerObject* pNewSession = new CPlayerObject(m_playerIndex, move(m_socket), io_service);

			g_clients.emplace_back(pNewSession);

			g_clients[m_playerIndex]->RecvPacket();

			++m_playerIndex;

			//��͸� ������� �ʰ� ������ ������ �� �� ����
			if (m_server_shutdown == false)
			{
				AcceptEvent(io_service);
			}

		}
	});
	
	
	
}

void CAccpetPlayer::MainLogic()
{
	//1. ��Ŷ �ۼ����� ����ϴ� Worker_Thread
	for (auto i = 0; i < m_myCPUCoreCnt - 2; ++i)
		m_pWorkerThread.emplace_back(new thread{ [&]() { g_io_service.run(); } });
	
	//2. ����ȿ���� �浹ó���� �ϴ� ������������ �۾��� �ϴ� Physics_Thread
	m_pWorkerThread.emplace_back(new thread{ [&]() {  g_physics_worker.Update();   } });

	//3. �ֱ������� �۾��� �ʿ��� �͵��� ó���ϴ� Timer_Thread 
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

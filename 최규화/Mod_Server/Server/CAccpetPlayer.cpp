//#include "CAccpetPlayer.h"
#include <thread>
#include "stdafx.h"


CAccpetPlayer::CAccpetPlayer() : m_acceptor(g_io_service,
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), SERVERPORT)), m_socket(g_io_service)
{
	//1. ����� ��Ʈ��ũ IPȹ��
	//GetMyServerIP();

	//2. ������ �۵��ϴ� ��ǻ�� cpu���� ȹ��
	CheckMyCPUCore();

	//3. ����ƽ������Ʈ �ʱ�ȭ
	g_staticobj = new CStaticObject();
	g_staticobj->StaticObjectLoop();

	//4. �ʱ�ȭ and ���� �ʱ�ȭ�� �����Ǹ��� ����Ǿ����
	//Monster_Init();		

	//5. Ŭ���̾�Ʈ ���� �� ���������� �ʱ�ȭ
	AcceptEvent();

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
	unsigned int numCPU = sysinfo.dwNumberOfProcessors;
	m_myCPUCoreCnt = numCPU;

	cout << "My CPU Core = " << m_myCPUCoreCnt << endl;
}

void CAccpetPlayer::InitMonster()
{
}

void CAccpetPlayer::AcceptEvent()
{
	//1. Ŭ���̾�Ʈ�� ����õ� 2. ����ƴٸ� �����Լ������� �ش� Ŭ���̾�Ʈ ���� �ʱ�ȭ���� �� ������ ����
	//Accept -> Recv -> Send �� 
	
	m_acceptor.async_accept(m_socket, [&](const boost::system::error_code& error)
	{
		if (!error)
		{
			cout << "Ŭ���̾�Ʈ [ " << m_playerIndex << " ] ���ӿϷ�. ";
			cout << "IP = " << m_socket.remote_endpoint().address().to_string() << " /// ";
			cout << "PORT = " << m_socket.remote_endpoint().port() << endl;

			CPlayerObject* pNewSession = new CPlayerObject(m_playerIndex, move(m_socket));
		
			if (pNewSession->CheckPlayerInfo())
			{
				//1. Client ���� �ʱ�ȭ �� Client ������ ���� ���Ϳ� ������ �߰�
				pNewSession->Init_PlayerInfo();

				g_clients.emplace_back(pNewSession);

				//2. Static Object �ʱ�ȭ 
				g_clients[m_playerIndex]->SendStaticObjectPacket(g_staticobjs);

				//3. �ʱ�ȭ�� ������ ����� Ŭ���̾�Ʈ�� ������.
				g_clients[m_playerIndex]->InitData_To_Client();

				//4.accept -> recv ������ �ؼ� Ŭ���̾�Ʈ�� ���� ��Ŷ�� ���� �غ� �Ѵ�
				g_clients[m_playerIndex]->RecvPacket();

				cout << "Ŭ���̾�Ʈ [ " << m_playerIndex << " ] �������Ҵ� �Ϸ�. " << endl;
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
				cout << "Ŭ���̾�Ʈ [ " << m_playerIndex << " ] �������Ҵ� ����. " << endl;
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

	//1. ��Ŷ �ۼ����� ����ϴ� Worker_Thread
	thread f_thread([]()
	{
		auto work = make_shared<boost::asio::io_service::work>(g_io_service);
		g_io_service.run();
	});

	//2. ����ȿ���� �浹ó���� �ϴ� ������������ �۾��� �ϴ� Physics_Thread
	m_pWorkerThread.emplace_back(new thread{ [&]()
	{ 
		g_physics_worker.CheckPrevTime();
		g_physics_worker.Update();   
	} });

	//3. �ֱ������� �۾��� �ʿ��� �͵��� ó���ϴ� Timer_Thread 
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

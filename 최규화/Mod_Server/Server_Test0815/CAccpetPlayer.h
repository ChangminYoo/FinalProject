#pragma once

class CAccpetPlayer
{
private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::ip::tcp::socket m_socket;

	unsigned short     m_playerIndex{ 0 };
	unsigned int	   m_myCPUCoreCnt{ 1 };
	const unsigned int m_monsterCnt{ MAX_NPC_MONSTER_NUM };
	bool			   m_server_shutdown{ false };

	vector<thread*>    m_pWorkerThread;
	//CScene			*m_scene;
	bool			   m_start_timer{ true };

public:
	CAccpetPlayer();

	void GetMyServerIP(boost::asio::io_service& io_service);
	void CheckMyCPUCore();
	void InitMonster();
	void AcceptEvent();
	void MainLogic();

	~CAccpetPlayer();
};


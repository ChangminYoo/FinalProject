#pragma once
//#include "stdafx.h"
//#include "ShareHeader.h"
//#include "CScene.h"

class CAccpetPlayer
{
private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::ip::tcp::socket m_socket;

	unsigned short m_playerIndex{ 0 };
	unsigned char  m_myCPUCoreCnt{ 1 };
	const unsigned int m_monsterCnt{ MAX_MONSTER_NUM };
	char m_server_shutdown{ false };

	vector<thread*> m_pWorkerThread;
	//CScene			*m_scene;

public:
	CAccpetPlayer();

	void GetMyServerIP(boost::asio::io_service& io_service);
	void CheckMyCPUCore();
	void InitMonster();
	void AcceptEvent();
	void MainLogic();

	~CAccpetPlayer();
};


#pragma once
#include <thread>
#include "Player_Session.h"
#include "StaticObject.h"
#include "CollisionCheck.h"
#include "ShareHeader.h"


class Player_Session;

class Player
{
private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::ip::tcp::socket m_socket;

	unsigned int m_playerIndex{ 0 };
	unsigned int m_myCPUCoreCnt{ 1 };
	const unsigned int m_monsterCnt{ MAX_MONSTER_NUM };
	bool m_serverShutDown{ false };

	vector<thread*>  m_pworkerThread;
	StaticObject    *m_psobj;
	//CollisionCheck	*m_collcheck;

public:
	Player();
	~Player();

	void GetMyServerIP(boost::asio::io_service& io_service);
	void CheckMyCPUCore();
	void Monster_Init();
	void Accept_Event();
	void MainLogic();

	void SetStaticObjects();
	StaticObject* GetPSObject() { return m_psobj; }

	//CollisionCheck* GetCollsionCheck() { return m_collcheck; }

};

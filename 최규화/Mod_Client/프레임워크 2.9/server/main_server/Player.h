#pragma once
#include "Player_Session.h"
#include <thread>

class Player
{
private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::ip::tcp::socket m_socket;

	unsigned int m_playerIndex{ 0 };
	unsigned int m_myCPUCoreCnt{ 1 };
	const unsigned int m_monsterCnt{ MAX_MONSTER_NUM };
	bool m_serverShutDown{ false };

	vector<Player_Session*> m_clients;
	vector<thread*> m_pworkerThread;

public:
	Player();
	~Player();

	void GetMyServerIP(boost::asio::io_service& io_service);
	void CheckMyCPUCore();
	void Monster_Init();
	void Accept_Event();
	void MainLogic();

	vector<Player_Session*> Get_Clients() { return &m_clients; }
};


/*
class ChatServer
{
private:
boost::asio::ip::tcp::acceptor m_acceptor;
vector<Session*> m_SessionList;
vector<int> m_SessionQueue;

public:
void Init(const int nMaxSessionCount);
};
*/
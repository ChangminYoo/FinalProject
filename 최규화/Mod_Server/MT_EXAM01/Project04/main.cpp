
#include "stdafx.h"
#include "ShareHeader.h"
#include "Player.h"

boost::asio::io_service g_io_service;
TimerWorker				g_timer_queue;

int main()
{
	try
	{
		Player player;

	}
	catch (const exception& e)
	{
		cout << "Main Thread error : " << e.what() << endl;
	}

}

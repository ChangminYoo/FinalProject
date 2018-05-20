#include "stdafx.h"
#include "ShareHeader.h"

boost::asio::io_service g_io_service;
CTimerWorker			g_timer_queue;

int main()
{
	try
	{

	}
	catch (const exception& e)
	{
		cout << "Main Thread Error: " << e.what() << endl;
	}
}
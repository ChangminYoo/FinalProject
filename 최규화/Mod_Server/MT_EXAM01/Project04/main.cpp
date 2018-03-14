
//#include "Database\CTextTest.h"
#include "Player.h"
#include "stdafx.h"

//#include "stdafx.h"

boost::asio::io_service g_io_service;

//CTextTest g_database;

int main()
{
	//CTextTest *t = new CTextTest();
	//t->FileOpen("Test.txt");

	try
	{
		Player player;

	}
	catch (const exception& e)
	{
		cout << "Main Thread error : " << e.what() << endl;
	}
	//getchar();
}

#include "CTextTest.h"



CTextTest::CTextTest()
{
}

void CTextTest::FileOpen(const string& fname)
{
	ifstream file(fname, ios::app);

	if (!file.is_open())
	{
		cout << "읽을 수 없습니다." << endl;
		exit(1);
	}
	else
	{
		cout << "Input ID & Password " << endl;
		
		char memid[MAX_BUFFER_SIZE]{ "1" };
		char id[MAX_BUFFER_SIZE]{ "guest" };
		char pw[MAX_BUFFER_SIZE]{ "guest" };

		while (file >> memid >> id >> pw)
		{
			memcpy(memid, memid, sizeof(memid));
			memcpy(id, id, sizeof(id));
			memcpy(pw, pw, sizeof(pw));

			cout << memid << " +++ " << id << " +++ " << pw << " +++ " << endl;
		}
	}

	file.close();
}

void CTextTest::Init()
{
}

void CTextTest::Update()
{
}


CTextTest::~CTextTest()
{
}

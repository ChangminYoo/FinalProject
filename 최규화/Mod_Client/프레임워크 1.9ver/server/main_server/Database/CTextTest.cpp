#include "CTextTest.h"

CTextTest::CTextTest()
{
}

/*
bool CTextTest::FileOpen(const string& fname)
{
	ifstream fin(fname, ios::app);

	wchar_t loginID[MAX_FILE_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t loginPW[MAX_FILE_BUFFER_SIZE / 4]{ L"Guest" };

	if (!fin.is_open())
	{
		cout << "읽을 수 없습니다." << endl;
		return false;
	}
	else
	{
		cout << "Input ID & Password " << endl;

		while(fin >> m_loginData.mem_ID >> m_loginData.name >> m_loginData.password)
		{
			try
			{
				Packet temp_buf[MAX_BUFFER_SIZE]{ 0 };


				m_loginDataList.emplace_back(m_loginData);
			}
			catch (const exception& e)
			{
				cout << "File Read Error : " << e.what() << endl;
				
				BYTE byte = 0xff;
			}
		}

		fin.close();
		return true;
	}
	
}*/

/*
void CTextTest::Update()
{

}
*/

CTextTest::~CTextTest()
{
}

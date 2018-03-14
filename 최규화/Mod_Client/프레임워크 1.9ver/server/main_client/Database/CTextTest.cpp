#include "CTextTest.h"
#include <memory>
#include <locale>

CTextTest::CTextTest()
{
}

void CTextTest::InputLoginData(const string& fname)
{
	cout << "플레이어의 아이디와 비밀번호를 입력해주세요:";
	cin >> m_loginID >> m_loginPW;

	//FileOpenAndWrite(fname);
	FileOpenAndRead(fname);
}

void CTextTest::FileOpenAndWrite(const string& fname)
{
	ofstream fout(fname, ios::out | ios::trunc);

	if (!fout.is_open())
	{
		cout << "Cannot read file..." << endl;
		return;
	}
	else
	{
		cout << "Write login data in text file..." << endl;

		fout << m_loginID << "     " << m_loginPW << endl;
	}

	fout.close();
}

void CTextTest::FileOpenAndRead(const string& fname)
{
	wifstream wfin(fname, ios::in);

	if (!wfin.is_open())
	{
		cout << "읽을 수 없습니다." << endl;
		return;
	}
	else
	{
		cout << "Read login data in text file and Send that to Server... " << endl;

		Player_LoginDB input_data;
		while(wfin >> input_data.name >> input_data.password)
		{
			try
			{
				//--------------------------------------------------------------------------------

				loginPacketBuf[0] = wcslen(input_data.name) * 2;
				wcscpy(reinterpret_cast<wchar_t*>(&loginPacketBuf[1]), input_data.name);
				loginPacketBuf[loginPacketBuf[0] + 3] = wcslen(input_data.password) * 2;
				wcscpy(reinterpret_cast<wchar_t*>(&loginPacketBuf[loginPacketBuf[0] + 4]), input_data.password);

				//--------------------------------------------------------------------------------
			}
			catch (const exception& e)
			{
				cout << "File Read Error : " << e.what() << endl;
			}
		}

		wfin.close();
		return;
	}
}

void CTextTest::Update()
{

}

CTextTest::~CTextTest()
{
}

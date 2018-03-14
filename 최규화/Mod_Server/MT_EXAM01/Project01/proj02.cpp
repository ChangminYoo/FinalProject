#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <locale>
#include <wchar.h>

using namespace std;
using PACKET = unsigned char;
using WORD = unsigned short;
using DWORD = unsigned int;

#define MAX_BUFFER 255

using LOGIN = struct LoginData
{
	WORD ID;
	WORD PW;
};

int main()
{
	LOGIN login;
	vector<LOGIN> loginList;
	aa m;

	setlocale(LC_ALL, "korean");
	wcout.imbue(locale("korean"));

	wchar_t loginID[MAX_BUFFER / 4]{ L"Guest" };
	wchar_t loginPW[MAX_BUFFER / 4]{ L"Guest" };

	wcout << "loginID : " << loginID << endl;

	PACKET temp_buf[MAX_BUFFER]{ 0 };
	temp_buf[0] = wcslen(loginID) * 2;
	wcscpy(reinterpret_cast<wchar_t*>(&temp_buf[1]), loginID);
	temp_buf[temp_buf[0] + 3] = wcslen(loginPW) * 2;
	wcscpy(reinterpret_cast<wchar_t*>(&temp_buf[temp_buf[0] + 4]), loginPW);

	
	try
	{
		cout << "아이디 입력: " << login.ID << "비밀번호 입력: " << login.PW;
		loginList.push_back(login);
	}
	catch (const exception& e)
	{
		cout << "Error: " << e.what() << endl;
		
		WORD comp = 0x00;
		// abcdefgh -> 00000000 abcdefgh --- 00000000 11111111
		// abcd		-> 00000000 0000abcd --- 00000000 00000000
		if ((login.ID >> 8) & comp == ~(login.ID >> 8))
			cout << "Success !! " << endl;

	}


}
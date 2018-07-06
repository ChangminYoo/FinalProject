#pragma once
#include "../stdafx.h"
#include <fstream>

class CTextTest
{
private:
	string m_loginID, m_loginPW;

public:
	CTextTest();

	void InputLoginData(const string& fname);
	void FileOpenAndWrite(const string& fname);
	void FileOpenAndRead(const string& fname);

	Packet loginPacketBuf[MAX_BUFFER_SIZE]{ 0 };

	void Update();

	// юс╫ц

	~CTextTest();
};


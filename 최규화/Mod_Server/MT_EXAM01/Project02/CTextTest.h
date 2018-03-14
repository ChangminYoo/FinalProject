#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define MAX_BUFFER_SIZE 8

using Packet = unsigned char;

using Player = struct CharInfo
{
	unsigned int mem_ID{ 0 };
	char nick[MAX_BUFFER_SIZE];
	char name[MAX_BUFFER_SIZE];
	char password[MAX_BUFFER_SIZE];
};

class CTextTest
{

public:
	CTextTest();

	void FileOpen(const string& fname);
	void Init();
	void Update();

	~CTextTest();
};


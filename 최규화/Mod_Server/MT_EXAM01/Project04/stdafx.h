#pragma once
//#pragma comment(lib, "d3dx12")
//#pragma comment(lib, "d3d12")

#include <iostream>

// boost asio ���� ���
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>


#include "protocol.h"
//#include "Player.h"
//#include "Database\CTextTest.h"

const char SERVERIP[] = "127.0.0.1";
const unsigned short PORT = 31400;

#define MAX_BOX_NUM 4

using namespace std;

//io_service�� IO�� ����� ���ؼ���� - ���� �ϳ��� �־ �ȴ�
extern boost::asio::io_service g_io_service;


//extern CTextTest g_database;

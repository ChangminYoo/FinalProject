#pragma once
//#pragma comment(lib, "d3dx12")
//#pragma comment(lib, "d3d12")

#include <iostream>
#include <unordered_map>

// boost asio ���� ���
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <mutex>
#include <queue>
#include <chrono>

#include "protocol.h"


const char SERVERIP[] = "127.0.0.1";
//const char SERVERIP[] = "127.30.1.11";
//const char SERVERIP[] = "192.168.22.172";
//const char SERVERIP[] = "192.168.80.252";
const unsigned short PORT = 31400;

#define MAX_BOX_NUM 54
#define MAX_SMALL_WALL_NUM 15
#define MAX_BIG_WALL_NUM 5
#define MAX_BULLDING_NUM 3

using namespace std;
using namespace chrono;

//io_service�� IO�� ����� ���ؼ���� - ���� �ϳ��� �־ �ȴ�
extern boost::asio::io_service g_io_service;





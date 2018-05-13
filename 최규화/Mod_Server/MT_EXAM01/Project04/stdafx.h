#pragma once
//#pragma comment(lib, "d3dx12")
//#pragma comment(lib, "d3d12")

#include <iostream>
#include <unordered_map>

// boost asio 관련 헤더
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

#define MAX_BOX_NUM 4

using namespace std;
using namespace chrono;

//io_service는 IO와 통신을 위해서사용 - 전역 하나만 있어도 된다
extern boost::asio::io_service g_io_service;





#pragma once

// boost asio 헤더파일 선언위에 _WIN32_WINNT 0x0601을 선언해줘야함
// 이렇게해야 WIN32_WINNT 버전이 VISTA이상을 타겟으로 하고 있는 WRL과
// WIN32_WINNT 버전이 WINDOWS XP인 BOOST 간의 충돌이 안일어남

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#define WIN32_LEAN_AND_MEAN

// boost asio 관련 헤더
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "protocol.h"
#include <iostream>

//const char SERVERIP[] = "192.168.41.29";
const char SERVERIP[] = "127.0.0.1";
//const char SERVERIP[] = "192.168.22.106";

//const char SERVERIP[] = "127.0.0.1";
const unsigned short PORT = 31400;

using namespace std;

//io_service는 IO와 통신을 위해서사용 - 전역 하나만 있어도된다
extern boost::asio::io_service g_io_service;

//extern CTextTest g_database;

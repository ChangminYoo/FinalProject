#pragma once

// boost asio ������� �������� _WIN32_WINNT 0x0601�� �����������
// �̷����ؾ� WIN32_WINNT ������ VISTA�̻��� Ÿ������ �ϰ� �ִ� WRL��
// WIN32_WINNT ������ WINDOWS XP�� BOOST ���� �浹�� ���Ͼ

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#define WIN32_LEAN_AND_MEAN

// boost asio ���� ���
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

//io_service�� IO�� ����� ���ؼ���� - ���� �ϳ��� �־�ȴ�
extern boost::asio::io_service g_io_service;

//extern CTextTest g_database;

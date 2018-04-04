#pragma once

#define SERVERPORT 31400
#define MAX_PLAYER 4
#define MAX_MONSTER_TYPE 3
#define MAX_MONSTER_NUM 5

#define DISCONNECTED 0
#define CONNECTED 1

#define MAX_BUFFER_SIZE 255

#define KEYINPUT_UP 0x0001
#define KEYINPUT_DOWN 0x0010

//�߰�
//������ ����, ��������

using Packet = unsigned char;


enum PACKET_PROTOCOL_TYPE
{
	// ĳ���� ���� �ʱ�ȭ
	INIT_CLIENT,
	INIT_OTHER_CLIENT,

	// ĳ���� ��ǥ �� ������ȯ
	CHANGED_PLAYER_POSITION,
	CHANGED_PLAYER_DIRECTION,

	//���������
	PLAYER_DISCONNECT
};

enum CONNECT_STATE { DISCONNECT = -1, CONNECT = 1 };
enum MONSTERS { NO_MONSTER, MONSTER01, MONSTER02, MONSTER03 };
enum PLAYERS { NO_PLAYER, LUNA, CMETRA, RONDO, DONALD };

struct Position
{
	float		x{ 100 };			           //4
	float		y{ 100 };			           //4
	float		z{ 50 };			           //4
	float		w{ 0.0f };			           //4
};
//16

struct Player_Status
{
	unsigned short			attack{ 50 };       //2
	unsigned short			defend{ 50 };       //2
	unsigned short			speed{ 100 };        //2
};
//6

struct Player_LoginDB
{
	wchar_t name[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t password[MAX_BUFFER_SIZE / 4]{ L"Guest" };
};
// MAX_BUFFER_SIZE = 255 -> �� ũ��: 252����Ʈ

struct Player_Info
{
	//PLAYER_CLASS player_class;
	Player_Status				player_status;    //6
	unsigned short				exp{ 0 };         //2
	unsigned short				origin_hp{ 300 }; //2
	unsigned short   			cur_hp{ 300 };    //2
	unsigned short				origin_mp{ 100 }; //2
	unsigned short				cur_mp{ 100 };    //2
	unsigned char				level{ 1 };		  //1 
};
// 17 + 1 = 18

#pragma pack (push, 1)

struct Player_Data
{
	Player_Info		UserInfo;					//18
	unsigned short	ID{ 0 };			    //2
	Position		Pos;						//16
	char			Is_AI{ false };				//1
	char			Dir;						//1
	char			Connect_Status{ false };    //1
	//Player_LoginDB  LoginData;
};
// 18 + 16 + 2 + 1 + 1 + 1 = 40 -> 39 pragma pack �ҽ�

#pragma pack (pop)

#pragma pack (push, 1)		//push ���ۺ��� 1����Ʈ�� �����͸� �߶� ĳ�ÿ� �����Ѵ�(ĳ�ö��� ���� ������) - pop���� �����

typedef struct Server_To_Client_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	Player_Data player_data;

}STC_SetMyClient;


typedef struct Server_To_Client_Player_TO_Other_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	Player_Data player_data;

}STC_SetOtherClient;

typedef struct Server_To_Client_Player_Disconnected_Info
{
	unsigned char pack_size = sizeof(unsigned short) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_DISCONNECT;
	char connect;
	unsigned short id;

}STC_Disconnected;


typedef struct Server_To_Client_Player_Position_Changed
{
	unsigned char packet_size = sizeof(unsigned short) + sizeof(Position) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;
	unsigned short id;
	Position pos;

}STC_ChangedPos;

typedef struct Server_To_Client_Player_Direction_Changed
{
	unsigned char packet_size = sizeof(Position) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_DIRECTION;
	Position pos;
	char dir;

}STC_ChangedDir;

#pragma pack (pop)
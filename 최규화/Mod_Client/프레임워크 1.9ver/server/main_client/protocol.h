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

//추가
//레벨업 보상, 마법레벨

using Packet = unsigned char;

enum PACKET_PROTOCOL_TYPE
{
	// 캐릭터 정보 초기화
	INIT_CLIENT,
	INIT_OTHER_CLIENT,

	// 캐릭터 좌표 및 방향전환
	CHANGED_PLAYER_POSITION,
	CHANGED_PLAYER_DIRECTION,

	//연결끊어짐
	PLAYER_DISCONNECT
};

enum CONNECT_STATE { DISCONNECT = -1, CONNECT = 1 };
enum MONSTERS { NO_MONSTER, MONSTER01, MONSTER02, MONSTER03 };
enum PLAYERS { NO_PLAYER, LUNA, CMETRA, RONDO, DONALD };


enum OPTYPE { OP_SEND, OP_RECV };

/*
struct OverlappedEx
{
WSAOVERLAPPED over;
WSABUF wsabuf;
unsigned char IOCP_buf[MAX_BUF_SIZE];
OPTYPE event_type;
int event_target;
};
*/

struct Position
{
	float		x{ 100 };
	float		y{ 100 };
	float		z{ 50 };
	float		w{ 0.0f };
};
//12

struct Player_Status
{
	unsigned int			attack{ 50 };
	unsigned int			defend{ 50 };
	unsigned int			speed{ 10 };
};
//12

struct Player_LoginDB
{
	wchar_t name[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t password[MAX_BUFFER_SIZE / 4]{ L"Guest" };
};
// MAX_BUFFER_SIZE = 255 -> 총 크기: 252바이트

struct Player_Info
{
	//PLAYER_CLASS player_class;
	Player_Status				player_status;
	unsigned int				origin_hp{ 300 };
	unsigned int				cur_hp{ 300 };
	unsigned int				origin_mp{ 100 };
	unsigned int				cur_mp{ 100 };
	unsigned char				level{ 1 };
	unsigned short				exp{ 0 };
};
// 12 - 4 - 4 - 4 - 4 - [ 1 - 2 ] = 31

struct Player_Data
{
	Position		Pos;
	unsigned int	ID{ 0 };
	Player_Info		UserInfo;
	bool			Is_AI{ false };
	Player_LoginDB  LoginData;
	char			Dir;

};
// 12 - 4 - 31 - 1 - 252 - 1

#pragma pack (push, 1)

typedef struct Server_To_Client_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	Player_Data player_data;

}STC_ClientInit;


typedef struct Server_To_Client_Player_TO_Other_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	Player_Data player_data;

}STC_OtherClientInit;

typedef struct Server_To_Client_Player_Disconnected_Info
{
	unsigned char pack_size = sizeof(unsigned int) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_DISCONNECT;
	unsigned int id;

}STC_Disconnected;


typedef struct Server_To_Client_Player_Position_Changed
{
	unsigned char packet_size = sizeof(unsigned int) + sizeof(Position) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;
	unsigned int id;
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
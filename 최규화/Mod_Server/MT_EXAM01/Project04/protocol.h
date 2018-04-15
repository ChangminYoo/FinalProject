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

	PLAYER_DISCONNECT,		//연결끊어짐
	PLAYER_ROTATE,			//플레이어 회전
	PLAYER_ATTACK,		    //플레이어 공격
	STATIC_OBJECT,			//고정된 물체
	TEST					//테스트용 패킷
};


enum Ani_State
{
	Idle = 0,
	Run,
	Attack,
	Victory,
	Crying
};

enum STATIC_OBJECT_TYPE
{
	Map,
	Box
};

enum CONNECT_STATE { DISCONNECT = -1, CONNECT = 1 };
enum MONSTERS { NO_MONSTER, MONSTER01, MONSTER02, MONSTER03 };
enum PLAYERS { NO_PLAYER, LUNA, CMETRA, RONDO, DONALD };

struct Time
{
	float c_time{ 0.f };
	float p_time{ 0.f };
	float t_time{ 0.f };
};

struct CollisionBox
{
	float	x{ 0.0f };
	float	y{ 0.0f };
	float	z{ 0.0f };
};

struct Rotation
{
	float		x{ 0.0f };
	float		y{ 0.0f };
	float		z{ 0.0f };
	float		w{ 0.0f };
};

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
	unsigned short			speed{ 100};        //2
};
//6

struct Player_LoginDB
{
	wchar_t name[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t password[MAX_BUFFER_SIZE / 4]{ L"Guest" };
};
// MAX_BUFFER_SIZE = 255 -> 총 크기: 252바이트

struct Player_Info
{
	//PLAYER_CLASS player_class;
	Player_Status				player_status;    //6
	unsigned short				origin_hp{ 300 }; //2
	unsigned short   			cur_hp{ 300 };    //2
	unsigned short				exp{ 0 };         //2
	unsigned char				level{ 1 };		  //1 
};
// 12 + 1 = 13

struct StaticObject_Info
{
	Position					Pos;					//16
	Rotation				    Rotate_status;			//16
	CollisionBox				CollisionBox_Size;		//12
	unsigned short				origin_hp{ 100 };		//2
	unsigned short   			cur_hp{ 100 };			//2
	Player_Status				player_status;			//6
	unsigned short				ID{ 0 };				//2
	char						GodMode{ false };		//1
	char						Ani{ Ani_State::Idle }; //1
	char						Fixed{ true };

};

#pragma pack (push, 1)

struct Player_Data
{
	Player_Info		UserInfo;					//18
	unsigned short  ID{ 0 };					//2
	Position		Pos;						//16
	char			Is_AI{ false };				//1
	char			Dir;						//1
	char			Connect_Status{ false };    //1
	char			Ani{ Ani_State::Idle };     //1
	char			GodMode{ false };			//1
	Rotation		Rotate_status;		        //16
	//Player_LoginDB  LoginData;
};
// 18 + 16 + 2 + 1 + 1 + 1 + 1 + 16 = 58  pragma pack 할시
#pragma pack (pop)


#pragma pack (push, 1)		//push 시작부터 1바이트씩 데이터를 잘라 캐시에 저장한다(캐시라인 문제 때문에) - pop에서 멈춘다

typedef struct Server_To_Client_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) +  sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	Player_Data player_data;

}STC_SetMyClient;


typedef struct Server_To_Client_Player_TO_Other_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) +  sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT;
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
	unsigned char packet_size = sizeof(unsigned short) + sizeof(Position) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char);;
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;
	unsigned short id;
	unsigned char ani_state;
	Position pos;

}STC_ChangedPos;

typedef struct Server_To_Client_Player_Direction_Changed
{
	unsigned char packet_size = sizeof(Position) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_DIRECTION;
	char dir;
	Position pos;

}STC_ChangedDir;


typedef struct Server_To_Client_Player_Rotate
{
	unsigned char packet_size = sizeof(Rotation) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned short);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ROTATE;
	unsigned short id;
	Rotation rotate_status;

}STC_Rotation;

typedef struct Server_To_Client_Player_Attack
{
	unsigned char packet_size = sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ATTACK;

}STC_Attack;

typedef struct Server_To_Client_Static_Object
{
	unsigned char packet_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::STATIC_OBJECT;
	StaticObject_Info sobj_data;
	unsigned char type;

}STC_StaticObject;

typedef struct Server_To_Client_Player_Test
{
	unsigned char packet_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::TEST;
	Player_Data player_data;
	Time time;

}STC_Test;

#pragma pack (pop)
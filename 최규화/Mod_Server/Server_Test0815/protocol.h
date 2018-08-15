#pragma once

#define SERVERPORT 31400
#define MAX_PLAYER 2

#define MAX_NPC_MONSTER_TYPE 1
#define MAX_IMP_NUM 1
#define MAX_NPC_MONSTER_NUM (MAX_IMP_NUM)

#define MONSTER_IMP_SCORE 100
#define NORMAL_PLAYER_SCORE 10
#define TOP_RANKER_SCORE 30

#define DISCONNECTED 0
#define CONNECTED 1

#define MAX_BUFFER_SIZE 255

#define KEYINPUT_UP 0x0001
#define KEYINPUT_DOWN 0x0010

#define SKILL_LIGHTBULLET_COOLTIME 0.6
#define SKILL_HEAVYBULLET_COOLTIME 1.1
#define SKILL_TETRIKE_COOLTIME 25.0
#define SKILL_DICESTRIKE_COOLTIME 5.0

#define SKILL_SHIELD_OP_TIME 5.0
#define SKILL_SHIELD_COOLTIME 20.0

#define SKILL_WAVESHOCK_OP_TIME 0.0125
#define SKILL_WAVESHOCK_COOLTIME 20.0

#define Player_RespawnTime 5.0
#define RegularPacketLoadSceneExchangeTime (0.01f)
#define RegularPacketExchangeTime  (1.0 / 20.0) // 1초에 20번 패킷을 교환(morpg 형식)


//추가
//레벨업 보상, 마법레벨

using Packet = unsigned char;


enum PACKET_PROTOCOL_TYPE
{
	DRAW_STATE_BY_DEAD,
	STAGE_TIMER,
	LOAD_SCENE_SELECT_CHARACTER,
	SCENE_STATE_CHANGE,
	// 캐릭터 정보 초기화
	INIT_CLIENT,
	INIT_OTHER_CLIENT,
	INIT_NPC,

	// 캐릭터 좌표 및 방향전환
	PLAYER_MOVE,
	CHANGED_PLAYER_POSITION,
	CHANGED_PLAYER_DIRECTION,

	PLAYER_LOGIN,
	PLAYER_DISCONNECT,		//연결끊어짐
	PLAYER_ROTATE,			//플레이어 회전
	PLAYER_JUMP,
	PLAYER_ATTACK,		    //플레이어 공격
	STATIC_OBJECT,			//고정된 물체,
	RIGIDBODY_OBJECT,		//물리효과가 적용된 물체
	MOVE_OBJECT,			//움직이는 물체
	MOVE_OBJECT_NOCREATE,
	PLAYER_ANIMATION,

	PLAYER_CURR_STATE,		//플레이어의 현재 상태(모든 정보 저장)
	NPC_MONSTER_CURR_STATE, //몬스터 npc의 현재 상태(모든 정보 저장)
	NPC_MONSTER_IMP_ATTACK_STONEBULLET,
	NPC_MONSTER_ANIM,

	PLAYER_SKILL_SHIELD,
	PLAYER_SKILL_WAVESHOCK,
	PLAYER_SKILL_DICESTRIKE,
	PLAYER_SKILL_HAMMER,
	TEST					//테스트용 패킷
};

enum CHAR_MOVE { FRONT_MOVE = 1, BACK_MOVE, LEFT_MOVE, RIGHT_MOVE, JUMP };
enum CHAR_SKILL { TETRIKE = 2, DICE_STRIKE, WAVE_SHOCK, SHIELD };

enum Ani_State
{
	Idle = 0,
	Run,
	Attack,
	Dead,
	Victory,
	Crying
};

enum BULLET_TYPE
{
	protocol_LightBullet = 0,
	protocol_HeavyBullet,
	protocol_DiceBullet,
	protocol_HammerBullet,
	protocol_NpcStoneBullet
};

enum INSTALLED_OBJECT_TYPE
{
	BasicObject,
	Map,
	SecondFloor,
	Box,
	SmallWall,
	BigWall,
	NormalBuilding,
	MoveCube,
	Column,
	BrokenCart,
	Rigidbodybox
};


enum OBJECT_TYPE
{
	PLAYER,
	FIXED_OBJECT,
	BULLET,
	NPC_MONSTER_IMP
};

enum NPC_MONSTER_TYPE
{
	IMP
};

enum AFTER_COLLISION_EFFECT {EMPTY, BOOM, DAMAGE, DAMAGE_AND_BOOM};

//-------------------------------기본데이터-------------------------------//

struct Time
{
	float c_time{ 0.f };
	float p_time{ 0.f };
	float t_time{ 0.f };
};

struct Vel3f
{
	float x{ 0.f };
	float y{ 0.f };
	float z{ 0.f };
};

struct Position3D
{
	float	x{ 0.f };
	float	y{ 0.f };
	float	z{ 0.f };
};

struct Position
{
	float		x{ 100 };			           //4
	float		y{ 100 };			           //4
	float		z{ 50 };			           //4
	float		w{ 0.0f };			           //4
};


struct Rotation
{
	float		x{ 0.0f };
	float		y{ 0.0f };
	float		z{ 0.0f };
	float		w{ 0.0f };
};

// MAX_BUFFER_SIZE = 255 -> 총 크기: 252바이트
struct Player_LoginDB
{
	wchar_t name[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t password[MAX_BUFFER_SIZE / 4]{ L"Guest" };
};


//-------------------------------패킷용오브젝트데이터-------------------------------//

struct StaticObject_Info
{
	Position					Pos;										//16
	Rotation				    Rotate_status{ 0.f,0.f,0.f,1.0f };			//16
	double						degree{ 0 };								//4
	unsigned short				ID{ 0 };									//2
	unsigned short				TextureColor;
	char						Fixed{ true };								//1
	unsigned char				type;										//1
	
};

struct RigidbodyData
{
	Position       pos4f;
	Rotation	   rot4f;
	unsigned short id;
	unsigned char  type;
};


//------------------------패킷용 움직이는 오브젝트데이터----------------------------//


struct MoveObjectData_NoCreate
{
	Position	   pos4f;
	Rotation	   rot4f;
	unsigned short id;
};

struct MoveObjectData
{
	Position	   pos4f;
	Rotation	   rot4f;
	unsigned short id;
	unsigned char  texture_color;
	float          len;
	float		   n;
	char		   create_first;
};

//-------------------------------패킷용불렛데이터-------------------------------//

struct CTS_BulletObject_Info
{
	Position					pos4f;					//16
	Rotation					rot4f;					//16
	Vel3f						vel3f;					//12
	float						degree;					//4
	Position3D					endpoint;				//12
	unsigned short				master_id{ 0 };			//2
	unsigned short				my_id;					//2
	unsigned char				type;					//1
	char						alive;					//1
	char						after_coll;				//1
	unsigned short				damage;					//2
};

struct STC_BulletObject_Info
{
	Position					pos4f;					//16
	Rotation					rot4f;					//16
	Position3D					endpoint;				//12
	float						degree;					//4
	unsigned short				master_id;				//2
	unsigned short				my_id;					//2
	unsigned char				type;					//1
	char						alive;					//1
	char						after_coll;			//1
	unsigned short				damage;					//2
};

struct NPC_BulletObject_Info
{
	Position					pos4f;					//16
	Rotation					rot4f;					//16
	unsigned short				master_id;				//2
	unsigned short				my_id;					//2
	char						alive;					//1
	char						create_first;			//1
	char						after_coll;			//1
	unsigned short				damage;					//2
};


struct STC_HammerSkillInfo
{
	Position					pos4f;					//16
	Position					opp_pos4f;				//16
	Rotation					rot4f;					//16
	unsigned short				master_id;				//2
	unsigned short				my_id;					//2
	unsigned short				damage;					//2
	unsigned char				weapon_num;				//1
	char						after_coll;				//1
	char						create_first;			//1
	char						alive;					//1
	char					    headBullet;				//1  해머스킬을 시전할때 마다 생긴 첫번째 해머
};


struct CTS_HammerSkillInfo
{
	Position					pos4f;					//16
	Position					opp_pos4f;				//16
	Rotation					rot4f;					//16
	unsigned short				master_id;				//2
	unsigned short				my_id;					//2
	unsigned char				weapon_num;				//1
}; 


//-------------------클라이언트 로그인 관련데이터-----------------------------//

struct STC_ShowSelectCharacter
{
	unsigned short             my_id;
	unsigned char              sel_id;
};

struct STC_DrawState
{
	unsigned short			   my_id;
	char                       drawobj;
	char					   isTopRanker;
};

struct STC_LoginData
{
	unsigned short				my_id;
	unsigned char               texture_id;
};

struct CTS_LoginData
{
	//unsigned short			my_id;
	unsigned char				texture_id;
	char						isReady;
    //wchar_t				    name[MAX_BUFFER_SIZE / 4]{ L"Guest" };

};

//---------------------클라이언트 씬 변경에 관련된데이터------------------------//

struct STC_ChangeScene
{
	unsigned short				my_id;
	unsigned char				my_currScene;
	char						my_currSceneReady;
};

struct CTS_ChangeScene
{
	unsigned short				my_id;
	unsigned char				my_currScene;
	char						my_currSceneReady;
};


//-------------------------------패킷용스킬데이터-------------------------------//

struct STC_SkillData
{
	unsigned short master_id;   //스킬을 사용한 플레이어
	char		   my_id;		//스킬 넘버
	char		   alive;

};

//--------------------------스테이지 타이머관련 데이터------------------------------//

struct STC_StageTimer
{
	double stage_time;
};


//-------------------------------캐릭터 - NPC 데이터-------------------------------//

struct PlayerStatus
{
	unsigned short			attack{ 50 };       //2
	unsigned short			speed{ 50 };        //2
	unsigned short			origin_hp{ 100 };   //2
	unsigned short   		cur_hp{ 100 };      //2
	unsigned short			exp{ 0 };           //2
	unsigned char			level{ 1 };		    //1 
};
// 12 + 1 = 13


#pragma pack (push, 1)
struct Player_Data
{
	Position		pos;						//16
	Rotation		rot;		                //16
	PlayerStatus    status;					    //11
	unsigned short  id{ 0 };					//2
	char			ai{ false };				//1
	char			dir;						//1
	char			connect{ false };			//1
	unsigned char	ani{ Ani_State::Idle };     //1
	char			godmode{ false };			//1
	char			airbone{ false };			//1
												//Player_LoginDB  LoginData;
	char			alive{ true };				//1
	unsigned short  score;						//2
	char			killcount;					//1
	char			deathcount;					//1
	unsigned char   rank;						//1
	char			topRank;					//1
	unsigned char   respawn_cnt;			    //1
};
// 18 + 16 + 2 + 1 + 1 + 1 + 1 + 16 = 58  pragma pack 할시

struct Npc_Data
{
	Position		pos;						//16
	Rotation		rot;		                //16
	PlayerStatus    status;					    //11
	unsigned short  id{ 0 };					//2
	char			connect{ false };			//1
	unsigned char	ani{ Ani_State::Idle };     //1
	char			godmode{ false };			//1
	char			airbone{ false };			//1
	unsigned char	monster_type;				//1
	char			alive;						//1
};
#pragma pack (pop)

//-------------------------------//-------------------------------//


#pragma pack (push, 1)		//push 시작부터 1바이트씩 데이터를 잘라 캐시에 저장한다(캐시라인 문제 때문에) - pop에서 멈춘다

typedef struct Server_To_Client_StageTimer_Info
{
	unsigned char pack_size = sizeof(STC_StageTimer) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::STAGE_TIMER;
	STC_StageTimer data;

}STC_STAGE_TIMER;


//캐릭터 상태 초기화 및 업데이트에 사용한다
typedef struct Server_To_Client_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_CLIENT;
	Player_Data player_data;

}STC_SetMyClient;

typedef struct Server_To_Client_Player_TO_Other_Player_Info
{
	unsigned char pack_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT;
	Player_Data player_data;

}STC_SetOtherClient;

typedef struct Server_To_Client_NPC_Info
{
	unsigned char pack_size = sizeof(Npc_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::INIT_NPC;
	Npc_Data npc_data;

}STC_SetMyNPC;

typedef struct Server_To_Client_NPC_Anim
{
	unsigned char pack_size = sizeof(unsigned short) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::NPC_MONSTER_ANIM;
	unsigned char npc_anim;
	unsigned short id;

}STC_MyNPCAnim;

typedef struct Server_To_Client_Player_Disconnected_Info
{
	unsigned char pack_size = sizeof(unsigned short) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_DISCONNECT;
	char connect;
	unsigned short id;

}STC_Disconnected;

typedef struct Server_To_Client_Player_Position_Changed
{
	unsigned char packet_size = sizeof(unsigned short) + sizeof(Position) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(float);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;
	unsigned short id;
	unsigned char ani_state;
	float deltime;
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

typedef struct Server_To_Client_Static_Object
{
	unsigned char packet_size = sizeof(StaticObject_Info) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::STATIC_OBJECT;
	StaticObject_Info sobj_data;

}STC_StaticObject;

typedef struct Server_To_Client_Rigidbody_Object
{
	unsigned char packet_size = sizeof(RigidbodyData) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::RIGIDBODY_OBJECT;
	RigidbodyData rbobj_data;

}STC_RigidbodyObject;

typedef struct Server_To_Client_Move_Object
{
	unsigned char packet_size = sizeof(MoveObjectData) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::MOVE_OBJECT;
	MoveObjectData mvobj_data;

}STC_MoveObject;

typedef struct Server_To_Client_Move_Object_NoCreate
{
	unsigned char packet_size = sizeof(MoveObjectData_NoCreate) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::MOVE_OBJECT_NOCREATE;
	MoveObjectData_NoCreate mvobj_data;

}STC_MoveObject_NoCreate;

typedef struct Client_To_Server_Attack_Info
{
	unsigned char pack_size = sizeof(CTS_BulletObject_Info) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(float);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ATTACK;
	CTS_BulletObject_Info bull_data;
	float			      lifetime;

}CTS_Attack;

typedef struct Server_To_Client_Attack_Info
{
	unsigned char pack_size = sizeof(STC_BulletObject_Info) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ATTACK;
	STC_BulletObject_Info bull_data;
	char		  is_first;

}STC_Attack;

typedef struct Server_To_Client_CharAnimation
{
	unsigned char pack_size = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned short);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ANIMATION;
	unsigned short id;
	unsigned char ani_state;

}STC_CharAnimation;

typedef struct Server_To_Client_CharJump
{
	unsigned char packet_size = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned short);
	unsigned char packet_type = PACKET_PROTOCOL_TYPE::PLAYER_JUMP;
	unsigned short id;

}STC_CharJump;

typedef struct Server_To_Client_CharMove
{
	unsigned char packet_size = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(char) + sizeof(unsigned char) + sizeof(float);
	unsigned char packet_type = PACKET_PROTOCOL_TYPE::PLAYER_MOVE;
	char		  dir;

}STC_CharMove;

typedef struct Server_To_Client_Curr_PlayerState
{
	unsigned char packet_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char packet_type = PACKET_PROTOCOL_TYPE::PLAYER_CURR_STATE;
	Player_Data   player_data;

}STC_CharCurrState;

typedef struct Server_To_Client_Curr_NpcMonsterState
{
	unsigned char packet_size = sizeof(Npc_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char packet_type = PACKET_PROTOCOL_TYPE::NPC_MONSTER_CURR_STATE;
	Npc_Data   npc_data;

}STC_NpcMonsterCurrState;

typedef struct Server_To_Client_Curr_NpcMonsterAttackStoneBullet
{
	unsigned char packet_size = sizeof(NPC_BulletObject_Info) + sizeof(Position) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char packet_type = PACKET_PROTOCOL_TYPE::NPC_MONSTER_IMP_ATTACK_STONEBULLET;
	NPC_BulletObject_Info   npc_bulldata;
	Position                stone_rnpos;

}STC_NpcMonsterAttackStoneBullet;


typedef struct Server_To_Client_Player_Test
{
	unsigned char packet_size = sizeof(Player_Data) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::TEST;
	Player_Data player_data;
	Time time;

}STC_Test;

typedef struct Server_To_Client_Skill_Shield
{
	unsigned char packet_size = sizeof(STC_SkillData) + sizeof(double) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char packet_type = PACKET_PROTOCOL_TYPE::PLAYER_SKILL_SHIELD;
	STC_SkillData skill_data;
	double		  cooltime;

}STC_SKILL_SHIELD;

typedef struct Client_To_Server_Skill_Shield
{
	unsigned char packet_size = sizeof(STC_SkillData) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char packet_type = PACKET_PROTOCOL_TYPE::PLAYER_SKILL_SHIELD;
	STC_SkillData skill_data;
	int			  ringobjectID;

}CTS_SKILL_SHIELD;

typedef struct Server_To_Client_Skill_WaveShock
{
	unsigned char packet_size = sizeof(STC_SkillData) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char packet_type = PACKET_PROTOCOL_TYPE::PLAYER_SKILL_WAVESHOCK;
	STC_SkillData skill_data;

}STC_SKILL_WAVESHOCK;

typedef struct Client_To_Server_Skill_WaveShock
{
	unsigned char packet_size = sizeof(STC_SkillData) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char packet_type = PACKET_PROTOCOL_TYPE::PLAYER_SKILL_WAVESHOCK;
	STC_SkillData skill_data;

}CTS_SKILL_WAVESHOCK;

typedef struct Server_To_Client_DiceSkillAttack_Info
{
	unsigned char pack_size = sizeof(STC_BulletObject_Info) + sizeof(Position3D) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_SKILL_DICESTRIKE;
	STC_BulletObject_Info bull_data;
	Position3D	  lookvector;
	char		  is_first;

}STC_SKILL_DICESTRIKE;

typedef struct Client_To_Server_DiceSkillAttack_Info
{
	unsigned char pack_size = sizeof(CTS_BulletObject_Info) + sizeof(Position3D) + sizeof(unsigned char) + sizeof(unsigned char) + sizeof(char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_SKILL_DICESTRIKE;
	CTS_BulletObject_Info bull_data;
	Position3D			  lookvector;
    char				  is_firstdice;

}CTS_SKILL_DICESTRIKE;

typedef struct Client_To_Server_HammerSkill_Info
{
	unsigned char pack_size = sizeof(CTS_HammerSkillInfo) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_SKILL_HAMMER;
	CTS_HammerSkillInfo   skill_data;

}CTS_SKILL_HAMMERBULLET;

typedef struct Server_To_Client_HammerSkill_Info
{
	unsigned char pack_size = sizeof(STC_HammerSkillInfo) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_SKILL_HAMMER;
	STC_HammerSkillInfo   skill_data;

}STC_SKILL_HAMMERBULLET;

typedef struct Server_To_Client_LoginServer_Info
{
	unsigned char pack_size = sizeof(STC_LoginData) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_LOGIN;
	STC_LoginData logindata;

}STC_PLAYER_LOGIN;

typedef struct Client_To_Client_LoginServer_Info
{
	unsigned char pack_size = sizeof(CTS_LoginData) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::PLAYER_LOGIN;
	CTS_LoginData logindata;

}CTS_PLAYER_LOGIN;


typedef struct Server_To_Client_SceneState_Info
{
	unsigned char pack_size = sizeof(STC_ChangeScene) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::SCENE_STATE_CHANGE;
	STC_ChangeScene state;

}STC_CHANGE_SCENE;

typedef struct Client_To_Server_SceneState_Info
{
	unsigned char pack_size = sizeof(CTS_ChangeScene) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::SCENE_STATE_CHANGE;
	CTS_ChangeScene state;

}CTS_CHANGE_SCENE;

typedef struct Server_To_Client_ShowSelectChar_Info
{
	unsigned char pack_size = sizeof(STC_ShowSelectCharacter) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::LOAD_SCENE_SELECT_CHARACTER;
	STC_ShowSelectCharacter show_char_number;

}STC_CHAR_NUMBER_LOAD_SCENE;

typedef struct Server_To_Client_RespawnDraw
{
	unsigned char pack_size = sizeof(STC_DrawState) + sizeof(unsigned char) + sizeof(unsigned char);
	unsigned char pack_type = PACKET_PROTOCOL_TYPE::DRAW_STATE_BY_DEAD;
	STC_DrawState stc_draw_state;

}STC_RESPAWN_DRAW;

#pragma pack (pop)
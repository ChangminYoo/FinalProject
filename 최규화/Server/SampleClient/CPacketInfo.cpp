#include "CPacketInfo.h"



CPacketInfo::CPacketInfo()
{
}

void CPacketInfo::WaitRoomWhoIsSelInit()
{
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		m_waitRoomSel.id[i] = -1;
		m_waitRoomSel.selected[i] = false;
		m_waitRoomSel.buttonDownBitmap[i] = IDB_BITMAP_READY_BUTTON_F;
		m_waitRoomSel.logoBitmap[i] = IDB_BITMAP_READY_DISPLAY;
	}
}
void CPacketInfo::WaitRoomWhoIsSelInit(int id[], bool flag[], int logo[], int button[])
{
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		m_waitRoomSel.id[i] = id[i];
		m_waitRoomSel.selected[i] = flag[i];
		m_waitRoomSel.logoBitmap[i] = logo[i];
		m_waitRoomSel.buttonDownBitmap[i] = button[i];
	}

	//for (int i = 0; i < MAX_PLAYER; ++i)
	//{
	//	if (id == g_myID == i)
	//	{
	//		m_waitRoomSel.id[g_myID] = id;
	//		m_waitRoomSel.selected[g_myID] = flag;
	//		m_waitRoomSel.buttonDownBitmap[g_myID] = button;
	//		m_waitRoomSel.logoBitmap[g_myID] = logo;
	//	}
	//	else
	//	{
	//		m_waitRoomSel.id[i] = -1;
	//		m_waitRoomSel.selected[i] = false;
	//		m_waitRoomSel.buttonDownBitmap[i] = IDB_BITMAP20;
	//		m_waitRoomSel.logoBitmap[i] = IDB_BITMAP21;
	//	}
	//}

}
void CPacketInfo::WaitRoomCharInfoInit()
{
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		m_waitRoomCharInfo.charPos[i] = { -1, -1 };
		m_waitRoomCharInfo.size[i] = { -1 };
		m_waitRoomCharInfo.buttonPos[i] = { -1,-1 };
		m_waitRoomCharInfo.buttonSize[i] = { -1 };
		m_waitRoomCharInfo.myBitMapNum[i] = { IDB_BITMAP_READY_BUTTON_F };
		m_waitRoomCharInfo.joinRoom[i] = -1;
	}
}

void CPacketInfo::WaitRoomCharInfoInit(int id, Pos charPos, int charSize, Pos buttonPos, int buttonSize,
									   int bitNum, int join)
{
	m_waitRoomCharInfo.charPos[id] = charPos;
	m_waitRoomCharInfo.size[id] = charSize;
	m_waitRoomCharInfo.buttonPos[id] = buttonPos;
	m_waitRoomCharInfo.buttonSize[id] = buttonSize;
	m_waitRoomCharInfo.myBitMapNum[id] = bitNum;
	m_waitRoomCharInfo.joinRoom [id]= join;
}

void CPacketInfo::PacketInfoInit(PACK_TYPE type)
{
	m_packInfo.packet = type;
	m_iteminfo.item_pos.x = 300;
	m_iteminfo.item_pos.y = 200;
	m_iteminfo.is_alive = false;
	

	switch (type)
	{
	case SELECT_ROOM:
		m_packInfo.size = sizeof(Character_Select);
		break;
	case WAIT_ROOM:
		m_packInfo.size = sizeof(Wait_Room);
		break;
	case STAGE_TIME:
		m_packInfo.size = sizeof(TimeInfo);
		break;
	case CHARACTER_INFO:
		m_packInfo.size = sizeof(CharacterInfo);
		break;
	case OBSTACLE_CAR:
		m_packInfo.size = sizeof(CarObjectInfo);
		break;
	case OBSTACLE_LASER:
		m_packInfo.size = sizeof(ObstacleObjectInfo);
		break;
	case ITEM_CLICK:
		m_packInfo.size = sizeof(ItemObjectInfo);
		break;
	case GAME_OVER:
		m_packInfo.size = sizeof(Game_Info);
		break;
	case PACKET_INFO:
		m_packInfo.size = sizeof(PackInfo);
		break;
	default:
		break;
	}

}

void CPacketInfo::CharPacketInfoInit(long id2)
{
	m_charInfo.packType = PACK_TYPE::CHARACTER_INFO;
	if(id2==0)
		m_charInfo.char_pos = { 40, 600};
	else
		m_charInfo.char_pos = { 640, 600 };
	m_charInfo.id = id2;
	m_charInfo.life = 20;
	m_charInfo.score = 0;
	m_charInfo.isjump = false;
	if (id2 == 0) {
		m_opcharInfo.char_pos = { 640, 600 };
		m_opcharInfo.id = id2;
	}
	else {
		m_opcharInfo.char_pos = { 40, 600 };
		m_opcharInfo.id = id2;
	}
	m_opcharInfo.life = 20;
	m_opcharInfo.score = 0;
	m_opcharInfo.isjump = false;

}

void CPacketInfo::CharPacketInfoInit(Pos pos, int id, int life, int score)
{
	m_charInfo.packType = PACK_TYPE::CHARACTER_INFO;
	m_charInfo.char_pos = pos;
	m_charInfo.id = id;
	m_charInfo.life = life;
	m_charInfo.score = score;
}


void CPacketInfo::WaitRoomPacketInfoInit(Pos pos, int resource, int id, bool ready, bool start)
{
	m_waitInfo.packType = PACK_TYPE::WAIT_ROOM;
	m_waitInfo.char_pos = pos;
	m_waitInfo.resource_type = resource;
	m_waitInfo.id = id;
	m_waitInfo.game_ready = ready;
	m_waitInfo.game_start = start;
	
		m_opcharInfo.char_pos = { 640, 600 };
		
	
	
		m_opcharInfo.char_pos = { 40, 600 };
		
	
	m_opcharInfo.life = 20;
	m_opcharInfo.score = 0;
	m_opcharInfo.isjump = false;
}

void CPacketInfo::SelectPacketInfoInit(long id, bool start)
{
	//m_selInfo.packType = PACK_TYPE::SELECT_ROOM;
	m_selInfo.id = id;
	m_selInfo.go_to_wr = htons(static_cast<short>(start));
}
void CPacketInfo::GameScenePacketInfoInit()
{

}
CPacketInfo::~CPacketInfo()
{
}

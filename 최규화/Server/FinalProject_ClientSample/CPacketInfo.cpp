#include "CPacketInfo.h"



CPacketInfo::CPacketInfo()
{
}

void CPacketInfo::CharPacketInfoInit(long id2)
{
	if(id2==0)
		m_charInfo.char_pos = { 40, 600};
	else
		m_charInfo.char_pos = { 640, 600 };

	m_charInfo.id = id2;

	if (id2 == 0) 
	{
		m_opcharInfo.char_pos = { 640, 600 };
		m_opcharInfo.id = id2;
	}
	else 
	{
		m_opcharInfo.char_pos = { 40, 600 };
		m_opcharInfo.id = id2;
	}
}

void CPacketInfo::CharPacketInfoInit(Pos pos, int id, int life, int score)
{
	m_charInfo.char_pos = pos;
	m_charInfo.id = id;
}

CPacketInfo::~CPacketInfo()
{
}

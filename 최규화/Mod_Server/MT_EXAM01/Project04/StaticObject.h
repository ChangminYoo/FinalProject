#pragma once
#include "Player_Session.h"
#include <unordered_map>

class StaticObject : public Player_Session
{
private:
	int m_soID{ -1 };
	unordered_map<int, Position> m_boxPos;

public:
	StaticObject();

	void SET_PosOfBox();

	void InitStaticObject();

	void SendStaticObject();

	~StaticObject();
};


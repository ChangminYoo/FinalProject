#include "StaticObject.h"



StaticObject::StaticObject()
{
}

void StaticObject::SET_PosOfBox()
{
	m_boxPos = 
	{
		{0, {-20.f, 0.f, 0.f, 0.f}},
		{1, {-40.f, 10.f, 0.f, 0.f}},
		{2, {50.f, 0.f, -40.f, 0.f}},
		{3, {30.f, 0.f, 40.f, 0.f}}
	};
}

void StaticObject::InitStaticObject()
{
	//1. StaticObjectList�� ������ �־��ֱ�.
	//   �ѹ� �����ϰ� �ٲ��� �����Ƿ� �ʱ� �ѹ��� ����

	SET_PosOfBox();

	StaticObject *sobj = new StaticObject();

	sobj->m_connect_state = true;
	sobj->m_state = PLAYER_STATE::IDLE;
	sobj->m_playerData.Ani = Ani_State::Idle;
	sobj->m_playerData.Connect_Status = true;
	sobj->m_playerData.Dir = 0;
	sobj->m_playerData.GodMode = true;
	sobj->m_playerData.Is_AI = false;
	sobj->m_playerData.Rotate_status = { 0.0f, 0.0f, 0.0f, 0.0f };
	sobj->m_playerData.UserInfo.cur_hp = 100;
	sobj->m_playerData.UserInfo.origin_hp = 100;
	sobj->m_playerData.UserInfo.player_status.attack = 0;
	sobj->m_playerData.UserInfo.player_status.speed = 0;
	sobj->m_playerType = PLAYERS::NO_PLAYER;
	sobj->m_monsterType = MONSTERS::NO_MONSTER;
	sobj->staticobject = true;
	sobj->OffLookvector = XMFLOAT3(0, 0, 1);
	sobj->OffRightvector = XMFLOAT3(1, 0, 0);
	
	sobj->UpdateLookVector();
	sobj->GetUpvector();

	for (int i = 0; i < MAX_BOX_NUM; ++i)
	{
		sobj->m_id = i;
		sobj->m_playerData.ID = i;
		sobj->m_playerData.Pos = move(m_boxPos[i]);

		m_staticobjs.emplace_back(sobj);
	}
	
}

void StaticObject::SendStaticObject()
{
	//1. ���� staticobject�� �ʱ�ȭ�ϴ� �κ�. 1000�� �̻��� staticobject�� ������� �ʴ´ٸ� 
	//   ���� �� ��Ŷ�� �������� �����͸� ��Ƽ� �Ѳ����� ���� �ʿ������ ����
	//   ���� ������ ���� ���ߵǴ°� �ƴ϶� �ʱ�ȭ �� �ѹ��� ���� ���̹Ƿ� . . .

	m_soID = m_id;
	STC_StaticObject stc_sobj;

	//1. ���ڿ� ���� �����͸� ����
	for (auto iter = m_staticobjs.begin(); iter != m_staticobjs.end(); ++iter)
	{
		stc_sobj.player_data = move((*iter)->GetPlayerData());
		stc_sobj.type = STATIC_OBJECT_TYPE::Box;

		m_clients[m_soID]->SendPacket(reinterpret_cast<Packet*>(&stc_sobj));
	}
	
}


StaticObject::~StaticObject()
{
}

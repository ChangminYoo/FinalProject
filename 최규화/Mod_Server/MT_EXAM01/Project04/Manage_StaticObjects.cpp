#include "Player_Session.h"

void Player_Session::SendStaticObjects(const unordered_set<StaticObject*>& sobjudset)
{
	//1. ���� staticobject�� �ʱ�ȭ�ϴ� �κ�. 1000�� �̻��� staticobject�� ������� �ʴ´ٸ� 
	//   ���� �� ��Ŷ�� �������� �����͸� ��Ƽ� �Ѳ����� ���� �ʿ������ ����
	//   ���� ������ ���� ���ߵǴ°� �ƴ϶� �ʱ�ȭ �� �ѹ��� ���� ���̹Ƿ� . . .
	STC_StaticObject stc_sobj;
	stc_sobj.packet_size = sizeof(STC_StaticObject);
	stc_sobj.pack_type = PACKET_PROTOCOL_TYPE::STATIC_OBJECT;

	//1. ���ڿ� ���� �����͸� ����
	for (auto iter = sobjudset.begin(); iter != sobjudset.end(); ++iter)
	{
		stc_sobj.sobj_data = move((*iter)->GetInfoOfSObj());
		stc_sobj.type = STATIC_OBJECT_TYPE::Box;

		m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&stc_sobj));
	}

}

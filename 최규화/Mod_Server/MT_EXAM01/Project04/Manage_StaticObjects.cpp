#include "Player_Session.h"

void Player_Session::SendStaticObjects(const unordered_set<StaticObject*>& sobjudset)
{
	//1. ���� staticobject�� �ʱ�ȭ�ϴ� �κ�. 1000�� �̻��� staticobject�� ������� �ʴ´ٸ� 
	//   ���� �� ��Ŷ�� �������� �����͸� ��Ƽ� �Ѳ����� ���� �ʿ������ ����
	//   ���� ������ ���� ���ߵǴ°� �ƴ϶� �ʱ�ȭ �� �ѹ��� ���� ���̹Ƿ� . . .
	Player_Session::m_sobjs = move(sobjudset);

	STC_StaticObject stc_sobj;

	//1. ���ڿ� ���� �����͸� ����
	for (auto iter = sobjudset.begin(); iter != sobjudset.end(); ++iter)
	{	
	    stc_sobj.sobj_data = move((*iter)->GetInfoOfSObj());	
		m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&stc_sobj));
	}

}

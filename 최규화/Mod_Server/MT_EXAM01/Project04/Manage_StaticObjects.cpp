#include "Player_Session.h"

void Player_Session::SendStaticObjects(const unordered_set<StaticObject*>& sobjudset)
{
	//1. 각종 staticobject를 초기화하는 부분. 1000개 이상의 staticobject를 사용하지 않는다면 
	//   굳이 한 패킷에 여러개의 데이터를 모아서 한꺼번에 보낼 필요까지는 없다
	//   또한 여러번 오고 가야되는게 아니라 초기화 때 한번만 해줄 것이므로 . . .
	Player_Session::m_sobjs = move(sobjudset);

	STC_StaticObject stc_sobj;

	//1. 상자에 대한 데이터를 보냄
	for (auto iter = sobjudset.begin(); iter != sobjudset.end(); ++iter)
	{	
	    stc_sobj.sobj_data = move((*iter)->GetInfoOfSObj());	
		m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&stc_sobj));
	}

}

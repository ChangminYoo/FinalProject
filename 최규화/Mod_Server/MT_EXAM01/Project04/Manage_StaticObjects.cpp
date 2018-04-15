#include "Player_Session.h"

/*void Player_Session::SET_PosOfBox()
{
	m_boxPos =
	{
		{ 0,{ -20.f, 0.f, 0.f, 0.f } },
		{ 1,{ -40.f, 10.f, 0.f, 0.f } },
		{ 2,{ 50.f, 0.f, -40.f, 0.f } },
		{ 3,{ 30.f, 0.f, 40.f, 0.f } }
	};
}
*/

//void Player_Session::InitStaticObjects(boost::asio::ip::tcp::socket my_sock)
/*
void Player_Session::InitStaticObjects()
{
	//1. StaticObjectList에 데이터 넣어주기.
	//   한번 생성하고 바뀌지 않으므로 초기 한번만 실행
	SET_PosOfBox();

	//현재 접속한 내 클라이언트 아이디와 소켓정보가 들어간다

	for (int i = 0; i < MAX_BOX_NUM; ++i)
	{
		Player_Session *sobj = new Player_Session(i, move(m_socket));

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

		sobj->m_id = i;
		sobj->m_playerData.ID = i;
		sobj->m_playerData.Pos = move(m_boxPos[i]);

		m_staticobjs.emplace_back(sobj);
	}

	//delete sobj;
}
*/


void Player_Session::SendStaticObjects(const list<StaticObject*>& SObjList)
{
	//1. 각종 staticobject를 초기화하는 부분. 1000개 이상의 staticobject를 사용하지 않는다면 
	//   굳이 한 패킷에 여러개의 데이터를 모아서 한꺼번에 보낼 필요까지는 없다
	//   또한 여러번 오고 가야되는게 아니라 초기화 때 한번만 해줄 것이므로 . . .
	STC_StaticObject stc_sobj;
	stc_sobj.packet_size = sizeof(STC_StaticObject);
	stc_sobj.pack_type = PACKET_PROTOCOL_TYPE::STATIC_OBJECT;

	//1. 상자에 대한 데이터를 보냄
	for (auto iter = SObjList.begin(); iter != SObjList.end(); ++iter)
	{
		stc_sobj.sobj_data = move((*iter)->GET_InfoOfSObjs());
		stc_sobj.type = STATIC_OBJECT_TYPE::Box;

		//cout << "socket: " << reinterpret_cast<int*>(&((*iter)->m_socket)) << endl;
		
		//cout << "socket : " << reinterpret_cast<int>(&m_clients[m_id]->m_socket) << endl;
		m_clients[m_id]->SendPacket(reinterpret_cast<Packet*>(&stc_sobj));
	}

}

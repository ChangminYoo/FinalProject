#include "AsyncClient.h"
#include "..\..\Scene.h"

void AsyncClient::ProcessPacket(Packet* packet, CGameObject& obj, Scene& scene)
{
	
switch (packet[1])
{
	case PACKET_PROTOCOL_TYPE::INIT_CLIENT:
	{
		// 0번 1번 2번 캐릭터

		//서버로부터 받은 새로운 데이터
		auto init_data = reinterpret_cast<STC_SetMyClient*>(packet);

		Player_Data new_initdata;
		//wcscpy(new_initdata.LoginData.name, init_data->player_data.LoginData.name);
		//wcscpy(new_initdata.LoginData.password, init_data->player_data.LoginData.password);
		
		new_initdata = move(init_data->player_data);
		scene.my_ClientID = init_data->player_data.ID;
		scene.Set_PlayerServerData(new_initdata.ID, new_initdata);

		break;
	}
	case PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT:
	{
		auto init_otherdata = reinterpret_cast<STC_SetOtherClient*>(packet);

		Player_Data new_initdata_other;
		new_initdata_other = move(init_otherdata->player_data);

		scene.Set_PlayerServerData(new_initdata_other.ID, new_initdata_other);

	}
		break;
	case PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION:
	{
		auto move_data = reinterpret_cast<STC_ChangedPos*>(packet);
		Player_Data new_movedata;

		new_movedata.Pos = move(move_data->pos);
		new_movedata.ID = move(move_data->id);
		new_movedata.Ani = move(move_data->ani_state);

		scene.Set_PlayerServerData(new_movedata.ID, new_movedata);

	}
		break;

	case PACKET_PROTOCOL_TYPE::PLAYER_DISCONNECT:
	{
		auto disc_data = reinterpret_cast<STC_Disconnected*>(packet);

		Player_Data new_discdata;
		new_discdata.Connect_Status = disc_data->connect;
		new_discdata.ID = disc_data->id;

		scene.Set_RemovePlayerData(new_discdata.ID, new_discdata);
	}

	break;
}
	

}


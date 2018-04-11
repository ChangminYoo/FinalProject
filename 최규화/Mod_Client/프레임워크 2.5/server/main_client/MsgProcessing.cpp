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
		
		scene.my_ClientID = init_data->player_data.ID;

		//new_initdata = move(init_data->player_data);

		new_initdata.Ani = init_data->player_data.Ani;
		new_initdata.Connect_Status = init_data->player_data.Connect_Status;
		new_initdata.Dir = init_data->player_data.Dir;
		new_initdata.ID = init_data->player_data.ID;
		new_initdata.Is_AI = init_data->player_data.Is_AI;
		new_initdata.Pos = move(init_data->player_data.Pos);
		new_initdata.UserInfo = move(init_data->player_data.UserInfo);


		scene.SET_PLAYER_BY_SEVER_DATA(init_data->player_data.ID, new_initdata, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT:
	{
		auto init_otherdata = reinterpret_cast<STC_SetOtherClient*>(packet);

		Player_Data new_initdata_other;
		new_initdata_other = move(init_otherdata->player_data);

		scene.SET_PLAYER_BY_SEVER_DATA(init_otherdata->player_data.ID, new_initdata_other, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION:
	{
		auto move_data = reinterpret_cast<STC_ChangedPos*>(packet);
		Player_Data new_movedata;

		new_movedata.Pos = move_data->pos;
		new_movedata.Ani = move_data->ani_state;
		//new_movedata.Rotate_status = move(move_data->rotate);

		scene.SET_PLAYER_BY_SEVER_DATA(move_data->id, new_movedata, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_DISCONNECT:
	{
		auto disc_data = reinterpret_cast<STC_Disconnected*>(packet);

		Player_Data new_discdata;
		new_discdata.Connect_Status = disc_data->connect;
		new_discdata.ID = disc_data->id;

		scene.SET_PLAYER_BY_SEVER_DATA(disc_data->id, new_discdata, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ROTATE:
	{
		auto rot_data = reinterpret_cast<STC_Rotation*>(packet);

		Player_Data new_rotdata;
		new_rotdata.Rotate_status = move(rot_data->rotate_status);

		scene.SET_PLAYER_BY_SEVER_DATA(rot_data->id, new_rotdata, packet[1]);
	}

	break;

	case PACKET_PROTOCOL_TYPE::STATIC_OBJECT:
	{
		auto sobj_data = reinterpret_cast<STC_StaticObject*>(packet);

		Player_Data new_sobjdata;
		new_sobjdata = move(sobj_data->player_data);
		
		scene.SET_SOBJECT_BY_SERVER_DATA(new_sobjdata.ID, new_sobjdata, sobj_data->type);
	}

	break;
}
	

}

void AsyncClient::SendPacketRegular(CGameObject * obj)
{
	//클라이언트에서 매 프레임마다 정기적으로 보내줘야할 데이터 패킷
	obj->Lookvector;
	obj->Rightvector;
}


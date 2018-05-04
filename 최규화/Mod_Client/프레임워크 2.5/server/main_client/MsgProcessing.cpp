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

		StaticObject_Info new_sobjdata;
		new_sobjdata = move(sobj_data->sobj_data);
		
		scene.SET_SOBJECT_BY_SERVER_DATA(new_sobjdata.ID, new_sobjdata, sobj_data->type);
	}

	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ATTACK:
	{
		//내가 공격키를 누르면 나한테는 불렛이 생성되지만 상대방에게는 생성되지 않는다.
		//생성정보를 서버에서 받아와도 클라이언트에 불렛이 없으므로 해당 정보를 이용해 만들어줘야한다(처음에만)

		auto mybulldata = reinterpret_cast<STC_Attack*>(packet);

		scene.SET_BULLET_BY_SERVER_DATA(mybulldata->bull_data, mybulldata->bull_data.type);
	}
	break;
}
	

}

void AsyncClient::SendPacketRegular(CGameObject& gobj, const GameTimer& gt)
{
	//클라이언트에서 매 프레임마다 정기적으로 보내줘야할 데이터 패킷
	m_totalTime += gt.DeltaTime();

	// 1.서버에서 담고있는 캐릭터회전정보를 주기적으로 0.2초마다 검사 및 패킷송신
	RgCkInfo.RtCheck.t.t_time += gt.DeltaTime();
	if (RgCkInfo.RtCheck.t.t_time > RegularPacketExchangeTime)
	{
		//
		//STC_Test cts_test;
		//cts_test.packet_size = sizeof(STC_Test);
		//cts_test.pack_type = PACKET_PROTOCOL_TYPE::TEST;
		//cts_test.time.t_time = RgCkInfo.RtCheck.t.t_time;
		//

		RgCkInfo.RtCheck.t.p_time = RgCkInfo.RtCheck.t.t_time - RegularPacketExchangeTime;

		RgCkInfo.RtCheck.t.t_time = 0.f;
		RgCkInfo.RtCheck.t.t_time += RgCkInfo.RtCheck.t.p_time;

		//
		//cts_test.time.p_time = RgCkInfo.RtCheck.t.p_time;
		//

		RgCkInfo.RtCheck.t.p_time = 0.f;

		if (gobj.m_player_data.Rotate_status.x != RgCkInfo.RtCheck.RotationInfo.x ||
			gobj.m_player_data.Rotate_status.y != RgCkInfo.RtCheck.RotationInfo.y ||
			gobj.m_player_data.Rotate_status.z != RgCkInfo.RtCheck.RotationInfo.z ||
			gobj.m_player_data.Rotate_status.w != RgCkInfo.RtCheck.RotationInfo.w)
		{

			STC_Rotation cts_rot;
			cts_rot.packet_size = sizeof(STC_Rotation);
			cts_rot.pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ROTATE;

			cts_rot.id = gobj.m_player_data.ID;
			cts_rot.rotate_status = { RgCkInfo.RtCheck.RotationInfo.x,  RgCkInfo.RtCheck.RotationInfo.y,
									  RgCkInfo.RtCheck.RotationInfo.z, RgCkInfo.RtCheck.RotationInfo.w };

			//cts_test.player_data.ID = cts_rot.id;
			//cts_test.player_data.Rotate_status = cts_rot.rotate_status;

			//SendPacket(reinterpret_cast<Packet*>(&cts_test));

			SendPacket(reinterpret_cast<Packet*>(&cts_rot));

		}

	}

	// 2.서버에서 담고있는 캐릭터위치정보를 주기적으로 0.2초마다 검사 및 패킷송신
	RgCkInfo.PtCheck.t.t_time += gt.DeltaTime();
	if (RgCkInfo.PtCheck.t.t_time > RegularPacketExchangeTime)
	{
		//
		//STC_Test cts_test;
		//cts_test.packet_size = sizeof(STC_Test);
		//cts_test.pack_type = PACKET_PROTOCOL_TYPE::TEST;

		//cts_test.time.t_time = RgCkInfo.PtCheck.t.t_time;
		//

		RgCkInfo.PtCheck.t.p_time = RgCkInfo.PtCheck.t.t_time - RegularPacketExchangeTime;

		RgCkInfo.PtCheck.t.t_time = 0.f;
		RgCkInfo.PtCheck.t.t_time += RgCkInfo.PtCheck.t.p_time;

		//
		//cts_test.time.p_time = RgCkInfo.PtCheck.t.p_time;
		//

		RgCkInfo.PtCheck.t.p_time = 0.f;

		//SendPacket(reinterpret_cast<Packet*>(&cts_test));
	
		if (gobj.m_player_data.Pos.x != RgCkInfo.PtCheck.PositionInfo.x ||
			gobj.m_player_data.Pos.y != RgCkInfo.PtCheck.PositionInfo.y ||
			gobj.m_player_data.Pos.z != RgCkInfo.PtCheck.PositionInfo.z ||
			gobj.m_player_data.Pos.w != RgCkInfo.PtCheck.PositionInfo.w ||
			gobj.m_player_data.Ani != RgCkInfo.PtCheck.AniState)
		{
			STC_ChangedPos cts_pos;
			cts_pos.packet_size = sizeof(STC_ChangedPos);
			cts_pos.pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;

			cts_pos.id = gobj.m_player_data.ID;
			cts_pos.pos = { RgCkInfo.PtCheck.PositionInfo.x, RgCkInfo.PtCheck.PositionInfo.y,
							RgCkInfo.PtCheck.PositionInfo.z, RgCkInfo.PtCheck.PositionInfo.w };
			cts_pos.ani_state = RgCkInfo.PtCheck.AniState;
			cts_pos.deltime = RgCkInfo.PtCheck.Deltime;

			//
			//cts_test.player_data.ID = cts_pos.id;
			//cts_test.player_data.Pos = cts_pos.pos;
			//SendPacket(reinterpret_cast<Packet*>(&cts_test));

			SendPacket(reinterpret_cast<Packet*>(&cts_pos));


		}
		
		
	}
	//obj->Lookvector;
	//obj->Rightvector;
}


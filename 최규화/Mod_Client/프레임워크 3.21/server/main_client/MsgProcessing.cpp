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

		//Player_Data new_initdata;
		//wcscpy(new_initdata.LoginData.name, init_data->player_data.LoginData.name);
		//wcscpy(new_initdata.LoginData.password, init_data->player_data.LoginData.password);
		
		scene.my_ClientID = init_data->player_data.id;

		//RgCkInfo.RtCheck.RotationInfo = move(init_data->player_data.rot);
		//RgCkInfo.PtCheck.PositionInfo = move(init_data->player_data.pos);

		RgCkInfo.AniCheck.AniState = static_cast<Ani_State>(init_data->player_data.ani);

		scene.SET_PLAYER_BY_SEVER_DATA(init_data->player_data.id, init_data->player_data, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::INIT_OTHER_CLIENT:
	{
		auto init_otherdata = reinterpret_cast<STC_SetOtherClient*>(packet);

		scene.SET_PLAYER_BY_SEVER_DATA(init_otherdata->player_data.id, init_otherdata->player_data, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::INIT_NPC:
	{
		auto init_npcdata = reinterpret_cast<STC_SetMyNPC*>(packet);

		scene.SET_NPC_BY_SERVER_DATA(init_npcdata->npc_data.id, init_npcdata->npc_data, init_npcdata->npc_data.monster_type, packet[1]);
	}
	break;
	
	case PACKET_PROTOCOL_TYPE::PLAYER_DISCONNECT:
	{
		auto disc_data = reinterpret_cast<STC_Disconnected*>(packet);

		Player_Data new_discdata;
		new_discdata.connect = disc_data->connect;
		new_discdata.id = disc_data->id;

		scene.SET_PLAYER_BY_SEVER_DATA(disc_data->id, new_discdata, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ROTATE:
	{
		auto rot_data = reinterpret_cast<STC_Rotation*>(packet);

		Player_Data new_rotdata;
		new_rotdata.rot = move(rot_data->rotate_status);

		scene.SET_PLAYER_BY_SEVER_DATA(rot_data->id, new_rotdata, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::STATIC_OBJECT:
	{
		auto mysobjdata = reinterpret_cast<STC_StaticObject*>(packet);

		scene.SET_SOBJECT_BY_SERVER_DATA(mysobjdata->sobj_data.ID, mysobjdata->sobj_data, mysobjdata->sobj_data.type);
	}
	break;

	case PACKET_PROTOCOL_TYPE::RIGIDBODY_OBJECT:
	{
		auto myrbobjdata = reinterpret_cast<STC_RigidbodyObject*>(packet);

		scene.SET_RIGIDOBJECT_BY_SERVER_DATA(myrbobjdata->rbobj_data.id, myrbobjdata->rbobj_data, myrbobjdata->rbobj_data.type);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ATTACK:
	{
		//내가 공격키를 누르면 나한테는 불렛이 생성되지만 상대방에게는 생성되지 않는다.
		//생성정보를 서버에서 받아와도 클라이언트에 불렛이 없으므로 해당 정보를 이용해 만들어줘야한다(처음에만)

		auto mybulldata = reinterpret_cast<STC_Attack*>(packet);

		scene.SET_BULLET_BY_SERVER_DATA(mybulldata->bull_data, mybulldata->bull_data.type, mybulldata->is_first);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_SKILL_SHIELD:
	{
		auto myskilldata = reinterpret_cast<STC_SKILL_SHIELD*>(packet);

		scene.SET_PLAYER_SKILL(myskilldata->skill_data.master_id, myskilldata->skill_data);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_SKILL_WAVESHOCK:
	{
		auto myskilldata = reinterpret_cast<STC_SKILL_WAVESHOCK*>(packet);

		scene.SET_PLAYER_SKILL(myskilldata->skill_data.master_id, myskilldata->skill_data, myskilldata->texture_number);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_SKILL_DICESTRIKE:
	{
		auto myskilldata = reinterpret_cast<STC_SKILL_DICESTRIKE*>(packet);

		XMFLOAT3 xmf3_offlookvector{ myskilldata->lookvector.x, myskilldata->lookvector.y, myskilldata->lookvector.z };
		scene.SET_BULLET_BY_SERVER_DATA(myskilldata->bull_data, myskilldata->bull_data.type, myskilldata->is_first, xmf3_offlookvector);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_ANIMATION:
	{
		auto mycharani = reinterpret_cast<STC_CharAnimation*>(packet);
		
		Player_Data new_anidata;
		new_anidata.ani = mycharani->ani_state;
	
		RgCkInfo.AniCheck.AniState = static_cast<Ani_State>(mycharani->ani_state);

		scene.SET_PLAYER_BY_SEVER_DATA(mycharani->id, new_anidata, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::PLAYER_CURR_STATE:
	{
		auto my_curr_state = reinterpret_cast<STC_CharCurrState*>(packet);

		scene.SET_PLAYER_BY_SEVER_DATA(my_curr_state->player_data.id, my_curr_state->player_data, packet[1]);
	}
	break;

	case PACKET_PROTOCOL_TYPE::NPC_MONSTER_CURR_STATE:
	{
		auto npc_curr_state = reinterpret_cast<STC_NpcMonsterCurrState*>(packet);
		
		scene.SET_NPC_BY_SERVER_DATA(npc_curr_state->npc_data.id, npc_curr_state->npc_data, npc_curr_state->npc_data.monster_type, packet[1]);
	}
	break;
}
	

}

void AsyncClient::SendPacketRegular(CGameObject& gobj, const GameTimer& gt)
{
	//클라이언트에서 매 프레임마다 정기적으로 보내줘야할 데이터 패킷

	m_totalTime += gt.DeltaTime();

	//클라이언트에서 플레이어의 공격 모션이 끝났을 때 바로 IDLE 모션을 서버로 전송해야한다
	if (m_start_attack & gobj.m_end_attack)
	{
		STC_CharAnimation cts_cani;
		cts_cani.id = gobj.m_player_data.id;
		cts_cani.ani_state = Ani_State::Idle;

		SendPacket(reinterpret_cast<Packet*>(&cts_cani));
		gobj.m_end_attack = false;
		m_start_attack = false;

	}
	

	//플레이어가 죽는 모션을 끝마쳤을 때
	if (gobj.m_end_die)
	{

	}


	// 1.서버에서 담고있는 캐릭터회전정보를 주기적으로 0.2초마다 검사 및 패킷송신
	RgCkInfo.AniCheck.t.t_time += gt.DeltaTime();

	if (RgCkInfo.AniCheck.t.t_time > RegularPacketExchangeTime)
	{
		RgCkInfo.AniCheck.t.p_time = RgCkInfo.AniCheck.t.t_time - RegularPacketExchangeTime;

		RgCkInfo.AniCheck.t.t_time = 0.f;
		RgCkInfo.AniCheck.t.t_time += RgCkInfo.AniCheck.t.p_time;

		RgCkInfo.AniCheck.t.p_time = 0.f;

		if (RgCkInfo.AniCheck.AniState != gobj.m_player_data.ani)
		{
			STC_CharAnimation cts_c_ani;

			cts_c_ani.id = gobj.m_player_data.id;
			cts_c_ani.ani_state = RgCkInfo.AniCheck.AniState;

			//SendPacket(reinterpret_cast<Packet*>(&cts_c_ani));
		}

	}
	
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

		if (gobj.m_player_data.rot.x != RgCkInfo.RtCheck.RotationInfo.x ||
			gobj.m_player_data.rot.y != RgCkInfo.RtCheck.RotationInfo.y ||
			gobj.m_player_data.rot.z != RgCkInfo.RtCheck.RotationInfo.z ||
			gobj.m_player_data.rot.w != RgCkInfo.RtCheck.RotationInfo.w)
		{

			STC_Rotation cts_rot;
			cts_rot.packet_size = sizeof(STC_Rotation);
			cts_rot.pack_type = PACKET_PROTOCOL_TYPE::PLAYER_ROTATE;

			cts_rot.id = gobj.m_player_data.id;
		
			cts_rot.rotate_status = { RgCkInfo.RtCheck.RotationInfo.x,  RgCkInfo.RtCheck.RotationInfo.y,
									  RgCkInfo.RtCheck.RotationInfo.z, RgCkInfo.RtCheck.RotationInfo.w };

			gobj.m_player_data.rot = RgCkInfo.RtCheck.RotationInfo;
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
	
		if (gobj.m_player_data.pos.x != RgCkInfo.PtCheck.PositionInfo.x ||
			gobj.m_player_data.pos.y != RgCkInfo.PtCheck.PositionInfo.y ||
			gobj.m_player_data.pos.z != RgCkInfo.PtCheck.PositionInfo.z ||
			gobj.m_player_data.pos.w != RgCkInfo.PtCheck.PositionInfo.w ||
			gobj.m_player_data.ani   != RgCkInfo.PtCheck.AniState         )
		{
			//STC_ChangedPos cts_pos;
			//cts_pos.packet_size = sizeof(STC_ChangedPos);
			//cts_pos.pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;
			//
			//cts_pos.id = gobj.m_player_data.id;
			//cts_pos.pos = { RgCkInfo.PtCheck.PositionInfo.x, RgCkInfo.PtCheck.PositionInfo.y,
			//				RgCkInfo.PtCheck.PositionInfo.z, RgCkInfo.PtCheck.PositionInfo.w };
			//cts_pos.ani_state = RgCkInfo.PtCheck.AniState;
			//cts_pos.deltime = RgCkInfo.PtCheck.Deltime;

			//
			//cts_test.player_data.ID = cts_pos.id;
			//cts_test.player_data.Pos = cts_pos.pos;
			//SendPacket(reinterpret_cast<Packet*>(&cts_test));

			//SendPacket(reinterpret_cast<Packet*>(&cts_pos));


		}
		
		
	}
	//obj->Lookvector;
	//obj->Rightvector;
}


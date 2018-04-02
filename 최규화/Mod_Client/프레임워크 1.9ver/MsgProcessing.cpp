#include "server\main_client\AsyncClient.h"
#include "../../Scene.h"

void AsyncClient::ProcessPacket(Packet* packet, CGameObject& obj, Scene& scene)
{
	
	switch (packet[1])
	{
	case PACKET_PROTOCOL_TYPE::INIT_CLIENT:
	{
		// 0번 1번 2번 캐릭터

		//서버로부터 받은 새로운 데이터
		auto init_data = reinterpret_cast<STC_ClientInit*>(packet);

		//서버의 데이터를 이용해 클라이언트의 플레이어 정보를 갱신해준다
		//Scene -> DynamicObject -> CGameObject 의 m_PlayerData를 갱신해준다.

		//서버에서 플레이어의 초기화정보를 넘겨주므로 모든 PlayerData 값을 변경해야한다
		//서버쪽은 ID 0 ~ 4까지가 몬스터 -- 클라쪽은 ID 0 ~ 2까지가 플레이어 
		//즉 서버ID - 몬스터아이디를 클라쪽 ID에 집어넣어야함
		//auto data = scene.Get_PlayerServerData(init_data->player_data.ID);
		// GameObject = scene.Get_GameObject;

		Player_Data new_initdata;
		wcscpy(new_initdata.LoginData.name, init_data->player_data.LoginData.name);
		wcscpy(new_initdata.LoginData.password, init_data->player_data.LoginData.password);

		new_initdata.Dir = init_data->player_data.Dir;
		new_initdata.ID = init_data->player_data.ID; //- MAX_MONSTER_NUM;
		new_initdata.Is_AI = init_data->player_data.Is_AI;
		new_initdata.Pos = move(init_data->player_data.Pos);

		new_initdata.UserInfo = move(init_data->player_data.UserInfo);
		new_initdata.UserInfo.player_status = move(init_data->player_data.UserInfo.player_status);

		scene.Set_PlayerServerData(new_initdata.ID, new_initdata);
		//data->UserInfo.cur_hp				= init_data->player_data.UserInfo.cur_hp;
		//data->UserInfo.cur_mp				= init_data->player_data.UserInfo.cur_mp;
		//data->UserInfo.exp					= init_data->player_data.UserInfo.exp;
		//data->UserInfo.level				= init_data->player_data.UserInfo.level;
		//data->UserInfo.origin_hp			= init_data->player_data.UserInfo.origin_hp;
		//data->UserInfo.origin_mp			= init_data->player_data.UserInfo.origin_mp;
		//data->UserInfo.player_status.attack = init_data->player_data.UserInfo.player_status.attack;
		//data->UserInfo.player_status.defend = init_data->player_data.UserInfo.player_status.defend;
		//data->UserInfo.player_status.speed  = init_data->player_data.UserInfo.player_status.speed;


		//GameObject->CenterPos = { data->Pos.x , data->Pos.y , data->Pos.z , data->Pos.w };
		//GameObject->Speed = delivered_data->player_data.UserInfo.player_status.speed;

		//캐릭터 스테이터스 초기화값 서버->클라. 클라에서 추가해줘야됌
			
		break;
	}
	case PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION:
	{
		auto delivered_data = reinterpret_cast<STC_ChangedPos*>(packet);
		//obj->CenterPos.x = updated_data->pos.x;
		//obj->CenterPos.y = updated_data->pos.y;
		//obj->CenterPos.z = updated_data->pos.z;

		auto data = scene.Get_PlayerServerData(delivered_data->id);
		auto GameObject = scene.Get_GameObject;

		data->Pos = { delivered_data->pos.x, delivered_data->pos.y ,delivered_data->pos.z , delivered_data->pos.w };

		GameObject->CenterPos = { data->Pos.x , data->Pos.y , data->Pos.z , data->Pos.w };
		



		break;
	}
	}
	

}


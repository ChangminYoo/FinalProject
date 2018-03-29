#include "server\main_client\AsyncClient.h"
#include "../../Scene.h"

void AsyncClient::ProcessPacket(Packet* packet, CGameObject& obj, Scene& scene)
{
	
	switch (packet[1])
	{
	case PACKET_PROTOCOL_TYPE::INIT_CLIENT:
	{
		//STC_ClientInit* update_data = reinterpret_cast<STC_ClientInit*>(&packet[2]);
		//obj->CenterPos.x = delivered_data.Pos.x;
		//obj->CenterPos.y = delivered_data.Pos.y;
		//obj->CenterPos.z = delivered_data.Pos.z;
		//
		//obj->m_player_data->ID = delivered_data.ID;

		// 0�� 1�� 2�� ĳ����

		//�����κ��� ���� ���ο� ������
		auto delivered_data = reinterpret_cast<STC_ClientInit*>(packet);

		//������ �����͸� �̿��� Ŭ���̾�Ʈ�� �÷��̾� ������ �������ش�
		//Scene -> DynamicObject -> CGameObject �� m_PlayerData�� �������ش�.

		//�������� �÷��̾��� �ʱ�ȭ������ �Ѱ��ֹǷ� ��� PlayerData ���� �����ؾ��Ѵ�
		//�������� ID 0 ~ 4������ ���� -- Ŭ������ ID 0 ~ 2������ �÷��̾� 
		//�� ����ID - ���;��̵� Ŭ���� ID�� ����־����
		auto data = scene.Get_PlayerServerData(delivered_data->player_data.ID);
		auto GameObject = scene.Get_GameObject;

		data->Dir = delivered_data->player_data.Dir;
		data->ID = delivered_data->player_data.ID; //- MAX_MONSTER_NUM;
		data->Is_AI = delivered_data->player_data.Is_AI;

		wcscpy(data->LoginData.name, delivered_data->player_data.LoginData.name);
		wcscpy(data->LoginData.password, delivered_data->player_data.LoginData.password);

		data->Pos = move(delivered_data->player_data.Pos);

		data->UserInfo.cur_hp = delivered_data->player_data.UserInfo.cur_hp;
		data->UserInfo.cur_mp = delivered_data->player_data.UserInfo.cur_mp;
		data->UserInfo.exp = delivered_data->player_data.UserInfo.exp;
		data->UserInfo.level = delivered_data->player_data.UserInfo.level;
		data->UserInfo.origin_hp = delivered_data->player_data.UserInfo.origin_hp;
		data->UserInfo.origin_mp = delivered_data->player_data.UserInfo.origin_mp;
		data->UserInfo.player_status.attack = delivered_data->player_data.UserInfo.player_status.attack;
		data->UserInfo.player_status.defend = delivered_data->player_data.UserInfo.player_status.defend;
		data->UserInfo.player_status.speed = delivered_data->player_data.UserInfo.player_status.speed;

		GameObject->CenterPos = { data->Pos.x , data->Pos.y , data->Pos.z , data->Pos.w };
		GameObject->Speed = delivered_data->player_data.UserInfo.player_status.speed;

		//ĳ���� �������ͽ� �ʱ�ȭ�� ����->Ŭ��. Ŭ�󿡼� �߰�����߉�
			
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


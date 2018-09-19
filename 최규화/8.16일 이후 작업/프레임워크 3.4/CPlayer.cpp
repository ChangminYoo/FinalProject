#include "CPlayer.h"
#include"Scene.h"

CPlayer::CPlayer(HWND hWnd,ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist, float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u) : Camera(hWnd,Device,commandlist,asp,e,a,u)
{
	//����� �ٸ� ������Ʈ�� �߰��Ұ�.
	TraceObject = new RangeObject(Device, commandlist, NULL, NULL, XMFLOAT4(-10000, -10000, -10000, 1));
	PlayerObject = NULL;
	
	
	
	//��ų�ѹ��� 0 : ����Ʈ ť��
	//	��ų�ѹ��� 1 : ��� ť��
	//	��ų�ѹ��� 2 : ��Ʈ����ũ
	//	��ų�ѹ��� 3 : ���̽�Ʈ����ũ
	//	��ų�ѹ��� 4 : �ĵ���
	//	��ų�ѹ��� 5 : �ǵ�
	//	��ų�ѹ��� 6 : �ظӺҷ�
	//	��ų�ѹ��� 7 : ���׿�
		
	//�� ������ �ٲٸ� ��ų�� �޶��� 
	skilldata.Skills[0] = 0;
	skilldata.Skills[1] = 4; //1
	skilldata.Skills[2] = 5; //2
	skilldata.Skills[3] = 3;
}


CPlayer::~CPlayer()
{
	if (TraceObject != NULL)
		delete TraceObject;
}

void CPlayer::TPSCameraSystem(int mx, int my,float DeltaTime)
{

	
			bool rotate = false;
			float playerYtheta = 0;
			if (ox == -1 ||oy == -1)//���콺 ��ǥ�� ���� �����Ѵ�. �ʱ�ȭ��
			{
				ox = mx;
				oy = my;
			}
			else
			{
				if (mx- ox >1)//���������� �̵�
				{
					//���̰� Ŭ���� �� ���� ȸ���Ѵ�. �� ���̰� ������ ���� �ȿ����̹Ƿ� ��鸲�� �����ȴ�.
					float dx = fabsf(mx - ox);
					if (dx > 10)
						dx = 10;

						ytheta += ((dx/10) * MMPE_PI* 180 / 180)*DeltaTime;

						playerYtheta = ((dx / 10) * MMPE_PI * 180 / 180)*DeltaTime;
						ox = mx;
						rotate = true;
				
					
				}
				else if (mx- ox<-1)//�������� �̵�
				{
					float dx = fabsf(mx - ox);
					if (dx > 10)
						dx = 10;

					ytheta += ((-dx/10) * MMPE_PI*180 / 180)*DeltaTime;
					playerYtheta = ((-dx / 10) * MMPE_PI * 180 / 180)*DeltaTime;
					ox = mx;
					rotate = true;
				
				}
				else
				{

					ox = mx;
				}



				if (my- oy > 1)//�Ʒ����̵�
				{
					float dy = fabsf(my - oy);
					if (dy > 10)
						dy = 10;
						xtheta += ((dy/10) * MMPE_PI*100 / 180)*DeltaTime;
						if (xtheta >= (85 * 3.14 / 180))
							xtheta = (85 * 3.14 / 180);



						oy = my;
					
				}
				else if (my- oy<-1)//�����̵�
				{

					float dy = fabsf(my - oy);
					if (dy > 10)
						dy = 10;

					xtheta += ((-dy / 10) * MMPE_PI * 100 / 180)*DeltaTime;

						if (xtheta <= (-65 * 3.14 / 180))
							xtheta = (-65 * 3.14 / 180);

						oy = my;

				}
				else
				{

					oy = my;
				}
				//ytheta = 0;
				
				//1. AtToEye��ŭ �̵���Ų��.
				XMFLOAT3 oe(0, 0, -Camera.CamData.AtToEye);
				
				//2. ������ŭ ȸ����Ų��.

				XMMATRIX Xr;
				Xr=XMMatrixRotationX(xtheta);


				XMMATRIX Yr;
				Yr=XMMatrixRotationY(ytheta);


				//y��ȸ���� �����ص�

				//�÷��̾ ȸ�����ѵ�. ���� -180�� ȸ�����Ѽ� �޸���� ���̰��Ѵ�
				//���Ŀ� ī�޶� ȸ���Ѹ�ŭ �߰������� �����̰���
				//PlayerObject->ThetaY += playerYtheta * 180 / MMPE_PI;
				
				
				
				XMFLOAT3 axis{ 0,1,0 };
				auto q2 = QuaternionRotation(axis, playerYtheta);
				PlayerObject->Orient = QuaternionMultiply(PlayerObject->Orient, q2);
			
				XMVECTOR result = XMLoadFloat3(&oe);
				 

				 //Xr*= Yr;
				 //result = XMVector3Transform(result, Xr);
				 ////ȸ������� offEye�� �����. offEye�� ���������� Eye��ġ��.
				 //XMStoreFloat3(&Camera.offEye, result);

				 //���ʹϾ� ����.
				 auto result2 = result;
				 auto rpy = XMQuaternionRotationRollPitchYaw(xtheta, ytheta, 0);
				 result2 = XMVector3Transform(result2, XMMatrixRotationQuaternion(rpy));
				 XMStoreFloat3(&Camera.offEye, result2);

				 //offAt�� ������ �����̴�.
				 Camera.CamData.Look = XMFLOAT3(Camera.offAt.x - Camera.offEye.x, Camera.offAt.y - Camera.offEye.y, Camera.offAt.z - Camera.offEye.z);
				 //�躤�ʹ� ���ȭ �س��.
				 Camera.CamData.Look = Float3Normalize(Camera.CamData.Look);

				 //
				 ////���ʹϾ� �׽�Ʈ �ڵ�.
				 //
				 //auto xr = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), xtheta);
				 //auto yr = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), ytheta);
				 //XMVECTOR test = XMVectorSet(1, 0, 0, 0);
				 //auto test2 = test;
				 //
				 //auto t=XMVector4Transform(test, Xr);//�׳� ȸ����İ��� ���(�� ���� �񱳴��)
				 ////***********V*************8
				 ////x������xtheta , y�ष�� ytheta��ŭ ȸ���� ���ʹϾ𳢸� ������� ���ϰ� �װ� ��ķ� ǥ���Ѱ��� ��
				 //auto xy = XMMatrixRotationQuaternion(XMQuaternionMultiply(xr,yr));//������ ����������.
				 //auto vv = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(xtheta, ytheta, 0));//����
				 ////���ʹϾ��� ���ϰ� �� �ӷ����ʹϾ��� ���غ�
				 //auto test3 = test2;
				 //auto qm = XMQuaternionMultiply(xr, yr);
				 //auto qms = XMQuaternionConjugate(qm);
				 //test2=XMQuaternionMultiply(qm, test2);
				 ////����� ���� �� �´µ�, ��ȣ�� �� �̻��ϰ� ����. ���� ���ʹϾ� ���Ÿ� ***V*** �� ����� ����.
				 //
				 //test2 = XMQuaternionMultiply(test2, qms);
				 //auto tt=XMVector4Transform(test3, xy);

			}
		
			//����� ī�޶� ��ġ�� �����Ѵ�.
			PlayerCameraReLocate();
}

void CPlayer::PlayerCameraReLocate()//ī�޶� ��ġ�� �缳���Ѵ�.
{
	if (PlayerObject != NULL)
	{

		auto e=XMLoadFloat3(&Camera.offEye);
		auto p = XMLoadFloat4(&PlayerObject->CenterPos);
		auto a = XMLoadFloat3(&Camera.offAt);

		//ī�޶� ��ġ�� �÷��̾������Ʈ + ������ eye�̴�. ������ eye�� ���� ���ؼ� �÷��̾ ������������ ī�޶���ġ��.
		//���߿� ī�޶� ȸ���Ҷ� CamData�� Eye�� At�� �ٲٴ°Ծƴ϶�
		//off������ ȸ�����Ѿ��Ѵ�.
		XMStoreFloat3(&Camera.CamData.EyePos, p + e);

		XMStoreFloat3(&Camera.CamData.At, p + a);

	
	}
}

void CPlayer::SetPlayer(CGameObject * obj)
{
	PlayerObject = obj;
	((CCubeManObject*)obj)->player = this;
	PlayerCameraReLocate();
}

//���⼭ �߿������� Ű���� ���� ��� �浹�˻縦 �ѹ� ������Ѵ�.
//�ֳ��ϸ� �̰� ������ ���� �� ���̱� ���ؼ��ε� ������� ��ü�� �� ���� ���ִٰ� ġ��.
//�̶� ��ȭ��ǥ�� ������ȭ��ǥ�� ������ �����δ� ������ �����ν������ϰ� ���������Ѵ�.
//�̸� ���� Ű���� �浹ó���� �Ѵ��� �浹�ϸ� �����·� �ǵ����� �ϴ°��̴�.
//���� �׻� pp�� ��ġ�� ����������Ѵ�.
void CPlayer::PlayerInput(float DeltaTime, Scene* scene)
{
	if (scene->GetGameState() == GS_PLAY)
	{
		
		if (PlayerObject != NULL && PlayerObject->gamedata.HP > 0 && GetFocus())
		{
			bool move = false;

			STC_CharMove cts_move;
			STC_CharAnimation cts_cani;

			if (GetAsyncKeyState(0x57) & 0x8000)//WŰ
			{
				move = true;

				curr_playerdir = 1;
				move = true;

				m_up_flag = true;
			}
			else
			{
				if (m_up_flag)
				{
					//cout << "up_flag_false" << "\n";
					m_up_flag = false;

					cts_move.dir = 0;
					m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_move));
					m_async_client->SetMovePrevDir(0);

					if (PlayerObject->n_Animation != Ani_State::Attack)
					{
						cts_cani.ani_state = Ani_State::Idle;
						cts_cani.id = PlayerObject->m_player_data.id;
						m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_cani));
					}
				}

			}

			if (GetAsyncKeyState(0x53) & 0x8000)//SŰ
			{
				move = true;

				curr_playerdir = 2;
				move = true;

				m_down_flag = true;
			}
			else
			{
				if (m_down_flag)
				{
					//cout << "down_flag_false" << "\n";
					m_down_flag = false;

					cts_move.dir = 0;
					m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_move));
					m_async_client->SetMovePrevDir(0);

					if (PlayerObject->n_Animation != Ani_State::Attack)
					{
						cts_cani.ani_state = Ani_State::Idle;
						cts_cani.id = PlayerObject->m_player_data.id;
						m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_cani));
					}
				}
			}

			if (GetAsyncKeyState(0x41) & 0x8000)//AŰ
			{
				move = true;

				curr_playerdir = 3;
				move = true;

				m_left_flag = true;
			}
			else
			{
				if (m_left_flag)
				{
					//cout << "left_flag_false" << "\n";
					m_left_flag = false;

					cts_move.dir = 0;
					m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_move));
					m_async_client->SetMovePrevDir(0);

					if (PlayerObject->n_Animation != Ani_State::Attack)
					{
						cts_cani.ani_state = Ani_State::Idle;
						cts_cani.id = PlayerObject->m_player_data.id;
						m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_cani));
					}
				}
			}

			if (GetKeyState(0x44) & 0x8000)//DŰ
			{
				move = true;
				curr_playerdir = 4;
				move = true;

				m_right_flag = true;
			}
			else
			{
				if (m_right_flag)
				{
					//cout << "left_flag_false" << "\n";
					m_right_flag = false;

					cts_move.dir = 0;
					m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_move));
					m_async_client->SetMovePrevDir(0);

					if (PlayerObject->n_Animation != Ani_State::Attack)
					{
						cts_cani.ani_state = Ani_State::Idle;
						cts_cani.id = PlayerObject->m_player_data.id;
						m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_cani));
					}
				}
			}

			//ĳ���Ͱ� �̵��� �� ��, �� Ű�� ������ �����ٴ� ������ ������ ����		
			if (move == true)//�����̰� ������ �����̴� �������
			{
				if (PlayerObject->n_Animation != Ani_State::Attack)//���ݸ���� �ƴϸ� �ٽ� �����·�
				{
					PlayerObject->SetAnimation(Ani_State::Run);

					if (m_async_client->GetMovePrevDir() != curr_playerdir)
					{
						cts_move.dir = curr_playerdir;
						m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_move));

						cts_cani.ani_state = Ani_State::Run;
						cts_cani.id = PlayerObject->m_player_data.id;
						m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_cani));

						//cout << "move keydown prev_playerdir : " << static_cast<int>(m_async_client->GetMovePrevDir()) << "\n";
						//cout << "move keydown curr_playerdir : " << static_cast<int>(curr_playerdir) << "\n";
					}

					m_async_client->SetMovePrevDir(curr_playerdir);
				}
			}
			else
			{
				if (PlayerObject->n_Animation != Ani_State::Attack)//���ݸ���� �ƴϸ� �ٽ� �����·�
				{
					PlayerObject->SetAnimation(Ani_State::Idle);
				}
			}


			if (GetKeyState(VK_SPACE) & 0x8000)
			{
				if (!PlayerObject->AirBone)
				{
					m_jump_flag = true;
					curr_playerdir = 5;

					if (m_async_client->GetMovePrevDir() != curr_playerdir)
					{
						cts_move.dir = curr_playerdir;
						m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_move));

						//���� �ִϸ��̼� �߰� �� �̰��� �־��ش�
					}
					m_async_client->SetMovePrevDir(curr_playerdir);

				}
			}
			else
			{
				if (m_jump_flag)
				{
					//cout << "Jump Flag!!" << "\n";
					m_jump_flag = false;

					cts_move.dir = 0;
					m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_move));
					m_async_client->SetMovePrevDir(0);
				}
			}

			if (GetAsyncKeyState(0x31) & 0x0001)
				skilldata.SellectBulletIndex = 0;
			else if (GetAsyncKeyState(0x32) & 0x0001)
				skilldata.SellectBulletIndex = 1;
			else if (GetAsyncKeyState(0x33) & 0x0001)
				skilldata.SellectBulletIndex = 2;
			else if (GetAsyncKeyState(0x34) & 0x0001)
				skilldata.SellectBulletIndex = 3;


			
			//���� ��ư�� �������� ������ ��ų�� �ѹ��� 4�� 5�� ���(4�� �ĵ��� , 5�� ��������(?))
			if (skilldata.Skills[skilldata.SellectBulletIndex] == 4 && skilldata.isSkillOn[skilldata.SellectBulletIndex])//�ĵ����ΰ��
			{
				CTS_SKILL_WAVESHOCK cts_skill_waveshock;
				cts_skill_waveshock.skill_data.master_id = PlayerObject->m_player_data.id;
				cts_skill_waveshock.skill_data.my_id = skilldata.Skills[skilldata.SellectBulletIndex];
				cts_skill_waveshock.skill_data.alive = true;

				//������
				scene->StaticObject.push_back(new RingObject(scene->device, scene->commandlist, &scene->BbObject, &scene->Shadows, PlayerObject, PlayerObject->CenterPos));
				skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
				skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;
				skilldata.SellectBulletIndex = 0;//��ų ������ ���� ù��° ��ų�� ������

				scene->Player->m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_skill_waveshock));

			}
			else if (skilldata.Skills[skilldata.SellectBulletIndex] == 5 && skilldata.isSkillOn[skilldata.SellectBulletIndex])//�ǵ�
			{
				CTS_SKILL_SHIELD cts_skill_shield;
				cts_skill_shield.skill_data.master_id = PlayerObject->m_player_data.id;
				cts_skill_shield.skill_data.my_id = skilldata.Skills[skilldata.SellectBulletIndex];
				cts_skill_shield.skill_data.alive = true;

				scene->Player->m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_skill_shield));

				//�ǵ� ����
				scene->NoCollObject.push_back(new ShieldArmor(scene->device, scene->commandlist, &scene->BbObject, &scene->Shadows, PlayerObject, PlayerObject->CenterPos));
				skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
				skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;
				skilldata.SellectBulletIndex = 0;
			}


			if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
			{
				if (MouseOn <= 1)
				{
					MouseOn = 2;
					ShowCursor(true);
				}
			}
			else
			{
				if (MouseOn > 1)
				{
					MouseOn = 0;
					ShowCursor(false);
				}
			}
			//Ű�� ���������� �ش� ��ų�� �˻��ؼ� ����������Ʈ�� �־�� �ϴ��� �˻��Ѵ�.
			CheckTraceSkill();
		}
	}
	
}

void CPlayer::Tick(float DeltaTime)
{
	for (int i = 0; i < 4; i++)
	{
		if (skilldata.SkillsCoolTime[i] > 0)
		{
			skilldata.SkillsCoolTime[i] -= DeltaTime;
			skilldata.isSkillOn[i] = false;
		}
		
		if (skilldata.SkillsCoolTime[i] <= 0)
		{
			skilldata.SkillsCoolTime[i] = 0;
			skilldata.isSkillOn[i] = true;
		}
	}

	/*
	if (pointrank.Rank == 1 && pointrank.Point>=50)
	{
		if (pointrank.Init == false)
		{
			pointrank.TopMode = true;
			PlayerObject->ObjData.Scale = 4;
			XMFLOAT3 rx(4, 0, 0);
			XMFLOAT3 ry(0, 13, 0);
			XMFLOAT3 rz(0, 0, 4);
			PlayerObject->rco.SetPlane(rx, ry, rz);
			PlayerObject->pp->SetHalfBox(4, 13, 4);//�浹 �ڽ��� x,y,z ũ��
			((CCubeManObject*)PlayerObject)->s->ObjData.Scale = 3.0f;
			((CCubeManObject*)PlayerObject)->Hpbar->YPos = 16;
			((CCubeManObject*)PlayerObject)->HPFrame->YPos = 16;
			PlayerObject->gamedata.MAXHP = 1000;
			PlayerObject->gamedata.Speed = 75;
			PlayerObject->gamedata.HP = PlayerObject->gamedata.HP + 800;
			pointrank.Init = true;


		}
	}
	else
	{
		if (pointrank.TopMode == true)
		{
			pointrank.Init = false;
			pointrank.TopMode = false;
			PlayerObject->ObjData.Scale = 3;
			XMFLOAT3 rx(3, 0, 0);
			XMFLOAT3 ry(0, 10, 0);
			XMFLOAT3 rz(0, 0, 3);
			PlayerObject->rco.SetPlane(rx, ry, rz);
			PlayerObject->gamedata.Speed = 50;
			PlayerObject->pp->SetHalfBox(3, 10, 3);//�浹 �ڽ��� x,y,z ũ��
			((CCubeManObject*)PlayerObject)->s->ObjData.Scale = 2.0f;
			((CCubeManObject*)PlayerObject)->Hpbar->YPos = 10;
			((CCubeManObject*)PlayerObject)->HPFrame->YPos = 10;
		}
	}
	*/

}

void CPlayer::CreateBullet(ID3D12Device* Device, ID3D12GraphicsCommandList* cl,XMFLOAT3 & Goal,CGameObject* lock, list<CGameObject*>* bulletlist)
{
	//���缱�õ� 0~3�� ������ �ϳ��� �ε����� �־��ָ� �� Skills[�ε���]�� ����� �ٷ� � ���������� ��Ÿ��.

	switch (skilldata.Skills[skilldata.SellectBulletIndex])
	{

	case 0://�ҷ�ť��(����Ʈ ť��)
	{

		PlayerObject->SetAnimation(2);
		//���� �ش罺ų�� ��Ÿ���� �־�����.
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;


		auto v = Float3Add(Goal, XMFloat4to3(PlayerObject->CenterPos), false);

		v = Float3Normalize(v);//���ο� �躤��(�߻����)
		//���⼭ �躤�Ͷ� ����, �÷��̾ �ƴ϶�, �Ѿ��� �躤�ʹ�. ��� ������Ʈ�� ���� �躤�ʹ� 0,0,1 �Ǵ� 0,0,-1 �ε�, ���ư��� ������ �ٶ󺸵���  
		//�ؾ��ϹǷ� ���ο� �躤�͸� �ʿ�� �ϴ°��̴�.  

		//���� �躤�Ϳ� ���ο� �躤�͸� �����ؼ� �������� ���Ѵ�.
		XMFLOAT3 l{ 0,0,1 };
		XMVECTOR ol = XMLoadFloat3(&l);
		XMVECTOR nl = XMLoadFloat3(&v);
		auto axis = XMVector3Cross(ol, nl);
		//�������� �ϼ�.
		axis = XMVector3Normalize(axis);
		XMFLOAT3 Axis;
		XMStoreFloat3(&Axis, axis);
		//���� ȸ�������� ���ؾ��Ѵ�. ������ ���� ȸ�������� ���Ѵ�.

		auto temp = XMVector3Dot(ol, nl);

		float d;//���� �躤�Ϳ� ���ο� �躤�͸� ������ ���.
		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������

		auto ori = QuaternionRotation(Axis, d);

		//��¥ �躤�͸� �������� ���� ��¥ Right���͸� ���Ѵ�. ��¥ Up�� ��¥ ��� ��¥ ����Ʈ�� �������ϸ��.
		//ȸ���� �������ش�. ȸ���࿡�� �躤�͸� �����ϸ� ��¥ Right���Ͱ� ���´�.
		//�̶� ������ �ִ� RightVector�� ��¥ RightVector�� ���հ��� ����ϰ�
		//�躤�͸� ȸ�������� �����ش�. �� ȸ���࿡ ������ ����°�?
		//�����ϴ� ���� �躤�Ϳ� �߻������ �躤���� ȸ�����
		//���� RightVector�� �߻������ ȸ�������ϸ� ���� �ٸ��� ���´�.
		//������ ���� �׳� �̴�� �Ѿ�� �Ǹ� RightVector�� ��¥ RightVector�� ������ŭ ������ ����Ƿ�
		//�̷��ԵǸ� ���߿� Up�� ���Ҷ��� ������ �����.
		//��� X�� Y�� Z�� ������� �����ϸ� �̷��� ������ ���Ǿ�����, Ư������ ������� ȸ���Ҷ� ����� ������.

		auto wmatrix = XMMatrixIdentity();
		auto quater = XMLoadFloat4(&ori);
		wmatrix *= XMMatrixRotationQuaternion(quater);

		auto orr = XMVectorSet(1, 0, 0, 0);
		orr = XMVector4Transform(orr , wmatrix);//��¥ ����Ʈ ����
		orr = XMVector3Normalize(orr );
		auto RealRight = XMVector3Cross(axis, nl);//��¥ ����Ʈ����
		RealRight = XMVector3Normalize(RealRight);

		//��¥ ����Ʈ ���Ϳ� ��¥ ����Ʈ ���͸� ������.
		temp = XMVector3Dot(RealRight, orr );

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������
		auto ori2 = XMQuaternionRotationAxis(nl, d);//��¥ �躤�͸� ȸ�������� ��� ��¥����Ʈ�� ��¥����Ʈ�� ���հ���ŭȸ��

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//���� ȸ�� ����
 

		CGameObject *bul = new BulletCube(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, lock, PlayerObject->CenterPos, true);
		bulletlist->push_back(bul);

		//������
		CTS_Attack cts_attack;
		cts_attack.bull_data.master_id = bul->m_bullet_data.master_id;
		cts_attack.bull_data.my_id = bul->m_bullet_data.my_id;

		cts_attack.bull_data.pos4f = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };
		cts_attack.bull_data.rot4f = { PlayerObject->Orient.x, PlayerObject->Orient.y, PlayerObject->Orient.z, PlayerObject->Orient.w };
		cts_attack.bull_data.vel3f = { bul->pp->GetVelocity().x, bul->pp->GetVelocity().y, bul->pp->GetVelocity().z };
		cts_attack.bull_data.type = BULLET_TYPE::protocol_LightBullet;
		cts_attack.bull_data.alive = true;
		cts_attack.bull_data.endpoint = { Goal.x , Goal.y , Goal.z };

		cts_attack.lifetime = 0.0;
		cts_attack.bull_data.degree = 0.f;

		m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_attack));

		//cout << "SavePoint: " << Goal.x << "," << Goal.y << "," << Goal.z << "\n";
		//

		break;
	}

	case 1://�ҷ�ť��(��� ť��)
	{
		PlayerObject->SetAnimation(2);

		//���� �ش罺ų�� ��Ÿ���� �־�����.
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;


		auto v = Float3Add(Goal, XMFloat4to3(PlayerObject->CenterPos), false);

		v = Float3Normalize(v);//���ο� �躤��(�߻����)
							   //���⼭ �躤�Ͷ� ����, �÷��̾ �ƴ϶�, �Ѿ��� �躤�ʹ�. ��� ������Ʈ�� ���� �躤�ʹ� 0,0,1 �Ǵ� 0,0,-1 �ε�, ���ư��� ������ �ٶ󺸵���  
							   //�ؾ��ϹǷ� ���ο� �躤�͸� �ʿ�� �ϴ°��̴�.  

							   //���� �躤�Ϳ� ���ο� �躤�͸� �����ؼ� �������� ���Ѵ�.
		XMFLOAT3 l{ 0,0,1 };
		XMVECTOR ol = XMLoadFloat3(&l);
		XMVECTOR nl = XMLoadFloat3(&v);
		auto axis = XMVector3Cross(ol, nl);
		//�������� �ϼ�.
		axis = XMVector3Normalize(axis);
		XMFLOAT3 Axis;
		XMStoreFloat3(&Axis, axis);
		//���� ȸ�������� ���ؾ��Ѵ�. ������ ���� ȸ�������� ���Ѵ�.

		auto temp = XMVector3Dot(ol, nl);

		float d;//���� �躤�Ϳ� ���ο� �躤�͸� ������ ���.
		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������

		auto ori = QuaternionRotation(Axis, d);

		auto wmatrix = XMMatrixIdentity();
		auto quater = XMLoadFloat4(&ori);
		wmatrix *= XMMatrixRotationQuaternion(quater);

		auto orr = XMVectorSet(1, 0, 0, 0);
		orr = XMVector4Transform(orr , wmatrix);//��¥ ����Ʈ ����
		orr = XMVector3Normalize(orr );
		auto RealRight = XMVector3Cross(axis, nl);//��¥ ����Ʈ����
		RealRight = XMVector3Normalize(RealRight);

		//��¥ ����Ʈ ���Ϳ� ��¥ ����Ʈ ���͸� ������.
		temp = XMVector3Dot(RealRight, orr );

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������
		auto ori2 = XMQuaternionRotationAxis(nl, d);//��¥ �躤�͸� ȸ�������� ��� ��¥����Ʈ�� ��¥����Ʈ�� ���հ���ŭȸ��

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//���� ȸ�� ����

		CGameObject *bul = new HeavyBulletCube(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, lock, PlayerObject->CenterPos);
		bulletlist->push_back(bul);


		//������
		CTS_Attack cts_attack;
		cts_attack.bull_data.master_id = bul->m_bullet_data.master_id;
		cts_attack.bull_data.my_id = bul->m_bullet_data.my_id;

		cts_attack.bull_data.pos4f = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };
		cts_attack.bull_data.rot4f = { PlayerObject->Orient.x, PlayerObject->Orient.y, PlayerObject->Orient.z, PlayerObject->Orient.w };
		cts_attack.bull_data.vel3f = { bul->pp->GetVelocity().x, bul->pp->GetVelocity().y, bul->pp->GetVelocity().z };
		cts_attack.bull_data.type = BULLET_TYPE::protocol_HeavyBullet;
		cts_attack.bull_data.alive = true;
		cts_attack.bull_data.endpoint = { Goal.x , Goal.y , Goal.z };

		cts_attack.lifetime = 0.0;
		cts_attack.bull_data.degree = 0.f;

		m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_attack));

		//

		break;

	}
	case 2://��Ʈ����ũ
	{
		PlayerObject->SetAnimation(2);
		//���� �ش罺ų�� ��Ÿ���� �־�����.
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;

		
		bulletlist->push_back(new Tetrike(Device, cl, PlayerObject->ParticleList,NULL, bulletlist, PlayerObject, lock, XMFloat3to4(Goal)));
		break;

	}
	case 3://���̽�Ʈ����ũ
	{

		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;

		PlayerObject->ParticleList->push_back(new DiceObject(Device, cl, PlayerObject->ParticleList,NULL, PlayerObject,Goal, bulletlist ,XMFLOAT4(PlayerObject->CenterPos.x, 35, PlayerObject->CenterPos.z, 0)));

		break;
	}
	case 6://�ظӺҷ�
	{
		PlayerObject->SetAnimation(2);
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;

		CGameObject *hammer = new HammerBullet(Device, cl, PlayerObject->ParticleList, NULL,NULL,3, PlayerObject, XMFLOAT4(0,0,0,1),NULL, PlayerObject->CenterPos,XMFLOAT4(0,0,20,0));
		bulletlist->push_back(hammer);
		
		//������ �ظӺҷ� ������ ���� �ʱⰪ ��Ŷ ����
		CTS_HammerSkillInfo cts_hammer_skill_info;
		cts_hammer_skill_info.pos4f = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };
		cts_hammer_skill_info.rot4f = { 0, 0, 0, 1 };	//�ʱ� rot�� ori �� 0 0 0 1
		cts_hammer_skill_info.master_id = PlayerObject->m_player_data.id;
		cts_hammer_skill_info.my_id = hammer->m_bullet_data.my_id;
		cts_hammer_skill_info.weapon_num = 3;
		cts_hammer_skill_info.opp_pos4f = { 0.f, 0.f, 20.f, 0.f };

		CTS_SKILL_HAMMERBULLET cts_hammer_skill_packet;
		cts_hammer_skill_packet.skill_data = cts_hammer_skill_info;
		
		m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_hammer_skill_packet));

		break;
	}
	case 7:
	{
		PlayerObject->SetAnimation(2);
		//���� �ش罺ų�� ��Ÿ���� �־�����.
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;


		bulletlist->push_back(new MeteorObject(Device, cl, PlayerObject->ParticleList, PlayerObject->Shadow, PlayerObject, XMFLOAT4(0,0,0,1), XMFloat3to4(Goal)));
		break;
	}

	}

}

void CPlayer::CheckTraceSkill()
{
	switch (skilldata.Skills[ skilldata.SellectBulletIndex])
	{

	case 2://�ѹ����� 2�� (��Ʈ����ũ) ��ų�� ���.
		MouseTrace = true;
		break;

	case 7:
		MouseTrace = true;
		break;
	default:
		MouseTrace = false;
		TraceObject->CenterPos = XMFLOAT4(-10000, -10000, -10000, 1);
		
	}

}

void CPlayer::CreateOtherClientBullet(ID3D12Device * Device, ID3D12GraphicsCommandList * cl, Position3D & Goal, CGameObject * lock, list<CGameObject*>* bulletlist, STC_BulletObject_Info server_bulldata)
{
	XMFLOAT4 xmf4_pos{ server_bulldata.pos4f.x, server_bulldata.pos4f.y, server_bulldata.pos4f.z, server_bulldata.pos4f.w };
	XMFLOAT3 xmf3_pos{ Goal.x, Goal.y, Goal.z };

	XMFLOAT4 ori = XMFLOAT4(0, 0, 0, 0);
	CGameObject *bul = nullptr;

	//PlayerObject�� ���� �� Ŭ���̾�Ʈ �÷��̾ �ǹ��Ѵ�
	switch (server_bulldata.type)
	{
		case protocol_LightBullet:
		case protocol_HeavyBullet:
		{
			//2018.07.25 - ori�� ���ϴ� �۾��� �ᱹ �ؾ��� particle�� ������ �°� �����ȴ�
			//�̰� �������� �۾��ؼ� Orient���� ������Ʈ�ؼ� ��������, �ᱹ Ŭ�󿡼� �� �۾��� ���� �������
			//�� �۾��� �ҷ��� �ѹ� ������ ���� �ϸ�ȴ�.
			//[1]ori���� �ҷ������� Ŭ��->����->Ŭ�� �� ������ �ѹ� �����ϴ°��� ���ɻ� �����ΰ� �ƴϸ� [2]Ŭ�󿡼� �̷��� �ѹ����ִ°� �����ΰ�
			//Ŭ�� -> ������ �� �� degree�� ori�� ���� ������Ʈ �� rotation���� ���޵ǹǷ� 
			//�������� �Ʒ��� �۾��� �������� ���� 
			//[2]�� �� ȿ�����ΰ� ����
			//[1]�� �ϸ�, ������Ŷ�� ũ�Ⱑ �þ ��Ŷ����� ���� -> �������Ϸ� �߻�,
			//           Ŭ���̾�Ʈ�� cpu���� ������ ���� gpu�� �̿�, �� �۾��� �߰��Ѵٰ��ؼ� ���ϰ� �߻����� ����

			auto v = Float3Add(xmf3_pos, XMFloat4to3(xmf4_pos), false);

			v = Float3Normalize(v);//���ο� �躤��(�߻����)
								   //���⼭ �躤�Ͷ� ����, �÷��̾ �ƴ϶�, �Ѿ��� �躤�ʹ�. ��� ������Ʈ�� ���� �躤�ʹ� 0,0,1 �Ǵ� 0,0,-1 �ε�, ���ư��� ������ �ٶ󺸵���  
								   //�ؾ��ϹǷ� ���ο� �躤�͸� �ʿ�� �ϴ°��̴�.  

								   //���� �躤�Ϳ� ���ο� �躤�͸� �����ؼ� �������� ���Ѵ�.
			XMFLOAT3 l{ 0,0,1 };
			XMVECTOR ol = XMLoadFloat3(&l);
			XMVECTOR nl = XMLoadFloat3(&v);

			auto axis = XMVector3Cross(ol, nl);

			//�������� �ϼ�.
			axis = XMVector3Normalize(axis);
			XMFLOAT3 Axis;
			XMStoreFloat3(&Axis, axis);
			//���� ȸ�������� ���ؾ��Ѵ�. ������ ���� ȸ�������� ���Ѵ�.

			auto temp = XMVector3Dot(ol, nl);

			float d;//���� �躤�Ϳ� ���ο� �躤�͸� ������ ���.
			XMStoreFloat(&d, temp);
			if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
				d = acos(d);//���� �ϼ�. ������

			auto ori = QuaternionRotation(Axis, d);

			//��¥ �躤�͸� �������� ���� ��¥ Right���͸� ���Ѵ�. ��¥ Up�� ��¥ ��� ��¥ ����Ʈ�� �������ϸ��.
			//ȸ���� �������ش�. ȸ���࿡�� �躤�͸� �����ϸ� ��¥ Right���Ͱ� ���´�.
			//�̶� ������ �ִ� RightVector�� ��¥ RightVector�� ���հ��� ����ϰ�
			//�躤�͸� ȸ�������� �����ش�. �� ȸ���࿡ ������ ����°�?
			//�����ϴ� ���� �躤�Ϳ� �߻������ �躤���� ȸ�����
			//���� RightVector�� �߻������ ȸ�������ϸ� ���� �ٸ��� ���´�.
			//������ ���� �׳� �̴�� �Ѿ�� �Ǹ� RightVector�� ��¥ RightVector�� ������ŭ ������ ����Ƿ�
			//�̷��ԵǸ� ���߿� Up�� ���Ҷ��� ������ �����.
			//��� X�� Y�� Z�� ������� �����ϸ� �̷��� ������ ���Ǿ�����, Ư������ ������� ȸ���Ҷ� ����� ������.

			auto wmatrix = XMMatrixIdentity();
			auto quater = XMLoadFloat4(&ori);
			wmatrix *= XMMatrixRotationQuaternion(quater);

			auto orr = XMVectorSet(1, 0, 0, 0);
			orr = XMVector4Transform(orr, wmatrix);//��¥ ����Ʈ ����
			orr = XMVector3Normalize(orr);
			auto RealRight = XMVector3Cross(axis, nl);//��¥ ����Ʈ����
			RealRight = XMVector3Normalize(RealRight);

			//��¥ ����Ʈ ���Ϳ� ��¥ ����Ʈ ���͸� ������.
			temp = XMVector3Dot(RealRight, orr);

			XMStoreFloat(&d, temp);
			if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
				d = acos(d);//���� �ϼ�. ������
			auto ori2 = XMQuaternionRotationAxis(nl, d);//��¥ �躤�͸� ȸ�������� ��� ��¥����Ʈ�� ��¥����Ʈ�� ���հ���ŭȸ��

			auto tempori = XMLoadFloat4(&ori);
			tempori = XMQuaternionMultiply(tempori, ori2);
			XMStoreFloat4(&ori, tempori);//���� ȸ�� ����

			if (server_bulldata.type == protocol_LightBullet)
				bul = new BulletCube(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, lock, PlayerObject->CenterPos, true);

			if (server_bulldata.type == protocol_HeavyBullet)
				bul = new HeavyBulletCube(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, lock, PlayerObject->CenterPos, true);

		}
		break;

	default:
		break;
	}

	bul->m_bullet_data.master_id = server_bulldata.master_id;
	bul->m_bullet_data.my_id = server_bulldata.my_id;

	bul->CenterPos = XMFLOAT4(server_bulldata.pos4f.x, server_bulldata.pos4f.y, server_bulldata.pos4f.z, server_bulldata.pos4f.w);
	bul->pp->SetPosition(&bul->CenterPos);

	//bul->Orient = XMFLOAT4(server_bulldata.rot4f.x, server_bulldata.rot4f.y, server_bulldata.rot4f.z, server_bulldata.rot4f.w);
	//bul->UpdateLookVector();

	bul->m_bullet_data = move(server_bulldata);

	//bul->ParticleList->back()->Orient = bul->Orient;
	//bul->ParticleList->back()->UpdateLookVector();

	bulletlist->push_back(bul);

}

void CPlayer::CreateOtherClientDicestrikeSkill(ID3D12Device * Device, ID3D12GraphicsCommandList * cl, Position3D & Goal, CGameObject * lock, list<CGameObject*>* bulletlist, STC_BulletObject_Info server_bulldata, const XMFLOAT3 & OffLookvector)
{
	XMFLOAT4 xmf4_pos{ server_bulldata.pos4f.x, server_bulldata.pos4f.y, server_bulldata.pos4f.z, server_bulldata.pos4f.w };
	XMFLOAT3 xmf3_pos{ Goal.x, Goal.y, Goal.z };

	XMFLOAT3 l{ 0,0,1 };
	XMVECTOR ol = XMLoadFloat3(&l);
	XMVECTOR nl = XMLoadFloat3(&OffLookvector);
	auto axis = XMVector3Cross(ol, nl);
	//�������� �ϼ�.
	axis = XMVector3Normalize(axis);
	XMFLOAT3 Axis;
	XMStoreFloat3(&Axis, axis);
	//���� ȸ�������� ���ؾ��Ѵ�. ������ ���� ȸ�������� ���Ѵ�.

	auto temp = XMVector3Dot(ol, nl);

	float d;//���� �躤�Ϳ� ���ο� �躤�͸� ������ ���.
	XMStoreFloat(&d, temp);
	if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
		d = acos(d);//���� �ϼ�. ������

	auto ori = QuaternionRotation(Axis, d);

	//��¥ �躤�͸� �������� ���� ��¥ Right���͸� ���Ѵ�. ��¥ Up�� ��¥ ��� ��¥ ����Ʈ�� �������ϸ��.
	//ȸ���� �������ش�. ȸ���࿡�� �躤�͸� �����ϸ� ��¥ Right���Ͱ� ���´�.
	//�̶� ������ �ִ� RightVector�� ��¥ RightVector�� ���հ��� ����ϰ�
	//�躤�͸� ȸ�������� �����ش�. �� ȸ���࿡ ������ ����°�?
	//�����ϴ� ���� �躤�Ϳ� �߻������ �躤���� ȸ�����
	//���� RightVector�� �߻������ ȸ�������ϸ� ���� �ٸ��� ���´�.
	//������ ���� �׳� �̴�� �Ѿ�� �Ǹ� RightVector�� ��¥ RightVector�� ������ŭ ������ ����Ƿ�
	//�̷��ԵǸ� ���߿� Up�� ���Ҷ��� ������ �����.
	//��� X�� Y�� Z�� ������� �����ϸ� �̷��� ������ ���Ǿ�����, Ư������ ������� ȸ���Ҷ� ����� ������.

	auto wmatrix = XMMatrixIdentity();
	auto quater = XMLoadFloat4(&ori);
	wmatrix *= XMMatrixRotationQuaternion(quater);

	auto orr = XMVectorSet(1, 0, 0, 0);
	orr = XMVector4Transform(orr, wmatrix);//��¥ ����Ʈ ����
	orr = XMVector3Normalize(orr);
	auto RealRight = XMVector3Cross(axis, nl);//��¥ ����Ʈ����
	RealRight = XMVector3Normalize(RealRight);

	//��¥ ����Ʈ ���Ϳ� ��¥ ����Ʈ ���͸� ������.
	temp = XMVector3Dot(RealRight, orr);

	XMStoreFloat(&d, temp);
	if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
		d = acos(d);//���� �ϼ�. ������
	auto ori2 = XMQuaternionRotationAxis(nl, d);//��¥ �躤�͸� ȸ�������� ��� ��¥����Ʈ�� ��¥����Ʈ�� ���հ���ŭȸ��

	auto tempori = XMLoadFloat4(&ori);
	tempori = XMQuaternionMultiply(tempori, ori2);
	XMStoreFloat4(&ori, tempori);//���� ȸ�� ����

	//cout << "CreateOtherClientBullet Master Lookvector xyz: " << OffLookvector.x << " , " << OffLookvector.y << " , " << OffLookvector.z << "\n";
	//cout << "\n";

	CGameObject *bul = new DiceStrike(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, server_bulldata.degree, NULL, PlayerObject->CenterPos);

	bul->m_bullet_data.master_id = server_bulldata.master_id;
	bul->m_bullet_data.my_id = server_bulldata.my_id;

	bul->CenterPos = XMFLOAT4(server_bulldata.pos4f.x, server_bulldata.pos4f.y, server_bulldata.pos4f.z, server_bulldata.pos4f.w);
	bul->pp->SetPosition(&bul->CenterPos);

	//bul->Orient = XMFLOAT4(server_bulldata.rot4f.x, server_bulldata.rot4f.y, server_bulldata.rot4f.z, server_bulldata.rot4f.w);
	//bul->UpdateLookVector();

	bul->m_bullet_data = move(server_bulldata);

	//bul->ParticleList->back()->Orient = bul->Orient;
	//bul->ParticleList->back()->UpdateLookVector();

	bulletlist->push_back(bul);
}

void CPlayer::CreateOtherClientEndBullet(ID3D12Device * Device, ID3D12GraphicsCommandList * cl, const Position3D & Goal, CGameObject * lock, list<CGameObject*>* bulletlist, const STC_BulletObject_EndPos_Info& server_endbulldata)
{
	XMFLOAT4 xmf4_pos{ server_endbulldata.end_pos4f.x, server_endbulldata.end_pos4f.y, server_endbulldata.end_pos4f.z, server_endbulldata.end_pos4f.w };
	XMFLOAT3 xmf3_pos{ Goal.x, Goal.y, Goal.z };

	XMFLOAT4 ori = XMFLOAT4(0, 0, 0, 0);
	CGameObject *bul = nullptr;

	//PlayerObject�� ���� �� Ŭ���̾�Ʈ �÷��̾ �ǹ��Ѵ�
	switch (server_endbulldata.type)
	{
	case protocol_LightBullet:
	case protocol_HeavyBullet:
	{
		//2018.07.25 - ori�� ���ϴ� �۾��� �ᱹ �ؾ��� particle�� ������ �°� �����ȴ�
		//�̰� �������� �۾��ؼ� Orient���� ������Ʈ�ؼ� ��������, �ᱹ Ŭ�󿡼� �� �۾��� ���� �������
		//�� �۾��� �ҷ��� �ѹ� ������ ���� �ϸ�ȴ�.
		//[1]ori���� �ҷ������� Ŭ��->����->Ŭ�� �� ������ �ѹ� �����ϴ°��� ���ɻ� �����ΰ� �ƴϸ� [2]Ŭ�󿡼� �̷��� �ѹ����ִ°� �����ΰ�
		//Ŭ�� -> ������ �� �� degree�� ori�� ���� ������Ʈ �� rotation���� ���޵ǹǷ� 
		//�������� �Ʒ��� �۾��� �������� ���� 
		//[2]�� �� ȿ�����ΰ� ����
		//[1]�� �ϸ�, ������Ŷ�� ũ�Ⱑ �þ ��Ŷ����� ���� -> �������Ϸ� �߻�,
		//           Ŭ���̾�Ʈ�� cpu���� ������ ���� gpu�� �̿�, �� �۾��� �߰��Ѵٰ��ؼ� ���ϰ� �߻����� ����

		auto v = Float3Add(xmf3_pos, XMFloat4to3(xmf4_pos), false);

		v = Float3Normalize(v);//���ο� �躤��(�߻����)
							   //���⼭ �躤�Ͷ� ����, �÷��̾ �ƴ϶�, �Ѿ��� �躤�ʹ�. ��� ������Ʈ�� ���� �躤�ʹ� 0,0,1 �Ǵ� 0,0,-1 �ε�, ���ư��� ������ �ٶ󺸵���  
							   //�ؾ��ϹǷ� ���ο� �躤�͸� �ʿ�� �ϴ°��̴�.  

							   //���� �躤�Ϳ� ���ο� �躤�͸� �����ؼ� �������� ���Ѵ�.
		XMFLOAT3 l{ 0,0,1 };
		XMVECTOR ol = XMLoadFloat3(&l);
		XMVECTOR nl = XMLoadFloat3(&v);

		auto axis = XMVector3Cross(ol, nl);

		//�������� �ϼ�.
		axis = XMVector3Normalize(axis);
		XMFLOAT3 Axis;
		XMStoreFloat3(&Axis, axis);
		//���� ȸ�������� ���ؾ��Ѵ�. ������ ���� ȸ�������� ���Ѵ�.

		auto temp = XMVector3Dot(ol, nl);

		float d;//���� �躤�Ϳ� ���ο� �躤�͸� ������ ���.
		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������

		auto ori = QuaternionRotation(Axis, d);

		//��¥ �躤�͸� �������� ���� ��¥ Right���͸� ���Ѵ�. ��¥ Up�� ��¥ ��� ��¥ ����Ʈ�� �������ϸ��.
		//ȸ���� �������ش�. ȸ���࿡�� �躤�͸� �����ϸ� ��¥ Right���Ͱ� ���´�.
		//�̶� ������ �ִ� RightVector�� ��¥ RightVector�� ���հ��� ����ϰ�
		//�躤�͸� ȸ�������� �����ش�. �� ȸ���࿡ ������ ����°�?
		//�����ϴ� ���� �躤�Ϳ� �߻������ �躤���� ȸ�����
		//���� RightVector�� �߻������ ȸ�������ϸ� ���� �ٸ��� ���´�.
		//������ ���� �׳� �̴�� �Ѿ�� �Ǹ� RightVector�� ��¥ RightVector�� ������ŭ ������ ����Ƿ�
		//�̷��ԵǸ� ���߿� Up�� ���Ҷ��� ������ �����.
		//��� X�� Y�� Z�� ������� �����ϸ� �̷��� ������ ���Ǿ�����, Ư������ ������� ȸ���Ҷ� ����� ������.

		auto wmatrix = XMMatrixIdentity();
		auto quater = XMLoadFloat4(&ori);
		wmatrix *= XMMatrixRotationQuaternion(quater);

		auto orr = XMVectorSet(1, 0, 0, 0);
		orr = XMVector4Transform(orr, wmatrix);//��¥ ����Ʈ ����
		orr = XMVector3Normalize(orr);
		auto RealRight = XMVector3Cross(axis, nl);//��¥ ����Ʈ����
		RealRight = XMVector3Normalize(RealRight);

		//��¥ ����Ʈ ���Ϳ� ��¥ ����Ʈ ���͸� ������.
		temp = XMVector3Dot(RealRight, orr);

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������
		auto ori2 = XMQuaternionRotationAxis(nl, d);//��¥ �躤�͸� ȸ�������� ��� ��¥����Ʈ�� ��¥����Ʈ�� ���հ���ŭȸ��

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//���� ȸ�� ����

		if (server_endbulldata.type == BULLET_TYPE::protocol_LightBullet)
			bul = new BulletCube(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, lock, PlayerObject->CenterPos, true);

		if (server_endbulldata.type == BULLET_TYPE::protocol_HeavyBullet)
			bul = new HeavyBulletCube(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, lock, PlayerObject->CenterPos, true);

	}
	break;

	default:
		break;
	}

	bul->m_bullet_data.master_id = server_endbulldata.master_id;
	bul->m_bullet_data.my_id = server_endbulldata.my_id;

	bul->CenterPos = XMFLOAT4(server_endbulldata.end_pos4f.x, server_endbulldata.end_pos4f.y, server_endbulldata.end_pos4f.z, server_endbulldata.end_pos4f.w);
	bul->pp->SetPosition(&bul->CenterPos);

	//bul->Orient = XMFLOAT4(server_bulldata.rot4f.x, server_bulldata.rot4f.y, server_bulldata.rot4f.z, server_bulldata.rot4f.w);
	//bul->UpdateLookVector();

	bul->m_bullet_data.alive = true;
	bul->m_bullet_data.degree = server_endbulldata.degree;
	bul->m_bullet_data.endpoint = server_endbulldata.endpoint;
	bul->m_bullet_data.master_id = server_endbulldata.master_id;
	bul->m_bullet_data.my_id = server_endbulldata.my_id;
	bul->m_bullet_data.pos4f = server_endbulldata.ori_pos4f;
	bul->m_bullet_data.type = server_endbulldata.type;
	bul->m_bullet_data.rot4f = { ori.x, ori.y, ori.z, ori.w };

	//�ҷ��� ������ ��ġ
	bul->m_end_bulletPos = { server_endbulldata.end_pos4f.x ,server_endbulldata.end_pos4f.y, server_endbulldata.end_pos4f.z, server_endbulldata.end_pos4f.w };
	//bul->ParticleList->back()->Orient = bul->Orient;
	//bul->ParticleList->back()->UpdateLookVector();

	bulletlist->push_back(bul);
}

void CPlayer::CreateOtherClientEndDicestrikeSkill(ID3D12Device * Device, ID3D12GraphicsCommandList * cl, const Position3D & Goal, CGameObject * lock, list<CGameObject*>* bulletlist, const STC_BulletObject_EndPos_Info& server_endbulldata, const XMFLOAT3 & OffLookvector)
{
	XMFLOAT4 xmf4_pos{ server_endbulldata.end_pos4f.x, server_endbulldata.end_pos4f.y, server_endbulldata.end_pos4f.z, server_endbulldata.end_pos4f.w };
	XMFLOAT3 xmf3_pos{ Goal.x, Goal.y, Goal.z };

	XMFLOAT3 l{ 0,0,1 };
	XMVECTOR ol = XMLoadFloat3(&l);
	XMVECTOR nl = XMLoadFloat3(&OffLookvector);
	auto axis = XMVector3Cross(ol, nl);
	//�������� �ϼ�.
	axis = XMVector3Normalize(axis);
	XMFLOAT3 Axis;
	XMStoreFloat3(&Axis, axis);
	//���� ȸ�������� ���ؾ��Ѵ�. ������ ���� ȸ�������� ���Ѵ�.

	auto temp = XMVector3Dot(ol, nl);

	float d;//���� �躤�Ϳ� ���ο� �躤�͸� ������ ���.
	XMStoreFloat(&d, temp);
	if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
		d = acos(d);//���� �ϼ�. ������

	auto ori = QuaternionRotation(Axis, d);

	//��¥ �躤�͸� �������� ���� ��¥ Right���͸� ���Ѵ�. ��¥ Up�� ��¥ ��� ��¥ ����Ʈ�� �������ϸ��.
	//ȸ���� �������ش�. ȸ���࿡�� �躤�͸� �����ϸ� ��¥ Right���Ͱ� ���´�.
	//�̶� ������ �ִ� RightVector�� ��¥ RightVector�� ���հ��� ����ϰ�
	//�躤�͸� ȸ�������� �����ش�. �� ȸ���࿡ ������ ����°�?
	//�����ϴ� ���� �躤�Ϳ� �߻������ �躤���� ȸ�����
	//���� RightVector�� �߻������ ȸ�������ϸ� ���� �ٸ��� ���´�.
	//������ ���� �׳� �̴�� �Ѿ�� �Ǹ� RightVector�� ��¥ RightVector�� ������ŭ ������ ����Ƿ�
	//�̷��ԵǸ� ���߿� Up�� ���Ҷ��� ������ �����.
	//��� X�� Y�� Z�� ������� �����ϸ� �̷��� ������ ���Ǿ�����, Ư������ ������� ȸ���Ҷ� ����� ������.

	auto wmatrix = XMMatrixIdentity();
	auto quater = XMLoadFloat4(&ori);
	wmatrix *= XMMatrixRotationQuaternion(quater);

	auto orr = XMVectorSet(1, 0, 0, 0);
	orr = XMVector4Transform(orr, wmatrix);//��¥ ����Ʈ ����
	orr = XMVector3Normalize(orr);
	auto RealRight = XMVector3Cross(axis, nl);//��¥ ����Ʈ����
	RealRight = XMVector3Normalize(RealRight);

	//��¥ ����Ʈ ���Ϳ� ��¥ ����Ʈ ���͸� ������.
	temp = XMVector3Dot(RealRight, orr);

	XMStoreFloat(&d, temp);
	if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
		d = acos(d);//���� �ϼ�. ������
	auto ori2 = XMQuaternionRotationAxis(nl, d);//��¥ �躤�͸� ȸ�������� ��� ��¥����Ʈ�� ��¥����Ʈ�� ���հ���ŭȸ��

	auto tempori = XMLoadFloat4(&ori);
	tempori = XMQuaternionMultiply(tempori, ori2);
	XMStoreFloat4(&ori, tempori);//���� ȸ�� ����

								 //cout << "CreateOtherClientBullet Master Lookvector xyz: " << OffLookvector.x << " , " << OffLookvector.y << " , " << OffLookvector.z << "\n";
								 //cout << "\n";

	CGameObject *bul = new DiceStrike(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, server_endbulldata.degree, NULL, PlayerObject->CenterPos);

	bul->m_bullet_data.master_id = server_endbulldata.master_id;
	bul->m_bullet_data.my_id = server_endbulldata.my_id;

	bul->CenterPos = XMFLOAT4(server_endbulldata.end_pos4f.x, server_endbulldata.end_pos4f.y, server_endbulldata.end_pos4f.z, server_endbulldata.end_pos4f.w);
	bul->pp->SetPosition(&bul->CenterPos);

	//bul->Orient = XMFLOAT4(server_bulldata.rot4f.x, server_bulldata.rot4f.y, server_bulldata.rot4f.z, server_bulldata.rot4f.w);
	//bul->UpdateLookVector();

	bul->m_bullet_data.alive = true;
	bul->m_bullet_data.degree = server_endbulldata.degree;
	bul->m_bullet_data.endpoint = server_endbulldata.endpoint;
	bul->m_bullet_data.master_id = server_endbulldata.master_id;
	bul->m_bullet_data.my_id = server_endbulldata.my_id;
	bul->m_bullet_data.pos4f = server_endbulldata.ori_pos4f;
	bul->m_bullet_data.type = server_endbulldata.type;
	bul->m_bullet_data.rot4f = { ori.x, ori.y, ori.z, ori.w };

	//�ҷ��� ������ ��ġ
	bul->m_end_bulletPos = { server_endbulldata.end_pos4f.x ,server_endbulldata.end_pos4f.y, server_endbulldata.end_pos4f.z, server_endbulldata.end_pos4f.w };

	//bul->ParticleList->back()->Orient = bul->Orient;
	//bul->ParticleList->back()->UpdateLookVector();

	bulletlist->push_back(bul);
}

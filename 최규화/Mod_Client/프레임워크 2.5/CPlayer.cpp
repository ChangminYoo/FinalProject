#include "CPlayer.h"
#include"Scene.h"

CPlayer::CPlayer(ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist, float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u) : Camera(Device,commandlist,asp,e,a,u)
{
	PlayerObject = NULL;
}


CPlayer::~CPlayer()
{
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
				if (mx >ox)//���������� �̵�
				{

					ytheta += (190 * MMPE_PI / 180)*DeltaTime;

					playerYtheta= (190 * MMPE_PI / 180)*DeltaTime;
					ox = mx;
					rotate = true;
				}
				else if (mx<ox)//�������� �̵�
				{
					ytheta += (-190 *MMPE_PI / 180)*DeltaTime;
					playerYtheta= (-190 * MMPE_PI / 180)*DeltaTime;
					ox = mx;
					rotate = true;
				}
				else
				{

					ox = mx;
				}



				if (my > oy)//�Ʒ����̵�
				{
					xtheta += (120 * 3.14 / 180)*DeltaTime;
					if (xtheta >= (85 * 3.14 / 180))
						xtheta = (85 * 3.14 / 180);



					oy = my;
				}
				else if (my<oy)//�����̵�
				{
					xtheta += (-120 * 3.14 / 180)*DeltaTime;

					if (xtheta <= (-65 * 3.14 / 180))
						xtheta = (-65 * 3.14 / 180);

					oy = my;
				}
				else
				{

					oy = my;
				}

				
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
	PlayerCameraReLocate();

}

//���⼭ �߿������� Ű���� ���� ��� �浹�˻縦 �ѹ� ������Ѵ�.
//�ֳ��ϸ� �̰� ������ ���� �� ���̱� ���ؼ��ε� ������� ��ü�� �� ���� ���ִٰ� ġ��.
//�̶� ��ȭ��ǥ�� ������ȭ��ǥ�� ������ �����δ� ������ �����ν������ϰ� ���������Ѵ�.
//�̸� ���� Ű���� �浹ó���� �Ѵ��� �浹�ϸ� �����·� �ǵ����� �ϴ°��̴�.
//���� �׻� pp�� ��ġ�� ����������Ѵ�.
void CPlayer::PlayerInput(float DeltaTime, Scene* scene)
{
	if (PlayerObject != NULL)
	{	
		bool move = false;

		if (GetKeyState(0x57) & 0x8000)//WŰ
		{
			move = true;

			//�躤���� +�������� �����δ�.
			auto l = XMLoadFloat3(&PlayerObject->Lookvector);
			l *= PlayerObject->gamedata.Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			auto tempP = p;
			p += l;

			
			XMStoreFloat4(&PlayerObject->CenterPos, p);
			PlayerObject->pp->SetPosition(PlayerObject->CenterPos);
			
			
				//�浹����Ʈ�� ����� ���� �˻��Ѵ�.
			//���̳��Ϳ�����Ʈ,����������Ʈ ���� �˻��ؾ���.
			for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
			{
					
				if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
					if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
						PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector())==true&&abs((*i)->pp->pAxis.y)!=1)//�浹������
					{
						//������ WŰ�� ������ �����θ�ŭ �ǵ�����������.
	
						XMStoreFloat4(&PlayerObject->CenterPos, tempP);
						PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

					}
			}
			//������ ��ü�� ��
			for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
			{

				if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
					if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
						PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
					{
						//������ WŰ�� ������ �����θ�ŭ �ǵ�����������.

						XMStoreFloat4(&PlayerObject->CenterPos, tempP);
						PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

					}
			}
			//������ �ٵ� �� ��
			for (auto i = scene->RigidObject.begin(); i != scene->RigidObject.end(); i++)
			{

			}

			
		}
		else if (GetKeyState(0x53) & 0x8000)//SŰ
		{
			move = true;

			//�躤���� -�������� �����δ�.
			auto l = XMLoadFloat3(&PlayerObject->Lookvector);
			l *= PlayerObject->gamedata.Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			auto tempP = p;
			p -= l;

			XMStoreFloat4(&PlayerObject->CenterPos, p);
			PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

			
			//�浹����Ʈ�� ����� ���� �˻��Ѵ�.
			//���̳��Ϳ�����Ʈ,����������Ʈ ���� �˻��ؾ���.
			for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
			{

				if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
					if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
						PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
					{
						//������ WŰ�� ������ �����θ�ŭ �ǵ�����������.

						XMStoreFloat4(&PlayerObject->CenterPos, tempP);
						PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

					}
			}
			//������ ��ü�� ��
			for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
			{

				if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
					if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
						PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
					{
						//������ WŰ�� ������ �����θ�ŭ �ǵ�����������.

						XMStoreFloat4(&PlayerObject->CenterPos, tempP);
						PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

					}
			}
			
		}

		if (GetKeyState(0x41) & 0x8000)//AŰ
		{
			move = true;

			//����Ʈ������ -�������� �����δ�.
			auto r = XMLoadFloat3(&PlayerObject->Rightvector);
			r *= PlayerObject->gamedata.Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			auto tempP = p;
			p -= r;

			XMStoreFloat4(&PlayerObject->CenterPos, p);
			PlayerObject->pp->SetPosition(PlayerObject->CenterPos);


			//�浹����Ʈ�� ����� ���� �˻��Ѵ�.
			//���̳��Ϳ�����Ʈ,����������Ʈ ���� �˻��ؾ���.
			for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
			{

				if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
					if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
						PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
					{
						//������ WŰ�� ������ �����θ�ŭ �ǵ�����������.

						XMStoreFloat4(&PlayerObject->CenterPos, tempP);
						PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

					}
			}
			//������ ��ü�� ��
			for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
			{

				if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
					if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
						PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
					{
						//������ WŰ�� ������ �����θ�ŭ �ǵ�����������.

						XMStoreFloat4(&PlayerObject->CenterPos, tempP);
						PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

					}
			}
		
		}
		else if (GetKeyState(0x44) & 0x8000)//DŰ
		{
			move = true;

			//����Ʈ������ +�������� �����δ�.
			auto r = XMLoadFloat3(&PlayerObject->Rightvector);
			r *= PlayerObject->gamedata.Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			auto tempP = p;//���� ��ġ
			p += r;

			
			XMStoreFloat4(&PlayerObject->CenterPos, p);
			PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

			//�浹����Ʈ�� ����� ���� �˻��Ѵ�.
			//���̳��Ϳ�����Ʈ,����������Ʈ ���� �˻��ؾ���.
			for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
			{

				if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
					if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
						PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
					{
						//������ WŰ�� ������ �����θ�ŭ �ǵ�����������.

						XMStoreFloat4(&PlayerObject->CenterPos, tempP);
						PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

					}
			}
			//������ ��ü�� ��
			for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
			{

				if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
					if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
						PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
					{
						//������ WŰ�� ������ �����θ�ŭ �ǵ�����������.

						XMStoreFloat4(&PlayerObject->CenterPos, tempP);
						PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

					}
			}

		}
		
		if (GetKeyState(VK_SPACE) & 0x8000 && PlayerObject->AirBone == false)
		{
			GeneratorJump j;
			j.SetJumpVel(XMFLOAT3(0, 80, 0));//���߿� �÷��̾��� �����¸�ŭ �߰��Ұ�
			j.Update(DeltaTime, *PlayerObject->pp);
			PlayerObject->AirBone = true;//���߻��¸� true��
		}


		STC_ChangedPos change_pos_ani;
		change_pos_ani.packet_size = sizeof(STC_ChangedPos);
		change_pos_ani.pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;
		
		change_pos_ani.id = PlayerObject->m_player_data.ID;
		change_pos_ani.pos = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };

		if (move == true)//�����̰� ������ �����̴� �������
		{
			if (PlayerObject->n_Animation != Ani_State::Attack)//���ݸ���� �ƴϸ� �ٽ� �����·�
			{
				PlayerObject->SetAnimation(Ani_State::Run);

				//
				m_async_client->RgCkInfo.PtCheck.PositionInfo = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };
				m_async_client->RgCkInfo.PtCheck.AniState = Ani_State::Run;


				//change_pos_ani.ani_state = Ani_State::Run;
				//m_async_client->SendPacket(reinterpret_cast<Packet*>(&change_pos_ani));
			}
		}
		else
		{
			if (PlayerObject->n_Animation != Ani_State::Attack)//���ݸ���� �ƴϸ� �ٽ� �����·�
			{
				PlayerObject->SetAnimation(Ani_State::Idle);

				//
				m_async_client->RgCkInfo.PtCheck.PositionInfo = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };
				m_async_client->RgCkInfo.PtCheck.AniState = Ani_State::Idle;

				//change_pos_ani.ani_state = Ani_State::Idle;
				//m_async_client->SendPacket(reinterpret_cast<Packet*>(&change_pos_ani));
			}

			m_async_client->RgCkInfo.PtCheck.Deltime = DeltaTime;
		}


		//if (key_flag)
		//{
		//	STC_ChangedPos change_pos;
		//	change_pos.id = PlayerObject->m_player_data.ID;
		//	change_pos.ani_state = Ani_State::Run;
		//	change_pos.pos = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };
		//
		//	m_async_client->SendPacket(reinterpret_cast<Packet*>(&change_pos));
		//
		//	key_flag = false;
		//}
	}
}

void CPlayer::CreateBullet(ID3D12Device* Device, ID3D12GraphicsCommandList* cl,XMFLOAT3 & Goal,CGameObject* lock, list<CGameObject*>* bulletlist)
{
	switch (SellectBulletNumber)
	{
	case 0://�ҷ�ť��


		auto v = Float3Add(Goal, XMFloat4to3(PlayerObject->CenterPos), false);
		
		v = Float3Normalize(v);//���ο� �躤��(�߻����)

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

		auto or = XMVectorSet(1, 0, 0, 0);
		or = XMVector4Transform(or , wmatrix);//��¥ ����Ʈ ����
		or = XMVector3Normalize(or );
		auto RealRight = XMVector3Cross(axis, nl);//��¥ ����Ʈ����
		RealRight = XMVector3Normalize(RealRight);

		//��¥ ����Ʈ ���Ϳ� ��¥ ����Ʈ ���͸� ������.
		temp = XMVector3Dot(RealRight, or );

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//�ݵ�� �� ����� -1~1 ���̿����Ѵ�. �׷��� ������ ��������.
			d = acos(d);//���� �ϼ�. ������
		auto ori2 = XMQuaternionRotationAxis(nl, d);//��¥ �躤�͸� ȸ�������� ��� ��¥����Ʈ�� ��¥����Ʈ�� ���հ���ŭȸ��

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//���� ȸ�� ����


		CGameObject* bul = new BulletCube(Device, cl, PlayerObject->ParticleList, PlayerObject, ori, lock, PlayerObject->CenterPos);

		bulletlist->push_back(bul);
		
		STC_Attack cts_att;
		cts_att.bull_data.LookOn_ID = -1;
		cts_att.bull_data.Master_ID = PlayerObject->m_player_data.ID;
		cts_att.bull_data.pos = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };
		cts_att.bull_data.Rotate_status = { PlayerObject->Orient.x, PlayerObject->Orient.y, PlayerObject->Orient.z, PlayerObject->Orient.w };
		cts_att.start_time = 0.f;



		//m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_att));
		break;

	}
}

#include "CPlayer.h"

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

					ytheta += (240 * MMPE_PI / 180)*DeltaTime;

					playerYtheta= (240 * MMPE_PI / 180)*DeltaTime;
					ox = mx;
					rotate = true;
				}
				else if (mx<ox)//�������� �̵�
				{
					ytheta += (-240 *MMPE_PI / 180)*DeltaTime;
					playerYtheta= (-240 * MMPE_PI / 180)*DeltaTime;
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
				PlayerObject->ThetaY += playerYtheta * 180 / MMPE_PI;
				
				
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
void CPlayer::PlayerInput(float DeltaTime)
{
	//�ӽ��߰�
	bool key_flag = false;

	if (PlayerObject != NULL)
	{
		if (GetKeyState(0x57) & 0x8000)//WŰ
		{
			key_flag = true;

			//�躤���� +�������� �����δ�.
			auto l = XMLoadFloat3(&PlayerObject->Lookvector);
			l *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p += l;

			XMFLOAT3 sp;
			XMStoreFloat3(&sp, l);
			XMStoreFloat4(&PlayerObject->CenterPos, p);
			PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

			if (PlayerObject->CollisionList != NULL)
			{
				//�浹����Ʈ�� ����� ���� �˻��Ѵ�.
				for (auto i = PlayerObject->CollisionList->begin(); i != PlayerObject->CollisionList->end(); i++)
				{

					if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
						{
							//������ WŰ�� ������ �����θ�ŭ �ǵ�����������.
							PlayerObject->pp->CollisionResolve(*(*i)->pp, sp, DeltaTime, true);
							PlayerObject->UpdatePPosCenterPos();//�浹�ؼ��� ���� �� ����� CenterPos�� �����Ѵ�.
						}
				}
			}

		}
		else if (GetKeyState(0x53) & 0x8000)//SŰ
		{
			key_flag = true;

			//�躤���� -�������� �����δ�.
			auto l = XMLoadFloat3(&PlayerObject->Lookvector);
			l *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p -= l;

			XMFLOAT3 sp;
			XMStoreFloat3(&sp, -l);
			XMStoreFloat4(&PlayerObject->CenterPos, p);
			PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

			if (PlayerObject->CollisionList != NULL)
			{
				//�浹����Ʈ�� ����� ���� �˻��Ѵ�.
				for (auto i = PlayerObject->CollisionList->begin(); i != PlayerObject->CollisionList->end(); i++)
				{

					if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
						{
							//������ SŰ�� ������ �����θ�ŭ �ǵ�����������.
							PlayerObject->pp->CollisionResolve(*(*i)->pp, sp, DeltaTime, true);
							PlayerObject->UpdatePPosCenterPos();//�浹�ؼ��� ���� �� ����� CenterPos�� �����Ѵ�.
						}
				}
			}
		}

		if (GetKeyState(0x41) & 0x8000)//AŰ
		{
			key_flag = true;

			//����Ʈ������ -�������� �����δ�.
			auto r = XMLoadFloat3(&PlayerObject->Rightvector);
			r *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p -= r;

			XMFLOAT3 sp;
			XMStoreFloat3(&sp, -r);
			XMStoreFloat4(&PlayerObject->CenterPos, p);
			PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

			if (PlayerObject->CollisionList != NULL)
			{
				//�浹����Ʈ�� ����� ���� �˻��Ѵ�.
				for (auto i = PlayerObject->CollisionList->begin(); i != PlayerObject->CollisionList->end(); i++)
				{

					if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
						{
							//������ AŰ�� ������ �����θ�ŭ �ǵ�����������.
							PlayerObject->pp->CollisionResolve(*(*i)->pp, sp, DeltaTime, true);
							PlayerObject->UpdatePPosCenterPos();//�浹�ؼ��� ���� �� ����� CenterPos�� �����Ѵ�.
						}
				}
			}
		}
		else if (GetKeyState(0x44) & 0x8000)//DŰ
		{
			key_flag = true;

			//����Ʈ������ +�������� �����δ�.
			auto r = XMLoadFloat3(&PlayerObject->Rightvector);
			r *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p += r;

			XMFLOAT3 sp;
			XMStoreFloat3(&sp, r);
			XMStoreFloat4(&PlayerObject->CenterPos, p);
			PlayerObject->pp->SetPosition(PlayerObject->CenterPos);

			if (PlayerObject->CollisionList != NULL)
			{
				//�浹����Ʈ�� ����� ���� �˻��Ѵ�.
				for (auto i = PlayerObject->CollisionList->begin(); i != PlayerObject->CollisionList->end(); i++)
				{

					if ((*i) != PlayerObject)//����Ʈ�� �ڱ��ڽ���������츦 ������.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//�浹������
						{
							//������ DŰ�� ������ �����θ�ŭ �ǵ�����������. �ٸ� pAxis�� z���ΰ��� ����
							PlayerObject->pp->CollisionResolve(*(*i)->pp, sp, DeltaTime, true);
							PlayerObject->UpdatePPosCenterPos();//�浹�ؼ��� ���� �� ����� CenterPos�� �����Ѵ�.
						}
				}
			}
		}

		if (key_flag)
		{
			STC_ChangedPos changed_pos;
			changed_pos.packet_size = sizeof(STC_ChangedPos);
			changed_pos.pack_type = PACKET_PROTOCOL_TYPE::CHANGED_PLAYER_POSITION;
			changed_pos.id = PlayerObject->m_player_data.ID;
			changed_pos.pos = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };

			m_async_client->SendPacket(reinterpret_cast<Packet*>(&changed_pos));
		}

	}
}

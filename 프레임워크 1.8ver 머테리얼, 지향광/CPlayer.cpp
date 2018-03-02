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
				
				 Xr*= Yr;
				
				XMVECTOR result=XMLoadFloat3(&oe);
				result=XMVector3Transform(result,Xr);
				//ȸ������� offEye�� �����. offEye�� ���������� Eye��ġ��.
				XMStoreFloat3(&Camera.offEye, result);
				
				//offAt�� ������ �����̴�.
				Camera.CamData.Look = XMFLOAT3(Camera.offAt.x - Camera.offEye.x, Camera.offAt.y - Camera.offEye.y, Camera.offAt.z - Camera.offEye.z);
				//�躤�ʹ� ���ȭ �س��.
				Camera.CamData.Look = Float3Normalize(Camera.CamData.Look);
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

void CPlayer::PlayerInput(float DeltaTime)
{
	if (PlayerObject != NULL)
	{


		if (GetKeyState(0x57) & 0x8000)//WŰ
		{
			//�躤���� +�������� �����δ�.
			auto l = XMLoadFloat3(&PlayerObject->Lookvector);
			l *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p += l;

			XMStoreFloat4(&PlayerObject->CenterPos, p);

		}
		else if (GetKeyState(0x53) & 0x8000)//SŰ
		{
			//�躤���� -�������� �����δ�.
			auto l = XMLoadFloat3(&PlayerObject->Lookvector);
			l *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p -= l;

			XMStoreFloat4(&PlayerObject->CenterPos, p);
		}


		if (GetKeyState(0x41) & 0x8000)//AŰ
		{
			//����Ʈ������ -�������� �����δ�.
			auto r = XMLoadFloat3(&PlayerObject->Rightvector);
			r *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p -= r;

			XMStoreFloat4(&PlayerObject->CenterPos, p);

		}
		else if (GetKeyState(0x44) & 0x8000)//DŰ
		{
			//����Ʈ������ +�������� �����δ�.
			auto r = XMLoadFloat3(&PlayerObject->Rightvector);
			r *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p += r;

			XMStoreFloat4(&PlayerObject->CenterPos, p);

		}
	}
}

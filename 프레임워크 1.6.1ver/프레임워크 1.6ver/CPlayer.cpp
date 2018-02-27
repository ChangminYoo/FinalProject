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
			if (ox == -1 ||oy == -1)//마우스 좌표를 먼저 저장한다. 초기화임
			{
				ox = mx;
				oy = my;
				
			}
			else
			{
				if (mx >ox)//오른쪽으로 이동
				{

					ytheta += (240 * MMPE_PI / 180)*DeltaTime;

					playerYtheta= (240 * MMPE_PI / 180)*DeltaTime;
					ox = mx;
					rotate = true;
				}
				else if (mx<ox)//왼쪽으로 이동
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



				if (my > oy)//아래로이동
				{
					xtheta += (120 * 3.14 / 180)*DeltaTime;
					if (xtheta >= (85 * 3.14 / 180))
						xtheta = (85 * 3.14 / 180);



					oy = my;
				}
				else if (my<oy)//위로이동
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

				
				//1. AtToEye만큼 이동시킨다.
				XMFLOAT3 oe(0, 0, -Camera.CamData.AtToEye);
				
				//2. 각도만큼 회전시킨다.

				XMMATRIX Xr;
				Xr=XMMatrixRotationX(xtheta);


				XMMATRIX Yr;
				Yr=XMMatrixRotationY(ytheta);


				//y축회전은 저장해둠

				//플레이어를 회전시켜둠. 먼저 -180도 회전시켜서 뒷모습을 보이게한다
				//이후에 카메라가 회전한만큼 추가적으로 움직이게함
				PlayerObject->ThetaY += playerYtheta * 180 / MMPE_PI;
				
				 Xr*= Yr;
				
				XMVECTOR result=XMLoadFloat3(&oe);
				result=XMVector3Transform(result,Xr);
				//회전결과를 offEye로 만든다. offEye는 원점에서의 Eye위치다.
				XMStoreFloat3(&Camera.offEye, result);
				
				//offAt은 여전히 원점이다.
				Camera.CamData.Look = XMFLOAT3(Camera.offAt.x - Camera.offEye.x, Camera.offAt.y - Camera.offEye.y, Camera.offAt.z - Camera.offEye.z);
				//룩벡터는 노멀화 해논다.
				Camera.CamData.Look = Float3Normalize(Camera.CamData.Look);
			}
		
			//변경된 카메라 위치를 적용한다.
			PlayerCameraReLocate();
			

}

void CPlayer::PlayerCameraReLocate()//카메라 위치를 재설정한다.
{
	if (PlayerObject != NULL)
	{

		auto e=XMLoadFloat3(&Camera.offEye);
		auto p = XMLoadFloat4(&PlayerObject->CenterPos);
		auto a = XMLoadFloat3(&Camera.offAt);

		//카메라 위치는 플레이어오브젝트 + 오프셋 eye이다. 오프셋 eye는 쉽게 말해서 플레이어가 원점에있을때 카메라위치다.
		//나중에 카메라가 회전할때 CamData의 Eye나 At을 바꾸는게아니라
		//off값들을 회전시켜야한다.
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


		if (GetKeyState(0x57) & 0x8000)//W키
		{
			//룩벡터의 +방향으로 움직인다.
			auto l = XMLoadFloat3(&PlayerObject->Lookvector);
			l *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p += l;

			XMStoreFloat4(&PlayerObject->CenterPos, p);

		}
		else if (GetKeyState(0x53) & 0x8000)//S키
		{
			//룩벡터의 -방향으로 움직인다.
			auto l = XMLoadFloat3(&PlayerObject->Lookvector);
			l *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p -= l;

			XMStoreFloat4(&PlayerObject->CenterPos, p);
		}


		if (GetKeyState(0x41) & 0x8000)//A키
		{
			//라이트벡터의 -방향으로 움직인다.
			auto r = XMLoadFloat3(&PlayerObject->Rightvector);
			r *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p -= r;

			XMStoreFloat4(&PlayerObject->CenterPos, p);

		}
		else if (GetKeyState(0x44) & 0x8000)//D키
		{
			//라이트벡터의 +방향으로 움직인다.
			auto r = XMLoadFloat3(&PlayerObject->Rightvector);
			r *= PlayerObject->Speed*DeltaTime;
			auto p = XMLoadFloat4(&PlayerObject->CenterPos);
			p += r;

			XMStoreFloat4(&PlayerObject->CenterPos, p);

		}
	}
}

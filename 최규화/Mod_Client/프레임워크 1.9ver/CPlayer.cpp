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
				
				
				 XMVECTOR result = XMLoadFloat3(&oe);
				 

				 //Xr*= Yr;
				 //result = XMVector3Transform(result, Xr);
				 ////회전결과를 offEye로 만든다. offEye는 원점에서의 Eye위치다.
				 //XMStoreFloat3(&Camera.offEye, result);

				 //쿼터니언 버전.
				 auto result2 = result;
				 auto rpy = XMQuaternionRotationRollPitchYaw(xtheta, ytheta, 0);
				 result2 = XMVector3Transform(result2, XMMatrixRotationQuaternion(rpy));
				 XMStoreFloat3(&Camera.offEye, result2);

				 //offAt은 여전히 원점이다.
				 Camera.CamData.Look = XMFLOAT3(Camera.offAt.x - Camera.offEye.x, Camera.offAt.y - Camera.offEye.y, Camera.offAt.z - Camera.offEye.z);
				 //룩벡터는 노멀화 해논다.
				 Camera.CamData.Look = Float3Normalize(Camera.CamData.Look);

				 //
				 ////쿼터니언 테스트 코드.
				 //
				 //auto xr = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), xtheta);
				 //auto yr = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), ytheta);
				 //XMVECTOR test = XMVectorSet(1, 0, 0, 0);
				 //auto test2 = test;
				 //
				 //auto t=XMVector4Transform(test, Xr);//그냥 회전행렬곱한 결과(즉 원본 비교대상)
				 ////***********V*************8
				 ////x축으로xtheta , y축ㅇ로 ytheta만큼 회전한 쿼터니언끼리 순서대로 곱하고 그걸 행렬로 표현한것의 곱
				 //auto xy = XMMatrixRotationQuaternion(XMQuaternionMultiply(xr,yr));//원본과 같은값나옴.
				 //auto vv = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(xtheta, ytheta, 0));//동일
				 ////쿼터니언을 곱하고 그 켤레쿼터니언을 곱해봄
				 //auto test3 = test2;
				 //auto qm = XMQuaternionMultiply(xr, yr);
				 //auto qms = XMQuaternionConjugate(qm);
				 //test2=XMQuaternionMultiply(qm, test2);
				 ////결과가 값은 다 맞는데, 부호가 좀 이상하게 나옴. 따라서 쿼터니언 쓸거면 ***V*** 인 방식을 쓰자.
				 //
				 //test2 = XMQuaternionMultiply(test2, qms);
				 //auto tt=XMVector4Transform(test3, xy);

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

//여기서 중요한점은 키보드 누른 즉시 충돌검사를 한번 해줘야한다.
//왜냐하면 이건 게임의 질을 좀 높이기 위해서인데 예를들어 물체가 내 앞쪽 에있다고 치자.
//이때 위화살표와 오른쪽화살표를 누르면 앞으로는 못가도 옆으로스무스하게 움직여야한다.
//이를 위해 키마다 충돌처리를 한다음 충돌하면 원상태로 되돌리고 하는것이다.
//또한 항상 pp의 위치를 갱신해줘야한다.
void CPlayer::PlayerInput(float DeltaTime)
{
	//임시추가
	bool key_flag = false;

	if (PlayerObject != NULL)
	{
		if (GetKeyState(0x57) & 0x8000)//W키
		{
			key_flag = true;

			//룩벡터의 +방향으로 움직인다.
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
				//충돌리스트의 목록을 전부 검사한다.
				for (auto i = PlayerObject->CollisionList->begin(); i != PlayerObject->CollisionList->end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 W키를 눌러서 움직인만큼 되돌려보내야함.
							PlayerObject->pp->CollisionResolve(*(*i)->pp, sp, DeltaTime, true);
							PlayerObject->UpdatePPosCenterPos();//충돌해소한 다음 그 결과를 CenterPos에 저장한다.
						}
				}
			}

		}
		else if (GetKeyState(0x53) & 0x8000)//S키
		{
			key_flag = true;

			//룩벡터의 -방향으로 움직인다.
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
				//충돌리스트의 목록을 전부 검사한다.
				for (auto i = PlayerObject->CollisionList->begin(); i != PlayerObject->CollisionList->end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 S키를 눌러서 움직인만큼 되돌려보내야함.
							PlayerObject->pp->CollisionResolve(*(*i)->pp, sp, DeltaTime, true);
							PlayerObject->UpdatePPosCenterPos();//충돌해소한 다음 그 결과를 CenterPos에 저장한다.
						}
				}
			}
		}

		if (GetKeyState(0x41) & 0x8000)//A키
		{
			key_flag = true;

			//라이트벡터의 -방향으로 움직인다.
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
				//충돌리스트의 목록을 전부 검사한다.
				for (auto i = PlayerObject->CollisionList->begin(); i != PlayerObject->CollisionList->end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 A키를 눌러서 움직인만큼 되돌려보내야함.
							PlayerObject->pp->CollisionResolve(*(*i)->pp, sp, DeltaTime, true);
							PlayerObject->UpdatePPosCenterPos();//충돌해소한 다음 그 결과를 CenterPos에 저장한다.
						}
				}
			}
		}
		else if (GetKeyState(0x44) & 0x8000)//D키
		{
			key_flag = true;

			//라이트벡터의 +방향으로 움직인다.
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
				//충돌리스트의 목록을 전부 검사한다.
				for (auto i = PlayerObject->CollisionList->begin(); i != PlayerObject->CollisionList->end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 D키를 눌러서 움직인만큼 되돌려보내야함. 다만 pAxis가 z축인경우는 제외
							PlayerObject->pp->CollisionResolve(*(*i)->pp, sp, DeltaTime, true);
							PlayerObject->UpdatePPosCenterPos();//충돌해소한 다음 그 결과를 CenterPos에 저장한다.
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

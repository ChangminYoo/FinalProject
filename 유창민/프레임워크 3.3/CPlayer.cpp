#include "CPlayer.h"
#include"Scene.h"

CPlayer::CPlayer(HWND hWnd,ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist, float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u) : Camera(hWnd,Device,commandlist,asp,e,a,u)
{
	//벽대신 다른 오브젝트를 추가할것.
	TraceObject = new RangeObject(Device, commandlist, NULL, NULL, XMFLOAT4(-10000, -10000, -10000, 1));
	PlayerObject = NULL;


	skilldata.Skills[0] = 0;
	skilldata.Skills[1] = 7; //1
	skilldata.Skills[2] = 5; //2
	skilldata.Skills[3] = 6;
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
			if (ox == -1 ||oy == -1)//마우스 좌표를 먼저 저장한다. 초기화임
			{
				ox = mx;
				oy = my;
			}
			else
			{
				if (mx- ox >1)//오른쪽으로 이동
				{
					//차이가 클수록 더 빨리 회전한다. 즉 차이가 작으면 별로 안움직이므로 흔들림이 보정된다.
					float dx = fabsf(mx - ox);
					if (dx > 10)
						dx = 10;

						ytheta += ((dx/10) * MMPE_PI* 180 / 180)*DeltaTime;

						playerYtheta = ((dx / 10) * MMPE_PI * 180 / 180)*DeltaTime;
						ox = mx;
						rotate = true;
				
					
				}
				else if (mx- ox<-1)//왼쪽으로 이동
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



				if (my- oy > 1)//아래로이동
				{
					float dy = fabsf(my - oy);
					if (dy > 10)
						dy = 10;
						xtheta += ((dy/10) * MMPE_PI*100 / 180)*DeltaTime;
						if (xtheta >= (85 * 3.14 / 180))
							xtheta = (85 * 3.14 / 180);



						oy = my;
					
				}
				else if (my- oy<-1)//위로이동
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
				//PlayerObject->ThetaY += playerYtheta * 180 / MMPE_PI;
				
				
				
				XMFLOAT3 axis{ 0,1,0 };
				auto q2 = QuaternionRotation(axis, playerYtheta);
				PlayerObject->Orient = QuaternionMultiply(PlayerObject->Orient, q2);
			
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
void CPlayer::PlayerInput(float DeltaTime, Scene* scene)
{
	if (scene->GetGameState() == GS_PLAY)
	{
		
		if (PlayerObject != NULL && PlayerObject->gamedata.HP > 0 && GetFocus())
		{


			bool move = false;
			if (GetKeyState(0x57) & 0x8000)//W키
			{
				move = true;
				//룩벡터의 +방향으로 움직인다.
				auto l = XMLoadFloat3(&PlayerObject->Lookvector);
				l *= PlayerObject->gamedata.Speed*DeltaTime;
				auto p = XMLoadFloat4(&PlayerObject->CenterPos);
				auto tempP = p;
				p += l;


				XMStoreFloat4(&PlayerObject->CenterPos, p);
				


				//충돌리스트의 목록을 전부 검사한다.
			//다이나믹오브젝트,고정오브젝트 등을 검사해야함.
				for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 W키를 눌러서 움직인만큼 되돌려보내야함.

							XMStoreFloat4(&PlayerObject->CenterPos, tempP);
							

						}
				}
				//고정된 물체와 비교
				for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 W키를 눌러서 움직인만큼 되돌려보내야함.

							XMStoreFloat4(&PlayerObject->CenterPos, tempP);
							

						}
				}
				//리지드 바디 와 비교
				for (auto i = scene->RigidObject.begin(); i != scene->RigidObject.end(); i++)
				{

				}


			}
			else if (GetKeyState(0x53) & 0x8000)//S키
			{
				move = true;
				//룩벡터의 -방향으로 움직인다.
				auto l = XMLoadFloat3(&PlayerObject->Lookvector);
				l *= PlayerObject->gamedata.Speed*DeltaTime;
				auto p = XMLoadFloat4(&PlayerObject->CenterPos);
				auto tempP = p;
				p -= l;

				XMStoreFloat4(&PlayerObject->CenterPos, p);
				


				//충돌리스트의 목록을 전부 검사한다.
				//다이나믹오브젝트,고정오브젝트 등을 검사해야함.
				for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 W키를 눌러서 움직인만큼 되돌려보내야함.

							XMStoreFloat4(&PlayerObject->CenterPos, tempP);
							

						}
				}
				//고정된 물체와 비교
				for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 W키를 눌러서 움직인만큼 되돌려보내야함.

							XMStoreFloat4(&PlayerObject->CenterPos, tempP);
							

						}
				}

			}

			if (GetKeyState(0x41) & 0x8000)//A키
			{
				move = true;
				//라이트벡터의 -방향으로 움직인다.
				auto r = XMLoadFloat3(&PlayerObject->Rightvector);
				r *= PlayerObject->gamedata.Speed*DeltaTime;
				auto p = XMLoadFloat4(&PlayerObject->CenterPos);
				auto tempP = p;
				p -= r;

				XMStoreFloat4(&PlayerObject->CenterPos, p);
				


				//충돌리스트의 목록을 전부 검사한다.
				//다이나믹오브젝트,고정오브젝트 등을 검사해야함.
				for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 W키를 눌러서 움직인만큼 되돌려보내야함.

							XMStoreFloat4(&PlayerObject->CenterPos, tempP);
							

						}
				}
				//고정된 물체와 비교
				for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 W키를 눌러서 움직인만큼 되돌려보내야함.

							XMStoreFloat4(&PlayerObject->CenterPos, tempP);
							

						}
				}

			}
			else if (GetKeyState(0x44) & 0x8000)//D키
			{
				move = true;
				//라이트벡터의 +방향으로 움직인다.
				auto r = XMLoadFloat3(&PlayerObject->Rightvector);
				r *= PlayerObject->gamedata.Speed*DeltaTime;
				auto p = XMLoadFloat4(&PlayerObject->CenterPos);
				auto tempP = p;//기존 위치
				p += r;


				XMStoreFloat4(&PlayerObject->CenterPos, p);
				

				//충돌리스트의 목록을 전부 검사한다.
				//다이나믹오브젝트,고정오브젝트 등을 검사해야함.
				for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 W키를 눌러서 움직인만큼 되돌려보내야함.

							XMStoreFloat4(&PlayerObject->CenterPos, tempP);
							

						}
				}
				//고정된 물체와 비교
				for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
				{

					if ((*i) != PlayerObject)//리스트에 자기자신이있을경우를 제외함.
						if (PlayerObject->pp->CollisionTest(*(*i)->pp, PlayerObject->Lookvector, PlayerObject->Rightvector,
							PlayerObject->GetUpvector(), (*i)->Lookvector, (*i)->Rightvector, (*i)->GetUpvector()) == true && abs((*i)->pp->pAxis.y) != 1)//충돌했으면
						{
							//기존에 W키를 눌러서 움직인만큼 되돌려보내야함.

							XMStoreFloat4(&PlayerObject->CenterPos, tempP);
						

						}
				}

			}

			if (GetKeyState(VK_SPACE) & 0x8000 && PlayerObject->AirBone == false)
			{
				GeneratorJump j;
				j.SetJumpVel(XMFLOAT3(0, 80, 0));//나중에 플레이어의 점프력만큼 추가할것
				j.Update(DeltaTime, *PlayerObject->pp);
				PlayerObject->AirBone = true;//공중상태를 true로
			}

			if (move == true)//움직이고 있으면 움직이는 모션으로
			{
				if (PlayerObject->n_Animation != Ani_State::Attack)//공격모션이 아니면 다시 대기상태로
					PlayerObject->SetAnimation(Ani_State::Run);
			}
			else
			{
				if (PlayerObject->n_Animation != Ani_State::Attack)//공격모션이 아니면 다시 대기상태로
					PlayerObject->SetAnimation(Ani_State::Idle);
			}

			if (GetAsyncKeyState(0x31) & 0x0001)
				skilldata.SellectBulletIndex = 0;
			else if (GetAsyncKeyState(0x32) & 0x0001)
				skilldata.SellectBulletIndex = 1;
			else if (GetAsyncKeyState(0x33) & 0x0001)
				skilldata.SellectBulletIndex = 2;
			else if (GetAsyncKeyState(0x34) & 0x0001)
				skilldata.SellectBulletIndex = 3;


			
			//위의 버튼을 눌렀을때 생존기 스킬인 넘버링 4와 5인 경우(4는 파동파 , 5는 방어력증가(?))
			if (skilldata.Skills[skilldata.SellectBulletIndex] == 4 && skilldata.isSkillOn[skilldata.SellectBulletIndex])//파동파인경우
			{
				//고리생성
				scene->StaticObject.push_back(new RingObject(scene->device, scene->commandlist,&scene->BbObject, &scene->Shadows, PlayerObject->CenterPos));
				skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
				skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;
				skilldata.SellectBulletIndex = 0;//스킬 시전후 가장 첫번째 스킬로 변경함

				XMFLOAT4 파동파위치 = PlayerObject->CenterPos;
				파동파위치.y += -PlayerObject->pp->GetHalfBox().y;
				float impurse = 20000;
				float rad = 150.f;//범위
				//다이나믹 오브젝트와 리지드오브젝트를 날려버린다.
				for (auto l : scene->DynamicObject)
				{
					if (l != PlayerObject)
					{
						auto l2 = Float3Add(XMFloat4to3(l->CenterPos), XMFloat4to3(파동파위치), false);
						
						if (FloatLength(l2) <= rad)//범위 안에적이있으면
						{
							l2 = Float3Normalize(l2);
							l2 = Float3Float(l2, impurse*(1-FloatLength(l2)/rad));
							l->pp->AddForce(l2);
							l->pp->integrate(0.01f);
						}

					}
				}
				for (auto l : scene->RigidObject)
				{
					auto l2 = Float3Add(XMFloat4to3(l->CenterPos), XMFloat4to3(파동파위치), false);

					if (FloatLength(l2) <= rad)//범위 안에적이있으면
					{
						l2 = Float3Normalize(l2);
						l2 = Float3Float(l2, impurse*(1 - FloatLength(l2) / rad));
						l->rb->AddForce(l2);
						l->rb->integrate(0.01f);
					}

				}
				
			}
			else if (skilldata.Skills[skilldata.SellectBulletIndex] == 5 && skilldata.isSkillOn[skilldata.SellectBulletIndex])//실드
			{
				//실드 생성
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
			//키를 누를때마다 해당 스킬을 검사해서 추적오브젝트가 있어야 하는지 검사한다.
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

}

void CPlayer::CreateBullet(ID3D12Device* Device, ID3D12GraphicsCommandList* cl,XMFLOAT3 & Goal,CGameObject* lock, list<CGameObject*>* bulletlist)
{
	//현재선택된 0~3번 마법중 하나를 인덱스로 넣어주면 그 Skills[인덱스]의 결과가 바로 어떤 마법인지를 나타냄.

	switch (skilldata.Skills[skilldata.SellectBulletIndex])
	{

	case 0://불렛큐브(라이트 큐브)
	{

		PlayerObject->SetAnimation(2);
		//먼저 해당스킬의 쿨타임을 넣어주자.
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;


		auto v = Float3Add(Goal, XMFloat4to3(PlayerObject->CenterPos), false);

		v = Float3Normalize(v);//새로운 룩벡터(발사방향)
		//여기서 룩벡터라 함은, 플레이어가 아니라, 총알의 룩벡터다. 모든 오브젝트는 보통 룩벡터는 0,0,1 또는 0,0,-1 인데, 날아가는 방향을 바라보도록  
		//해야하므로 새로운 룩벡터를 필요로 하는것이다.  

		//기존 룩벡터와 새로운 룩벡터를 외적해서 방향축을 구한다.
		XMFLOAT3 l{ 0,0,1 };
		XMVECTOR ol = XMLoadFloat3(&l);
		XMVECTOR nl = XMLoadFloat3(&v);
		auto axis = XMVector3Cross(ol, nl);
		//방향축을 완성.
		axis = XMVector3Normalize(axis);
		XMFLOAT3 Axis;
		XMStoreFloat3(&Axis, axis);
		//이제 회전각도를 구해야한다. 내적을 통해 회전각도를 구한다.

		auto temp = XMVector3Dot(ol, nl);

		float d;//기존 룩벡터와 새로운 룩벡터를 내적한 결과.
		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
			d = acos(d);//각도 완성. 라디안임

		auto ori = QuaternionRotation(Axis, d);

		//진짜 룩벡터를 구했으니 이제 진짜 Right벡터를 구한다. 진짜 Up은 진짜 룩과 진짜 라이트를 외적만하면됨.
		//회전을 보정해준다. 회전축에서 룩벡터를 외적하면 진짜 Right벡터가 나온다.
		//이때 오차가 있는 RightVector를 진짜 RightVector의 사잇각을 계산하고
		//룩벡터를 회전축으로 돌려준다. 왜 회전축에 오차가 생기는가?
		//간단하다 기존 룩벡터와 발사방향을 룩벡터의 회전축과
		//기존 RightVector와 발사방향을 회전축을하면 서로 다르게 나온다.
		//문제는 만약 그냥 이대로 넘어가게 되면 RightVector와 진짜 RightVector의 각도만큼 오차가 생기므로
		//이렇게되면 나중에 Up을 구할때도 문제가 생긴다.
		//사실 X축 Y축 Z축 순서대로 곱을하면 이러한 문제는 거의없지만, 특정축을 기반으로 회전할때 생기는 문제다.

		auto wmatrix = XMMatrixIdentity();
		auto quater = XMLoadFloat4(&ori);
		wmatrix *= XMMatrixRotationQuaternion(quater);

		auto orr = XMVectorSet(1, 0, 0, 0);
		orr = XMVector4Transform(orr , wmatrix);//가짜 라이트 벡터
		orr = XMVector3Normalize(orr );
		auto RealRight = XMVector3Cross(axis, nl);//진짜 라이트벡터
		RealRight = XMVector3Normalize(RealRight);

		//진짜 라이트 벡터와 가짜 라이트 벡터를 내적함.
		temp = XMVector3Dot(RealRight, orr );

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
			d = acos(d);//각도 완성. 라디안임
		auto ori2 = XMQuaternionRotationAxis(nl, d);//진짜 룩벡터를 회전축으로 삼고 진짜라이트와 가짜라이트의 사잇각만큼회전

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//최종 회전 방향
 

		bulletlist->push_back(new BulletCube(Device, cl, PlayerObject->ParticleList,NULL, PlayerObject, ori, lock, PlayerObject->CenterPos));
		break;
	}

	case 1://불렛큐브(헤비 큐브)
	{
		PlayerObject->SetAnimation(2);

		//먼저 해당스킬의 쿨타임을 넣어주자.
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;


		auto v = Float3Add(Goal, XMFloat4to3(PlayerObject->CenterPos), false);

		v = Float3Normalize(v);//새로운 룩벡터(발사방향)
							   //여기서 룩벡터라 함은, 플레이어가 아니라, 총알의 룩벡터다. 모든 오브젝트는 보통 룩벡터는 0,0,1 또는 0,0,-1 인데, 날아가는 방향을 바라보도록  
							   //해야하므로 새로운 룩벡터를 필요로 하는것이다.  

							   //기존 룩벡터와 새로운 룩벡터를 외적해서 방향축을 구한다.
		XMFLOAT3 l{ 0,0,1 };
		XMVECTOR ol = XMLoadFloat3(&l);
		XMVECTOR nl = XMLoadFloat3(&v);
		auto axis = XMVector3Cross(ol, nl);
		//방향축을 완성.
		axis = XMVector3Normalize(axis);
		XMFLOAT3 Axis;
		XMStoreFloat3(&Axis, axis);
		//이제 회전각도를 구해야한다. 내적을 통해 회전각도를 구한다.

		auto temp = XMVector3Dot(ol, nl);

		float d;//기존 룩벡터와 새로운 룩벡터를 내적한 결과.
		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
			d = acos(d);//각도 완성. 라디안임

		auto ori = QuaternionRotation(Axis, d);

		auto wmatrix = XMMatrixIdentity();
		auto quater = XMLoadFloat4(&ori);
		wmatrix *= XMMatrixRotationQuaternion(quater);

		auto orr = XMVectorSet(1, 0, 0, 0);
		orr = XMVector4Transform(orr , wmatrix);//가짜 라이트 벡터
		orr = XMVector3Normalize(orr );
		auto RealRight = XMVector3Cross(axis, nl);//진짜 라이트벡터
		RealRight = XMVector3Normalize(RealRight);

		//진짜 라이트 벡터와 가짜 라이트 벡터를 내적함.
		temp = XMVector3Dot(RealRight, orr );

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
			d = acos(d);//각도 완성. 라디안임
		auto ori2 = XMQuaternionRotationAxis(nl, d);//진짜 룩벡터를 회전축으로 삼고 진짜라이트와 가짜라이트의 사잇각만큼회전

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//최종 회전 방향


		
		bulletlist->push_back(new HeavyBulletCube(Device, cl, PlayerObject->ParticleList,NULL, PlayerObject, ori, lock, PlayerObject->CenterPos));
		break;

	}
	case 2://테트라이크
	{
		PlayerObject->SetAnimation(2);
		//먼저 해당스킬의 쿨타임을 넣어주자.
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;

		
		bulletlist->push_back(new Tetrike(Device, cl, PlayerObject->ParticleList,NULL, bulletlist, PlayerObject, lock, XMFloat3to4(Goal)));
		break;

	}
	case 3://다이스트라이크
	{

		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;

		PlayerObject->ParticleList->push_back(new DiceObject(Device, cl, PlayerObject->ParticleList,NULL, PlayerObject,Goal, bulletlist ,XMFLOAT4(PlayerObject->CenterPos.x, 35, PlayerObject->CenterPos.z, 0)));

		break;
	}
	case 6://해머불렛
	{
		PlayerObject->SetAnimation(2);
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;

		bulletlist->push_back(new HammerBullet(Device, cl, PlayerObject->ParticleList, NULL,NULL,3, PlayerObject, XMFLOAT4(0,0,0,1),NULL, PlayerObject->CenterPos,XMFLOAT4(0,0,20,0)));
		

		break;
	}
	case 7:
	{
		PlayerObject->SetAnimation(2);
		//먼저 해당스킬의 쿨타임을 넣어주자.
		skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
		skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;


		bulletlist->push_back(new MeteorObject(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, XMFLOAT4(0,0,0,1), XMFloat3to4(Goal)));
		break;
	}

	}

}

void CPlayer::CheckTraceSkill()
{
	switch (skilldata.Skills[ skilldata.SellectBulletIndex])
	{

	case 2://넘버링이 2인 (테트라이크) 스킬일 경우.
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

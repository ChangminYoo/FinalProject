#include "CPlayer.h"
#include"Scene.h"

CPlayer::CPlayer(HWND hWnd,ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist, float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u) : Camera(hWnd,Device,commandlist,asp,e,a,u)
{
	//벽대신 다른 오브젝트를 추가할것.
	TraceObject = new RangeObject(Device, commandlist, NULL, NULL, XMFLOAT4(-10000, -10000, -10000, 1));
	PlayerObject = NULL;
	
	
	
	//스킬넘버링 0 : 라이트 큐브
	//	스킬넘버링 1 : 헤비 큐브
	//	스킬넘버링 2 : 테트라이크
	//	스킬넘버링 3 : 다이스트라이크
	//	스킬넘버링 4 : 파동파
	//	스킬넘버링 5 : 실드
	//	스킬넘버링 6 : 해머불렛
	//	스킬넘버링 7 : 메테오
		
	//이 수들을 바꾸면 스킬이 달라짐 
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
	((CCubeManObject*)obj)->player = this;
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

			STC_CharMove cts_move;
			STC_CharAnimation cts_cani;

			if (GetAsyncKeyState(0x57) & 0x8000)//W키
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

			if (GetAsyncKeyState(0x53) & 0x8000)//S키
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

			if (GetAsyncKeyState(0x41) & 0x8000)//A키
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

			if (GetKeyState(0x44) & 0x8000)//D키
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

			//캐릭터가 이동을 할 때, 즉 키를 누를때 눌렀다는 정보를 서버로 보냄		
			if (move == true)//움직이고 있으면 움직이는 모션으로
			{
				if (PlayerObject->n_Animation != Ani_State::Attack)//공격모션이 아니면 다시 대기상태로
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
				if (PlayerObject->n_Animation != Ani_State::Attack)//공격모션이 아니면 다시 대기상태로
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

						//점프 애니메이션 추가 시 이곳에 넣어준다
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


			
			//위의 버튼을 눌렀을때 생존기 스킬인 넘버링 4와 5인 경우(4는 파동파 , 5는 방어력증가(?))
			if (skilldata.Skills[skilldata.SellectBulletIndex] == 4 && skilldata.isSkillOn[skilldata.SellectBulletIndex])//파동파인경우
			{
				CTS_SKILL_WAVESHOCK cts_skill_waveshock;
				cts_skill_waveshock.skill_data.master_id = PlayerObject->m_player_data.id;
				cts_skill_waveshock.skill_data.my_id = skilldata.Skills[skilldata.SellectBulletIndex];
				cts_skill_waveshock.skill_data.alive = true;

				//고리생성
				scene->StaticObject.push_back(new RingObject(scene->device, scene->commandlist, &scene->BbObject, &scene->Shadows, PlayerObject, PlayerObject->CenterPos));
				skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
				skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;
				skilldata.SellectBulletIndex = 0;//스킬 시전후 가장 첫번째 스킬로 변경함

				scene->Player->m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_skill_waveshock));

			}
			else if (skilldata.Skills[skilldata.SellectBulletIndex] == 5 && skilldata.isSkillOn[skilldata.SellectBulletIndex])//실드
			{
				CTS_SKILL_SHIELD cts_skill_shield;
				cts_skill_shield.skill_data.master_id = PlayerObject->m_player_data.id;
				cts_skill_shield.skill_data.my_id = skilldata.Skills[skilldata.SellectBulletIndex];
				cts_skill_shield.skill_data.alive = true;

				scene->Player->m_async_client->SendPacket(reinterpret_cast<Packet*>(&cts_skill_shield));

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
			PlayerObject->pp->SetHalfBox(4, 13, 4);//충돌 박스의 x,y,z 크기
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
			PlayerObject->pp->SetHalfBox(3, 10, 3);//충돌 박스의 x,y,z 크기
			((CCubeManObject*)PlayerObject)->s->ObjData.Scale = 2.0f;
			((CCubeManObject*)PlayerObject)->Hpbar->YPos = 10;
			((CCubeManObject*)PlayerObject)->HPFrame->YPos = 10;
		}
	}
	*/

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
 

		CGameObject *bul = new BulletCube(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, lock, PlayerObject->CenterPos, true);
		bulletlist->push_back(bul);

		//서버용
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

		CGameObject *bul = new HeavyBulletCube(Device, cl, PlayerObject->ParticleList, NULL, PlayerObject, ori, lock, PlayerObject->CenterPos);
		bulletlist->push_back(bul);


		//서버용
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

		CGameObject *hammer = new HammerBullet(Device, cl, PlayerObject->ParticleList, NULL,NULL,3, PlayerObject, XMFLOAT4(0,0,0,1),NULL, PlayerObject->CenterPos,XMFLOAT4(0,0,20,0));
		bulletlist->push_back(hammer);
		
		//서버로 해머불렛 생성에 대한 초기값 패킷 전달
		CTS_HammerSkillInfo cts_hammer_skill_info;
		cts_hammer_skill_info.pos4f = { PlayerObject->CenterPos.x, PlayerObject->CenterPos.y, PlayerObject->CenterPos.z, PlayerObject->CenterPos.w };
		cts_hammer_skill_info.rot4f = { 0, 0, 0, 1 };	//초기 rot는 ori 값 0 0 0 1
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
		//먼저 해당스킬의 쿨타임을 넣어주자.
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

void CPlayer::CreateOtherClientBullet(ID3D12Device * Device, ID3D12GraphicsCommandList * cl, Position3D & Goal, CGameObject * lock, list<CGameObject*>* bulletlist, STC_BulletObject_Info server_bulldata)
{
	XMFLOAT4 xmf4_pos{ server_bulldata.pos4f.x, server_bulldata.pos4f.y, server_bulldata.pos4f.z, server_bulldata.pos4f.w };
	XMFLOAT3 xmf3_pos{ Goal.x, Goal.y, Goal.z };

	XMFLOAT4 ori = XMFLOAT4(0, 0, 0, 0);
	CGameObject *bul = nullptr;

	//PlayerObject는 현재 이 클라이언트 플레이어를 의미한다
	switch (server_bulldata.type)
	{
		case protocol_LightBullet:
		case protocol_HeavyBullet:
		{
			//2018.07.25 - ori를 구하는 작업을 결국 해야지 particle의 방향이 맞게 생성된다
			//이걸 서버에서 작업해서 Orient값을 업데이트해서 보내지만, 결국 클라에서 그 작업을 같이 해줘야함
			//이 작업은 불렛이 한번 생성될 때만 하면된다.
			//[1]ori값을 불렛생성시 클라->서버->클라 이 과정을 한번 진행하는것이 성능상 이점인가 아니면 [2]클라에서 이렇게 한번해주는게 이점인가
			//클라 -> 서버로 갈 때 degree와 ori에 의해 업데이트 된 rotation값이 전달되므로 
			//서버에서 아래의 작업은 진행하지 않음 
			//[2]가 더 효율적인것 같음
			//[1]을 하면, 전달패킷의 크기가 늘어나 패킷사이즈가 증가 -> 서버부하로 발생,
			//           클라이언트는 cpu보다 성능이 좋은 gpu를 이용, 이 작업을 추가한다고해서 부하가 발생되지 않음

			auto v = Float3Add(xmf3_pos, XMFloat4to3(xmf4_pos), false);

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
			orr = XMVector4Transform(orr, wmatrix);//가짜 라이트 벡터
			orr = XMVector3Normalize(orr);
			auto RealRight = XMVector3Cross(axis, nl);//진짜 라이트벡터
			RealRight = XMVector3Normalize(RealRight);

			//진짜 라이트 벡터와 가짜 라이트 벡터를 내적함.
			temp = XMVector3Dot(RealRight, orr);

			XMStoreFloat(&d, temp);
			if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
				d = acos(d);//각도 완성. 라디안임
			auto ori2 = XMQuaternionRotationAxis(nl, d);//진짜 룩벡터를 회전축으로 삼고 진짜라이트와 가짜라이트의 사잇각만큼회전

			auto tempori = XMLoadFloat4(&ori);
			tempori = XMQuaternionMultiply(tempori, ori2);
			XMStoreFloat4(&ori, tempori);//최종 회전 방향

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
	orr = XMVector4Transform(orr, wmatrix);//가짜 라이트 벡터
	orr = XMVector3Normalize(orr);
	auto RealRight = XMVector3Cross(axis, nl);//진짜 라이트벡터
	RealRight = XMVector3Normalize(RealRight);

	//진짜 라이트 벡터와 가짜 라이트 벡터를 내적함.
	temp = XMVector3Dot(RealRight, orr);

	XMStoreFloat(&d, temp);
	if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
		d = acos(d);//각도 완성. 라디안임
	auto ori2 = XMQuaternionRotationAxis(nl, d);//진짜 룩벡터를 회전축으로 삼고 진짜라이트와 가짜라이트의 사잇각만큼회전

	auto tempori = XMLoadFloat4(&ori);
	tempori = XMQuaternionMultiply(tempori, ori2);
	XMStoreFloat4(&ori, tempori);//최종 회전 방향

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

	//PlayerObject는 현재 이 클라이언트 플레이어를 의미한다
	switch (server_endbulldata.type)
	{
	case protocol_LightBullet:
	case protocol_HeavyBullet:
	{
		//2018.07.25 - ori를 구하는 작업을 결국 해야지 particle의 방향이 맞게 생성된다
		//이걸 서버에서 작업해서 Orient값을 업데이트해서 보내지만, 결국 클라에서 그 작업을 같이 해줘야함
		//이 작업은 불렛이 한번 생성될 때만 하면된다.
		//[1]ori값을 불렛생성시 클라->서버->클라 이 과정을 한번 진행하는것이 성능상 이점인가 아니면 [2]클라에서 이렇게 한번해주는게 이점인가
		//클라 -> 서버로 갈 때 degree와 ori에 의해 업데이트 된 rotation값이 전달되므로 
		//서버에서 아래의 작업은 진행하지 않음 
		//[2]가 더 효율적인것 같음
		//[1]을 하면, 전달패킷의 크기가 늘어나 패킷사이즈가 증가 -> 서버부하로 발생,
		//           클라이언트는 cpu보다 성능이 좋은 gpu를 이용, 이 작업을 추가한다고해서 부하가 발생되지 않음

		auto v = Float3Add(xmf3_pos, XMFloat4to3(xmf4_pos), false);

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
		orr = XMVector4Transform(orr, wmatrix);//가짜 라이트 벡터
		orr = XMVector3Normalize(orr);
		auto RealRight = XMVector3Cross(axis, nl);//진짜 라이트벡터
		RealRight = XMVector3Normalize(RealRight);

		//진짜 라이트 벡터와 가짜 라이트 벡터를 내적함.
		temp = XMVector3Dot(RealRight, orr);

		XMStoreFloat(&d, temp);
		if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
			d = acos(d);//각도 완성. 라디안임
		auto ori2 = XMQuaternionRotationAxis(nl, d);//진짜 룩벡터를 회전축으로 삼고 진짜라이트와 가짜라이트의 사잇각만큼회전

		auto tempori = XMLoadFloat4(&ori);
		tempori = XMQuaternionMultiply(tempori, ori2);
		XMStoreFloat4(&ori, tempori);//최종 회전 방향

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

	//불렛의 마지막 위치
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
	orr = XMVector4Transform(orr, wmatrix);//가짜 라이트 벡터
	orr = XMVector3Normalize(orr);
	auto RealRight = XMVector3Cross(axis, nl);//진짜 라이트벡터
	RealRight = XMVector3Normalize(RealRight);

	//진짜 라이트 벡터와 가짜 라이트 벡터를 내적함.
	temp = XMVector3Dot(RealRight, orr);

	XMStoreFloat(&d, temp);
	if (fabsf(d) <= 1)//반드시 이 결과는 -1~1 사이여야한다. 그래야 각도가 구해진다.
		d = acos(d);//각도 완성. 라디안임
	auto ori2 = XMQuaternionRotationAxis(nl, d);//진짜 룩벡터를 회전축으로 삼고 진짜라이트와 가짜라이트의 사잇각만큼회전

	auto tempori = XMLoadFloat4(&ori);
	tempori = XMQuaternionMultiply(tempori, ori2);
	XMStoreFloat4(&ori, tempori);//최종 회전 방향

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

	//불렛의 마지막 위치
	bul->m_end_bulletPos = { server_endbulldata.end_pos4f.x ,server_endbulldata.end_pos4f.y, server_endbulldata.end_pos4f.z, server_endbulldata.end_pos4f.w };

	//bul->ParticleList->back()->Orient = bul->Orient;
	//bul->ParticleList->back()->UpdateLookVector();

	bulletlist->push_back(bul);
}

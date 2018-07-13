#include "CPlayer.h"
#include"Scene.h"

CPlayer::CPlayer(HWND hWnd,ID3D12Device* Device, ID3D12GraphicsCommandList* commandlist, float asp, XMFLOAT3& e, XMFLOAT3& a, XMFLOAT3& u) : Camera(hWnd,Device,commandlist,asp,e,a,u)
{
	//����� �ٸ� ������Ʈ�� �߰��Ұ�.
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

			if (move == true)//�����̰� ������ �����̴� �������
			{
				if (PlayerObject->n_Animation != Ani_State::Attack)//���ݸ���� �ƴϸ� �ٽ� �����·�
					PlayerObject->SetAnimation(Ani_State::Run);
			}
			else
			{
				if (PlayerObject->n_Animation != Ani_State::Attack)//���ݸ���� �ƴϸ� �ٽ� �����·�
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


			
			//���� ��ư�� �������� ������ ��ų�� �ѹ��� 4�� 5�� ���(4�� �ĵ��� , 5�� ��������(?))
			if (skilldata.Skills[skilldata.SellectBulletIndex] == 4 && skilldata.isSkillOn[skilldata.SellectBulletIndex])//�ĵ����ΰ��
			{
				//������
				scene->StaticObject.push_back(new RingObject(scene->device, scene->commandlist,&scene->BbObject, &scene->Shadows, PlayerObject->CenterPos));
				skilldata.SkillsCoolTime[skilldata.SellectBulletIndex] = skilldata.SkillsMaxCoolTime[skilldata.Skills[skilldata.SellectBulletIndex]];
				skilldata.isSkillOn[skilldata.SellectBulletIndex] = false;
				skilldata.SellectBulletIndex = 0;//��ų ������ ���� ù��° ��ų�� ������

				XMFLOAT4 �ĵ�����ġ = PlayerObject->CenterPos;
				�ĵ�����ġ.y += -PlayerObject->pp->GetHalfBox().y;
				float impurse = 20000;
				float rad = 150.f;//����
				//���̳��� ������Ʈ�� �����������Ʈ�� ����������.
				for (auto l : scene->DynamicObject)
				{
					if (l != PlayerObject)
					{
						auto l2 = Float3Add(XMFloat4to3(l->CenterPos), XMFloat4to3(�ĵ�����ġ), false);
						
						if (FloatLength(l2) <= rad)//���� �ȿ�����������
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
					auto l2 = Float3Add(XMFloat4to3(l->CenterPos), XMFloat4to3(�ĵ�����ġ), false);

					if (FloatLength(l2) <= rad)//���� �ȿ�����������
					{
						l2 = Float3Normalize(l2);
						l2 = Float3Float(l2, impurse*(1 - FloatLength(l2) / rad));
						l->rb->AddForce(l2);
						l->rb->integrate(0.01f);
					}

				}
				
			}
			else if (skilldata.Skills[skilldata.SellectBulletIndex] == 5 && skilldata.isSkillOn[skilldata.SellectBulletIndex])//�ǵ�
			{
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
 

		bulletlist->push_back(new BulletCube(Device, cl, PlayerObject->ParticleList,NULL, PlayerObject, ori, lock, PlayerObject->CenterPos));
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


		
		bulletlist->push_back(new HeavyBulletCube(Device, cl, PlayerObject->ParticleList,NULL, PlayerObject, ori, lock, PlayerObject->CenterPos));
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

		bulletlist->push_back(new HammerBullet(Device, cl, PlayerObject->ParticleList, NULL,NULL,3, PlayerObject, XMFLOAT4(0,0,0,1),NULL, PlayerObject->CenterPos,XMFLOAT4(0,0,20,0)));
		

		break;
	}
	case 7:
	{
		PlayerObject->SetAnimation(2);
		//���� �ش罺ų�� ��Ÿ���� �־�����.
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

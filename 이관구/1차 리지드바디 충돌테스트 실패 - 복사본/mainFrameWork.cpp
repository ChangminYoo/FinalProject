#include"mainFrameWork.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    try
    {
        MainFrameWork theApp(hInstance);
        if(!theApp.Initialize())
            return 0;

        return theApp.Run();
    }
    catch(DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}

MainFrameWork::MainFrameWork(HINSTANCE hInstance): FrameWork(hInstance) 
{

}

MainFrameWork::~MainFrameWork()
{
}

bool MainFrameWork::Initialize()
{
    if(!FrameWork::Initialize())
		return false;
	
	

	return true;
}

//충돌 시스템은 반드시 틱함수 이후에 처리되야한다. 왜냐하면 틱함수까지 처리되면 실제로 움직인게 되기 때문이다.
//충돌은 움직인 이후에 발생한다.
//먼저 고정된 객체를 제외한 모든 오브젝트를 검사해야한다.
//요소 하나씩 접근을 하는데, 접근한다음, 해당 오브젝트가 검사해야할 목록을 찾아서 인자로 넣어준다.
//오브젝트는 Collision함수를 호출하는데 아까 인자로 받은 리스트 목록을 주르륵 돌면서
//CollisionTest함수를 호출한다. 이녀석이 실제로 OBB충돌검사를 하는 녀석이다.
//충돌을 했으면 CollsionResolve 함수를 호출해서 충돌을 해소해준다.
//중요한점은 충돌해소후에 반드시! UpdatePPosCenterPos함수를 호출해서 PP의 중점으로 게임오브젝트의 중점을 갱신해야한다.
void MainFrameWork::CollisionSystem(const GameTimer& gt)
{
	//반드시 투사체가 가장먼저 상대를 검사한다.
	for (auto i = scene->BulletObject.begin(); i != scene->BulletObject.end(); i++)
	{
		//FindCollisionObject 함수를 나중에 만들어야함. 현재는 그냥 DynamicObject를 준다.
		//해당 객체가 충돌처리 검사를 할 목록을 저장함.
		//이 목록을 가지고 충돌검사를 하도록함.

		//투사체끼리는 검사 X 투사체는 반드시 다이나믹오브젝트 들을 검사해야함.
		(*i)->Collision(&scene->DynamicObject, gt.DeltaTime());
		(*i)->Collision(&scene->StaticObject, gt.DeltaTime());
	}

	//고정된 객체를 제외한 모든 오브젝트를 충돌검사를 하도록 함.
	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		//FindCollisionObject 함수를 나중에 만들어야함. 현재는 그냥 DynamicObject를 준다.
		//해당 객체가 충돌처리 검사를 할 목록을 저장함.
		//이 목록을 가지고 충돌검사를 하도록함.
		
		(*i)->Collision(&scene->DynamicObject, gt.DeltaTime());
		(*i)->Collision(&scene->StaticObject, gt.DeltaTime());

	}

}

void MainFrameWork::System(const GameTimer & gt)
{
	GravitySystem(gt);
}

void MainFrameWork::GravitySystem(const GameTimer & gt)
{
	GeneratorGravity gg;
	gg.SetGravityAccel(XMFLOAT3(0, -100, 0));
	//고정된 물체를 제외한 모든오브젝트에 중력을 가한다. 단 투사체는 제외한다.
	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		
		gg.Update(gt.DeltaTime(), *(*i)->pp);
	}
	for (auto i = scene->StaticObject.begin(); i != scene->StaticObject.end(); i++)
	{

		gg.Update(gt.DeltaTime(), *(*i)->rb);
	}

}

void MainFrameWork::AfterGravitySystem(const GameTimer & gt)
{
	
	for (auto i = scene->DynamicObject.begin(); i != scene->DynamicObject.end(); i++)
	{
		//왜 실제 중점이 아닌 pp의 중점으로 처리하냐면 실제중점을 움직인후 pp의 중점을 움직이나
		//pp의중점을 움직이고 실제중점을 움직이나 같지만, UpdatePPosCenterPos를 쓰기위해
		//pp를 움직이고 cp를 pp로 맞춘다.
		float ppy = (*i)->pp->GetPosition().y;
		float hby = (*i)->pp->GetHalfBox().y;
		if (ppy-hby < 0)//pp의 중점y-하프박스의 y값을 한결과가 0보다 작으면 땅아래에 묻힌셈
		{
			XMFLOAT3 gp = (*i)->pp->GetPosition();
			gp.y += hby - ppy;//그러면 반대로 하프박스y값-중점y만큼 올리면 된다.
			(*i)->pp->SetPosition(gp);
			(*i)->UpdatePPosCenterPos();
			auto v=(*i)->pp->GetVelocity();
			v.y = 0;//중력에 의한 속도를 0으로 만듬
			(*i)->pp->SetVelocity(v);
			(*i)->AirBone = false;
		}
	}

	//투사체는 y가 0보다 작으면 제거된다.
	for (auto i = scene->BulletObject.begin(); i != scene->BulletObject.end(); i++)
	{
		if ((*i)->CenterPos.y <= 0)
			(*i)->DelObj = true;
	}

	for (auto j = scene->StaticObject.begin(); j != scene->StaticObject.end(); j++)
	{
		RigidBodyCollisionPlane(XMFLOAT3(0, 1, 0), 0, *j);
	}
	//for (auto j = scene->StaticObject.begin();j != scene->StaticObject.end();j++)
	//{
	//	
	//	XMFLOAT4 arr[8];
	//	(*j)->rb->GetEightPoint(arr, (*j)->GetUpvector(), (*j)->Lookvector, (*j)->Rightvector);//먼저 8 개의 점을 가져온다.
	//	//점이 충돌했는지 검사를 해야한다.
	//	XMFLOAT3 PlaneNormal{ 0,1,0 };
	//	vector<CollisionPoint> tempcp;
	//	int c = 0;
	//	for (int i = 0; i < 8; i++)
	//	{
	//		float temppenetration = arr[i].x*PlaneNormal.x + arr[i].y*PlaneNormal.y + arr[i].z*PlaneNormal.z;
	//		//충돌 점을 생성한다음 저장한다.
	//		CollisionPoint cp;
	//		cp.Pos = arr[i];
	//		cp.penetration = temppenetration;
	//		cp.pAxis = PlaneNormal;
	//		tempcp.push_back(cp);

	//		if (temppenetration < 0)//평면의 뒤에있으면 충돌한것.
	//		{
	//			c += 1;

	//					}

	//	}

	//	//사이즈가 0보다 크면 충돌한 지점이 있는것.
	//	if (c > 0)
	//	{
	//		//이제 가장 깊은 충돌 깊이를 기준으로 정렬한다.왜? 가장깊이 충돌한만큼 밀어내야하니까.
	//		sort(tempcp.begin(), tempcp.end(), [](CollisionPoint& a, CollisionPoint& b) {
	//			return a.penetration < b.penetration;
	//		});

	//		
	//	

	//		
	//		// 이제 여기까지오면 충돌지점을 처리해야할것들을 다 저장하고있다는것이다.
	//		//이제 뭘 해야하나? 1. 속도를 변경한다. 2. 겹쳐진 부분을 해소한다.
	//		
	//		//테스트 결과 임펄스가 작으면 작을수록, 정확도가 높아짐..

	//		
	//		
	//			auto v2 = Float4Add(tempcp[1].Pos, tempcp[0].Pos, false);
	//			v2 = Float4Normalize(v2);
	//			auto v3=Float4Add(tempcp[2].Pos, tempcp[0].Pos,false);
	//			tempcp[2].Pos.y = tempcp[0].Pos.y;
	//			v3 = Float4Normalize(v3);
	//			auto v4 = Float4Add(tempcp[2].Pos, tempcp[0].Pos, false);
	//			v4 = Float4Normalize(v4);
	//			if (v2.x*v4.x + v2.y*v4.y + v2.z*v4.z < MMPE_EPSILON)
	//			{
	//				auto axis=Float3Cross(XMFloat4to3(v3), XMFloat4to3(v4));
	//				axis = Float3Normalize(axis);
	//				double theta=acos(v3.x*v4.x + v3.y*v4.y + v3.z*v4.z);
	//				if (theta != NAN)
	//				{
	//					XMFLOAT4 r;
	//					if (fabsf(theta) < MMPE_PI/36 && FloatLength(axis)!=0)
	//					{
	//						(*j)->UpdateLookVector();
	//						r = QuaternionRotation(axis, theta);
	//						r=Float4Normalize(r);
	//						(*j)->rb->SetAngularVelocity(0, 0, 0);
	//						(*j)->Orient = QuaternionMultiply((*j)->Orient, r);
	//						(*j)->UpdateLookVector();
	//						tempcp.clear();
	//						(*j)->rb->CollisionPointVector.clear();
	//					}
	//					(*j)->rb->GetEightPoint(arr, (*j)->GetUpvector(), (*j)->Lookvector, (*j)->Rightvector);//먼저 8 개의 점을 가져온다.
	//					int c = 0;
	//					for (int i = 0; i < 8; i++)
	//					{
	//						float temppenetration = arr[i].x*PlaneNormal.x + arr[i].y*PlaneNormal.y + arr[i].z*PlaneNormal.z;
	//						//충돌 점을 생성한다음 저장한다.
	//						CollisionPoint cp;
	//						cp.Pos = arr[i];
	//						cp.penetration = temppenetration;
	//						cp.pAxis = PlaneNormal;
	//						tempcp.push_back(cp);

	//						if (temppenetration < 0)//평면의 뒤에있으면 충돌한것.
	//						{
	//							c += 1;

	//						}

	//					}
	//					//사이즈가 0보다 크면 충돌한 지점이 있는것.
	//					if (c > 0)
	//					{
	//						//이제 가장 깊은 충돌 깊이를 기준으로 정렬한다.왜? 가장깊이 충돌한만큼 밀어내야하니까.
	//						sort(tempcp.begin(), tempcp.end(), [](CollisionPoint& a, CollisionPoint& b) {
	//							return a.penetration < b.penetration;
	//						});
	//					}

	//				}
	//			}

	//			float firstpenetration = tempcp.front().penetration;//가장 깊은값을 꺼냄.
	//			if (firstpenetration > 0)
	//				firstpenetration = 0;
	//																//이제 가장 깊은 녀석을 기준으로 충돌리스트를 돌면서 penetration을 뺐을때 결과가 충분히 작으면
	//																//이녀석도 밀어내거나 힘을가할때 사용될 수 있는 충돌점이다. 이 것들이 최종적인 충돌점이다.
	//			for (auto i : tempcp)
	//			{
	//				if (fabsf(firstpenetration - i.penetration) <= MMPE_EPSILON)
	//				{
	//					(*j)->rb->CollisionPointVector.push_back(i);
	//				}
	//			}
	//			CollisionPoint fp;
	//			fp.Pos = XMFLOAT4(0, 0, 0, 0);
	//			fp.pAxis = PlaneNormal;
	//			for (auto i : (*j)->rb->CollisionPointVector)
	//			{
	//				fp.Pos = Float4Add(fp.Pos, i.Pos);
	//				fp.penetration += i.penetration;



	//			}

	//			if (firstpenetration < 0)
	//			{
	//				if ((*j)->rb->CollisionPointVector.size() > 0)
	//				{
	//					fp.Pos.x /= (*j)->rb->CollisionPointVector.size();
	//					fp.Pos.y /= (*j)->rb->CollisionPointVector.size();
	//					fp.Pos.z /= (*j)->rb->CollisionPointVector.size();
	//					fp.penetration /= (*j)->rb->CollisionPointVector.size();
	//				}
	//				auto impurse = (*j)->rb->CalculateImpulse(fp);

	//				impurse = XMFLOAT3(0, 3, 0);
	//				(*j)->rb->SetAngularVelocity(0, 0, 0);
	//				if ((*j)->rb->CollisionPointVector.size() > 2)
	//				{
	//					//impurse = XMFLOAT3(0, 0, 0);
	//					//(*j)->rb->SetAngularVelocity(0, 0, 0);
	//				//impurse = XMFLOAT3(0, 0, 0);
	//				}
	//				XMFLOAT3 p = XMFloat4to3(fp.Pos);
	//				auto p2 = XMFloat4to3((*j)->rb->GetPosition());
	//				p = Float3Add(p, p2, false);//p-=p2
	//				if (fabsf(p.x) <= MMPE_EPSILON)//떨리면서 각도 애매하게 바뀌는거 막기위함
	//					p.x = 0;
	//				if (fabsf(p.y) <= MMPE_EPSILON)//떨리면서 각도 애매하게 바뀌는거 막기위함
	//					p.y = 0;
	//				if (fabsf(p.z) <= MMPE_EPSILON)//떨리면서 각도 애매하게 바뀌는거 막기위함
	//					p.z = 0;


	//				XMFLOAT3 t = Float3Cross(p, impurse);
	//				XMVECTOR torq = XMLoadFloat3(&t);
	//				XMMATRIX iim = XMLoadFloat4x4(&(*j)->rb->GetIMoment());
	//				XMFLOAT3 av;
	//				XMStoreFloat3(&av, XMVector3Transform(torq, iim));//각속도

	//				(*j)->rb->SetAngularVelocity(av);


	//				//겹쳐진 부분 해소

	//				auto px = fabsf((*j)->rb->CollisionPointVector.front().penetration)*(*j)->rb->CollisionPointVector.front().pAxis.x;
	//				auto py = fabsf((*j)->rb->CollisionPointVector.front().penetration)*(*j)->rb->CollisionPointVector.front().pAxis.y;
	//				auto pz = fabsf((*j)->rb->CollisionPointVector.front().penetration)*(*j)->rb->CollisionPointVector.front().pAxis.z;
	//				(*j)->CenterPos.x += px;
	//				(*j)->CenterPos.y += py;
	//				(*j)->CenterPos.z += pz;

	//				auto d = (*j)->rb->GetVelocity();
	//				d.y = 0;
	//				(*j)->rb->SetVelocity(d);
	//				(*j)->rb->SetVelocity(impurse.x*(*j)->rb->GetMass(), impurse.y*(*j)->rb->GetMass(), impurse.z*(*j)->rb->GetMass());
	//				if ((*j)->rb->GetPosition().y < -50)
	//					(*j)->rb->SetVelocity(0, 5, 0);

	//				//(*j)->rb->integrate(gt.DeltaTime());
	//			}
	//			(*j)->rb->CollisionPointVector.clear();
	//		}
	//	}

		

		
	
}

void MainFrameWork::RigidBodyCollisionPlane(XMFLOAT3 & Normal, float distance, CGameObject * obj)
{
	
	if (obj->rb != NULL)
	{
		XMFLOAT4 arr[8];
		obj->rb->GetEightPoint(arr, obj->GetUpvector(), obj->Lookvector, obj->Rightvector);//먼저 8 개의 점을 가져온다.
		std::vector<CollisionPoint> allpoint;
		std::vector<CollisionPoint> tempcollisionpoint;
		std::vector<CollisionPoint> contactpoint;


		
		for (int i = 0; i < 8; i++)
		{
			float temppenetration = arr[i].x*Normal.x + arr[i].y*Normal.y + arr[i].z*Normal.z;
			//충돌 점을 생성한다음 저장한다.
			CollisionPoint cp;
			cp.Pos = arr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			allpoint.push_back(cp);
		}
		// 5) 번을 처리함
		//penetration이 작은 순으로 정렬
		sort(allpoint.begin(), allpoint.end(), [](CollisionPoint& a, CollisionPoint& b) 
		{
			return a.penetration < b.penetration;
		});

		// 6) 번을 처리함.
		//실제로 정렬된 녀석중 1차적으로 충돌이 된 녀석들을 tempcollisionpoint에 저장한다.
		for (auto i : allpoint)
		{
			float temppenetration = i.Pos.x*Normal.x +i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0이하면 실제로 충돌함
				tempcollisionpoint.push_back(i);

		}
		//이제 가장 깊은 녀석을 기준으로 충돌리스트를 돌면서 penetration을 뺐을때 결과가 충분히 작으면
		//이녀석도 밀어내거나 힘을가할때 사용될 수 있는 충돌점이다. 이 것들이 최종적인 충돌점이다.
		
		// 7) 번을 처리함
		for (auto i : tempcollisionpoint)
		{
			//실제충돌의 첫번째 깊이 - penetration들을 비교해서 Epsilon이면 이제 실제 접촉점이 저장되는 곳에 넣는다.
			if (fabsf(tempcollisionpoint.front().penetration - i.penetration) <= 0.1)//차가 0.1정도면 실제로 충돌 가능성이 있다고 판단한다.
			{
				contactpoint.push_back(i);
			}

		}

		//------------- 여기가 가장 중요한 분기 -----------------//

		//------------- size 가 1/2/4/그외 일때 로 나눠서 처리 ------------------//
		int size = contactpoint.size();
		if (size == 1)
		{
			//size가 1이면 뭘해야할까? 바로 정점 2개가 맞부딪힐수있도록 회전시켜야한다.
			//그러기 위해서는 적당한 위치에 충격량을 가한다.
			//문제는 이게 과회전이 일어날 수 있다.
			//따라서 적당한 회전각도가 되면 그 각도로 보정한다.

			//2인자점 - 1인자점을 해서 2인자점을 향하는 벡터를 만든다.
			//이때 2인자 점을 향하는 각도가 특정각도 이하면 그 각도로 보정한다.

			//문제는 2인자점을 어느것으로 할것인가 이다. 경우의수는 2가지다. 왼쪽이나 오른쪽점 / 또는 대각선 점.

			auto V1 = Float4Add(allpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			float NdotV1 = Normal.x*V1.x + Normal.y*V1.y + Normal.z*V1.z;
			
			XMFLOAT3 ProjAB = Normal;
			ProjAB.x *= NdotV1;
			ProjAB.y *= NdotV1;
			ProjAB.z *= NdotV1;

			auto V2 = Float4Add(V1, XMFloat3to4(ProjAB), false);
			V2 = Float4Normalize(V2);
			

			//먼저 사잇각도를 구한다.
			//기존에는 float으로 했는데, 0이아니어야 하는데 0이나오는경우가 생김..
			double theta = acos(V1.x*V2.x + V1.y*V2.y + V1.z*V2.z);

			//그후 사잇각이 특정각도 이하면 보정시킨다. 
			//단 이게 double로 해도 0이아닌데 0이나오는경우가 생긴다.
			//따라서 0일경우 그냥 충격량을 가해서 각도를 변경시킨다.
			if (abs(theta) <= MMPE_PI / 36 && abs(theta)!=0)//대략 5도 이하면 보정시킴.
			{
				//회전축을 구하고..
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V2));
				mAxis = Float3Normalize(mAxis);
				//보정을 시킨다.
				AmendObject(mAxis, theta, obj);

				//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
				//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
				obj->rb->SetAngularVelocity(0, 0, 0);
				RigidBodyCollisionPlane(Normal, distance, obj);

			}
			else//아니라면 이제 충격량을 적당한 지점에 가한다!
			{
				//여기에 왔다는것은 더이상 보정을 하지 않거나 보정을 아직 할필요가 없는 경우다.


				XMFLOAT3 impurse{ 0,70,0 };// 충격량 . 여기서는 선속도를 증가시키는것은 적다 다만 각속도를 변화시킬때 유효하다.

				CollisionPoint fp;//충격량을 가할 지점
				fp.Pos = XMFLOAT4(0, 0, 0, 0);
				fp.pAxis = Normal;

				fp.Pos =  contactpoint[0].Pos;
				fp.penetration = contactpoint[0].penetration;

				
			
				////충격량을 가하려면 (Q-P)XImpurse 이므로 (Q-P)를한다. 여기서는 충돌지점의평균 - 오브젝트의 중점 이다.
				//XMFLOAT3 p = XMFloat4to3(fp.Pos);
				//auto p2 = XMFloat4to3(obj->rb->GetPosition());
				//p = Float3Add(p, p2, false);//p-=p2
				//if (fabsf(p.x) <= MMPE_EPSILON)//떨리면서 각도 애매하게 바뀌는거 막기위함
				//	p.x = 0;
				//if (fabsf(p.y) <= MMPE_EPSILON)//떨리면서 각도 애매하게 바뀌는거 막기위함
				//	p.y = 0;
				//if (fabsf(p.z) <= MMPE_EPSILON)//떨리면서 각도 애매하게 바뀌는거 막기위함
				//	p.z = 0;


				//땅에 닿았으니 현재 속도의 y는 반감되어야 한다. 원래는 탄성계수가 있지만.. 그냥 2배 증가시킨후 부호를 -로 하자.


				//현재 여기서 선속도의 가장 많은 부분을 차지함
				auto d = obj->rb->GetVelocity();
				d.y = -0.5 * d.y;
				obj->rb->SetVelocity(d);


				//충격량을 가함. impurse = impurse만큼 0.01초동안 가한것. 시간을 작게둔 이유는 힘을 줄이기 위해서.

				obj->rb->AddForcePoint(impurse, fp.Pos);
				obj->rb->integrate(0.01);


				//이제 속도와 각속도는 변경 했으니, 겹쳐진 부분 해소
				//가장 작은값의 penetration(가장 깊은)만큼 올리면 된다.
				auto px = fabsf(contactpoint[0].penetration)*Normal.x;
				auto py = fabsf(contactpoint[0].penetration)*Normal.y;
				auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
				obj->CenterPos.x += px;
				obj->CenterPos.y += py;
				obj->CenterPos.z += pz;





			}


		}
		else if (size == 2)
		{
			//삐뚫어진 점 2개면 보정을 해야한다.
			//보정을 해야하는지 검사를 하기 위해 평면의 노멀과 접촉점 두개를 이은 벡터가 내적시 0이 나오는지 검사한다.
			auto V1 = Float4Add(contactpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			float Dot = V1.x*Normal.x + V1.y*Normal.y + V1.z*Normal.z;

			if (fabsf(Dot) <= MMPE_EPSILON)
				Dot = 0;
			//결과가 0이면 보정이 필요가 없음.
			if (Dot == 0)
			{
				//이제 2개의 충돌을 4개의 점충돌로 바꿀 수 있는 보정을 할 수 있는지 검사한다.
				//그러기 위해서 3번째 점- 첫번째 점 을 해서 3번째 점을 향하는 벡터를 만든다.
				//그 후 이게 V1과 내적시 0 이 나오면 첫번째 점과 세번째 점이 대각선이 아니란것이고,
				//아니라면 두번째 점과 세번째점을 이용해 벡터를 만들어야한다.
				auto V2 = Float4Add(allpoint[2].Pos, contactpoint[0].Pos, false);
				V2 = Float4Normalize(V2);
				float Dot2 = V1.x*V2.x + V1.y*V2.y + V1.z*V2.z;
				
				if (fabsf(Dot2) <= MMPE_EPSILON)
					Dot2 = 0;

				//첫번째 점과 세번째점이 맞지 않는경우 두번째 점과 세번째점으로 맞춘다.
				if (Dot2 != 0)
				{

					V2 = Float4Add(allpoint[2].Pos, contactpoint[1].Pos, false);
					V2 = Float4Normalize(V2);
				}
					//이제 여기 왔다는것은, 4개의 점을 만들 수 있는 녀석들을 찾은 셈이다.
					//다만 바로 보정하면 안되고, 이게 어느정도 각도 차가 덜 나야 된다.
					//그럼 그 각도는 무엇인가??
					/*
					ㅅ  1)(V2)
					ㅣ /
					ㅣ/
					ㅡㅡㅡ2)(V3)

					현재 V2가 바로 1) 이다.
					이게 평면의 노멀과 수직인 2) 의 벡터로 만들건데
					1)과 2) 각도를 재다가 적당한 각도 한 5~10 도 정도 되면 그만큼 회전 시키면 끝이다!
					그러기 위해선 2)를 구해야한다. 2)는 평면의 노멀을 A 현재 V2를 B라 하고 ProjAB 한결과를 V2에 뺀다음 V2를 노멀화 하면 된다.
					*/

					float NdotV2 = Normal.x*V2.x + Normal.y*V2.y + Normal.z*V2.z;
					XMFLOAT3 ProjAB = Normal;
					ProjAB.x *= NdotV2;
					ProjAB.y *= NdotV2;
					ProjAB.z *= NdotV2;

					auto V3 = Float4Add(V2, XMFloat3to4(ProjAB), false);
					V3 = Float4Normalize(V3);
					//이제 V2와 V3를 아니까 V2와 V3의 사잇각을 구한다음 V2에서 V3로 외적해서 Axis를 구한다.

					//먼저 사잇각도를 구한다.
					double theta = acos(V2.x*V3.x + V2.y*V3.y + V2.z*V3.z);

					//그후 사잇각이 특정각도 이하면 보정시킨다. 
					if (abs(theta) <= MMPE_PI / 36 && abs(theta) != 0)//대략 5도 이하면 보정시킴.
					{
						//회전축을 구하고
						XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
						mAxis = Float3Normalize(mAxis);
						//보정을 시킨다.
						AmendObject(mAxis, theta, obj);

						//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
						//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
						obj->rb->SetAngularVelocity(0, 0, 0);
						RigidBodyCollisionPlane(Normal, distance, obj);

					}
					else//아니라면 이제 충격량을 적당한 지점에 가한다!
					{
						//여기에 왔다는것은 더이상 보정을 하지 않거나 보정을 아직 할필요가 없는 경우다.

						//먼저 현재 충돌점은 2개이니 2개의 충돌점을 더한 후 2로 나눠주면 충돌지점 완성이다.
						//충격량은 차후에 다시 구해야겠지만, 현재는 0,50,0 정도로 가하겠다.

						//근데 여기서 경우의 수가 생긴다. 알다시피 대각선형태로 정확하게 45도 기준으로 서있는 경우의수가 생긴다.
						//이것을 없애려면 먼저 해당경우의 수에 속하는지를 검사해야한다.

						//그후 그 경우의수를 해결해야겠다. 검사를 하려면 좌우 대칭인지를 보면될것같다.
						//그리고 대칭이면 랜덤이든 아니면 오른쪽으로 충격량을 약간 가하던 하면 될것같다.

						//먼저 2/3/4/5 인덱스의 점을 비교하면서 이 길이가 0.0001정도 차이면 통과. 4개모두 통과면 특정방향으로 충격량가함.
						float ComparePenetration = allpoint[2].penetration;
						bool pass = true;
						for (int i = 2; i < 6; i++)
						{
							if (fabsf(ComparePenetration - allpoint[i].penetration) > 0.25)
							{
								pass = false;
								break;
							}
						}


						XMFLOAT3 impurse{ 0,70,0 };// 충격량 . 여기서는 선속도를 증가시키는것은 적다 다만 각속도를 변화시킬때 유효하다.


						//4개가 모두 같은 깊이면 균형을 이루는것이므로 균형을 부셔버린다.
						if (pass)
						{
							theta = -MMPE_PI /18;
							XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
							mAxis = Float3Normalize(mAxis);
							//보정을 시킨다.
							AmendObject(mAxis, theta, obj);

							//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
							//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
							obj->rb->SetAngularVelocity(0, 0, 0);
							RigidBodyCollisionPlane(Normal, distance, obj);
							
							return;
						}


						
						CollisionPoint fp;//충격 량을 가할 지점
						fp.Pos = XMFLOAT4(0, 0, 0, 0);
						fp.pAxis = Normal;

						//실제 충돌지점들을 다 더함.
						for (auto i : contactpoint)
						{
							fp.Pos = Float4Add(fp.Pos, i.Pos);
							fp.penetration += i.penetration;

						}
						//사이즈로 나눠서 평균을 낸다.
						fp.Pos.x /= contactpoint.size();
						fp.Pos.y /= contactpoint.size();
						fp.Pos.z /= contactpoint.size();
						fp.penetration /= contactpoint.size();

						////충격량을 가하려면 (Q-P)XImpurse 이므로 (Q-P)를한다. 여기서는 충돌지점의평균 - 오브젝트의 중점 이다.
						//XMFLOAT3 p = XMFloat4to3(fp.Pos);
						//auto p2 = XMFloat4to3(obj->rb->GetPosition());
						//p = Float3Add(p, p2, false);//p-=p2
						//if (fabsf(p.x) <= MMPE_EPSILON)//떨리면서 각도 애매하게 바뀌는거 막기위함
						//	p.x = 0;
						//if (fabsf(p.y) <= MMPE_EPSILON)//떨리면서 각도 애매하게 바뀌는거 막기위함
						//	p.y = 0;
						//if (fabsf(p.z) <= MMPE_EPSILON)//떨리면서 각도 애매하게 바뀌는거 막기위함
						//	p.z = 0;


						//땅에 닿았으니 현재 속도의 y는 반감되어야 한다. 원래는 탄성계수가 있지만.. 그냥 2배 증가시킨후 부호를 -로 하자.
					

						//현재 여기서 선속도의 가장 많은 부분을 차지함
						auto d = obj->rb->GetVelocity();
						d.y = -0.5 * d.y;
						obj->rb->SetVelocity(d);
						
						
						//충격량을 가함. impurse = impurse만큼 0.01초동안 가한것. 시간을 작게둔 이유는 힘을 줄이기 위해서.
						
						obj->rb->AddForcePoint(impurse, fp.Pos);
						obj->rb->integrate(0.01);


						//이제 속도와 각속도는 변경 했으니, 겹쳐진 부분 해소
						//가장 작은값의 penetration(가장 깊은)만큼 올리면 된다.
						auto px = fabsf(contactpoint[0].penetration)*Normal.x;
						auto py = fabsf(contactpoint[0].penetration)*Normal.y;
						auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
						obj->CenterPos.x += px;
						obj->CenterPos.y += py;
						obj->CenterPos.z += pz;

						



					}
				

				

			}
			else//Dot의 결과 삐뚫어진 점 일경우 즉 노멀과 (두번째점-첫번째점) 벡터가 나온경우 
			{
				
				XMFLOAT3 ProjAB = Normal;
				ProjAB.x *= Dot;
				ProjAB.y *= Dot;
				ProjAB.z *= Dot;

				//교정되었을때 V1
				auto V4 = Float4Add(V1, XMFloat3to4(ProjAB), false);
				V4 = Float4Normalize(V4);

				//교정할 각도.
				double theta = acos(V1.x*V4.x + V1.y*V4.y + V1.z*V4.z);

				double theta2 = -MMPE_PI / 180;

				//회전축을 구하고
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V4));
				mAxis = Float3Normalize(mAxis);


				//보정을 시킨다.
				AmendObject(mAxis, theta, obj);

				//보정이되면 바로 재귀시켜서 검사한다. 다만 자기자신을 보정하면 굳이 각속도를 변경 시키진 않아도 된다.
				RigidBodyCollisionPlane(Normal, distance, obj);

				



			}


		}
		else if (size == 4)
		{


			//임시적이지만 그냥 일단 penetration과 속도만 변경하자.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->CenterPos.x += px;
			obj->CenterPos.y += py;
			obj->CenterPos.z += pz;

			//땅에 닿았으니 현재 속도의 y는 반감되어야 한다. 원래는 탄성계수가 있지만.. 그냥 절반 감소시킨후 부호를 -로 하자.
			auto d = obj->rb->GetVelocity();
			d.y = -0.5 * d.y;
			obj->rb->SetVelocity(d);
			obj->rb->SetAccel(0, 0, 0);


		}
		else if (size != 0)//그 외
		{
			//임시적이지만 그냥 일단 penetration과 속도만 변경하자.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->CenterPos.x += px;
			obj->CenterPos.y += py;
			obj->CenterPos.z += pz;

			//땅에 닿았으니 현재 속도의 y는 반감되어야 한다. 원래는 탄성계수가 있지만.. 그냥 절반 감소시킨후 부호를 -로 하자.
			auto d = obj->rb->GetVelocity();
			d.y = -0.2 * d.y;
			obj->rb->SetVelocity(d);
			obj->rb->SetAccel(0, 0, 0);

		}



		allpoint.clear();
		tempcollisionpoint.clear();
		contactpoint.clear();
	}
}

void MainFrameWork::OnResize()
{
	FrameWork::OnResize();
}

void MainFrameWork::FrameAdvance(const GameTimer& gt)
{
	//프레임어드벤스의 시작은 메모리리셋부터다.
	mDirectCmdListAlloc->Reset();
	mCommandList->Reset(mDirectCmdListAlloc.Get(),NULL);
	
	//뷰포트 연결
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);



	//현재백버퍼를 렌더타겟으로 변경
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//후면버퍼와 깊이버퍼 초기화
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// 현재 백버퍼를 연결.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	//현재 여기까지 연결된 파이프라인의 구성요소
	// RS 뷰포트와 시저렉트 , OM의 렌더타겟과 뎁스스텐실 뷰
	//앞으로 해야할것. 루트시그니처와 VS,PS등 PSO, 정점버퍼, 인덱스버퍼 , 상수버퍼뷰

	Update(gt);
	Draw(gt);

	//여기까지 왔으면 PSO에 모든게 다 연결되어 있고, 다 그려져있는것이다.
	//리소스 베리어를 읽기용으로 변경한다.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));


	// 커맨드리스트를 닫는다.
	ThrowIfFailed(mCommandList->Close());

	// 커맨드리스트를 커맨드 큐에 집어넣는다.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// 읽기용으로 프리젠트한다
	ThrowIfFailed(SwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	//플러쉬
	FlushCommandQueue();

}

void MainFrameWork::Update(const GameTimer& gt)
{
	//먼저 시스템 클래스를 업데이트 한다. 시스템 클래스는 게임 전반적인것들을 관여함. 중력 등
	System(gt);
	//씬클래스의 업데이트를 호출한다.
	scene->Tick(gt);

	//중력 후처리 시스템은 틱함수 이후에 처리해야함
	AfterGravitySystem(gt);
	//충돌 처리 시스템은 틱함수 이후에 처리해야한다.
	CollisionSystem(gt);
}

void MainFrameWork::Draw(const GameTimer& gt)
{
	scene->Render(gt);
}

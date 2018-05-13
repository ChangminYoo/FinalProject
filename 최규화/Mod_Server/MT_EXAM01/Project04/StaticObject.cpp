#include "StaticObject.h"


void StaticObject::CycleStaticObjects()
{
	pe = new PhysicalEffect();

	//1. 상자
	SetBasicInfoOfSObj();

	InitBoxObjects();
	InitSmallWallObjects();
	InitBigWallObjects();
	InitBuildingObjects();

	AfterGravitySystem();
}

void StaticObject::SetBasicInfoOfSObj()
{
	m_SobjPos =
	{
		{0, {{40.f, 18.f, -70.f, 0.f    },{0}, {Box}} },
		{1, {{50.f, 70.f, -80.f, 0.f    },{0}, {Box}} },
		{2, {{35.f, 50.f, -70.f, 0.f    },{0}, {Box}} },
		{3, {{118.f, 58.f, 47.f, 0.f    },{0}, {Box}} },
		{4, {{102.f, 69.f, 81.f, 0.f    },{0}, {Box}} },
		{5, {{58.f, 39.f,  78.f, 0.f    },{0}, {Box}} },
		{6, {{65.f, 35.f,-110.f, 0.f    },{0}, {Box}} },
		{7, {{92.f, 61.f,  60.f, 0.f    },{0}, {Box}} },
		{8, {{103.f, 36.f, 15.f, 0.f    },{0}, {Box}} },
		{9, {{65.f, 17.f, -65.f ,0.f    },{0}, {Box}} },
		{10,{{82.f, 11.f, 34.f, 0.f     },{0}, {Box}} },
		{11,{{72.f, 53.f, 41.f, 0.f     },{0}, {Box}} },
		{12,{{24.f, 44.f, 40.f, 0.f     },{0}, {Box}} },
		{13,{{-20.f, 33.f, 51.f,0.f     },{0}, {Box}} },
		{14,{{0.f, 24.f, 34.f ,0.f      },{0}, {Box}} },
		{15,{{0.f, 58.f, -71.f, 0.f     },{0}, {Box}} },
		{16,{{ -76.f, 53.f, 8.f, 0.f    },{0}, {Box}} },
		{17,{{ -40.f, 10.f, 0.f, 0.f    },{0}, {Box}} },
		{18,{{ -38.f, 48.f, -126.f, 0.f },{0}, {Box}} },
		{19,{{ -45.f, 30.f, -60.f, 0.f  },{0}, {Box}} },
		{20,{{ -65.f, 40.f, -102.f, 0.f },{0}, {Box}} },
		{21,{{ -69.f, 62.f,  61.f, 0.f  },{0}, {Box}} },
		{22,{{ -85.f, 42.f, -45.f, 0.f  },{0}, {Box}} },
		{23,{{ -106.f, 40.f,  42.f, 0.f },{0}, {Box}} },
		{24,{{ -20.f, 5.f, 210.f, 0.f   },{0}, {Box}} },
		{25,{{ -20.f, 15.f, 210.f ,0.f  },{0}, {Box}} },
		{26,{{ -20.f, 25.f, 210.f, 0.f  },{0}, {Box}} },
		{27,{{ -20.f, 35.f, 210.f, 0.f  },{0}, {Box}} },
		{28,{{ -10.f, 35.f, 210.f, 0.f  },{0}, {Box}} },
		{29,{{ 0.f, 35.f, 210.f,0.f     },{0}, {Box}} },
		{30,{{ 10.f, 35.f, 210.f ,0.f   },{0}, {Box}} },
		{31,{{ 10.f, 25.f, 210.f, 0.f   },{0}, {Box}} },
		{32,{{ 10.f, 15.f, 210.f ,0.f   },{0}, {Box}} },
		{33,{{ 10.f, 5.f, 210.f, 0.f    },{0}, {Box}} },
		{34,{{ 260.f, 5.f,  -30.f, 0.f  },{0}, {Box}} },
		{35,{{ 260.f, 15.f, -30.f, 0.f  },{0}, {Box}} },
		{36,{{ 260.f, 25.f, -30.f, 0.f  },{0}, {Box}} },
		{37,{{ 260.f, 35.f, -30.f, 0.f  },{0}, {Box}} },
		{38,{{ 260.f, 35.f, -20.f, 0.f  },{0}, {Box}} },
		{39,{{ 260.f, 35.f, -10.f, 0.f  },{0}, {Box}} },
		{40,{{ 260.f, 35.f,  0.f, 0.f   },{0}, {Box}} },
		{41,{{ 260.f, 25.f,  0.f, 0.f   },{0}, {Box}} },
		{42,{{ 260.f, 15.f,  0.f, 0.f   },{0}, {Box}} },
		{43,{{ 260.f,  5.f,  0.f, 0.f   },{0}, {Box}} },
		{44,{{ -20.f,  5.f, -280.f, 0.f },{0}, {Box}} },
		{45,{{ -20.f, 15.f, -280.f, 0.f },{0}, {Box}} },
		{46,{{ -20.f, 25.f, -280.f, 0.f },{0}, {Box}} },
		{47,{{ -20.f, 35.f, -280.f, 0.f },{0}, {Box}} },
		{48,{{ -10.f, 35.f, -280.f, 0.f },{0}, {Box}} },
		{49,{{  0.f,  35.f, -280.f, 0.f },{0}, {Box}} },
		{50,{{ 10.f,  35.f, -280.f, 0.f },{0}, {Box}} },
		{51,{{ 10.f,  25.f, -280.f, 0.f },{0}, {Box}} },
		{52,{{ 10.f,  15.f, -280.f, 0.f },{0}, {Box}} },
		{53,{{ 10.f,   5.f, -280.f, 0.f },{0}, {Box}} },
		{54,{{-BigWall_X1 - 200, 0.f, 480 * cosf(0.4f * MMPE_PI), 0.f}, {MMPE_PI / 8}, {SmallWall}} },
		{ 55,{ { -BigWall_X1 - 140.f, 0.f, 400.f * cosf(0.4f * MMPE_PI), 0 },{ MMPE_PI / 8 },{ SmallWall } } },
		{ 56,{ { -BigWall_X1 - 80, 0, 320 * cosf(0.4f * MMPE_PI), 0 },{ MMPE_PI / 8 },{ SmallWall } } },
		{ 57,{ { 0, 0, 530, 0 },{ MMPE_PI / 2 },{ SmallWall } } },
		{ 58,{ { 0, 0, 460, 0 },{ MMPE_PI / 2 },{ SmallWall } } },
		{ 59,{ { 0, 0, 390, 0 },{ MMPE_PI / 2 },{ SmallWall } } },
		{ 60,{ { BigWall_X1 + 200, 0, 480 * cosf(0.4f*MMPE_PI),0 },{ -MMPE_PI / 8 },{ SmallWall } } },
		{ 61,{ { BigWall_X1 + 140, 0, 400 * cosf(0.4f*MMPE_PI),0 },{ -MMPE_PI / 8 },{ SmallWall } } },
		{ 62,{ { BigWall_X1 + 80,  0, 320 * cosf(0.4f*MMPE_PI),0 },{ -MMPE_PI / 8 },{ SmallWall } } },
		{ 63,{ { 520 * sinf(0.8f*MMPE_PI), 0, -500 * cosf(0.2f*MMPE_PI),0 },{ MMPE_PI / 3 },{ SmallWall } } },
		{ 64,{ { 470 * sinf(0.8f*MMPE_PI), 0, -440 * cosf(0.2f*MMPE_PI),0 },{ MMPE_PI / 3 },{ SmallWall } } },
		{ 65,{ { 420 * sinf(0.8f*MMPE_PI), 0, -380 * cosf(0.2f*MMPE_PI),0 },{ MMPE_PI / 3 },{ SmallWall } } },
		{ 66,{ { -520 * sinf(0.8f*MMPE_PI), 0, -500 * cosf(0.2f*MMPE_PI),0 },{ -MMPE_PI / 3 },{ SmallWall } } },
		{ 67,{ { -470 * sinf(0.8f*MMPE_PI), 0, -440 * cosf(0.2f*MMPE_PI),0 },{ -MMPE_PI / 3 },{ SmallWall } } },
		{ 68,{ { -420 * sinf(0.8f*MMPE_PI), 0, -380 * cosf(0.2f*MMPE_PI),0 },{ -MMPE_PI / 3 },{ SmallWall } } },
		{ 69,{ { -BigWall_X1, 0, BigWall_Z1, 0 },{ -BigWall_Rad1 },{ BigWall } } },
		{ 70,{ { BigWall_X1, 0, BigWall_Z1, 0 },{ BigWall_Rad1 },{ BigWall } } },
		{ 71,{ { BigWall_X2, 0, BigWall_Z2, 0 },{ -BigWall_Rad1 * 2 },{ BigWall } } },
		{ 72,{ { 0,0,BigWall_Z3,0 },{ 0 },{ BigWall } } },
		{ 73,{ { -BigWall_X2, 0, BigWall_Z2, 0 },{ BigWall_Rad1 * 2 },{ BigWall } } },
		{ 74,{ { 90.f, 0.f, 60.f, 0.f },{ 0 },{ Building } }},
		{ 75,{ { -75.f, 0.f, 40.f, 0.f },{ 0 },{ Building }}},
		{ 76,{ { 0.f, 0.f, -100.f, 0.f },{ 0 },{ Building } }}
	};
}

void StaticObject::InitBoxObjects()
{
	//1. StaticObjectList에 데이터 넣어주기.
	//   한번 생성하고 바뀌지 않으므로 초기 한번만 실행
	for (int i = 0; i < MAX_BOX_NUM; ++i)
	{
		StaticObject *sobj = new StaticObject();
		sobj->pp = new PhysicsPoint();
		
		sobj->m_sobjdata.ID = i;
		sobj->m_sobjdata.Pos = m_SobjPos[i].pos;
		sobj->m_sobjdata.Rotate_status = { 0.0f, 0.0f, 0.0f, 1.0f };
		sobj->m_sobjdata.Fixed = true;
		sobj->m_sobjdata.degree = m_SobjPos[i].degree;
		sobj->m_sobjdata.type = Box;

		sobj->OffLookvector = XMFLOAT3(0, 0, 1);
		sobj->OffRightvector = XMFLOAT3(1, 0, 0);

		sobj->pe->UpdateLookVector(sobj->OffLookvector, sobj->OffRightvector, sobj->m_sobjdata.Rotate_status,
								   sobj->Lookvector, sobj->Rightvector);

		sobj->pe->GetUpVector(sobj->Lookvector, sobj->Rightvector, sobj->Upvector);

		XMFLOAT4 xmf4 = { sobj->m_sobjdata.Pos.x, sobj->m_sobjdata.Pos.y, sobj->m_sobjdata.Pos.z, sobj->m_sobjdata.Pos.w };
	
		//광선충돌 검사용 육면체
		//XMFLOAT3 rx(5, 0, 0);
		//XMFLOAT3 ry(0, 5, 0);
		//XMFLOAT3 rz(0, 0, 5);
		//rco.SetPlane(rx, ry, rz);

		sobj->pp->SetPosition(xmf4);
		sobj->pp->SetHalfBox(5, 5, 5);
		sobj->pp->SetDamping(0.5f);
		sobj->pp->SetBounce(false);
		sobj->pp->SetMass(INFINITY);

		m_sobjs.insert(sobj);
	}
}

void StaticObject::InitSmallWallObjects()
{
	for (int i = 0; i < MAX_SMALL_WALL_NUM; ++i)
	{
		StaticObject *sobj = new StaticObject();
		sobj->pp = new PhysicsPoint();

		sobj->m_sobjdata.ID = i + MAX_BOX_NUM;
		sobj->m_sobjdata.Pos = m_SobjPos[i + MAX_BOX_NUM].pos;
		sobj->m_sobjdata.Rotate_status = { 0.0f, 0.0f, 0.0f, 1.0f };
		sobj->m_sobjdata.Fixed = true;
		sobj->m_sobjdata.degree = m_SobjPos[i + MAX_BOX_NUM].degree;
		sobj->m_sobjdata.type = SmallWall;

		sobj->OffLookvector = XMFLOAT3(0, 0, 1);
		sobj->OffRightvector = XMFLOAT3(1, 0, 0);

		XMFLOAT4 xmf4_rot;
		xmf4_rot.x = m_sobjdata.Rotate_status.x;
		xmf4_rot.y = m_sobjdata.Rotate_status.y;
		xmf4_rot.z = m_sobjdata.Rotate_status.z;
		xmf4_rot.w = m_sobjdata.Rotate_status.w;

		auto q = XMLoadFloat4(&xmf4_rot);//방향을 degree만큼 돌리려 한다.
		XMFLOAT3 axis{ 0,1,0 };
		auto q2 = QuaternionRotation(axis, m_SobjPos[i+MAX_BOX_NUM].degree);
		xmf4_rot = QuaternionMultiply(xmf4_rot, q2);

		sobj->pe->UpdateLookVector(sobj->OffLookvector, sobj->OffRightvector, sobj->m_sobjdata.Rotate_status,
			sobj->Lookvector, sobj->Rightvector);

		sobj->pe->GetUpVector(sobj->Lookvector, sobj->Rightvector, sobj->Upvector);

		XMFLOAT4 xmf4 = { sobj->m_sobjdata.Pos.x, sobj->m_sobjdata.Pos.y, sobj->m_sobjdata.Pos.z, sobj->m_sobjdata.Pos.w };

		/*
		XMFLOAT3 raxis{ 0,1,0 };
		//광선충돌 검사용 육면체
		XMFLOAT3 rx(20, 0, 0);
		auto rqx = QuaternionRotation(raxis, m_SobjPos[i + MAX_BOX_NUM].degree);
		xmf4_rot = QuaternionMultiply(XMFLOAT4(rx.x, rx.y, rx.z, 0), rqx);
		rx.x = xmf4_rot.x; rx.y = xmf4_rot.y, rx.z = xmf4_rot.z;

		XMFLOAT3 ry(0, 10, 0);
		auto rqy = QuaternionRotation(raxis, m_SobjPos[i + MAX_BOX_NUM].degree);
		xmf4_rot = QuaternionMultiply(XMFLOAT4(ry.x, ry.y, ry.z, 0), rqy);
		ry.x = xmf4_rot.x; ry.y = xmf4_rot.y, ry.z = xmf4_rot.z;

		XMFLOAT3 rz(0, 0, 5);
		auto rqz = QuaternionRotation(raxis, m_SobjPos[i + MAX_BOX_NUM].degree);
		xmf4_rot = QuaternionMultiply(XMFLOAT4(ry.x, ry.y, ry.z, 0), rqz);
		rz.x = xmf4_rot.x; rz.y = xmf4_rot.y, rz.z = xmf4_rot.z;

		rco.SetPlane(rx, ry, rz);
		*/

		sobj->m_sobjdata.Rotate_status.x = xmf4_rot.x;
		sobj->m_sobjdata.Rotate_status.y = xmf4_rot.y;
		sobj->m_sobjdata.Rotate_status.z = xmf4_rot.z;
		sobj->m_sobjdata.Rotate_status.w = xmf4_rot.w;

		sobj->pp->SetPosition(xmf4);
		sobj->pp->SetHalfBox(20, 10, 5);
		sobj->pp->SetDamping(0.5f);
		sobj->pp->SetBounce(false);
		sobj->pp->SetMass(INFINITY);

		m_sobjs.insert(sobj);
	}
}

void StaticObject::InitBigWallObjects()
{
	for (int i = 0; i < MAX_BIG_WALL_NUM; ++i)
	{
		StaticObject *sobj = new StaticObject();
		sobj->pp = new PhysicsPoint();

		sobj->m_sobjdata.ID = i + MAX_BOX_NUM + MAX_SMALL_WALL_NUM;
		sobj->m_sobjdata.Pos = m_SobjPos[i + MAX_BOX_NUM + MAX_SMALL_WALL_NUM].pos;
		sobj->m_sobjdata.Rotate_status = { 0.0f, 0.0f, 0.0f, 1.0f };
		sobj->m_sobjdata.Fixed = true; 
		sobj->m_sobjdata.degree = m_SobjPos[i + MAX_BOX_NUM + MAX_SMALL_WALL_NUM].degree;
		sobj->m_sobjdata.type = BigWall;

		sobj->OffLookvector = XMFLOAT3(0, 0, 1);
		sobj->OffRightvector = XMFLOAT3(1, 0, 0);

		XMFLOAT4 xmf4_rot;
		xmf4_rot.x = m_sobjdata.Rotate_status.x;
		xmf4_rot.y = m_sobjdata.Rotate_status.y;
		xmf4_rot.z = m_sobjdata.Rotate_status.z;
		xmf4_rot.w = m_sobjdata.Rotate_status.w;

		auto q = XMLoadFloat4(&xmf4_rot);//방향을 degree만큼 돌리려 한다.
		XMFLOAT3 axis{ 0,1,0 };
		auto q2 = QuaternionRotation(axis, m_SobjPos[i + MAX_BOX_NUM + MAX_SMALL_WALL_NUM].degree);
		xmf4_rot = QuaternionMultiply(xmf4_rot, q2);

		sobj->pe->UpdateLookVector(sobj->OffLookvector, sobj->OffRightvector, sobj->m_sobjdata.Rotate_status,
			sobj->Lookvector, sobj->Rightvector);

		sobj->pe->GetUpVector(sobj->Lookvector, sobj->Rightvector, sobj->Upvector);

		XMFLOAT4 xmf4 = { sobj->m_sobjdata.Pos.x, sobj->m_sobjdata.Pos.y, sobj->m_sobjdata.Pos.z, sobj->m_sobjdata.Pos.w };

		/*
		XMFLOAT3 raxis{ 0,1,0 };
		//광선충돌 검사용 육면체
		XMFLOAT3 rx(350, 0, 0);
		auto rqx = QuaternionRotation(raxis, m_SobjPos[i + MAX_BOX_NUM].degree);
		xmf4_rot = QuaternionMultiply(XMFLOAT4(rx.x, rx.y, rx.z, 0), rqx);
		rx.x = xmf4_rot.x; rx.y = xmf4_rot.y, rx.z = xmf4_rot.z;

		XMFLOAT3 ry(0, 50, 0);
		auto rqy = QuaternionRotation(raxis, m_SobjPos[i + MAX_BOX_NUM].degree);
		xmf4_rot = QuaternionMultiply(XMFLOAT4(ry.x, ry.y, ry.z, 0), rqy);
		ry.x = xmf4_rot.x; ry.y = xmf4_rot.y, ry.z = xmf4_rot.z;

		XMFLOAT3 rz(0, 0, 5);
		auto rqz = QuaternionRotation(raxis, m_SobjPos[i + MAX_BOX_NUM].degree);
		xmf4_rot = QuaternionMultiply(XMFLOAT4(ry.x, ry.y, ry.z, 0), rqz);
		rz.x = xmf4_rot.x; rz.y = xmf4_rot.y, rz.z = xmf4_rot.z;

		rco.SetPlane(rx, ry, rz);
		*/

		sobj->m_sobjdata.Rotate_status.x = xmf4_rot.x;
		sobj->m_sobjdata.Rotate_status.y = xmf4_rot.y;
		sobj->m_sobjdata.Rotate_status.z = xmf4_rot.z;
		sobj->m_sobjdata.Rotate_status.w = xmf4_rot.w;

		sobj->pp->SetPosition(xmf4);
		sobj->pp->SetHalfBox(350, 50, 5);
		sobj->pp->SetDamping(0.5f);
		sobj->pp->SetBounce(false);
		sobj->pp->SetMass(INFINITY);

		m_sobjs.insert(sobj);
	}
}

void StaticObject::InitBuildingObjects()
{
	for (int i = 0; i < MAX_BULLDING_NUM; ++i)
	{
		StaticObject *sobj = new StaticObject();
		sobj->pp = new PhysicsPoint();

		sobj->m_sobjdata.ID = i + MAX_BOX_NUM + MAX_SMALL_WALL_NUM + MAX_BIG_WALL_NUM;
		sobj->m_sobjdata.Pos = m_SobjPos[i + MAX_BOX_NUM + MAX_SMALL_WALL_NUM + MAX_BIG_WALL_NUM].pos;
		sobj->m_sobjdata.Rotate_status = { 0.0f, 0.0f, 0.0f, 1.0f };
		sobj->m_sobjdata.Fixed = true;
		sobj->m_sobjdata.degree = m_SobjPos[i + MAX_BOX_NUM + MAX_SMALL_WALL_NUM + MAX_BIG_WALL_NUM].degree;
		sobj->m_sobjdata.type = Building;

		sobj->OffLookvector = XMFLOAT3(0, 0, 1);
		sobj->OffRightvector = XMFLOAT3(1, 0, 0);

		XMFLOAT4 xmf4_rot;
		xmf4_rot.x = m_sobjdata.Rotate_status.x;
		xmf4_rot.y = m_sobjdata.Rotate_status.y;
		xmf4_rot.z = m_sobjdata.Rotate_status.z;
		xmf4_rot.w = m_sobjdata.Rotate_status.w;

		auto q = XMLoadFloat4(&xmf4_rot);//방향을 degree만큼 돌리려 한다.
		XMFLOAT3 axis{ 0,1,0 };
		auto q2 = QuaternionRotation(axis, m_SobjPos[i + MAX_BOX_NUM + MAX_SMALL_WALL_NUM + MAX_BIG_WALL_NUM].degree);
		xmf4_rot = QuaternionMultiply(xmf4_rot, q2);

		sobj->pe->UpdateLookVector(sobj->OffLookvector, sobj->OffRightvector, sobj->m_sobjdata.Rotate_status,
			sobj->Lookvector, sobj->Rightvector);

		sobj->pe->GetUpVector(sobj->Lookvector, sobj->Rightvector, sobj->Upvector);

		XMFLOAT4 xmf4 = { sobj->m_sobjdata.Pos.x, sobj->m_sobjdata.Pos.y, sobj->m_sobjdata.Pos.z, sobj->m_sobjdata.Pos.w };

		//광선충돌 검사용 육면체
		//XMFLOAT3 rx(15, 0, 0);
		//XMFLOAT3 ry(0, 45, 0);
		//XMFLOAT3 rz(0, 0, 15);
		//rco.SetPlane(rx, ry, rz);
		
		sobj->m_sobjdata.Rotate_status.x = xmf4_rot.x;
		sobj->m_sobjdata.Rotate_status.y = xmf4_rot.y;
		sobj->m_sobjdata.Rotate_status.z = xmf4_rot.z;
		sobj->m_sobjdata.Rotate_status.w = xmf4_rot.w;

		sobj->pp->SetPosition(xmf4);
		sobj->pp->SetHalfBox(15, 45, 15);
		sobj->pp->SetDamping(0.5f);
		sobj->pp->SetBounce(false);
		sobj->pp->SetMass(INFINITY);

		m_sobjs.insert(sobj);
	}
}

void StaticObject::AfterGravitySystem()
{
	for(auto sobj : m_sobjs)
	{
		//왜 실제 중점이 아닌 pp의 중점으로 처리하냐면 실제중점을 움직인후 pp의 중점을 움직이나
		//pp의중점을 움직이고 실제중점을 움직이나 같지만, UpdatePPosCenterPos를 쓰기위해
		//pp를 움직이고 cp를 pp로 맞춘다.
		float ppy = sobj->pp->GetPosition().y;
		float hby = sobj->pp->GetHalfBox().y;
		if (ppy - hby < 0)											//pp의 중점y-하프박스의 y값을 한결과가 0보다 작으면 땅아래에 묻힌셈
		{
			XMFLOAT3 gp = sobj->pp->GetPosition();
			gp.y += hby - ppy;										//그러면 반대로 하프박스y값-중점y만큼 올리면 된다.
			sobj->pp->SetPosition(gp);
			sobj->UpdatePPosCenterPos(sobj->pp->GetPosition());
			auto v = sobj->pp->GetVelocity();
			v.y = 0;												//중력에 의한 속도를 0으로 만듬
			sobj->pp->SetVelocity(v);
			sobj->m_airbone = false;
		}
	}
}

void StaticObject::UpdatePPosCenterPos(XMFLOAT3& xmf3)
{
	XMFLOAT4 xmf4 = XMFloat3to4(xmf3);

	m_sobjdata.Pos.x = xmf4.x;
	m_sobjdata.Pos.y = xmf4.y; 
	m_sobjdata.Pos.z = xmf4.z;
	m_sobjdata.Pos.w = xmf4.w;
}


StaticObject::~StaticObject()
{
	for (auto sobj : m_sobjs)
		delete sobj;

	delete pe;

}


//#include "CStaticObject.h"
#include "../stdafx.h"

map<int, BasicInfo> CStaticObject::m_sobj_bdata;

CStaticObject::CStaticObject()
{
}

void CStaticObject::MakeStaticObjectBasicData()
{
	//여기다 물체 배치에 대한 상수들 넣는다.
	float BigWall_X1 = 300 * sinf(0.4f * MMPE_PI);
	float BigWall_Z1 = (600 * cosf(0.4f * MMPE_PI) + 600) / 2;

	float BigWall_X2 = ((600 * sinf(0.4f * MMPE_PI)) + (600 * sinf(0.8f * MMPE_PI))) / 2;
	float BigWall_Z2 = ((600 * cosf(0.4f * MMPE_PI)) + (-600 * cosf(0.2f * MMPE_PI))) / 2;

	float BigWall_Z3 = -600 * cosf(0.2f * MMPE_PI);

	float BigWall_Rad1 = MMPE_PI / 5; //36degree

	m_sobj_bdata =
	{
		{ 0,{ { 40.f, 18.f, -70.f, 0.f },{ 0 },{ Box } } },
		{ 1,{ { 50.f, 70.f, -80.f, 0.f },{ 0 },{ Box } } },
		{ 2,{ { 35.f, 50.f, -70.f, 0.f },{ 0 },{ Box } } },
		{ 3,{ { 118.f, 58.f, 47.f, 0.f },{ 0 },{ Box } } },
		{ 4,{ { 102.f, 69.f, 81.f, 0.f },{ 0 },{ Box } } },
		{ 5,{ { 58.f, 39.f,  78.f, 0.f },{ 0 },{ Box } } },
		{ 6,{ { 65.f, 35.f,-110.f, 0.f },{ 0 },{ Box } } },
		{ 7,{ { 92.f, 61.f,  60.f, 0.f },{ 0 },{ Box } } },
		{ 8,{ { 103.f, 36.f, 15.f, 0.f },{ 0 },{ Box } } },
		{ 9,{ { 65.f, 17.f, -65.f ,0.f },{ 0 },{ Box } } },
		{ 10,{ { 82.f, 11.f, 34.f, 0.f },{ 0 },{ Box } } },
		{ 11,{ { 72.f, 53.f, 41.f, 0.f },{ 0 },{ Box } } },
		{ 12,{ { 24.f, 44.f, 40.f, 0.f },{ 0 },{ Box } } },
		{ 13,{ { -20.f, 33.f, 51.f,0.f },{ 0 },{ Box } } },
		{ 14,{ { 0.f, 24.f, 34.f ,0.f },{ 0 },{ Box } } },
		{ 15,{ { 0.f, 58.f, -71.f, 0.f },{ 0 },{ Box } } },
		{ 16,{ { -76.f, 53.f, 8.f, 0.f },{ 0 },{ Box } } },
		{ 17,{ { -40.f, 10.f, 0.f, 0.f },{ 0 },{ Box } } },
		{ 18,{ { -38.f, 48.f, -126.f, 0.f },{ 0 },{ Box } } },
		{ 19,{ { -45.f, 30.f, -60.f, 0.f },{ 0 },{ Box } } },
		{ 20,{ { -65.f, 40.f, -102.f, 0.f },{ 0 },{ Box } } },
		{ 21,{ { -69.f, 62.f,  61.f, 0.f },{ 0 },{ Box } } },
		{ 22,{ { -85.f, 42.f, -45.f, 0.f },{ 0 },{ Box } } },
		{ 23,{ { -106.f, 40.f,  42.f, 0.f },{ 0 },{ Box } } },
		{ 24,{ { -20.f, 5.f, 210.f, 0.f },{ 0 },{ Box } } },
		{ 25,{ { -20.f, 15.f, 210.f ,0.f },{ 0 },{ Box } } },
		{ 26,{ { -20.f, 25.f, 210.f, 0.f },{ 0 },{ Box } } },
		{ 27,{ { -20.f, 35.f, 210.f, 0.f },{ 0 },{ Box } } },
		{ 28,{ { -10.f, 35.f, 210.f, 0.f },{ 0 },{ Box } } },
		{ 29,{ { 0.f, 35.f, 210.f,0.f },{ 0 },{ Box } } },
		{ 30,{ { 10.f, 35.f, 210.f ,0.f },{ 0 },{ Box } } },
		{ 31,{ { 10.f, 25.f, 210.f, 0.f },{ 0 },{ Box } } },
		{ 32,{ { 10.f, 15.f, 210.f ,0.f },{ 0 },{ Box } } },
		{ 33,{ { 10.f, 5.f, 210.f, 0.f },{ 0 },{ Box } } },
		{ 34,{ { 260.f, 5.f,  -30.f, 0.f },{ 0 },{ Box } } },
		{ 35,{ { 260.f, 15.f, -30.f, 0.f },{ 0 },{ Box } } },
		{ 36,{ { 260.f, 25.f, -30.f, 0.f },{ 0 },{ Box } } },
		{ 37,{ { 260.f, 35.f, -30.f, 0.f },{ 0 },{ Box } } },
		{ 38,{ { 260.f, 35.f, -20.f, 0.f },{ 0 },{ Box } } },
		{ 39,{ { 260.f, 35.f, -10.f, 0.f },{ 0 },{ Box } } },
		{ 40,{ { 260.f, 35.f,  0.f, 0.f },{ 0 },{ Box } } },
		{ 41,{ { 260.f, 25.f,  0.f, 0.f },{ 0 },{ Box } } },
		{ 42,{ { 260.f, 15.f,  0.f, 0.f },{ 0 },{ Box } } },
		{ 43,{ { 260.f,  5.f,  0.f, 0.f },{ 0 },{ Box } } },
		{ 44,{ { -20.f,  5.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 45,{ { -20.f, 15.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 46,{ { -20.f, 25.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 47,{ { -20.f, 35.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 48,{ { -10.f, 35.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 49,{ { 0.f,  35.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 50,{ { 10.f,  35.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 51,{ { 10.f,  25.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 52,{ { 10.f,  15.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 53,{ { 10.f,   5.f, -280.f, 0.f },{ 0 },{ Box } } },
		{ 54,{ { -BigWall_X1 - 200, 0.f, 480 * cosf(0.4f * MMPE_PI), 0.f },{ MMPE_PI / 8 },{ SmallWall } } },
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
		{ 74,{ { 90.f, 0.f, 60.f, 0.f },{ 0 },{ NormalBuilding } } },
		{ 75,{ { -75.f, 0.f, 40.f, 0.f },{ 0 },{ NormalBuilding } } },
		{ 76,{ { 0.f, 0.f, -100.f, 0.f },{ 0 },{ NormalBuilding } } },
		{ 77, {{0.f, 200.0f, 290.0f, 0.0f},{0},{Rigidbody}}},
		{ 78,{ {-270.f, 250.f, 60.f, 0.0f },{ 0 },{ Rigidbody } } },
		{ 79,{ { 270.f, 330.f, 60.f, 0.0f },{ 0 },{ Rigidbody } } },
		{ 80,{ { -210.f, 390.f, -200.f, 0.f },{ 0 },{ Rigidbody } } },
		{ 81,{ { 200.f, 180.0f, -180.0f, 0.f },{ 0 },{ Rigidbody } } },
		{ 82,{ { 80.f, 310.0f, -30.0f, 0.f },{ 0 },{ Rigidbody } } },
		{ 83,{ { -31.f, 250.0f, 160.0f, 0.f },{ 0 },{ Rigidbody } } },
		{ 84,{ { 90.f, 270.f, -340.f, 0.f },{ 0 },{ Rigidbody } } },
		{ 85,{ { -70.f, 220.f, -55.f, 0.f },{ 0 },{ Rigidbody } } },

	};
}

void CStaticObject::CreateGameObject()
{
	for (unsigned int i = 0; i < MAX_BOX_NUM; ++i)
		g_staticobjs.insert(new NormalBoxObject(i));
	
	auto max_fsize = MAX_BOX_NUM + MAX_SMALL_WALL_NUM;
	for (unsigned int i = MAX_BOX_NUM; i < max_fsize; ++i)
		g_staticobjs.insert(new SmallWallObject(i));
	
	auto max_ssize = max_fsize + MAX_BIG_WALL_NUM;
	for (unsigned int i = max_fsize; i < max_ssize; ++i)
		g_staticobjs.insert(new BigWallObject(i));
	
	auto max_tsize = max_ssize + MAX_BULLDING_NUM;
	for (unsigned int i = max_ssize; i < max_tsize; ++i)
		g_staticobjs.insert(new Building(i));
	
	auto max_lsize = max_tsize + MAX_RIGIDBODY_NUM;
	for (unsigned int i = max_tsize; i < max_lsize; ++i)
		g_rigidobjs.insert(new RigidCubeObject(i));


	for (auto sobj : g_staticobjs)
		sobj->AfterGravitySystem(0);


}

void CStaticObject::StaticObjectLoop()
{
	MakeStaticObjectBasicData();
	CreateGameObject();
}

void CStaticObject::AfterGravitySystem(double deltime)
{
	float ppy = pp->GetPosition().y;
	float hby = pp->GetHalfBox().y;
	if (ppy - hby < 0)
	{
		XMFLOAT4 gp = pp->GetPosition();
		gp.y += hby - ppy;
		*pp->CenterPos = gp;
		UpdatePPosCenterPos();

		auto v = pp->GetVelocity();
		v.y = 0;
		pp->SetVelocity(v);
		m_airbone = false;
	}
}

CStaticObject::~CStaticObject()
{
	if (pp != nullptr)
		delete pp;

	if (rb != nullptr)
		delete rb;
}

NormalBoxObject::NormalBoxObject(unsigned int id)
{
	pp = new PhysicsPoint();
	
	m_id = id;
	m_fixed = true;
	m_alive = true;
	m_pos4f = m_sobj_bdata[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };

	m_dir = 0;
	m_ai = true;
	m_godmode = true;
	m_airbone = false;
	m_type = Box;

	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.attack = 0;
	m_ability.speed = 0;
	m_ability.level = 1;
	m_ability.exp = 0;

	m_degree = m_sobj_bdata[m_id].degree;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookvector();
	UpdateUpvector();

	//pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
	//XMFLOAT4 xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(5, 5, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//client 통신용 send packet에 들어갈 정보들
	m_stc_sobjdata.ID = m_id;
	m_stc_sobjdata.degree = m_degree;
	m_stc_sobjdata.Fixed = m_fixed;
	m_stc_sobjdata.Pos = m_pos4f;
	m_stc_sobjdata.Rotate_status = m_rot4f;
	m_stc_sobjdata.type = m_type;
	//
}

SmallWallObject::SmallWallObject(unsigned int id)
{
	pp = new PhysicsPoint();

	m_id = id;
	m_fixed = true;
	m_alive = true;
	m_pos4f = m_sobj_bdata[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };
	m_dir = 0;
	m_ai = true;
	m_godmode = true;
	m_airbone = false;
	m_type = SmallWall;

	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.attack = 0;
	m_ability.speed = 0;
	m_ability.level = 1;
	m_ability.exp = 0;

	m_degree = m_sobj_bdata[m_id].degree;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	//XMFLOAT4 xmf4_rot{ m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };

	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);

	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };
	
	UpdateLookvector();
	UpdateUpvector();

	//pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
	//XMFLOAT4 xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(20, 10, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//client 통신용 send packet에 들어갈 정보들
	m_stc_sobjdata.ID = m_id;
	m_stc_sobjdata.degree = m_degree;
	m_stc_sobjdata.Fixed = m_fixed;
	m_stc_sobjdata.Pos = m_pos4f;
	m_stc_sobjdata.Rotate_status = m_rot4f;
	m_stc_sobjdata.type = m_type;
	//
}

BigWallObject::BigWallObject(unsigned int id)
{
	pp = new PhysicsPoint();

	m_id = id;
	m_fixed = true;
	m_alive = true;
	m_pos4f = m_sobj_bdata[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };
	m_dir = 0;
	m_ai = true;
	m_godmode = true;
	m_type = BigWall;

	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.attack = 0;
	m_ability.speed = 0;
	m_ability.level = 1;
	m_ability.exp = 0;

	m_degree = m_sobj_bdata[m_id].degree;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	//XMFLOAT4 xmf4_rot{ m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };

	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);

	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();
	UpdateUpvector();

	//pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
	//XMFLOAT4 xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };

	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(350, 50, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//client 통신용 send packet에 들어갈 정보들
	m_stc_sobjdata.ID = m_id;
	m_stc_sobjdata.degree = m_degree;
	m_stc_sobjdata.Fixed = m_fixed;
	m_stc_sobjdata.Pos = m_pos4f;
	m_stc_sobjdata.Rotate_status = m_rot4f;
	m_stc_sobjdata.type = m_type;
	//
}

Building::Building(unsigned int id)
{
	pp = new PhysicsPoint();

	m_id = id;
	m_fixed = true;
	m_alive = true;
	m_pos4f = m_sobj_bdata[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };
	m_dir = 0;
	m_ai = true;
	m_godmode = true;
	m_type = NormalBuilding;

	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.attack = 0;
	m_ability.speed = 0;
	m_ability.level = 1;
	m_ability.exp = 0;

	m_degree = m_sobj_bdata[m_id].degree;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	//XMFLOAT4 xmf4_rot{ m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };

	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);

	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };			
	
	//스테틱오브젝트 충돌 시, 회전된 스테틱오브젝트들은 Lookvector 와 Rightvector를 회전한 값에 따라 바꿔줘야한다
	//그래야지 충돌처리할 때 Lookvector, Rightvector를 이용하는데, 바뀐 값이 적용되서 충돌처리를 한다 
	UpdateLookvector();
	UpdateUpvector();

	//pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
	//XMFLOAT4 xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };

	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(15, 45, 15);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//client 통신용 send packet에 들어갈 정보들
	m_stc_sobjdata.ID = m_id;
	m_stc_sobjdata.degree = m_degree;
	m_stc_sobjdata.Fixed = m_fixed;
	m_stc_sobjdata.Pos = m_pos4f;
	m_stc_sobjdata.Rotate_status = m_rot4f;
	m_stc_sobjdata.type = m_type;
	//
}

RigidCubeObject::RigidCubeObject(unsigned int id)
{
	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);
	
	UpdateLookvector();
	UpdateUpvector();

	m_fixed = false;
	m_ability.attack = 0;
	m_ability.orignHP = 100;
	m_ability.curHP = 100;
	m_godmode = true;
	m_ability.speed = 0;

	rb = new RigidBody();

	//XMFLOAT4 xmf4 = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	xmf4_pos = XMFLOAT4(m_sobj_bdata[id].pos.x, m_sobj_bdata[id].pos.y, m_sobj_bdata[id].pos.z, m_sobj_bdata[id].pos.w);
	rb->SetPosition(&xmf4_pos);
	rb->SetHalfBox(10, 10, 10);
	rb->SetDamping(0.5f, 0.38f);
	rb->SetBounce(false);
	rb->SetMass(1.5);
	rb->SetIMoment(10, 10, 10);

	//XMFLOAT4 Orient = { 0,0,0,1 };
	//rb->SetOrient(&Orient);
	xmf4_rot = { 0,0,0,1 };
	rb->SetOrient(&xmf4_rot);

	XMFLOAT3 testForce{ -5,-3,2 };
	XMFLOAT3 testPoint{ -15, 5,-5 };

	rb->AddForcePoint(testForce, testPoint);
	rb->integrate(0.1);

}

void RigidCubeObject::AmendObject(XMFLOAT3 axis, float radian, CMonoObject * obj)
{
	XMFLOAT4 q = QuaternionRotation(axis, radian);
	obj->SetRotatePos4f(QuaternionMultiply(obj->GetRotatePos4f(), q));
	obj->UpdateLookvector();
}

void RigidCubeObject::RigidBodyCollisionPlane(XMFLOAT3 & Normal, float distance, double deltime, CMonoObject * obj)
{
	
	if (obj->GetRigidBody() != nullptr)
	{
		if (obj->GetRigidBody()->AmendTime > 0)
		{
			obj->GetRigidBody()->AmendTime -= deltime;

			if (obj->GetRigidBody()->AmendTime <= 0)
			{
				obj->GetRigidBody()->AmendTime = 0;
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);
				obj->GetRigidBody()->SetVelocity(0, 0, 0);
			}
		}
		else
		{
			obj->GetRigidBody()->AmendTime = 0;
		}

		XMFLOAT4 arr[8];

		//먼저 8 개의 점을 가져온다.
		obj->GetRigidBody()->GetEightPoint(arr, obj->GetUpVector(), obj->GetLookVector(), obj->GetRightVector());
		vector<CollisionPoint> allpoint;
		vector<CollisionPoint> tempcollisionpoint;
		vector<CollisionPoint> contactpoint;

		for (int i = 0; i < 8; ++i)
		{
			float temppenetration = arr[i].x * Normal.x + arr[i].y * Normal.y + arr[i].z * Normal.z;
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
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0이하면 실제로 충돌함
				tempcollisionpoint.push_back(i);

		}
		//이제 가장 깊은 녀석을 기준으로 충돌리스트를 돌면서 penetration을 뺐을때 결과가 충분히 작으면
		//이녀석도 밀어내거나 힘을가할때 사용될 수 있는 충돌점이다. 이 것들이 최종적인 충돌점이다.

		// 7) 번을 처리함
		for (auto i : tempcollisionpoint)
		{
			//실제충돌의 첫번째 깊이 - penetration들을 비교해서 Epsilon이면 이제 실제 접촉점이 저장되는 곳에 넣는다.
			if (fabsf(tempcollisionpoint.front().penetration - i.penetration) <= 0.12)//차가 0.1정도면 실제로 충돌 가능성이 있다고 판단한다.
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
			//따라서 적당한 회전각도가 되면 그 각도로 보정한다. 다만 해당 충격량이 너무 크지 않을때만 보정한다.

			//2인자점 - 1인자점을 해서 2인자점을 향하는 벡터를 만든다.
			//이때 2인자 점을 향하는 각도가 특정각도 이하면 그 각도로 보정한다.

			//문제는 2인자점을 어느것으로 할것인가 이다. 경우의수는 2가지다. 왼쪽이나 오른쪽점 / 또는 대각선 점.

			//가장가까운 2인자점.
			auto V1 = Float4Add(allpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			//현재점의 대각선에 위치한 2인자점
			auto sV1 = Float4Add(allpoint[3].Pos, contactpoint[0].Pos, false);
			sV1 = Float4Normalize(sV1);


			//가장가까운 2인자점의 각도를 구하기
			float NdotV1 = Normal.x*V1.x + Normal.y*V1.y + Normal.z*V1.z;

			XMFLOAT3 ProjAB = Normal;
			ProjAB.x *= NdotV1;
			ProjAB.y *= NdotV1;
			ProjAB.z *= NdotV1;

			auto V2 = Float4Add(V1, XMFloat3to4(ProjAB), false);
			V2 = Float4Normalize(V2);


			auto tempdot = V1.x*V2.x + V1.y*V2.y + V1.z*V2.z;
			if (abs(tempdot) > 1)
			{
				if (tempdot > 0)
					tempdot = 1;
				else
					tempdot = -1;
			}
			//교정할 각도.  
			double theta = acos(tempdot);

			//대각선 2인자점의 각도를 구하기
			float sNdotV1 = Normal.x*sV1.x + Normal.y*sV1.y + Normal.z*sV1.z;

			XMFLOAT3 sProjAB = Normal;
			sProjAB.x *= sNdotV1;
			sProjAB.y *= sNdotV1;
			sProjAB.z *= sNdotV1;

			auto sV2 = Float4Add(sV1, XMFloat3to4(sProjAB), false);
			sV2 = Float4Normalize(sV2);


			auto stempdot = sV1.x*sV2.x + sV1.y*sV2.y + sV1.z*sV2.z;
			if (abs(stempdot) > 1)
			{
				if (stempdot > 0)
					stempdot = 1;
				else
					stempdot = -1;
			}
			//교정할 각도.  
			double stheta = acos(stempdot);

			//충격량을 구함. 충격량이 특정 값 이하일때만 보정가능.

			CollisionPoint fp;//충격량을 가할 지점
			fp.Pos = XMFLOAT4(0, 0, 0, 0);
			fp.pAxis = Normal;

			fp.Pos = contactpoint[0].Pos;
			fp.penetration = contactpoint[0].penetration;
			float impurse = obj->GetRigidBody()->CalculateImpulse(fp, NULL, 1);

			//최대임펄스를 구한다.
			if (impurse > obj->GetRigidBody()->GetMaxImpurse())
				impurse = obj->GetRigidBody()->GetMaxImpurse();


			//최소 임펄스를 구한다.
			if (impurse < obj->GetRigidBody()->GetMinImpurse())
				impurse = obj->GetRigidBody()->GetMinImpurse();




			//그후 사잇각이 특정각도 이하면 보정시킨다. 
			//단 이게 double로 해도 0이아닌데 0이나오는경우가 생긴다.
			//따라서 0일경우 그냥 충격량을 가해서 각도를 변경시킨다.

			if (abs(stheta) <= MMPE_PI / 20 && abs(stheta) != 0 && abs(impurse) < obj->GetRigidBody()->GetMaxImpurse() && obj->GetRigidBody()->AmendTime <= 0)
			{
				//회전축을 구하고..
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(sV1, sV2));
				mAxis = Float3Normalize(mAxis);
				//보정을 시킨다.
				AmendObject(mAxis, stheta, obj);

				//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
				//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

			}
			else if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && obj->GetRigidBody()->GetMaxImpurse() && obj->GetRigidBody()->AmendTime <= 0)
			{
				//대략 5도 이하면 보정시킴.

				//회전축을 구함
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V2));
				mAxis = Float3Normalize(mAxis);

				//보정을 시킨다
				AmendObject(mAxis, theta, obj);

				//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
				//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

			}
			else //아니라면 이제 충격량을 적당한 지점에 가한다!
			{
				//여기에 왔다는것은 더이상 보정을 하지 않거나 보정을 아직 할필요가 없는 경우다.

				//Jm = J/M
				//임펄스의 비율을 나눈다.  즉. 일반적인 1:1 관계에서 1.3: 0.3 정도로 둔다.
				//나머지 0.7은 소실된 에너지라 치자.
				if (obj->GetRigidBody()->AmendTime <= 0)
				{
					auto ratioImpurse = impurse * 0.3;

					auto Jm = Normal;
					Jm.x *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);
					Jm.y *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);
					Jm.z *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);


					//각속도 계산
					//W = 기존 각속도 + ((Q-P)Ximpurse)*InverseI
					auto W = obj->GetRigidBody()->GetAngularVelocity();
					XMVECTOR rxi = XMLoadFloat3(&XMFloat4to3(Float4Add(fp.Pos, obj->GetCenterPos4f(), false)));
					rxi = XMVector3Cross(rxi, XMLoadFloat3(&Normal));
					rxi *= (ratioImpurse);
					rxi = XMVector3Transform(rxi, XMLoadFloat4x4(&obj->GetRigidBody()->GetIMoment()));

					XMFLOAT3 ia;
					XMStoreFloat3(&ia, rxi);

					W = Float3Add(W, ia);
					W = Float3Float(W, obj->GetRigidBody()->GetE() / 2);
					XMFLOAT3 lastvel = obj->GetRigidBody()->GetVelocity();
					obj->GetRigidBody()->SetVelocity(Float3Float(Float3Add(lastvel, Jm), obj->GetRigidBody()->GetE()));
					obj->GetRigidBody()->SetAngularVelocity(W);
				}

				//이제 속도와 각속도는 변경 했으니, 겹쳐진 부분 해소
				//가장 작은값의 penetration(가장 깊은)만큼 올리면 된다.
				auto px = fabsf(contactpoint[0].penetration)*Normal.x;
				auto py = fabsf(contactpoint[0].penetration)*Normal.y;
				auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
				obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
		
			}
		}
		else if (size == 2)
		{
			//삐뚫어진 점 2개면 보정을 해야한다.
			//보정을 해야하는지 검사를 하기 위해 평면의 노멀과 접촉점 두개를 이은 벡터가 내적시 0이 나오는지 검사한다.
			auto V1 = Float4Add(contactpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			float Dot = V1.x * Normal.x + V1.y * Normal.y + V1.z * Normal.z;

			if (fabsf(Dot) <= MMPE_EPSILON)
				Dot = 0;

			//결과가 0이면 보정이 필요없음.
			if (Dot == 0)
			{
				//이제 2개의 충돌을 4개의 점충돌로 바꿀 수 있는 보정을 할 수 있는지 검사한다.
				//그러기 위해서 3번째 점- 첫번째 점 을 해서 3번째 점을 향하는 벡터를 만든다.
				//그 후 이게 V1과 내적시 0 이 나오면 첫번째 점과 세번째 점이 대각선이 아니란것이고,
				//아니라면 두번째 점과 세번째점을 이용해 벡터를 만들어야한다.
				auto V2 = Float4Add(allpoint[2].Pos, contactpoint[0].Pos, false);
				V2 = Float4Normalize(V2);
				float Dot2 = V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;

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

				//기존에는 float으로 했는데, 0이아니어야 하는데 0이나오는경우가 생김..


				auto tempdot = V2.x*V3.x + V2.y*V3.y + V2.z*V3.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//교정할 각도.  
				double theta = acos(tempdot);


				CollisionPoint fp;//충격량을 가할 지점
				fp.Pos = XMFLOAT4(0, 0, 0, 0);
				fp.pAxis = Normal;

				for (auto i : contactpoint)
				{
					fp.Pos = Float4Add(fp.Pos, i.Pos);
					fp.penetration += i.penetration;
				}

				fp.Pos.x /= contactpoint.size();
				fp.Pos.y /= contactpoint.size();
				fp.Pos.z /= contactpoint.size();

				fp.penetration /= contactpoint.size();

				float impurse = obj->GetRigidBody()->CalculateImpulse(fp, NULL, 1);

				//최대임펄스를 구한다.
				if (impurse > obj->GetRigidBody()->GetMaxImpurse())
					impurse = obj->GetRigidBody()->GetMaxImpurse();


				//최소 임펄스를 구한다.
				if (impurse < obj->GetRigidBody()->GetMinImpurse())
					impurse = obj->GetRigidBody()->GetMinImpurse();


				//그후 사잇각이 특정각도 이하면 보정시킨다. 
				if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && abs(impurse) < obj->GetRigidBody()->GetMaxImpurse() && obj->GetRigidBody()->AmendTime <= 0)//대략 5도 이하면 보정시킴.
				{
					//회전축을 구하고
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
					mAxis = Float3Normalize(mAxis);
					//보정을 시킨다.
					AmendObject(mAxis, theta, obj);

					//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
					//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
					obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);
					obj->GetRigidBody()->SetAccel(0, 0, 0);


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

					//먼저 2/3/4/5 인덱스의 점을 비교하면서 이 길이가 0.0001정도 차이면 통과.
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

					//4개가 모두 같은 깊이면 균형을 이루는것이므로 균형을 부셔버린다.
					if (pass)
					{
						theta = -MMPE_PI / 18;
						XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
						mAxis = Float3Normalize(mAxis);
						//보정을 시킨다.
						AmendObject(mAxis, theta, obj);

						//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
						//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
						obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);


						return;
					}

					//Jm = J/M

					//임펄스의 비율을 나눈다.  즉. 일반적인 1:1 관계에서 1.3: 0.3 정도로 둔다.

					if (obj->GetRigidBody()->AmendTime <= 0)
					{
						auto ratioImpurse = impurse * 0.3;

						auto Jm = Normal;



						Jm.x *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);
						Jm.y *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);
						Jm.z *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);



						//각속도 계산
						//W = 기존 각속도 + ((Q-P)Ximpurse)*InverseI
						auto W = obj->GetRigidBody()->GetAngularVelocity();
						XMVECTOR rxi = XMLoadFloat3(&XMFloat4to3(Float4Add(fp.Pos, obj->GetCenterPos4f(), false)));
						rxi = XMVector3Cross(rxi, XMLoadFloat3(&Normal));
						rxi *= (ratioImpurse);
						rxi = XMVector3Transform(rxi, XMLoadFloat4x4(&obj->GetRigidBody()->GetIMoment()));

						XMFLOAT3 ia;
						XMStoreFloat3(&ia, rxi);

						W = Float3Add(W, ia);
						W = Float3Float(W, obj->GetRigidBody()->GetE() / 2);
						XMFLOAT3 lastvel = obj->GetRigidBody()->GetVelocity();
						obj->GetRigidBody()->SetVelocity(Float3Float(Float3Add(lastvel, Jm), obj->GetRigidBody()->GetE()));
						obj->GetRigidBody()->SetAngularVelocity(W);

					}
					//이제 속도와 각속도는 변경 했으니, 겹쳐진 부분 해소
					//가장 작은값의 penetration(가장 깊은)만큼 올리면 된다.
					auto px = fabsf(contactpoint[0].penetration)*Normal.x;
					auto py = fabsf(contactpoint[0].penetration)*Normal.y;
					auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
					obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);


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


				//기존에는 float으로 했는데, 0이아니어야 하는데 0이나오는경우가 생김..


				auto tempdot = V1.x*V4.x + V1.y*V4.y + V1.z*V4.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//교정할 각도.  
				double theta = acos(tempdot);

				if (abs(theta) != 0 && obj->GetRigidBody()->AmendTime <= 0)
				{

					//회전축을 구하고
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V4));
					mAxis = Float3Normalize(mAxis);


					//보정을 시킨다.
					AmendObject(mAxis, theta, obj);
					obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

				}

			}
		}
		else if (size == 4)
		{
			//임시적이지만 그냥 일단 penetration과 속도만 변경하자.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);

			//땅에 닿았으니 현재 속도의 y는 반감되어야 한다. 원래는 탄성계수가 있지만.. 그냥 절반 감소시킨후 부호를 -로 하자.
			auto d = obj->GetRigidBody()->GetVelocity();
			d.y = -0.01 * d.y;
			obj->GetRigidBody()->SetVelocity(d);
			obj->GetRigidBody()->SetAccel(0, 0, 0);
		}
		else if (size != 0) //그외
		{
			//임시적이지만 그냥 일단 penetration과 속도만 변경하자.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);

			//땅에 닿았으니 현재 속도의 y는 반감되어야 한다. 원래는 탄성계수가 있지만.. 그냥 절반 감소시킨후 부호를 -로 하자.
			auto d = obj->GetRigidBody()->GetVelocity();
			d.y = -0.01 * d.y;
			obj->GetRigidBody()->SetVelocity(d);
			obj->GetRigidBody()->SetAccel(0, 0, 0);
		}

		//==================================== 충돌 후 예외의 상황을 방지하기 위한 처리들 ============================================//

		//너무 낮은위치에 있을때 속도를 0으로
		if (obj->GetCenterPos4f().y < -200)
			obj->GetRigidBody()->SetVelocity(0, 0, 0);


		// 뭔가 보정을 하거나 한다음 겹쳐진 부분을 재해결.

		XMFLOAT4 tarr[8];
		obj->GetRigidBody()->GetEightPoint(tarr, obj->GetUpVector(), obj->GetLookVector(), obj->GetRightVector());//먼저 8 개의 점을 가져온다.
		std::vector<CollisionPoint> tallpoint;
		std::vector<CollisionPoint> ttempcollisionpoint;
		std::vector<CollisionPoint> tcontactpoint;

		for (int i = 0; i < 8; i++)
		{
			float temppenetration = tarr[i].x*Normal.x + tarr[i].y*Normal.y + tarr[i].z*Normal.z;
			//충돌 점을 생성한다음 저장한다.
			CollisionPoint cp;
			cp.Pos = tarr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			tallpoint.push_back(cp);
		}

		//penetration이 작은 순으로 정렬
		sort(tallpoint.begin(), tallpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
		{
			return a.penetration < b.penetration;
		});

		//실제로 정렬된 녀석중 1차적으로 충돌이 된 녀석들을 tempcollisionpoint에 저장한다.
		for (auto i : tallpoint)
		{
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0이하면 실제로 충돌함
				ttempcollisionpoint.push_back(i);

		}

		if (ttempcollisionpoint.size() > 0)
		{
			auto px = fabsf(ttempcollisionpoint[0].penetration)*Normal.x;
			auto py = fabsf(ttempcollisionpoint[0].penetration)*Normal.y;
			auto pz = fabsf(ttempcollisionpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);

		}


		allpoint.clear();
		tempcollisionpoint.clear();
		contactpoint.clear();
	}
}

void RigidCubeObject::GravitySystem(double deltime)
{
	GeneratorGravity gg;
	gg.SetGravityAccel(XMFLOAT3(0, -100, 0));

	gg.Update(deltime, *rb);
}

void RigidCubeObject::AfterGravitySystem(double deltime)
{
	RigidBodyCollisionPlane(XMFLOAT3(0, 1, 0), 0, deltime,  this);
}

void RigidCubeObject::Tick(double deltime)
{
	if (rb != nullptr)
		rb->integrate(deltime);
}

void RigidCubeObject::Collision(unordered_set<RigidCubeObject*>* rbobjs, double deltime)
{
	for (auto iter = rbobjs->begin(); iter != rbobjs->end(); ++iter)
	{
		if ((*iter) != this)
		{
			if ((*iter)->rb != nullptr)
			{
				bool test = rb->CollisionTest(*(*iter)->rb, m_Lookvector, m_Rightvector, m_Upvector, (*iter)->m_Lookvector, (*iter)->m_Rightvector, (*iter)->m_Upvector);

				if (test)
				{
					if (rb->CollisionPointVector[0].pAxis.y > 0)
					{
						rb->SetVelocity(rb->GetVelocity().x, 0, rb->GetVelocity().z);
						m_airbone = false;
					}

					if (rb->CollisionPointVector[0].pAxis.y < 0)
					{
						(*iter)->rb->SetVelocity((*iter)->rb->GetVelocity().x, 0, (*iter)->rb->GetVelocity().z);
						(*iter)->m_airbone = false;
					}

					rb->ResolvePenetration(*(*iter)->rb, deltime);
				}
			}
			else
			{
				if ((*iter)->pp != nullptr)
				{
					RigidBody ppConvertrb;
					ppConvertrb.SetVelocity((*iter)->pp->GetVelocity());
					ppConvertrb.SetPosition(&(*iter)->GetCenterPos4f());
					ppConvertrb.SetMass((*iter)->pp->GetMass(false));
					ppConvertrb.SetHalfBox((*iter)->pp->GetHalfBox().x, (*iter)->pp->GetHalfBox().y, (*iter)->pp->GetHalfBox().z);
					ppConvertrb.SetE(1);
					ppConvertrb.SetDamping((*iter)->pp->GetDamping(), 0);
					ppConvertrb.SetBounce((*iter)->pp->GetBounce());
					ppConvertrb.SetAngularVelocity(0, 0, 0);
					ppConvertrb.SetAccel((*iter)->pp->GetAccel());


					bool test = rb->CollisionTest(ppConvertrb, m_Lookvector, m_Rightvector, m_Upvector,
						                         (*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());
					if (test)
					{
						if (rb->CollisionPointVector[0].pAxis.y < 0)
						{
							ppConvertrb.SetVelocity(ppConvertrb.GetVelocity().x, 0, ppConvertrb.GetVelocity().z);
							(*iter)->m_airbone = false;
						}

						rb->AmendTime = 0;
						rb->ResolvePenetration(ppConvertrb, deltime);
						(*iter)->pp->SetVelocity(ppConvertrb.GetVelocity());
						*(*iter)->pp->CenterPos = ppConvertrb.GetPosition();
						(*iter)->pp->SetAccel(ppConvertrb.GetAccel());
						(*iter)->UpdatePPosCenterPos();
					}
					
				}
			}
		}
	}
}

/*
void RigidCubeObject::Collision(list<CBulletObject*>* bullets, double deltime)
{
	for (auto iter = bullets->begin(); iter != bullets->end(); ++iter)
	{
		if ((*iter)->GetRigidBody() != nullptr)
		{
			bool test = rb->CollisionTest(*(*iter)->GetRigidBody(), m_Lookvector, m_Rightvector, m_Upvector, (*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				if (rb->CollisionPointVector[0].pAxis.y > 0)
				{
					rb->SetVelocity(rb->GetVelocity().x, 0, rb->GetVelocity().z);
					m_airbone = false;
				}

				if (rb->CollisionPointVector[0].pAxis.y < 0)
				{
					(*iter)->GetRigidBody()->SetVelocity((*iter)->GetRigidBody()->GetVelocity().x, 0, (*iter)->GetRigidBody()->GetVelocity().z);
					(*iter)->SetAirbone(false);
				}

				rb->ResolvePenetration(*(*iter)->GetRigidBody(), deltime);
			}
		}
		else
		{
			if ((*iter)->GetPhysicsPoint() != nullptr)
			{
				RigidBody ppConvertrb;
				ppConvertrb.SetVelocity((*iter)->GetPhysicsPoint()->GetVelocity());
				ppConvertrb.SetPosition(&(*iter)->GetCenterPos4f());
				ppConvertrb.SetMass((*iter)->GetPhysicsPoint()->GetMass(false));
				ppConvertrb.SetHalfBox((*iter)->GetPhysicsPoint()->GetHalfBox().x, (*iter)->GetPhysicsPoint()->GetHalfBox().y, (*iter)->GetPhysicsPoint()->GetHalfBox().z);
				ppConvertrb.SetE(1);
				ppConvertrb.SetDamping((*iter)->GetPhysicsPoint()->GetDamping(), 0);
				ppConvertrb.SetBounce((*iter)->GetPhysicsPoint()->GetBounce());
				ppConvertrb.SetAngularVelocity(0, 0, 0);
				ppConvertrb.SetAccel((*iter)->GetPhysicsPoint()->GetAccel());


				bool test = rb->CollisionTest(ppConvertrb, m_Lookvector, m_Rightvector, m_Upvector,
					(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

				if (test)
				{
					if (rb->CollisionPointVector[0].pAxis.y < 0)
					{
						ppConvertrb.SetVelocity(ppConvertrb.GetVelocity().x, 0, ppConvertrb.GetVelocity().z);
						(*iter)->SetAirbone(false);
					}

					(*iter)->SetAlive(false);
					//충돌 처리. 충격량을 가하고 겹침부분해소
					rb->CollisionResolve(ppConvertrb, XMFLOAT3(0, 0, 0), deltime, 6000, 1400, 1.5);

					(*iter)->GetPhysicsPoint()->SetVelocity(ppConvertrb.GetVelocity());
					*(*iter)->GetPhysicsPoint()->CenterPos = ppConvertrb.GetPosition();
					(*iter)->GetPhysicsPoint()->SetAccel(ppConvertrb.GetAccel());
					(*iter)->UpdatePPosCenterPos();

				}

			}
		}

			
	}
}
*/
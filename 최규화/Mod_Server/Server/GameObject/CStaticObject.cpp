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
		{ 76,{ { 0.f, 0.f, -100.f, 0.f },{ 0 },{ NormalBuilding } } }
	};
}

void CStaticObject::CreateGameObject()
{
	for (int i = 0; i < MAX_BOX_NUM; ++i)
		g_staticobjs.insert(new NormalBoxObject(i));
	
	auto max_fsize = MAX_BOX_NUM + MAX_SMALL_WALL_NUM;
	for (int i = MAX_BOX_NUM; i < max_fsize; ++i)
		g_staticobjs.insert(new SmallWallObject(i));
	
	auto max_ssize = max_fsize + MAX_BIG_WALL_NUM;
	for (int i = max_fsize; i < max_ssize; ++i)
		g_staticobjs.insert(new BigWallObject(i));
	
	auto max_tsize = max_ssize + MAX_BULLDING_NUM;
	for (int i = max_ssize; i < max_tsize; ++i)
		g_staticobjs.insert(new Building(i));
	
	for (auto sobj : g_staticobjs)
		sobj->AfterGravitySystem(0);
}

void CStaticObject::StaticObjectLoop()
{
	MakeStaticObjectBasicData();
	CreateGameObject();
}

void CStaticObject::AfterGravitySystem(float deltime)
{
	float ppy = pp->GetPosition().y;
	float hby = pp->GetHalfBox().y;
	if (ppy - hby < 0)
	{
		XMFLOAT3 gp = pp->GetPosition();
		gp.y += hby - ppy;
		UpdatePPosCenterPos();
		auto v = pp->GetVelocity();
		v.y = 0;
		pp->SetVelocity(v);
		m_airbone = false;
	}
}


void CStaticObject::Collision()
{
}


CStaticObject::~CStaticObject()
{
}

NormalBoxObject::NormalBoxObject(unsigned short id)
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
	m_type = STATIC_OBJECT_TYPE::Box;

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

	pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
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

SmallWallObject::SmallWallObject(unsigned short id)
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
	m_type = STATIC_OBJECT_TYPE::SmallWall;

	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.attack = 0;
	m_ability.speed = 0;
	m_ability.level = 1;
	m_ability.exp = 0;

	m_degree = m_sobj_bdata[m_id].degree;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	XMFLOAT4 xmf4_rot{ m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };

	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);

	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };
	
	UpdateLookvector();
	UpdateUpvector();

	pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
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

BigWallObject::BigWallObject(unsigned short id)
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
	m_type = STATIC_OBJECT_TYPE::BigWall;

	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.attack = 0;
	m_ability.speed = 0;
	m_ability.level = 1;
	m_ability.exp = 0;

	m_degree = m_sobj_bdata[m_id].degree;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	XMFLOAT4 xmf4_rot{ m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };

	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);

	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();
	UpdateUpvector();

	pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
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

Building::Building(unsigned short id)
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
	m_type = STATIC_OBJECT_TYPE::NormalBuilding;

	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.attack = 0;
	m_ability.speed = 0;
	m_ability.level = 1;
	m_ability.exp = 0;

	m_degree = m_sobj_bdata[m_id].degree;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	XMFLOAT4 xmf4_rot{ m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };

	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);

	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };			
	
	//스테틱오브젝트 충돌 시, 회전된 스테틱오브젝트들은 Lookvector 와 Rightvector를 회전한 값에 따라 바꿔줘야한다
	//그래야지 충돌처리할 때 Lookvector, Rightvector를 이용하는데, 바뀐 값이 적용되서 충돌처리를 한다 
	UpdateLookvector();
	UpdateUpvector();

	pp->SetPosition(m_pos4f.x, m_pos4f.y, m_pos4f.z);
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

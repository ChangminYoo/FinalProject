//#include "CStaticObject.h"
#include "../stdafx.h"

map<int, BasicInfo> CStaticObject::m_sobj_bdata;

CStaticObject::CStaticObject()
{
}

void CStaticObject::MakeStaticObjectBasicData()
{
	//����� ��ü ��ġ�� ���� ����� �ִ´�.
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

	//client ��ſ� send packet�� �� ������
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

	//client ��ſ� send packet�� �� ������
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

	//client ��ſ� send packet�� �� ������
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
	
	//����ƽ������Ʈ �浹 ��, ȸ���� ����ƽ������Ʈ���� Lookvector �� Rightvector�� ȸ���� ���� ���� �ٲ�����Ѵ�
	//�׷����� �浹ó���� �� Lookvector, Rightvector�� �̿��ϴµ�, �ٲ� ���� ����Ǽ� �浹ó���� �Ѵ� 
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

	//client ��ſ� send packet�� �� ������
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

		//���� 8 ���� ���� �����´�.
		obj->GetRigidBody()->GetEightPoint(arr, obj->GetUpVector(), obj->GetLookVector(), obj->GetRightVector());
		vector<CollisionPoint> allpoint;
		vector<CollisionPoint> tempcollisionpoint;
		vector<CollisionPoint> contactpoint;

		for (int i = 0; i < 8; ++i)
		{
			float temppenetration = arr[i].x * Normal.x + arr[i].y * Normal.y + arr[i].z * Normal.z;
			//�浹 ���� �����Ѵ��� �����Ѵ�.
			CollisionPoint cp;
			cp.Pos = arr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			allpoint.push_back(cp);
		}

		// 5) ���� ó����
		//penetration�� ���� ������ ����
		sort(allpoint.begin(), allpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
		{
			return a.penetration < b.penetration;
		});

		// 6) ���� ó����.
		//������ ���ĵ� �༮�� 1�������� �浹�� �� �༮���� tempcollisionpoint�� �����Ѵ�.
		for (auto i : allpoint)
		{
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0���ϸ� ������ �浹��
				tempcollisionpoint.push_back(i);

		}
		//���� ���� ���� �༮�� �������� �浹����Ʈ�� ���鼭 penetration�� ������ ����� ����� ������
		//�̳༮�� �о�ų� �������Ҷ� ���� �� �ִ� �浹���̴�. �� �͵��� �������� �浹���̴�.

		// 7) ���� ó����
		for (auto i : tempcollisionpoint)
		{
			//�����浹�� ù��° ���� - penetration���� ���ؼ� Epsilon�̸� ���� ���� �������� ����Ǵ� ���� �ִ´�.
			if (fabsf(tempcollisionpoint.front().penetration - i.penetration) <= 0.12)//���� 0.1������ ������ �浹 ���ɼ��� �ִٰ� �Ǵ��Ѵ�.
			{
				contactpoint.push_back(i);
			}

		}

		//------------- ���Ⱑ ���� �߿��� �б� -----------------//

		//------------- size �� 1/2/4/�׿� �϶� �� ������ ó�� ------------------//
		int size = contactpoint.size();
		if (size == 1)
		{
			//size�� 1�̸� ���ؾ��ұ�? �ٷ� ���� 2���� �ºε������ֵ��� ȸ�����Ѿ��Ѵ�.
			//�׷��� ���ؼ��� ������ ��ġ�� ��ݷ��� ���Ѵ�.
			//������ �̰� ��ȸ���� �Ͼ �� �ִ�.
			//���� ������ ȸ�������� �Ǹ� �� ������ �����Ѵ�. �ٸ� �ش� ��ݷ��� �ʹ� ũ�� �������� �����Ѵ�.

			//2������ - 1�������� �ؼ� 2�������� ���ϴ� ���͸� �����.
			//�̶� 2���� ���� ���ϴ� ������ Ư������ ���ϸ� �� ������ �����Ѵ�.

			//������ 2�������� ��������� �Ұ��ΰ� �̴�. ����Ǽ��� 2������. �����̳� �������� / �Ǵ� �밢�� ��.

			//���尡��� 2������.
			auto V1 = Float4Add(allpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			//�������� �밢���� ��ġ�� 2������
			auto sV1 = Float4Add(allpoint[3].Pos, contactpoint[0].Pos, false);
			sV1 = Float4Normalize(sV1);


			//���尡��� 2�������� ������ ���ϱ�
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
			//������ ����.  
			double theta = acos(tempdot);

			//�밢�� 2�������� ������ ���ϱ�
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
			//������ ����.  
			double stheta = acos(stempdot);

			//��ݷ��� ����. ��ݷ��� Ư�� �� �����϶��� ��������.

			CollisionPoint fp;//��ݷ��� ���� ����
			fp.Pos = XMFLOAT4(0, 0, 0, 0);
			fp.pAxis = Normal;

			fp.Pos = contactpoint[0].Pos;
			fp.penetration = contactpoint[0].penetration;
			float impurse = obj->GetRigidBody()->CalculateImpulse(fp, NULL, 1);

			//�ִ����޽��� ���Ѵ�.
			if (impurse > obj->GetRigidBody()->GetMaxImpurse())
				impurse = obj->GetRigidBody()->GetMaxImpurse();


			//�ּ� ���޽��� ���Ѵ�.
			if (impurse < obj->GetRigidBody()->GetMinImpurse())
				impurse = obj->GetRigidBody()->GetMinImpurse();




			//���� ���հ��� Ư������ ���ϸ� ������Ų��. 
			//�� �̰� double�� �ص� 0�̾ƴѵ� 0�̳����°�찡 �����.
			//���� 0�ϰ�� �׳� ��ݷ��� ���ؼ� ������ �����Ų��.

			if (abs(stheta) <= MMPE_PI / 20 && abs(stheta) != 0 && abs(impurse) < obj->GetRigidBody()->GetMaxImpurse() && obj->GetRigidBody()->AmendTime <= 0)
			{
				//ȸ������ ���ϰ�..
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(sV1, sV2));
				mAxis = Float3Normalize(mAxis);
				//������ ��Ų��.
				AmendObject(mAxis, stheta, obj);

				//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
				//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

			}
			else if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && obj->GetRigidBody()->GetMaxImpurse() && obj->GetRigidBody()->AmendTime <= 0)
			{
				//�뷫 5�� ���ϸ� ������Ŵ.

				//ȸ������ ����
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V2));
				mAxis = Float3Normalize(mAxis);

				//������ ��Ų��
				AmendObject(mAxis, theta, obj);

				//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
				//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

			}
			else //�ƴ϶�� ���� ��ݷ��� ������ ������ ���Ѵ�!
			{
				//���⿡ �Դٴ°��� ���̻� ������ ���� �ʰų� ������ ���� ���ʿ䰡 ���� ����.

				//Jm = J/M
				//���޽��� ������ ������.  ��. �Ϲ����� 1:1 ���迡�� 1.3: 0.3 ������ �д�.
				//������ 0.7�� �ҽǵ� �������� ġ��.
				if (obj->GetRigidBody()->AmendTime <= 0)
				{
					auto ratioImpurse = impurse * 0.3;

					auto Jm = Normal;
					Jm.x *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);
					Jm.y *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);
					Jm.z *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);


					//���ӵ� ���
					//W = ���� ���ӵ� + ((Q-P)Ximpurse)*InverseI
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

				//���� �ӵ��� ���ӵ��� ���� ������, ������ �κ� �ؼ�
				//���� �������� penetration(���� ����)��ŭ �ø��� �ȴ�.
				auto px = fabsf(contactpoint[0].penetration)*Normal.x;
				auto py = fabsf(contactpoint[0].penetration)*Normal.y;
				auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
				obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
		
			}
		}
		else if (size == 2)
		{
			//�߶վ��� �� 2���� ������ �ؾ��Ѵ�.
			//������ �ؾ��ϴ��� �˻縦 �ϱ� ���� ����� ��ְ� ������ �ΰ��� ���� ���Ͱ� ������ 0�� �������� �˻��Ѵ�.
			auto V1 = Float4Add(contactpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			float Dot = V1.x * Normal.x + V1.y * Normal.y + V1.z * Normal.z;

			if (fabsf(Dot) <= MMPE_EPSILON)
				Dot = 0;

			//����� 0�̸� ������ �ʿ����.
			if (Dot == 0)
			{
				//���� 2���� �浹�� 4���� ���浹�� �ٲ� �� �ִ� ������ �� �� �ִ��� �˻��Ѵ�.
				//�׷��� ���ؼ� 3��° ��- ù��° �� �� �ؼ� 3��° ���� ���ϴ� ���͸� �����.
				//�� �� �̰� V1�� ������ 0 �� ������ ù��° ���� ����° ���� �밢���� �ƴ϶����̰�,
				//�ƴ϶�� �ι�° ���� ����°���� �̿��� ���͸� �������Ѵ�.
				auto V2 = Float4Add(allpoint[2].Pos, contactpoint[0].Pos, false);
				V2 = Float4Normalize(V2);
				float Dot2 = V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;

				if (fabsf(Dot2) <= MMPE_EPSILON)
					Dot2 = 0;

				//ù��° ���� ����°���� ���� �ʴ°�� �ι�° ���� ����°������ �����.
				if (Dot2 != 0)
				{

					V2 = Float4Add(allpoint[2].Pos, contactpoint[1].Pos, false);
					V2 = Float4Normalize(V2);
				}
				//���� ���� �Դٴ°���, 4���� ���� ���� �� �ִ� �༮���� ã�� ���̴�.
				//�ٸ� �ٷ� �����ϸ� �ȵǰ�, �̰� ������� ���� ���� �� ���� �ȴ�.
				//�׷� �� ������ �����ΰ�??
				/*
				��  1)(V2)
				�� /
				��/
				�ѤѤ�2)(V3)

				���� V2�� �ٷ� 1) �̴�.
				�̰� ����� ��ְ� ������ 2) �� ���ͷ� ����ǵ�
				1)�� 2) ������ ��ٰ� ������ ���� �� 5~10 �� ���� �Ǹ� �׸�ŭ ȸ�� ��Ű�� ���̴�!
				�׷��� ���ؼ� 2)�� ���ؾ��Ѵ�. 2)�� ����� ����� A ���� V2�� B�� �ϰ� ProjAB �Ѱ���� V2�� ������ V2�� ���ȭ �ϸ� �ȴ�.
				*/

				float NdotV2 = Normal.x*V2.x + Normal.y*V2.y + Normal.z*V2.z;
				XMFLOAT3 ProjAB = Normal;
				ProjAB.x *= NdotV2;
				ProjAB.y *= NdotV2;
				ProjAB.z *= NdotV2;

				auto V3 = Float4Add(V2, XMFloat3to4(ProjAB), false);
				V3 = Float4Normalize(V3);
				//���� V2�� V3�� �ƴϱ� V2�� V3�� ���հ��� ���Ѵ��� V2���� V3�� �����ؼ� Axis�� ���Ѵ�.

				//���� ���հ����� ���Ѵ�.

				//�������� float���� �ߴµ�, 0�̾ƴϾ�� �ϴµ� 0�̳����°�찡 ����..


				auto tempdot = V2.x*V3.x + V2.y*V3.y + V2.z*V3.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//������ ����.  
				double theta = acos(tempdot);


				CollisionPoint fp;//��ݷ��� ���� ����
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

				//�ִ����޽��� ���Ѵ�.
				if (impurse > obj->GetRigidBody()->GetMaxImpurse())
					impurse = obj->GetRigidBody()->GetMaxImpurse();


				//�ּ� ���޽��� ���Ѵ�.
				if (impurse < obj->GetRigidBody()->GetMinImpurse())
					impurse = obj->GetRigidBody()->GetMinImpurse();


				//���� ���հ��� Ư������ ���ϸ� ������Ų��. 
				if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && abs(impurse) < obj->GetRigidBody()->GetMaxImpurse() && obj->GetRigidBody()->AmendTime <= 0)//�뷫 5�� ���ϸ� ������Ŵ.
				{
					//ȸ������ ���ϰ�
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
					mAxis = Float3Normalize(mAxis);
					//������ ��Ų��.
					AmendObject(mAxis, theta, obj);

					//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
					//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
					obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);
					obj->GetRigidBody()->SetAccel(0, 0, 0);


				}
				else//�ƴ϶�� ���� ��ݷ��� ������ ������ ���Ѵ�!
				{
					//���⿡ �Դٴ°��� ���̻� ������ ���� �ʰų� ������ ���� ���ʿ䰡 ���� ����.

					//���� ���� �浹���� 2���̴� 2���� �浹���� ���� �� 2�� �����ָ� �浹���� �ϼ��̴�.
					//��ݷ��� ���Ŀ� �ٽ� ���ؾ߰�����, ����� 0,50,0 ������ ���ϰڴ�.

					//�ٵ� ���⼭ ����� ���� �����. �˴ٽ��� �밢�����·� ��Ȯ�ϰ� 45�� �������� ���ִ� ����Ǽ��� �����.
					//�̰��� ���ַ��� ���� �ش����� ���� ���ϴ����� �˻��ؾ��Ѵ�.

					//���� �� ����Ǽ��� �ذ��ؾ߰ڴ�. �˻縦 �Ϸ��� �¿� ��Ī������ ����ɰͰ���.
					//�׸��� ��Ī�̸� �����̵� �ƴϸ� ���������� ��ݷ��� �ణ ���ϴ� �ϸ� �ɰͰ���.

					//���� 2/3/4/5 �ε����� ���� ���ϸ鼭 �� ���̰� 0.0001���� ���̸� ���.
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

					//4���� ��� ���� ���̸� ������ �̷�°��̹Ƿ� ������ �μŹ�����.
					if (pass)
					{
						theta = -MMPE_PI / 18;
						XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
						mAxis = Float3Normalize(mAxis);
						//������ ��Ų��.
						AmendObject(mAxis, theta, obj);

						//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
						//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
						obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);


						return;
					}

					//Jm = J/M

					//���޽��� ������ ������.  ��. �Ϲ����� 1:1 ���迡�� 1.3: 0.3 ������ �д�.

					if (obj->GetRigidBody()->AmendTime <= 0)
					{
						auto ratioImpurse = impurse * 0.3;

						auto Jm = Normal;



						Jm.x *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);
						Jm.y *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);
						Jm.z *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);



						//���ӵ� ���
						//W = ���� ���ӵ� + ((Q-P)Ximpurse)*InverseI
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
					//���� �ӵ��� ���ӵ��� ���� ������, ������ �κ� �ؼ�
					//���� �������� penetration(���� ����)��ŭ �ø��� �ȴ�.
					auto px = fabsf(contactpoint[0].penetration)*Normal.x;
					auto py = fabsf(contactpoint[0].penetration)*Normal.y;
					auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
					obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);


				}

			}
			else//Dot�� ��� �߶վ��� �� �ϰ�� �� ��ְ� (�ι�°��-ù��°��) ���Ͱ� ���°�� 
			{
				XMFLOAT3 ProjAB = Normal;
				ProjAB.x *= Dot;
				ProjAB.y *= Dot;
				ProjAB.z *= Dot;

				//�����Ǿ����� V1
				auto V4 = Float4Add(V1, XMFloat3to4(ProjAB), false);
				V4 = Float4Normalize(V4);

				//������ ����.


				//�������� float���� �ߴµ�, 0�̾ƴϾ�� �ϴµ� 0�̳����°�찡 ����..


				auto tempdot = V1.x*V4.x + V1.y*V4.y + V1.z*V4.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//������ ����.  
				double theta = acos(tempdot);

				if (abs(theta) != 0 && obj->GetRigidBody()->AmendTime <= 0)
				{

					//ȸ������ ���ϰ�
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V4));
					mAxis = Float3Normalize(mAxis);


					//������ ��Ų��.
					AmendObject(mAxis, theta, obj);
					obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

				}

			}
		}
		else if (size == 4)
		{
			//�ӽ��������� �׳� �ϴ� penetration�� �ӵ��� ��������.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);

			//���� ������� ���� �ӵ��� y�� �ݰ��Ǿ�� �Ѵ�. ������ ź������� ������.. �׳� ���� ���ҽ�Ų�� ��ȣ�� -�� ����.
			auto d = obj->GetRigidBody()->GetVelocity();
			d.y = -0.01 * d.y;
			obj->GetRigidBody()->SetVelocity(d);
			obj->GetRigidBody()->SetAccel(0, 0, 0);
		}
		else if (size != 0) //�׿�
		{
			//�ӽ��������� �׳� �ϴ� penetration�� �ӵ��� ��������.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);

			//���� ������� ���� �ӵ��� y�� �ݰ��Ǿ�� �Ѵ�. ������ ź������� ������.. �׳� ���� ���ҽ�Ų�� ��ȣ�� -�� ����.
			auto d = obj->GetRigidBody()->GetVelocity();
			d.y = -0.01 * d.y;
			obj->GetRigidBody()->SetVelocity(d);
			obj->GetRigidBody()->SetAccel(0, 0, 0);
		}

		//==================================== �浹 �� ������ ��Ȳ�� �����ϱ� ���� ó���� ============================================//

		//�ʹ� ������ġ�� ������ �ӵ��� 0����
		if (obj->GetCenterPos4f().y < -200)
			obj->GetRigidBody()->SetVelocity(0, 0, 0);


		// ���� ������ �ϰų� �Ѵ��� ������ �κ��� ���ذ�.

		XMFLOAT4 tarr[8];
		obj->GetRigidBody()->GetEightPoint(tarr, obj->GetUpVector(), obj->GetLookVector(), obj->GetRightVector());//���� 8 ���� ���� �����´�.
		std::vector<CollisionPoint> tallpoint;
		std::vector<CollisionPoint> ttempcollisionpoint;
		std::vector<CollisionPoint> tcontactpoint;

		for (int i = 0; i < 8; i++)
		{
			float temppenetration = tarr[i].x*Normal.x + tarr[i].y*Normal.y + tarr[i].z*Normal.z;
			//�浹 ���� �����Ѵ��� �����Ѵ�.
			CollisionPoint cp;
			cp.Pos = tarr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			tallpoint.push_back(cp);
		}

		//penetration�� ���� ������ ����
		sort(tallpoint.begin(), tallpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
		{
			return a.penetration < b.penetration;
		});

		//������ ���ĵ� �༮�� 1�������� �浹�� �� �༮���� tempcollisionpoint�� �����Ѵ�.
		for (auto i : tallpoint)
		{
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0���ϸ� ������ �浹��
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
					//�浹 ó��. ��ݷ��� ���ϰ� ��ħ�κ��ؼ�
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
#include "StaticObject.h"


void StaticObject::SET_PosOfBox()
{
	m_boxPos = 
	{
		{0, {-20.f, 0.f, 0.f, 0.f}},
		{1, {-40.f, 10.f, 0.f, 0.f}},
		{2, {50.f, 0.f, -40.f, 0.f}},
		{3, {30.f, 0.f, 40.f, 0.f}}
	};
}

void StaticObject::InitBoxObjects()
{
	//1. StaticObjectList에 데이터 넣어주기.
	//   한번 생성하고 바뀌지 않으므로 초기 한번만 실행
	SET_PosOfBox();

	for (int i = 0; i < MAX_BOX_NUM; ++i)
	{
		StaticObject *sobj = new StaticObject();

		sobj->sobj_data.Ani = Ani_State::Idle;
		sobj->sobj_data.Connect_Status = true;
		sobj->sobj_data.Dir = 0;
		sobj->sobj_data.GodMode = true;
		sobj->sobj_data.ID = i;
		sobj->sobj_data.Is_AI = false;
		sobj->sobj_data.Pos = m_boxPos[i];
		sobj->sobj_data.Rotate_status = { 0.0f, 0.0f, 0.0f, 0.0f };
		sobj->sobj_data.UserInfo.cur_hp = 100;
		sobj->sobj_data.UserInfo.origin_hp = 100;
		sobj->sobj_data.UserInfo.player_status.attack = 0;
		sobj->sobj_data.UserInfo.player_status.speed = 0;

		sobj->OffLookvector = XMFLOAT3(0, 0, 1);
		sobj->OffRightvector = XMFLOAT3(1, 0, 0);

		sobj->UpdateLookVector();
		sobj->GetUpVector();

		m_staticObjs.emplace_back(sobj);
	}
	

	//sobj->m_connect_state = true;
	//sobj->m_state = PLAYER_STATE::IDLE;
	//sobj->m_playerData.Ani = Ani_State::Idle;
	//sobj->m_playerData.Connect_Status = true;

	//sobj_data.Connect_Status = true;
	//sobj->m_playerData.Dir = 0;
	//sobj->m_playerData.GodMode = true;
	//sobj->m_playerData.Is_AI = false;
	//sobj->m_playerData.Rotate_status = { 0.0f, 0.0f, 0.0f, 0.0f };
	//sobj->m_playerData.UserInfo.cur_hp = 100;
	//sobj->m_playerData.UserInfo.origin_hp = 100;
	//sobj->m_playerData.UserInfo.player_status.attack = 0;
	//sobj->m_playerData.UserInfo.player_status.speed = 0;
	//sobj->m_playerType = PLAYERS::NO_PLAYER;
	//sobj->m_monsterType = MONSTERS::NO_MONSTER;
	//sobj->staticobject = true;
	//sobj->OffLookvector = XMFLOAT3(0, 0, 1);
	//sobj->OffRightvector = XMFLOAT3(1, 0, 0);
	

	//for (int i = 0; i < MAX_BOX_NUM; ++i)
	//{
	//	sobj->m_id = i;
	//	sobj->m_playerData.ID = i;
	//	sobj->m_playerData.Pos = move(m_boxPos[i]);
	//
	//	m_staticobjs.emplace_back(sobj);
	//}
	
}

void StaticObject::UpdateLookVector()
{
	auto wmatrix = XMMatrixIdentity();

	//클라이언트에서 MouseMove를 통해 카메라를 회전할 때 마다 Rotate_status가 달라짐
	XMFLOAT4 orient_xmfloat4 =
	{	sobj_data.Rotate_status.x ,
		sobj_data.Rotate_status.y ,
		sobj_data.Rotate_status.z ,
		sobj_data.Rotate_status.w };

	auto quater = XMLoadFloat4(&orient_xmfloat4);
	wmatrix *= XMMatrixRotationQuaternion(quater);

	//OffLookvector 와 OffRightvector는 플레이어타입(캐릭터, 불렛, 스테틱오브젝트 등에 따라 다름)
	auto ol = XMLoadFloat3(&OffLookvector);
	auto or = XMLoadFloat3(&OffRightvector);

	ol = XMVector4Transform(ol, wmatrix);
	or = XMVector4Transform(or , wmatrix);

	XMStoreFloat3(&Lookvector, ol);
	XMStoreFloat3(&Rightvector, or );

	if (fabsf(Lookvector.x) < MMPE_EPSILON / 10)
		Lookvector.x = 0;
	if (fabsf(Lookvector.y) < MMPE_EPSILON / 10)
		Lookvector.y = 0;
	if (fabsf(Lookvector.z) < MMPE_EPSILON / 10)
		Lookvector.z = 0;


	if (fabsf(Rightvector.x) < MMPE_EPSILON / 10)
		Rightvector.x = 0;
	if (fabsf(Rightvector.y) < MMPE_EPSILON / 10)
		Rightvector.y = 0;
	if (fabsf(Rightvector.z) < MMPE_EPSILON / 10)
		Rightvector.z = 0;

	Lookvector = Float3Normalize(Lookvector);
	Rightvector = Float3Normalize(Rightvector);

}

void StaticObject::GetUpVector()
{
	XMVECTOR l = XMLoadFloat3(&Lookvector);
	XMVECTOR r = XMLoadFloat3(&Rightvector);
	auto u = XMVector3Cross(l, r);

	XMFLOAT3 up;
	XMStoreFloat3(&up, u);
	up = Float3Normalize(up);

	Upvector = move(up);
}


StaticObject::~StaticObject()
{
	for (auto obj : m_staticObjs)
		delete obj;

}


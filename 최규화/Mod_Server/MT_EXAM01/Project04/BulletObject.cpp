#include "BulletObject.h"



BulletObject::BulletObject(const unsigned short& master_id, const unsigned short& target_id,
						   const Position& pos, const Rotation& rot, float bulltime, const unsigned short& my_id)
{
	OffLookvector = XMFLOAT3(0, 0, 1);
	OffRightvector = XMFLOAT3(1, 0, 0);

	m_bulldata.Rotate_status = move(rot);

	UpdateLookVector();
	GetUpVector();

	lifetime = bulltime;

	m_bulldata.pos = move(pos);
	
	m_bulldata.Master_ID = master_id;
	m_bulldata.LookOn_ID = target_id;
	
	myID = my_id;

}

void BulletObject::UpdateLookVector()
{
	auto wmatrix = XMMatrixIdentity();

	//Ŭ���̾�Ʈ���� MouseMove�� ���� ī�޶� ȸ���� �� ���� Rotate_status�� �޶���
	XMFLOAT4 orient_xmfloat4 =
	{	m_bulldata.Rotate_status.x ,
		m_bulldata.Rotate_status.y ,
		m_bulldata.Rotate_status.z ,
		m_bulldata.Rotate_status.w };

	auto quater = XMLoadFloat4(&orient_xmfloat4);
	wmatrix *= XMMatrixRotationQuaternion(quater);

	//OffLookvector �� OffRightvector�� �÷��̾�Ÿ��(ĳ����, �ҷ�, ����ƽ������Ʈ � ���� �ٸ�)
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

void BulletObject::GetUpVector()
{
	XMVECTOR l = XMLoadFloat3(&Lookvector);
	XMVECTOR r = XMLoadFloat3(&Rightvector);
	auto u = XMVector3Cross(l, r);

	XMFLOAT3 up;
	XMStoreFloat3(&up, u);
	up = Float3Normalize(up);

	Upvector = move(up);
}


void BulletObject::Update(float deltatime)
{
	
}

BulletObject::~BulletObject()
{
}

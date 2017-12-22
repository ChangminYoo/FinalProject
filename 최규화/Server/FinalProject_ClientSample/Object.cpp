#include "Object.h"

Object::Object() {};

Object::~Object() {};


Object::Object(int x, int y, int dirX, int dirY, int speed, int sizeX, int sizeY, int bitX, int bitY, int resX, int bitNum, int aniSize, HINSTANCE hInst) :
	m_positionX(x), m_positionY(y), // ��ǥ �ʱ�ȭ
	m_directionX(dirX), m_directionY(dirY), // �̵� ���� �ʱ�ȭ
	m_speed(speed), // �ӵ� �ʱ�ȭ
	m_sizeofbit(make_pair(sizeX, sizeY)), m_resourceSize(make_pair(bitX, bitY)), m_resourcePosX(resX), m_animationSize(aniSize), // ������ �ʱ�ȭ
	m_isJump(false), m_isAlive(true) // ��Ÿ ��� �ʱ�ȭ
{
	m_bitmapNum = bitNum;
	m_hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(m_bitmapNum));
};


//====������====
void Object::Draw(HDC hdc, HDC memdc)
{
	if (m_isAlive)
	{
		SelectObject(memdc, m_hBitmap);
		TransparentBlt(hdc, m_positionX, m_positionY, m_sizeofbit.first, m_sizeofbit.second, memdc, m_resourcePosX*m_animationCount, 2, m_resourceSize.first, m_resourceSize.second, RGB(255, 122, 255));
	}
};




//====��ü ������====
void Object::Move()
{
	// ��ġ���� �����ֹǷ� ��ü ������ �־��ִ� ���ڰ��� �� �����Ұ�.
	// �Ǵ� set�Լ��� �����Ұ�.
	m_positionX += m_directionX * CHAR_SPEED;
	m_positionY += m_directionY * CHAR_SPEED;

	// ȭ������� ����������.
	//if (m_positionX > 640 || m_positionX < 50)
		//m_positionX -= m_directionX * m_speed;
		
	//if (m_positionY > 440 || m_positionY < 0)
		//m_positionY -= m_directionY * m_speed;
}
void Object::Jump() {

	if (GetJumpCoolTime() < JUMP_COOLTIME / 2.0f)
	{
		SetBitSize
		(
			GetBitSize().first + 1,
			GetBitSize().second + 1
		);
	}
	else if (GetJumpCoolTime() < JUMP_COOLTIME)
	{
		SetBitSize
		(
			GetBitSize().first - 1,
			GetBitSize().second - 1
		);
	}
	else
	{
		SetBitSize(70, 70);
		SetIsJump(false);
		SetJumpCoolTime(0.0f);
	}

};

// ����
void Object::Jump(float elapsedTime) {

	PlusJumpCoolTime(elapsedTime);

	if (GetJumpCoolTime() < JUMP_COOLTIME / 2.0f)
	{
		SetBitSize
		(
			GetBitSize().first + 1,
			GetBitSize().second + 1
		);
	}
	
	else if (GetJumpCoolTime() < JUMP_COOLTIME)
	{
		SetBitSize
		(
			GetBitSize().first - 1,
			GetBitSize().second - 1
		);
	}
	else
	{
		SetBitSize(70, 70);
		SetJumpCoolTime(0.0f);
		SetIsJump(false);
	}

};



//====�浹 üũ====
bool Object::IsCollide(Object* other) {
	// this ������ other ����
	bool col1 = (m_positionX + m_sizeofbit.first / 2 > other->GetPositionX());
	// this ������ other ����
	bool col2 = (m_positionX  < other->GetPositionX() + other->GetBitSize().first / 2);
	// this �ϴܰ� other ���
	bool col3 = (m_positionY + m_sizeofbit.second > other->GetPositionY());
	// this ��ܰ� other �ϴ�
	bool col4 = (m_positionY < other->GetPositionY() + other->GetBitSize().second / 2);

	bool col5 = m_isAlive && other->GetIsAlive() && !m_isJump;

	if ((col1 && col2) && (col3 && col4) && col5)
	{
		return true;
	}
		
	else
		return false;
};
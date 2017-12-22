#pragma once
#include "stdafx.h"


class Object
{
private:
	HBITMAP m_hBitmap;
	pair<int, int> m_sizeofbit; //�󸶸�ŭ ũ�⸦ �ٱ�?
	pair<int, int> m_resourceSize; //���ҽ� ��� �ۿñ�?
	int m_resourcePosX;
	int m_bitmapNum;
	int m_animationSize;
	int m_animationCount = 0;

	int m_positionX, m_positionY; // ��ü ��ǥ
	int m_directionX, m_directionY; // ��ü �̵� ����
	int m_speed;

	int m_health = 20; // ��ü ü��
	bool m_isAlive; // ��ü ���� ����
	
	bool m_isJump; // ��ü ���� ����
	float m_JumpCoolTime = 0.0f;
	int m_JumpAnimateCount = 0;
	
	bool m_isStuck = false; // �÷��̾ ���� �������..
	float m_StuckCoolTime = 0.0f;

	bool m_isDamaged = false;
	float m_DamageCoolTime = 0.0f;
	long m_CharId{ -1 }; //��ȭ �߰�
	bool m_SelectSceneReady{ false }; //��ȭ �߰� 
public:
	Object();
	~Object();
	Object(int posX, int posY, int dirX, int dirY, int speed, int sizeX, int sizeY, int bitX, int bitY, int resX, int bitNum, int aniSize, HINSTANCE hInst);

	// ��Ʈ(���� �� ���), ���ҽ�(����) ������ ��ȯ
	pair<int, int> GetBitSize() const { return m_sizeofbit; };
	void SetBitSize(int x, int y) { m_sizeofbit.first = x; m_sizeofbit.second = y; };
	pair<int, int> GetResourceSize() const { return m_resourceSize; };


	// ��ü "��ǥ" ��ȯ �� ����
	int GetPositionX() const { return m_positionX; };
	int GetPositionY() const { return m_positionY; };
	void SetPositionX(int x) { m_positionX = x; };
	void SetPositionY(int y) { m_positionY = y; };

	// ��ü "�̵� ����" ��ȯ �� ����
	int GetDirectionX() const { return m_directionX; };
	int GetDirectionY() const { return m_directionY; };
	void SetDirectionX(int x) { m_directionX = x; };
	void SetDirectionY(int y) { m_directionY = y; };

	// ��ü "�ӵ�" ��ȯ �� ����
	int GetSpeed() const { return m_speed; };
	void SetSpeed(int s) { m_speed = s; };

	// ��ü "ü��" ��ȯ �� ����
	int GetHealth() const { return m_health; };
	void SetHealth(int h) { m_health = h; };
	void Damaged(int d) { m_health -= d; m_isDamaged = true; };
	

	bool GetIsDamaged() const { return m_isDamaged; };
	void SetIsDamaged(bool b) { m_isDamaged = b; };
	float GetDamageCoolTime() const { return m_DamageCoolTime; };
	void SetDamageCoolTime(float elapsedTime) { m_DamageCoolTime = elapsedTime; };
	void PlusDamageCoolTime(float elapsedTime) { m_DamageCoolTime += elapsedTime; };


	// ��ü "���� ����" ��ȯ �� ����
	bool GetIsAlive() const { return m_isAlive; };
	void SetIsAlive(bool a) { m_isAlive = a; };

	// ��ü "���� ����" ��ȯ �� ����
	bool GetIsJump() const { return m_isJump; };
	void SetIsJump(bool j) { m_isJump = j; };
	
	float GetJumpCoolTime() const { return m_JumpCoolTime; };
	void SetJumpCoolTime(float ct) { m_JumpCoolTime = ct; };
	void PlusJumpCoolTime(float ct) { m_JumpCoolTime += ct; };


	// ��ü "���� �ִϸ���Ʈ Ƚ��" ��ȯ
	int GetJumpAnimateCount() const { return m_JumpAnimateCount; };


	// Bitmap
	void SetBitmapNum(int bmn) { m_bitmapNum = bmn; };
	void SetBitmap(HBITMAP bm) { m_hBitmap = bm; };

	//��ȭ �߰�
	void SetCharID(long id) { m_CharId = id; }
	long GetCharID() { return m_CharId; }
	void SetSelectSceneReady(bool flag) { m_SelectSceneReady = flag; }
	bool GetSelectSceneReady() { return m_SelectSceneReady; }
	// stuck
	bool GetIsStuck() const { return m_isStuck; };
	void SetIsStuck(bool s) { m_isStuck = s; };

	float GetStuckCoolTime() const { return m_StuckCoolTime; };
	void SetStuckCoolTime(float ct) { m_StuckCoolTime = ct; };
	void PlusStuckCoolTime(float ct) { m_StuckCoolTime += ct; };


	//=========================================


	// ��ü "Move"
	void Move();

	// ��ü "Draw"
	void Draw(HDC hdc, HDC memdc);
	void Jump();
	// ��ü "����"
	void Jump(float elapsedTime);

	//====�ִϸ��̼�====
	void Animate() { m_animationCount = (m_animationCount + 1) % m_animationSize; };

	// ���ڷ� ���� �ٸ� ��ü�� �浹 üũ
	bool IsCollide(Object* other); // ��������??

};

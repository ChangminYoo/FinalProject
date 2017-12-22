#pragma once
#include "stdafx.h"


class Object
{
private:
	HBITMAP m_hBitmap;
	pair<int, int> m_sizeofbit; //얼마만큼 크기를 줄까?
	pair<int, int> m_resourceSize; //리소스 어디를 퍼올까?
	int m_resourcePosX;
	int m_bitmapNum;
	int m_animationSize;
	int m_animationCount = 0;

	int m_positionX, m_positionY; // 객체 좌표
	int m_directionX, m_directionY; // 객체 이동 방향
	int m_speed;

	int m_health = 20; // 객체 체력
	bool m_isAlive; // 객체 생존 여부
	
	bool m_isJump; // 객체 점프 여부
	float m_JumpCoolTime = 0.0f;
	int m_JumpAnimateCount = 0;
	
	bool m_isStuck = false; // 플레이어가 덫을 밟을경우..
	float m_StuckCoolTime = 0.0f;

	bool m_isDamaged = false;
	float m_DamageCoolTime = 0.0f;
	long m_CharId{ -1 }; //규화 추가
	bool m_SelectSceneReady{ false }; //규화 추가 
public:
	Object();
	~Object();
	Object(int posX, int posY, int dirX, int dirY, int speed, int sizeX, int sizeY, int bitX, int bitY, int resX, int bitNum, int aniSize, HINSTANCE hInst);

	// 비트(게임 상 출력), 리소스(원본) 사이즈 반환
	pair<int, int> GetBitSize() const { return m_sizeofbit; };
	void SetBitSize(int x, int y) { m_sizeofbit.first = x; m_sizeofbit.second = y; };
	pair<int, int> GetResourceSize() const { return m_resourceSize; };


	// 객체 "좌표" 반환 및 설정
	int GetPositionX() const { return m_positionX; };
	int GetPositionY() const { return m_positionY; };
	void SetPositionX(int x) { m_positionX = x; };
	void SetPositionY(int y) { m_positionY = y; };

	// 객체 "이동 방향" 반환 및 설정
	int GetDirectionX() const { return m_directionX; };
	int GetDirectionY() const { return m_directionY; };
	void SetDirectionX(int x) { m_directionX = x; };
	void SetDirectionY(int y) { m_directionY = y; };

	// 객체 "속도" 반환 및 설정
	int GetSpeed() const { return m_speed; };
	void SetSpeed(int s) { m_speed = s; };

	// 객체 "체력" 반환 및 설정
	int GetHealth() const { return m_health; };
	void SetHealth(int h) { m_health = h; };
	void Damaged(int d) { m_health -= d; m_isDamaged = true; };
	

	bool GetIsDamaged() const { return m_isDamaged; };
	void SetIsDamaged(bool b) { m_isDamaged = b; };
	float GetDamageCoolTime() const { return m_DamageCoolTime; };
	void SetDamageCoolTime(float elapsedTime) { m_DamageCoolTime = elapsedTime; };
	void PlusDamageCoolTime(float elapsedTime) { m_DamageCoolTime += elapsedTime; };


	// 객체 "생존 여부" 반환 및 설정
	bool GetIsAlive() const { return m_isAlive; };
	void SetIsAlive(bool a) { m_isAlive = a; };

	// 객체 "점프 여부" 반환 및 설정
	bool GetIsJump() const { return m_isJump; };
	void SetIsJump(bool j) { m_isJump = j; };
	
	float GetJumpCoolTime() const { return m_JumpCoolTime; };
	void SetJumpCoolTime(float ct) { m_JumpCoolTime = ct; };
	void PlusJumpCoolTime(float ct) { m_JumpCoolTime += ct; };


	// 객체 "점프 애니메이트 횟수" 반환
	int GetJumpAnimateCount() const { return m_JumpAnimateCount; };


	// Bitmap
	void SetBitmapNum(int bmn) { m_bitmapNum = bmn; };
	void SetBitmap(HBITMAP bm) { m_hBitmap = bm; };

	//규화 추가
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


	// 객체 "Move"
	void Move();

	// 객체 "Draw"
	void Draw(HDC hdc, HDC memdc);
	void Jump();
	// 객체 "점프"
	void Jump(float elapsedTime);

	//====애니메이션====
	void Animate() { m_animationCount = (m_animationCount + 1) % m_animationSize; };

	// 인자로 받은 다른 객체와 충돌 체크
	bool IsCollide(Object* other); // 서버에서??

};

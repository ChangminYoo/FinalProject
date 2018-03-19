#pragma once
#include"d3dUtil.h"
using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

class CVertex
{
protected:

public:
	XMFLOAT3 V;
	XMFLOAT4 N;//노멀벡터
	XMFLOAT2 Tex;//텍스처좌표


	int StartWeight;//시작 웨이트
	int WeightCount;//웨이트 갯수

	float JointID[4];//조인트ID들
	XMFLOAT4 WeightPos1;//마지막은 바이어스
	XMFLOAT4 WeightPos2;//마지막은 바이어스
	XMFLOAT4 WeightPos3;//마지막은 바이어스
	XMFLOAT4 WeightPos4;//마지막은 바이어스


	CVertex(void);
	CVertex(XMFLOAT3 v)
	{
		
		V = v;
		N = XMFLOAT4(0, 0, 0, 1);
		
	}
	~CVertex(void);

	XMFLOAT3 operator-(CVertex& ref)
	{
		XMFLOAT3 t;
		XMVECTOR s=XMLoadFloat3(&V);
		XMVECTOR s2 = XMLoadFloat3(&ref.V);
		XMStoreFloat3(&t, s - s2);
		return t;
	}
};


class BillVertex
{
public:
	XMFLOAT3 V;
	XMFLOAT2 Size;

	BillVertex(void);
	BillVertex(XMFLOAT3 v)
	{

		V = v;
		Size = XMFLOAT2(0, 0);

	}

	~BillVertex(void);

};
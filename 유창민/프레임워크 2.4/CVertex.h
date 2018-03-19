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
	XMFLOAT4 N;//��ֺ���
	XMFLOAT2 Tex;//�ؽ�ó��ǥ


	int StartWeight;//���� ����Ʈ
	int WeightCount;//����Ʈ ����

	float JointID[4];//����ƮID��
	XMFLOAT4 WeightPos1;//�������� ���̾
	XMFLOAT4 WeightPos2;//�������� ���̾
	XMFLOAT4 WeightPos3;//�������� ���̾
	XMFLOAT4 WeightPos4;//�������� ���̾


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
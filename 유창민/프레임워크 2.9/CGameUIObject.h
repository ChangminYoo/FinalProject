#pragma once
#include"CGameObject.h"


// ============================= UI==================================//
//에임, 스킬, 쿨바
//HP는 GameOBject에

class AimObject : public CGameObject
{
public:
	AimObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));


public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성

	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};

class SkillUIObject : public CGameObject
{
public:
	SkillUIObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, int SkillNum, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));


public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성

	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};

class SelectBarObject : public CGameObject
{
public:
	SelectBarObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙

public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};

class BackGroundSkillObject : public CGameObject
{
public:
	BackGroundSkillObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));


public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙


public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}

};


class CoolBarObject : public CGameObject
{
public:
	CoolBarObject(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist, list<CGameObject*>*Plist, float maxtime, CGameObject* master, XMFLOAT4 cp = XMFLOAT4(0, 0, 0, 0));
	CGameObject* Master = NULL;//소유자
public:
	static bool CreateMesh;//최초로 false며 메쉬를 만든후 true가된다.
	static unordered_map<string, unique_ptr<CTexture>> Textures;//텍스처들을 저장함
	static CMesh Mesh;//오로지 한번만 만들어짐
	static ComPtr<ID3D12DescriptorHeap> SrvDescriptorHeap;//텍스처 용 힙

	float MaxCoolTime = 0;
public:
	virtual void SetMesh(ID3D12Device* m_Device, ID3D12GraphicsCommandList* commandlist);//셋메시는 메시를 최종적으로 생성한다. 즉 메시를구성하는 정점과 삼각형을구성하는인덱스버퍼생성
	virtual void Render(ID3D12GraphicsCommandList* commandlist, const GameTimer& gt);
	virtual void Collision(list<CGameObject*>* collist, float DeltaTime) {}


};


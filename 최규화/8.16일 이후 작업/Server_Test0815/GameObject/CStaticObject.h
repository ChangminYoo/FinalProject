#pragma once
#include <map>
#include <unordered_set>

class CPlayerObject;
class CBulletObject;


//CMonoObject: �⺻ ������Ʈ 
//CStaticObject: �������� �ʴ� ��ġ�� ������Ʈ
//CMoveObject: �����̴� ������Ʈ
//CRigidbody: ����ȿ���� ����� ������Ʈ

//--------------StaticObject (�������� �ʴ� ����ȿ���� ������� ���� ��ü--------------//

class CStaticObject : public CMonoObject
{
protected:
	StaticObject_Info m_stc_sobjdata;
	int				  m_textureNumber{ 0 };
public:
	CStaticObject();
	virtual ~CStaticObject();

	void MakeStaticObjectBasicData();
	void CreateGameObject();
	void StaticObjectLoop();

public:
	//static object�� gravitysystem x // tick x
	//static object�� aftergravitysystem�� ����

	virtual void UpdateDataForPacket() override;
	StaticObject_Info GetStaticObjectInfo() const { return m_stc_sobjdata; }
};

class CNormalBoxObject : public CStaticObject
{
public:
	CNormalBoxObject(unsigned int id);
};

class CSmallWallObject : public CStaticObject
{
public:
	CSmallWallObject(unsigned int id);
};

class CBigWallObject : public CStaticObject
{
public:
	CBigWallObject(unsigned int id);
};

class CBuildingObject : public CStaticObject
{
public:
	CBuildingObject(unsigned int id);
};

class CSecondFloorObject : public CStaticObject
{
public:
	CSecondFloorObject(unsigned int id);
};

class CColumnObject : public CStaticObject
{
public:
	CColumnObject(unsigned int id);
};

class CBrokenCartObject : public CStaticObject
{
public:
	CBrokenCartObject(unsigned int id);
};

//--------------MoveCubeObject (�����̴� ����ȿ���� ����ȵ� ��ü)--------------//

class CMoveCubeObject : public CMonoObject
{
private:
	int m_selectedColor;

protected:
	double m_n;
	float m_len;
	MoveObjectData m_stc_mvobjdata;

public:
	CMoveCubeObject(unsigned int id, float len);
	MoveObjectData GetMoveObjectData() const { return m_stc_mvobjdata; }

	void UpdateCreateFirstPacketData(bool flag) { m_stc_mvobjdata.create_first = flag; }
	virtual void Collision(vector<CPlayerObject*>* clients, double deltime);

	virtual void Tick(double deltime) override;

	virtual void UpdateDataForPacket() override;
};


//--------------RigidCubeObject (����ȿ���� ����� ��ü)--------------//

class CRigidCubeObject : public CMonoObject
{
private:
	void AmendObject(XMFLOAT3 axis, float radian, CMonoObject *obj);
	void RigidBodyCollisionPlane(XMFLOAT3 & Normal, float distance, double deltime, CMonoObject *obj);

protected:
	RigidbodyData m_stc_robjdata;

public:
	CRigidCubeObject(unsigned int id);
	RigidbodyData GetRigidbodyData() const { return  m_stc_robjdata; }

	virtual void GravitySystem(double deltime) override;
	virtual void AfterGravitySystem(double deltime) override;
	virtual void Tick(double deltime) override;

	virtual void UpdateDataForPacket() override;

	void Collision(unordered_set<CRigidCubeObject*>* rbobjs, double deltime);
	void Collision(unordered_set<CStaticObject*>* sobjs, double deltime);
	void Collision(vector<CPlayerObject*>* clients, double deltime);
	void Collision(list<CBulletObject*>* bullets, double deltime);
};

//-------------------------------------------------------------------//


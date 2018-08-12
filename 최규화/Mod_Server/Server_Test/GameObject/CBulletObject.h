#pragma once
//#include "CMonoObject.h"
#define MAX_LIGHT_BULLET_TIME 2
#define MAX_HEAVY_BULLET_TIME 3.5
#define MAX_DICE_BULLET_TIME 5.5

class CBulletObject : public CMonoObject
{
private: 
	Position3D m_dicestrike_offlookvector;

protected:
	unsigned short m_masterID;			//불렛 마스터(불렛을 쏜 주인캐릭터)의 ID
	Position3D	   m_savept;		    //불렛의 최종도달지점
	Vel3f		   m_vel3f;			    //불렛 속도
	double		   m_lifetime{ 0 };		//불렛 생명주기
	char		   m_after_collision;	//0: 없음 1: 펑 2: 데미지 3: 펑&데미지
	XMFLOAT4	   m_orgpluspos;
	double		   m_tempangle;

public:
	CTS_BulletObject_Info         m_bulldata;


public:
	CBulletObject() {};
	CBulletObject(const unsigned short& master_id, const unsigned short& my_id,
		const Position& pos, const Rotation& rot, float bulltime,
		Vel3f& vel, const unsigned char& type, const Position3D& endpt, const float& degree);

	~CBulletObject();

	float			  GetBulletLifeTime() const { return m_lifetime; }
	void			  SetBulletLifeTime(float time) { m_lifetime += time; }

	CTS_BulletObject_Info GetBulletInfo() { return m_bulldata; }
	Position		  GetBulletOldPos() const { return m_pos4f; }

	void			  SetBulletNewPos(float x, float y, float z, float w) { m_pos4f.x = x; m_pos4f.y = y; m_pos4f.z = z; m_pos4f.w = w; }
	void			  SetBulletNewPos(const XMFLOAT4& xmf4) { m_pos4f.x = xmf4.x; m_pos4f.y = xmf4.y; m_pos4f.z = xmf4.z; m_pos4f.w = xmf4.w; }
	void			  DestroyBullet() { m_alive = false; }
	//virtual bool	  GetBulletIsAlive() { return static_cast<bool>(m_alive); }

	XMFLOAT3		  GetLookvector() const { return m_Lookvector; }
	Rotation		  GetBulletOldRot() const { return m_rot4f; }
	void			  SetBulletRotatevalue(const XMFLOAT4& xmf4);


	Position3D		  GetDicestrikeOffLookvector() const { return m_dicestrike_offlookvector; }
	void			  SetDicestrikeOffLookvector(const Position3D& pos3d) { m_dicestrike_offlookvector = move(pos3d); }
	
	STC_BulletObject_Info  GetChangedBulletState() const;

public:
	virtual NPC_BulletObject_Info GetChangedNPCBulletState() const {};	  //안씀 
	virtual STC_HammerSkillInfo   GetChangedHammerBulletState() const {}; //안씀

	unsigned short	  GetBulletID() const { return m_id; }
	unsigned short    GetBulletMasterID() const { return m_masterID; }

	virtual void UpdateDataForPacket() override;

	virtual void Tick(double deltime) override;
	virtual void AfterGravitySystem();

	virtual void Collision(vector<CPlayerObject*>* clients, double deltime);
	virtual void Collision(vector<CNpcObject*>* npcs, double deltime);
	virtual void Collision(unordered_set<CStaticObject*>* sobjs, double deltime);
};

class CHammerBulletObject : public CBulletObject
{
private:
	int		m_numOfHammer;
	bool    m_headBullet;

public:
	STC_HammerSkillInfo	m_stc_hammerInfo;
	virtual STC_HammerSkillInfo GetChangedHammerBulletState() const override;

public:
	CHammerBulletObject(const unsigned short& master_id, const unsigned short& myid,
		const Position& pos, const Rotation& rot, int numOfHammer, const XMFLOAT4& opp);
	
	virtual void UpdateDataForPacket() override;

	virtual void Tick(double deltime) override;
	virtual void AfterGravitySystem() override;

	virtual void Collision(vector<CPlayerObject*>* clients, double deltime) override;
	virtual void Collision(vector<CNpcObject*>* npcs, double deltime) override;
	virtual void Collision(unordered_set<CStaticObject*>* sobjs, double deltime) override;


};


class CStoneBulletObject : public CBulletObject
{
private:
	CNpcObject * m_npc_master;
	int	     m_npc_bulletID{ 0 };
	int      m_masterID{ 0 };				//몬스터 NPC는 최대 65535마리를 넘지않음 - 각방 최대 3마리 
											//5000명 동시접속 -> 5명씩 1000개의 방 -> 몬스터 최대 3000마리
public:
	NPC_BulletObject_Info m_npc_bulldata;
	virtual NPC_BulletObject_Info GetChangedNPCBulletState() const override;

public:
	CStoneBulletObject(CNpcObject *master, const XMFLOAT4& in_pos4f, const XMFLOAT4& in_rot4f, XMFLOAT4& ori, const XMFLOAT4& opp);

	static int g_npc_bulletID;
	const XMFLOAT4& GetOrgPlusPos() const { return m_orgpluspos; }

	virtual void UpdateDataForPacket() override;

	virtual void Tick(double deltime) override;
	virtual void AfterGravitySystem() override;

	virtual void Collision(vector<CPlayerObject*>* clients, double deltime) override;
	virtual void Collision(vector<CNpcObject*>* npcs, double deltime) override;
	virtual void Collision(unordered_set<CStaticObject*>* sobjs, double deltime) override;

};


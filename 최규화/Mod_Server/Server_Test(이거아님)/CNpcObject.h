#pragma once

class CPlayerObject;
class CStaticObject;
class FSM;

enum PLAYER_STATE
{
	DISCONNECT,
	IDLE,
	MOVE,
	ROTATE,
	ATTACK,
	DEAD,
};

class CNpcObject : public CMonoObject
{
protected:
	char				m_state{ IDLE };
	char				m_connect{ false };
	unsigned char		m_ani;
	int					m_monster_type;

private:
	Npc_Data			npc_data;

public:
	CNpcObject(int id, int type);
	CNpcObject();
	FSM *fsm{ nullptr };
	bool            m_respawn_state{ false };

	virtual void    UpdateDataForPacket() override;
	virtual void	GetDamaged(int damage);
	Npc_Data GetMyBasicPacketData() const;
	//Npc_Data& GetMyBasicPacketData();
	
	virtual void SetMyAnimation(unsigned char curr_anim);
	virtual unsigned char GetMyAnimation() const;

	virtual void Tick(double deltime) override;
	virtual void Tick(double deltime, Position& pos4f);
	
	void Collision(vector<CPlayerObject*>* clients, double deltime);			//Collision부분 상속할지 그냥할지 선택해야함
	void Collision(unordered_set<CStaticObject*>* sobjs, double deltime);

};


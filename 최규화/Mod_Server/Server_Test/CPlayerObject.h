#pragma once
#include <vector>

class CPlayerObject : public CNpcObject
{
private:
	boost::asio::ip::tcp::socket m_socket;

	//unsigned short		m_id;

	wchar_t				m_loginID[MAX_BUFFER_SIZE / 4]{ L"Guest" };
	wchar_t				m_loginPW[MAX_BUFFER_SIZE / 4]{ L"Guest" };

	Packet				m_recvbuf[MAX_BUFFER_SIZE]{ 0 };
	Packet				m_databuf[MAX_BUFFER_SIZE]{ 0 };

	//CBulletObject		*m_bullObj;
	float				m_prevtime{ 0 };
	float				m_elaspedtime{ 0 };
	SKILL_SHIELD_DATA	m_skill_shield;
	SKILL_WAVESHOCK_DATA m_skill_waveshock;
	SKILL_DICESTRIKE_DATA m_skill_dicestrike;

	bool				m_set_first_moveobjs{ false };

	//�÷��̾� ���� ���� ������
	int					m_player_score{ 0 };
	int					m_killCount;
	int					m_deathCount;
	float				m_score_for_rankcheck;
	int                 m_myCurrRank;
	int					m_myOldRank;
	bool                m_isTopRanker;
	int				    m_topRankerID{ -1 };

	//�÷��̾� �α��� ���õ�����(�÷��̾� �ؽ��� ���̵�, ���̵�, ��������)
	int					m_myTextureID;
	bool				m_isReady;

	bool				m_isReadyToPlay;
public:
	unsigned int		m_curr_packet_size{ 0 };
	unsigned int		m_prev_packet_size{ 0 };

	Player_Data			m_pdata;
	mutex				m_mtx;

	void				Lock() { m_mtx.lock(); }
	void				UnLock() { m_mtx.unlock(); }

public:
	CPlayerObject(const unsigned short& count, boost::asio::ip::tcp::socket socket) : m_socket(move(socket)) { m_id = count; m_isReadyToPlay = true; };
	~CPlayerObject();

	// ---------------------------------------------------------------------------------------
	// [1]. ���� ��� ���� �Լ�

	// 1. �ش� ��ü�� ����� Ŭ���̾�Ʈ�� ����ϴ� ���� ���������� �Ѱ���
	boost::asio::ip::tcp::socket& Socket() { return m_socket; }

	// 2. Ŭ���̾�Ʈ�� ��Ŷ�� ���� �� ����ϴ� ��������� Send�Լ�  - async_write ���
	void SendPacket(Packet* packet);
	void SendStaticObjectPacket(const unordered_set<CStaticObject*>& sobjs);

	// 3. Ŭ���̾�Ʈ�κ��� ��Ŷ�� �޾ƿ��� ��Ŷ�� �������� �� ����ϴ� ��������� Recv�Լ� - async_read_some ���
	void RecvPacket();

	// 4. Ŭ���̾�Ʈ�� ���� ����, �׸��� ������ �� ��Ŷ������ ����� ���۸� �޾� �˸°� ó��
	void ProcessPacket(Packet* packet);

	// [2]. DB���� �Լ�

	// 1. DB �α������� üũ
	bool CheckPlayerInfo();
	void CheckMyClient();

	// ---------------------------------------------------------------------------------------

	// [3]. �������� Ŭ���̾�Ʈ�� ������ ó���� �� ����ϴ� �Լ���
	// 1. �ʱ�ȭ (1. �������� �ʱ�ȭ 2. �÷��̾����� �ʱ�ȭ 3. �ʱ�ȭ������ ������ Ŭ���̾�Ʈ�� ����)
	//void Init_MonsterInfo();
	void Init_PlayerInfo();
	void InitData_To_Client();
	void InitNPCData_To_Client();
	void PlayerInput(double deltime);
	
	// ---------------------------------------------------------------------------------------
	// [4].��Ÿ GET - SET �Լ�
	
	Player_Data GetPlayerData() { return m_pdata; } const
	void SetPlayerData(Player_Data& pdata) { m_pdata = move(pdata); }

	//int		GetID() const { return m_id; }
	int		GetPlayerScore() const { return m_player_score; }
	void	SetPlayerScore(int score) { m_player_score += score; }

	void    SetState(int state) { m_state = state; }
	int		GetState() const { return m_state; }

	void	SetConnectState(char flag) { m_connect = flag; }
	bool	GetConnectState() const { return static_cast<bool>(m_connect); }

	virtual void	GetDamaged(int damage) override;
	bool    GetPlayerIsAlive() { return static_cast<bool>(m_alive); }
	void    SetPlayerIsAlive(char flag) { m_alive = flag; }

	void    SetIsAI(char ai) { m_ai = ai; }
	bool    GetIsAI() const { return static_cast<bool>(m_ai); }

	void    SetPlayerData_Airbone(char airbone) { m_airbone = airbone; }
	void	SetPlayerData_Pos(const XMFLOAT4& xmf4) { m_pos4f.x = xmf4.x; m_pos4f.y = xmf4.y; m_pos4f.z = xmf4.z; m_pos4f.w = xmf4.w; }
	void    SetPlayerData_Pos(const float& x, const float& y, const float& z, const float& w) { m_pos4f.x = x; m_pos4f.y = y; m_pos4f.z = z; m_pos4f.w = w; }

	void    SetPlayerDirection(char dir) { m_dir = dir; }
	char	GetPlayerDirection() const   { return m_dir; }
	void	SetPlayerAnimation(unsigned char ani) { m_ani = ani; }
	unsigned char GetPlayerAnimation() const { return m_ani; }

	bool	GetShieldState() const { return m_skill_shield.data.on_using; }
	void	SetShieldState(bool flag) { m_skill_shield.data.on_using = flag; }

	bool    GetShieldOnceFlag() const { return m_skill_shield.data.once_flag; }
	void	SetShieldOnceFlag(bool flag) { m_skill_shield.data.once_flag = flag; }

	double  GetShieldCurrtime() const { return m_skill_shield.data.op_time; }
	void	SetShieldCurrtime(double time) { m_skill_shield.data.op_time = time; }
	
	bool    GetWaveshockState() const { return m_skill_waveshock.data.on_using; }
	void	SetWaveshockState(bool flag) { m_skill_waveshock.data.on_using = flag; }

	bool    GetWaveshockOnceFlag() const { return m_skill_waveshock.data.once_flag; }
	void	SetWaveshockOnceFlag(bool flag) { m_skill_waveshock.data.once_flag = flag; }

	double  GetWaveshockCurrtime() const { return m_skill_waveshock.data.op_time; }
	void    SetWaveshockCurrtime(double time) { m_skill_waveshock.data.op_time = time; }

	bool	GetFirstMoveObjects() const { return m_set_first_moveobjs; }
	void    SetFirstMoveObjects(bool flag) { m_set_first_moveobjs = flag; }

	int     GetMyCharacterKillCount() const { return m_killCount; }
	void    SetMyCharacterKillCount(int count = 1) { m_killCount += count; }

	int     GetMyCharacterDeathCount() const { return m_deathCount; }
	void    SetMyCharacterDeathCount(int count = 1) { m_deathCount += count; }

	void    CalculatePlayerScoreForRanking();
	float   GetMyScoreForRanking() const { return m_score_for_rankcheck; }

	void    SetMyCharacterCurrRank(int _newrank) { m_myCurrRank = _newrank; }
	int		GetMyCharacterCurrRank() const    { return m_myCurrRank; }

	void    SetMyCharacterOldRank(int _oldrank) { m_myOldRank = _oldrank; }
	int     GetMyCharacterOldRank() const       { return m_myOldRank; }

	void    SetIsTopRanker(bool flag) { m_isTopRanker = flag; }
	bool    GetIsTopRanker() const { return m_isTopRanker; }

	void    SetCurrTopRankerID(int _id) { m_topRankerID = _id; }
	int 	GetCurrTopRankerID() const { return m_topRankerID; }

	void    SetIsReadyToPlay(bool flag) { m_isReadyToPlay = flag; }
	bool	GetIsReadyToPlay() const { return m_isReadyToPlay; }
	// ---------------------------------------------------------------------------------------
	// [5]. ����ȿ�� �Լ�

	//XMFLOAT3				GetLookVector()  const { return m_Lookvector; }
	//XMFLOAT3				GetRightVector() const { return m_Rightvector; }
	//XMFLOAT3				GetUpVector()	 const { return m_Upvector; }

	//PhysicsPoint*			GetPhysicsPoint() { return pp; }
	//PhysicalEffect*       GetPhysicsEffect() { return pe; }

	// ---------------------------------------------------------------------------------------
	//CScene				    *scene;

public:
	virtual void UpdateDataForPacket() override;

	virtual void Tick(double deltime) override;
	virtual void Tick(double deltime, Position& pos4f) override;

	void Collision(vector<CPlayerObject*>* clients, double deltime);
	void Collision(unordered_set<CStaticObject*>* sobjs, double deltime);
	void Collision_With_Waveshock();

};


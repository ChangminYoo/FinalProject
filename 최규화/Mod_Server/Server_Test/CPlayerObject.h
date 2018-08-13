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

	//플레이어 점수 관련 데이터
	int					m_player_score{ 0 };
	int					m_killCount;
	int					m_deathCount;
	float				m_score_for_rankcheck;
	int                 m_myCurrRank;
	int					m_myOldRank;
	bool                m_isTopRanker;
	int				    m_topRankerID{ -1 };

	//플레이어 로그인 관련데이터(플레이어 텍스쳐 아이디, 아이디, 레디유무)
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
	// [1]. 서버 통신 관련 함수

	// 1. 해당 객체로 연결된 클라이언트와 통신하는 서버 소켓정보를 넘겨줌
	boost::asio::ip::tcp::socket& Socket() { return m_socket; }

	// 2. 클라이언트로 패킷을 보낼 때 사용하는 사용자정의 Send함수  - async_write 사용
	void SendPacket(Packet* packet);
	void SendStaticObjectPacket(const unordered_set<CStaticObject*>& sobjs);

	// 3. 클라이언트로부터 패킷을 받아오고 패킷을 재조립할 때 사용하는 사용자정의 Recv함수 - async_read_some 사용
	void RecvPacket();

	// 4. 클라이언트로 부터 받은, 그리고 재조립 된 패킷정보가 저장된 버퍼를 받아 알맞게 처리
	void ProcessPacket(Packet* packet);

	// [2]. DB관련 함수

	// 1. DB 로그인정보 체크
	bool CheckPlayerInfo();
	void CheckMyClient();

	// ---------------------------------------------------------------------------------------

	// [3]. 서버에서 클라이언트의 정보를 처리할 때 사용하는 함수들
	// 1. 초기화 (1. 몬스터정보 초기화 2. 플레이어정보 초기화 3. 초기화정보를 접속한 클라이언트로 보냄)
	//void Init_MonsterInfo();
	void Init_PlayerInfo();
	void InitData_To_Client();
	void InitNPCData_To_Client();
	void PlayerInput(double deltime);
	
	// ---------------------------------------------------------------------------------------
	// [4].기타 GET - SET 함수
	
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
	// [5]. 물리효과 함수

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


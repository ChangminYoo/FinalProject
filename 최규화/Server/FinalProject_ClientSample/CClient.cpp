#include "CClient.h"

CharacterInfo CClient::g_charinfo = { {0,0},0 };
CharacterInfo CClient::g_opcharinfo = {{ 0,0 }, 1};
int CClient::g_myID = -1;

CClient::CClient()
{
	m_Packet = new CPacketInfo();

	//m_Packet->SceneStateInit(CHAR_SELECT_SCENE);
	//m_Packet->PacketInfoInit(PACK_TYPE::PACKET_INFO);
	//m_Packet->SelectPacketInfoInit(-1, false);
	//m_Packet->WaitRoomPacketInfoInit({ 0,0 }, -1, -1, false, false);
	//m_Packet->WaitThreadPacketInit();
	//m_Packet->WaitRoomCharInfoInit();
	//m_Packet->();

	m_Player = new Object();

	m_CurScene = IN_GAME_SCENE;

}

void CClient::mainClientThread()
{
	CreateThread(NULL, 0, this->ProcessClient, NULL, 0, NULL);
}
void CClient::MovePlayerInfo(int direction) {
	if (direction == 0) {//UP
		printf("이동!!\n\n\n\n\n\n\n\n");
		m_Packet->m_charInfo.char_pos.y = m_Packet->m_charInfo.char_pos.y - 1;
		

		if ( m_Packet->m_charInfo.char_pos.y < 0)
			m_Packet->m_charInfo.char_pos.y = 0;
	}
	else if (direction == 1) {//DOWN
		m_Packet->m_charInfo.char_pos.y = m_Packet->m_charInfo.char_pos.y + 1;
		

		if ( m_Packet->m_charInfo.char_pos.y > 440)
			m_Packet->m_charInfo.char_pos.y = 440;
	}
	else if (direction == 2) {//LEFT
		m_Packet->m_charInfo.char_pos.x = m_Packet->m_charInfo.char_pos.x - 1;
		
		if (m_Packet->m_charInfo.char_pos.x < 60)
			m_Packet->m_charInfo.char_pos.x = 60;
	}
	else if (direction == 3) {//RIGHT
		m_Packet->m_charInfo.char_pos.x = m_Packet->m_charInfo.char_pos.x + 1;
		
		if (m_Packet->m_charInfo.char_pos.x > 640)
			m_Packet->m_charInfo.char_pos.x = 640;
	}
	
}
void CClient::err_quit(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CClient::err_display(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int CClient::recvn(SOCKET s, char * buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);
}

void CClient::SetCurScene(int scene)
{
	m_CurScene = scene;
}

int CClient::GetCurScene() const
{
	return m_CurScene;
}

void CClient::SetPlayerInfo(int id, int pos_x, int pos_y, int speed, int hp, bool alive, bool ready)
{
	m_Player->SetCharID(id);
	m_Player->SetPositionX(pos_x);
	m_Player->SetPositionY(pos_y);
	m_Player->SetSpeed(speed);
	m_Player->SetHealth(hp);
	m_Player->SetIsAlive(alive);
	m_Player->SetSelectSceneReady(ready);
}

Object * CClient::GetPlayerInfo()
{
	return m_Player;
}


//클라이언트와의 통신용 스레드 - 연결
DWORD CClient::ProcessClient(LPVOID arg)
{
	CClient *myClass = (CClient*)arg;
	//myClass = new CClient();
	int count=0;
	int retval;
	int tempid;
	bool imInWait = true;
	bool intoWait = true;
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//socket()
	myClass->m_ClientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (myClass->m_ClientSock == INVALID_SOCKET)
		myClass->err_quit("socket()");


	//connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(myClass->m_ClientSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) 
		myClass->err_quit("connect()");

	while (1)
	{
		count = (count + 1) % MAX_COUNT;
		//1. 현재 씬에 대한 정보를 서버로 보냄 - 캐릭터 선택창
		if (count == EXCUTE_COUNT) 
		{
			retval = recv(myClass->m_ClientSock, (char*)&myClass->m_myID, sizeof(myClass->m_myID), 0);
			if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
			g_myID = myClass->m_myID.id;
			
			if (intoWait)
			{
				// 전송할 캐릭터 정보 구조체를 클라에서 사용할 구조체에 할당
				if (g_myID == 0)
				{
					myClass->m_Packet->m_charInfo.id = 0;
					myClass->m_Packet->m_charInfo.char_pos = { INIT_LEFT_CHAR_POSX , INIT_CHAR_POSY };
					g_charinfo = myClass->m_Packet->m_charInfo;
				}
				else
				{
					// 전송할 상대 캐릭터 정보 구조체를 클라에서 사용할 구조체에 할당
					myClass->m_opcharInfo.id = 1;
					myClass->m_opcharInfo.char_pos = { INIT_RIGHT_CHAR_POSX, INIT_CHAR_POSY };
					g_opcharinfo = myClass->m_opcharInfo;
				}
		
				// intoWait = 대기방에서 게임씬으로 전환될 때 한번 실행할 수 있도록 하는 변수
				intoWait = false;
			}

			//1. 캐릭터 정보전달
			if (g_myID == 0)
			{
				retval = send(myClass->m_ClientSock, (char*)&g_charinfo, sizeof(g_charinfo), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				printf("%d바이트  캐릭터정보 전송 완료,(%d,%d)\n", retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
			}

			if (g_myID == 1)
			{
				retval = send(myClass->m_ClientSock, (char*)&g_opcharinfo, sizeof(g_opcharinfo), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				printf("%d바이트  캐릭터정보 전송 완료,(%d,%d)\n", retval, myClass->g_opcharinfo.char_pos.x, myClass->g_opcharinfo.char_pos.y);
			}

			// 캐릭터 정보 수신
			retval = recv(myClass->m_ClientSock, (char*)&myClass->m_opcharInfo, sizeof(myClass->m_charInfo), 0);
			if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
			printf("★★%d바이트  상대방 캐릭터정보 수신 완료 posx : %d posy:%d★★\n\n\n\n", retval, myClass->m_opcharInfo.char_pos.x, myClass->m_opcharInfo.char_pos.y);					
		}
	}

	return 0;
}





CClient::~CClient()
{
}

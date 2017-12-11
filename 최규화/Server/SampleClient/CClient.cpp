#include "CClient.h"

bool CClient::g_charSelected = true;
bool CClient::g_charSelectedWait = false;
GameStateInfo CClient::g_gameinfo = { STAGE_NORMAL_STATE };
bool CClient::g_swapScene = false;
int CClient::g_selected = -1;
WaitRoomCharInfo CClient::g_waitRoomCharInfo = { {-1,-1},{-1},{-1,-1},{ IDB_BITMAP_CHAR_1_SEL }, {-1,-1 }, {false, false } };
int CClient::g_myID = -1;
CharacterInfo CClient::g_charinfo;
CharacterInfo CClient::g_opcharinfo;
ItemObjectInfo CClient::g_iteminfo = { ITEM_CLICK ,{0,0},false,-1 };
ObstacleObjectInfo CClient::g_obsinfo = { OBSTACLE_LASER,false,{0,0},-1 };
ObstacleObjectInfo CClient::g_obsinfo2 = { OBSTACLE_LASER,false,{ 0,0 },-1 };
ObstacleObjectInfo CClient::g_obsinfo3 = { OBSTACLE_LASER,false,{ 0,0 },-1 };
CClient::CClient()
{
	m_Packet = new CPacketInfo();

	m_Packet->SceneStateInit(CHAR_SELECT_SCENE);
	m_Packet->PacketInfoInit(PACK_TYPE::PACKET_INFO);
	m_Packet->SelectPacketInfoInit(-1, false);
	m_Packet->WaitRoomPacketInfoInit({ 0,0 }, -1, -1, false, false);
	m_Packet->WaitThreadPacketInit();
	m_Packet->WaitRoomCharInfoInit();
	//m_Packet->();

	m_Player = new Object();

	m_CurScene = CHAR_SELECT_SCENE;

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
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
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
	bool imInWait=true;
	bool intoWait = false;
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
		count = (count + 1) % 20000;
		//1. 현재 씬에 대한 정보를 서버로 보냄 - 캐릭터 선택창
		if (count == 15) {
			retval = recv(myClass->m_ClientSock, (char*)&myClass->m_sceneState, sizeof(myClass->m_sceneState), 0);
			if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
			else if (retval == 0) 		    break;
			cout << "패킷1크기 " << sizeof(myClass->m_sceneState) << "를 받았습니다.";
			cout << myClass->m_sceneState.sceneState << "번째 씬입니다." << endl;

			//여기서 분기별로 나눠짐 - 캐릭터선택창 / 캐릭터대기방 / 메인스테이지...
			//2 and 3. 현재 씬이 캐릭터 선택창이면? 
			if (myClass->m_sceneState.sceneState == CHAR_SELECT_SCENE)
			{
				//자신의 id값을 받아온다.
				retval = recv(myClass->m_ClientSock, (char*)&tempid, sizeof(int), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				printf("%d바이트의 id값: %d\n\n\n", retval, tempid);
				myClass->m_Packet->g_myID = tempid;
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_selected, sizeof(int), 0);
				//if (myClass->m_whoIsSelected.selected != -1)
				//	myClass->m_Packet->g_selected = myClass->m_whoIsSelected.selected;
				if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				else if (retval == 0) 		    break;
				cout << "패킷2- 크기 " << sizeof(myClass->m_whoIsSelected) << "를 받았습니다." << endl;
				/*
				//2. 현재 패킷에 대한 정보를 서버로 보냄 - 캐릭터 선택창
				myClass->PacketInfoInit(SELECT_ROOM);
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_packInfo, sizeof(myClass->m_packInfo), 0);
				if (retval == SOCKET_ERROR)		{ myClass->err_display("send()"); break; }
				else if (retval == 0) 		    break;
				cout << "패킷크기 " << sizeof(myClass->m_packInfo) << "를 보냈습니다." << endl;
				*/
				//3-[1]. [1].캐릭터 선택을 기다리라는 메시지를 담은 구조체패킷을 서버로 보냄
				myClass->m_waitThread.isReady = g_charSelected;
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_waitThread, sizeof(bool), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				else if (retval == 0) 		    break;
				cout << "패킷크기 " << sizeof(myClass->m_waitThread) << "를 보냈습니다." << endl;


				//3-[1]. [2].CreateEvent를 이용하여 현재 실행중인 클라이언트 스레드를 잠시 중시시킴
				//myClass->g_handle = CreateEvent(NULL, FALSE, FALSE, NULL);
				//if (myClass->g_handle == NULL)
				//	return 1;

				//3-[1]. [3].WaitForSingleObject를 이용하여 해당 핸들값에 해당하는 스레드가 다시 
				//	         신호상태가 될 때까지 기다림
				//WaitForSingleObject(myClass->g_handle, INFINITE);

				//-----------------------------캐릭터 선택 완료-------------------------------

				//3-[1]. [4].캐릭터 선택이 완료됐다는 메시지를 담은 구조체패킷을 서버로 보냄
				//myClass->m_waitThread.isReady = false;
				//retval = send(myClass->m_ClientSock, (char*)&myClass->m_waitThread, sizeof(myClass->m_waitThread), 0);
				//if (retval == SOCKET_ERROR)		{ myClass->err_display("send()"); break; }
				//else if (retval == 0) 		    break;
				//cout << "패킷크기 " << sizeof(myClass->m_waitThread) << "를 보냈습니다." << endl;

				//3-[1]. [5].선택한 캐릭터정보id와 캐릭터선택창에서 변화된 값을 지닌 캐릭터선택창구조체
				//           패킷을 서버로 보냄
				if (!myClass->m_waitThread.isReady)
				{
					myClass->SelectPacketInfoInit(myClass->m_Player->GetCharID(), true);
					retval = send(myClass->m_ClientSock, (char*)&myClass->m_selInfo, sizeof(myClass->m_selInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					else if (retval == 0) 		    break;
					cout << "패킷크기 " << sizeof(myClass->m_selInfo) << "를 보냈습니다." << endl;

					//3-[1]. [6].현재 씬에 대한 정보를 서버에서 선택창 -> 대기방으로 바꾼 뒤 클라이언트에서 받음 -> 씬교체
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_sceneState, sizeof(myClass->m_sceneState), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					else if (retval == 0) 		    break;
					cout << "패킷크기 " << sizeof(myClass->m_sceneState) << "를 받았습니다." << endl;

					//3-[1]. [7].씬을 다음씬으로 바꾸는데, 다른 플레이어도 캐릭터를 선택했을 때를 알려주기 위한 스택카운트
					myClass->SceneChangeCountIncrease(1);

					retval = send(myClass->m_ClientSock, (char*)&myClass->m_sceneChange, sizeof(myClass->m_sceneChange), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					else if (retval == 0) 		    break;
					cout << "패킷크기 " << sizeof(myClass->m_sceneChange) << "를 보냈습니다." << endl;

					/*
					//3-[1]. [8]. 캐릭터 대기방씬에서 캐릭터를 띄울 초기 위치값 및 기타 정보들을 담은 구조체 패킷을 받음
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_waitRoomCharInfo, sizeof(myClass->m_waitRoomCharInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					else if (retval == 0) 		    break;
					cout << "패킷크기 " << sizeof(myClass->m_waitRoomCharInfo) << "를 받았습니다." << endl;

					for (int i = 0; i < MAX_PLAYER; ++i)
					{
						myClass->g_waitRoomCharInfo.charPos[i] = myClass->m_waitRoomCharInfo.charPos[i];
						myClass->g_waitRoomCharInfo.size[i] = myClass->m_waitRoomCharInfo.size[i];
						myClass->g_waitRoomCharInfo.buttonPos[i] = myClass->m_waitRoomCharInfo.buttonPos[i];
						myClass->g_waitRoomCharInfo.buttonSize[i] = myClass->m_waitRoomCharInfo.buttonSize[i];
						myClass->g_waitRoomCharInfo.myBitMapNum[i] = myClass->m_waitRoomCharInfo.myBitMapNum[i];
						myClass->g_waitRoomCharInfo.joinRoom = myClass->m_waitRoomCharInfo.joinRoom;
					}
					*/
					//myClass->m_waitThread.isReady = true;
					//myClass->m_Packet->CharPacketInfoInit(myClass->m_selInfo.id);
					myClass->m_Packet->m_charInfo.packType = PACK_TYPE::CHARACTER_INFO;
					if (tempid == 0) {
						myClass->m_Packet->m_charInfo.char_pos = { 40, 400 };
						//m_Packet->m_charInfo.char_pos = { 40, 600 };
					}
					else {
						myClass->m_Packet->m_charInfo.char_pos = { 640, 400 };
						//ch.char_pos = { 640, 600 };
					}
					//ch.id= myClass->m_selInfo.id;
					myClass->m_Packet->m_charInfo.id = tempid;
					//ch.life = 20;
					myClass->m_Packet->m_charInfo.life = 20;
					//ch.score = 0;
					//ch.isjump = false;
					//myClass->g_swapScene = true;

					myClass->m_Packet->m_charInfo.score = 0;
					myClass->m_Packet->m_charInfo.isjump = false;
					myClass->g_swapScene = true;
				}
			}

			//4. 캐릭터 대기방
			else if (myClass->m_sceneState.sceneState == WAIT_ROOM_SCENE)
			{
			
				if (!intoWait) {
					retval = send(myClass->m_ClientSock, (char*)&imInWait, sizeof(bool), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d바이트 송신. 대기방 좌표값 요청\n\n", retval);
					intoWait = true;
				}
				retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_waitRoomCharInfo, sizeof(WaitRoomCharInfo), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				myClass->m_Packet->g_waitRoomCharInfo = myClass->m_Packet->m_waitRoomCharInfo;
				

				printf("%d바이트  수신, 대기방 좌표값 수신 완료. %d,%d로 출력\n\n", retval, myClass->m_Packet->m_waitRoomCharInfo.charPos[tempid].x, myClass->m_Packet->m_waitRoomCharInfo.charPos[tempid].y);
				myClass->m_Packet->m_waitRoomCharInfo.isready[tempid]= g_charSelectedWait;
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_waitRoomCharInfo.isready[tempid], sizeof(bool), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				printf("%d바이트 송신. 준비를 했는지 안했는지에 대한 여부 송신\n\n", retval);
				
				retval = recv(myClass->m_ClientSock, (char*)&myClass->m_waitSelected, sizeof(myClass->m_Packet->m_waitSelected), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				
				if(g_charSelectedWait)
					g_waitRoomCharInfo.isready[tempid] = g_charSelectedWait;
				//myClass->m_Packet->g_waitSelected[0] = myClass->m_Packet->m_waitSelected[0];
				//myClass->m_Packet->g_waitSelected[1] = myClass->m_Packet->m_waitSelected[1];
				printf("%d바이트 수신. 게임시작신으로 가는지의 여부 수신%d\n\n", retval, myClass->m_waitThreadInWait.isReady);
				if (myClass->m_waitThreadInWait.isReady&&myClass->m_Packet->g_waitRoomCharInfo.isready[0]&& myClass->m_Packet->g_waitRoomCharInfo.isready[1]) {
					myClass->m_sceneState.sceneState = IN_GAME_SCENE;
				}
				
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_sceneState.sceneState, sizeof(myClass->m_sceneState.sceneState), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				else if (retval == 0) 		    break;
				cout << "패킷크기 " << sizeof(myClass->m_sceneChange) << "를 보냈습니다." << endl;
				g_charinfo.char_pos.x = myClass->m_Packet->m_charInfo.char_pos.x;
				g_charinfo.char_pos.y = myClass->m_Packet->m_charInfo.char_pos.y;


			}

			else if (myClass->m_sceneState.sceneState == IN_GAME_SCENE)
			{
				if (intoWait) {
					//retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo, sizeof(myClass->m_Packet->m_charInfo), 0);
					//if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					//printf("%d바이트 수신, 게임캐릭터 정보 출력(%d,%d)\n\n\n",retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					g_charinfo = myClass->m_Packet->m_charInfo;
					g_opcharinfo = myClass->m_opcharInfo;
					intoWait = false;
				}
				retval = recv(myClass->m_ClientSock, (char*)&myClass->m_gameinfo, sizeof(myClass->m_gameinfo), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				printf("%d바이트 수신 완료. 서버에서 보낼 데이터는 %d번째 상황 데이터\n\n", retval, myClass->m_gameinfo.type);
				if (myClass->m_gameinfo.type == STAGE_NORMAL_STATE) {
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_carinfo, sizeof(myClass->m_carinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d바이트  자동차정보 전송 완료, 자동차posx : %d\n", retval, myClass->m_carinfo.posY[0]);
					retval = send(myClass->m_ClientSock, (char*)&g_charinfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d바이트  캐릭터정보 전송 완료,(%d,%d)\n", retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_opcharInfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("★★%d바이트  상대방 캐릭터정보 수신 완료 posx : %d posy:%d★★\n\n\n\n", retval, myClass->m_opcharInfo.char_pos.x, myClass->m_opcharInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d바이트  라이프정보 수신 완료 체력:%d\n", retval, myClass->m_Packet->m_charInfo.life);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_iteminfo, sizeof(myClass->m_Packet->m_iteminfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo2, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo3, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				}
				else if (myClass->m_gameinfo.type == STAGE_ITEM_STATE) 
				{
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_carinfo, sizeof(myClass->m_carinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d바이트  자동차정보 전송 완료, 자동차posx : %d\n", retval, myClass->m_carinfo.posY[0]);
					retval = send(myClass->m_ClientSock, (char*)&g_charinfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d바이트  캐릭터정보 전송 완료,(%d,%d)\n", retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_opcharInfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("★★%d바이트  상대방 캐릭터정보 수신 완료 posx : %d posy:%d★★\n\n\n\n", retval, myClass->m_opcharInfo.char_pos.x, myClass->m_opcharInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d바이트  라이프정보 수신 완료 체력:%d\n", retval, myClass->m_Packet->m_charInfo.life);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_iteminfo, sizeof(myClass->m_Packet->m_iteminfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					
					retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_iteminfo, sizeof(myClass->m_Packet->g_iteminfo), 0);

					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					if (myClass->m_Packet->m_iteminfo.id==-1 && myClass->m_Packet->g_iteminfo.id == -1) {
						g_iteminfo = myClass->m_Packet->m_iteminfo;
					}
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo2, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo3, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				}
				else if (myClass->m_gameinfo.type == STAGE_TRAP_STATE) {
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_carinfo, sizeof(myClass->m_carinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d바이트  자동차정보 전송 완료, 자동차posx : %d\n", retval, myClass->m_carinfo.posY[0]);
					retval = send(myClass->m_ClientSock, (char*)&g_charinfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d바이트  캐릭터정보 전송 완료,(%d,%d)\n", retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_opcharInfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("★★%d바이트  상대방 캐릭터정보 수신 완료 posx : %d posy:%d★★\n\n\n\n", retval, myClass->m_opcharInfo.char_pos.x, myClass->m_opcharInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d바이트  라이프정보 수신 완료 체력:%d\n", retval, myClass->m_Packet->m_charInfo.life);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_obsinfo, sizeof(myClass->m_Packet->g_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					if (myClass->m_Packet->m_obsinfo.id == -1 && myClass->m_Packet->g_obsinfo.id == -1) {
						g_obsinfo = myClass->m_Packet->m_obsinfo;
					}
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo2, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_obsinfo2, sizeof(myClass->m_Packet->g_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					if (myClass->m_Packet->m_obsinfo2.id == -1 && myClass->m_Packet->g_obsinfo2.id == -1) {
						g_obsinfo2 = myClass->m_Packet->m_obsinfo2;
					}
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo3, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_obsinfo3, sizeof(myClass->m_Packet->g_obsinfo), 0);

					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					if (myClass->m_Packet->m_obsinfo3.id == -1 && myClass->m_Packet->g_obsinfo3.id == -1) {
						g_obsinfo3 = myClass->m_Packet->m_obsinfo3;
					}
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_iteminfo, sizeof(myClass->m_Packet->m_iteminfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }

				}
				else if (myClass->m_gameinfo.type == STAGE_ITEMTRAP_STATE)
				{
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_carinfo, sizeof(myClass->m_carinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d바이트  자동차정보 전송 완료, 자동차posx : %d\n", retval, myClass->m_carinfo.posY[0]);
					retval = send(myClass->m_ClientSock, (char*)&g_charinfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d바이트  캐릭터정보 전송 완료,(%d,%d)\n", retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_opcharInfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("★★%d바이트  상대방 캐릭터정보 수신 완료 posx : %d posy:%d★★\n\n\n\n", retval, myClass->m_opcharInfo.char_pos.x, myClass->m_opcharInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d바이트  라이프정보 수신 완료 체력:%d\n", retval, myClass->m_Packet->m_charInfo.life);

					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_iteminfo, sizeof(myClass->m_Packet->m_iteminfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }

					retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_iteminfo, sizeof(myClass->m_Packet->g_iteminfo), 0);

					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					if (myClass->m_Packet->m_iteminfo.id == -1 && myClass->m_Packet->g_iteminfo.id == -1) {
						g_iteminfo = myClass->m_Packet->m_iteminfo;
					}

					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_obsinfo, sizeof(myClass->m_Packet->g_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					if (myClass->m_Packet->m_obsinfo.id == -1 && myClass->m_Packet->g_obsinfo.id == -1) {
						g_obsinfo = myClass->m_Packet->m_obsinfo;
					}
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo2, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_obsinfo2, sizeof(myClass->m_Packet->g_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					if (myClass->m_Packet->m_obsinfo2.id == -1 && myClass->m_Packet->g_obsinfo2.id == -1) {
						g_obsinfo2 = myClass->m_Packet->m_obsinfo2;
					}
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_obsinfo3, sizeof(myClass->m_Packet->m_obsinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_obsinfo3, sizeof(myClass->m_Packet->g_obsinfo), 0);

					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					if (myClass->m_Packet->m_obsinfo3.id == -1 && myClass->m_Packet->g_obsinfo3.id == -1) {
						g_obsinfo3 = myClass->m_Packet->m_obsinfo3;
					}
				}
				//g_charinfo = myClass->m_charInfo;
				
				
			//아이템이나 장애물을 독립적으로 짜라- 구역을 나눠서 짜라 

			}
		}
	}

	return 0;
}

//클라이언트와의 통신용 스레드 - 통신
/*
DWORD CClient::ProcessMainServer(LPVOID arg)
{
	CClient *myClass = (CClient *)arg;
	myClass = new CClient();

	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;

	//클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while (1)
	{
		//0.현재 클라이언트의 씬 정보에 대해서
		retval = recv(client_sock, (char*)&myClass->m_packInfo, sizeof(PackInfo), 0);

		//1.캐릭터 선택창 - 선택정보 넘어옴 - 고정길이
		if (myClass->m_packInfo.sceneState == CHAR_SELECT_SCENE)
		{
			retval = recv(client_sock, (char*)&myClass->m_selInfo, sizeof(Character_Select), 0);
			if (retval == SOCKET_ERROR)
				myClass->err_display("recv()");
			else if (retval == 0)
				break;
		}
		
		//if (myClass->m_curScene == CHAR_SELECT_SCENE)
		//{
			
		//}


	}



	return 0;
}
*/



CClient::~CClient()
{
}

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
		printf("�̵�!!\n\n\n\n\n\n\n\n");
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


//Ŭ���̾�Ʈ���� ��ſ� ������ - ����
DWORD CClient::ProcessClient(LPVOID arg)
{
	CClient *myClass = (CClient*)arg;
	//myClass = new CClient();
	int count=0;
	int retval;
	int tempid;
	bool imInWait=true;
	bool intoWait = false;
	//���� �ʱ�ȭ
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
		//1. ���� ���� ���� ������ ������ ���� - ĳ���� ����â
		if (count == 15) {
			retval = recv(myClass->m_ClientSock, (char*)&myClass->m_sceneState, sizeof(myClass->m_sceneState), 0);
			if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
			else if (retval == 0) 		    break;
			cout << "��Ŷ1ũ�� " << sizeof(myClass->m_sceneState) << "�� �޾ҽ��ϴ�.";
			cout << myClass->m_sceneState.sceneState << "��° ���Դϴ�." << endl;

			//���⼭ �б⺰�� ������ - ĳ���ͼ���â / ĳ���ʹ��� / ���ν�������...
			//2 and 3. ���� ���� ĳ���� ����â�̸�? 
			if (myClass->m_sceneState.sceneState == CHAR_SELECT_SCENE)
			{
				//�ڽ��� id���� �޾ƿ´�.
				retval = recv(myClass->m_ClientSock, (char*)&tempid, sizeof(int), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				printf("%d����Ʈ�� id��: %d\n\n\n", retval, tempid);
				myClass->m_Packet->g_myID = tempid;
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->g_selected, sizeof(int), 0);
				//if (myClass->m_whoIsSelected.selected != -1)
				//	myClass->m_Packet->g_selected = myClass->m_whoIsSelected.selected;
				if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				else if (retval == 0) 		    break;
				cout << "��Ŷ2- ũ�� " << sizeof(myClass->m_whoIsSelected) << "�� �޾ҽ��ϴ�." << endl;
				/*
				//2. ���� ��Ŷ�� ���� ������ ������ ���� - ĳ���� ����â
				myClass->PacketInfoInit(SELECT_ROOM);
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_packInfo, sizeof(myClass->m_packInfo), 0);
				if (retval == SOCKET_ERROR)		{ myClass->err_display("send()"); break; }
				else if (retval == 0) 		    break;
				cout << "��Ŷũ�� " << sizeof(myClass->m_packInfo) << "�� ���½��ϴ�." << endl;
				*/
				//3-[1]. [1].ĳ���� ������ ��ٸ���� �޽����� ���� ����ü��Ŷ�� ������ ����
				myClass->m_waitThread.isReady = g_charSelected;
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_waitThread, sizeof(bool), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				else if (retval == 0) 		    break;
				cout << "��Ŷũ�� " << sizeof(myClass->m_waitThread) << "�� ���½��ϴ�." << endl;


				//3-[1]. [2].CreateEvent�� �̿��Ͽ� ���� �������� Ŭ���̾�Ʈ �����带 ��� �߽ý�Ŵ
				//myClass->g_handle = CreateEvent(NULL, FALSE, FALSE, NULL);
				//if (myClass->g_handle == NULL)
				//	return 1;

				//3-[1]. [3].WaitForSingleObject�� �̿��Ͽ� �ش� �ڵ鰪�� �ش��ϴ� �����尡 �ٽ� 
				//	         ��ȣ���°� �� ������ ��ٸ�
				//WaitForSingleObject(myClass->g_handle, INFINITE);

				//-----------------------------ĳ���� ���� �Ϸ�-------------------------------

				//3-[1]. [4].ĳ���� ������ �Ϸ�ƴٴ� �޽����� ���� ����ü��Ŷ�� ������ ����
				//myClass->m_waitThread.isReady = false;
				//retval = send(myClass->m_ClientSock, (char*)&myClass->m_waitThread, sizeof(myClass->m_waitThread), 0);
				//if (retval == SOCKET_ERROR)		{ myClass->err_display("send()"); break; }
				//else if (retval == 0) 		    break;
				//cout << "��Ŷũ�� " << sizeof(myClass->m_waitThread) << "�� ���½��ϴ�." << endl;

				//3-[1]. [5].������ ĳ��������id�� ĳ���ͼ���â���� ��ȭ�� ���� ���� ĳ���ͼ���â����ü
				//           ��Ŷ�� ������ ����
				if (!myClass->m_waitThread.isReady)
				{
					myClass->SelectPacketInfoInit(myClass->m_Player->GetCharID(), true);
					retval = send(myClass->m_ClientSock, (char*)&myClass->m_selInfo, sizeof(myClass->m_selInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					else if (retval == 0) 		    break;
					cout << "��Ŷũ�� " << sizeof(myClass->m_selInfo) << "�� ���½��ϴ�." << endl;

					//3-[1]. [6].���� ���� ���� ������ �������� ����â -> �������� �ٲ� �� Ŭ���̾�Ʈ���� ���� -> ����ü
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_sceneState, sizeof(myClass->m_sceneState), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					else if (retval == 0) 		    break;
					cout << "��Ŷũ�� " << sizeof(myClass->m_sceneState) << "�� �޾ҽ��ϴ�." << endl;

					//3-[1]. [7].���� ���������� �ٲٴµ�, �ٸ� �÷��̾ ĳ���͸� �������� ���� �˷��ֱ� ���� ����ī��Ʈ
					myClass->SceneChangeCountIncrease(1);

					retval = send(myClass->m_ClientSock, (char*)&myClass->m_sceneChange, sizeof(myClass->m_sceneChange), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					else if (retval == 0) 		    break;
					cout << "��Ŷũ�� " << sizeof(myClass->m_sceneChange) << "�� ���½��ϴ�." << endl;

					/*
					//3-[1]. [8]. ĳ���� ��������� ĳ���͸� ��� �ʱ� ��ġ�� �� ��Ÿ �������� ���� ����ü ��Ŷ�� ����
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_waitRoomCharInfo, sizeof(myClass->m_waitRoomCharInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					else if (retval == 0) 		    break;
					cout << "��Ŷũ�� " << sizeof(myClass->m_waitRoomCharInfo) << "�� �޾ҽ��ϴ�." << endl;

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

			//4. ĳ���� ����
			else if (myClass->m_sceneState.sceneState == WAIT_ROOM_SCENE)
			{
			
				if (!intoWait) {
					retval = send(myClass->m_ClientSock, (char*)&imInWait, sizeof(bool), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d����Ʈ �۽�. ���� ��ǥ�� ��û\n\n", retval);
					intoWait = true;
				}
				retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_waitRoomCharInfo, sizeof(WaitRoomCharInfo), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				myClass->m_Packet->g_waitRoomCharInfo = myClass->m_Packet->m_waitRoomCharInfo;
				

				printf("%d����Ʈ  ����, ���� ��ǥ�� ���� �Ϸ�. %d,%d�� ���\n\n", retval, myClass->m_Packet->m_waitRoomCharInfo.charPos[tempid].x, myClass->m_Packet->m_waitRoomCharInfo.charPos[tempid].y);
				myClass->m_Packet->m_waitRoomCharInfo.isready[tempid]= g_charSelectedWait;
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_waitRoomCharInfo.isready[tempid], sizeof(bool), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				printf("%d����Ʈ �۽�. �غ� �ߴ��� ���ߴ����� ���� ���� �۽�\n\n", retval);
				
				retval = recv(myClass->m_ClientSock, (char*)&myClass->m_waitSelected, sizeof(myClass->m_Packet->m_waitSelected), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				
				if(g_charSelectedWait)
					g_waitRoomCharInfo.isready[tempid] = g_charSelectedWait;
				//myClass->m_Packet->g_waitSelected[0] = myClass->m_Packet->m_waitSelected[0];
				//myClass->m_Packet->g_waitSelected[1] = myClass->m_Packet->m_waitSelected[1];
				printf("%d����Ʈ ����. ���ӽ��۽����� �������� ���� ����%d\n\n", retval, myClass->m_waitThreadInWait.isReady);
				if (myClass->m_waitThreadInWait.isReady&&myClass->m_Packet->g_waitRoomCharInfo.isready[0]&& myClass->m_Packet->g_waitRoomCharInfo.isready[1]) {
					myClass->m_sceneState.sceneState = IN_GAME_SCENE;
				}
				
				retval = send(myClass->m_ClientSock, (char*)&myClass->m_sceneState.sceneState, sizeof(myClass->m_sceneState.sceneState), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
				else if (retval == 0) 		    break;
				cout << "��Ŷũ�� " << sizeof(myClass->m_sceneChange) << "�� ���½��ϴ�." << endl;
				g_charinfo.char_pos.x = myClass->m_Packet->m_charInfo.char_pos.x;
				g_charinfo.char_pos.y = myClass->m_Packet->m_charInfo.char_pos.y;


			}

			else if (myClass->m_sceneState.sceneState == IN_GAME_SCENE)
			{
				if (intoWait) {
					//retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo, sizeof(myClass->m_Packet->m_charInfo), 0);
					//if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					//printf("%d����Ʈ ����, ����ĳ���� ���� ���(%d,%d)\n\n\n",retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					g_charinfo = myClass->m_Packet->m_charInfo;
					g_opcharinfo = myClass->m_opcharInfo;
					intoWait = false;
				}
				retval = recv(myClass->m_ClientSock, (char*)&myClass->m_gameinfo, sizeof(myClass->m_gameinfo), 0);
				if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
				printf("%d����Ʈ ���� �Ϸ�. �������� ���� �����ʹ� %d��° ��Ȳ ������\n\n", retval, myClass->m_gameinfo.type);
				if (myClass->m_gameinfo.type == STAGE_NORMAL_STATE) {
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_carinfo, sizeof(myClass->m_carinfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d����Ʈ  �ڵ������� ���� �Ϸ�, �ڵ���posx : %d\n", retval, myClass->m_carinfo.posY[0]);
					retval = send(myClass->m_ClientSock, (char*)&g_charinfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d����Ʈ  ĳ�������� ���� �Ϸ�,(%d,%d)\n", retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_opcharInfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("�ڡ�%d����Ʈ  ���� ĳ�������� ���� �Ϸ� posx : %d posy:%d�ڡ�\n\n\n\n", retval, myClass->m_opcharInfo.char_pos.x, myClass->m_opcharInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d����Ʈ  ���������� ���� �Ϸ� ü��:%d\n", retval, myClass->m_Packet->m_charInfo.life);
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
					printf("%d����Ʈ  �ڵ������� ���� �Ϸ�, �ڵ���posx : %d\n", retval, myClass->m_carinfo.posY[0]);
					retval = send(myClass->m_ClientSock, (char*)&g_charinfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d����Ʈ  ĳ�������� ���� �Ϸ�,(%d,%d)\n", retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_opcharInfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("�ڡ�%d����Ʈ  ���� ĳ�������� ���� �Ϸ� posx : %d posy:%d�ڡ�\n\n\n\n", retval, myClass->m_opcharInfo.char_pos.x, myClass->m_opcharInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d����Ʈ  ���������� ���� �Ϸ� ü��:%d\n", retval, myClass->m_Packet->m_charInfo.life);
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
					printf("%d����Ʈ  �ڵ������� ���� �Ϸ�, �ڵ���posx : %d\n", retval, myClass->m_carinfo.posY[0]);
					retval = send(myClass->m_ClientSock, (char*)&g_charinfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d����Ʈ  ĳ�������� ���� �Ϸ�,(%d,%d)\n", retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_opcharInfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("�ڡ�%d����Ʈ  ���� ĳ�������� ���� �Ϸ� posx : %d posy:%d�ڡ�\n\n\n\n", retval, myClass->m_opcharInfo.char_pos.x, myClass->m_opcharInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d����Ʈ  ���������� ���� �Ϸ� ü��:%d\n", retval, myClass->m_Packet->m_charInfo.life);
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
					printf("%d����Ʈ  �ڵ������� ���� �Ϸ�, �ڵ���posx : %d\n", retval, myClass->m_carinfo.posY[0]);
					retval = send(myClass->m_ClientSock, (char*)&g_charinfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("send()"); break; }
					printf("%d����Ʈ  ĳ�������� ���� �Ϸ�,(%d,%d)\n", retval, myClass->m_Packet->m_charInfo.char_pos.x, myClass->m_Packet->m_charInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_opcharInfo, sizeof(myClass->m_charInfo), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("�ڡ�%d����Ʈ  ���� ĳ�������� ���� �Ϸ� posx : %d posy:%d�ڡ�\n\n\n\n", retval, myClass->m_opcharInfo.char_pos.x, myClass->m_opcharInfo.char_pos.y);
					retval = recv(myClass->m_ClientSock, (char*)&myClass->m_Packet->m_charInfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR) { myClass->err_display("recv()"); break; }
					printf("%d����Ʈ  ���������� ���� �Ϸ� ü��:%d\n", retval, myClass->m_Packet->m_charInfo.life);

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
				
				
			//�������̳� ��ֹ��� ���������� ¥��- ������ ������ ¥�� 

			}
		}
	}

	return 0;
}

//Ŭ���̾�Ʈ���� ��ſ� ������ - ���
/*
DWORD CClient::ProcessMainServer(LPVOID arg)
{
	CClient *myClass = (CClient *)arg;
	myClass = new CClient();

	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;

	//Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while (1)
	{
		//0.���� Ŭ���̾�Ʈ�� �� ������ ���ؼ�
		retval = recv(client_sock, (char*)&myClass->m_packInfo, sizeof(PackInfo), 0);

		//1.ĳ���� ����â - �������� �Ѿ�� - ��������
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

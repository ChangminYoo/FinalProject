#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996)
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include "stdafx.h"
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

//소켓 함수 오류 출력 후 종료

HANDLE hPlayer1Event;
HANDLE hPlayer2Event;
WaitRoomCharInfo g_waitRoomCharInfo = { { -1 },{ -1,-1 },{ -1 },{ -1,-1 },{ -1 },{ 115/*IDB_BITMAP12*/ },{ -1,-1 },false };
WaitRoomWhoIsSelected g_waitRoomSel = { { -1,-1 },{ false,false } ,{ 124,124 } ,{ 122,122 } };
int ready_people = 0;
//서버에서 기록하고 있는 패킷들의 정보
PacketClass packet_Folder;
bool m_ReadyOneID = false;
bool m_ReadyTwoID = false;
CharacterInfo g_cinfo[2];
GameStateInfo g_stateinfo = { STAGE_ITEM_STATE };
float g_PrevTime = 0.0f;
float g_CurTime = 0.0f;
float g_ElaspedTime = 0.0f;
int carposx[8] = { 140,200,260,320,410,470,530,600 };
int sendtype = 0;
int sendid = 0;
int tempcount = 0;
int usercount = 0;
CarObjectInfo carinfo;
ItemObjectInfo g_iteminfo = { ITEM_CLICK,{300,300},true,-1 };
ObstacleObjectInfo g_obsinfo = { OBSTACLE_LASER ,true,{ 100,200 },-1 };
ObstacleObjectInfo g_obsinfo2 = { OBSTACLE_LASER ,true,{ 400,140 },-1 };
ObstacleObjectInfo g_obsinfo3= { OBSTACLE_LASER ,true,{ 220,380 },-1 };

void MoveCar(CarObjectInfo);
int player1 = -1;
int player2 = -1;
bool goToGame = false;
bool IsCollide(CharacterInfo, CarObjectInfo);
void err_quit(char* msg)
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

void err_display(char *msg)
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
//사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;
	while (left > 0) {
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


DWORD WINAPI Player1Thread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	bool tempbool;
	ItemObjectInfo m_iteminfo;
	ObstacleObjectInfo m_obsinfo;
	ObstacleObjectInfo m_obsinfo_2;
	ObstacleObjectInfo m_obsinfo_3;

	int g_nextSceneCnt = 0;
	SOCKADDR_IN clientaddr;
	int addrlen;
	//PacketClass packet_Party;
	Game_Info gameinfo;
	int count = 0;
	Character_Select csinfo2;
	Character_Select csinfo3;
	CharacterInfo cinfo;
	WaitRoomCharInfo m_waitRoomCharInfo = { { -1 },{ -1,-1 },{ -1 },{ -1,-1 },{ -1 },{ 115/*IDB_BITMAP12*/ },{ -1,-1 },false };
	//CharacterInfo cinfo2;
	bool giveWaitPos = false;


	CarObjectInfo carinfo2;
	Wait_Room wrinfo;
	Wait_Room wrinfo2;
	int scenetype = 0;
	//ZeroMemory(&csinfo, sizeof(csinfo));
	//retval = WaitForSingleObject(hSendEvent, INFINITE);
	//클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);

	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);


	csinfo2.go_to_wr = false;
	csinfo2.id = sendid + 1;

	int tempid;
	tempid = 0;
	sendid++;
	cinfo.packType = PACK_TYPE::CHARACTER_INFO;
	cinfo.life = 20;
	cinfo.score = 0;

	packet_Folder.InitializePacket();


	
	for (int i = 0; i < 8; ++i)
		carinfo.posY[i] = 100;
	//retval = WaitForSingleObject(hPlayer2Event, INFINITE);
	int count5 = 0;

	int selected = -1;
	bool isR = true;
	while (1)
	{
		count = (count + 1) % 20000;
	
		if (count == 15) {

			retval = send(client_sock, (char*)&scenetype, sizeof(int), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}

			printf("%d바이트 수신, 현재 씬은 %d번째씬\n", retval, scenetype + 1);
			if (scenetype == 0)
			{
				if (scenetype != 1 && scenetype != 2) {
					retval = send(client_sock, (char*)&tempid, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					retval = recv(client_sock, (char*)&selected, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 수신, %d를 골랐습니다. \n", retval, selected);
					retval = recv(client_sock, (char*)&isR, sizeof(bool), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					printf("%d바이트 수신, 골랐나? %d \n", retval, isR);

				}
				if (!isR) {

					retval = recv(client_sock, (char*)&csinfo3, sizeof(csinfo3), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}

					printf("%d번째 수신완료 : What did you choose :  %d바이트를 수신했습니다.\n", csinfo3.id, retval);
					//retval = send(client_sock, (char*)&csinfo2, sizeof(csinfo3), 0);
					retval = send(client_sock, (char*)&scenetype, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					//				}
					retval = recv(client_sock, (char*)&scenetype, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					printf("%dbyte Received. SceneType : %d\n", retval, scenetype);
					//if (csinfo2.go_to_wr)


				}
			}
			else if (scenetype == 1)
			{


				printf("난 대기창이라오\n\n");
				if (!giveWaitPos) {

					retval = recv(client_sock, (char*)&tempbool, sizeof(bool), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					//giveWaitPos = true;
					if (ready_people == 0) {
						g_waitRoomCharInfo.charPos[tempid].x = 110;
						g_waitRoomCharInfo.charPos[tempid].y = 340;
						g_waitRoomCharInfo.id[tempid] = tempid;
						g_waitRoomCharInfo.joinRoom[tempid] = 1;
						if (selected == 0) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 115;
						}
						else if (selected == 1) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 116;
						}
						else if (selected == 2) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 117;
						}
						player1 = ready_people;
						g_waitRoomCharInfo.isready[tempid] = false;

						cinfo.char_pos.x = 60;
						cinfo.char_pos.y = 400;
						cinfo.id = tempid;
						cinfo.life = 20;
						cinfo.score = 0;
					}
					else {
						g_waitRoomCharInfo.charPos[tempid].x = 340;
						g_waitRoomCharInfo.charPos[tempid].y = 340;
						g_waitRoomCharInfo.id[tempid] = tempid;
						g_waitRoomCharInfo.joinRoom[tempid] = 1;
						if (selected == 0) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 115;
						}
						else if (selected == 1) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 116;
						}
						else if (selected == 2) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 117;
						}
						player1 = ready_people;
						g_waitRoomCharInfo.isready[tempid] = false;
						cinfo.char_pos.x = 640;
						cinfo.char_pos.y = 400;
						cinfo.id = tempid;
						cinfo.life = 20;
						cinfo.score = 0;
					}
					ready_people = ready_people + 1;

					giveWaitPos = true;
				}
				//bool형이나 pos만 출력하거나, 한번만 전송하는 방식으로 변경 예정
				retval = send(client_sock, (char*)&g_waitRoomCharInfo, sizeof(g_waitRoomCharInfo), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}
				printf("%d바이트  송신, 대기방 좌표값 송신 완료. %d,%d로 출력\n\n", retval, g_waitRoomCharInfo.charPos[tempid].x, g_waitRoomCharInfo.charPos[tempid].y);
				retval = recv(client_sock, (char*)&m_waitRoomCharInfo.isready[tempid], sizeof(bool), 0);
				g_waitRoomCharInfo.isready[tempid] = m_waitRoomCharInfo.isready[tempid];
				if (retval == SOCKET_ERROR)
				{
					err_display("recv()");
					break;
				}
				printf("%d바이트 수신. 준비를 했는지 안했는지에 대한 여부 수신\n\n", retval);


				if (m_waitRoomCharInfo.isready[tempid]) {
					m_ReadyOneID = true;
				}
				if (m_ReadyTwoID&& m_ReadyOneID) {
					goToGame = true;
				}
				if (goToGame) {
					scenetype = 2;
				}

				retval = send(client_sock, (char*)&goToGame, sizeof(goToGame), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}
				printf("%d바이트  송신, 대기방 좌표값 송신 완료. 모두 준비완료 됬을 시 True값으로 전송되는 bool형 데이터 전송\n\n", retval);
				retval = recv(client_sock, (char*)&scenetype, sizeof(int), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("recv()");
					break;
				}
				printf("%dbyte Received. SceneType : %d\n", retval, scenetype);


			}
			else if (scenetype == 2)
			{
				if (giveWaitPos) {
					//retval = send(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					//if (retval == SOCKET_ERROR)
					//{
					//	err_display("send()");
					//	break;
					//}
					//printf("%d바이트 발신, 초기의 캐릭터 정보 제공, 초기포지션은 (%d,%d) : %d\n", retval, cinfo.char_pos.x, cinfo.char_pos.y);
					giveWaitPos = false;
				}
				/*
				retval = send(client_sock, (char*)&cinfo2, sizeof(cinfo2), 0);
				if (retval == SOCKET_ERROR)
				{
				err_display("send()");
				break;
				}
				*/
				g_PrevTime = timeGetTime();
				printf("난 게임씬이요\n\n");
				//자동차 이동
				carinfo.posY[0] = (carinfo.posY[0] + 2) % (u_short)420;
				carinfo.posY[1] = (carinfo.posY[1] + 1) % (u_short)420;
				carinfo.posY[2] = (carinfo.posY[2] + (u_short)3) % (u_short)420;
				carinfo.posY[3] = (carinfo.posY[3] + (u_short)4) % (u_short)420;
				carinfo.posY[4] = (carinfo.posY[4] - (u_short)5);
				carinfo.posY[5] = (carinfo.posY[5] - (u_short)1);
				carinfo.posY[6] = (carinfo.posY[6] - (u_short)2);
				carinfo.posY[7] = (carinfo.posY[7] - (u_short)3);
				printf("난 게임씬이요\n\n");
				//자동차 이동
				retval = send(client_sock, (char*)&g_stateinfo, sizeof(g_stateinfo), 0);
				if (retval == SOCKET_ERROR) { err_display("send()");	break; }
				printf("%d바이트 수신, 내 게임 상태는 %d상황\n\n", retval, g_stateinfo);
				if (g_stateinfo.type == STAGE_NORMAL_STATE) {
					
					
					for (int i = 0; i < 4; ++i) {
						if (carinfo.posY[i + 4] <= 0) {
							carinfo.posY[i + 4] = 420;
						}
						carinfo2.posY[i] = (carinfo.posY[i]);
						carinfo2.posY[i + 4] = (carinfo.posY[i + 4]);

					}
					retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, carPosx : %d\n", retval, carinfo.posY[0]);
					retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0) {
						g_cinfo[0].char_pos.x = cinfo.char_pos.x;
						g_cinfo[0].char_pos.y = cinfo.char_pos.y;
						g_cinfo[0].life = cinfo.life;
						g_cinfo[0].score = cinfo.score;

					}
					else {
						g_cinfo[1].char_pos.x = cinfo.char_pos.x;
						g_cinfo[1].char_pos.y = cinfo.char_pos.y;
						g_cinfo[1].life = cinfo.life;
						g_cinfo[1].score = cinfo.score;

					}
					printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);
					if (tempid == 0)
						retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
					else
						retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0)
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
					else
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);
					retval = send(client_sock, (char*)&cinfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 남은 체력은 %d\n\n", retval, cinfo.life);
					retval = send(client_sock, (char*)&g_iteminfo, sizeof(g_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_iteminfo.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_obsinfo.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo2, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_obsinfo2.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo3, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_obsinfo3.item_pos.x);
					g_CurTime = timeGetTime();
					g_ElaspedTime += (g_CurTime - g_PrevTime) / 1000.0f;
				}
				else if (g_stateinfo.type == STAGE_ITEM_STATE)
				{
					for (int i = 0; i < 4; ++i) {
						if (carinfo.posY[i + 4] <= 0) {
							carinfo.posY[i + 4] = 420;
						}
						carinfo2.posY[i] = (carinfo.posY[i]);
						carinfo2.posY[i + 4] = (carinfo.posY[i + 4]);

					}
					retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, carPosx : %d\n", retval, carinfo.posY[0]);
					retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0) {
						g_cinfo[0].char_pos.x = cinfo.char_pos.x;
						g_cinfo[0].char_pos.y = cinfo.char_pos.y;
						g_cinfo[0].life = cinfo.life;
						g_cinfo[0].score = cinfo.score;

					}
					else {
						g_cinfo[1].char_pos.x = cinfo.char_pos.x;
						g_cinfo[1].char_pos.y = cinfo.char_pos.y;
						g_cinfo[1].life = cinfo.life;
						g_cinfo[1].score = cinfo.score;

					}
					printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);
					if (tempid == 0)
						retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
					else
						retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0)
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
					else
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);
					retval = send(client_sock, (char*)&cinfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 남은 체력은 %d\n\n", retval, cinfo.life);
					retval = send(client_sock, (char*)&g_iteminfo, sizeof(g_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval,g_iteminfo.item_pos.x);
					retval = recv(client_sock, (char*)&m_iteminfo, sizeof(m_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					if (m_iteminfo.id!=-1) {
						printf("%d유저가 아이템을 획득!!!!!!\n\n\n",tempid);
						g_iteminfo.is_alive = false;
						
					}
					retval = send(client_sock, (char*)&g_obsinfo, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_obsinfo.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo2, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_obsinfo2.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo3, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_obsinfo3.item_pos.x);
					if (!g_iteminfo.is_alive) {
						if (!g_obsinfo.is_alive)
							g_stateinfo.type = STAGE_NORMAL_STATE;
						else
							g_stateinfo.type = STAGE_TRAP_STATE;
					}
					else {
						if (g_obsinfo.is_alive)
							g_stateinfo.type = STAGE_ITEMTRAP_STATE;
					}
					g_CurTime = timeGetTime();
					g_ElaspedTime += (g_CurTime - g_PrevTime) / 1000.0f;
				}
				else if (g_stateinfo.type == STAGE_TRAP_STATE) {
					for (int i = 0; i < 4; ++i) {
						if (carinfo.posY[i + 4] <= 0) {
							carinfo.posY[i + 4] = 420;
						}
						carinfo2.posY[i] = (carinfo.posY[i]);
						carinfo2.posY[i + 4] = (carinfo.posY[i + 4]);

					}
					retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, carPosx : %d\n", retval, carinfo.posY[0]);
					retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0) {
						g_cinfo[0].char_pos.x = cinfo.char_pos.x;
						g_cinfo[0].char_pos.y = cinfo.char_pos.y;
						g_cinfo[0].life = cinfo.life;
						g_cinfo[0].score = cinfo.score;

					}
					else {
						g_cinfo[1].char_pos.x = cinfo.char_pos.x;
						g_cinfo[1].char_pos.y = cinfo.char_pos.y;
						g_cinfo[1].life = cinfo.life;
						g_cinfo[1].score = cinfo.score;

					}
					printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);
					if (tempid == 0)
						retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
					else
						retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0)
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
					else
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);
					retval = send(client_sock, (char*)&cinfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 남은 체력은 %d\n\n", retval, cinfo.life);
					retval = send(client_sock, (char*)&g_iteminfo, sizeof(g_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					
					retval = recv(client_sock, (char*)&m_obsinfo, sizeof(m_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_obsinfo2, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo_2, sizeof(m_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_obsinfo3, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo_3, sizeof(m_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_iteminfo, sizeof(g_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (m_obsinfo.id != -1) {
						printf("%d유저가 아이템을 획득!!!!!!\n\n\n", tempid);
						g_obsinfo.is_alive = false;
						
					}
					if (m_obsinfo_2.id != -1) {
						printf("%d유저가 장애물을 걸림!!!!!!\n\n\n", tempid);
						g_obsinfo2.is_alive = false;

					}
					if (m_obsinfo_3.id != -1) {
						printf("%d유저가 장애물을 걸림!!!!!!\n\n\n", tempid);
						g_obsinfo3.is_alive = false;

					}
					if (!g_obsinfo.is_alive&&!g_obsinfo2.is_alive&&!g_obsinfo3.is_alive) {
						if (!g_iteminfo.is_alive)
							g_stateinfo.type = STAGE_NORMAL_STATE;
						else
							g_stateinfo.type = STAGE_ITEM_STATE;
					}
					else {
						if (g_iteminfo.is_alive)
							g_stateinfo.type = STAGE_ITEMTRAP_STATE;
					}
					g_CurTime = timeGetTime();
					g_ElaspedTime += (g_CurTime - g_PrevTime) / 1000.0f;
				}
				else if (g_stateinfo.type == STAGE_ITEMTRAP_STATE) {
					for (int i = 0; i < 4; ++i) {
						if (carinfo.posY[i + 4] <= 0) {
							carinfo.posY[i + 4] = 420;
						}
						carinfo2.posY[i] = (carinfo.posY[i]);
						carinfo2.posY[i + 4] = (carinfo.posY[i + 4]);

					}
					retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, carPosx : %d\n", retval, carinfo.posY[0]);
					retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0) {
						g_cinfo[0].char_pos.x = cinfo.char_pos.x;
						g_cinfo[0].char_pos.y = cinfo.char_pos.y;
						g_cinfo[0].life = cinfo.life;
						g_cinfo[0].score = cinfo.score;

					}
					else {
						g_cinfo[1].char_pos.x = cinfo.char_pos.x;
						g_cinfo[1].char_pos.y = cinfo.char_pos.y;
						g_cinfo[1].life = cinfo.life;
						g_cinfo[1].score = cinfo.score;

					}
					printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);
					if (tempid == 0)
						retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
					else
						retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0)
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
					else
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);
					retval = send(client_sock, (char*)&cinfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 남은 체력은 %d\n\n", retval, cinfo.life);
					retval = send(client_sock, (char*)&g_iteminfo, sizeof(g_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_iteminfo, sizeof(m_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					if (m_iteminfo.id != -1) {
						printf("%d유저가 아이템을 획득!!!!!!\n\n\n", tempid);
						g_iteminfo.is_alive = false;

					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_iteminfo.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo, sizeof(m_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_obsinfo2, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo_2, sizeof(m_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_obsinfo3, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo_3, sizeof(m_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					if (m_obsinfo.id != -1) {
						printf("%d유저가 아이템을 획득!!!!!!\n\n\n", tempid);
						g_obsinfo.is_alive = false;

					}
					if (m_obsinfo_2.id != -1) {
						printf("%d유저가 장애물을 걸림!!!!!!\n\n\n", tempid);
						g_obsinfo2.is_alive = false;

					}
					if (m_obsinfo_3.id != -1) {
						printf("%d유저가 장애물을 걸림!!!!!!\n\n\n", tempid);
						g_obsinfo3.is_alive = false;

					}
					if (!g_iteminfo.is_alive){
						if (!g_obsinfo.is_alive && !g_obsinfo2.is_alive && !g_obsinfo3.is_alive) 
							g_stateinfo.type = STAGE_NORMAL_STATE;
						else
							g_stateinfo.type = STAGE_TRAP_STATE;
					}
					else if (!g_obsinfo.is_alive && !g_obsinfo2.is_alive && !g_obsinfo3.is_alive) {
						if (!g_iteminfo.is_alive)
							g_stateinfo.type = STAGE_NORMAL_STATE;
						else
							g_stateinfo.type = STAGE_ITEM_STATE;
					}
					g_CurTime = timeGetTime();
					g_ElaspedTime += (g_CurTime - g_PrevTime) / 1000.0f;
				}
			}
		}
		/*
		if (csinfo.character_type != 0) {
		packet_Folder.packet_Cs[0].go_to_wr = true;
		//retval = send(client_sock0, (char*)&packet_Folder.packet_Cs[0], sizeof(Character_Select), 0);
		retval = send(client_sock0, (char*)&csinfo, sizeof(Character_Select), 0);


		}
		else
		{
		packet_Folder.packet_Cs[0].go_to_wr = false;
		retval = send(client_sock0, (char*)&packet_Folder.packet_Cs[0], sizeof(Character_Select), 0);
		//retval = send(client_sock0, (char*)&addrlen, sizeof(int), 0);
		}
		if (retval == SOCKET_ERROR)
		{
		err_display("send()");
		break;
		}
		printf("id :%d 발신 완료\n", csinfo.id);
		retval = recv(client_sock0, (char*)&csinfo, sizeof(csinfo), 0);
		if (retval == SOCKET_ERROR)
		{
		err_display("recv()");
		break;
		}
		//printf("%d", csinfo.character_type);


		printf("수신완료 : What did you choose : %d\n", csinfo.character_type);

		*/
		//SetEvent(hCharacterSelectEvent);
		//SetEvent(hPlayer1Event);
	}

	//packet_Folder.SetC_SInfo(csinfo.id, csinfo.character_type);
	//csinfo.go_to_wr = true;
	//retval = send(client_sock0, (char*)&csinfo, sizeof(csinfo), 0);
	/*
	if (inet_ntoa(clientaddr.sin_addr) == inet_ntoa(clientaddr1.sin_addr)) {
	sendid = 2;
	}
	else if(inet_ntoa(clientaddr.sin_addr) == inet_ntoa(clientaddr2.sin_addr)) {
	sendid = 1;
	}
	*/







	//closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소 =%s,포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}
DWORD WINAPI Player2Thread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	bool tempbool;
	int g_nextSceneCnt = 0;
	SOCKADDR_IN clientaddr;
	int addrlen;
	//PacketClass packet_Party;
	Game_Info gameinfo;
	int count = 0;
	ItemObjectInfo m_iteminfo2;
	ObstacleObjectInfo m_obsinfo2;
	ObstacleObjectInfo m_obsinfo2_2;
	ObstacleObjectInfo m_obsinfo2_3;

	Character_Select csinfo2;
	Character_Select csinfo3;
	CharacterInfo cinfo;
	WaitRoomCharInfo m_waitRoomCharInfo = { { -1 },{ -1,-1 },{ -1 },{ -1,-1 },{ -1 },{ 115/*IDB_BITMAP12*/ },{ -1,-1 },false };
	//CharacterInfo cinfo2;
	bool giveWaitPos = false;

	
	Wait_Room wrinfo;
	Wait_Room wrinfo2;
	int scenetype = 0;
	//ZeroMemory(&csinfo, sizeof(csinfo));
	//retval = WaitForSingleObject(hSendEvent, INFINITE);
	//클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);

	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	CarObjectInfo carinfo2;
	csinfo2.go_to_wr = false;
	csinfo2.id = sendid + 1;

	int tempid;
	tempid = 1;

	cinfo.packType = PACK_TYPE::CHARACTER_INFO;
	cinfo.life = 20;
	cinfo.score = 0;

	packet_Folder.InitializePacket();
	if (tempid == 1) {
		//	cinfo2.char_pos.x = htonl(60);
		//	cinfo2.char_pos.y = htonl(400);
		cinfo.char_pos.x = 640;
		cinfo.char_pos.y = 400;
	}
	else {
		//	cinfo2.char_pos.x = 640;
		//	cinfo2.char_pos.y = 400;
		cinfo.char_pos.x = 60;
		cinfo.char_pos.y = 400;
	}
	//retval = WaitForSingleObject(hPlayer1Event, INFINITE);
	
	//for (int i = 0; i < 8; ++i)
	//	carinfo.posY[i] = 100;

	int count5 = 0;

	int selected = -1;
	bool isR = true;
	while (1)
	{
		count = (count + 1) % 20000;
		if (count == 15) {

			retval = send(client_sock, (char*)&scenetype, sizeof(int), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}

			printf("%d바이트 수신, 현재 씬은 %d번째씬\n", retval, scenetype + 1);
			if (scenetype == 0)
			{
				if (scenetype != 1 && scenetype != 2) {
					retval = send(client_sock, (char*)&tempid, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					retval = recv(client_sock, (char*)&selected, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 수신, %d를 골랐습니다. \n", retval, selected);
					retval = recv(client_sock, (char*)&isR, sizeof(bool), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					printf("%d바이트 수신, 골랐나? %d \n", retval, isR);

				}
				if (!isR) {

					retval = recv(client_sock, (char*)&csinfo3, sizeof(csinfo3), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}

					printf("%d번째 수신완료 : What did you choose :  %d바이트를 수신했습니다.\n", csinfo3.id, retval);
					//retval = send(client_sock, (char*)&csinfo2, sizeof(csinfo3), 0);
					retval = send(client_sock, (char*)&scenetype, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					//				}
					retval = recv(client_sock, (char*)&scenetype, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					printf("%dbyte Received. SceneType : %d\n", retval, scenetype);
					//if (csinfo2.go_to_wr)


				}
			}
			else if (scenetype == 1)
			{
				printf("난 대기창이라오\n\n");
				if (!giveWaitPos) {

					retval = recv(client_sock, (char*)&tempbool, sizeof(bool), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					//giveWaitPos = true;
					if (ready_people == 0) {
						g_waitRoomCharInfo.charPos[tempid].x = 110;
						g_waitRoomCharInfo.charPos[tempid].y = 340;
						g_waitRoomCharInfo.id[tempid] = tempid;
						g_waitRoomCharInfo.joinRoom[tempid] = 1;
						if (selected == 0) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 115;
						}
						else if (selected == 1) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 116;
						}
						else {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 117;
						}
						player2 = ready_people;
						g_waitRoomCharInfo.isready[tempid] = false;

						cinfo.char_pos.x = 60;
						cinfo.char_pos.y = 400;

					}
					else {
						g_waitRoomCharInfo.charPos[tempid].x = 340;
						g_waitRoomCharInfo.charPos[tempid].y = 340;
						g_waitRoomCharInfo.id[tempid] = tempid;
						g_waitRoomCharInfo.joinRoom[tempid] = 1;
						if (selected == 0) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 115;
						}
						else if (selected == 1) {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 116;
						}
						else {
							g_waitRoomCharInfo.myBitMapNum[tempid] = 117;
						}
						player2 = ready_people;
						g_waitRoomCharInfo.isready[tempid] = false;
						cinfo.char_pos.x = 640;
						cinfo.char_pos.y = 400;
					}
					ready_people = ready_people + 1;

					giveWaitPos = true;
				}
				//bool형이나 pos만 출력하거나, 한번만 전송하는 방식으로 변경 예정

				retval = send(client_sock, (char*)&g_waitRoomCharInfo, sizeof(g_waitRoomCharInfo), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}
				printf("%d바이트  송신, 대기방 좌표값 송신 완료. %d,%d로 출력\n\n", retval, g_waitRoomCharInfo.charPos[tempid].x, g_waitRoomCharInfo.charPos[tempid].y);
				printf("%d바이트  송신, 대기방 좌표값 송신 완료. %d,%d로 출력\n\n", retval, g_waitRoomCharInfo.charPos[tempid].x, g_waitRoomCharInfo.charPos[tempid].y);
				retval = recv(client_sock, (char*)&m_waitRoomCharInfo.isready[tempid], sizeof(bool), 0);
				g_waitRoomCharInfo.isready[tempid] = m_waitRoomCharInfo.isready[tempid];
				if (retval == SOCKET_ERROR)
				{
					err_display("recv()");
					break;
				}
				printf("%d바이트 수신. 준비를 했는지 안했는지에 대한 여부 수신\n\n", retval);


				if (m_waitRoomCharInfo.isready[tempid]) {
					m_ReadyTwoID = true;
				}
				if (m_ReadyTwoID&& m_ReadyOneID) {
					goToGame = true;
				}

				if (goToGame) {
					scenetype = 2;
				}

				retval = send(client_sock, (char*)&goToGame, sizeof(bool), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}
				printf("%d바이트  송신, 대기방 좌표값 송신 완료. 모두 준비완료 됬을 시 True값으로 전송되는 bool형 데이터 전송\n\n", retval);
				retval = recv(client_sock, (char*)&scenetype, sizeof(int), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("recv()");
					break;
				}
				

				printf("%dbyte Received. SceneType : %d\n", retval, scenetype);
				

			}
			else if (scenetype == 2)
			{
				if (giveWaitPos) {
					//retval = send(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					//if (retval == SOCKET_ERROR)
					//{
					//	err_display("send()");
					//	break;
					//}
					//printf("%d바이트 발신, 초기의 캐릭터 정보 제공, 초기포지션은 (%d,%d) : %d\n", retval, cinfo.char_pos.x, cinfo.char_pos.y);
					//giveWaitPos = false;
				}
				/*
				retval = send(client_sock, (char*)&cinfo2, sizeof(cinfo2), 0);
				if (retval == SOCKET_ERROR)
				{
				err_display("send()");
				break;
				}
				*/
				printf("난 게임씬이요\n\n");
				//자동차 이동
				retval =  send(client_sock, (char*)&g_stateinfo, sizeof(g_stateinfo), 0);
				if (retval == SOCKET_ERROR){err_display("send()");	break;}
				printf("%d바이트 수신, 내 게임 상태는 %d상황\n\n", retval, g_stateinfo);
				if (g_stateinfo.type == STAGE_NORMAL_STATE) {
					for (int i = 0; i < 4; ++i) {
						if (carinfo.posY[i + 4] <= 0) {
							carinfo.posY[i + 4] = 420;
						}
						carinfo2.posY[i] = (carinfo.posY[i]);
						carinfo2.posY[i + 4] = (carinfo.posY[i + 4]);

					}
					retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, carPosx : %d\n", retval, carinfo.posY[0]);
					retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0) {
						g_cinfo[0].char_pos.x = cinfo.char_pos.x;
						g_cinfo[0].char_pos.y = cinfo.char_pos.y;
						g_cinfo[0].life = cinfo.life;
						g_cinfo[0].score = cinfo.score;

					}
					else {
						g_cinfo[1].char_pos.x = cinfo.char_pos.x;
						g_cinfo[1].char_pos.y = cinfo.char_pos.y;
						g_cinfo[1].life = cinfo.life;
						g_cinfo[1].score = cinfo.score;

					}
					printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);
					if (tempid == 0)
						retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
					else
						retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0)
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
					else
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);
					retval = send(client_sock, (char*)&cinfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 남은 체력은 %d\n\n", retval, cinfo.life);
					retval = send(client_sock, (char*)&g_iteminfo, sizeof(g_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_iteminfo.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_obsinfo.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo2, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_obsinfo2.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo3, sizeof(g_obsinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_obsinfo3.item_pos.x);

				}
				else if (g_stateinfo.type == STAGE_ITEM_STATE)
				{
					for (int i = 0; i < 4; ++i) {
						if (carinfo.posY[i + 4] <= 0) {
							carinfo.posY[i + 4] = 420;
						}
						carinfo2.posY[i] = (carinfo.posY[i]);
						carinfo2.posY[i + 4] = (carinfo.posY[i + 4]);

					}
					retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, carPosx : %d\n", retval, carinfo.posY[0]);
					retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0) {
						g_cinfo[0].char_pos.x = cinfo.char_pos.x;
						g_cinfo[0].char_pos.y = cinfo.char_pos.y;
						g_cinfo[0].life = cinfo.life;
						g_cinfo[0].score = cinfo.score;

					}
					else {
						g_cinfo[1].char_pos.x = cinfo.char_pos.x;
						g_cinfo[1].char_pos.y = cinfo.char_pos.y;
						g_cinfo[1].life = cinfo.life;
						g_cinfo[1].score = cinfo.score;

					}
					printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);
					if (tempid == 0)
						retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
					else
						retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0)
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
					else
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);
					retval = send(client_sock, (char*)&cinfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 남은 체력은 %d\n\n", retval, cinfo.life);
					retval = send(client_sock, (char*)&g_iteminfo, sizeof(g_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_iteminfo.item_pos.x);
					retval = recv(client_sock, (char*)& m_iteminfo2, sizeof(m_iteminfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_obsinfo, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}retval = send(client_sock, (char*)&g_obsinfo2, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}retval = send(client_sock, (char*)&g_obsinfo3, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (m_iteminfo2.id != -1) {
						printf("%d유저가 아이템을 획득!!!!!!\n\n\n", tempid);
						g_iteminfo.is_alive = false;
						
					}
					if (!g_iteminfo.is_alive) {
						if (!g_obsinfo.is_alive)
							g_stateinfo.type = STAGE_NORMAL_STATE;
						else
							g_stateinfo.type = STAGE_TRAP_STATE;
					}
					else {
						if (g_obsinfo.is_alive||g_obsinfo2.is_alive||g_obsinfo3.is_alive)
							g_stateinfo.type = STAGE_ITEMTRAP_STATE;
					}
				}
				else if (g_stateinfo.type == STAGE_TRAP_STATE) {
					for (int i = 0; i < 4; ++i) {
						if (carinfo.posY[i + 4] <= 0) {
							carinfo.posY[i + 4] = 420;
						}
						carinfo2.posY[i] = (carinfo.posY[i]);
						carinfo2.posY[i + 4] = (carinfo.posY[i + 4]);

					}
					retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, carPosx : %d\n", retval, carinfo.posY[0]);
					retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0) {
						g_cinfo[0].char_pos.x = cinfo.char_pos.x;
						g_cinfo[0].char_pos.y = cinfo.char_pos.y;
						g_cinfo[0].life = cinfo.life;
						g_cinfo[0].score = cinfo.score;

					}
					else {
						g_cinfo[1].char_pos.x = cinfo.char_pos.x;
						g_cinfo[1].char_pos.y = cinfo.char_pos.y;
						g_cinfo[1].life = cinfo.life;
						g_cinfo[1].score = cinfo.score;

					}
					printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);
					if (tempid == 0)
						retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
					else
						retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0)
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
					else
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);
					retval = send(client_sock, (char*)&cinfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 남은 체력은 %d\n\n", retval, cinfo.life);
					retval = send(client_sock, (char*)&g_obsinfo, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo2, sizeof(m_obsinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_obsinfo2, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo2_2, sizeof(m_obsinfo2_2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_obsinfo3, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo2_3, sizeof(m_obsinfo2_3), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_iteminfo, sizeof(g_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (m_obsinfo2.id != -1) {
						printf("%d유저가 장애물을 걸림!!!!!!\n\n\n", tempid);
						g_obsinfo.is_alive = false;
						
					}
					if (m_obsinfo2_2.id != -1) {
						printf("%d유저가 장애물을 걸림!!!!!!\n\n\n", tempid);
						g_obsinfo2.is_alive = false;

					}
					if (m_obsinfo2_3.id != -1) {
						printf("%d유저가 장애물을 걸림!!!!!!\n\n\n", tempid);
						g_obsinfo3.is_alive = false;

					}
					if (!g_obsinfo.is_alive && !g_obsinfo2.is_alive && !g_obsinfo3.is_alive) {
						if (!g_iteminfo.is_alive)
							g_stateinfo.type = STAGE_NORMAL_STATE;
						else
							g_stateinfo.type = STAGE_ITEM_STATE;
					}
					else {
						if (g_iteminfo.is_alive)
							g_stateinfo.type = STAGE_ITEMTRAP_STATE;
					}

				}
				else if (g_stateinfo.type == STAGE_ITEMTRAP_STATE) {
					for (int i = 0; i < 4; ++i) {
						if (carinfo.posY[i + 4] <= 0) {
							carinfo.posY[i + 4] = 420;
						}
						carinfo2.posY[i] = (carinfo.posY[i]);
						carinfo2.posY[i + 4] = (carinfo.posY[i + 4]);

					}
					retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, carPosx : %d\n", retval, carinfo.posY[0]);
					retval = recv(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0) {
						g_cinfo[0].char_pos.x = cinfo.char_pos.x;
						g_cinfo[0].char_pos.y = cinfo.char_pos.y;
						g_cinfo[0].life = cinfo.life;
						g_cinfo[0].score = cinfo.score;

					}
					else {
						g_cinfo[1].char_pos.x = cinfo.char_pos.x;
						g_cinfo[1].char_pos.y = cinfo.char_pos.y;
						g_cinfo[1].life = cinfo.life;
						g_cinfo[1].score = cinfo.score;

					}
					printf("%d바이트 수신, ID는 %d\n\n", retval, cinfo.id);
					if (tempid == 0)
						retval = send(client_sock, (char*)&g_cinfo[1], sizeof(cinfo), 0);
					else
						retval = send(client_sock, (char*)&g_cinfo[0], sizeof(cinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					if (tempid == 0)
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[1].id);
					else
						printf("%d바이트 발신, 상대방 ID는 %d\n\n", retval, g_cinfo[0].id);
					retval = send(client_sock, (char*)&cinfo.life, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					printf("%d바이트 발신, 남은 체력은 %d\n\n", retval, cinfo.life);
					retval = send(client_sock, (char*)&g_iteminfo, sizeof(g_iteminfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_iteminfo2, sizeof(m_iteminfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					if (m_iteminfo2.id != -1) {
						printf("%d유저가 아이템을 획득!!!!!!\n\n\n", tempid);
						g_iteminfo.is_alive = false;

					}
					printf("%d바이트 발신, 아이템 위치는 %d\n\n", retval, g_iteminfo.item_pos.x);
					retval = send(client_sock, (char*)&g_obsinfo, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo2, sizeof(m_obsinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_obsinfo2, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo2_2, sizeof(m_obsinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					retval = send(client_sock, (char*)&g_obsinfo3, sizeof(g_obsinfo), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
					retval = recv(client_sock, (char*)&m_obsinfo2_3, sizeof(m_obsinfo2), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					if (m_obsinfo2.id != -1) {
						printf("%d유저가 아이템을 획득!!!!!!\n\n\n", tempid);
						g_obsinfo.is_alive = false;

					}
					if (m_obsinfo2_2.id != -1) {
						printf("%d유저가 장애물을 걸림!!!!!!\n\n\n", tempid);
						g_obsinfo2.is_alive = false;

					}
					if (m_obsinfo2_3.id != -1) {
						printf("%d유저가 장애물을 걸림!!!!!!\n\n\n", tempid);
						g_obsinfo3.is_alive = false;

					}
					if (!g_iteminfo.is_alive) {
						if (!g_obsinfo.is_alive && !g_obsinfo2.is_alive && !g_obsinfo3.is_alive)
							g_stateinfo.type = STAGE_NORMAL_STATE;
						else
							g_stateinfo.type = STAGE_TRAP_STATE;
					}
					else if (!g_obsinfo.is_alive && !g_obsinfo2.is_alive && !g_obsinfo3.is_alive) {
						if (!g_iteminfo.is_alive)
							g_stateinfo.type = STAGE_NORMAL_STATE;
						else
							g_stateinfo.type = STAGE_ITEM_STATE;
					}
				}
				/*
				retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2), 0);
				if (retval == SOCKET_ERROR)
				{
				err_display("send()");
				break;
				}
				printf("%d바이트 발신, carPosx : %d\n", retval,carinfo.posY[0]);
				retval = recv(client_sock, (char*)&cinfo2, sizeof(cinfo2), 0);
				if (retval == SOCKET_ERROR)
				{
				err_display("recv()");
				break;
				}

				printf("ID : %d\n", cinfo2.id);
				if (IsCollide(cinfo.char_pos, carinfo))
				{

				cinfo2.life = cinfo2.life - 1;
				}
				printf("%d바이트 수신완료 : %d \n",retval, cinfo2.id);
				*/
			}
		}

		//SetEvent(hPlayer2Event);
	}








	//closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소 =%s,포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}
/*
DWORD WINAPI SendThread(LPVOID arg)
{
SOCKET send_sock = (SOCKET)arg;
SOCKADDR_IN clientaddr;
Character_Select csinfo;
int addrlen;
int retval;
addrlen = sizeof(clientaddr);
getpeername(send_sock, (SOCKADDR*)&clientaddr, &addrlen);
csinfo.packType = PACK_TYPE::SELECT_ROOM;
csinfo.character_type = 0;
csinfo.go_to_wr = false;

csinfo.character_left = packet_Folder.packet_Cs[0].character_left;
csinfo.id = 0;
retval = WaitForSingleObject(hCharacterSelectEvent, INFINITE);
if (sendtype == PACK_TYPE::SELECT_ROOM) {
if (sendid == 1) {
//retval = send(client_sock, (char*)&packet_Folder.packet_Cs, sizeof(packet_Folder.packet_Cs), 0);
sendid = 0;

}
else if (sendid == 2) {
//retval = send(client_sock, (char*)&packet_Folder.packet_Cs, sizeof(packet_Folder.packet_Cs), 0);
sendid = 0;

}

}
SetEvent(hSendEvent);
return 0;
}
*/
int main(int argc, char* argv[])
{
	//소켓관련함수를 실행할 때 return value를 출력해주는 함수
	int retval;
	//접속자 수를 세는 int형 변수

	int scenetype = 0;
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	int count = 0;
	Character_Select csinfo2;
	Character_Select csinfo3;
	CharacterInfo cinfo;
	CharacterInfo cinfo2;
	CarObjectInfo carinfo;
	CarObjectInfo carinfo2;
	Wait_Room wrinfo;
	Wait_Room wrinfo2;
	printf("sizeof short : %d, %d\n", sizeof(u_short), sizeof(short));
	//socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN clientaddr1;

	if (sock == INVALID_SOCKET)
		err_quit("socket()");
	SOCKET client_sock;
	//현재 사용하는 패킷 구조체 초기값 설정
	printf("CharcterInfo size: %dByte\n", sizeof(CharacterInfo));
	printf("CarObjectInfo size: %dByte\n", sizeof(CarObjectInfo));
	printf("ItemObjectInfo size: %dByte\n", sizeof(ItemObjectInfo));
	printf("ObstacleObjectInfo size: %dByte\n", sizeof(ObstacleObjectInfo));
	printf("Game_Info size: %dByte\n", sizeof(Game_Info));
	printf("Character_Select size: %dByte\n", sizeof(Character_Select));
	printf("Wait_Room size: %dByte\n", sizeof(Wait_Room));
	printf("TimeInfo size: %dByte\n", sizeof(TimeInfo));
	g_cinfo[0].id = 0;
	g_cinfo[1].id = 1;
	g_cinfo[0].char_pos.x = 60;
	g_cinfo[0].char_pos.y = 400;
	g_cinfo[1].char_pos.x = 640;
	g_cinfo[1].char_pos.y = 400;

	packet_Folder.InitializePacket();
	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");
	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");







	ZeroMemory(&clientaddr1, sizeof(clientaddr1));

	//hPlayer1Event = CreateEvent(NULL, FALSE, TRUE, NULL);
	//hPlayer2Event = CreateEvent(NULL, FALSE, FALSE, NULL);

	int addrlen;
	HANDLE hThread;

	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr1);



		client_sock = accept(sock, (SOCKADDR *)&clientaddr1, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}
		// 접속한 클라이언트 정보 출력

		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr1.sin_addr), ntohs(clientaddr1.sin_port));


		for (int i = 0; i < 8; ++i)
			carinfo.posY[i] = 100;

		/*
		while (1) {

		if (scenetype == 0)
		{
		retval = send(client_sock, (char*)&csinfo2, sizeof(csinfo2), 0);
		if (retval == SOCKET_ERROR)
		{
		err_display("send()");
		break;
		}

		printf("id :%d 발신 완료\n", csinfo2.id);
		retval = recv(client_sock, (char*)&csinfo3, sizeof(csinfo3), 0);
		if (retval == SOCKET_ERROR)
		{
		err_display("recv()");
		break;
		}
		//printf("%d", csinfo.character_type);
		printf("수신완료 : What did you choose : %d\n", csinfo3.character_type);

		if (csinfo3.character_type != 0) {
		csinfo2.go_to_wr = true;
		csinfo2.character_left = csinfo3.character_type;

		}
		if(csinfo2.go_to_wr)
		scenetype = 2;
		}
		else if (scenetype == 1)
		{
		retval = send(client_sock, (char*)&wrinfo, sizeof(wrinfo), 0);
		if (retval == SOCKET_ERROR)
		{
		err_display("send()");
		break;
		}

		printf("발신 완료\n");
		retval = recv(client_sock, (char*)&wrinfo2, sizeof(wrinfo2), 0);
		if (retval == SOCKET_ERROR)
		{
		err_display("recv()");
		break;
		}
		//printf("%d", csinfo.character_type);


		if (wrinfo.game_ready)
		scenetype = 2;
		}
		else if (scenetype == 2)
		{
		//MoveCar(carinfo);
		//retval = send(client_sock, (char*)&cinfo, sizeof(cinfo), 0);
		//if (retval == SOCKET_ERROR)
		//{
		//	err_display("send()");
		//	break;
		//}

		carinfo.posY[0] = (carinfo.posY[0] + (u_short)2) % (u_short)420;
		carinfo.posY[1] = (carinfo.posY[1] + (u_short)1) % (u_short)420;
		carinfo.posY[2] = (carinfo.posY[2] + (u_short)3) % (u_short)420;
		carinfo.posY[3] = (carinfo.posY[3] + (u_short)4) % (u_short)420;
		carinfo.posY[4] = (carinfo.posY[4] - (u_short)5);
		carinfo.posY[5] = (carinfo.posY[5] - (u_short)1);
		carinfo.posY[6] = (carinfo.posY[6] - (u_short)2);
		carinfo.posY[7] = (carinfo.posY[7] - (u_short)3);

		for (int i = 0; i < 4; ++i) {
		if (carinfo.posY[i + 4] <= 0) {
		carinfo.posY[i + 4] = 420;
		}
		carinfo2.posY[i] = htons(carinfo.posY[i]);
		carinfo2.posY[i+4] = htons(carinfo.posY[i+4]);

		}
		retval = send(client_sock, (char*)&carinfo2, sizeof(carinfo2),0);
		if (retval == SOCKET_ERROR)
		{
		err_display("send()");
		break;
		}
		printf("carPosx : %d\n", carinfo.posY[0]);
		retval = recv(client_sock, (char*)&cinfo2, sizeof(cinfo2), 0);
		if (retval == SOCKET_ERROR)
		{
		err_display("recv()");
		break;
		}
		if (IsCollide(cinfo.char_pos, carinfo))
		{

		cinfo2.life = cinfo2.life - 1;
		}
		printf("수신완료 : %d\n", cinfo2.id);
		}
		}*/
		if (usercount == 0) {
			hThread = CreateThread(NULL, 0, Player1Thread, (LPVOID)client_sock, 0, NULL);

			if (hThread == NULL) {
				closesocket(client_sock);
			}
			else {
				CloseHandle(hThread);
			}
		}
		else if(usercount==1){
			hThread = CreateThread(NULL, 0, Player2Thread, (LPVOID)client_sock, 0, NULL);

			if (hThread == NULL) {
				closesocket(client_sock);
			}
			else {
				CloseHandle(hThread);
			}
		}
		//closesocket(client_sock);



		usercount = usercount + 1;;

	}
	CloseHandle(hPlayer1Event);
	CloseHandle(hPlayer2Event);

	closesocket(sock);
	WSACleanup();
	return 0;
}


void MoveCar(CarObjectInfo car) {

	//car.posY[0] = (car.posY[0] + 3) % 420;
	car.posY[0] = 150;
	car.posY[1] = (car.posY[1] + 4) % 420;
	car.posY[2] = (car.posY[2] + 2) % 420;
	car.posY[3] = (car.posY[3] + 1) % 420;
	car.posY[4] = (car.posY[4] - 1) % 420;
	car.posY[5] = (car.posY[5] - 2) % 420;
	car.posY[6] = (car.posY[6] - 2) % 420;
	car.posY[7] = (car.posY[7] - 5) % 420;


}
bool IsCollide(CharacterInfo ch, CarObjectInfo car) {
	for (int i = 0; i < 8; ++i)
	{

		bool col1 = (ch.char_pos.x + 35 > carposx[i] - 25);
		bool col2 = (ch.char_pos.x - 35 < carposx[i] + 25);
		bool col3 = (ch.char_pos.y + 35 > car.posY[i] - 40);
		bool col4 = (ch.char_pos.y - 35 < car.posY[i] + 40);
		if ((col1 && col2) && (col3 && col4))
		{
			cout << "충돌" << endl;
			return true;
		}

		else
			return false;
	}
	return false;
}
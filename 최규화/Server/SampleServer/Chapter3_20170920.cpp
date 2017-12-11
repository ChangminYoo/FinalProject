#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iterator>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#define SERVERPORT 9000
#define BUFSIZE    512
#define TESTNAME2 "www.pimang.com"
#define TESTNAME "www.facebook.com"
#define TESTNAME3 "www.kpu.ac.kr"
using namespace std;
//소켓 함수 오류 출력 후 중지
void err_quit(char *msg)
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

// 소켓 함수 오류 출력
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


//도메인 이름->IPv4 주소 변환
BOOL GetIPAddr(char* name, IN_ADDR* addr) {
	HOSTENT* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET) { 
		return FALSE;
	}
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return TRUE;
}
BOOL GetDomainAliases(char* domainName, IN_ADDR* addr);

//IPv4 주소 ->도메인 이름
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen) {
	HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);

	if (ptr == NULL) {
		err_display("gethostbyaddr()");
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET)
		return FALSE;
	printf("Domain Name : %s\n", ptr->h_name);
	//별명 찾기
	if (*ptr->h_aliases == NULL)
		printf("No Aliase\n");
	while (*ptr->h_aliases != NULL) {
		printf("Aliases : %s\n", *ptr->h_aliases);
		ptr->h_aliases++;
	}
	strncpy(name, ptr->h_name, namelen);
	GetDomainAliases(ptr->h_name, &addr);
	return TRUE;

}
BOOL GetDomainByIPv6() {
	return FALSE;
}


int main(int argc, char* argv[]) {
	WSADATA wsa;
	IN_ADDR addr;
	int caseOfType;
	char name[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	while (1) {
		printf("1.도메인으로 정보 출력, 2.IPv4로 정보 출력 : ");
		scanf("%d", &caseOfType);
		switch (caseOfType)
		{
		case 1:
			printf("도메인입력 :");
			scanf("%s", &name);
			printf("검색한 도메인: %s\n", name);
			//도메인이름->별명
			if (GetDomainAliases(name, &addr)) {
				//성공이면 결과 출력
				//printf("IPv4 Address =%s\n", inet_ntoa(addr));
			}
			break;
		case 2:
			printf("IP입력 :");
			scanf("%s", &name);
			printf("검색한 도메인: %s\n", name);
			IN_ADDR tempaddr;
			tempaddr.s_addr = inet_addr(name);
			GetDomainName(tempaddr, name, sizeof(name));
			
			break;
		default:
			printf("잘못된 입력!!\n");
			break;
		}
		
	}
	/*
	//도메인이름->IP주소
	if (GetIPAddr(TESTNAME, &addr)) {
		//성공이면 결과 출력
		printf("IP 주소(변환 후) =%s\n", inet_ntoa(addr));
		
		//IP주소 ->도메인 이름
		if (GetDomainName(addr, name, sizeof(name))) {
			//성공이면 결과 출력
			printf("도메인 이름(다시 변환 후) = %s\n", name);
		}
	}
	*/
	
	WSACleanup();
	return 0;
}

BOOL GetDomainAliases(char* domainName, IN_ADDR* addr) {
	HOSTENT* ptr = gethostbyname(domainName);
	addrinfo* result = NULL;

	addrinfo addInfo;
	ZeroMemory(&addInfo, sizeof(addInfo));
	sockaddr_in* ipv4;
	sockaddr_in6* ipv6;
	LPSOCKADDR sockaddr_ip; //WSAAddressToString을 위한 파라미터1
	DWORD ipbufferlength = 30;
	char ipstringbuffer[30];
	char ipstringbuffer4[16];

	int iRetval;
	
	addInfo.ai_family = AF_UNSPEC;
	addInfo.ai_socktype = SOCK_STREAM;
	addInfo.ai_protocol = IPPROTO_TCP;
	addInfo.ai_flags = AI_ALL;
	int test = getaddrinfo(domainName, NULL, &addInfo, &result);
	

	if (test != 0) {
		err_display("getaddrinfo()");
	}

	if (result == NULL) {
		printf("Don't have Ipv6 address");
		err_display("result is null.");
		return FALSE;

	}
	if (ptr == NULL) {
		printf("Err2");
		err_display("gethostbyname()");
		return FALSE;
	}
	while (result != NULL) {

		switch (result->ai_family) {

		case AF_INET6:


			sockaddr_ip = (LPSOCKADDR)result->ai_addr;

			ipbufferlength = 30;
			iRetval = WSAAddressToString(sockaddr_ip, (DWORD)result->ai_addrlen, NULL, ipstringbuffer, &ipbufferlength);
			if (iRetval)
				printf("변환실패!! %u\n", WSAGetLastError());
			else
				printf("\tIPv6 address : %s\n", ipstringbuffer);
			break;
		case AF_INET:
			sockaddr_ip = (LPSOCKADDR)result->ai_addr;
			ipbufferlength = 16;
			iRetval = WSAAddressToString(sockaddr_ip, (DWORD)result->ai_addrlen, NULL, ipstringbuffer, &ipbufferlength);
			if (iRetval)
				printf("변환실패!! %u\n", WSAGetLastError());
			else
				printf("\tIPv4 address : %s\n", ipstringbuffer);
			//ipv4 = (sockaddr_in *) result->ai_addr;

			//printf("\tIPv4 address %s\n", inet_ntoa(ipv4->sin_addr));
			break;
		default:
			printf("불명\n");
			break;
		}
		result = result->ai_next;
	}
	//memcpy(addr, ptr->h_addr, ptr->h_length);




	//while (*ptr->h_addr_list != NULL) {
	//	printf("IPv4 Address : %s\n", inet_ntoa(*addr));
	//	addr++;
	//	ptr->h_addr_list++;
	//}
	printf("Official Domain Name : %s\n", ptr->h_name);
	//별명 찾기
	
		if (*ptr->h_aliases == NULL)
			printf("No Aliase\n");
		while (*ptr->h_aliases != NULL) {
			printf("Aliases : %s\n", *ptr->h_aliases);
			ptr->h_aliases++;
		}
	



	freeaddrinfo(result);
	return TRUE;
}
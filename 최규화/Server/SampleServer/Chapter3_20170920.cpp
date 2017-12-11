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
//���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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


//������ �̸�->IPv4 �ּ� ��ȯ
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

//IPv4 �ּ� ->������ �̸�
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen) {
	HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);

	if (ptr == NULL) {
		err_display("gethostbyaddr()");
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET)
		return FALSE;
	printf("Domain Name : %s\n", ptr->h_name);
	//���� ã��
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
		printf("1.���������� ���� ���, 2.IPv4�� ���� ��� : ");
		scanf("%d", &caseOfType);
		switch (caseOfType)
		{
		case 1:
			printf("�������Է� :");
			scanf("%s", &name);
			printf("�˻��� ������: %s\n", name);
			//�������̸�->����
			if (GetDomainAliases(name, &addr)) {
				//�����̸� ��� ���
				//printf("IPv4 Address =%s\n", inet_ntoa(addr));
			}
			break;
		case 2:
			printf("IP�Է� :");
			scanf("%s", &name);
			printf("�˻��� ������: %s\n", name);
			IN_ADDR tempaddr;
			tempaddr.s_addr = inet_addr(name);
			GetDomainName(tempaddr, name, sizeof(name));
			
			break;
		default:
			printf("�߸��� �Է�!!\n");
			break;
		}
		
	}
	/*
	//�������̸�->IP�ּ�
	if (GetIPAddr(TESTNAME, &addr)) {
		//�����̸� ��� ���
		printf("IP �ּ�(��ȯ ��) =%s\n", inet_ntoa(addr));
		
		//IP�ּ� ->������ �̸�
		if (GetDomainName(addr, name, sizeof(name))) {
			//�����̸� ��� ���
			printf("������ �̸�(�ٽ� ��ȯ ��) = %s\n", name);
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
	LPSOCKADDR sockaddr_ip; //WSAAddressToString�� ���� �Ķ����1
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
				printf("��ȯ����!! %u\n", WSAGetLastError());
			else
				printf("\tIPv6 address : %s\n", ipstringbuffer);
			break;
		case AF_INET:
			sockaddr_ip = (LPSOCKADDR)result->ai_addr;
			ipbufferlength = 16;
			iRetval = WSAAddressToString(sockaddr_ip, (DWORD)result->ai_addrlen, NULL, ipstringbuffer, &ipbufferlength);
			if (iRetval)
				printf("��ȯ����!! %u\n", WSAGetLastError());
			else
				printf("\tIPv4 address : %s\n", ipstringbuffer);
			//ipv4 = (sockaddr_in *) result->ai_addr;

			//printf("\tIPv4 address %s\n", inet_ntoa(ipv4->sin_addr));
			break;
		default:
			printf("�Ҹ�\n");
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
	//���� ã��
	
		if (*ptr->h_aliases == NULL)
			printf("No Aliase\n");
		while (*ptr->h_aliases != NULL) {
			printf("Aliases : %s\n", *ptr->h_aliases);
			ptr->h_aliases++;
		}
	



	freeaddrinfo(result);
	return TRUE;
}
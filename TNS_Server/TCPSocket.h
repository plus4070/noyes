#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <winsock2.h> 
#include "stdafx.h"
#include "TopicNameTable.h"
#include "RequestTable.h"


using namespace std;
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib, "ws2_32.lib")


class TCPSocket
{
public:
	TCPSocket();
	TNSN_ENTRY		TNSNDatagram;
	TopicNameTable*	TNTable;
	RequestTable *	RTable;

private:
	// 변수 선언	
	WSADATA wsaData;
	SOCKET hServSock;
	SOCKADDR_IN servAddr;

	SOCKET hSockArray[WSA_MAXIMUM_WAIT_EVENTS]; //소켓 핸들배열 - 연결 요청이 들어올 때마다 생성되는 소켓의 핸들을 이 배열에 저장. (최대64)
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;

	WSAEVENT hEventArray[WSA_MAXIMUM_WAIT_EVENTS];	// 이벤트 배열 
	WSAEVENT newEvent;
	WSANETWORKEVENTS netEvents;

	int clntLen;
	int sockTotal;
	int index, i;
	char message[BUFSIZE];
	int strLen;

	//void CompressSockets(SOCKET* hSockArray, int omitIndex, int total);
	//void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total);

	//void ErrorHandling(char *message);
	//구현 함수
	void inputDummyEntryToTNTable();


public:
	int StartServer();
	void ResetTCPSocket();
	void Response();
	void SaveRequests(IN_ADDR ip, TNSN_ENTRY tnsData);
	void initialize();
};


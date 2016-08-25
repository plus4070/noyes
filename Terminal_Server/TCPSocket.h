// 해더파일 선언
#include <winsock2.h>
#include "stdafx.h"
#include "ParticipantDataDistributor.h"
#include "TerminalTable.h"
#include "RequestTable.h"
#include "DBManager.h"

using namespace std;

// ws2_32.lib 링크
#pragma comment(lib, "ws2_32.lib")

#pragma once
class TCPSocket
{
public:
	TNSN_ENTRY					TNSNDatagram;
	RequestTable *				RTable;
	TerminalTable *				participantList;
	ParticipantDataDistributor*	distributor;
	DBManager * DB;

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


public:
	TCPSocket();
	int StartServer();

	void ResetTCPSocket();
	void Response();
	void SaveRequests(IN_ADDR ip, TNSN_ENTRY tnsData);
	void participantDataDistribute();
	void inputDummyData();
	void inputDummyDataToDB();
};

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <winsock2.h> 
#include <deque>
#include <vector>
#include "stdafx.h"
#include "TopicNameTable.h"
#include "RequestTable.h"


using namespace std;
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

#define MESSAGE_ERROR_WSA_STARTUP	"WSAStartup() error"
#define MESSAGE_ERROR_WSA_SELECT	"WSAEventSelect() error"
#define MESSAGE_ERROR_BIND					"bind() error"
#define MESSAGE_ERROR_SOCKET				"socket() error"
#define MESSAGE_ERROR_LISTEN				"listen() error"

#define TOPIC_NAME_FULL							"A/BB/CCC/DDDD/EEEEEE"
#define TOPIC_NAME_1								"A"
#define TOPIC_NAME_2								"BB"
#define TOPIC_NAME_3								"CCC"
#define TOPIC_NAME_4								"DDDD"
#define TOPIC_NAME_5								"EEEEE"

class TCPSocket
{
public:
	TCPSocket();
	TopicNameTable*			TNTable;
	RequestTable *				RTable;
	deque<PDD_NODE>		recvQueue;

private:
	//상수 집함
	static const int FirstIndex = 0;

	// 변수 선언	
	WSADATA						 wsaData;
	SOCKET							 hServSock;	//서버 소켓
	SOCKADDR_IN				 servAddr;	//서버 주소

	SOCKET							 hSockArray[WSA_MAXIMUM_WAIT_EVENTS]; //소켓 핸들배열 - 연결 요청이 들어올 때마다 생성되는 소켓의 핸들을 이 배열에 저장. (최대64)
	SOCKET							 hClntSock;	//클라이언트 소켓
	SOCKADDR_IN				 clntAddr;	//클라이언트 주소

	WSAEVENT						 hEventArray[WSA_MAXIMUM_WAIT_EVENTS];	// 이벤트 배열 
	WSAEVENT						 newEvent;
	WSANETWORKEVENTS	 netEvents;

	int		clntLen;
	int		sockTotal;
	int		index, i;
	char		message[BUFSIZE];
	int		strLen;
	
	//구현 함수
	void		inputDummyEntryToTNTable();

public:
	int		StartServer();
	void		ResetTCPSocket();
	void		Response();
	void		SaveRequests(IN_ADDR ip, PDD_NODE receiveData);
	void		initialize();
};


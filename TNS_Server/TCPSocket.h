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
	// ���� ����	
	WSADATA						 wsaData;
	SOCKET							 hServSock;	//���� ����
	SOCKADDR_IN				 servAddr;	//���� �ּ�

	SOCKET							 hSockArray[WSA_MAXIMUM_WAIT_EVENTS]; //���� �ڵ�迭 - ���� ��û�� ���� ������ �����Ǵ� ������ �ڵ��� �� �迭�� ����. (�ִ�64)
	SOCKET							 hClntSock;	//Ŭ���̾�Ʈ ����
	SOCKADDR_IN				 clntAddr;	//Ŭ���̾�Ʈ �ּ�

	WSAEVENT						 hEventArray[WSA_MAXIMUM_WAIT_EVENTS];	// �̺�Ʈ �迭 
	WSAEVENT						 newEvent;
	WSANETWORKEVENTS	 netEvents;

	int		clntLen;
	int		sockTotal;
	int		index, i;
	char		message[BUFSIZE];
	int		strLen;

	//void CompressSockets(SOCKET* hSockArray, int omitIndex, int total);
	//void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total);
	//void ErrorHandling(char *message);
	
	//���� �Լ�
	void		inputDummyEntryToTNTable();

public:
	int		StartServer();
	void		ResetTCPSocket();
	void		Response();
	void		SaveRequests(IN_ADDR ip, PDD_NODE receiveData);
	void		initialize();
};

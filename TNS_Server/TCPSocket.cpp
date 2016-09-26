#include <process.h>
#include "TCPSocket.h"
#include "TopicNameTable.h"

static UINT WINAPI Sending(LPVOID p);
static UINT WINAPI Receiving(LPVOID p);
static void CheckEntry(TCPSocket * tcpSocket, PDD_NODE entry, TN_ENTRY TE);
static SOCKET CreateSocket();
static void BindingSocket(SOCKET servSocket);
static void LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);

static void OpenConnection(vector<SOCKET> * hSockArray, vector<WSAEVENT> * hEventArray, int index, int sockTotal);
static void TransferData(LPVOID p, SOCKET * sockArray, int index);
static bool CloseConnection(WSANETWORKEVENTS netEvents, vector<SOCKET> * hSockArray, vector<WSAEVENT> * hEventArray, SOCKET * sockArray, WSAEVENT * eventArray, int index);
void ErrorHandling(char *message);

TCPSocket::TCPSocket()
{
	ResetTCPSocket();
}

int TCPSocket::StartServer()
{
	//Test Dummy Insert

	// 윈속 초기화 (성공시 0, 실패시 에러 코드리턴)
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling(MESSAGE_ERROR_WSA_STARTUP);
	}

	HANDLE recvThread = (HANDLE)_beginthreadex(NULL, 0, Receiving, (LPVOID)this, 0, NULL);
	HANDLE sendThread = (HANDLE)_beginthreadex(NULL, 0, Sending, (LPVOID)this, 0, NULL);

	this->initialize();
	TNTable->testShowAll();

	while (true) {}

	CloseHandle(recvThread);
	CloseHandle(sendThread);

	return 0;
}

void TCPSocket::initialize() {
	int	test_type;
	TN_ENTRY TE;

	printf("[ INITIALIZE TNS SERVER ]\n\n");

	printf("***** Select inital type *****\n");
	printf("[1] Create Transmission Topic\n");
	printf("[2] Input  Dummy        Topic\n");
	printf("[others] Exit\n");
	printf("******************************\n");

	printf("input>");
	scanf("%d", &test_type);
	
	if(test_type == 2)
		inputDummyEntryToTNTable();

	while (test_type == 1) {
		printf("[ INPUT TOPIC NAME SPACE ENTRY ]\n\n");
		printf("Input Topic >");
		scanf("%s", &TE.TN_TOPIC);

		printf("Input Token >");
		scanf("%s", &TE.TN_TOKEN);

		printf("Input Level of Token >");
		scanf("%d", &TE.TN_LEVEL);

		printf("Input Next Zone >");
		scanf("%s", &TE.TN_NEXTZONE);

		TNTable->addEntry(TE);
		TNTable->testShowAll();

		fflush(stdin);

		printf("***** Select inital type *****\n");
		printf("[1] Create Transmission Topic\n");
		printf("[others] Exit\n");
		printf("******************************\n");

		printf("input>");
		scanf("%d", &test_type);
	}

	printf("[ Complete Inital TNS SERVER ]\n\n");
}

void TCPSocket::ResetTCPSocket() {
	this->sockTotal = 0;
	this->RTable = new RequestTable();
	this->TNTable = new TopicNameTable();
	this->recvQueue.clear();
}

/************************************
/*
/*	ErrorHandling
/*
*/
//void TCPSocket::ErrorHandling(char *message)
void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

///////////Test Function
void TCPSocket::inputDummyEntryToTNTable() {
	TN_ENTRY TE;

	TE.TN_LEVEL = 1;
	memcpy(TE.TN_NEXTZONE, IP, ADDRESS_SIZE);

	memcpy(TE.TN_TOKEN, TOPIC_NAME_1, sizeof(TOPIC_NAME_1));
	memcpy(TE.TN_TOPIC, TOPIC_NAME_FULL, sizeof(TOPIC_NAME_FULL));
	TNTable->addEntry(TE);

	TE.TN_LEVEL = 2;
	memcpy(TE.TN_TOKEN, TOPIC_NAME_2, sizeof(TOPIC_NAME_2));
	TNTable->addEntry(TE);

	TE.TN_LEVEL = 3;
	memcpy(TE.TN_TOKEN, TOPIC_NAME_3, sizeof(TOPIC_NAME_3));
	TNTable->addEntry(TE);

	TE.TN_LEVEL = 4;
	memcpy(TE.TN_TOKEN, TOPIC_NAME_4, sizeof(TOPIC_NAME_4));
	TNTable->addEntry(TE);

	TE.TN_LEVEL = 5;
	memcpy(TE.TN_TOKEN, TOPIC_NAME_5, sizeof(TOPIC_NAME_5));
	//memcpy(TE.TN_NEXTZONE, "192.168.0.22", ADDRESS_SIZE);

	memcpy(TE.TN_NEXTZONE, IP, ADDRESS_SIZE);
	TNTable->addEntry(TE);
	TNTable->testShowAll();
}

void TCPSocket::Response() {
	TN_ENTRY TE;
	//저장된 RequestTable에서 꺼내와서 데이터 송신
	if (RTable->isRequestExist()) {
		PR_NODE PN = RTable->getLastEntry();
		PDD_NODE entry = PN->key.REQUEST_DATA;

		if (entry.PDD_HEADER.MESSAGE_TYPE == MESSAGE_TYPE_REQUEST) {
			memcpy(&TE.TN_LEVEL, entry.PDD_DATA[0].PARTICIPANT_DATA, sizeof(int));
			memcpy(TE.TN_TOPIC, entry.PDD_DATA[0].PARTICIPANT_TOPIC, MAX_CHAR);
			
			if (TNTable->isEntryExist(TE)) {
				TNTable->getEntry(&TE);
				memcpy(entry.PDD_DATA[0].PARTICIPANT_DATA, TE.TN_NEXTZONE, sizeof(TE.TN_NEXTZONE));
				entry.PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_RESPONSE;
			}
			else {
				entry.PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_NOTEXIST;
			}

			cout << "Response TOKEN :" << TE.TN_TOKEN << "/" << TE.TN_NEXTZONE << endl;
			cout << "Response ADDRESS :" << entry.PDD_DATA[0].PARTICIPANT_DATA << endl;
		}

		// 에코(데이터를준 클라이언트에 다시 데이터쏘기)
		//send(hSockArray[index - WSA_WAIT_EVENT_0], (char*)&entry, sizeof(entry), 0);

		cout << "=============================================" << endl;
	}
}

void TCPSocket::SaveRequests(IN_ADDR ip, PDD_NODE receiveData) {
	RTable->addEntry(ip, receiveData);
}

static UINT WINAPI Sending(LPVOID p) {
	TCPSocket *			tcpSocket = (TCPSocket*)p;
	SOCKET					clientSocket;
	SOCKADDR_IN		tempAddr;
	vector<string>			tokenArray;

	while (true) {
		Sleep(SLEEP_TIME_SHORT);
		if (tcpSocket->RTable->isRequestExist()) {
			TN_ENTRY TE;
			//저장된 RequestTable에서 꺼내와서 데이터 송신
			PR_NODE PN = tcpSocket->RTable->getLastEntry();
			PDD_NODE entry = PN->key.REQUEST_DATA;

			//메세지 타입 REQUEST일 경우
			if (entry.PDD_HEADER.MESSAGE_TYPE == MESSAGE_TYPE_REQUEST) {
				tokenArray = tcpSocket->TNTable->splitTopic(entry.PDD_DATA[FIRST_INDEX].PARTICIPANT_TOPIC);

				//수신 메시지 출력
				cout << "Request MSG" << endl;
				cout << "Request Topic :" << entry.PDD_DATA[FIRST_INDEX].PARTICIPANT_TOPIC << endl;
				cout << "Request TokenLevel :" << entry.PDD_DATA[FIRST_INDEX].PARTICIPANT_DATA << endl;
				cout << "Request TYPE :" << entry.PDD_HEADER.MESSAGE_TYPE << endl;

				TE.TN_LEVEL = atoi(entry.PDD_DATA[FIRST_INDEX].PARTICIPANT_DATA);

				memcpy(TE.TN_TOPIC, entry.PDD_DATA[FIRST_INDEX].PARTICIPANT_TOPIC, MAX_CHAR);
				memcpy(TE.TN_TOKEN, tokenArray.at(TE.TN_LEVEL - 1).c_str(), MAX_CHAR);

				//Entry존재여부에 따라 Message Type 변경해줌
				CheckEntry(tcpSocket, entry, TE);
			}

			clientSocket = socket(PF_INET, SOCK_STREAM, 0);

			if (clientSocket == INVALID_SOCKET) {
				ErrorHandling("clientSocket() error");
			}

			memset(&tempAddr, FIRST_INDEX, sizeof(tempAddr));
			tempAddr.sin_family = AF_INET;
			tempAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(PN->key.REQUEST_IP));
			tempAddr.sin_port = htons(FES_PORT);

			cout << inet_ntoa(PN->key.REQUEST_IP) << endl;

			if (connect(clientSocket, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR) {
				ErrorHandling("connect() error!");
			}

			send(clientSocket, (char*)&entry, sizeof(entry), 0);

			closesocket(clientSocket);

			cout << "Send" << endl;
			cout << "========================" << endl;
		}
	}
}

static void CheckEntry(TCPSocket * tcpSocket, PDD_NODE entry, TN_ENTRY TE) {
	if (tcpSocket->TNTable->isEntryExist(TE)) {
		tcpSocket->TNTable->getEntry(&TE);
		memcpy(entry.PDD_DATA[FIRST_INDEX].PARTICIPANT_DATA, TE.TN_NEXTZONE, sizeof(TE.TN_NEXTZONE));
		entry.PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_RESPONSE;
		printf("%s", entry.PDD_DATA[FIRST_INDEX].PARTICIPANT_DATA);

		cout << "RESPONSE" << endl;
	}
	else {
		entry.PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_NOTEXIST;
		cout << "NOT EXIST" << endl;
	}
}


static SOCKET CreateSocket() {
	// 소켓 생성 (성공시 핸들을, 실패시 "INVALID_SOCKET" 반환)
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, 0);

	// 소켓 생성 실패 처리
	if (servSock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	return servSock;
}

static void BindingSocket(SOCKET servSocket){
	// 소켓 통신을 위한 기본 정보
	SOCKADDR_IN servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(TNS_PORT);

	// 주소와 Port 할당 (바인드!!)
	if (bind(servSocket, (struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}

}
static void LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray) {
//static void LinkingEvents(SOCKET servSock, int* sockNum, SOCKET sockArray[WSA_MAXIMUM_WAIT_EVENTS], WSAEVENT eventArray[WSA_MAXIMUM_WAIT_EVENTS]) {
	// 이벤트 발생을 확인 (성공시 0, 실패시 "SOCKET_ERROR" 반환)
	vector<SOCKET>::iterator sVec_it;
	vector<WSAEVENT>::iterator eVec_it;

	WSAEVENT newEvent = WSACreateEvent();
	if (WSAEventSelect(servSock, newEvent, FD_ACCEPT) == SOCKET_ERROR) {
		ErrorHandling("WSAEventSelect() error");
	}


	// 연결 대기 요청 상태로의 진입 (신호가 들어올때까지 대기)
	if (listen(servSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}
	
	sVec_it = sockArray->begin() + *sockNum;
	// 서버 소켓 핸들 정보
	sockArray->insert(sVec_it, servSock);

	eVec_it = eventArray->begin() + *sockNum;
	// 이벤트 오브젝트 핸들 정보
	eventArray->insert(eVec_it, newEvent);

	(*sockNum)++;
}

static UINT WINAPI Receiving(LPVOID p) {
	WSADATA wsaData;
	SOCKET hServSock;
	WSANETWORKEVENTS netEvents;

	vector<SOCKET> hSockArray; //소켓 핸들배열 - 연결 요청이 들어올 때마다 생성되는 소켓의 핸들을 이 배열에 저장. (최대64)
	vector<WSAEVENT> hEventArray; //이벤트 벡터

	SOCKET * sockArray;
	WSAEVENT * eventArray;

	int sockTotal = 0;
	int index, i;
	char message[BUFSIZE];
	int strLen;

	//소켓 연결
	hServSock = CreateSocket();
	BindingSocket(hServSock);
	LinkingEvents(hServSock, &sockTotal, &hSockArray, &hEventArray);

	// 루프
	while (true) {
		sockArray = &hSockArray[FIRST_INDEX];
		eventArray = &hEventArray[FIRST_INDEX];

		// 이벤트 종류 구분하기(WSAWaitForMultipleEvents)
		index = WSAWaitForMultipleEvents(sockTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		index = index - WSA_WAIT_EVENT_0;

		for (i = index; i < sockTotal; i++) {
			index = WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, FIRST_INDEX, FALSE);

			if ((index == WSA_WAIT_FAILED || index == WSA_WAIT_TIMEOUT)) {
				continue;
			}
			else {
				index = i;
				WSAEnumNetworkEvents(sockArray[index], eventArray[index], &netEvents);

				if (netEvents.lNetworkEvents & FD_ACCEPT) { // 초기 연결 요청의 경우.
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0) {
						puts("Accept Error");
						break;
					}
					OpenConnection(&hSockArray, &hEventArray, index, sockTotal);
					sockTotal++;

				}
				else if (netEvents.lNetworkEvents & FD_READ) { // 데이터 전송해올 경우.
					if (netEvents.iErrorCode[FD_READ_BIT] != 0) {
						puts("Read Error");
						break;
					}
					TransferData(p, sockArray, index);
				}
				else if (netEvents.lNetworkEvents & FD_CLOSE) { 	// 연결 종료 요청의 경우.
					if (!CloseConnection(netEvents, &hSockArray, &hEventArray, sockArray, eventArray, index)) {
						break;
					}
					sockTotal--;
				}
			}//else
		}//for
	}//while

	 // 할당 받은 리소스 반환.
	WSACleanup();

	return 0;
}

static void OpenConnection(vector<SOCKET> * hSockArray, vector<WSAEVENT> * hEventArray, int index, int sockTotal) {
	WSAEVENT newEvent;

	vector<SOCKET>::iterator sVec_it;
	vector<WSAEVENT>::iterator eVec_it;

	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;

	int clntLen;

	clntLen = sizeof(clntAddr);

	// 연결을 수락 (accept | 성공시 소켓핸들, 실패시 "INVALID_SOCKET" 반환)
	hClntSock = accept(hSockArray->at(index), (SOCKADDR*)&clntAddr, &clntLen);

	// 이벤트 커널 오브젝트 생성(WSACreateEvent)
	newEvent = WSACreateEvent();

	// 이벤트 발생 유무 확인(WSAEventSelect)
	WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

	sVec_it = hSockArray->begin() + sockTotal;
	eVec_it = hEventArray->begin() + sockTotal;

	hSockArray->insert(sVec_it, hClntSock);
	hEventArray->insert(eVec_it, newEvent);

	printf("새로 연결된 소켓의 핸들 %d \n", hClntSock);
	printf("vector size = %d\n", hSockArray->size());
	printf("array  size : %d\n", sockTotal);
}

static void TransferData(LPVOID p, SOCKET * sockArray, int index) {
	TCPSocket * tcpSocket = (TCPSocket*)p;
	// 데이터를 받음 (message에 받은 데이터를 담음)
	PDD_NODE	receiveData;
	struct sockaddr_in name;
	int len = sizeof(name);
	int strLen;

	strLen = recv(sockArray[index - WSA_WAIT_EVENT_0], (char*)&receiveData, sizeof(PDD_NODE), 0);

	if (getpeername(sockArray[index - WSA_WAIT_EVENT_0], (struct sockaddr *)&name, &len) != 0) {
		perror("getpeername Error");
	}

	if (strLen != -1) {
		//RequestTable에 일단 저장
		tcpSocket->SaveRequests(name.sin_addr, receiveData);
	}
}

static bool CloseConnection(WSANETWORKEVENTS netEvents, vector<SOCKET> * hSockArray, vector<WSAEVENT> * hEventArray, SOCKET * sockArray, WSAEVENT * eventArray, int index) {

	vector<SOCKET>::iterator sVec_it;
	vector<WSAEVENT>::iterator eVec_it;

	if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0) {
		puts("Close Error");
		return false;
	}

	WSACloseEvent(eventArray[index]);

	// 소켓 종류
	closesocket(sockArray[index]);
	printf("종료 된 소켓의 핸들 %d \n", sockArray[index]);

	// 배열 정리.
	printf("삭제 : %d\n", index);
	sVec_it = hSockArray->begin() + index;
	hSockArray->erase(sVec_it);

	eVec_it = hEventArray->begin() + index;
	hEventArray->erase(eVec_it);
}
#include <process.h>
#include "TCPSocket.h"
#include "RequestTable.h"

static UINT WINAPI receiving(LPVOID p);
static UINT WINAPI storing(LPVOID p);
void ErrorHandling(char *message);

TCPSocket::TCPSocket()
{
	ResetTCPSocket();
}

int TCPSocket::StartServer()
{
	// 윈속 초기화 (성공시 0, 실패시 에러 코드리턴)
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	initialize();

	puts("Front-End Server Start");
	this->TNSPTable->testShowAll();
	
	HANDLE recvThread = (HANDLE)_beginthreadex(NULL, 0, receiving, (LPVOID)this, 0, NULL);
	HANDLE saveThread = (HANDLE)_beginthreadex(NULL, 0, storing, (LPVOID)this, 0, NULL);

	while (1) {}

	CloseHandle(recvThread);
	CloseHandle(saveThread);

	return 0;
}


void TCPSocket::ResetTCPSocket() {
	this->sockTotal = 0;
	this->RTable = new RequestTable();
	this->TNSPTable = new TopicNameSpaceTable();
}

void TCPSocket::inputDummy() {
	//this->TNSPTable->addEntry("Q/WW/EEE/RRRR/TTTTTT", "DDS_3",
	//	"127.0.0.1", "127.0.0.10", 1000, "DDS_TEST_DATA3", NULL, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
	
	this->TNSPTable->addEntry("A/BB/CCC/DDDD/EEEEEE", "DDS_1",
			"127.0.0.1", "127.0.0.20", 70, "CCCCCCC", NULL, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
}


void TCPSocket::initialize() {
	int	test_type;
	char topic[MAX_CHAR];
	char domain[MAX_CHAR];
	char nextzone[ADDRESS_SIZE];
	char participantip[ADDRESS_SIZE];
	int participantport;
	char data[MAX_DATA_SIZE];
	int pubsub;
	int messageType;

	printf("[ INITIALIZE Front-End SERVER ]\n\n");

	printf("***** Select inital type *****\n");
	printf("[1] Create Transmission Topic\n");
	printf("[2] Input  Dummy        Topic\n");
	printf("[3] Run	   Front-End	Server\n");
	printf("[others] Exit\n");
	printf("******************************\n");

	printf("input>");
	scanf("%d", &test_type);

	if (test_type == 2)
		inputDummy();
	else if (test_type == 3)
		return;

	while (test_type == 1) {

		printf("input Topic>");
		scanf("%s", topic);

		printf("input Domain>");
		scanf("%s", domain);

		printf("input Next Zone IP>");
		scanf("%s", nextzone);

		printf("input Participant IP>");
		scanf("%s", participantip);

		printf("input Participant Port>");
		scanf("%d", &participantport);

		printf("input Sample Data>");
		scanf("%s", data);

		printf("input Pub/Sub Type (100 : PUB / 200 : SUB)>");
		scanf("%d", &pubsub);

		printf("input Message Type (10 : SAVE / 11 : REMOVE / 12 : MODIFY)>");
		scanf("%d", &messageType);

		this->TNSPTable->addEntry(topic, domain,
			nextzone, participantip, participantport, data, NULL, pubsub, messageType);

		printf("***** Select inital type *****\n");
		printf("[1] Create Transmission Topic\n");
		printf("[2] Input  Dummy        Topic\n");
		printf("[3] Run	   Front-End	Server\n");
		printf("[others] Exit\n");
		printf("******************************\n");

		printf("input>");
		scanf("%d", &test_type);
	}

	printf("[ Complete Inital Front-End SERVER ]\n\n");
}


/************************************
/*
/*	CompressSockets
/*
*/
//void TCPSocket::CompressSockets(SOCKET* hSockArray, int omitIndex, int total)
void CompressSockets(SOCKET* hSockArray, int omitIndex, int total)
{
	int i;

	for (i = omitIndex; i<total; i++)
	{
		hSockArray[i] = hSockArray[i + 1];
	}
}

/************************************
/*
/*	CompressEvents
/*
*/
//void TCPSocket::CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total)
void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total)
{
	int i;

	for (i = omitIndex; i<total; i++)
	{
		hEventArray[i] = hEventArray[i + 1];
	}
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

static UINT WINAPI storing(LPVOID p) {
	TCPSocket *		tcpSocket = (TCPSocket*)p;
	PTNSP_NODE		PTNSP;
	TNSN_ENTRY		TNSNDatagram;
	SOCKADDR_IN		tempAddr;

	SOCKET hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
		ErrorHandling("hSocket() error");

	while (1) {
		Sleep(10);
		//세팅된 데이터 전송
		if (tcpSocket->TNSPTable->isEntryExist(STATE_SET)) {
			PTNSP = tcpSocket->TNSPTable->getEntry(STATE_SET);
			
			{
				SOCKET clientSocket = socket(PF_INET, SOCK_STREAM, 0);

				if (clientSocket == INVALID_SOCKET)
					ErrorHandling("clientSocket() error");

				PTNSP->key.TN_SPACE_CURRENT_SOCKET = clientSocket;
			}


			cout << "====================" << endl;
			cout << "SEND MSG" << endl;
			cout << "SEND Topic :" << PTNSP->key.TN_SPACE_TOPIC << endl;
			cout << "SEND Token :" << PTNSP->key.TN_SPACE_TOKEN << endl;
			cout << "SEND TokenLevel :" << PTNSP->key.TN_SPACE_CURRENT_LEVEL << " / " << PTNSP->key.TN_SPACE_TOTAL_LEVEL << endl;

			memset(&tempAddr, 0, sizeof(tempAddr));
			tempAddr.sin_family = AF_INET;
			tempAddr.sin_addr.S_un.S_addr = inet_addr(PTNSP->key.TN_SPACE_NEXTZONE_ADDR);
			
			if (PTNSP->key.TN_SPACE_TOTAL_LEVEL < PTNSP->key.TN_SPACE_CURRENT_LEVEL) {
				tempAddr.sin_port = htons(TERMINAL_PORT);
				TNSNDatagram.TNSN_MESSAGETYPE = PTNSP->key.TN_SPACE_MESSAGETYPE;
				strcpy(TNSNDatagram.TNSN_DATA, PTNSP->key.TN_SPACE_DATA);
			}
			else {
				tempAddr.sin_port = htons(TNS_PORT);
				TNSNDatagram.TNSN_MESSAGETYPE = MESSAGE_TYPE_REQUEST;
			}

			if (PTNSP->key.TN_SPACE_NODETYPE == NODE_TYPE_PUB)
				TNSNDatagram.TNSN_NODETYPE = NODE_TYPE_PUB;
			else
				TNSNDatagram.TNSN_NODETYPE = NODE_TYPE_SUB;

			TNSNDatagram.TNSN_ID = PTNSP->key.TN_SPACE_ID;
			memcpy(TNSNDatagram.TNSN_DOMAIN, PTNSP->key.TN_SPACE_DOMAIN, sizeof(PTNSP->key.TN_SPACE_DOMAIN));
			memcpy(TNSNDatagram.TNSN_TOPIC, PTNSP->key.TN_SPACE_TOPIC, sizeof(PTNSP->key.TN_SPACE_TOPIC));
			memcpy(TNSNDatagram.TNSN_TOKEN, PTNSP->key.TN_SPACE_TOKEN, sizeof(PTNSP->key.TN_SPACE_TOKEN));
			memcpy(TNSNDatagram.TNSN_PARTICIPANT_ADDR, PTNSP->key.TN_SPACE_PARTICIPANT_ADDR, ADDRESS_SIZE);
			TNSNDatagram.TNSN_TOKENLEVEL = PTNSP->key.TN_SPACE_CURRENT_LEVEL;
			TNSNDatagram.TNSN_PARTICIPANT_PORT = PTNSP->key.TN_SPACE_PARTICIPANT_PORT;

			cout << "SEND TYPE :" << TNSNDatagram.TNSN_MESSAGETYPE << endl;
			cout << "====================" << endl;

			if (connect(PTNSP->key.TN_SPACE_CURRENT_SOCKET, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR)
				ErrorHandling("connect() error!");
			else {
				send(PTNSP->key.TN_SPACE_CURRENT_SOCKET, (char*)&TNSNDatagram, sizeof(TNSNDatagram), 0);
				PTNSP->key.TN_SPACE_STATE = STATE_RESPONSE;
				closesocket(PTNSP->key.TN_SPACE_CURRENT_SOCKET);
			}
		}

		//Response Data, SaveDone Data 처리
		if (tcpSocket->RTable->isRequestExist()) {
			PR_NODE PRN = tcpSocket->RTable->getLastEntry();
			TNSN_ENTRY entry = PRN->key.REQUEST_DATA;

			//T_ENTRY TD;
			if (entry.TNSN_MESSAGETYPE == MESSAGE_TYPE_RESPONSE) {
				cout << "====================" << endl;
				cout << "RECEIVE MSG" << endl;
				cout << "RECEIVE Topic :" << entry.TNSN_TOPIC << endl;
				cout << "RECEIVE Token :" << entry.TNSN_TOKEN << endl;
				cout << "RECEIVE TokenLevel :" << entry.TNSN_TOKENLEVEL << endl;
				cout << "RECEIVE TYPE :" << entry.TNSN_MESSAGETYPE << endl;
				cout << "RECEIVE DATA :" << entry.TNSN_DATA << endl;
				cout << "====================" << endl;


				tcpSocket->TNSPTable->setEntry(entry.TNSN_ID, entry.TNSN_DATA, STATE_SET);

				//closesocket(hSocket);
			}
			else if (entry.TNSN_MESSAGETYPE == MESSAGE_TYPE_SAVEDONE) {
				puts("SAVE Complete");
			}
			else {
				puts("ERROR MSG");
			}
		}
	}
}

void TCPSocket::SaveRequests(SOCKET rSocket, TNSN_ENTRY tnsData) {
	RTable->addEntry(rSocket, tnsData);
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

static void BindingSocket(SOCKET servSocket) {
	// 소켓 통신을 위한 기본 정보
	SOCKADDR_IN servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(PORT);

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

static UINT WINAPI receiving(LPVOID p) {
	TCPSocket * tcpSocket = (TCPSocket*)p;

	TNSN_ENTRY	TNSNDatagram;

	WSADATA wsaData;
	SOCKET hServSock;

	//SOCKET hSockArray[WSA_MAXIMUM_WAIT_EVENTS];
	vector<SOCKET> hSockArray; //소켓 핸들배열 - 연결 요청이 들어올 때마다 생성되는 소켓의 핸들을 이 배열에 저장. (최대64)
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;

	//WSAEVENT hEventArray[WSA_MAXIMUM_WAIT_EVENTS];	// 이벤트 배열
	vector<WSAEVENT> hEventArray;
	WSAEVENT newEvent;
	WSANETWORKEVENTS netEvents;


	int clntLen;
	int sockTotal = 0;
	int index, i;
	char message[BUFSIZE];
	int strLen;

	hServSock = CreateSocket();

	BindingSocket(hServSock);

	LinkingEvents(hServSock, &sockTotal, &hSockArray, &hEventArray);


	SOCKET * sockArray;
	WSAEVENT * eventArray;
	vector<SOCKET>::iterator sVec_it;
	vector<WSAEVENT>::iterator eVec_it;

	// 루프
	while (1)
	{
		sockArray = &hSockArray[0];
		eventArray = &hEventArray[0];

		// 이벤트 종류 구분하기(WSAWaitForMultipleEvents)
		index = WSAWaitForMultipleEvents(sockTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		index = index - WSA_WAIT_EVENT_0;

		for (i = index; i < sockTotal; i++)
		{
			index = WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, 0, FALSE);

			if ((index == WSA_WAIT_FAILED || index == WSA_WAIT_TIMEOUT)) continue;
			else
			{
				index = i;
				WSAEnumNetworkEvents(sockArray[index], eventArray[index], &netEvents);


				// 초기 연결 요청의 경우.
				if (netEvents.lNetworkEvents & FD_ACCEPT)
				{
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
					{
						puts("Accept Error");
						break;
					}

					clntLen = sizeof(clntAddr);

					// 연결을 수락 (accept | 성공시 소켓핸들 실패시 "INVALID_SOCKET" 반환)
					hClntSock = accept(hSockArray.at(index), (SOCKADDR*)&clntAddr, &clntLen);

					// 이벤트 커널 오브젝트 생성(WSACreateEvent)
					newEvent = WSACreateEvent();

					// 이벤트 발생 유무 확인(WSAEventSelect)
					WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);


					sVec_it = hSockArray.begin() + sockTotal;
					eVec_it = hEventArray.begin() + sockTotal;

					hSockArray.insert(sVec_it, hClntSock);
					hEventArray.insert(eVec_it, newEvent);
					//eventArray[sockTotal] = newEvent;
					//sockArray[sockTotal] = hClntSock;
					sockTotal++;

					printf("새로 연결된 소켓의 핸들 %d \n", hClntSock);
					printf("vector size = %d\n", hSockArray.size());
					printf("array  size : %d\n", sockTotal);
				}


				// 데이터 전송해올 경우.
				if (netEvents.lNetworkEvents & FD_READ)
				{
					if (netEvents.iErrorCode[FD_READ_BIT] != 0)
					{
						puts("Read Error");
						break;
					}

					//////////////////////////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////////////////
					//
					//		서버 작업은 여기서 다하겠지..
					//
					// 데이터를 받음 (message에 받은 데이터를 담음)
					strLen = recv(sockArray[index - WSA_WAIT_EVENT_0], (char*)&TNSNDatagram, sizeof(TNSNDatagram), 0);

					if (strLen != -1) {
						//RequestTable에 일단 저장
						tcpSocket->SaveRequests(sockArray[index - WSA_WAIT_EVENT_0], TNSNDatagram);
						//Response();
					}

					//////////////////////////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////////////////////////////
				}


				// 연결 종료 요청의 경우.
				if (netEvents.lNetworkEvents & FD_CLOSE)
				{
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
					{
						puts("Close Error");
						break;
					}

					WSACloseEvent(eventArray[index]);

					// 소켓 종류
					closesocket(sockArray[index]);
					printf("종료 된 소켓의 핸들 %d \n", sockArray[index]);

					sockTotal--;

					// 배열 정리.
					printf("삭제 : %d\n", index);
					sVec_it = hSockArray.begin() + index;
					hSockArray.erase(sVec_it);

					eVec_it = hEventArray.begin() + index;
					hEventArray.erase(eVec_it);

					//CompressSockets(hSockArray, index, sockTotal);
					//CompressEvents(hEventArray, index, sockTotal);
				}

			}//else
		}//for
	}//while

	 // 할당 받은 리소스 반환.
	WSACleanup();

	return 0;
}

///////////////////////////////////
/*

#include "TCPSocket.h"

TCPSocket::TCPSocket()
{
}

int TCPSocket::StartServer()
{
	WSADATA wsaData;
	SOCKET hSocket;
	TP = new TopicParser();

	RequestsTable *RT = new RequestsTable();
	SOCKADDR_IN    tempAddr;

	char message[30];
	int    strLen;
	SOCKADDR_IN    servAddr;
	char			message2[] = "Hello World\n";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("hSocket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr(IP);
	servAddr.sin_port = htons(TNS_PORT);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");



	vector<string> x = TP->split("A/BB/CCC/DDDD/EEEEEE");

	RequestsTable::R_NODE *PE = new RequestsTable::R_NODE();
	PE->key.currentLevel = 1;
	PE->key.currentSocket = NULL;
	PE->key.currentState = 0;
	PE->key.totalLevel = x.size();
	memset(PE->key.data, 0, sizeof(PE->key.data));
	PE->key.nextSocket = hSocket;
	PE->key.state = 0;
	memcpy(PE->key.topic, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	memcpy(PE->key.token, x.at(0).c_str(), sizeof(x.at(0).c_str()));

	RT->addEntry(PE->key);
	RT->test_showAllEntry();


	for (int i = 0; i < 1; i++) {
		//cout << x.at(i) << endl;
		//cout << x.at(i).c_str() << endl;
		TNSNDatagram.TNSN_MESSAGETYPE = MESSAGE_TYPE_REQUEST;
		TNSNDatagram.TNSN_ID = 700;
		TNSNDatagram.TNSN_NODETYPE = NODE_TYPE_PUB;
		memcpy(TNSNDatagram.TNSN_TOPIC, PE->key.topic, sizeof(PE->key.topic));
		memcpy(TNSNDatagram.TNSN_TOKEN, x.at(i).c_str(), sizeof(x.at(i).c_str()));
		TNSNDatagram.TNSN_TOKENLEVEL = PE->key.currentLevel;

		cout << "Send MSG" << endl;
		cout << "Send Topic :" << TNSNDatagram.TNSN_TOPIC << endl;
		cout << "Send Token :" << TNSNDatagram.TNSN_TOKEN << endl;
		cout << "Send TYPE :" << TNSNDatagram.TNSN_MESSAGETYPE << endl;

		send(hSocket, (char*)&TNSNDatagram, sizeof(TNSNDatagram), 0);
	}

	while (true) {
		//메시지 수신시 일단 출력
		strLen = recv(hSocket, (char*)&TNSNDatagram, sizeof(TNSNDatagram), 0);

		if (strLen == -1)
			ErrorHandling("read() error!");

		//message[strLen] = 0;
		printf("\nMessage from server : %d \n", TNSNDatagram.TNSN_MESSAGETYPE);
		printf("Message from server : %s \n", TNSNDatagram.TNSN_TOKEN);
		printf("Message from server : %s \n", TNSNDatagram.TNSN_DATA);

		if (TNSNDatagram.TNSN_MESSAGETYPE == MESSAGE_TYPE_SAVEDONE)
			break;
		else if (TNSNDatagram.TNSN_MESSAGETYPE == MESSAGE_TYPE_RESPONSE) {
			{
				closesocket(hSocket);
				PE->key.currentLevel++;

				if(PE->key.currentLevel > PE->key.totalLevel)
					memcpy(PE->key.token, x.at(PE->key.currentLevel - 2).c_str(), sizeof(x.at(PE->key.currentLevel - 2)));
				else 
					memcpy(PE->key.token, x.at(PE->key.currentLevel - 1).c_str(), sizeof(x.at(PE->key.currentLevel - 1)));
				
				PE->key.nextSocket = socket(PF_INET, SOCK_STREAM, 0);

				if (PE->key.nextSocket == INVALID_SOCKET)
					ErrorHandling("nextSocket error");

				memset(&tempAddr, 0, sizeof(tempAddr));
				tempAddr.sin_family = AF_INET;
				tempAddr.sin_addr.S_un.S_addr = inet_addr(TNSNDatagram.TNSN_DATA);
				tempAddr.sin_port = htons(TERMINAL_PORT);

			}

			if (PE->key.currentLevel > PE->key.totalLevel) {
				TNSNDatagram.TNSN_MESSAGETYPE = MESSAGE_TYPE_SAVE;
				tempAddr.sin_port = htons(TERMINAL_PORT);
			}
			else {
				TNSNDatagram.TNSN_MESSAGETYPE = MESSAGE_TYPE_REQUEST;
				tempAddr.sin_port = htons(TNS_PORT);
			}

			{
				PE->key.currentSocket = PE->key.nextSocket;
				PE->key.nextSocket = NULL;
				if (connect(PE->key.currentSocket, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR)
					ErrorHandling("connect() error!");
				PE->key.state = 2;

				if (PE->key.currentLevel > PE->key.totalLevel)
					TNSNDatagram.TNSN_TOKENLEVEL = PE->key.currentLevel - 1;
				else
					TNSNDatagram.TNSN_TOKENLEVEL = PE->key.currentLevel;

				memcpy(TNSNDatagram.TNSN_TOKEN, PE->key.token, sizeof(PE->key.token));


				cout << "Send MSG" << endl;
				cout << "Send Topic :" << TNSNDatagram.TNSN_TOPIC << endl;
				cout << "Send Token :" << TNSNDatagram.TNSN_TOKEN << endl;
				cout << "Send TYPE :" << TNSNDatagram.TNSN_MESSAGETYPE << endl;
				cout << "current Level :" << PE->key.currentLevel << endl;

				send(hSocket, (char*)&TNSNDatagram, sizeof(TNSNDatagram), 0);

				cout << "=============================================" << endl;
			}
		}
		else {
			puts("ERROR!!!");
		}
	}

	puts("COMPLETE");


	closesocket(hSocket);
	WSACleanup();
	return 0;
}
*/
#include <process.h>
#include "TCPSocket.h"
#include "TopicNameTable.h"

static UINT WINAPI sending(LPVOID p);
static UINT WINAPI receiving(LPVOID p);
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
		ErrorHandling("WSAStartup() error!");
	}

	HANDLE recvThread = (HANDLE)_beginthreadex(NULL, 0, receiving, (LPVOID)this, 0, NULL);
	HANDLE sendThread = (HANDLE)_beginthreadex(NULL, 0, sending, (LPVOID)this, 0, NULL);

	this->initialize();
	TNTable->TestShowAll();

	while (1) {}

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

		TNTable->AddEntry(TE);
		TNTable->TestShowAll();

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



///////////Test Function
void TCPSocket::inputDummyEntryToTNTable() {
	TN_ENTRY TE;
	TE.TN_LEVEL = 1;
	memcpy(TE.TN_NEXTZONE, "127.0.0.1", ADDRESS_SIZE);
	memcpy(TE.TN_TOKEN, "A", sizeof("A"));
	memcpy(TE.TN_TOPIC, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	TNTable->AddEntry(TE);
	TE.TN_LEVEL = 2;
	memcpy(TE.TN_TOKEN, "BB", sizeof("BB"));
	TNTable->AddEntry(TE);
	TE.TN_LEVEL = 3;
	memcpy(TE.TN_TOKEN, "CCC", sizeof("CCC"));
	TNTable->AddEntry(TE);
	TE.TN_LEVEL = 4;
	memcpy(TE.TN_TOKEN, "DDDD", sizeof("DDDD"));
	TNTable->AddEntry(TE);
	TE.TN_LEVEL = 5;
	memcpy(TE.TN_TOKEN, "EEEEEE", sizeof("EEEEEE"));
	memcpy(TE.TN_NEXTZONE, "127.0.0.1", ADDRESS_SIZE);
	//memcpy(TE.TN_NEXTZONE, "192.168.0.22", ADDRESS_SIZE);
	TNTable->AddEntry(TE);
	TNTable->TestShowAll();
}

void TCPSocket::Response() {
	TN_ENTRY TE;
	//저장된 RequestTable에서 꺼내와서 데이터 송신
	if (RTable->isRequestExist()) {
		PR_NODE PN = RTable->getLastEntry();
		PDD_NODE entry = PN->key.REQUEST_DATA;

		if (entry.PDD_HEADER.MESSAGE_TYPE == MESSAGE_TYPE_REQUEST) {
			//수신 메시지 출력
			//cout << "Request MSG" << endl;
			//cout << "Request Topic :" << entry.TNSN_TOPIC << endl;
			//cout << "Request Token :" << entry.TNSN_TOKEN << endl;
			//cout << "Request TokenLevel :" << entry.TNSN_TOKENLEVEL << endl;
			//cout << "Request TYPE :" << entry.TNSN_DATATYPE << endl;
			memcpy(&TE.TN_LEVEL, entry.PDD_DATA[0].PARTICIPANT_DATA, sizeof(int));

			memcpy(TE.TN_TOPIC, entry.PDD_DATA[0].PARTICIPANT_TOPIC, MAX_CHAR);
			
			if (TNTable->IsEntryExist(TE)) {
				TNTable->GetEntry(&TE);
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


void TCPSocket::SaveRequests(IN_ADDR ip, PDD_NODE receiveData) {
	RTable->AddEntry(ip, receiveData);
}

static UINT WINAPI receiving(LPVOID p) {
	TCPSocket * tcpSocket = (TCPSocket*)p;

	PDD_NODE	receiveData;

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

	struct sockaddr_in name;
	int len = sizeof(name);

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
					strLen = recv(sockArray[index - WSA_WAIT_EVENT_0], (char*)&receiveData, sizeof(PDD_NODE), 0);

					if (getpeername(sockArray[index - WSA_WAIT_EVENT_0], (struct sockaddr *)&name, &len) != 0) {
						perror("getpeername Error");
					}

					//cout << inet_ntoa(name.sin_addr) << endl;
					//cout << sizeof(inet_ntoa(name.sin_addr)) << endl;					

					if (strLen != -1) {
						//RequestTable에 일단 저장
						tcpSocket->SaveRequests(name.sin_addr, receiveData);
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

static UINT WINAPI sending(LPVOID p) {
	TCPSocket *			tcpSocket = (TCPSocket*)p;
	SOCKET				clientSocket;
	SOCKADDR_IN			tempAddr;
	vector<string>		tokenArray;

	while (1) {
		Sleep(10);
		if (tcpSocket->RTable->isRequestExist()) {
			TN_ENTRY TE;
			//저장된 RequestTable에서 꺼내와서 데이터 송신
			PR_NODE PN = tcpSocket->RTable->getLastEntry();
			PDD_NODE entry = PN->key.REQUEST_DATA;

			if (entry.PDD_HEADER.MESSAGE_TYPE == MESSAGE_TYPE_REQUEST) {
				tokenArray = tcpSocket->TNTable->SplitTopic(entry.PDD_DATA[0].PARTICIPANT_TOPIC);
				//수신 메시지 출력
				
				cout << "Request MSG" << endl;
				cout << "Request Topic :" << entry.PDD_DATA[0].PARTICIPANT_TOPIC << endl;
				cout << "Request TokenLevel :" << entry.PDD_DATA[0].PARTICIPANT_DATA << endl;
				cout << "Request TYPE :" << entry.PDD_HEADER.MESSAGE_TYPE << endl;

				TE.TN_LEVEL = atoi(entry.PDD_DATA[0].PARTICIPANT_DATA);
				
				memcpy(TE.TN_TOPIC, entry.PDD_DATA[0].PARTICIPANT_TOPIC, MAX_CHAR);
				memcpy(TE.TN_TOKEN, tokenArray.at(TE.TN_LEVEL - 1).c_str(), MAX_CHAR);

				if (tcpSocket->TNTable->IsEntryExist(TE)) {
					tcpSocket->TNTable->GetEntry(&TE);
					memcpy(entry.PDD_DATA[0].PARTICIPANT_DATA, TE.TN_NEXTZONE, sizeof(TE.TN_NEXTZONE));
					entry.PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_RESPONSE;
					printf("%s", entry.PDD_DATA[0].PARTICIPANT_DATA);

					cout << "RESPONSE" << endl;
				}
				else {
					entry.PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_NOTEXIST;
					cout << "NOT EXIST" << endl;
				}

				//cout << "Response TOKEN :" << TE.TN_TOKEN << "/" << TE.TN_NEXTZONE << endl;
				//cout << "Response ADDRESS :" << entry.TNSN_DATA << endl;
			}

			// 에코(데이터를준 클라이언트에 다시 데이터쏘기)
			//send(PN->key.REQUEST_SOCEKT, (char*)&entry, sizeof(entry), 0);

			clientSocket = socket(PF_INET, SOCK_STREAM, 0);

			if (clientSocket == INVALID_SOCKET)
				ErrorHandling("clientSocket() error");

			memset(&tempAddr, 0, sizeof(tempAddr));
			tempAddr.sin_family = AF_INET;
			tempAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(PN->key.REQUEST_IP));
			tempAddr.sin_port = htons(FES_PORT);

			cout << inet_ntoa(PN->key.REQUEST_IP) << endl;

			if (connect(clientSocket, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR)
				ErrorHandling("connect() error!");

			send(clientSocket, (char*)&entry, sizeof(entry), 0);

			closesocket(clientSocket);

			cout << "Send" << endl;
			cout << "========================" << endl;
		}
	}
}

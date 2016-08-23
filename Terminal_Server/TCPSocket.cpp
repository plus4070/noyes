#include <process.h>
#include "TCPSocket.h"
#include "TerminalTable.h"
#include "ParticipantDataDistributor.h"

static UINT WINAPI receiving(LPVOID p);
static UINT WINAPI storing(LPVOID p);
void ErrorHandling(char *message);
static SOCKET CreateSocket();
static void BindingSocket(SOCKET servSocket);

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

	HANDLE recvThread = (HANDLE)_beginthreadex(NULL, 0, receiving, (LPVOID)this, 0, NULL);
	HANDLE saveThread = (HANDLE)_beginthreadex(NULL, 0, storing, (LPVOID)this, 0, NULL);

	inputDummyData();
	printf("==============  LIST  ==============\n");
	this->participantList->test_showAllEntry();
	printf("====================================\n");

	participantDataDistribute();
	

	CloseHandle(recvThread);
	CloseHandle(saveThread);

	return 0;
}


void TCPSocket::ResetTCPSocket() {
	this->sockTotal		= 0;
	this->participantList = new TerminalTable();
	this->RTable		= new RequestTable();
	this->distributor	= new ParticipantDataDistributor();
	this->distributor->setPubSubList(participantList);
}

void TCPSocket::participantDataDistribute() {
	SOCKADDR_IN								tempAddr;
	list<pair<IN_ADDR, PDD_DATA>>			sendList;
	list<pair<IN_ADDR, PDD_DATA>>::iterator it;
	int										counter;
	char									compareAddress[ADDRESS_SIZE];

	PPDD_NODE PDatagram		= (PPDD_NODE)malloc(sizeof(_PDD_NODE));
	SOCKET ClientSocket		= CreateSocket();

	memset(&tempAddr, 0, sizeof(tempAddr));
	tempAddr.sin_family = AF_INET;
	tempAddr.sin_port = htons(DDS_PORT);
	//Socket 생성 후 Send
	while (true) {
		Sleep(10);
		counter = 0;
		if (this->distributor->checkModifyTableEntry()) {
			sendList = this->distributor->getParticipantData();

			for (it = sendList.begin(); it != sendList.end(); ++it) {
				if (it == sendList.begin()) {
					strcpy(compareAddress, inet_ntoa((*it).first));
				}

				if (strcmp(compareAddress, inet_ntoa((*it).first)) == 0) {
					PDatagram->PDD_DATA[counter] = (*it).second;
					PDatagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA = counter + 1;
					counter++;
				}
				else {
					ClientSocket = CreateSocket();

					tempAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa((*it).first));

					printf("Sending to %s..... \n", inet_ntoa((*it).first));

					if (connect(ClientSocket, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR)
						ErrorHandling("connect() error!");

					send(ClientSocket, (const char *)PDatagram, sizeof(_PDD_NODE), 0);

					closesocket(ClientSocket);

					printf("Complete ..... \n");

					memset(PDatagram->PDD_DATA, 0, sizeof(PDD_DATA) * MAX_PDD_NUMBER);
					PDatagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA = 0;
					counter = 0;
				}
			}


			ClientSocket = CreateSocket();

			tempAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(sendList.back().first));

			printf("Sending to %s..... \n", inet_ntoa(sendList.back().first));

			if (connect(ClientSocket, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR)
				ErrorHandling("connect() error!");

			send(ClientSocket, (const char *)PDatagram, sizeof(_PDD_NODE), 0);

			closesocket(ClientSocket);

			printf("Complete ..... \n");

		}
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

void TCPSocket::SaveRequests(IN_ADDR ip, TNSN_ENTRY tnsData) {
	RTable->addEntry(ip, tnsData);
}

static UINT WINAPI storing(LPVOID p) {
	TCPSocket *		tcpSocket = (TCPSocket*)p;
	SOCKET			clientSocket;
	SOCKADDR_IN		tempAddr;

	while (1) {
		Sleep(10);
		if (tcpSocket->RTable->isRequestExist()) {
			T_ENTRY TD;
			//저장된 RequestTable에서 꺼내와서 데이터 송신
			PR_NODE PN = tcpSocket->RTable->getLastEntry();

			//echo Data
			TNSN_ENTRY entry = PN->key.REQUEST_DATA;

			if (entry.TNSN_DATATYPE == MESSAGE_TYPE_SAVE) {
				//수신 메시지 출력
				cout << "SAVE MSG" << endl;
				cout << "SAVE Topic :" << entry.TNSN_TOPIC << endl;
				cout << "SAVE TokenLevel :" << entry.TNSN_TOKENLEVEL << endl;
				cout << "SAVE TYPE :" << entry.TNSN_DATATYPE << endl;

				entry.TNSN_DATATYPE = MESSAGE_TYPE_SAVEDONE;

				//테이블에 엔트리 삽입
				T_NODE * addNode = new T_NODE;
				T_ENTRY receiveData;
				
				memcpy(receiveData.TD_TOPIC, entry.TNSN_TOPIC, MAX_CHAR);
				memcpy(receiveData.TD_DOMAIN, entry.TNSN_DOMAIN, MAX_CHAR);
				receiveData.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr(entry.TNSN_PARTICIPANT_ADDR);
				receiveData.TD_PUBSUBTYPE = entry.TNSN_NODETYPE;
				memcpy(receiveData.TD_DATA, entry.TNSN_DATA, MAX_DATA_SIZE);
				receiveData.TD_CHANGE_FLAG = true;


				//TerminalTable에 Entry 저장
				tcpSocket->participantList->addEntry(receiveData);
				

				cout << "===========   LIST   ===========" << endl;
				tcpSocket->participantList->test_showAllEntry();

				memcpy(entry.TNSN_DATA, "SAVE COMPLETE", sizeof("SAVE COMPLETE"));
			}
			else {
				puts("ERROR MSG");
			}
			// 에코(데이터를준 클라이언트에 다시 데이터쏘기)
			clientSocket = socket(PF_INET, SOCK_STREAM, 0);

			if (clientSocket == INVALID_SOCKET)
				ErrorHandling("clientSocket() error");

			memset(&tempAddr, 0, sizeof(tempAddr));
			tempAddr.sin_family = AF_INET;
			tempAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(PN->key.REQUEST_IP));
			tempAddr.sin_port = htons(FES_PORT);

			//cout << inet_ntoa(PN->key.REQUEST_IP) << endl;

			if (connect(clientSocket, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR)
				ErrorHandling("connect() error!");

			send(clientSocket, (char*)&entry, sizeof(entry), 0);

			closesocket(clientSocket);

			cout << "Send" << endl;
			cout << "========================" << endl;
		}
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

static void BindingSocket(SOCKET servSocket, int PORT) {
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

	struct sockaddr_in name;
	int len = sizeof(name);

	hServSock = CreateSocket();

	BindingSocket(hServSock, TERMINAL_PORT);

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

					if (getpeername(sockArray[index - WSA_WAIT_EVENT_0], (struct sockaddr *)&name, &len) != 0) {
						perror("getpeername Error");
					}

					//cout << inet_ntoa(name.sin_addr) << endl;
					//cout << sizeof(inet_ntoa(name.sin_addr)) << endl;					

					if (strLen != -1) {
						//RequestTable에 일단 저장
						tcpSocket->SaveRequests(name.sin_addr, TNSNDatagram);
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

void TCPSocket::inputDummyData() {
	T_ENTRY dummy, dummy2, dummy3;
	memcpy(dummy.TD_DOMAIN, "DDS_1", sizeof("DDS_1"));
	memcpy(dummy.TD_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy.TD_PUBSUBTYPE = NODE_TYPE_PUB;
	//memcpy(dummy.TD_TOKEN, "BBBBBB", sizeof("BBBBBB"));
	memcpy(dummy.TD_TOPIC, "Z/XX/CCC/VVVV/BBBBBB", sizeof("Z/XX/CCC/VVVV/BBBBBB"));
	dummy.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.1");

	memcpy(dummy2.TD_DOMAIN, "DDS_1", sizeof("DDS_1"));
	memcpy(dummy2.TD_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy2.TD_PUBSUBTYPE = NODE_TYPE_PUB;
	//memcpy(dummy2.TD_TOKEN, "EEEEEE", sizeof("EEEEEE"));
	memcpy(dummy2.TD_TOPIC, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	dummy2.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.1");

	memcpy(dummy3.TD_DOMAIN, "DDS_1", sizeof("DDS_1"));
	memcpy(dummy3.TD_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy3.TD_PUBSUBTYPE = NODE_TYPE_PUB;
	//memcpy(dummy3.TD_TOKEN, "TTTTTT", sizeof("TTTTTT"));
	memcpy(dummy3.TD_TOPIC, "Q/WW/EEE/RRRR/TTTTTT", sizeof("Q/WW/EEE/RRRR/TTTTTT"));
	dummy3.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.1");

	this->participantList->addEntry(dummy);
	this->participantList->addEntry(dummy2);
	this->participantList->addEntry(dummy3);

	dummy.TD_PUBSUBTYPE = NODE_TYPE_SUB;
	dummy2.TD_PUBSUBTYPE = NODE_TYPE_SUB;
	dummy3.TD_PUBSUBTYPE = NODE_TYPE_SUB;


	this->participantList->addEntry(dummy);
	this->participantList->addEntry(dummy2);
	this->participantList->addEntry(dummy3);

	memcpy(dummy.TD_DOMAIN, "DDS_2", sizeof("DDS_1"));
	memcpy(dummy.TD_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy.TD_PUBSUBTYPE = NODE_TYPE_PUB;
	//memcpy(dummy.TD_TOKEN, "BBBBBB", sizeof("BBBBBB"));
	memcpy(dummy.TD_TOPIC, "Z/XX/CCC/VVVV/BBBBBB", sizeof("Z/XX/CCC/VVVV/BBBBBB"));
	//dummy.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.5");
	dummy.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.1");
	
	memcpy(dummy2.TD_DOMAIN, "DDS_2", sizeof("DDS_1"));
	memcpy(dummy2.TD_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy2.TD_PUBSUBTYPE = NODE_TYPE_PUB;
	//memcpy(dummy2.TD_TOKEN, "EEEEEE", sizeof("EEEEEE"));
	memcpy(dummy2.TD_TOPIC, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	//dummy2.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.6");
	dummy2.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.1");

	memcpy(dummy3.TD_DOMAIN, "DDS_2", sizeof("DDS_1"));
	memcpy(dummy3.TD_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy3.TD_PUBSUBTYPE = NODE_TYPE_PUB;
	//memcpy(dummy3.TD_TOKEN, "TTTTTT", sizeof("TTTTTT"));
	memcpy(dummy3.TD_TOPIC, "Q/WW/EEE/RRRR/TTTTTT", sizeof("Q/WW/EEE/RRRR/TTTTTT"));
	//dummy3.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.7");
	dummy3.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.1");

	this->participantList->addEntry(dummy);
	this->participantList->addEntry(dummy2);
	this->participantList->addEntry(dummy3);

	dummy.TD_PUBSUBTYPE = NODE_TYPE_SUB;
	dummy2.TD_PUBSUBTYPE = NODE_TYPE_SUB;
	dummy3.TD_PUBSUBTYPE = NODE_TYPE_SUB;

	this->participantList->addEntry(dummy);
	this->participantList->addEntry(dummy2);
	this->participantList->addEntry(dummy3);

	memcpy(dummy.TD_DOMAIN, "DDS_3", sizeof("DDS_1"));
	memcpy(dummy.TD_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy.TD_PUBSUBTYPE = NODE_TYPE_PUB;
	//memcpy(dummy.TD_TOKEN, "BBBBBB", sizeof("BBBBBB"));
	memcpy(dummy.TD_TOPIC, "Z/XX/CCC/VVVV/BBBBBB", sizeof("Z/XX/CCC/VVVV/BBBBBB"));
	//dummy.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.7");
	dummy.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.1");

	memcpy(dummy2.TD_DOMAIN, "DDS_3", sizeof("DDS_1"));
	memcpy(dummy2.TD_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy2.TD_PUBSUBTYPE = NODE_TYPE_PUB;
	//memcpy(dummy2.TD_TOKEN, "EEEEEE", sizeof("EEEEEE"));
	memcpy(dummy2.TD_TOPIC, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	//dummy2.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.6");
	dummy2.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.1");

	memcpy(dummy3.TD_DOMAIN, "DDS_3", sizeof("DDS_1"));
	memcpy(dummy3.TD_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy3.TD_PUBSUBTYPE = NODE_TYPE_PUB;
	//memcpy(dummy3.TD_TOKEN, "TTTTTT", sizeof("TTTTTT"));
	memcpy(dummy3.TD_TOPIC, "Q/WW/EEE/RRRR/TTTTTT", sizeof("Q/WW/EEE/RRRR/TTTTTT"));
	//dummy3.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.5");
	dummy3.TD_PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.1");

	this->participantList->addEntry(dummy);
	this->participantList->addEntry(dummy2);
	this->participantList->addEntry(dummy3);

	dummy.TD_PUBSUBTYPE = NODE_TYPE_SUB;
	dummy2.TD_PUBSUBTYPE = NODE_TYPE_SUB;
	dummy3.TD_PUBSUBTYPE = NODE_TYPE_SUB;

	this->participantList->addEntry(dummy);
	this->participantList->addEntry(dummy2);
	this->participantList->addEntry(dummy3);

	printf("Input Complete\n");
}
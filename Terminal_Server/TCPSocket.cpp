#include <process.h>
#include "TCPSocket.h"
#include "TerminalTable.h"

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

	HANDLE recvThread = (HANDLE)_beginthreadex(NULL, 0, receiving, (LPVOID)this, 0, NULL);
	HANDLE saveThread = (HANDLE)_beginthreadex(NULL, 0, storing, (LPVOID)this, 0, NULL);

	while (1) {}

	CloseHandle(recvThread);
	CloseHandle(saveThread);

	return 0;
}


void TCPSocket::ResetTCPSocket() {
	this->sockTotal = 0;
	this->pubList = new TerminalTable();
	this->subList = new TerminalTable();
	this->RTable = new RequestTable();
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

void TCPSocket::SaveRequests(SOCKET rSocket, TNSN_ENTRY tnsData) {
	RTable->addEntry(rSocket, tnsData);
}

static UINT WINAPI storing(LPVOID p) {
	TCPSocket * tcpSocket = (TCPSocket*)p;
	while (1) {
		Sleep(10);
		if (tcpSocket->RTable->isRequestExist()) {
			T_ENTRY TD;
			//저장된 RequestTable에서 꺼내와서 데이터 송신
			PR_NODE PN = tcpSocket->RTable->getLastEntry();
			TNSN_ENTRY entry = PN->key.REQUEST_DATA;

			if (entry.TNSN_DATATYPE == MESSAGE_TYPE_SAVE) {
				//수신 메시지 출력
				cout << "SAVE MSG" << endl;
				cout << "SAVE Topic :" << entry.TNSN_TOPIC << endl;
				cout << "SAVE Token :" << entry.TNSN_TOKEN << endl;
				cout << "SAVE TokenLevel :" << entry.TNSN_TOKENLEVEL << endl;
				cout << "SAVE TYPE :" << entry.TNSN_DATATYPE << endl;

				entry.TNSN_DATATYPE = MESSAGE_TYPE_SAVEDONE;

				//테이블에 엔트리 삽입
				T_NODE * addNode = new T_NODE;
				//memset(addNode->key.token, 0, sizeof(addNode->key.token));
				//memset(addNode->key.topic, 0, sizeof(addNode->key.topic));
				//memset(addNode->key.topic_data, 0, sizeof(addNode->key.topic_data));

				//dummy
				addNode->key.TD_LEVEL = 3;
				memcpy(addNode->key.TD_TOPIC, entry.TNSN_TOPIC, sizeof(entry.TNSN_TOPIC));
				memcpy(addNode->key.TD_TOKEN, entry.TNSN_TOKEN, sizeof(entry.TNSN_TOKEN));
				memcpy(addNode->key.TD_DATA, entry.TNSN_DATA, sizeof(entry.TNSN_DATA));

				if (entry.TNSN_NODETYPE == NODE_TYPE_PUB)
					tcpSocket->pubList->addTopic(addNode->key);
				else if (entry.TNSN_NODETYPE == NODE_TYPE_SUB)
					tcpSocket->subList->addTopic(addNode->key);
				else
					puts("TYPE ERROR!");


				cout << "=========== PUB LIST ===========" << endl;
				tcpSocket->pubList->test_showAllEntry();
				cout << "=========== SUB LIST ===========" << endl;
				tcpSocket->subList->test_showAllEntry();

				memcpy(entry.TNSN_DATA, "SAVE COMPLETE", sizeof("SAVE COMPLETE"));
				entry.TNSN_DATASIZE = sizeof(entry.TNSN_DATA);
			}
			else {
				puts("ERROR MSG");
			}
			// 에코(데이터를준 클라이언트에 다시 데이터쏘기)
			send(PN->key.REQUEST_SOCEKT, (char*)&entry, sizeof(entry), 0);
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

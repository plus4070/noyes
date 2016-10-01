#include "TNSNheader.h"
#include "TNSNServer.h"

namespace CNU_DDS
{
	CRITICAL_SECTION	cs;
	HANDLE				recvThread;
	HANDLE				saveThread;
	deque<PDD_NODE>		recvQueue;
	int					TNS_TYPE;

	void InitalizeTNSServer() {
		printf("[ INITIALIZE < Front-End / DDS > SERVER ]\n\n");

		printf("***** Select Program type *****\n");
		printf("[1] TNS Front-End Server\n");
		printf("[2] DDS Extended  Node\n");
		printf("[others] Exit\n");
		printf("******************************\n");
		printf("input>");
		scanf("%d", &TNS_TYPE);

		if (TNS_TYPE == MESSAGE_TYPE_REQUEST || TNS_TYPE == MESSAGE_TYPE_RESPONSE)
			StartServer();
		else
			return;
	}

	void ErrorHandling(char *message)
	{
		printf("%s", message);
		//fputs(message, stderr);
		//fputc('\n', stderr);
		//exit(1);
	}

	void StartServer() {
		recvQueue.clear();

		InitializeCriticalSection(&cs);

		recvThread = (HANDLE)_beginthreadex(NULL, FIRST_INDEX, Receiving, (LPVOID)&recvQueue, 0, NULL);
		saveThread = (HANDLE)_beginthreadex(NULL, FIRST_INDEX, Storing, (LPVOID)&recvQueue, 0, NULL);

		while (true) {};
		EndServer();
	}

	void EndServer() {
		CloseHandle(saveThread);
		CloseHandle(recvThread);

		DeleteCriticalSection(&cs);

		recvQueue.clear();
	}

	SOCKET CreateSocket() {
		// 소켓 생성 (성공시 핸들을, 실패시 "INVALID_SOCKET" 반환)
		SOCKET servSock = socket(PF_INET, SOCK_STREAM, 0);

		// 소켓 생성 실패 처리
		if (servSock == INVALID_SOCKET) {
			ErrorHandling(MESSAGE_ERROR_SOCKET);
		}

		return servSock;
	}

	void BindingSocket(SOCKET servSocket) {
		// 소켓 통신을 위한 기본 정보
		SOCKADDR_IN servAddr;

		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(PORT);

		// 주소와 Port 할당 (바인드!!)
		if (bind(servSocket, (struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
			ErrorHandling(MESSAGE_ERROR_BIND);
		}

	}

	void LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray) {
		//static void LinkingEvents(SOCKET servSock, int* sockNum, SOCKET sockArray[WSA_MAXIMUM_WAIT_EVENTS], WSAEVENT eventArray[WSA_MAXIMUM_WAIT_EVENTS]) {
		// 이벤트 발생을 확인 (성공시 0, 실패시 "SOCKET_ERROR" 반환)
		vector<SOCKET>::iterator sVec_it;
		vector<WSAEVENT>::iterator eVec_it;

		WSAEVENT newEvent = WSACreateEvent();
		if (WSAEventSelect(servSock, newEvent, FD_ACCEPT) == SOCKET_ERROR) {
			ErrorHandling(MESSAGE_ERROR_WSA_SELECT);
		}


		// 연결 대기 요청 상태로의 진입 (신호가 들어올때까지 대기)
		if (listen(servSock, 5) == SOCKET_ERROR) {
			ErrorHandling(MESSAGE_ERROR_LISTEN);
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
		WSADATA				wsaData;
		SOCKET				hServSock;
		
		vector<SOCKET>		hSockArray; //소켓 핸들배열 - 연결 요청이 들어올 때마다 생성되는 소켓의 핸들을 이 배열에 저장. (최대64)
		vector<WSAEVENT>	hEventArray;
		
		WSANETWORKEVENTS	netEvents;

		int					sockTotal = 0;
		int					index, i;

		// 윈속 초기화 (성공시 0, 실패시 에러 코드리턴)
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			ErrorHandling(MESSAGE_ERROR_WSA_STARTUP);
		}

		hServSock = CreateSocket();
		BindingSocket(hServSock);
		LinkingEvents(hServSock, &sockTotal, &hSockArray, &hEventArray);

		SOCKET * sockArray;
		WSAEVENT * eventArray;

		printf("START RECEIVING TEST\n");

		while (true) {
			sockArray = &hSockArray[0];
			eventArray = &hEventArray[0];

			// 이벤트 종류 구분하기(WSAWaitForMultipleEvents)
			index = WSAWaitForMultipleEvents(sockTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
			index = index - WSA_WAIT_EVENT_0;

			for (i = index; i < sockTotal; i++) {
				index = WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, 0, FALSE);

				if ((index == WSA_WAIT_FAILED || index == WSA_WAIT_TIMEOUT)) {
					continue;
				} else {
					index = i;
					WSAEnumNetworkEvents(sockArray[index], eventArray[index], &netEvents);

					// 초기 연결 요청의 경우.
					if (netEvents.lNetworkEvents & FD_ACCEPT) {
						if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0) {
							puts(MESSAGE_ERROR_ACCEPT);
							break;
						} else {
							OpenConnection(hSockArray, hEventArray, index, sockTotal);
							sockTotal++;
						}
					}

					// 데이터 전송해올 경우.
					if (netEvents.lNetworkEvents & FD_READ)
					{
						if (netEvents.iErrorCode[FD_READ_BIT] != 0)
						{
							puts(MESSAGE_ERROR_READ);
							break;
						} else {
							TransferData(p, sockArray, index);
						}
					}

					// 연결 종료 요청의 경우.
					if (netEvents.lNetworkEvents & FD_CLOSE)
					{
						if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
						{
							puts(MESSAGE_ERROR_CLOSE);
							break;
						} else {
							CloseConnection(hSockArray, hEventArray, sockArray, eventArray, index);
							sockTotal--;
						}
					}
				}//else
			}//for
		}//while

		 // 할당 받은 리소스 반환.
		WSACleanup();

		DeleteCriticalSection(&cs);
	}

	void OpenConnection(vector<SOCKET> hSockArray, vector<WSAEVENT>	hEventArray, int index, int sockTotal) {
		SOCKADDR_IN			clntAddr;
		int					clntLen;

		SOCKET				hClntSock;
		WSAEVENT			newEvent;

		vector<SOCKET>::iterator sVec_it;
		vector<WSAEVENT>::iterator eVec_it;

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

		printf("새로 연결된 소켓의 핸들 %d \n", hClntSock);
		printf("vector size = %d\n", hSockArray.size());
		printf("array  size : %d\n", sockTotal);
	}

	void TransferData(LPVOID p, SOCKET * sockArray, int index) {
		deque<PDD_NODE>		*recvQueue = (deque<PDD_NODE>*)p;
		PDD_NODE			receiveData;
		int					strLen;

		struct	sockaddr_in name;
		int len = sizeof(name);

		// 데이터를 받음 (message에 받은 데이터를 담음)
		strLen = recv(sockArray[index - WSA_WAIT_EVENT_0], (char*)&receiveData, sizeof(_PDD_NODE), 0);

		if (getpeername(sockArray[index - WSA_WAIT_EVENT_0], (struct sockaddr *)&name, &len) != 0) {
			perror("getpeername Error");
		}

		EnterCriticalSection(&cs);
		recvQueue->push_back(receiveData);
		LeaveCriticalSection(&cs);
	}

	void CloseConnection(vector<SOCKET> hSockArray, vector<WSAEVENT>	hEventArray, SOCKET * sockArray, WSAEVENT * eventArray, int index) {
		vector<SOCKET>::iterator sVec_it;
		vector<WSAEVENT>::iterator eVec_it;

		WSACloseEvent(eventArray[index]);

		// 소켓 종류
		closesocket(sockArray[index]);
		printf("종료 된 소켓의 핸들 %d \n", sockArray[index]);

		// 배열 정리.
		printf("삭제 : %d\n", index);
		sVec_it = hSockArray.begin() + index;
		hSockArray.erase(sVec_it);

		eVec_it = hEventArray.begin() + index;
		hEventArray.erase(eVec_it);
	}

	static UINT WINAPI Storing(LPVOID p) {
		PTNSP_NODE					PTNSP;
		PDD_NODE					sendDatagram;
		SOCKADDR_IN					tempAddr;
		SOCKET						clientSocket;

		deque<PDD_NODE>				* recvQueue = (deque<PDD_NODE>*)p;
		TNSTable					* TNST = new TNSTable();
		InputDummy(TNST);

		while (true) {
			Sleep(SLEEP_TIME_SHORT);
			//receive Data Process

			//큐에 메세지 존재하면 메세지 타입에 따라 분류
			if (recvQueue->size() > 0) {
				SeperateMessagePerType(recvQueue, TNST, sendDatagram);
			}

			if (TNS_TYPE == 1 && TNST->isEntryExist(STATE_SET)) {
				PTNSP = TNST->getEntry(STATE_SET);
				PrintPTNSP(PTNSP);

				memset(&tempAddr, 0, sizeof(tempAddr));

				SetDatagramFromPTNSP(&sendDatagram, PTNSP, &tempAddr);

				clientSocket = socket(PF_INET, SOCK_STREAM, 0);

				if (clientSocket == INVALID_SOCKET) {
					ErrorHandling(MESSAGE_ERROR_CLIENT);
				}

				if (connect(clientSocket, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR) {
					ErrorHandling(MESSAGE_ERROR_CONNECT);
				} else {
					send(clientSocket, (char*)&sendDatagram, sizeof(PDD_NODE), 0);
					PTNSP->key.TN_SPACE_STATE = STATE_RESPONSE;
					closesocket(clientSocket);
				}
			}
		}
	}

	void SeperateMessagePerType(deque<PDD_NODE> * recvQueue, TNSTable * TNST, PDD_NODE receiveDatagram) {
		PDD_NODE sendDatagram;
		deque<PDD_NODE>::iterator	it;

		int counter = 0;
		for (it = recvQueue->begin(); it != recvQueue->end(); it++) {
			sendDatagram = (*it);
			if (TNS_TYPE == 1) {
				if (sendDatagram.PDD_HEADER.MESSAGE_TYPE == MESSAGE_TYPE_RESPONSE) {
					TNST->setEntry(receiveDatagram.PDD_HEADER.ID, sendDatagram.PDD_DATA[0].PARTICIPANT_DATA, STATE_SET);
				}
				else if (sendDatagram.PDD_HEADER.MESSAGE_TYPE == MESSAGE_TYPE_SAVEDONE) {
					TNST->removeEntry(sendDatagram.PDD_HEADER.ID);
					puts("SAVE Complete");
				}
				else {
					TNST->removeEntry(sendDatagram.PDD_HEADER.ID);
					puts("ERROR MSG");
				}
			}

			if (sendDatagram.PDD_HEADER.MESSAGE_TYPE == MESSAGE_TYPE_SAVE) {
				// 출력
				PrintMessage(sendDatagram);
			}
			counter++;
		}

		EnterCriticalSection(&cs);
		for (int k = 0; k < counter; k++) {
			recvQueue->pop_front();
		}
		LeaveCriticalSection(&cs);
	}
	
	void SetDatagramFromPTNSP(PDD_NODE * datagram, PTNSP_NODE PTNSP, SOCKADDR_IN * tempAddr) {
		tempAddr->sin_family = AF_INET;
		tempAddr->sin_addr.S_un.S_addr = inet_addr(PTNSP->key.TN_SPACE_NEXTZONE_ADDR);

		if (PTNSP->key.TN_SPACE_TOTAL_LEVEL < PTNSP->key.TN_SPACE_CURRENT_LEVEL) {
			tempAddr->sin_port = htons(TERMINAL_PORT);
			datagram->PDD_HEADER.MESSAGE_TYPE = PTNSP->key.TN_SPACE_MESSAGETYPE;
			memcpy(datagram->PDD_DATA[FIRST_INDEX].PARTICIPANT_DATA, PTNSP->key.TN_SPACE_DATA, MAX_DATA_SIZE);
		}
		else {
			tempAddr->sin_port = htons(TNS_PORT);
			datagram->PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_REQUEST;
		}

		datagram->PDD_HEADER.ID = PTNSP->key.TN_SPACE_ID;

		datagram->PDD_DATA[FIRST_INDEX].PARTICIPANT_NODE_TYPE = PTNSP->key.TN_SPACE_NODETYPE;

		memcpy(datagram->PDD_DATA[FIRST_INDEX].PARTICIPANT_DOMAIN_ID, PTNSP->key.TN_SPACE_DOMAIN, sizeof(PTNSP->key.TN_SPACE_DOMAIN));
		memcpy(datagram->PDD_DATA[FIRST_INDEX].PARTICIPANT_TOPIC, PTNSP->key.TN_SPACE_TOPIC, sizeof(PTNSP->key.TN_SPACE_TOPIC));
		memcpy(datagram->PDD_DATA[FIRST_INDEX].PARTICIPANT_IP, PTNSP->key.TN_SPACE_PARTICIPANT_ADDR, ADDRESS_SIZE);
		itoa(PTNSP->key.TN_SPACE_CURRENT_LEVEL, datagram->PDD_DATA[FIRST_INDEX].PARTICIPANT_DATA, 10);
		datagram->PDD_DATA[FIRST_INDEX].PARTICIPANT_PORT = PTNSP->key.TN_SPACE_PARTICIPANT_PORT;
		datagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA = 1;

		cout << "SEND TYPE :" << datagram->PDD_HEADER.MESSAGE_TYPE << endl;
		cout << "====================" << endl;
	}

	void InputDummy(TNSTable * TNSPTable) {
		//this->TNSPTable->addEntry("Q/WW/EEE/RRRR/TTTTTT", "DDS_3",
		//	"127.0.0.1", "127.0.0.10", 1000, "DDS_TEST_DATA3", NULL, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
		TNSPTable->addEntry("A/BB/CCC/DDDD/EEEEEE", "DDS_1", IP, "127.0.0.20", 70, "CCCCCCC", NULL, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
	}

	void InitialFrontEndServer() {
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

		if (test_type == 2) {
			//inputDummy();
		}
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

			//TNSPTable->addEntry(topic, domain, nextzone, participantip, participantport, data, NULL, pubsub, messageType);

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
	
	void PrintMessage(PDD_NODE datagram) {
		cout << "======================================================================" << endl;
		cout << "NUM OF DATA		:	" << datagram.PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA << endl;
		for (int k = FIRST_INDEX; k < datagram.PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA; k++) {
			cout << "**********************************************************************" << endl;
			cout << "NODE TYPE			:	" << datagram.PDD_DATA[k].PARTICIPANT_NODE_TYPE << endl;
			cout << "Domain ID			:	" << datagram.PDD_DATA[k].PARTICIPANT_DOMAIN_ID << endl;
			cout << "TOPIC				:	" << datagram.PDD_DATA[k].PARTICIPANT_TOPIC << endl;
			cout << "PARTICIPANT IP			:	" << datagram.PDD_DATA[k].PARTICIPANT_IP << endl;
			cout << "PARTICIPANT PORT		:	" << datagram.PDD_DATA[k].PARTICIPANT_PORT << endl;
			cout << "DATA				:	" << datagram.PDD_DATA[k].PARTICIPANT_DATA << endl;
		}
		cout << "======================================================================" << endl;
	}

	void PrintPTNSP(PTNSP_NODE PTNSP) {
		cout << "====================" << endl;
		cout << "SEND MSG" << endl;
		cout << "SEND Topic :" << PTNSP->key.TN_SPACE_TOPIC << endl;
		cout << "SEND Token :" << PTNSP->key.TN_SPACE_TOKEN << endl;
		cout << "SEND TokenLevel :" << PTNSP->key.TN_SPACE_CURRENT_LEVEL << " / " << PTNSP->key.TN_SPACE_TOTAL_LEVEL << endl;
	}
}
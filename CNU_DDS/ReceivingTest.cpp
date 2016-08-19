#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#include <vector>

#define MAX_CHAR		100
#define MAX_DATA_SIZE	1000

using namespace std;

namespace CNU_DDS
{
	typedef struct _PDD_HEADER {
		char *				PARTICIPANT_DOMAIN_ID;
		int					PARTICIPANT_DOMAIN_SIZE;
		int					PARTICIPANT_NODE_TYPE;
		int					PARTICIPANT_NUMBER_OF_DATA;
	} PDD_HEADER, *PPDD_HEADER;

	typedef struct _PDD_DATA {
		char				PARTICIPANT_TOPIC[MAX_CHAR];
		char 				PARTICIPANT_DATA[MAX_DATA_SIZE];
	} PDD_DATA, *PPDD_DATA;

	typedef struct _PDD_NODE {
		PDD_HEADER			PDD_HEADER;
		PDD_DATA *			PDD_DATA;
	} PDD_NODE, *PPDD_NODE;

	void ErrorHandling(char *message)
	{
		fputs(message, stderr);
		fputc('\n', stderr);
		exit(1);
	}

	SOCKET CreateSocket() {
		// 소켓 생성 (성공시 핸들을, 실패시 "INVALID_SOCKET" 반환)
		SOCKET servSock = socket(PF_INET, SOCK_STREAM, 0);

		// 소켓 생성 실패 처리
		if (servSock == INVALID_SOCKET) {
			ErrorHandling("socket() error");
		}

		return servSock;
	}

	void BindingSocket(SOCKET servSocket) {
		// 소켓 통신을 위한 기본 정보
		SOCKADDR_IN servAddr;

		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(4000);

		// 주소와 Port 할당 (바인드!!)
		if (bind(servSocket, (struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
			ErrorHandling("bind() error");
		}

	}
	
	void LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray) {
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

	void	StartReceving(){
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

		PPDD_NODE receiveData;

		int clntLen;
		int sockTotal = 0;
		int index, i;
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

		printf("START RECEIVING TEST\n");

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
						strLen = recv(sockArray[index - WSA_WAIT_EVENT_0], (char*)receiveData, sizeof(_PDD_NODE), 0);

						if (getpeername(sockArray[index - WSA_WAIT_EVENT_0], (struct sockaddr *)&name, &len) != 0) {
							perror("getpeername Error");
						}

						//cout << inet_ntoa(name.sin_addr) << endl;
						//cout << sizeof(inet_ntoa(name.sin_addr)) << endl;					

						if (strLen != -1) {
							// 출력
							cout << "======================================================================" << endl;
							cout << "Domain ID		:	"<< receiveData->PDD_HEADER.PARTICIPANT_DOMAIN_ID << endl;
							cout << "NODE TYPE		:	"<< receiveData->PDD_HEADER.PARTICIPANT_NODE_TYPE << endl;
							cout << "NUM OF DATA	:	"<< receiveData->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA << endl;
							for (int k = 0 ; k < receiveData->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA ; k++){
								cout << "**********************************************************************" << endl;
								cout << "TOPIC			:	"<< receiveData->PDD_HEADER.PARTICIPANT_DOMAIN_ID << endl;
								cout << "DATA			:	"<< receiveData->PDD_HEADER.PARTICIPANT_DOMAIN_ID << endl;
							}
							cout << "======================================================================" << endl;
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
	}
}
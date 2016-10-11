#include <process.h>
#include "SocketManager.h"

static UINT WINAPI Receiving(LPVOID p);
static UINT WINAPI Sending(LPVOID p);

SocketManager::SocketManager()
{

}

SocketManager::~SocketManager()
{
	
}

void SocketManager::StartRecevingThread()
{
	this->recvThread = (HANDLE)_beginthreadex(NULL, 0, Receiving, (LPVOID)this, 0, NULL);
}

void SocketManager::CloseRecevingThread() 
{
	CloseHandle(recvThread);
}

void SocketManager::GetRecevingDEQUE(deque<pair<IN_ADDR, PDD_NODE>> ** dq) 
{
	*dq = &(this->recvData);
}

SOCKET SocketManager::GetRecevingSocket(int port, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray)
{
	SOCKET s = this->CreateSocket();

	this->BindingSocket(s, port);
	this->LinkingEvents(s, sockNum, sockArray, eventArray);

	return s;
}

void SocketManager::StartSendingThread()
{
	this->sendThread = (HANDLE)_beginthreadex(NULL, 0, Sending, (LPVOID)this, 0, NULL);
}

void SocketManager::CloseSendingThread()
{
	CloseHandle(sendThread);
}

void SocketManager::SetTNTable(TopicNameTable * TNTable)
{
	this->TNTable = TNTable;
}

void SocketManager::SetCriticalSection(CRITICAL_SECTION * criticlaSection) 
{
	this->cs = *criticlaSection;
}

void SocketManager::SendPacket(char * TargetAddress, const char * Datagram, int SizeOfDatagram, int port)
{
	SOCKET Socket = socket(PF_INET, SOCK_STREAM, 0);

	if (Socket == INVALID_SOCKET) {
		puts("clientSocket() error");
	}

	SetSocketTargetAddress(&Socket, TargetAddress, port);

	send(Socket, Datagram, SizeOfDatagram, 0);

	closesocket(Socket);
}

SOCKET SocketManager::CreateSocket() 
{
	// ���� ���� (������ �ڵ���, ���н� "INVALID_SOCKET" ��ȯ)
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, 0);

	// ���� ���� ���� ó��
	if (servSock == INVALID_SOCKET) {
		puts("socket() error");
	}

	return servSock;
}
void SocketManager::SetSocketTargetAddress(SOCKET * s, char * TargetAddress, int port) 
{
	SOCKADDR_IN tempAddr;

	memset(&tempAddr, 0, sizeof(tempAddr));
	tempAddr.sin_family = AF_INET;
	tempAddr.sin_addr.S_un.S_addr = inet_addr(TargetAddress);
	tempAddr.sin_port = htons(port);

	if (connect(*s, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR)
		puts("connect() error!");
}

void SocketManager::BindingSocket(SOCKET servSocket, int PORT) 
{
	// ���� ����� ���� �⺻ ����
	SOCKADDR_IN servAddr;

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(PORT);

	// �ּҿ� Port �Ҵ� (���ε�!!)
	if (bind(servSocket, (struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		puts("bind() error");
	}
}

void SocketManager::LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray) 
{
	// �̺�Ʈ �߻��� Ȯ�� (������ 0, ���н� "SOCKET_ERROR" ��ȯ)
	vector<SOCKET>::iterator sVec_it;
	vector<WSAEVENT>::iterator eVec_it;

	WSAEVENT newEvent = WSACreateEvent();

	if (WSAEventSelect(servSock, newEvent, FD_ACCEPT) == SOCKET_ERROR) {
		puts("WSAEventSelect() error");
	}

	// ���� ��� ��û ���·��� ���� (��ȣ�� ���ö����� ���)
	if (listen(servSock, 5) == SOCKET_ERROR) {
		puts("listen() error");
	}

	sVec_it = sockArray->begin() + *sockNum;
	// ���� ���� �ڵ� ����
	sockArray->insert(sVec_it, servSock);

	eVec_it = eventArray->begin() + *sockNum;
	// �̺�Ʈ ������Ʈ �ڵ� ����
	eventArray->insert(eVec_it, newEvent);

	(*sockNum)++;
}

void SocketManager::InsertSocketEvent(int * idx, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray, SOCKET s, WSAEVENT Event) 
{
	vector<SOCKET>::iterator	sVec_it = SocketArray->begin() + *idx;
	vector<WSAEVENT>::iterator	eVec_it = EventArray->begin() + *idx;

	SocketArray->insert(sVec_it, s);
	EventArray->insert(eVec_it, Event);

	(*idx)++;
}

void SocketManager::DeleteSocketEvent(int * idx, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray) 
{
	vector<SOCKET>::iterator sVec_it;
	vector<WSAEVENT>::iterator eVec_it;

	sVec_it = SocketArray->begin() + *idx;
	eVec_it = EventArray->begin() + *idx;

	SocketArray->erase(sVec_it);
	EventArray->erase(eVec_it);
}

void SocketManager::AcceptProc(int idx, int * totalSocket, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray) 
{
	SOCKADDR_IN clntAddr;
	int clntLen = sizeof(clntAddr);

	// ������ ���� (accept | ������ �����ڵ� ���н� "INVALID_SOCKET" ��ȯ)
	SOCKET hClntSock = accept(SocketArray->at(idx), (SOCKADDR*)&clntAddr, &clntLen);

	// �̺�Ʈ Ŀ�� ������Ʈ ����(WSACreateEvent)
	WSAEVENT newEvent = WSACreateEvent();

	// �̺�Ʈ �߻� ���� Ȯ��(WSAEventSelect)
	WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

	InsertSocketEvent(totalSocket, SocketArray, EventArray, hClntSock, newEvent);

	//Test Print Code
	printf("���� ����� ������ �ڵ� %d \n", hClntSock);
	printf("vector size = %d\n", SocketArray->size());
	printf("array  size : %d\n", *totalSocket);
}

PDD_NODE SocketManager::ReadProc(int idx, int * strLen, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray, sockaddr_in * addr) 
{
	PDD_NODE	 receiveData;
	int len = sizeof(*addr);

	*strLen = recv(SocketArray->at(idx - WSA_WAIT_EVENT_0), (char*)&receiveData, sizeof(receiveData), 0);

	if (getpeername(SocketArray->at(idx - WSA_WAIT_EVENT_0), (struct sockaddr *)addr, &len) != 0) {
		perror("getpeername Error");
	}

	return receiveData;
}

void SocketManager::CloseProc(int idx, int * totalSocket, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray) 
{
	WSACloseEvent(EventArray->at(idx));

	// ���� ����
	closesocket(SocketArray->at(idx));
	printf("���� �� ������ �ڵ� %d \n", SocketArray->at(idx));

	(*totalSocket)--;

	// �迭 ����.
	printf("���� : %d\n", idx);

	DeleteSocketEvent(&idx, SocketArray, EventArray);
}

void SocketManager::SavePacketToDeque(CRITICAL_SECTION * cs, deque<pair<IN_ADDR, PDD_NODE>>	* dq, PDD_NODE * pn, sockaddr_in addr) 
{
	EnterCriticalSection(cs);
	dq->push_front(make_pair(addr.sin_addr, *pn));
	LeaveCriticalSection(cs);
}

static UINT WINAPI Receiving(LPVOID p) 
{
	SocketManager	 * manager = (SocketManager*)p;

	WSADATA	wsaData;
	PDD_NODE	receiveData;
	SOCKET	hServSock;

	vector<SOCKET>		hSockArray;		//���� �ڵ�迭 - ���� ��û�� ���� ������ �����Ǵ� ������ �ڵ��� �� �迭�� ����. (�ִ�64)
	vector<WSAEVENT>	hEventArray;	//EventArray
	WSANETWORKEVENTS	netEvents;		//Event Flags

	struct sockaddr_in name;

	int sockTotal = 0;
	int index, i, strLen;
	int len = sizeof(name);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		puts("WSAStartup() error!");
	}
	hServSock = manager->GetRecevingSocket(TNS_PORT, &sockTotal, &hSockArray, &hEventArray);

	SOCKET * sockArray;
	WSAEVENT * eventArray;
	// ����
	while (true){
		sockArray = &hSockArray[0];
		eventArray = &hEventArray[0];

		// �̺�Ʈ ���� �����ϱ�(WSAWaitForMultipleEvents)
		index = WSAWaitForMultipleEvents(sockTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		index = index - WSA_WAIT_EVENT_0;

		for (i = index; i < sockTotal; i++){
			index = WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, 0, FALSE);

			if ((index == WSA_WAIT_FAILED || index == WSA_WAIT_TIMEOUT)) {
				continue;
			}else {
				index = i;
				WSAEnumNetworkEvents(sockArray[index], eventArray[index], &netEvents);

				// �ʱ� ���� ��û�� ���.
				if (netEvents.lNetworkEvents & FD_ACCEPT) {
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0) {
						puts("Accept Error");
						break;
					}
					manager->AcceptProc(index, &sockTotal, &hSockArray, &hEventArray);
				}

				if (netEvents.lNetworkEvents & FD_READ) {
					if (netEvents.iErrorCode[FD_READ_BIT] != 0) {
						puts("Read Error");
						break;
					}
					receiveData = manager->ReadProc(index, &strLen, &hSockArray, &hEventArray, &name);
					
					printf("%s", inet_ntoa(name.sin_addr));

					if (strLen != -1) {
						manager->SavePacketToDeque(&(manager->cs), &(manager->recvData), &receiveData, name);
					}
					puts("Saved");
				}

				if (netEvents.lNetworkEvents & FD_CLOSE) {
					puts("close socket");
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0) {
						puts("Close Error");
						break;
					}
					manager->CloseProc(index, &sockTotal, &hSockArray, &hEventArray);
				}
			}
		}
	}
	puts("close");
	WSACleanup();
	return 0;
}

static UINT WINAPI Sending(LPVOID p)
{
	SocketManager *manager = (SocketManager*)p;

	SOCKET				clientSocket;
	SOCKADDR_IN			tempAddr;
	vector<string>		tokenArray;

	while (true) {
		Sleep(10);
		if (manager->recvData.size() != 0) {
			TN_ENTRY TE;
			//����� RequestTable���� �����ͼ� ������ �۽�
			PDD_NODE entry = manager->recvData.back().second;

			if (entry.PDD_HEADER.MESSAGE_TYPE == MESSAGE_TYPE_REQUEST) {
				tokenArray = manager->TNTable->SplitTopic(entry.PDD_DATA[0].PARTICIPANT_TOPIC);

				//���� �޽��� ���
				cout << "Request MSG" << endl;
				cout << "Request Topic :" << entry.PDD_DATA[0].PARTICIPANT_TOPIC << endl;
				cout << "Request TokenLevel :" << entry.PDD_DATA[0].PARTICIPANT_DATA << endl;
				cout << "Request TYPE :" << entry.PDD_HEADER.MESSAGE_TYPE << endl;

				TE.TN_LEVEL = atoi(entry.PDD_DATA[0].PARTICIPANT_DATA);

				memcpy(TE.TN_TOPIC, entry.PDD_DATA[0].PARTICIPANT_TOPIC, MAX_CHAR);
				memcpy(TE.TN_TOKEN, tokenArray.at(TE.TN_LEVEL - 1).c_str(), MAX_CHAR);

				if (manager->TNTable->IsEntryExist(TE)) {
					manager->TNTable->GetEntry(&TE);
					memcpy(entry.PDD_DATA[0].PARTICIPANT_DATA, TE.TN_NEXTZONE, sizeof(TE.TN_NEXTZONE));
					entry.PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_RESPONSE;
					printf("%s", entry.PDD_DATA[0].PARTICIPANT_DATA);

					cout << "RESPONSE" << endl;
				}else {
					entry.PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_NOTEXIST;
					cout << "NOT EXIST" << endl;
				}
			}

			// ����(�����͸��� Ŭ���̾�Ʈ�� �ٽ� �����ͽ��)
			clientSocket = socket(PF_INET, SOCK_STREAM, 0);

			if (clientSocket == INVALID_SOCKET)
				puts("clientSocket() error");

			memset(&tempAddr, 0, sizeof(tempAddr));
			tempAddr.sin_family = AF_INET;
			tempAddr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(manager->recvData.back().first));
			tempAddr.sin_port = htons(FES_PORT);

			cout << inet_ntoa(manager->recvData.back().first) << endl;

			if (connect(clientSocket, (SOCKADDR*)&tempAddr, sizeof(tempAddr)) == SOCKET_ERROR)
				puts("connect() error!");

			send(clientSocket, (char*)&entry, sizeof(entry), 0);

			closesocket(clientSocket);

			cout << "Send" << endl;
			cout << "========================" << endl;
			
			//Deque���� ����� Request Pop
			EnterCriticalSection(&manager->cs);
			manager->recvData.pop_back();
			LeaveCriticalSection(&manager->cs);
		}
	}
}
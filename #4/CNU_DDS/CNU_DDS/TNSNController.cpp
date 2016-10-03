#include "TNSNController.h"
#include "TNSNheader.h"

#include "include/DCPS/Domain/DomainParticipant.h"
#include "include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "include/Utility/Monitoring/Monitor.h"


TNSNController::TNSNController()
{
}


TNSNController::~TNSNController()
{

}

void TNSNController::initalizeServer() {
	printf("[ INITIALIZE [Front-End / DDS] SERVER ]\n\n");

	printf("***** Select Program type *****\n");
	printf("[1] TNS Front-End Server\n");
	printf("[2] DDS Extended  Node\n");
	printf("[others] Exit\n");
	printf("******************************\n");
	printf("input>");
	scanf("%d", &(this->TNS_TYPE));
}

/*addtional Funciont Start*/
void TNSNController::GetSubscriptionData(Monitor * m) {
	Monitor						*monitor			= Monitor::getMonitorInstance();
	//Monitor							*monitor = m;
	int								reader_counter = 0;
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredSubscriptionData	*sub_data = new SEDPdiscoveredSubscriptionData();

	//for (int i = 0; i<100; i++)
	for (int i = 0; i<5; i++) 
	{
		native_sleep(1000);

		/* Subscriber 정보 */
		while (monitoring_data = monitor->popReaderData())
		{
			sub_data->Deserialize(monitoring_data->data);
			sub_data->topic_name;
			sub_data->type_name;

			monitoring_data->locator->address;
			monitoring_data->locator->port;

			reader_counter++;

			std::cout << "[R]===================================================================" << endl;
			std::cout << "TOPIC NAME		:	" << sub_data->topic_name << endl;
			std::cout << "TYPE  NAME		:	" << sub_data->type_name << endl;
			std::cout << "LOCATER ADDR		:	" << monitoring_data->locator->address << endl;
			std::cout << "LOCATER PORT		:	" << monitoring_data->locator->port << endl;
			std::cout << "======================================================================" << endl;
		}
		printf("[# of discovered reader] %d\n", reader_counter);
	}
}

void TNSNController::SetSubscriptionData(DomainParticipant	*domainPart) {
	DomainParticipant				*part = domainPart;
	int								reader_counter = 0;
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredSubscriptionData	*sub_data = new SEDPdiscoveredSubscriptionData();

	for (int i = 0; i<100; i++)
	{
		native_sleep(1000);

		if (false) {
			if (monitoring_data) {
				/* Subscriber 정보 삽입 */
				sub_data->Deserialize(monitoring_data->data);
				sub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
				part->_remote_datareader_is_discovered(sub_data);

			}
		}
		printf("[# of discovered reader] %d\n", reader_counter);
	}
}

void TNSNController::GetPublicationData(Monitor	* m) {
	Monitor							*monitor			= Monitor::getMonitorInstance();
	//Monitor							*monitor = m;
	int								writer_counter = 0;
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredPublicationData	*pub_data = new SEDPdiscoveredPublicationData();

	//for (int i = 0; i<100; i++)
	for (int i = 0; i<5; i++)
	{
		native_sleep(1000);

		/* Publisher 정보 */
		while (monitoring_data = monitor->popWriterData())
		{
			pub_data->Deserialize(monitoring_data->data);
			
			pub_data->topic_name;
			pub_data->type_name;

			monitoring_data->locator->address;
			monitoring_data->locator->port;

			writer_counter++;

			std::cout << "[W]===================================================================" << endl;
			std::cout << "TOPIC NAME		:	" << pub_data->topic_name << endl;
			std::cout << "TYPE  NAME		:	" << pub_data->type_name << endl;
			std::cout << "LOCATER ADDR		:	" << monitoring_data->locator->address << endl;
			std::cout << "LOCATER PORT		:	" << monitoring_data->locator->port << endl;
			std::cout << "======================================================================" << endl;
		}

		printf("[# of discovered writer] %d\n", writer_counter);
	}
}

void TNSNController::SetPublicationData(DomainParticipant * domainPart) {
	//Monitor							*monitor = Monitor::getMonitorInstance();

	DomainParticipant				*part = domainPart;

	int								writer_counter = 0;
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredPublicationData	*pub_data = new SEDPdiscoveredPublicationData();

	for (int i = 0; i<100; i++)
	{
		native_sleep(1000);

		if (false) {
			if (monitoring_data) {
				/* Publisher 정보 삽입 */
				pub_data->Deserialize(monitoring_data->data);
				pub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
				part->_remote_datawriter_is_discovered(pub_data);
			}
		}

		printf("[# of discovered writer] %d\n", writer_counter);
	}

}

void TNSNController::checkMonitering() {
	Monitor*	monitor = Monitor::getMonitorInstance();
	int								reader_counter = 0;
	int								writer_counter = 0;
	MonitoringData*					monitoring_data = NULL;
	SEDPdiscoveredSubscriptionData	*sub_data = new SEDPdiscoveredSubscriptionData();
	SEDPdiscoveredPublicationData	*pub_data = new SEDPdiscoveredPublicationData();


	for (int i = 0; i<100; i++)
	{
		native_sleep(1000);

		/* Subscriber 정보 */
		while (monitoring_data = monitor->popReaderData())
		{
			sub_data->Deserialize(monitoring_data->data);
			sub_data->topic_name;
			sub_data->type_name;

			monitoring_data->locator->address;
			monitoring_data->locator->port;

			reader_counter++;
		}

		/* Publisher 정보 */
		while (monitoring_data = monitor->popWriterData())
		{
			pub_data->Deserialize(monitoring_data->data);
			pub_data->topic_name;
			pub_data->type_name;

			monitoring_data->locator->address;
			monitoring_data->locator->port;

			writer_counter++;
		}

		if (false) {
			if (monitoring_data) {
				/* Subscriber 정보 삽입 */
				sub_data->Deserialize(monitoring_data->data);
				sub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
				//part->_remote_datareader_is_discovered(sub_data);

				/* Publisher 정보 삽입 */
				pub_data->Deserialize(monitoring_data->data);
				pub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
				//part->_remote_datawriter_is_discovered(pub_data);
			}
		}
		printf("[# of discovered reader] %d\n", reader_counter);
		printf("[# of discovered writer] %d\n", writer_counter);
	}
}

/*addtional Funciont End*/


void TNSNController::setTNSType(int type) {
	this->TNS_TYPE = type;
}

void ErrorHandling(char *message)
{
	printf("%s", message);
	//fputs(message, stderr);
	//fputc('\n', stderr);
	//exit(1);
}

void TNSNController::initalizeSetting() {
	this->initalizeServer();

	this->socketManager		= new SocketManager();
	this->messageHandler	= new MessageHandler();
	this->TNST				= new TNSTable();
	
	InitializeCriticalSection(&cs);
	this->socketManager->setCriticalSection(&cs);

	inputDummy(this->TNST);

	if (this->TNS_TYPE == 1 || this->TNS_TYPE == 2)
		this->startTNSServer();
	else
		return;
}

void TNSNController::startTNSServer() {
	this->socketManager->getRecevingDEQUE(&(this->recvData));
	this->socketManager->startRecevingThread();
	
	//this->distibuteTNSData();
	this->DoTNSService();
}

void TNSNController::closeTNSServer() {
	this->socketManager->closeRecevingThread();
	DeleteCriticalSection(&cs);
	recvData->clear();
}

void TNSNController::setMoniter(Monitor * m) {
	this->monitor = m;
}

void TNSNController::setDomainParticipant(DomainParticipant	*domainPart) {
	this->domainParticipant = domainPart;
}

bool TNSNController::isReceviedDataExist() {
	return this->recvData->size() == 0 ? false : true;
}

bool TNSNController::isParticipantDataExist() {
	return this->TNS_TYPE == 1 && this->TNST->isEntryExist(STATE_SET);
}

void TNSNController::DoTNSService() {
	PTNSP_NODE					PTNSP;
	SOCKET						clientSocket;
	deque<PDD_NODE>::iterator	it;
	PDD_NODE					datagram;

	
	while (true) {
		this->CollectDDSParticipant();

		while (isReceviedDataExist()) {
			datagram = this->recvData->back();
			switch (datagram.PDD_HEADER.MESSAGE_TYPE)
			{
			case MESSAGE_TYPE_RESPONSE:
			case MESSAGE_TYPE_NOTEXIST:
			case MESSAGE_TYPE_SAVEDONE:
			case MESSAGE_TYPE_MODIFYDONE:
			case MESSAGE_TYPE_REMOVEDONE:
				ProcBackEndPacket(&datagram);
				break;
			case MESSAGE_TYPE_SAVE:
				ProcTerminalPacket(&datagram);
				break;
			default:
				printf("%d\n", datagram.PDD_HEADER.MESSAGE_TYPE);
				puts("Wrong Packet!");
				break;
			}

			EnterCriticalSection(&cs);
			recvData->pop_back();
			LeaveCriticalSection(&cs);
		}

		if (isParticipantDataExist()) {
			PTNSP = TNST->getEntry(STATE_SET);

			ProcFrontEndPacket(&datagram, &(PTNSP->key));
		}
	}
}

void TNSNController::CollectDDSParticipant() {
	this->GetPublicationData(this->monitor);
	this->GetSubscriptionData(this->monitor);
}

void TNSNController::ProcFrontEndPacket(PDD_NODE *datagram, TNSP_ENTRY *data) {
	SOCKADDR_IN					tempAddr;
	memset(datagram, 0, sizeof(PDD_NODE));

	std::cout << "====================" << endl;
	std::cout << "SEND MSG" << endl;
	std::cout << "SEND Topic :" << data->TN_SPACE_TOPIC << endl;
	std::cout << "SEND Token :" << data->TN_SPACE_TOKEN << endl;
	std::cout << "SEND TokenLevel :" << data->TN_SPACE_CURRENT_LEVEL << " / " << data->TN_SPACE_TOTAL_LEVEL << endl;
	/*
	memset(&tempAddr, 0, sizeof(tempAddr));
	tempAddr.sin_family = AF_INET;
	tempAddr.sin_addr.S_un.S_addr = inet_addr(PTNSP->key.TN_SPACE_NEXTZONE_ADDR);
	*/

	tempAddr = getAddr(data->TN_SPACE_NEXTZONE_ADDR);
	
	/*
	if (PTNSP->key.TN_SPACE_TOTAL_LEVEL < PTNSP->key.TN_SPACE_CURRENT_LEVEL) {
		tempAddr.sin_port = htons(TERMINAL_PORT);
		Datagram.PDD_HEADER.MESSAGE_TYPE = PTNSP->key.TN_SPACE_MESSAGETYPE;
		memcpy(Datagram.PDD_DATA[0].PARTICIPANT_DATA, PTNSP->key.TN_SPACE_DATA, MAX_DATA_SIZE);
	}
	else {
		tempAddr.sin_port = htons(TNS_PORT);
		Datagram.PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_REQUEST;
	}

	Datagram.PDD_HEADER.ID = PTNSP->key.TN_SPACE_ID;

	Datagram.PDD_DATA[0].PARTICIPANT_NODE_TYPE = PTNSP->key.TN_SPACE_NODETYPE;

	memcpy(Datagram.PDD_DATA[0].PARTICIPANT_DOMAIN_ID, PTNSP->key.TN_SPACE_DOMAIN, sizeof(PTNSP->key.TN_SPACE_DOMAIN));
	memcpy(Datagram.PDD_DATA[0].PARTICIPANT_TOPIC, PTNSP->key.TN_SPACE_TOPIC, sizeof(PTNSP->key.TN_SPACE_TOPIC));
	memcpy(Datagram.PDD_DATA[0].PARTICIPANT_IP, PTNSP->key.TN_SPACE_PARTICIPANT_ADDR, ADDRESS_SIZE);
	itoa(PTNSP->key.TN_SPACE_CURRENT_LEVEL, Datagram.PDD_DATA[0].PARTICIPANT_DATA, 10);
	Datagram.PDD_DATA[0].PARTICIPANT_PORT = PTNSP->key.TN_SPACE_PARTICIPANT_PORT;
	Datagram.PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA = 1;
	*/
	formationFrontEndPacket(datagram, data, &tempAddr);

	std::cout << "SEND TYPE :" << datagram->PDD_HEADER.MESSAGE_TYPE << endl;
	std::cout << "====================" << endl;

	if (data->TN_SPACE_TOTAL_LEVEL < data->TN_SPACE_CURRENT_LEVEL) {
		this->socketManager->sendPacket(data->TN_SPACE_NEXTZONE_ADDR, (const char *)datagram, sizeof(PDD_NODE), TERMINAL_PORT);
	} else {
		this->socketManager->sendPacket(data->TN_SPACE_NEXTZONE_ADDR, (const char *)datagram, sizeof(PDD_NODE), TNS_PORT);
	}
	data->TN_SPACE_STATE = STATE_RESPONSE;
}

void TNSNController::ProcBackEndPacket(PDD_NODE *datagram) {
	if (TNS_TYPE == 1) {
		// 출력
		std::cout << "======================================================================" << endl;
		std::cout << "NUM OF DATA		:	" << datagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA << endl;
		std::cout << "MESSAGE TYPE		:	" << datagram->PDD_HEADER.MESSAGE_TYPE << endl;
		for (int k = 0; k < datagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA; k++) {
			std::cout << "**********************************************************************" << endl;
			std::cout << "NODE TYPE			:	" << datagram->PDD_DATA[k].PARTICIPANT_NODE_TYPE << endl;
			std::cout << "Domain ID			:	" << datagram->PDD_DATA[k].PARTICIPANT_DOMAIN_ID << endl;
			std::cout << "TOPIC				:	" << datagram->PDD_DATA[k].PARTICIPANT_TOPIC << endl;
			std::cout << "PARTICIPANT IP			:	" << datagram->PDD_DATA[k].PARTICIPANT_IP << endl;
			std::cout << "PARTICIPANT PORT		:	" << datagram->PDD_DATA[k].PARTICIPANT_PORT << endl;
			std::cout << "DATA				:	" << datagram->PDD_DATA[k].PARTICIPANT_DATA << endl;
		}
		std::cout << "======================================================================" << endl;


		switch (datagram->PDD_HEADER.MESSAGE_TYPE)
		{
		case MESSAGE_TYPE_RESPONSE:
			TNST->setEntry(datagram->PDD_HEADER.ID, datagram->PDD_DATA[0].PARTICIPANT_DATA, STATE_SET);
			break;
		case MESSAGE_TYPE_MODIFYDONE:
		case MESSAGE_TYPE_SAVEDONE:
		case MESSAGE_TYPE_REMOVEDONE:
			TNST->removeEntry(datagram->PDD_HEADER.ID);
			puts("SAVE Complete");
			break;
		default:
			TNST->removeEntry(datagram->PDD_HEADER.ID);
			puts("ERROR MSG");
			break;
		}
	} else {
		puts("Wrong Packet!");
	}
}

void TNSNController::ProcTerminalPacket(PDD_NODE *datagram) {
	// 출력
	std::cout << "======================================================================" << endl;
	std::cout << "NUM OF DATA		:	" << datagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA << endl;
	std::cout << "MESSAGE TYPE		:	" << datagram->PDD_HEADER.MESSAGE_TYPE << endl;
	for (int k = 0; k < datagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA; k++) {
		std::cout << "**********************************************************************" << endl;
		std::cout << "NODE TYPE			:	" << datagram->PDD_DATA[k].PARTICIPANT_NODE_TYPE << endl;
		std::cout << "Domain ID			:	" << datagram->PDD_DATA[k].PARTICIPANT_DOMAIN_ID << endl;
		std::cout << "TOPIC				:	" << datagram->PDD_DATA[k].PARTICIPANT_TOPIC << endl;
		std::cout << "PARTICIPANT IP			:	" << datagram->PDD_DATA[k].PARTICIPANT_IP << endl;
		std::cout << "PARTICIPANT PORT		:	" << datagram->PDD_DATA[k].PARTICIPANT_PORT << endl;
		std::cout << "DATA				:	" << datagram->PDD_DATA[k].PARTICIPANT_DATA << endl;
	}
	std::cout << "======================================================================" << endl;
}

SOCKADDR_IN		TNSNController::getAddr(char * cp) {
	SOCKADDR_IN		tempAddr;
	memset(&tempAddr, 0, sizeof(tempAddr));
	tempAddr.sin_family = AF_INET;
	tempAddr.sin_addr.S_un.S_addr = inet_addr(cp);
	return tempAddr;
}

void TNSNController::formationFrontEndPacket(PDD_NODE *datagram, TNSP_ENTRY *data, SOCKADDR_IN *addr) {
	memset(datagram, 0, sizeof(PDD_NODE));

	if (data->TN_SPACE_TOTAL_LEVEL < data->TN_SPACE_CURRENT_LEVEL) {
		addr->sin_port = htons(TERMINAL_PORT);
		datagram->PDD_HEADER.MESSAGE_TYPE = data->TN_SPACE_MESSAGETYPE;
		memcpy(datagram->PDD_DATA[0].PARTICIPANT_DATA, data->TN_SPACE_DATA, MAX_DATA_SIZE);
	}
	else {
		addr->sin_port = htons(TNS_PORT);
		datagram->PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_REQUEST;
		itoa(data->TN_SPACE_CURRENT_LEVEL, datagram->PDD_DATA[0].PARTICIPANT_DATA, 10);
	}

	datagram->PDD_HEADER.ID = data->TN_SPACE_ID;
	datagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA = 1;

	datagram->PDD_DATA[0].PARTICIPANT_NODE_TYPE = data->TN_SPACE_NODETYPE;
	memcpy(datagram->PDD_DATA[0].PARTICIPANT_DOMAIN_ID, data->TN_SPACE_DOMAIN, sizeof(data->TN_SPACE_DOMAIN));
	memcpy(datagram->PDD_DATA[0].PARTICIPANT_TOPIC, data->TN_SPACE_TOPIC, sizeof(data->TN_SPACE_TOPIC));
	memcpy(datagram->PDD_DATA[0].PARTICIPANT_IP, data->TN_SPACE_PARTICIPANT_ADDR, ADDRESS_SIZE);
	datagram->PDD_DATA[0].PARTICIPANT_PORT = data->TN_SPACE_PARTICIPANT_PORT;
}

/*
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

PDD_NODE receiveData;

int clntLen;
int sockTotal = 0;
int index, i;
int strLen;

InitializeCriticalSection(&cs);

struct sockaddr_in name;
int len = sizeof(name);

// 윈속 초기화 (성공시 0, 실패시 에러 코드리턴)
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
ErrorHandling("WSAStartup() error!");
}

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
strLen = recv(sockArray[index - WSA_WAIT_EVENT_0], (char*)&receiveData, sizeof(_PDD_NODE), 0);

if (getpeername(sockArray[index - WSA_WAIT_EVENT_0], (struct sockaddr *)&name, &len) != 0) {
perror("getpeername Error");
}


//cout << inet_ntoa(name.sin_addr) << endl;
//cout << sizeof(inet_ntoa(name.sin_addr)) << endl;

if (strLen != -1) {
// 출력
cout << "======================================================================" << endl;
cout << "NUM OF DATA		:	"<< receiveData.PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA << endl;
for (int k = 0 ; k < receiveData.PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA ; k++){
cout << "**********************************************************************" << endl;
cout << "NODE TYPE			:	"<< receiveData.PDD_DATA[k].PARTICIPANT_NODE_TYPE << endl;
cout << "Domain ID			:	"<< receiveData.PDD_DATA[k].PARTICIPANT_DOMAIN_ID << endl;
cout << "TOPIC				:	"<< receiveData.PDD_DATA[k].PARTICIPANT_TOPIC << endl;
cout << "PARTICIPANT IP			:	"<< receiveData.PDD_DATA[k].PARTICIPANT_IP << endl;
cout << "PARTICIPANT PORT		:	"<< receiveData.PDD_DATA[k].PARTICIPANT_PORT << endl;
cout << "DATA				:	"<< receiveData.PDD_DATA[k].PARTICIPANT_DATA << endl;
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

DeleteCriticalSection(&cs);
}
*/
void TNSNController::inputDummy(TNSTable * TNSPTable) {
	//this->TNSPTable->addEntry("Q/WW/EEE/RRRR/TTTTTT", "DDS_3",
	//	"127.0.0.1", "127.0.0.10", 1000, "DDS_TEST_DATA3", NULL, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
	TNSPTable->addEntry("A/BB/CCC/DDDD/EEEEEE", "DDS_1", "127.0.0.1", "127.0.0.20", 70, "CCCCCCC", NULL, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
}

void TNSNController::initalFrontEndServer() {
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
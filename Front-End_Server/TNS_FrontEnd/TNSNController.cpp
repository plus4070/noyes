#include "StdAfx.h"
#include <stdio.h>
#include <WinSock2.h>
#include "TNSNController.h"

static				UINT WINAPI DoTNSService(LPVOID p);
static				UINT WINAPI ApplicationService(LPVOID p);
static				UINT WINAPI FrontEndService(LPVOID p);

TNSNController::TNSNController(int startOption)
{
	this->TNS_TYPE = startOption;
	
	Initialize();
	
	if(startOption == SERVER_TYPE_FRONT_END)
		InitialDDS();
	
	this->StartTNSServer();
}

TNSNController::~TNSNController()
{

}

void			TNSNController::Initialize() {
	this->socketManager = new SocketManager();
	this->TNST = new TNSTable();
	this->tm = new ThreadManager();
	this->domainParticipant = NULL;

	this->tm->SetTNSTable(this->TNST);

	InitializeCriticalSection(&cs);
	this->socketManager->SetCriticalSection(&cs);
	
	if (this->TNS_TYPE == SERVER_TYPE_FRONT_END) {
		SetMoniter();
	}
}

void			TNSNController::InitialDDS() {
	unsigned long	domain_id = 10;

	DomainParticipantQos*	part_qos;
	DomainParticipant*		part;

	this->monitor = Monitor::getMonitorInstance();

	int color_type = 0;

	part_qos = new DomainParticipantQos();
	part_qos->entity_factory.auto_enable_created_entities = true;

	part = new DomainParticipant(domain_id, 1);
	part->set_qos(part_qos);

	//printf("DomainParticipant is created\n");
}

void			TNSNController::SetTNSType(int type) {
	this->TNS_TYPE = type;
}

void			TNSNController::StartTNSServer() {
	this->socketManager->GetRecevingDEQUE(&(this->recvData));
	this->socketManager->StartRecevingThread();
	
	this->ApplicationThread = (HANDLE)_beginthreadex(NULL, 0, ApplicationService, (LPVOID)this, 0, NULL);
	this->tm->SetThread(this->ApplicationThread, THREAD_APPLICATION);

	if (this->TNS_TYPE == SERVER_TYPE_FRONT_END) {
		this->FrontEndThread = (HANDLE)_beginthreadex(NULL, 0, FrontEndService, (LPVOID)this, 0, NULL);
		this->tm->SetThread(this->FrontEndThread, THREAD_FRONTEND_SERVICE);
	}
}

void			TNSNController::CloseTNSServer() {
	this->socketManager->CloseRecevingThread();
	DeleteCriticalSection(&cs);
	recvData->clear();
}

void			TNSNController::SetMoniter() {
	this->monitor = Monitor::getMonitorInstance();
}

void			TNSNController::SetDomainParticipant(DomainParticipant	*domainPart, int participantType) {
	this->domainParticipant = domainPart;
	this->domainParticipantType = participantType;
}

void			TNSNController::SetSubscriptionData(PDD_NODE * pn) {
	MonitoringData					*monitoring_data = new MonitoringData();
	SEDPdiscoveredSubscriptionData	*sub_data = new SEDPdiscoveredSubscriptionData();

	char addr[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
	this->setAddr(addr, pn);

	monitoring_data->data = new ByteStream();
	monitoring_data->locator = new Locator_t();

	monitoring_data->data->setData(pn->PDD_DATA.PARTICIPANT_DATA_ORDERING_TYPE, pn->PDD_DATA.PARTICIPANT_DATA, pn->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA);
	
	//monitoring_data->locator->setAddress(pn->PDD_DATA.PARTICIPANT_IP, pn->PDD_DATA.PARTICIPANT_PORT);
	memcpy(monitoring_data->locator->address, addr, sizeof(addr));
	monitoring_data->locator->port = pn->PDD_DATA.PARTICIPANT_PORT;

	if (this->domainParticipant != NULL && this->domainParticipantType == NODE_TYPE_PUB && monitoring_data) {
		/* Subscriber 정보 삽입 */
		sub_data->Deserialize(monitoring_data->data);
		sub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
		this->domainParticipant->_remote_datareader_is_discovered(sub_data);
	}
}


void			TNSNController::SetPublicationData(PDD_NODE * pn) {
	MonitoringData					*monitoring_data = new MonitoringData();
	SEDPdiscoveredPublicationData	*pub_data = new SEDPdiscoveredPublicationData();
	
	
	monitoring_data->data = new ByteStream();
	monitoring_data->locator = new Locator_t();


	char addr[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
	unsigned addr2[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };

	this->setAddr(addr, pn);

	addr2[12] = (unsigned char)addr[12];
	addr2[13] = (unsigned char)addr[13];
	addr2[14] = (unsigned char)addr[14];
	addr2[15] = (unsigned char)addr[15];

	monitoring_data->data->setData(pn->PDD_DATA.PARTICIPANT_DATA_ORDERING_TYPE, pn->PDD_DATA.PARTICIPANT_DATA, pn->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA);

	//monitoring_data->locator->setAddress(pn->PDD_DATA.PARTICIPANT_IP, pn->PDD_DATA.PARTICIPANT_PORT);
	memcpy(monitoring_data->locator->address, addr, sizeof(addr));
	monitoring_data->locator->port = pn->PDD_DATA.PARTICIPANT_PORT;

	if (this->domainParticipant != NULL && this->domainParticipantType == NODE_TYPE_SUB && monitoring_data) {
		/* Publisher 정보 삽입 */
		pub_data->Deserialize(monitoring_data->data);
		pub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
		this->domainParticipant->_remote_datawriter_is_discovered(pub_data);
	}
}

bool			TNSNController::IsReceviedDataExist() {
	return this->recvData->size() == 0 ? false : true;
}

bool			TNSNController::IsParticipantDataExist() {
	return this->TNS_TYPE == 1 && this->TNST->IsEntryExist(STATE_SET);
}

static			UINT WINAPI DoTNSService(LPVOID p){
//void TNSNController::DoTNSService() {
	TNSNController				* controller = (TNSNController*)p;;
	PTNSP_ENTRY					PTNSP;
	SOCKET						clientSocket;
	deque<PDD_NODE>::iterator	it;
	PDD_NODE					datagram;

	
	while (true) {
		if(controller->TNS_TYPE == SERVER_TYPE_FRONT_END)
			//controller->CollectDDSParticipant();

		while (controller->IsReceviedDataExist()) {
			datagram = controller->recvData->back();
			switch (datagram.PDD_HEADER.MESSAGE_TYPE)
			{
			case MESSAGE_TYPE_RESPONSE:
			case MESSAGE_TYPE_NOTEXIST:
			case MESSAGE_TYPE_SAVEDONE:
			case MESSAGE_TYPE_MODIFYDONE:
			case MESSAGE_TYPE_REMOVEDONE:
				controller->ProcBackEndPacket(&datagram);
				break;
			case MESSAGE_TYPE_SAVE:
				controller->ProcTerminalPacket(&datagram);
				break;
			default:
				printf("%d\n", datagram.PDD_HEADER.MESSAGE_TYPE);
				puts("Wrong Packet!");
				break;
			}

			EnterCriticalSection(&controller->cs);
			controller->recvData->pop_back();
			LeaveCriticalSection(&controller->cs);
		}

		if (controller->IsParticipantDataExist()) {
			PTNSP = controller->TNST->GetEntry(STATE_SET);
			controller->ProcFrontEndPacket(&datagram, PTNSP);
		}
	}
}

static			UINT WINAPI FrontEndService(LPVOID p){
//void TNSNController::DoTNSService() {
	TNSNController				* controller = (TNSNController*)p;;
	PTNSP_ENTRY					PTNSP;
	PDD_NODE					datagram;

	while (true) {
		if (controller->IsParticipantDataExist()) {
			printf("FES[S]\n");
			PTNSP = controller->TNST->GetEntry(STATE_SET);
			controller->ProcFrontEndPacket(&datagram, PTNSP);
		} else {
			printf("FES[E]\n");
			controller->tm->Suspend(THREAD_FRONTEND_SERVICE);
		}
	}
}

static			UINT WINAPI ApplicationService(LPVOID p){
	TNSNController				* controller = (TNSNController*)p;;
	PTNSP_ENTRY					PTNSP;
	PDD_NODE					datagram;

	while (true) {
		if (controller->IsReceviedDataExist()) {
			printf("APP[S]\n");
			datagram = controller->recvData->back();
			switch (datagram.PDD_HEADER.MESSAGE_TYPE)
			{
			case MESSAGE_TYPE_RESPONSE:
			case MESSAGE_TYPE_NOTEXIST:
			case MESSAGE_TYPE_SAVEDONE:
			case MESSAGE_TYPE_MODIFYDONE:
			case MESSAGE_TYPE_REMOVEDONE:
				controller->ProcBackEndPacket(&datagram);
				break;
			case MESSAGE_TYPE_SAVE:
				controller->ProcTerminalPacket(&datagram);
				break;
			default:
				printf("%d\n", datagram.PDD_HEADER.MESSAGE_TYPE);
				puts("Wrong Packet!");
				break;
			}

			EnterCriticalSection(&controller->cs);
			controller->recvData->pop_back();
			LeaveCriticalSection(&controller->cs);

			if(controller->TNS_TYPE == SERVER_TYPE_FRONT_END)
				controller->tm->Resume(THREAD_FRONTEND_SERVICE);

		} else {
			printf("APP[E]\n");
			controller->tm->Suspend(THREAD_APPLICATION);
		}
	}
}

void			TNSNController::StartTNSServiceForFrontEndServer() {
	//void TNSNController::DoTNSService() {
	PTNSP_ENTRY					PTNSP;
	SOCKET						clientSocket;
	deque<PDD_NODE>::iterator	it;
	PDD_NODE					datagram;


	while (true) {
		if (this->TNS_TYPE == SERVER_TYPE_FRONT_END){
			//this->CollectDDSParticipant();
		}

		while (this->IsReceviedDataExist()) {
			datagram = this->recvData->back();
			switch (datagram.PDD_HEADER.MESSAGE_TYPE)
			{
			case MESSAGE_TYPE_RESPONSE:
			case MESSAGE_TYPE_NOTEXIST:
			case MESSAGE_TYPE_SAVEDONE:
			case MESSAGE_TYPE_MODIFYDONE:
			case MESSAGE_TYPE_REMOVEDONE:
				this->ProcBackEndPacket(&datagram);
				break;
			case MESSAGE_TYPE_SAVE:
				this->ProcTerminalPacket(&datagram);
				break;
			default:
				printf("%d\n", datagram.PDD_HEADER.MESSAGE_TYPE);
				puts("Wrong Packet!");
				break;
			}

			EnterCriticalSection(&cs);
			this->recvData->pop_back();
			LeaveCriticalSection(&cs);
		}

		if (this->IsParticipantDataExist()) {
			PTNSP = this->TNST->GetEntry(STATE_SET);
			this->ProcFrontEndPacket(&datagram, PTNSP);
		}
	}
}

void			TNSNController::ProcFrontEndPacket(PDD_NODE *datagram, TNSP_ENTRY *data) {
	if (this->TNS_TYPE != SERVER_TYPE_FRONT_END)
		return;

	SOCKADDR_IN					tempAddr;
	memset(datagram, 0, sizeof(PDD_NODE));
	tempAddr = GetAddr(data->TN_SPACE_NEXTZONE_ADDR);

	FormationFrontEndPacket(datagram, data, &tempAddr); //TNSN Table을 Datagram에 적재
	
	if (data->TN_SPACE_TOTAL_LEVEL < data->TN_SPACE_CURRENT_LEVEL) {
		this->socketManager->SendPacket(data->TN_SPACE_NEXTZONE_ADDR, (const char *)datagram, sizeof(PDD_NODE), TERMINAL_PORT);
	} else {
		this->socketManager->SendPacket(data->TN_SPACE_NEXTZONE_ADDR, (const char *)datagram, sizeof(PDD_NODE), TNS_PORT);
	}

	data->TN_SPACE_STATE = STATE_RESPONSE;

	PrintDatagram(datagram, 1);
}

void			TNSNController::ProcBackEndPacket(PDD_NODE *datagram) {
	if (this->TNS_TYPE != SERVER_TYPE_FRONT_END){
		return;
	}

	PrintDatagram(datagram, 2);

	switch (datagram->PDD_HEADER.MESSAGE_TYPE)
	{
	case MESSAGE_TYPE_RESPONSE:
		TNST->SetEntry(datagram->PDD_HEADER.ID, (char *)&(datagram->PDD_DATA.PARTICIPANT_DATA), STATE_SET);
		break;
	case MESSAGE_TYPE_MODIFYDONE:
	case MESSAGE_TYPE_SAVEDONE:
	case MESSAGE_TYPE_REMOVEDONE:
		TNST->RemoveEntry(datagram->PDD_HEADER.ID);
		puts("SAVE Complete");
		break;
	default:
		TNST->RemoveEntry(datagram->PDD_HEADER.ID);
		puts("ERROR MSG");
		break;
	}
}

void			TNSNController::ProcTerminalPacket(PDD_NODE *datagram) {
	PrintDatagram(datagram,3);
	if (datagram->PDD_DATA.PARTICIPANT_NODE_TYPE == NODE_TYPE_PUB) {
		this->SetPublicationData(datagram);
	}
	else {
		this->SetSubscriptionData(datagram);
	}
}

SOCKADDR_IN		TNSNController::GetAddr(char * cp) {
	SOCKADDR_IN		tempAddr;
	memset(&tempAddr, 0, sizeof(tempAddr));
	tempAddr.sin_family = AF_INET;
	tempAddr.sin_addr.S_un.S_addr = inet_addr(cp);
	return tempAddr;
}

void			TNSNController::FormationFrontEndPacket(PDD_NODE *datagram, TNSP_ENTRY *data, SOCKADDR_IN *addr) {
	memset(datagram, 0, sizeof(PDD_NODE));

	datagram->PDD_HEADER.ID = data->TN_SPACE_ID;
	datagram->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA = data->TN_SPACE_LENGTH_OF_DATA;

	if (data->TN_SPACE_TOTAL_LEVEL < data->TN_SPACE_CURRENT_LEVEL) {
		addr->sin_port = htons(TERMINAL_PORT);
		datagram->PDD_HEADER.MESSAGE_TYPE = data->TN_SPACE_MESSAGETYPE;
		memcpy(datagram->PDD_DATA.PARTICIPANT_DATA, data->TN_SPACE_DATA, MAX_DATA_SIZE);
	}
	else {
		addr->sin_port = htons(TNS_PORT);
		datagram->PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_REQUEST;
		itoa(data->TN_SPACE_CURRENT_LEVEL, (char *)&(datagram->PDD_DATA.PARTICIPANT_DATA), 10);
	}

	datagram->PDD_DATA.PARTICIPANT_NODE_TYPE = data->TN_SPACE_NODETYPE;
	memcpy(datagram->PDD_DATA.PARTICIPANT_DOMAIN_ID, data->TN_SPACE_DOMAIN, sizeof(data->TN_SPACE_DOMAIN));
	memcpy(datagram->PDD_DATA.PARTICIPANT_TOPIC, data->TN_SPACE_TOPIC, sizeof(data->TN_SPACE_TOPIC));
	memcpy(datagram->PDD_DATA.PARTICIPANT_TOPICTYPE, data->TN_SPACE_TOPICTYPE, sizeof(data->TN_SPACE_TOPICTYPE));
	memcpy(datagram->PDD_DATA.PARTICIPANT_IP, data->TN_SPACE_PARTICIPANT_ADDR, ADDRESS_SIZE);
	datagram->PDD_DATA.PARTICIPANT_PORT = data->TN_SPACE_PARTICIPANT_PORT;
	datagram->PDD_DATA.PARTICIPANT_DATA_ORDERING_TYPE = data->TN_SPCAE_DATA_ORDERING_TYPE;
}

void			TNSNController::PrintDatagram(PDD_NODE *datagram, int type) {
	switch (type)
	{
	case 1:
		std::cout << "[FRONT_END_PACKET]====================================================" << endl;
		std::cout << "======================================================================" << endl;
		break;
	case 2:
		std::cout << "[BACK_END_PACKET]=====================================================" << endl;
		std::cout << "======================================================================" << endl;
		break;
	case 3:
		std::cout << "[TERMINAL_PACKET]=====================================================" << endl;
		std::cout << "======================================================================" << endl;
		break;
	}
	
	std::cout << "MESSAGE TYPE		:	" << datagram->PDD_HEADER.MESSAGE_TYPE << endl;
	std::cout << "**********************************************************************" << endl;
	std::cout << "NODE TYPE			:	" << datagram->PDD_DATA.PARTICIPANT_NODE_TYPE << (datagram->PDD_DATA.PARTICIPANT_NODE_TYPE == NODE_TYPE_PUB ? "PUB" : "SUB") << endl;
	std::cout << "Domain ID			:	" << datagram->PDD_DATA.PARTICIPANT_DOMAIN_ID << endl;
	std::cout << "TOPIC				:	" << datagram->PDD_DATA.PARTICIPANT_TOPIC << endl;
	std::cout << "TOPIC TYPE			:	" << datagram->PDD_DATA.PARTICIPANT_TOPICTYPE << endl;
	std::cout << "PARTICIPANT IP			:	" << datagram->PDD_DATA.PARTICIPANT_IP << endl;
	std::cout << "PARTICIPANT PORT		:	" << datagram->PDD_DATA.PARTICIPANT_PORT << endl;
	std::cout << "DATA				:	" << datagram->PDD_DATA.PARTICIPANT_DATA << endl;
	std::cout << "LENGTH OF DATA	:	" << datagram->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA << endl;
	std::cout << "BYTE ORDERING		:	" << datagram->PDD_DATA.PARTICIPANT_DATA_ORDERING_TYPE << endl;
	std::cout << "======================================================================" << endl;
}

void			TNSNController::setAddr(char addr[16], PDD_NODE *datagram) {
	SOCKADDR_IN sock;
	sock.sin_addr.S_un.S_addr = inet_addr(datagram->PDD_DATA.PARTICIPANT_IP);

	addr[12] = sock.sin_addr.S_un.S_un_b.s_b1;
	addr[13] = sock.sin_addr.S_un.S_un_b.s_b2;
	addr[14] = sock.sin_addr.S_un.S_un_b.s_b3;
	addr[15] = sock.sin_addr.S_un.S_un_b.s_b4;
}
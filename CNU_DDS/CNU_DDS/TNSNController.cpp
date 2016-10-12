
#include <stdio.h>
#include <WinSock2.h>
#include "TNSNController.h"
#include "TNSNheader.h"

#include "include/DCPS/Domain/DomainParticipant.h"
#include "include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "include/Utility/Monitoring/Monitor.h"

#include "include/DCPS/Domain/DomainParticipant.h"
#include "include/DCPS/Publication/Publisher.h"
#include "include/DCPS/Publication/DataWriter.h"
#include "include/DCPS/Subscription/Subscriber.h"
#include "include/DCPS/Subscription/DataReader.h"
#include "include/DCPS/Topic/Topic.h"

static				UINT WINAPI DoTNSService(LPVOID p);

TNSNController::TNSNController(int startOption)
{
	this->TNS_TYPE = startOption;
	if(startOption == SERVER_TYPE_FRONT_END)
		InitialDDS();
	
	Initialize();
}


TNSNController::~TNSNController()
{

}


void			TNSNController::Initialize() {
	this->socketManager = new SocketManager();
	this->messageHandler = new MessageHandler();
	this->TNST = new TNSTable();
	this->domainParticipant = NULL;

	InitializeCriticalSection(&cs);
	this->socketManager->SetCriticalSection(&cs);

	InputDummy(this->TNST);

	if (this->TNS_TYPE == SERVER_TYPE_FRONT_END) {
		SetMoniter();
	}

	this->StartTNSServer();
}

void			TNSNController::InitialDDS() {
	printf("[ Shape test about CNU DDS ]\n\n");

	unsigned long	domain_id = 0;

	DomainParticipantQos*	part_qos;
	DomainParticipant*		part;
	Topic*					topic;

	PublisherQos*		pub_qos;
	Publisher*			pub;
	DataWriterQos*		writer_qos;
	DataWriter*			writer;

	this->monitor = Monitor::getMonitorInstance();
	ByteStream*			stream;

	int					i;
	int color_type = 0;

	part_qos = new DomainParticipantQos();
	part_qos->entity_factory.auto_enable_created_entities = true;

	part = new DomainParticipant(domain_id, 1);
	part->set_qos(part_qos);

	printf("DomainParticipant is created\n");

	topic = new Topic("Square", "ShapeType");
	topic->related_participant = part;

	printf("Topic is created");


	pub_qos = new PublisherQos();
	part->get_default_publisher_qos(pub_qos);

	pub_qos->entity_factory.auto_enable_created_entities = true;
	pub_qos->presentation.access_scope = INSTANCE_PRESENTATION_QOS;
	pub_qos->presentation.coherent_access = false;
	pub_qos->presentation.ordered_access = false;

	pub = part->create_publisher(pub_qos, NULL, 0x00000000);
	pub->related_participant = part;
	delete(pub_qos);

	printf("Publisher is created\n");
}

/*addtional Funciont Start*/
void			TNSNController::GetSubscriptionData(Monitor * m) {
	//Monitor							*monitor			= Monitor::getMonitorInstance();
	//Monitor							*monitor = m;
	int								reader_counter = 0;
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredSubscriptionData	*sub_data = new SEDPdiscoveredSubscriptionData();

	//for (int i = 0; i<100; i++)
	for (int i = 0; i<2; i++) 
	{
		native_sleep(500);

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
			int ipv4_1 = 256 + monitoring_data->locator->address[12];
			int ipv4_2 = 256 + monitoring_data->locator->address[13];
			int ipv4_3 = 256 + monitoring_data->locator->address[14];
			int ipv4_4 = 256 + monitoring_data->locator->address[15];
			std::cout << "LOCATER ADDR		:	" << ipv4_1 << "," << ipv4_2 << "," << ipv4_3 << "," << ipv4_4 << endl;
			std::cout << "LOCATER PORT		:	" << monitoring_data->locator->port << endl;
			std::cout << "======================================================================" << endl;
			InputMonitoringData(monitoring_data, NODE_TYPE_SUB);
		}

		printf("[# of discovered reader] %d\n", reader_counter);
	}
}

void			TNSNController::SetSubscriptionData(PDD_NODE * pn) {
	MonitoringData					*monitoring_data = new MonitoringData();
	SEDPdiscoveredSubscriptionData	*sub_data = new SEDPdiscoveredSubscriptionData();

	if (this->domainParticipant != NULL && this->domainParticipantType == NODE_TYPE_PUB) {

		char addr[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
		this->setAddr(addr, pn);

		monitoring_data->data->setData(pn->PDD_DATA.PARTICIPANT_DATA_ORDERING_TYPE, pn->PDD_DATA.PARTICIPANT_DATA, pn->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA);
		//memcpy(monitoring_data->data->data, pn->PDD_DATA.PARTICIPANT_DATA, pn->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA);
		//monitoring_data->data->length = pn->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA;
		//monitoring_data->data->ordering = pn->PDD_DATA.PARTICIPANT_DATA_ORDERING_TYPE;

		//monitoring_data->locator->setAddress(pn->PDD_DATA.PARTICIPANT_IP, pn->PDD_DATA.PARTICIPANT_PORT);
		memcpy(monitoring_data->locator->address, addr, sizeof(addr));
		monitoring_data->locator->port = pn->PDD_DATA.PARTICIPANT_PORT;


		/* Subscriber 정보 삽입 */
		sub_data->Deserialize(monitoring_data->data);
		sub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
		this->domainParticipant->_remote_datareader_is_discovered(sub_data);
	}
}

void			TNSNController::GetPublicationData(Monitor	* m) {
	//Monitor							*monitor			= Monitor::getMonitorInstance();
	//Monitor							*monitor = m;
	int								writer_counter = 0;
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredPublicationData	*pub_data = new SEDPdiscoveredPublicationData();

	//for (int i = 0; i<100; i++)
	for (int i = 0; i<2; i++)
	{
		native_sleep(500);

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
			int ipv4_1 = 256 + monitoring_data->locator->address[12];
			int ipv4_2 = 256 + monitoring_data->locator->address[13];
			int ipv4_3 = 256 + monitoring_data->locator->address[14];
			int ipv4_4 = 256 + monitoring_data->locator->address[15];
			
			unsigned char *p = monitoring_data->data->getData();
			unsigned char ab = monitoring_data->data->getByteOrderingType();
			
			std::cout << "LOCATER ADDR		:	" << ipv4_1 << "," << ipv4_2 << "," << ipv4_3 << "," << ipv4_4 << endl;
			std::cout << "LOCATER PORT		:	" << monitoring_data->locator->port << endl;
			std::cout << "======================================================================" << endl;
			InputMonitoringData(monitoring_data, NODE_TYPE_PUB);
			printf("[# of discovered writer] %d\n", writer_counter);
		}

		printf("[# of discovered writer] %d\n", writer_counter);
	}
}

void			TNSNController::SetPublicationData(PDD_NODE * pn) {
	int								writer_counter = 0;
	MonitoringData					*monitoring_data = new MonitoringData();
	SEDPdiscoveredPublicationData	*pub_data = new SEDPdiscoveredPublicationData();
	monitoring_data->data = new ByteStream();
	monitoring_data->locator = new Locator_t();

	if (this->domainParticipant != NULL && this->domainParticipantType == NODE_TYPE_SUB) {

		char addr[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
		this->setAddr(addr, pn);


		//monitoring_data->data->setData(pn->PDD_DATA.PARTICIPANT_DATA_ORDERING_TYPE, pn->PDD_DATA.PARTICIPANT_DATA, pn->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA);
		memcpy(&(monitoring_data->data->data), pn->PDD_DATA.PARTICIPANT_DATA, pn->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA);
		monitoring_data->data->length = pn->PDD_DATA.PARTICIPANT_LENGTH_OF_DATA;
		monitoring_data->data->ordering = pn->PDD_DATA.PARTICIPANT_DATA_ORDERING_TYPE;
		//monitoring_data->locator->setAddress(pn->PDD_DATA.PARTICIPANT_IP, pn->PDD_DATA.PARTICIPANT_PORT);
		memcpy(monitoring_data->locator->address, addr, sizeof(addr));
		monitoring_data->locator->port = pn->PDD_DATA.PARTICIPANT_PORT;

		/* Publisher 정보 삽입 */
		pub_data->Deserialize(monitoring_data->data);
		pub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
		this->domainParticipant->_remote_datawriter_is_discovered(pub_data);
	}

}

void			TNSNController::CheckMonitering() {
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

void			TNSNController::InputMonitoringData(MonitoringData * data, int type) {
	char ip[16];
	sprintf(ip, "%d.%d.%d.%d", 256+data->locator->address[12], 256 + data->locator->address[13], 256 + data->locator->address[14], 256 + data->locator->address[15]);
	printf("%s", ip);
	if (type == NODE_TYPE_PUB) {
		SEDPdiscoveredPublicationData	* _data = new SEDPdiscoveredPublicationData();
		_data->Deserialize(data->data);
		printf("LENGTH OF PUB DATA : %d\n", data->data->length);
		this->TNST->AddEntry((char *)_data->topic_name.c_str(), (char *)_data->type_name.c_str(), "DDS_1", "127.0.0.1", ip, data->locator->port, data->data->getData(), data->data->getByteOrderingType(), data->data->getLength(), NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
	} else {
		SEDPdiscoveredSubscriptionData	* _data = new SEDPdiscoveredSubscriptionData();
		_data->Deserialize(data->data);
		printf("LENGTH OF SUB DATA : %d\n", data->data->length);
		this->TNST->AddEntry((char *)_data->topic_name.c_str(), (char *)_data->type_name.c_str(), "DDS_1", "127.0.0.1", ip, data->locator->port, data->data->getData(), data->data->getByteOrderingType(), data->data->getLength(), NODE_TYPE_SUB, MESSAGE_TYPE_SAVE);
	}
}

void			TNSNController::SetTNSType(int type) {
	this->TNS_TYPE = type;
}

void			TNSNController::StartTNSServer() {
	this->socketManager->GetRecevingDEQUE(&(this->recvData));
	this->socketManager->StartRecevingThread();
	
	//this->distibuteTNSData();
	//this->DoTNSService();
	
	if (this->TNS_TYPE == SERVER_TYPE_FRONT_END) {
		this->StartTNSServiceForFrontEndServer();
	} else {
		this->FrontEndService = (HANDLE)_beginthreadex(NULL, 0, DoTNSService, (LPVOID)this, 0, NULL);
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
			controller->CollectDDSParticipant();

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

void			TNSNController::StartTNSServiceForFrontEndServer() {
	//void TNSNController::DoTNSService() {
	PTNSP_ENTRY					PTNSP;
	SOCKET						clientSocket;
	deque<PDD_NODE>::iterator	it;
	PDD_NODE					datagram;


	while (true) {
		if (this->TNS_TYPE == SERVER_TYPE_FRONT_END)
			this->CollectDDSParticipant();

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

void			TNSNController::CollectDDSParticipant() {
	this->GetPublicationData(this->monitor);
	this->GetSubscriptionData(this->monitor);
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

void			TNSNController::InputDummy(TNSTable * TNSPTable) {
	unsigned char wdata[200];
	wdata[0] = 'Z';
	wdata[1] = '\0';
	wdata[2] = '\x10';
	wdata[3] = '\0';
	wdata[4] = 205;
	wdata[5] = '\x1e';
	wdata[6] = '\0';
	wdata[7] = '\0';
	wdata[8] = 205;
	wdata[9] = '\x1e';
	wdata[10] = '\0';
	wdata[11] = '\0';
	wdata[12] = '\x1';
	wdata[13] = '\0';
	wdata[14] = '\0';
	wdata[15] = '\0';
	wdata[16] = '\x1';
	wdata[17] = '\0';
	wdata[18] = '\0';
	wdata[19] = '\a';
	wdata[20] = '\x5';
	wdata[21] = '\0';
	wdata[22] = '\x10';
	wdata[23] = '\0';
	wdata[24] = '\f';
	wdata[25] = '\0';
	wdata[26] = '\0';
	wdata[27] = '\0';
	wdata[28] = 'T';
	wdata[29] = 'o';
	wdata[30] = 'p';
	wdata[31] = 'i';
	wdata[32] = 'c';
	wdata[33] = 'N';
	wdata[34] = 'a';
	wdata[35] = 'm';
	wdata[36] = 'e';
	wdata[37] = '_';
	wdata[38] = '0';
	wdata[39] = '\0';
	wdata[40] = '\a';
	wdata[41] = '\0';
	wdata[42] = '\x12';
	wdata[43] = '\0';
	wdata[44] = '\v';
	wdata[45] = '\0';
	wdata[46] = '\0';
	wdata[47] = '\0';
	wdata[48] = 'T';
	wdata[49] = 'y';
	wdata[50] = 'p';
	wdata[51] = 'e';
	wdata[52] = 'N';
	wdata[53] = 'a';
	wdata[54] = 'm';
	wdata[55] = 'e';
	wdata[56] = '_';
	wdata[57] = '0';
	wdata[58] = '\0';
	wdata[59] = '\0';
	wdata[60] = '\0';
	wdata[61] = '\0';
	wdata[62] = '\x1a';
	wdata[63] = '\0';
	wdata[64] = '\f';
	wdata[65] = '\0';
	wdata[66] = '\x2';
	wdata[67] = '\0';
	wdata[68] = '\0';
	wdata[69] = '\0';
	wdata[70] = '\0';
	wdata[71] = '\0';
	wdata[72] = '\0';
	wdata[73] = '\0';
	wdata[74] = '\0';
	wdata[75] = '\0';
	wdata[76] = '\0';
	wdata[77] = '\0';
	wdata[78] = '\x1f';
	wdata[79] = '\0';
	wdata[80] = '\x4';
	wdata[81] = '\0';
	wdata[82] = '\0';
	wdata[83] = '\0';
	wdata[84] = '\0';
	wdata[85] = '\0';
	wdata[86] = '\x1b';
	wdata[87] = '\0';
	wdata[88] = '\f';
	wdata[89] = '\0';
	wdata[90] = '\0';
	wdata[91] = '\0';
	wdata[92] = '\0';
	wdata[93] = '\0';
	wdata[94] = '\0';
	wdata[95] = '\0';
	wdata[96] = '\0';
	wdata[97] = '\0';
	wdata[98] = '\0';
	wdata[99] = '\0';
	wdata[100] = '\0';
	wdata[101] = '\0';
	wdata[102] = '\x1d';
	wdata[103] = '\0';
	wdata[104] = '\x4';
	wdata[105] = '\0';
	wdata[106] = '\0';
	wdata[107] = '\0';
	wdata[108] = '\0';
	wdata[109] = '\0';
	wdata[110] = '%';
	wdata[111] = '\0';
	wdata[112] = '\x4';
	wdata[113] = '\0';
	wdata[114] = '\0';
	wdata[115] = '\0';
	wdata[116] = '\0';
	wdata[117] = '\0';
	wdata[118] = '#';
	wdata[119] = '\0';
	wdata[120] = '\b';
	wdata[121] = '\0';
	wdata[122] = '\0';
	wdata[123] = '\0';
	wdata[124] = '\0';
	wdata[125] = '\0';
	wdata[126] = '\0';
	wdata[127] = '\0';
	wdata[128] = '\0';
	wdata[129] = '\0';
	wdata[130] = '\'';
	wdata[131] = '\0';
	wdata[132] = '\b';
	wdata[133] = '\0';
	wdata[134] = '\0';
	wdata[135] = '\0';
	wdata[136] = '\0';
	wdata[137] = '\0';
	wdata[138] = '\0';
	wdata[139] = '\0';
	wdata[140] = '\0';
	wdata[141] = '\0';
	wdata[142] = '\x15';
	wdata[143] = '\0';
	wdata[144] = '\x4';
	wdata[145] = '\0';
	wdata[146] = '\x2';
	wdata[147] = '\x1';
	wdata[148] = '\0';
	wdata[149] = '\0';
	wdata[150] = '\x16';
	wdata[151] = '\0';
	wdata[152] = '\x4';
	wdata[153] = '\0';
	wdata[154] = '\0';
	wdata[155] = '\0';
	wdata[156] = '\0';
	wdata[157] = '\0';
	wdata[158] = '\x1';
	wdata[159] = '\0';
	wdata[160] = '\0';
	wdata[161] = '\0';
	wdata[162] = 205;
	wdata[163] = 205;
	wdata[164] = 205;
	wdata[165] = 205;
	wdata[166] = 205;
	wdata[167] = 205;
	wdata[168] = 205;
	wdata[169] = 205;
	wdata[170] = 205;
	wdata[171] = 205;
	wdata[172] = 205;
	wdata[173] = 205;
	wdata[174] = 205;
	wdata[175] = 205;
	wdata[176] = 205;
	wdata[177] = 205;
	wdata[178] = 205;
	wdata[179] = 205;
	wdata[180] = 205;
	wdata[181] = 205;
	wdata[182] = 205;
	wdata[183] = 205;
	wdata[184] = 205;
	wdata[185] = 205;
	wdata[186] = 205;
	wdata[187] = 205;
	wdata[188] = 205;
	wdata[189] = 205;
	wdata[190] = 205;
	wdata[191] = 205;
	wdata[192] = 205;
	wdata[193] = 205;
	wdata[194] = 205;
	wdata[195] = 205;
	wdata[196] = 205;
	wdata[197] = 205;
	wdata[198] = 205;
	wdata[199] = 205;

	//this->TNSPTable->addEntry("Q/WW/EEE/RRRR/TTTTTT", "DDS_3", "127.0.0.1", "127.0.0.10", 1000, "DDS_TEST_DATA3", NULL, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
	//TNSPTable->AddEntry("TopicName_0","TypeName_0", "DDS_1", "127.0.0.1", "127.0.0.20", 7411, wdata, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
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

	addr[12] = sock.sin_addr.S_un.S_un_b.s_b1 - 256;
	addr[13] = sock.sin_addr.S_un.S_un_b.s_b2 - 256;
	addr[14] = sock.sin_addr.S_un.S_un_b.s_b3 - 256;
	addr[15] = sock.sin_addr.S_un.S_un_b.s_b4 - 256;
}
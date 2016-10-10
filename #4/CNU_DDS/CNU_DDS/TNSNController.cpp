
#include <stdio.h>
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

void			TNSNController::SetSubscriptionData(DomainParticipant	*domainPart, PDD_NODE * pn) {
	DomainParticipant				*part = domainPart;
	int								reader_counter = 0;
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredSubscriptionData	*sub_data = new SEDPdiscoveredSubscriptionData();

	for (int i = 0; i<pn->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA; i++)
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
		else {
			//sub_data->Deserialize(pn->PDD_DATA[i].PARTICIPANT_DATA);
			//sub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
			std::cout << "[GET SUBSCRIPTION DATA]===============================================" << endl;
			//std::cout << "TOPIC NAME		:	" << pub_data->topic_name << endl;
			//std::cout << "TYPE  NAME		:	" << pub_data->type_name << endl;
			//std::cout << "LOCATER ADDR		:	" << monitoring_data->locator->address << endl;
			//std::cout << "LOCATER PORT		:	" << monitoring_data->locator->port << endl;
			std::cout << "======================================================================" << endl;
		}

		printf("[# of discovered reader] %d\n", reader_counter);
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
			std::cout << "LOCATER ADDR		:	" << ipv4_1 << "," << ipv4_2 << "," << ipv4_3 << "," << ipv4_4 << endl;
			std::cout << "LOCATER PORT		:	" << monitoring_data->locator->port << endl;
			std::cout << "======================================================================" << endl;
			InputMonitoringData(monitoring_data, NODE_TYPE_PUB);
		}

		printf("[# of discovered writer] %d\n", writer_counter);
	}
}

void			TNSNController::SetPublicationData(DomainParticipant * domainPart, PDD_NODE * pn) {
	//Monitor							*monitor = Monitor::getMonitorInstance();

	DomainParticipant				*part = domainPart;

	int								writer_counter = 0;
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredPublicationData	*pub_data = new SEDPdiscoveredPublicationData();

	for (int i = 0; i<pn->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA; i++)
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
		else {
			//pub_data->Deserialize(pn->PDD_DATA[i].PARTICIPANT_DATA);
			//pub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
			std::cout << "[GET PUBLICATION DATA]================================================" << endl;
			std::cout << "TOPIC NAME		:	" << pub_data->topic_name << endl;
			std::cout << "TYPE  NAME		:	" << pub_data->type_name << endl;
			std::cout << "LOCATER ADDR		:	" << monitoring_data->locator->address << endl;
			std::cout << "LOCATER PORT		:	" << monitoring_data->locator->port << endl;
			std::cout << "======================================================================" << endl;
		}

		printf("[# of discovered writer] %d\n", writer_counter);
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
		this->TNST->AddEntry((char *)_data->topic_name.c_str(), (char *)_data->type_name.c_str(), "DDS_1", "127.0.0.1", ip, data->locator->port, (char *)(data->data->data), NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
	} else {
		SEDPdiscoveredSubscriptionData	* _data = new SEDPdiscoveredSubscriptionData();
		_data->Deserialize(data->data);
		this->TNST->AddEntry((char *)_data->topic_name.c_str(), (char *)_data->type_name.c_str(), "DDS_1", "127.0.0.1", ip, data->locator->port, (char *)(data->data->data), NODE_TYPE_SUB, MESSAGE_TYPE_SAVE);
	}
}
/*addtional Funciont End*/

void			TNSNController::SetTNSType(int type) {
	this->TNS_TYPE = type;
}

void			TNSNController::Initialize() {
	this->socketManager		= new SocketManager();
	this->messageHandler	= new MessageHandler();
	this->TNST				= new TNSTable();
	
	InitializeCriticalSection(&cs);
	this->socketManager->SetCriticalSection(&cs);

	InputDummy(this->TNST);

	if (this->TNS_TYPE == SERVER_TYPE_FRONT_END) {
		SetMoniter();
	}

	this->StartTNSServer();
}

void			TNSNController::InitialDDS(){
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

void			TNSNController::SetDomainParticipant(DomainParticipant	*domainPart) {
	this->domainParticipant = domainPart;
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
		TNST->SetEntry(datagram->PDD_HEADER.ID, datagram->PDD_DATA[0].PARTICIPANT_DATA, STATE_SET);
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
	PrintDatagram(datagram, 3);
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
	datagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA = 1;

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

	datagram->PDD_DATA[0].PARTICIPANT_NODE_TYPE = data->TN_SPACE_NODETYPE;
	memcpy(datagram->PDD_DATA[0].PARTICIPANT_DOMAIN_ID, data->TN_SPACE_DOMAIN, sizeof(data->TN_SPACE_DOMAIN));
	memcpy(datagram->PDD_DATA[0].PARTICIPANT_TOPIC, data->TN_SPACE_TOPIC, sizeof(data->TN_SPACE_TOPIC));
	memcpy(datagram->PDD_DATA[0].PARTICIPANT_TOPICTYPE, data->TN_SPACE_TOPICTYPE, sizeof(data->TN_SPACE_TOPICTYPE));
	memcpy(datagram->PDD_DATA[0].PARTICIPANT_IP, data->TN_SPACE_PARTICIPANT_ADDR, ADDRESS_SIZE);
	datagram->PDD_DATA[0].PARTICIPANT_PORT = data->TN_SPACE_PARTICIPANT_PORT;
}

void			TNSNController::InputDummy(TNSTable * TNSPTable) {
	//this->TNSPTable->addEntry("Q/WW/EEE/RRRR/TTTTTT", "DDS_3", "127.0.0.1", "127.0.0.10", 1000, "DDS_TEST_DATA3", NULL, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
	//TNSPTable->addEntry("A/BB/CCC/DDDD/EEEEEE","TYPE_A", "DDS_1", "127.0.0.1", "127.0.0.20", 70, "CCCCCCC", NULL, NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
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
	
	std::cout << "NUM OF DATA		:	" << datagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA << endl;
	std::cout << "MESSAGE TYPE		:	" << datagram->PDD_HEADER.MESSAGE_TYPE << endl;
	for (int k = 0; k < datagram->PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA; k++) {
		std::cout << "**********************************************************************" << endl;
		std::cout << "NODE TYPE			:	" << datagram->PDD_DATA[k].PARTICIPANT_NODE_TYPE << endl;
		std::cout << "Domain ID			:	" << datagram->PDD_DATA[k].PARTICIPANT_DOMAIN_ID << endl;
		std::cout << "TOPIC				:	" << datagram->PDD_DATA[k].PARTICIPANT_TOPIC << endl;
		std::cout << "TOPIC	TYPE		:	" << datagram->PDD_DATA[k].PARTICIPANT_TOPICTYPE << endl;
		std::cout << "PARTICIPANT IP			:	" << datagram->PDD_DATA[k].PARTICIPANT_IP << endl;
		std::cout << "PARTICIPANT PORT		:	" << datagram->PDD_DATA[k].PARTICIPANT_PORT << endl;
		std::cout << "DATA				:	" << datagram->PDD_DATA[k].PARTICIPANT_DATA << endl;
	}
	std::cout << "======================================================================" << endl;
}
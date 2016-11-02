#include "TNSController.h"

TNSController::TNSController()
{
	initalizeSetting();
	printf("%d\n", sizeof(_PDD_DATA));
	printf("%d\n", sizeof(_PDD_HEADER));
	printf("%d\n", sizeof(_PDD_NODE));
}


TNSController::~TNSController()
{
	closeTNSServer();
	DeleteCriticalSection(&cs);
}

void TNSController::initalizeSetting() {
	this->socketManager = new SocketManager();
	this->databaseManager = new DBManager();
	this->messageHandler = new MessageHandler();

	this->databaseManager->initDBInfo();
	
	inputDummyDataToDB();
	
	InitializeCriticalSection(&cs);
	this->socketManager->setCriticalSection(&cs);
}

void TNSController::startTNSServer() {
	this->socketManager->getRecevingDEQUE(&(this->recvData));
	this->socketManager->startRecevingThread();
	this->distibuteTNSData();
}

void TNSController::closeTNSServer() {
	this->socketManager->closeRecevingThread();
}

void TNSController::distibuteTNSData() {
	list<PDD_DATA> distributeList;
	list<PDD_DATA>::iterator it;
	int NumOfParticipant;

	PPDD_NODE PDatagram = (PPDD_NODE)malloc(sizeof(_PDD_NODE));		//기존에 있는 노드들에게 새로운 참여자의 정보를 전파하기 위해 쓰는 데이터그램, Front-End에서 전달받은 패킷 그대로 써도 될듯
	PPDD_NODE ReturnDatagram = (PPDD_NODE)malloc(sizeof(_PDD_NODE));	//새로 추가된 노드에게 기존 참여자들의 정보를 전파하기 위해 쓰는 데이터그램, 여러개의 엔트리를 가짐

	while (true) {
		messageHandler->setPDDNode(PDatagram);
		messageHandler->setPDDNode(ReturnDatagram);

		Sleep(10);

		while (isReceviedDataExist()) {
			PDD_NODE entry = recvData->back().second;

			if (messageHandler->isPacketAvailable(&entry)) {
				if (!this->databaseManager->isTopicExist(entry.PDD_DATA[0].PARTICIPANT_TOPIC)) {
					printf("This Topic isn't Exist\n");
					messageHandler->setMessageTypeTopicNotExist(&entry);
				}
				else {// DB에 저장 및 copy protocol
					messageHandler->setMessageType(ReturnDatagram, entry.PDD_HEADER.MESSAGE_TYPE);
					messageHandler->copyDatagram(&entry, PDatagram);
					
					distributeList = databaseManager->InsertEntry(entry.PDD_DATA[0]);

					messageHandler->setMessageTypeProcessDone(&entry);
				}
			}
			else {
				messageHandler->setMessageTypeTopicNotExist(&entry);
				puts("ERROR MSG TYPE");
			}

			//Front-end에게 보내는 SAVEDONE
			socketManager->sendPacket(inet_ntoa(recvData->back().first), (char *)&entry, sizeof(_PDD_NODE), FES_PORT);
			Sleep(100);

			//기존 노드들에게 새로 노드 추가됨을 알림
			for (it = distributeList.begin(); it != distributeList.end(); ++it) {
				Sleep(100);
				messageHandler->addDataToNode(ReturnDatagram, *it);
				socketManager->sendPacket((*it).PARTICIPANT_IP, (const char *)PDatagram, sizeof(_PDD_NODE), DDS_PORT);
			}

			for (it = distributeList.begin(); it != distributeList.end(); ++it) {
				Sleep(100);
				memset(ReturnDatagram->PDD_DATA, 0, sizeof(MAX_PDD_NUMBER));
				ReturnDatagram->PDD_HEADER.NUMBER_OF_PARTICIPANT = 0;
				messageHandler->addDataToNode(ReturnDatagram, *it);
				socketManager->sendPacket(entry.PDD_DATA[0].PARTICIPANT_IP, (const char *)ReturnDatagram, sizeof(_PDD_NODE), DDS_PORT);
			}

			Sleep(200);

			//새로 추가된 노드에게 기존에 있는 노드들의 정보를 알림
			//socketManager->sendPacket(entry.PDD_DATA[0].PARTICIPANT_IP, (const char *)ReturnDatagram, sizeof(_PDD_NODE), DDS_PORT);

			EnterCriticalSection(&cs);
			recvData->pop_back();
			LeaveCriticalSection(&cs);
		}
	}
}

bool TNSController::isReceviedDataExist() {
	return this->recvData->size() == 0 ? false : true;
}

void TNSController::inputDummyDataToDB() {
	PDD_DATA dummy, dummy2, dummy3;
	memcpy(dummy.PARTICIPANT_DOMAIN_ID, "DDS_1", sizeof("DDS_1"));
	memcpy(dummy.PARTICIPANT_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	//memcpy(dummy.TD_TOKEN, "BBBBBB", sizeof("BBBBBB"));
	memcpy(dummy.PARTICIPANT_TOPIC, "Z/XX/CCC/VVVV/BBBBBB", sizeof("Z/XX/CCC/VVVV/BBBBBB"));
	strcpy(dummy.PARTICIPANT_IP, "127.0.0.1");
	strcpy(dummy.PARTICIPANT_TOPICTYPE, "TopicType_1");
	dummy.PARTICIPANT_PORT = 1000;

	memcpy(dummy2.PARTICIPANT_DOMAIN_ID, "DDS_1", sizeof("DDS_1"));
	memcpy(dummy2.PARTICIPANT_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy2.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	//memcpy(dummy2.TD_TOKEN, "EEEEEE", sizeof("EEEEEE"));
	memcpy(dummy2.PARTICIPANT_TOPIC, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	strcpy(dummy2.PARTICIPANT_IP, "127.0.0.1");
	strcpy(dummy2.PARTICIPANT_TOPICTYPE, "TopicType_1");
	dummy2.PARTICIPANT_PORT = 2000;

	memcpy(dummy3.PARTICIPANT_DOMAIN_ID, "DDS_1", sizeof("DDS_1"));
	memcpy(dummy3.PARTICIPANT_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy3.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	//memcpy(dummy3.TD_TOKEN, "TTTTTT", sizeof("TTTTTT"));
	memcpy(dummy3.PARTICIPANT_TOPIC, "Q/WW/EEE/RRRR/TTTTTT", sizeof("Q/WW/EEE/RRRR/TTTTTT"));
	strcpy(dummy3.PARTICIPANT_IP, "127.0.0.1");
	strcpy(dummy3.PARTICIPANT_TOPICTYPE, "TopicType_1");
	dummy3.PARTICIPANT_PORT = 3000;

	this->databaseManager->InsertEntry(dummy);
	this->databaseManager->InsertEntry(dummy2);
	this->databaseManager->InsertEntry(dummy3);

	dummy.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;
	dummy2.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;
	dummy3.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;


	this->databaseManager->InsertEntry(dummy);
	this->databaseManager->InsertEntry(dummy2);
	this->databaseManager->InsertEntry(dummy3);

	memcpy(dummy.PARTICIPANT_DOMAIN_ID, "DDS_2", sizeof("DDS_1"));
	memcpy(dummy.PARTICIPANT_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	//memcpy(dummy.TD_TOKEN, "BBBBBB", sizeof("BBBBBB"));
	memcpy(dummy.PARTICIPANT_TOPIC, "Z/XX/CCC/VVVV/BBBBBB", sizeof("Z/XX/CCC/VVVV/BBBBBB"));
	//dummy.PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.5");
	strcpy(dummy.PARTICIPANT_IP, "127.0.0.1");
	strcpy(dummy.PARTICIPANT_TOPICTYPE, "TopicType_1");

	memcpy(dummy2.PARTICIPANT_DOMAIN_ID, "DDS_2", sizeof("DDS_1"));
	memcpy(dummy2.PARTICIPANT_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy2.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	//memcpy(dummy2.TD_TOKEN, "EEEEEE", sizeof("EEEEEE"));
	memcpy(dummy2.PARTICIPANT_TOPIC, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	//dummy2.PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.6");
	strcpy(dummy2.PARTICIPANT_IP, "127.0.0.1");
	strcpy(dummy2.PARTICIPANT_TOPICTYPE, "TopicType_1");

	memcpy(dummy3.PARTICIPANT_DOMAIN_ID, "DDS_2", sizeof("DDS_1"));
	memcpy(dummy3.PARTICIPANT_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy3.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	//memcpy(dummy3.TD_TOKEN, "TTTTTT", sizeof("TTTTTT"));
	memcpy(dummy3.PARTICIPANT_TOPIC, "Q/WW/EEE/RRRR/TTTTTT", sizeof("Q/WW/EEE/RRRR/TTTTTT"));
	//dummy3.PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.7");
	strcpy(dummy3.PARTICIPANT_IP, "127.0.0.1");
	strcpy(dummy3.PARTICIPANT_TOPICTYPE, "TopicType_1");

	this->databaseManager->InsertEntry(dummy);
	this->databaseManager->InsertEntry(dummy2);
	this->databaseManager->InsertEntry(dummy3);

	dummy.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;
	dummy2.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;
	dummy3.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;

	this->databaseManager->InsertEntry(dummy);
	this->databaseManager->InsertEntry(dummy2);
	this->databaseManager->InsertEntry(dummy3);

	memcpy(dummy.PARTICIPANT_DOMAIN_ID, "DDS_3", sizeof("DDS_1"));
	memcpy(dummy.PARTICIPANT_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	//memcpy(dummy.TD_TOKEN, "BBBBBB", sizeof("BBBBBB"));
	memcpy(dummy.PARTICIPANT_TOPIC, "Z/XX/CCC/VVVV/BBBBBB", sizeof("Z/XX/CCC/VVVV/BBBBBB"));
	//dummy.PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.7");
	strcpy(dummy.PARTICIPANT_IP, "127.0.0.1");
	strcpy(dummy.PARTICIPANT_TOPICTYPE, "TopicType_1");

	memcpy(dummy2.PARTICIPANT_DOMAIN_ID, "DDS_3", sizeof("DDS_1"));
	memcpy(dummy2.PARTICIPANT_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy2.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	//memcpy(dummy2.TD_TOKEN, "EEEEEE", sizeof("EEEEEE"));
	memcpy(dummy2.PARTICIPANT_TOPIC, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	//dummy2.PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.6");
	strcpy(dummy2.PARTICIPANT_IP, "127.0.0.1");
	strcpy(dummy2.PARTICIPANT_TOPICTYPE, "TopicType_1");

	memcpy(dummy3.PARTICIPANT_DOMAIN_ID, "DDS_3", sizeof("DDS_1"));
	memcpy(dummy3.PARTICIPANT_DATA, "TEST_DDS_DATA", sizeof("TEST_DDS_DATA"));
	dummy3.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	//memcpy(dummy3.TD_TOKEN, "TTTTTT", sizeof("TTTTTT"));
	memcpy(dummy3.PARTICIPANT_TOPIC, "Q/WW/EEE/RRRR/TTTTTT", sizeof("Q/WW/EEE/RRRR/TTTTTT"));
	//dummy3.PARTICIPANT_IP.S_un.S_addr = inet_addr("127.0.0.5");
	strcpy(dummy3.PARTICIPANT_IP, "127.0.0.1");
	strcpy(dummy3.PARTICIPANT_TOPICTYPE, "TopicType_1");

	this->databaseManager->InsertEntry(dummy);
	this->databaseManager->InsertEntry(dummy2);
	this->databaseManager->InsertEntry(dummy3);

	dummy.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;
	dummy2.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;
	dummy3.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;

	this->databaseManager->InsertEntry(dummy);
	this->databaseManager->InsertEntry(dummy2);
	this->databaseManager->InsertEntry(dummy3);

	char wdata[4];
	wdata[0] = 'Z';
	wdata[1] = '\0';
	wdata[2] = '\x10';
	wdata[3] = '\0';

	memcpy(dummy.PARTICIPANT_DOMAIN_ID, "DDS_1", sizeof("DDS_1"));
	memcpy(dummy.PARTICIPANT_DATA, wdata, sizeof(wdata));
	dummy.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	memcpy(dummy.PARTICIPANT_TOPIC, "TopicName_0", sizeof("TopicName_0"));
	strcpy(dummy.PARTICIPANT_IP, "168.188.128.211");
	strcpy(dummy.PARTICIPANT_TOPICTYPE, "TypeName_0");
	dummy.PARTICIPANT_PORT = 7411;
	this->databaseManager->InsertEntry(dummy);

	wdata[0] = 'A';
	wdata[1] = '\0';
	wdata[2] = '\x10';
	wdata[3] = '\0';

	memcpy(dummy.PARTICIPANT_DOMAIN_ID, "DDS_1", sizeof("DDS_1"));
	memcpy(dummy.PARTICIPANT_DATA, wdata, sizeof(wdata));
	dummy.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	memcpy(dummy.PARTICIPANT_TOPIC, "TopicName_0", sizeof("TopicName_0"));
	strcpy(dummy.PARTICIPANT_IP, "168.188.128.211");
	strcpy(dummy.PARTICIPANT_TOPICTYPE, "TypeName_0");
	dummy.PARTICIPANT_PORT = 7411;
	this->databaseManager->InsertEntry(dummy);

	wdata[0] = 'D';
	wdata[1] = '\0';
	wdata[2] = '\x10';
	wdata[3] = '\0';

	memcpy(dummy.PARTICIPANT_DOMAIN_ID, "DDS_1", sizeof("DDS_1"));
	memcpy(dummy.PARTICIPANT_DATA, wdata, sizeof(wdata));
	dummy.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
	memcpy(dummy.PARTICIPANT_TOPIC, "TopicName_0", sizeof("TopicName_0"));
	strcpy(dummy.PARTICIPANT_IP, "168.188.128.211");
	strcpy(dummy.PARTICIPANT_TOPICTYPE, "TypeName_0");
	dummy.PARTICIPANT_PORT = 7411;
	this->databaseManager->InsertEntry(dummy);

	printf("Input Complete\n");
}


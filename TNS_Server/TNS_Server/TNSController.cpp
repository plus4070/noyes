#include "TNSController.h"

TNSController::TNSController()
{
	InitializeSetting();
}


TNSController::~TNSController()
{
	CloseTNSServer();
	DeleteCriticalSection(&cs);
}

void TNSController::InitializeSetting() 
{
	this->TNTable = new TopicNameTable();
	this->socketManager = new SocketManager();
	this->messageHandler = new MessageHandler();

	InitializeView();
	InitializeCriticalSection(&cs);
	this->socketManager->SetCriticalSection(&cs);
}

void TNSController::StartTNSServer() 
{
	this->socketManager->GetRecevingDEQUE(&(this->recvData));
	this->socketManager->StartRecevingThread();
	this->socketManager->StartSendingThread();

	this->TNTable->TestShowAll();

	while (1) {}
}

void TNSController::CloseTNSServer() 
{
	this->socketManager->CloseRecevingThread();
}

bool TNSController::IsReceviedDataExist() 
{
	return this->recvData->size() == 0 ? false : true;
}

void TNSController::InputDummyEntryToTNTable()
{
	TN_ENTRY TE;
	TE.TN_LEVEL = 1;
	memcpy(TE.TN_NEXTZONE, "127.0.0.1", ADDRESS_SIZE);
	memcpy(TE.TN_TOKEN, "A", sizeof("A"));
	memcpy(TE.TN_TOPIC, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	TNTable->AddEntry(TE);
	TE.TN_LEVEL = 2;
	memcpy(TE.TN_TOKEN, "BB", sizeof("BB"));
	TNTable->AddEntry(TE);
	TE.TN_LEVEL = 3;
	memcpy(TE.TN_TOKEN, "CCC", sizeof("CCC"));
	TNTable->AddEntry(TE);
	TE.TN_LEVEL = 4;
	memcpy(TE.TN_TOKEN, "DDDD", sizeof("DDDD"));
	TNTable->AddEntry(TE);
	TE.TN_LEVEL = 5;
	memcpy(TE.TN_TOKEN, "EEEEEE", sizeof("EEEEEE"));
	memcpy(TE.TN_NEXTZONE, "127.0.0.1", ADDRESS_SIZE);
	//memcpy(TE.TN_NEXTZONE, "192.168.0.22", ADDRESS_SIZE);
	TE.TN_LEVEL = 1;
	memcpy(TE.TN_NEXTZONE, "127.0.0.1", ADDRESS_SIZE);
	memcpy(TE.TN_TOKEN, "TopicName_0", sizeof("TopicName_0"));
	memcpy(TE.TN_TOPIC, "TopicName_0", sizeof("TopicName_0"));

	TNTable->AddEntry(TE);
	TNTable->TestShowAll();

	this->socketManager->SetTNTable(TNTable);
}

void TNSController::InitializeView()
{
	int	test_type;
	TN_ENTRY TE;

	printf("[ INITIALIZE TNS SERVER ]\n\n");

	printf("***** Select inital type *****\n");
	printf("[1] Create Transmission Topic\n");
	printf("[2] Input  Dummy        Topic\n");
	printf("[others] Exit\n");
	printf("******************************\n");

	printf("input>");
	scanf("%d", &test_type);

	if (test_type == 2)
		InputDummyEntryToTNTable();

	while (test_type == 1) {
		printf("[ INPUT TOPIC NAME SPACE ENTRY ]\n\n");
		printf("Input Topic >");
		scanf("%s", &TE.TN_TOPIC);

		printf("Input Token >");
		scanf("%s", &TE.TN_TOKEN);

		printf("Input Level of Token >");
		scanf("%d", &TE.TN_LEVEL);

		printf("Input Next Zone >");
		scanf("%s", &TE.TN_NEXTZONE);

		TNTable->AddEntry(TE);
		TNTable->TestShowAll();

		fflush(stdin);

		printf("***** Select inital type *****\n");
		printf("[1] Create Transmission Topic\n");
		printf("[others] Exit\n");
		printf("******************************\n");

		printf("input>");
		scanf("%d", &test_type);
	}

	printf("[ Complete Inital TNS SERVER ]\n\n");
}
#include "ViewManager.h"

ViewManager::ViewManager()
{
	this->TNTable = new TopicNameTable();
}

ViewManager::~ViewManager()
{

}

void ViewManager::initialize()
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
		inputDummyEntryToTNTable();

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

		TNTable->addEntry(TE);
		TNTable->testShowAll();

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

void ViewManager::inputDummyEntryToTNTable()
{
	TN_ENTRY TE;
	TE.TN_LEVEL = 1;
	memcpy(TE.TN_NEXTZONE, "127.0.0.1", ADDRESS_SIZE);
	memcpy(TE.TN_TOKEN, "A", sizeof("A"));
	memcpy(TE.TN_TOPIC, "A/BB/CCC/DDDD/EEEEEE", sizeof("A/BB/CCC/DDDD/EEEEEE"));
	TNTable->addEntry(TE);
	TE.TN_LEVEL = 2;
	memcpy(TE.TN_TOKEN, "BB", sizeof("BB"));
	TNTable->addEntry(TE);
	TE.TN_LEVEL = 3;
	memcpy(TE.TN_TOKEN, "CCC", sizeof("CCC"));
	TNTable->addEntry(TE);
	TE.TN_LEVEL = 4;
	memcpy(TE.TN_TOKEN, "DDDD", sizeof("DDDD"));
	TNTable->addEntry(TE);
	TE.TN_LEVEL = 5;
	memcpy(TE.TN_TOKEN, "EEEEEE", sizeof("EEEEEE"));
	memcpy(TE.TN_NEXTZONE, "127.0.0.1", ADDRESS_SIZE);
	//memcpy(TE.TN_NEXTZONE, "192.168.0.22", ADDRESS_SIZE);
	TNTable->addEntry(TE);
	TNTable->testShowAll();
}
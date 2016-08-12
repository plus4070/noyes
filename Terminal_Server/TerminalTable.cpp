#include "TerminalTable.h"

TerminalTable::TerminalTable() {
	resetTable();
}

void TerminalTable::resetTable() {
	t_head = NULL;
}

bool TerminalTable::isEntryExist(T_ENTRY entry) {
	return false;
}

bool TerminalTable::addTopic(T_ENTRY entry) {
	PT_NODE currNode;
	PT_NODE newNode = (PT_NODE)malloc(sizeof(T_NODE));

	newNode->key = entry;
	newNode->next = NULL;


	if (isEntryExist(entry))
		return false;

	// Router entry¿¡ Ãß°¡
	currNode = t_head;

	printf("%s, %d\n", entry.TD_TOKEN, entry.TD_LEVEL);

	//return true;

	if (t_head == NULL) {
		newNode->next = t_head;
		t_head = newNode;
	} else {
		while (currNode->next != NULL)
			currNode = currNode->next;
		newNode->next = currNode->next;
		currNode->next = newNode;
	}

	return true;
}

bool TerminalTable::deleteTopic(T_ENTRY entry) {

	return true;
}

void TerminalTable::test_showAllEntry() {
	PT_NODE currNode;
	currNode = t_head;

	if (t_head == NULL) {
		puts("NULL");
	} else {
		printf("1:%s\t2:%s\t3:%d\t4:%s\n", currNode->key.TD_TOPIC, currNode->key.TD_TOKEN, currNode->key.TD_LEVEL, currNode->key.TD_DATA);
		while (currNode->next != NULL) {
			currNode = currNode->next;
			printf("1:%s\t2:%s\t3:%d\t4:%s\n", currNode->key.TD_TOPIC, currNode->key.TD_TOKEN, currNode->key.TD_LEVEL, currNode->key.TD_DATA);
		}
	}
}
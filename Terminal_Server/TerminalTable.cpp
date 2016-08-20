#include <vector>
#include "stdafx.h"
#include "TerminalTable.h"


TerminalTable::TerminalTable() {
	resetTable();
}

void TerminalTable::resetTable() {
	this->t_head = NULL;
	this->isModifyEntryExist = false;
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

	newNode->key.TD_CHANGE_FLAG = true;
	this->isModifyEntryExist = true;
	return true;
}

void TerminalTable::resetModifyFlag() {
	this->isModifyEntryExist = false;
}

bool TerminalTable::isTableModified() {
	return this->isModifyEntryExist;
}

int		TerminalTable::getAllModifiedData(PDD_DATA dataArray[MAX_PDD_NUMBER]){
	int numOfData = 0;
	
	PT_NODE currNode = t_head;
	while (currNode != NULL) {
		if (numOfData == MAX_PDD_NUMBER)
			break;

		if (currNode->key.TD_CHANGE_FLAG == true) {
			memcpy(dataArray[numOfData].PARTICIPANT_DATA, currNode->key.TD_DATA, MAX_DATA_SIZE);
			memcpy(dataArray[numOfData].PARTICIPANT_TOPIC, currNode->key.TD_TOPIC, MAX_CHAR);
			currNode->key.TD_CHANGE_FLAG = false;
			numOfData++;
		}
		currNode = currNode->next;
	}
	return numOfData;
}

vector<IN_ADDR> TerminalTable::getAllAddressList() {
	vector<IN_ADDR> returnList;
	
	PT_NODE currNode = t_head;

	while (currNode != NULL) {
		returnList.push_back(currNode->key.TD_PARTICIPANT_IP);
		currNode = currNode->next;
	}
	return returnList;
}


void TerminalTable::test_showAllEntry() {
	PT_NODE currNode;
	currNode = t_head;

	if (t_head == NULL) {
		puts("NULL");
	}
	else {
		printf("[%s]\t1:%s\t2:%s\t3:%d\t4:%s\t5:%d\t6:%s\n",currNode->key.TD_DOMAIN , currNode->key.TD_TOPIC, currNode->key.TD_TOKEN, currNode->key.TD_LEVEL, currNode->key.TD_DATA, currNode->key.TD_CHANGE_FLAG, inet_ntoa(currNode->key.TD_PARTICIPANT_IP));
		while (currNode->next != NULL) {
			currNode = currNode->next;
			printf("[%s]\t1:%s\t2:%s\t3:%d\t4:%s\t5:%d\t6:%s\n", currNode->key.TD_DOMAIN, currNode->key.TD_TOPIC, currNode->key.TD_TOKEN, currNode->key.TD_LEVEL, currNode->key.TD_DATA, currNode->key.TD_CHANGE_FLAG, inet_ntoa(currNode->key.TD_PARTICIPANT_IP));
		}
	}
}


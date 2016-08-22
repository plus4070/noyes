#include <vector>
#include "stdafx.h"
#include "TerminalTable.h"


TerminalTable::TerminalTable() {
	resetTable();
}

void TerminalTable::resetTable() {
	this->t_head = NULL;
	this->d_head = NULL;
	this->isModifyEntryExist = false;
}

bool TerminalTable::isEntryExist(T_ENTRY entry) {
	return false;
}
bool TerminalTable::addEntry(T_ENTRY entry) {
	return addEntryToDomain(entry);
}

bool	TerminalTable::addEntryToDomain(T_ENTRY entry) {
	bool isChanged;

	if (this->d_head != NULL && isDomainExist(entry.TD_DOMAIN)) {
		PDOMAIN_ENTRY currDomain = this->d_head;
		while (strcmp(currDomain->TD_DOMAIN, entry.TD_DOMAIN)) {
			currDomain = currDomain->next;
		}

		currDomain->TD_CHANGE_FLAG = true;
		isChanged = addEntryToTopic(entry, currDomain);

		if (this->isModifyEntryExist == false)
			this->isModifyEntryExist = isChanged;
	}
	else {
		PDOMAIN_ENTRY newNode = (PDOMAIN_ENTRY)malloc(sizeof(DOMAIN_ENTRY));

		memcpy(newNode->TD_DOMAIN, entry.TD_DOMAIN, MAX_CHAR);
		newNode->TOPIC = NULL;
		newNode->next = NULL;
		newNode->TD_CHANGE_FLAG = true;

		newNode->next = this->d_head;
		this->d_head = newNode;

		newNode->TD_CHANGE_FLAG = true;
		addEntryToTopic(entry, newNode);

		this->isModifyEntryExist = true;
	}

	return true;
}

bool	TerminalTable::addEntryToTopic(T_ENTRY entry, DOMAIN_ENTRY * domainNode) {
	bool isChanged = false;
	if (domainNode->TOPIC != NULL && isTopicExist(entry.TD_TOPIC, domainNode->TOPIC)) { // 기존 토픽이 존재할 경우
		PTOPIC_ENTRY currTopic = domainNode->TOPIC;

		while (strcmp(currTopic->TD_TOPIC, entry.TD_TOPIC))
			currTopic = currTopic->next;

		isChanged = addEntryToParticipant(entry, currTopic);

		if (domainNode->TD_CHANGE_FLAG == false) {
			domainNode->TD_CHANGE_FLAG = isChanged;
		}
	}
	else { // Topic이 없거나 전체 토픽의 개수가 1이거나
		PTOPIC_ENTRY newTopic = (PTOPIC_ENTRY)malloc(sizeof(TOPIC_ENTRY));

		newTopic->TD_CHANGE_FLAG_OF_PUB = true;
		newTopic->TD_CHANGE_FLAG_OF_SUB = true;
		
		memcpy(newTopic->TD_TOPIC, entry.TD_TOPIC, MAX_CHAR);
		newTopic->PUB_PARTICIPANT_LIST = NULL;
		newTopic->SUB_PARTICIPANT_LIST = NULL;

		newTopic->next = domainNode->TOPIC;
		domainNode->TOPIC = newTopic;
		
		domainNode->TD_CHANGE_FLAG = true;

		addEntryToParticipant(entry, newTopic);
		isChanged = true;
	}

	printf("return %d\n", isChanged);
	return isChanged;
}

bool	TerminalTable::addEntryToParticipant(T_ENTRY entry, TOPIC_ENTRY * topicNode) {
	PPARTICIPANT_ENTRY currentP;
	bool isChanged = true;


	if (entry.TD_PUBSUBTYPE == NODE_TYPE_PUB) {
		currentP = topicNode->PUB_PARTICIPANT_LIST;
	}
	else {
		currentP = topicNode->SUB_PARTICIPANT_LIST;
	}

	while (currentP != NULL) {
		if (currentP->TD_PARTICIPANT_IP.S_un.S_addr == entry.TD_PARTICIPANT_IP.S_un.S_addr) {
			if (memcmp(currentP->TD_DATA, entry.TD_DATA, MAX_DATA_SIZE) == 0) {
				isChanged = false;
			}
			else {
				memcpy(currentP->TD_DATA, entry.TD_DATA, MAX_DATA_SIZE);
				currentP->TD_CHANGE_FLAG = true;
				break;
			}
		}
		currentP = currentP->next;
	}

	if (currentP == NULL) {
		PPARTICIPANT_ENTRY participant = (PPARTICIPANT_ENTRY)malloc(sizeof(PARTICIPANT_ENTRY));
		participant->TD_PARTICIPANT_IP = entry.TD_PARTICIPANT_IP;
		memcpy(participant->TD_DATA, entry.TD_DATA, MAX_DATA_SIZE);
		participant->TD_CHANGE_FLAG = true;
		
		if (entry.TD_PUBSUBTYPE == NODE_TYPE_PUB) {
			participant->next = topicNode->PUB_PARTICIPANT_LIST;
			topicNode->PUB_PARTICIPANT_LIST = participant;
			topicNode->TD_CHANGE_FLAG_OF_PUB = true;
		}
		else {
			participant->next = topicNode->SUB_PARTICIPANT_LIST;
			topicNode->SUB_PARTICIPANT_LIST = participant;
			topicNode->TD_CHANGE_FLAG_OF_SUB = true;
		}
	}
	printf("return %d\n", isChanged);
	return isChanged;
}

bool TerminalTable::addTopic2(T_ENTRY entry) {
	PT_NODE currNode;
	PT_NODE newNode = (PT_NODE)malloc(sizeof(T_NODE));

	newNode->key = entry;
	newNode->next = NULL;

	if (isEntryExist(entry))
		return false;

	// Router entry에 추가
	currNode = t_head;

	//printf("%s, %d\n", entry.TD_TOKEN, entry.TD_LEVEL);

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
	PDOMAIN_ENTRY domain = d_head;
	PTOPIC_ENTRY topic;
	PPARTICIPANT_ENTRY currentP;

	while (domain != NULL) {
		printf("DOMAIN\t:%s\n", domain->TD_DOMAIN);
		topic = domain->TOPIC;

		//printf("**************************************************************\n");
		while (topic != NULL) {
			printf("\tTOPIC\t:%s\n", topic->TD_TOPIC);


			printf("\t\tPUB LIST\n");
			//printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
			currentP = topic->PUB_PARTICIPANT_LIST;
			while (currentP != NULL) {
				printf("\t\t\t%s\t%s\t%d\n", currentP->TD_DATA, inet_ntoa(currentP->TD_PARTICIPANT_IP), currentP->TD_CHANGE_FLAG);
				//printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
				currentP = currentP->next;
			}

			printf("\t\tSUB LIST\n");
			//printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
			currentP = topic->SUB_PARTICIPANT_LIST;
			while (currentP != NULL) {
				printf("\t\t\t%s\t%s\t%d\n", currentP->TD_DATA, inet_ntoa(currentP->TD_PARTICIPANT_IP), currentP->TD_CHANGE_FLAG);
				//printf("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
				currentP = currentP->next;
			}

			//printf("**************************************************************\n");
			topic = topic->next;
		}
		
		//printf("==============================================================\n");
		domain = domain->next;
	}

	printf("Test Show End!\n");

	/*
	PT_NODE currNode;
	currNode = t_head;

	if (t_head == NULL) {
		puts("NULL");
	}
	else {
		printf("[%s]\t1:%s\t2:%s\t3:%d\t4:%s\t5:%d\t6:%s\n",currNode->key.TD_DOMAIN , currNode->key.TD_TOPIC, currNode->key.TD_PUBSUBTYPE, currNode->key.TD_LEVEL, currNode->key.TD_DATA, currNode->key.TD_CHANGE_FLAG, inet_ntoa(currNode->key.TD_PARTICIPANT_IP));
		while (currNode->next != NULL) {
			currNode = currNode->next;
			printf("[%s]\t1:%s\t2:%s\t3:%d\t4:%s\t5:%d\t6:%s\n", currNode->key.TD_DOMAIN, currNode->key.TD_TOPIC, currNode->key.TD_PUBSUBTYPE, currNode->key.TD_LEVEL, currNode->key.TD_DATA, currNode->key.TD_CHANGE_FLAG, inet_ntoa(currNode->key.TD_PARTICIPANT_IP));
		}
	}
	*/
}


bool				TerminalTable::isDomainExist(char * domain) {
	PDOMAIN_ENTRY currentNode = d_head;

	while (currentNode != NULL) {
		if (!strcmp(domain, currentNode->TD_DOMAIN))
			return true;
		currentNode = currentNode->next;
	}
	return false;
}

bool				TerminalTable::isTopicExist(char * topic, TOPIC_ENTRY * topicNode) {
	PTOPIC_ENTRY currentNode = topicNode;

	while (currentNode != NULL) {
		if (!strcmp(topic, currentNode->TD_TOPIC))
			return true;
		currentNode = currentNode->next;
	}

	return false;
}

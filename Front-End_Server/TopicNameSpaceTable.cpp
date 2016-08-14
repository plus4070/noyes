#include "TopicNameSpaceTable.h"
#include "stdafx.h"



TopicNameSpaceTable::TopicNameSpaceTable() {
	resetTable();
}


TopicNameSpaceTable::~TopicNameSpaceTable()
{
}

void		TopicNameSpaceTable::resetTable() {
	numOfRequests = 0;
	requestList.clear();
	t_head = NULL;
	topicParser = new TopicParser();
}

void		TopicNameSpaceTable::addEntry(char topic[MAX_CHAR], char addr[ADDRESS_SIZE], char data[MAX_DATA_SIZE], SOCKET s) {
	
	TNSP_ENTRY entry = makeTopicNameEntry(topic, addr, data, s);
	PTNSP_NODE newNode = (PTNSP_NODE)malloc(sizeof(TNSP_NODE));

	newNode->key = entry;
	newNode->next = NULL;

	if (t_head == NULL) {
		t_head = newNode;
	}
	else {
		newNode->next = t_head;
		t_head = newNode;
	}

	newNode->key.TN_SPACE_STATE = STATE_SET;

	numOfRequests++;
	return;
}


TNSP_ENTRY	TopicNameSpaceTable::makeTopicNameEntry(char topic[MAX_CHAR], char addr[ADDRESS_SIZE], char data[MAX_DATA_SIZE], SOCKET s) {
	vector<string> topicVector = topicParser->split(topic);

	TNSP_ENTRY entry;
	memcpy(entry.TN_SPACE_TOPIC, topic, MAX_CHAR);
	memcpy(entry.TN_SPACE_TOKEN, topicVector.at(0).c_str(), MAX_CHAR);
	entry.TN_SPACE_TOTAL_LEVEL = topicVector.size();
	entry.TN_SPACE_CURRENT_LEVEL = 1;
	entry.TN_SPACE_STATE = STATE_NEW;
	memcpy(entry.TN_SPACE_NEXTZONE_ADDR, addr, ADDRESS_SIZE);
	entry.TN_SPACE_ID = numOfRequests;
	entry.TN_SPACE_CURRENT_SOCKET = s;
	memcpy(entry.TN_SPACE_DATA, data, MAX_DATA_SIZE);

	return entry;
}

void		TopicNameSpaceTable::removeEntry(int id) {
	if (numOfRequests != 0) {
		PTNSP_NODE currentNode = t_head;
		PTNSP_NODE beforeNode = NULL;
		
		if (numOfRequests == 1 && currentNode->key.TN_SPACE_ID == id)
			t_head = NULL;
		else {
			while (currentNode->key.TN_SPACE_ID != id || currentNode->next != NULL) {
				beforeNode = currentNode;
				currentNode = currentNode->next;
			}
			if (currentNode->key.TN_SPACE_ID != id)
				return;

			beforeNode->next = currentNode->next;
		}
		free(currentNode);
		numOfRequests--;
	}
}

void		TopicNameSpaceTable::setEntry(int id, char addr[ADDRESS_SIZE], int state) {
	if (numOfRequests != 0) {
		PTNSP_NODE currentNode = t_head;

		while (currentNode->key.TN_SPACE_ID != id || currentNode->next != NULL) {
			currentNode = currentNode->next;
		}

		if (currentNode->key.TN_SPACE_ID == id){
			vector<string> topicVector = topicParser->split(currentNode->key.TN_SPACE_TOPIC);

			memcpy(currentNode->key.TN_SPACE_NEXTZONE_ADDR, addr, sizeof(addr));
			currentNode->key.TN_SPACE_STATE = state;
			if (currentNode->key.TN_SPACE_CURRENT_LEVEL == currentNode->key.TN_SPACE_TOTAL_LEVEL) {
				currentNode->key.TN_SPACE_CURRENT_LEVEL++;
			}
			else {
				memcpy(currentNode->key.TN_SPACE_TOKEN, topicVector.at(currentNode->key.TN_SPACE_CURRENT_LEVEL).c_str(), strlen(topicVector.at(currentNode->key.TN_SPACE_CURRENT_LEVEL).c_str()) + 1);
			}

			if (currentNode->key.TN_SPACE_CURRENT_LEVEL != currentNode->key.TN_SPACE_TOTAL_LEVEL)
				currentNode->key.TN_SPACE_CURRENT_LEVEL++;
		}
	}
}

bool		TopicNameSpaceTable::isEntryExist(int state) {
	if (numOfRequests == 0)
		return false;
	else {
		PTNSP_NODE currentNode = t_head;


		while (currentNode != NULL) {
			if (currentNode->key.TN_SPACE_STATE == state)
				return true;

			currentNode = currentNode->next;
		}

		return false;
	}
}

PTNSP_NODE	TopicNameSpaceTable::getEntry(int state) {
	if (numOfRequests == 0)
		return NULL;
	else {
		PTNSP_NODE currentNode = t_head;

		while (currentNode != NULL) {
			if (currentNode->key.TN_SPACE_STATE == state)
				return currentNode;

			currentNode = currentNode->next;
		}
		
		return NULL;
	}
}

void		TopicNameSpaceTable::testShowAll() {
	if (numOfRequests != 0) {
		PTNSP_NODE currentNode = t_head;

		while (currentNode != NULL) {
			cout << "ID \t : " << currentNode->key.TN_SPACE_ID << endl;
			cout << "TOPIC \t : " << currentNode->key.TN_SPACE_TOPIC << endl;
			cout << "TOKEN \t : " << currentNode->key.TN_SPACE_TOKEN << endl;
			cout << "TL \t : " << currentNode->key.TN_SPACE_TOTAL_LEVEL << endl;
			cout << "CL \t : " << currentNode->key.TN_SPACE_CURRENT_LEVEL << endl;
			cout << "NA \t : " << currentNode->key.TN_SPACE_NEXTZONE_ADDR << endl;
			cout << "STATE \t : " << currentNode->key.TN_SPACE_STATE << endl;
			cout << "DATA \t : " << currentNode->key.TN_SPACE_DATA << endl;

			currentNode = currentNode->next;
		}
	}

}
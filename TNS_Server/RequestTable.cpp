#include "RequestTable.h"
#include <stdio.h>

RequestTable::RequestTable() {
	resetTable();
}

RequestTable::~RequestTable(){}

void RequestTable::resetTable() {
	r_head = NULL;
	r_tail = NULL;
	numOfRequests = 0;
}

bool RequestTable::isRequestExist() {
	return numOfRequests == 0 ? false : true;
}

PR_NODE RequestTable::getLastEntry() {
	//get Tail Entry
	PR_NODE currentNode = NULL;
	PR_NODE beforeNode = NULL;

	if (r_head == NULL) {
		return NULL;
	}
	else {//1개라도 있는 경우에
		currentNode = r_head;

		if (numOfRequests == 1)
			r_head = NULL;
		else {
			while (currentNode->next->next != NULL)
				currentNode = currentNode->next;
			beforeNode = currentNode;
			currentNode = currentNode->next;
			beforeNode->next = NULL;
		}
		numOfRequests--;
	}
	return currentNode;
}

void	RequestTable::addEntry(SOCKET requestSocket, TNSN_ENTRY message) {
	R_ENTRY entry;
	PR_NODE newNode = (PR_NODE)malloc(sizeof(R_NODE));

	entry.REQUEST_SOCEKT = requestSocket;
	memcpy(&(entry.REQUEST_DATA), (char *)&message, sizeof(TNSN_ENTRY));

	newNode->key = entry;
	newNode->next = NULL;

	if (r_head == NULL) {
		r_tail = newNode;
		r_head = newNode;
	}
	else {
		newNode->next = r_head;
		r_head = newNode;
	}

	numOfRequests++;

	return;
}

#include "RequestTable.h"
#include <stdio.h>

RequestTable::RequestTable() {
	resetTable();
}

RequestTable::~RequestTable() {
	DeleteCriticalSection(&cs);
}

void RequestTable::resetTable() {
	r_head = NULL;
	numOfRequests = 0;
	InitializeCriticalSection(&cs);
}

bool RequestTable::isRequestExist() {
	return numOfRequests == 0 ? false : true;
}

PR_NODE RequestTable::getLastEntry() {
	//get Tail Entry
	PR_NODE currentNode = NULL;
	PR_NODE beforeNode = NULL;

	EnterCriticalSection(&cs);

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

	LeaveCriticalSection(&cs);

	return currentNode;
}

void	RequestTable::addEntry(IN_ADDR ip, PDD_NODE message) {
	R_ENTRY entry;
	PR_NODE newNode = (PR_NODE)malloc(sizeof(R_NODE));

	memcpy(&(entry.REQUEST_DATA), (char *)&message, sizeof(PDD_NODE));
	entry.REQUEST_IP = ip;

	newNode->key = entry;
	newNode->next = NULL;


	EnterCriticalSection(&cs);

	if (r_head == NULL) {
		r_head = newNode;
	}
	else {
		newNode->next = r_head;
		r_head = newNode;
	}

	numOfRequests++;

	LeaveCriticalSection(&cs);

	return;
}

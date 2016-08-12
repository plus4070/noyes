#include "TopicNameTable.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
TopicNameTable::TopicNameTable()
{
	resetTable();
}


TopicNameTable::~TopicNameTable()
{
}

void	TopicNameTable::resetTable() {
	TN_head = NULL;
}

bool	TopicNameTable::isEntryExist(TN_ENTRY entry){
	PTN_NODE currentNode = TN_head;

	if (TN_head == NULL)
		return false;
	else //만약 한개라도 있으면 탐색 시작, 다음 Node는 있을수도 있고 없을 수 도 있음
	{
		while (currentNode != NULL) {
			if (memcmp(currentNode->key.TN_TOKEN, entry.TN_TOKEN, sizeof(entry.TN_TOKEN)) &&
				memcmp(currentNode->key.TN_TOPIC, entry.TN_TOPIC, sizeof(entry.TN_TOPIC)) &&
				currentNode->key.TN_LEVEL == entry.TN_LEVEL) {
				return true;
			}
			currentNode = currentNode->next;
		}
	}
	return false;
}

bool	TopicNameTable::addEntry(TN_ENTRY entry){
	PTN_NODE currentNode;
	PTN_NODE newNode = (PTN_NODE)malloc(sizeof(TN_NODE));

	newNode->key  = entry;
	newNode->next = NULL;


	if (isEntryExist(entry))
		return false;

	// Router entry에 추가
	currentNode = TN_head;

	if (TN_head == NULL) {
		newNode->next = TN_head;
		TN_head = newNode;
	}
	else {
		while (currentNode->next != NULL)
			currentNode = currentNode->next;
		newNode->next = currentNode->next;
		currentNode->next = newNode;
	}

	return true;
}

bool	TopicNameTable::deleteEntry(TN_ENTRY entry){

	return false;
}

bool	TopicNameTable::getEntry(TN_ENTRY *p_entry){
	if (!isEntryExist(*p_entry)) {
		return false;
	}
	else {
		PTN_NODE currentNode = TN_head;
		while (currentNode != NULL) {
			if (memcmp(currentNode->key.TN_TOKEN, p_entry->TN_TOKEN, sizeof(p_entry->TN_TOKEN)) &&
				memcmp(currentNode->key.TN_TOPIC, p_entry->TN_TOPIC, sizeof(p_entry->TN_TOPIC)) &&
				currentNode->key.TN_LEVEL == p_entry->TN_LEVEL) {
				memcpy(p_entry->TN_NEXTZONE, currentNode->key.TN_NEXTZONE, ADDRESS_SIZE);
				return true;
			}
			currentNode = currentNode->next;
		}
	}
	return false;
}

bool	TopicNameTable::setEntry(TN_ENTRY *p_entry){
	return false;
}

void	TopicNameTable::testShowAll() {
	PTN_NODE currentNode = TN_head;
	puts("===============================================");
	if (currentNode == NULL) {
		printf("NULL\n");
	}
	else {
		printf("%s\t%s\t%d\t%s\n", currentNode->key.TN_TOPIC, currentNode->key.TN_TOKEN, currentNode->key.TN_LEVEL, currentNode->key.TN_NEXTZONE);
		while (currentNode->next != NULL) {
			currentNode = currentNode->next;
			printf("%s\t%s\t%d\t%s\n", currentNode->key.TN_TOPIC, currentNode->key.TN_TOKEN, currentNode->key.TN_LEVEL, currentNode->key.TN_NEXTZONE);
		}
	}
	puts("===============================================");
}
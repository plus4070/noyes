#include "TopicNameTable.h"

TopicNameTable::TopicNameTable()
{
	ResetTable();
}


TopicNameTable::~TopicNameTable()
{
}

void	TopicNameTable::ResetTable() {
	TN_head = NULL;
}

bool	TopicNameTable::IsEntryExist(TN_ENTRY entry){
	PTN_NODE currentNode = TN_head;

	if (TN_head == NULL)
		return false;
	else //만약 한개라도 있으면 탐색 시작, 다음 Node는 있을수도 있고 없을 수 도 있음
	{
		while (currentNode != NULL) {
			if (strcmp(currentNode->key.TN_TOKEN, entry.TN_TOKEN) == 0 &&
				strcmp(currentNode->key.TN_TOPIC, entry.TN_TOPIC) == 0 &&
				currentNode->key.TN_LEVEL == entry.TN_LEVEL) {
				return true;
			}
			currentNode = currentNode->next;
		}
	}
	return false;
}

bool	TopicNameTable::AddEntry(TN_ENTRY entry){
	PTN_NODE currentNode;
	PTN_NODE newNode = (PTN_NODE)malloc(sizeof(TN_NODE));

	newNode->key  = entry;
	newNode->next = NULL;


	if (IsEntryExist(entry))
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

bool	TopicNameTable::DeleteEntry(TN_ENTRY entry){

	return false;
}

bool	TopicNameTable::GetEntry(TN_ENTRY *p_entry){
	if (!IsEntryExist(*p_entry)) {
		return false;
	}
	else {
		PTN_NODE currentNode = TN_head;
		while (currentNode != NULL) {
			if (strcmp(currentNode->key.TN_TOKEN, p_entry->TN_TOKEN) == 0 &&
				strcmp(currentNode->key.TN_TOPIC, p_entry->TN_TOPIC) == 0 &&
				currentNode->key.TN_LEVEL == p_entry->TN_LEVEL){
				memcpy(p_entry->TN_NEXTZONE, currentNode->key.TN_NEXTZONE, ADDRESS_SIZE);
				return true;
			}
			currentNode = currentNode->next;
		}
	}
	return false;
}

bool	TopicNameTable::SetEntry(TN_ENTRY *p_entry){
	return false;
}

void	TopicNameTable::TestShowAll() {
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
vector<string> TopicNameTable::SplitTopic(const string & s) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}
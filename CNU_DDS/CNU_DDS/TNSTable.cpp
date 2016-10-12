#include "TNSTable.h"

TNSTable::TNSTable() {
	ResetTable();
}

TNSTable::~TNSTable()
{
}

void		TNSTable::ResetTable() {
	requestList.clear();
	this->CountOfID = 0;
}

void		TNSTable::AddEntry(char topic[MAX_CHAR], char topictype[MAX_CHAR], char domain[MAX_CHAR], char addr[ADDRESS_SIZE], char paddr[ADDRESS_SIZE], int port,unsigned char data[MAX_DATA_SIZE], unsigned char orderingType, int length, int nodetype, int messagetype) {
	TNSP_ENTRY entry = MakeTopicNameEntry(topic, topictype, domain, addr, paddr, port, data, orderingType, length, nodetype, messagetype);

	entry.TN_SPACE_STATE = STATE_SET;
	requestList.push_back(entry);
	CountOfID++;
	return;
}


TNSP_ENTRY	TNSTable::MakeTopicNameEntry(char topic[MAX_CHAR], char topictype[MAX_CHAR], char domain[MAX_CHAR], char addr[ADDRESS_SIZE], char paddr[ADDRESS_SIZE], int port, unsigned char data[MAX_DATA_SIZE], unsigned char orderingType, int length, int nodetype, int messagetype) {
	vector<string> topicVector = SplitTopic(topic);

	TNSP_ENTRY entry;
	memcpy(entry.TN_SPACE_DOMAIN, domain, MAX_CHAR);
	memcpy(entry.TN_SPACE_TOPIC, topic, MAX_CHAR);
	memcpy(entry.TN_SPACE_TOPICTYPE, topictype, MAX_CHAR);
	memcpy(entry.TN_SPACE_TOKEN, topicVector.at(0).c_str(), MAX_CHAR);
	entry.TN_SPACE_PARTICIPANT_PORT = port;
	entry.TN_SPACE_TOTAL_LEVEL = topicVector.size();
	entry.TN_SPACE_CURRENT_LEVEL = 1;
	entry.TN_SPACE_STATE = STATE_NEW;
	memcpy(entry.TN_SPACE_NEXTZONE_ADDR, addr, ADDRESS_SIZE);
	memcpy(entry.TN_SPACE_PARTICIPANT_ADDR, paddr, ADDRESS_SIZE);
	entry.TN_SPACE_ID = CountOfID;
	entry.TN_SPACE_NODETYPE = nodetype;
	memcpy(entry.TN_SPACE_DATA, data, MAX_DATA_SIZE);
	entry.TN_SPACE_LENGTH_OF_DATA = length;
	entry.TN_SPCAE_DATA_ORDERING_TYPE = orderingType;
	entry.TN_SPACE_MESSAGETYPE = messagetype;

	return entry;
}

void		TNSTable::RemoveEntry(int id) {
	if (this->GetNumOfRequests() != 0) {
		for (it = requestList.begin(); it != requestList.end();) {
			if (it->TN_SPACE_ID == id) {
				it = requestList.erase(it);
			}
			else {
				it++;
			}
		}
	}
}

void		TNSTable::SetEntry(int id, char addr[ADDRESS_SIZE], int state) {
	if (this->GetNumOfRequests() != 0) {
		for (it = requestList.begin(); it != requestList.end(); ++it) {
			if (it->TN_SPACE_ID == id) {
				vector<string> topicVector = SplitTopic(it->TN_SPACE_TOPIC);

				memcpy(it->TN_SPACE_NEXTZONE_ADDR, addr, sizeof(addr));
				it->TN_SPACE_STATE = state;
				if (it->TN_SPACE_CURRENT_LEVEL == it->TN_SPACE_TOTAL_LEVEL) {
					it->TN_SPACE_CURRENT_LEVEL++;
				}
				else {
					memcpy(it->TN_SPACE_TOKEN, topicVector.at(it->TN_SPACE_CURRENT_LEVEL).c_str(), strlen(topicVector.at(it->TN_SPACE_CURRENT_LEVEL).c_str()) + 1);
				}

				if (it->TN_SPACE_CURRENT_LEVEL != it->TN_SPACE_TOTAL_LEVEL){
					it->TN_SPACE_CURRENT_LEVEL++;
				}
			}
		}
	}
}

bool		TNSTable::IsEntryExist(int state) {
	if (this->GetNumOfRequests() != 0) {
		for (it = requestList.begin(); it != requestList.end(); ++it) {
			if (it->TN_SPACE_STATE == state) {
				return true;
			}
		}
	}
	return false;
}

PTNSP_ENTRY	TNSTable::GetEntry(int state) {
	if (this->GetNumOfRequests() != 0) {
		int count = 0;
		for (it = requestList.begin(); it != requestList.end(); ++it, ++count) {
			if (it->TN_SPACE_STATE == state) {
				return &(requestList.at(count));
			}
		}
	}
}

void		TNSTable::TestShowAll() {
	if (this->GetNumOfRequests() != 0) {
		
	}
}

vector<string> TNSTable::SplitTopic(const string & s) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

int		TNSTable::GetNumOfRequests() {
	//return this->requestList.size();
	return this->requestList.size();
}
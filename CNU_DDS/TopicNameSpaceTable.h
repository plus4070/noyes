#pragma once
#include "TNS.h"
#include <sstream> 

using namespace std;

#define delim '/'

class TopicNameSpaceTable
{
public:
	int					numOfRequests;
	vector<_TNSP_ENTRY>	requestList;
	PTNSP_NODE			t_head;

	TopicNameSpaceTable();
	~TopicNameSpaceTable();

	void		resetTable();

	void		addEntry(char topic[MAX_CHAR], char domain[MAX_CHAR], char addr[ADDRESS_SIZE], char paddr[ADDRESS_SIZE], int port, char data[MAX_DATA_SIZE], SOCKET s, int nodetype, int messagetype);
	void		removeEntry(int id);
	void		setEntry(int id, char addr[ADDRESS_SIZE], int state);
	TNSP_ENTRY	makeTopicNameEntry(char topic[MAX_CHAR], char domain[MAX_CHAR], char addr[ADDRESS_SIZE], char paddr[ADDRESS_SIZE], int port, char data[MAX_DATA_SIZE], SOCKET s, int nodetype, int messagetype);
	
	bool		isEntryExist(int state);
	PTNSP_NODE	getEntry(int state);

	void		testShowAll();

	vector<string> splitTopic(const string & s);
};


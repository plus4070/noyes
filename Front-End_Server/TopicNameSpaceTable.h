#pragma once
#include "stdafx.h"
#include <vector>
#include <stdlib.h>
#include "TopicParser.h"

using namespace std;

class TopicNameSpaceTable
{
public:
	int					numOfRequests;
	vector<_TNSP_ENTRY>	requestList;
	PTNSP_NODE			t_head;
	TopicParser*		topicParser;

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
};

